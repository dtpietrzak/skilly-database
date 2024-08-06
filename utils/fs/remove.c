#include "remove.h"

bool fs_file_remove(sdb_http_response_t* http_response,
                    const sdb_stater_t* stater, const char* file_path) {
  uv_loop_t* loop = uv_default_loop();

  uv_fs_t unlink_req;
  int result = uv_fs_unlink(loop, &unlink_req, file_path, NULL);
  if (result < 0) {
    int error_code = (int)unlink_req.result;
    apply_state(http_response, stater,
                " - Failed to remove document(%d - %s): %s", error_code,
                uv_strerror(error_code), file_path);
    uv_fs_req_cleanup(&unlink_req);
    return false;
  }

  uv_fs_req_cleanup(&unlink_req);
  return true;
}