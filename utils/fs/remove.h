#ifndef FS_REMOVE_H
#define FS_REMOVE_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <uv.h>

#include "../request_state/request_state.h"

bool fs_file_remove(sdb_http_response_t* http_response,
                    const sdb_stater_t* stater, const char* file_path);

#endif  // FS_REMOVE_H