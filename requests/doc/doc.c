#include "doc.h"

#define MAX_PATH_LENGTH 1024

int handle_request_doc(sdb_http_request_t* http_request,
                       sdb_http_response_t* http_response) {
  const char* params[] = {"id", "col"};

  sdb_query_params_t queries =
      validate_and_parse_queries(http_request, params, 2);
  if (queries.invalid != NULL) {
    http_response->status = 400;
    s_set(&http_response->body, queries.invalid);
    return 1;
  }

  sdb_stater_t* stater = calloc(1, sizeof(sdb_stater_t));

  char* db_path = NULL;
  stater->error_body = "Failed to derive path";
  stater->error_status = 500;
  if (!fs_path(http_response, stater, &db_path, 3, "collection", queries.col,
               queries.id)) {
    free_stater(stater);
    return 1;
  }

  // Check if the file exists
  stater->error_body = "Requested document does not exist";
  stater->error_status = 404;
  if (!fs_file_access(http_response, stater, db_path, F_OK)) {
    free_stater(stater);
    return 1;
  }

  // Check if the file is readable
  stater->error_body = "Requested document does not have read permissions";
  stater->error_status = 500;
  if (!fs_file_access(http_response, stater, db_path, R_OK)) {
    free_stater(stater);
    return 1;
  }

  char* file_content = NULL;
  stater->error_body = "Failed to load document";
  stater->error_status = 500;
  if (!fs_file_load(http_response, stater, &file_content, db_path)) {
    free_stater(stater);
    return 1;
  }

  free_stater(stater);

  if (contains_periods(queries.id)) {
    // remove the part before the first dot and the first dot itself
    const char* json_id = strchr(queries.id, '.') + 1;
    char* json_id_decoded = url_decode(json_id);

    // parse the json to get its value
    const JSON_Value* json = json_parse_string_with_comments(file_content);
    const JSON_Object* json_obj = json_value_get_object(json);
    const JSON_Value* value =
        json_object_dotget_value(json_obj, json_id_decoded);
    free(json_id_decoded);
    if (value == NULL) {
      http_response->status = 404;
      s_compile(&http_response->body, "id not found in document: %s",
                queries.id);
      free(file_content);
      return 1;
    } else {
      http_response->status = 200;
      s_set(&http_response->body, json_serialize_to_string(value));
      free(file_content);
      return 0;
    }
  } else {
    // get the whole object, but filter out any comments
    // from the JSON and minify it (done via parson)
    const JSON_Value* json = json_parse_string_with_comments(file_content);
    if (json == NULL) {
      http_response->status = 500;
      s_compile(&http_response->body, "Failed to parse JSON");
      free(file_content);
      return 1;
    } else {
      http_response->status = 200;
      s_set(&http_response->body, json_serialize_to_string(json));
      free(file_content);
      return 0;
    }
  }
}