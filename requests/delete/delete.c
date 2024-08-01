#include "delete.h"

int handle_request_delete(sdb_http_request_t* http_request,
                          sdb_http_response_t* http_response) {
  const char* params[] = {"id", "col"};

  sdb_query_params_t queries =
      validate_and_parse_queries(http_request, params, 2);
  if (queries.invalid != NULL) {
    http_response->status = 400;
    s_set(&http_response->body, queries.invalid);
    return 1;
  }

  char* db_path = NULL;
  sdb_stater_t* stater_db_path = calloc(1, sizeof(sdb_stater_t));
  stater_db_path->error_body = "Failed to derive path";
  stater_db_path->error_status = 500;
  if (!fs_path(http_response, stater_db_path, &db_path, 3, "collection",
               queries.col, queries.id)) {
    return 1;
  }

  // Check if the file exists
  sdb_stater_t* stater_doc_exists = calloc(1, sizeof(sdb_stater_t));
  stater_doc_exists->error_body = "Requested document does not exist";
  stater_doc_exists->error_status = 404;
  if (!fs_file_access(http_response, stater_doc_exists, db_path, F_OK)) {
    return 1;
  }

  // Try to delete the file
  if (remove(db_path) == 0) {
    http_response->status = 200;
    s_compile(&http_response->body, "\"Removed %s successfully\"", db_path);
    return 0;
  } else {
    http_response->status = 500;
    s_compile(&http_response->body, "Failed to remove %s", db_path);
    return 1;
  }
}