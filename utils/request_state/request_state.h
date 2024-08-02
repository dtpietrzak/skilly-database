#ifndef ERROR_H
#define ERROR_H

#include <stdbool.h>
#include <string.h>
#include <uv.h>

#include "../string/sstring.h"

#define DEBUG_REQUEST_STRING false
#define DEBUG_RESPONSE_STRING false

#define MAX_REQ_METHOD_SIZE 8
#define MAX_REQ_PATH_SIZE 1024
#define MAX_REQ_QUERIES 24
#define MAX_REQ_QUERY_SIZE 512
#define MAX_REQ_HEADERS 24
#define MAX_REQ_HEADER_SIZE 512
#define MAX_REQ_BODY_SIZE 39927

#define MAX_RES_SIZE 65535
#define MAX_REQ_SIZE 65535

#define REQUIRED_AUTH_LENGTH 32

typedef struct {
  char method[MAX_REQ_METHOD_SIZE];
  char path[MAX_REQ_PATH_SIZE];
  char queries[MAX_REQ_QUERIES][2][MAX_REQ_QUERY_SIZE];
  int num_queries;
  char headers[MAX_REQ_HEADERS][MAX_REQ_HEADER_SIZE];
  int num_headers;
  SString body;
} sdb_http_request_t;

typedef struct {
  uint16_t status;
  SString body;
} sdb_http_response_t;

typedef struct {
  const char* success_body;
  int success_status;
  const char* error_body;
  int error_status;
} sdb_stater_t;

typedef struct {
  char* invalid;
  char* id;
  char* key;
  char* value;
  char* query;
  char* col;
  char* limit;
  char* offset;
  char* sort;
} sdb_query_params_t;

void free_http_request(sdb_http_request_t* http_request);
void free_http_response(sdb_http_response_t* http_response);
void free_stater(sdb_stater_t* stater);

void apply_state(sdb_http_response_t* http_response, sdb_stater_t* stater,
                 char* format_string, ...);

void debug_request_string(const char* request_str);
void debug_response_string(const char* respoonse_str);

#endif  // ERROR_H