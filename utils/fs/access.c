#include "access.h"

bool fs_file_access(sdb_http_response_t* http_response, sdb_stater_t* stater,
                    const char* file_path, int mode) {
  uv_loop_t* loop = uv_default_loop();

  // acess
  uv_fs_t access_req;
  int result = uv_fs_access(loop, &access_req, file_path, mode, NULL);
  bool file_readable = result == 0;
  if (!file_readable) {
    int error_code = (int)access_req.result;
    apply_state(http_response, stater,
                " - Failed to access document(%d - %s): %s", error_code,
                uv_strerror(error_code), file_path);
  }

  free_stater(stater);
  uv_fs_req_cleanup(&access_req);
  return file_readable;
}