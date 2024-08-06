#ifndef REQUESTS_UTILS_PATH_H
#define REQUESTS_UTILS_PATH_H

#include <sdb_utils.h>

bool get_path_collection(sdb_http_response_t* http_response,
                         sdb_stater_t* stater, char** path_ptr,
                         const char* col_name);

bool get_path_document(sdb_http_response_t* http_response, sdb_stater_t* stater,
                       char** path_ptr, const char* col_name, const char* id);

bool get_path_schema(sdb_http_response_t* http_response, sdb_stater_t* stater,
                     char** path_ptr, const char* col_name);

bool get_path_index_meta(sdb_http_response_t* http_response,
                         sdb_stater_t* stater, char** path_ptr,
                         const char* col_name);

bool get_path_index_key(sdb_http_response_t* http_response,
                        sdb_stater_t* stater, char** path_ptr,
                        const char* col_name, const char* document_key);

bool get_path_index_value(sdb_http_response_t* http_response,
                          sdb_stater_t* stater, char** path_ptr,
                          const char* col_name, const char* document_key,
                          const char* document_value);

#endif  // REQUESTS_UTILS_PATH_H