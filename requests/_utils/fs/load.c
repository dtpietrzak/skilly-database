#include "load.h"

bool load_document(sdb_http_response_t* http_response, sdb_stater_t* stater,
                   char** out_buffer, const char* col_name, const char* id) {
  char* document_path = NULL;
  if (!get_path_document(http_response, stater, &document_path, col_name, id)) {
    return false;
  }

  // Check if the file exists
  stater->error_body = "Requested document does not exist";
  stater->error_status = 404;
  if (!fs_file_access(http_response, stater, document_path, F_OK)) {
    return false;
  }

  // Check if the file is readable
  stater->error_body = "Requested document does not have read permissions";
  stater->error_status = 500;
  if (!fs_file_access(http_response, stater, document_path, R_OK)) {
    return false;
  }

  stater->error_body = "Failed to load document";
  stater->error_status = 500;
  if (!fs_file_load(http_response, stater, out_buffer, document_path)) {
    return false;
  }

  return true;
}

bool load_index_value(sdb_http_response_t* http_response, sdb_stater_t* stater,
                      char** out_buffer, const char* col_name,
                      const char* document_key, const char* document_value) {
  char* index_value_path = NULL;
  if (!get_path_index_value(http_response, stater, &index_value_path, col_name,
                            document_key, document_value)) {
    return false;
  }

  // Check if the file exists
  stater->error_body = "Requested index value does not exist";
  stater->error_status = 404;
  if (!fs_file_access(http_response, stater, index_value_path, F_OK)) {
    return false;
  }

  // Check if the file is readable
  stater->error_body = "Requested index value does not have read permissions";
  stater->error_status = 500;
  if (!fs_file_access(http_response, stater, index_value_path, R_OK)) {
    return false;
  }

  stater->error_body = "Failed to load index value";
  stater->error_status = 500;
  if (!fs_file_load(http_response, stater, out_buffer, index_value_path)) {
    return false;
  }

  return true;
}

bool load_schema(sdb_http_response_t* http_response, sdb_stater_t* stater,
                 char** out_buffer, const char* col_name) {
  char* schema_path = NULL;
  if (!get_path_schema(http_response, stater, &schema_path, col_name)) {
    return false;
  }

  // Check if the file exists
  stater->error_body = "Requested schema does not exist";
  stater->error_status = 404;
  if (!fs_file_access(http_response, stater, schema_path, F_OK)) {
    return false;
  }

  // Check if the file is readable
  stater->error_body = "Requested schema does not have read permissions";
  stater->error_status = 500;
  if (!fs_file_access(http_response, stater, schema_path, R_OK)) {
    return false;
  }

  stater->error_body = "Failed to load schema";
  stater->error_status = 500;
  if (!fs_file_load(http_response, stater, out_buffer, schema_path)) {
    return false;
  }

  return true;
}