#include <errno.h>
#include <sdb_utils.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../_utils/fs/load.h"

bool compile_dot_notation_change(sdb_http_response_t* http_response,
                                 sdb_stater_t* stater,
                                 sdb_query_params_t queries,
                                 char** string_to_save, char* new_value);