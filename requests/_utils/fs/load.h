#ifndef REQUESTS_UTILS_FS_LOAD_H
#define REQUESTS_UTILS_FS_LOAD_H

#include <sdb_utils.h>

#include "../path/get.h"

bool load_document(sdb_http_response_t* http_response, sdb_stater_t* stater,
                   char** out_buffer, const char* col_name, const char* id);

bool load_index_value(sdb_http_response_t* http_response, sdb_stater_t* stater,
                      char** out_buffer, const char* col_name,
                      const char* document_key, const char* document_value);

#endif  // REQUESTS_UTILS_FS_LOAD_H