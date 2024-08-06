#include <errno.h>
#include <sdb_utils.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../_utils/path/get.h"

int handle_request_delete(sdb_http_request_t* http_request,
                          sdb_http_response_t* http_response);