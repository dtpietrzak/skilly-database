#include "get.h"

bool get_collection_path(sdb_http_response_t* http_response,
                         sdb_stater_t* stater, char** path_ptr,
                         const char* col_name) {
  stater->error_body = "Failed to derive collection path";
  stater->error_status = 500;
  if (!fs_path(http_response, stater, path_ptr, 2, "collection", col_name)) {
    return false;
  }
  return true;
}

bool get_document_path(sdb_http_response_t* http_response, sdb_stater_t* stater,
                       char** path_ptr, const char* col_name, const char* id) {
  stater->error_body = "Failed to derive document path";
  stater->error_status = 500;
  if (!fs_path(http_response, stater, path_ptr, 3, "collection", col_name,
               id)) {
    return false;
  }
  return true;
}

bool get_schema_path(sdb_http_response_t* http_response, sdb_stater_t* stater,
                     char** path_ptr, const char* col_name) {
  stater->error_body = "Failed to derive schema path";
  stater->error_status = 500;
  if (!fs_path(http_response, stater, path_ptr, 2, "schema", col_name)) {
    return false;
  }
  return true;
}

bool get_index_meta_path(sdb_http_response_t* http_response,
                         sdb_stater_t* stater, char** path_ptr,
                         const char* col_name) {
  stater->error_body = "Failed to derive index meta path";
  stater->error_status = 500;
  if (!fs_path(http_response, stater, path_ptr, 3, "index", col_name,
               "_meta")) {
    return false;
  }
  return true;
}

bool get_index_key_path(sdb_http_response_t* http_response,
                        sdb_stater_t* stater, char** path_ptr,
                        const char* col_name, const char* document_key) {
  stater->error_body = "Failed to derive index key path";
  stater->error_status = 500;
  if (!fs_path(http_response, stater, path_ptr, 3, "index", col_name,
               document_key)) {
    return false;
  }
  return true;
}

bool get_index_value_path(sdb_http_response_t* http_response,
                          sdb_stater_t* stater, char** path_ptr,
                          const char* col_name, const char* document_key,
                          const char* document_value) {
  stater->error_body = "Failed to derive index value path";
  stater->error_status = 500;
  if (!fs_path(http_response, stater, path_ptr, 4, "index", col_name,
               document_key, document_value)) {
    return false;
  }
  return true;
}