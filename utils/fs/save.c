#include "save.h"

bool fs_file_save(sdb_http_response_t* http_response, sdb_stater_t* stater,
                  char* buffer_to_save, const char* file_path) {
  uv_loop_t* loop = uv_default_loop();

  // open
  uv_fs_t open_req;
  int fd = uv_fs_open(loop, &open_req, file_path, 0, UV_FS_O_CREAT, NULL);
  if (fd < 0) {
    int error_code = (int)open_req.result;
    apply_state(http_response, stater,
                " - Failed to open document(%d - %s): %s", error_code,
                uv_strerror(error_code), file_path);
    uv_fs_req_cleanup(&open_req);
    return false;
  }

  // write
  uv_fs_t write_req;
  uv_buf_t write_buf = uv_buf_init(buffer_to_save, strlen(buffer_to_save));
  int write_result = uv_fs_write(loop, &write_req, fd, &write_buf, 1, 0, NULL);
  if (write_result < 0) {
    int error_code = (int)write_req.result;
    apply_state(http_response, stater,
                " - Failed to write document(%d - %s): %s", error_code,
                uv_strerror(error_code), file_path);
    uv_fs_close(loop, &open_req, fd, NULL);
    uv_fs_req_cleanup(&open_req);
    uv_fs_req_cleanup(&write_req);
    return false;
  }

  // cleanup
  uv_fs_close(loop, &open_req, fd, NULL);
  uv_fs_req_cleanup(&open_req);
  uv_fs_req_cleanup(&write_req);
  return true;
}