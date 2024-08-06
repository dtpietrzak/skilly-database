#include <errno.h>
#include <sdb_utils.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../_utils/fs/load.h"
#include "../_utils/json/parse.h"

int handle_request_doc(sdb_http_request_t* http_request,
                       sdb_http_response_t* http_response);