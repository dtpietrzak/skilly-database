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

  char* document_content = NULL;
  if (!load_document(http_response, stater, &document_content, queries.col,
                     queries.id)) {
    free_stater(stater);
    return 1;
  }

  JSON_Value* document_json_value = NULL;
  if (!parse_to_value(http_response, stater, &document_json_value,
                      document_content, queries.id)) {
    free_stater(stater);
    free(document_content);
    return 1;
  }

  free_stater(stater);
  free(document_content);

  http_response->status = 200;
  s_set(&http_response->body, json_serialize_to_string(document_json_value));
  json_value_free(document_json_value);
  return 0;
}