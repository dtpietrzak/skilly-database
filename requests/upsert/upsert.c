#include "upsert.h"

#define MAX_PATH_LENGTH 1024

int handle_request_upsert(sdb_http_request_t* http_request,
                          sdb_http_response_t* http_response) {
  if (contains_invalid_chars(http_request->body.value,
                             INVALID_CHARS_REQUEST_BODY)) {
    http_response->status = 400;
    s_set(&http_response->body, "Invalid characters in request body");
    return 1;
  }

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
  stater->error_body = "Failed to derive collection path";
  stater->error_status = 500;
  if (!fs_path(http_response, stater, &db_path, 3, "collection", queries.col,
               queries.id)) {
    free_stater(stater);
    return 1;
  }

  char* schema_path = NULL;
  stater->error_body = "Failed to derive schema path";
  stater->error_status = 500;
  if (!fs_path(http_response, stater, &schema_path, 3, "collection",
               queries.col, queries.id)) {
    free_stater(stater);
    return 1;
  }

  free_stater(stater);

  JSON_Value* request_body_json_value =
      get_json_value(http_response, http_request->body.value,
                     "Failed to parse request JSON data");
  if (request_body_json_value == NULL) return 1;

  char* schema_file_content =
      get_schema_file_content(http_response, schema_path, db_path, queries);
  if (schema_file_content == NULL) return 1;

  if (contains_periods(queries.id)) {
    // HANDLE DOT NOTATION UPSERT
    int compile_status = compile_dot_notation_change(
        http_response, http_request, db_path, queries, schema_file_content,
        request_body_json_value);
    if (compile_status == 1) return 1;
  } else {
    // HANDLE ENTIRE DOC UPSERT
    int compile_status =
        compile_doc_change(http_response, http_request, db_path, queries,
                           schema_file_content, request_body_json_value);
    if (compile_status == 1) return 1;
  }

  json_value_free(request_body_json_value);

  int save_status = save_string(http_response, http_request->body.value,
                                db_path, "\"Document inserted successfully\"",
                                "\"Document updated successfully\"",
                                "Failed to save data to document");
  if (save_status == -1) return 1;
  return 0;
}
