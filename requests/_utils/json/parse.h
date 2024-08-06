#ifndef REQUESTS_UTILS_JSON_LOAD_H
#define REQUESTS_UTILS_JSON_LOAD_H

#include <sdb_utils.h>

bool parse_to_value(sdb_http_response_t* http_response, sdb_stater_t* stater,
                    JSON_Value** out_buffer, const char* content,
                    const char* keys);

bool parse_to_array(sdb_http_response_t* http_response, sdb_stater_t* stater,
                    JSON_Array** out_buffer, const char* content);

#endif  // REQUESTS_UTILS_JSON_LOAD_H