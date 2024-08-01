#include "path.h"

#include "../../global.c"

#ifdef _WIN32
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif

bool fs_path(sdb_http_response_t* http_response, sdb_stater_t* stater,
             char** path_ptr, int num_args, ...) {
  // Start with the base path length
  size_t length = strlen(global_setting_path_for_data_ptr) +
                  5;  // "data/" + null terminator
  va_list args;
  va_start(args, num_args);

  // Calculate the total length required
  for (int i = 0; i < num_args; i++) {
    const char* arg = va_arg(args, const char*);
    if (arg == NULL) {
      apply_state(http_response, stater,
                  " - Null pointer encountered for argument %d", i);
      va_end(args);
      return false;
    }
    int arg_length = strlen(arg);
    if (arg_length == 0) {
      apply_state(http_response, stater,
                  " - Empty string encountered for argument %d", i);
      va_end(args);
      return false;
    }
    length += arg_length + 1;  // For the arg length and a slash
  }

  // Allocate memory for the resulting path
  char* relative_path = malloc(length);
  if (relative_path == NULL) {
    apply_state(http_response, stater, " - Failed to allocate memory");
    va_end(args);
    return false;
  }

  // Construct the path
  strcpy(relative_path, global_setting_path_for_data_ptr);
  strcat(relative_path, "data");
  strcat(relative_path, PATH_SEPARATOR);
  va_start(args, num_args);  // Re-initialize args to iterate again
  for (int i = 0; i < num_args; i++) {
    const char* arg = va_arg(args, const char*);
    strcat(relative_path, arg);
    if (i < num_args - 1) {
      strcat(relative_path, PATH_SEPARATOR);
    }
  }

  va_end(args);

  // remove everything after a . in the last / segment of relative_path
  const char* last_slash = strrchr(relative_path, PATH_SEPARATOR[0]);
  if (last_slash != NULL) {
    char* dot = strchr(last_slash, '.');
    if (dot != NULL) {
      *dot = '\0';
    }
  }

  free_stater(stater);
  *path_ptr = relative_path;
  return true;
}