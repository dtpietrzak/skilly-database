#include "request_state.h"

void free_http_request(sdb_http_request_t *http_request) {
  memset(&http_request->headers, 0, sizeof(http_request->headers));
  memset(&http_request->method, 0, sizeof(http_request->method));
  memset(&http_request->path, 0, sizeof(http_request->path));
  memset(&http_request->queries, 0, sizeof(http_request->queries));
  s_free(&http_request->body);
}

void free_http_response(sdb_http_response_t *http_response) {
  http_response->status = 0;
  s_free(&http_response->body);
}

void free_stater(sdb_stater_t *stater) {
  if (stater != NULL) {
    free(stater);
  }
}

void apply_state(sdb_http_response_t *http_response, const sdb_stater_t *stater,
                 char *format_string, ...) {
  char buffer[1024];
  va_list args;
  va_start(args, format_string);
  vsnprintf(buffer, sizeof(buffer), format_string, args);
  va_end(args);

  if (http_response != NULL) {  // attaching to the http_response struct
    if (stater->error_status != 0) {
      http_response->status = stater->error_status;
    }
    if (stater->error_body != NULL) {
      s_set(&http_response->body, stater->error_body);
      s_append(&http_response->body, buffer);
    }
    if (stater->success_status != 0) {
      http_response->status = stater->success_status;
    }
    if (stater->success_body != NULL) {
      s_set(&http_response->body, stater->error_body);
      s_append(&http_response->body, buffer);
    }
  } else {  // printing to the console
    if (stater->error_status != 0) {
      printf("Error: %d\n", stater->error_status);
    }
    if (stater->error_body != NULL) {
      printf("%s%s\n", stater->error_body, buffer);
    }
    if (stater->success_status != 0) {
      printf("Success: %d\n", stater->success_status);
    }
    if (stater->success_body != NULL) {
      printf("%s%s\n", stater->success_body, buffer);
    }
  }
}

void debug_request_string(const char *request_str) {
  if (DEBUG_REQUEST_STRING) {
    printf("Request String: %s\n", request_str);
  }
}

void debug_response_string(const char *response_str) {
  if (DEBUG_RESPONSE_STRING) {
    printf("Response String:\n%s\n\n\n", response_str);
  }
}