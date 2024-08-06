#ifndef REQUEST_UTILS_JSON_VALIDATE_H
#define REQUEST_UTILS_JSON_VALIDATE_H

#include <sdb_utils.h>

#include "../fs/load.h"
#include "../path/get.h"

bool validate_json_against_schema(sdb_http_response_t* http_response,
                                  sdb_stater_t* stater,
                                  const char* content_string,
                                  const char* collection);

#endif  // REQUEST_UTILS_JSON_VALIDATE_H