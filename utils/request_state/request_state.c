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
    stater->success_body = NULL;
    stater->error_body = NULL;
    stater->success_status = 0;
    stater->error_status = 0;
    free(stater);
  }
}

void apply_state(sdb_http_response_t *http_response, sdb_stater_t *stater,
                 char *format_string, ...) {
  if (http_response != NULL) {  // attaching to the http_response struct
    if (stater->error_status != 0) {
      http_response->status = stater->error_status;
    }
    if (stater->error_body != NULL) {
      va_list args;
      va_start(args, format_string);
      s_compile(&http_response->body, format_string, args);
      s_prepend(&http_response->body, stater->error_body);
      va_end(args);
    }
    if (stater->success_status != 0) {
      http_response->status = stater->success_status;
    }
    if (stater->success_body != NULL) {
      va_list args;
      va_start(args, format_string);
      s_compile(&http_response->body, format_string, args);
      s_prepend(&http_response->body, stater->error_body);
      va_end(args);
    }
  } else {  // printing to the console
    if (stater->error_status != 0) {
      printf("Error: %d\n", stater->error_status);
    }
    if (stater->error_body != NULL) {
      va_list args;
      va_start(args, format_string);
      printf("%s", stater->error_body);
      printf(format_string, args);
      printf("\n");
      va_end(args);
    }
    if (stater->success_status != 0) {
      printf("Success: %d\n", stater->success_status);
    }
    if (stater->success_body != NULL) {
      va_list args;
      va_start(args, format_string);
      printf("%s", stater->success_body);
      printf(format_string, args);
      printf("\n");
      va_end(args);
    }
  }
  free_stater(stater);
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