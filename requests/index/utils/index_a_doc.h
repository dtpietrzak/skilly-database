#include <sdb_utils.h>

#include "../index_distributor/index_distributor.h"
#include "index_utils.h"

int index_a_doc(sdb_http_response_t* http_response, const char* col_name,
                const char* file_name, const char* request_meta_string);