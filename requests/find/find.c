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

  char* index_value_content = NULL;
  if (!load_index_value(http_response, stater, &index_value_content,
                        queries.col, queries.key, queries.value)) {
    free_stater(stater);
    return 1;
  }

  JSON_Array* index_value_json_array = NULL;
  if (!parse_to_array(http_response, stater, &index_value_json_array,
                      index_value_content)) {
    free_stater(stater);
    free(index_value_content);
    return 1;
  }
  int index_value_json_array_length =
      json_array_get_count(index_value_json_array);

  for (int i = 0; i < index_value_json_array_length; i++) {
    const char* index_item_value =
        json_array_get_string(index_value_json_array, i);
    if (index_item_value == NULL) {
      http_response->status = 500;
      s_set(&http_response->body,
            "Failed to get index item value from index array");
      free_stater(stater);
      free(index_value_content);
      return 1;
    }

    char* document_content = NULL;
    if (!load_document(http_response, stater, &document_content, queries.col,
                       index_item_value)) {
      free_stater(stater);
      free(index_value_content);
      return 1;
    }

    JSON_Value* document_json_value = NULL;
    if (!parse_to_value(http_response, stater, &document_json_value,
                        document_content, queries.key)) {
      free_stater(stater);
      free(index_value_content);
      free(document_content);
      return 1;
    }

    JSON_Status array_replace_status = json_array_replace_value(
        index_value_json_array, i, document_json_value);
    if (array_replace_status != JSONSuccess) {
      http_response->status = 500;
      s_set(&http_response->body,
            "Failed to replace index item value with document content");
      free_stater(stater);
      free(index_value_content);
      free(document_content);
      return 1;
    }
  }

  http_response->status = 200;
  s_set(&http_response->body,
        json_serialize_to_string(
            json_array_get_wrapping_value(index_value_json_array)));
  free_stater(stater);
  return 0;
}