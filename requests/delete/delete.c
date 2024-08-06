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

  char* document_path = NULL;
  if (!get_path_document(http_response, stater, &document_path, queries.col,
                         queries.id)) {
    free_stater(stater);
    return 1;
  }

  // Check if the file exists
  stater->error_body = "Requested document does not exist";
  stater->error_status = 404;
  if (!fs_file_access(http_response, stater, document_path, F_OK)) {
    free(document_path);
    free_stater(stater);
    return 1;
  }

  if (!fs_file_remove(http_response, stater, document_path)) {
    free(document_path);
    free_stater(stater);
    return 1;
  }

  // TODO
  // we need to remove all references to the document from the index system

  free(document_path);
  free_stater(stater);

  http_response->status = 200;
  s_set(&http_response->body, "Document removed successfully");

  return 0;
}