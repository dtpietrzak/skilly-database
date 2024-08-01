#ifndef REQUESTS_INDEX_META_HANDLERS_H
#define REQUESTS_INDEX_META_HANDLERS_H

#include <sdb_utils.h>
#include "index_utils.h"

typedef struct {
  JSON_Value* json_value;
  int removed_items_count;
  char** removed_items;
} JSON_Value_With_Removed_Items;

const JSON_Value* upserted_meta(sdb_http_response_t* http_response,
                                JSON_Array_With_Count request_array_with_count,
                                char* existing_meta);

void free_removed_items(char** removed_items, int count);

const JSON_Value_With_Removed_Items* removed_meta(
    sdb_http_response_t* http_response, JSON_Array_With_Count request_array_with_count,
    char* existing_meta);

#endif  // REQUESTS_INDEX_META_HANDLERS_H