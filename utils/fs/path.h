#ifndef FS_PATH_H
#define FS_PATH_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <uv.h>

#include "../request_state/request_state.h"

bool fs_path(sdb_http_response_t* http_response, sdb_stater_t* stater,
             char** path_ptr, int num_args, ...);

#endif  // FS_PATH_H