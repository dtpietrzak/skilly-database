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

  char* document_path = NULL;
  if (!get_path_document(http_response, stater, &document_path, queries.col,
                         queries.id)) {
    free_stater(stater);
    return 1;
  }

  char* schema_path = NULL;
  if (!get_path_schema(http_response, stater, &schema_path, queries.col)) {
    free(document_path);
    free_stater(stater);
    return 1;
  }

  char* index_meta_path = NULL;
  if (!get_path_index_meta(http_response, stater, &index_meta_path,
                           queries.col)) {
    free(document_path);
    free(schema_path);
    free_stater(stater);
    return 1;
  }

  // get all the index files

  // load index meta file
  // char* index_meta_file_content = NULL;
  // stater->error_body = "Failed to load index meta file";
  // stater->error_status = 500;
  // if (!fs_file_load(http_response, stater, &index_meta_file_content,
  //                   index_meta_path)) {
  //   free_stater(stater);
  //   return 1;
  // }

  // const JSON_Value* index_meta_json_value =
  //     json_parse_string_with_comments(index_meta_file_content);
  // if (index_meta_json_value == NULL) {
  //   http_response->status = 500;
  //   s_set(&http_response->body, "Failed to parse index meta file");
  //   free_stater(stater);
  //   return 1;
  // }

  // const JSON_Array* index_meta_json_array =
  //     json_value_get_array(index_meta_json_value);
  // if (index_meta_json_array == NULL) {
  //   http_response->status = 500;
  //   s_set(&http_response->body, "Failed to parse index meta file");
  //   free_stater(stater);
  //   return 1;
  // }

  // for (size_t i = 0; i < json_array_get_count(index_meta_json_array); i++) {
  //   const char* index_property_name =
  //       json_array_get_string(index_meta_json_array, i);
  //   if (index_property_name == NULL) {
  //     http_response->status = 500;
  //     s_set(&http_response->body, "Failed to parse index meta file");
  //     free_stater(stater);
  //     return 1;
  //   }

  //   printf("%s", index_property_name);
  // }

  // char* index_files_path = NULL;
  // stater->error_body = "Failed to derive index files path";
  // stater->error_status = 500;
  // if (!fs_path(http_response, stater, &index_files_path, 3, "index",
  //              queries.col)) {
  //   free_stater(stater);
  //   return 1;
  // }

  if (contains_periods(queries.id)) {
    // HANDLE DOT NOTATION UPSERT
    char* string_to_save = NULL;
    if (!compile_dot_notation_change(http_response, stater, queries,
                                     &string_to_save,
                                     http_request->body.value)) {
      free(document_path);
      free(schema_path);
      free_stater(stater);
      return 1;
    }

    free_stater(stater);

    int save_status = save_string(http_response, string_to_save, document_path,
                                  "\"Document inserted successfully\"",
                                  "\"Document updated successfully\"",
                                  "Failed to save data to document");
    free(document_path);
    free(schema_path);
    free(string_to_save);
    if (save_status == -1) return 1;
    return 0;
  } else {
    // HANDLE ENTIRE DOC UPSERT
    if (!validate_json_against_schema(http_response, stater,
                                      http_request->body.value, queries.col)) {
      free(document_path);
      free(schema_path);
      free_stater(stater);
      return 1;
    }

    free_stater(stater);

    int save_status = save_string(
        http_response, http_request->body.value, document_path,
        "\"Document inserted successfully\"",
        "\"Document updated successfully\"", "Failed to save data to document");
    free(document_path);
    free(schema_path);
    if (save_status == -1) return 1;
    return 0;
  }
}
