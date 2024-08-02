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

  // Try to delete the file
  if (remove(db_path) == 0) {
    http_response->status = 200;
    s_compile(&http_response->body, "\"Removed %s successfully\"", db_path);
    free_stater(stater);
    return 0;
  } else {
    http_response->status = 500;
    s_compile(&http_response->body, "Failed to remove %s", db_path);
    free_stater(stater);
    return 1;
  }
}