#ifndef REQUESTS_UTILS_PATH_H
#define REQUESTS_UTILS_PATH_H

#include <sdb_utils.h>

bool get_collection_path(sdb_http_response_t* http_response,
                         sdb_stater_t* stater, char** path_ptr,
                         const char* col_name);

bool get_document_path(sdb_http_response_t* http_response, sdb_stater_t* stater,
                       char** path_ptr, const char* col_name, const char* id);

bool get_schema_path(sdb_http_response_t* http_response, sdb_stater_t* stater,
                     char** path_ptr, const char* col_name);

bool get_index_meta_path(sdb_http_response_t* http_response,
                         sdb_stater_t* stater, char** path_ptr,
                         const char* col_name);

bool get_index_key_path(sdb_http_response_t* http_response,
                        sdb_stater_t* stater, char** path_ptr,
                        const char* col_name, const char* document_key);

bool get_index_value_path(sdb_http_response_t* http_response,
                          sdb_stater_t* stater, char** path_ptr,
                          const char* col_name, const char* document_key,
                          const char* document_value);

#endif  // REQUESTS_UTILS_PATH_H