#include "find.h"

#define MAX_PATH_LENGTH 1024

int handle_request_find(sdb_http_request_t* http_request,
                        sdb_http_response_t* http_response) {
  const char* params[] = {"col", "key", "value"};

  sdb_query_params_t queries =
      validate_and_parse_queries(http_request, params, 3);
  if (queries.invalid != NULL) {
    http_response->status = 400;
    s_set(&http_response->body, queries.invalid);
    return 1;
  }

  // initialize stater
  sdb_stater_t* stater = calloc(1, sizeof(sdb_stater_t));

  char* index_path = NULL;
  stater->error_body = "Failed to derive index path";
  stater->error_status = 500;
  if (!fs_path(http_response, stater, &index_path, 4, "index", queries.col,
               queries.key, queries.value)) {
    free_stater(stater);
    return 1;
  }

  // Check if the file exists
  stater->error_body = "Requested index document does not exist";
  stater->error_status = 404;
  if (!fs_file_access(http_response, stater, index_path, F_OK)) {
    free_stater(stater);
    return 1;
  }

  // Check if the file is readable
  stater->error_body =
      "Requested index document does not have read permissions";
  stater->error_status = 500;
  if (!fs_file_access(http_response, stater, index_path, R_OK)) {
    free_stater(stater);
    return 1;
  }

  // Read file content into string
  char* file_content = NULL;
  stater->error_body = "Failed to load index document";
  stater->error_status = 500;
  if (!fs_file_load(http_response, stater, &file_content, index_path)) {
    free_stater(stater);
    return 1;
  }

  http_response->status = 200;
  s_set(&http_response->body, file_content);
  free_stater(stater);
  return 0;

  // if (contains_periods(queries.id)) {
  //   // remove the before the first dot and the first dot itself
  //   char* json_id = strchr(queries.id, '.') + 1;

  //   // parse the json to get its value
  //   JSON_Value* json = json_parse_string_with_comments(file_content);
  //   JSON_Object* json_object = json_value_get_object(json);
  //   JSON_Value* value = json_object_dotget_value(json_object, json_id);
  //   if (value == NULL) {
  //     http_response->status = 404;
  //     s_compile(&http_response->body, "id not found in document: %s",
  //               queries.id);
  //     return 1;
  //   } else {
  //     http_response->status = 200;
  //     s_set(&http_response->body, json_serialize_to_string(value));
  //     return 0;
  //   }
  // } else {
  //   // get the whole object, but filter out any comments
  //   // from the JSON and minify it (done via parson)
  //   JSON_Value* json = json_parse_string_with_comments(file_content);
  //   if (json == NULL) {
  //     http_response->status = 500;
  //     s_compile(&http_response->body, "Failed to parse JSON");
  //     return 1;
  //   } else {
  //     http_response->status = 200;
  //     s_set(&http_response->body, json_serialize_to_string(json));
  //     return 0;
  //   }
  // }
}