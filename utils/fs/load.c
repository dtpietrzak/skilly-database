#include "load.h"

bool fs_file_load(sdb_http_response_t* http_response, sdb_stater_t* stater,
                  char** out_buffer, const char* file_path) {
  uv_loop_t* loop = uv_default_loop();

  // open
  uv_fs_t open_req;
  int fd = uv_fs_open(loop, &open_req, file_path, 0, UV_FS_O_RDONLY, NULL);
  if (fd < 0) {
    int error_code = (int)open_req.result;
    apply_state(http_response, stater,
                " - Failed to open document(%d - %s): %s", error_code,
                uv_strerror(error_code), file_path);
    uv_fs_req_cleanup(&open_req);
    *out_buffer = NULL;
    return false;
  }

  // fstat
  uv_fs_t fstat_req;
  int stat_result = uv_fs_fstat(loop, &fstat_req, fd, NULL);
  if (stat_result < 0) {
    int error_code = (int)fstat_req.result;
    apply_state(http_response, stater,
                " - Failed to get document status(%d - %s): %s", error_code,
                strerror(error_code), file_path);
    uv_fs_close(loop, &open_req, fd, NULL);
    uv_fs_req_cleanup(&open_req);
    uv_fs_req_cleanup(&fstat_req);
    *out_buffer = NULL;
    return false;
  }

  // buffer
  size_t file_size = ((uv_stat_t*)fstat_req.ptr)->st_size;
  char* buffer = (char*)malloc((file_size + 1) * sizeof(char));
  if (!buffer) {
    apply_state(http_response, stater,
                " - Failed to allocate buffer for document read: %s",
                file_path);
    uv_fs_close(loop, &open_req, fd, NULL);
    uv_fs_req_cleanup(&open_req);
    uv_fs_req_cleanup(&fstat_req);
    *out_buffer = NULL;
    return false;
  }
  buffer[file_size] = '\0';

  // read
  uv_fs_t read_req;
  const uv_buf_t iov = uv_buf_init(buffer, file_size);
  int result = uv_fs_read(loop, &read_req, fd, &iov, 1, 0, NULL);
  if (result < 0) {
    int error_code = (int)read_req.result;
    apply_state(http_response, stater,
                " - Failed to read document(%d - %s): %s", error_code,
                uv_strerror(error_code), file_path);
    uv_fs_close(loop, &open_req, fd, NULL);
    uv_fs_req_cleanup(&open_req);
    uv_fs_req_cleanup(&fstat_req);
    uv_fs_req_cleanup(&read_req);
    free(buffer);
    *out_buffer = NULL;
    return false;
  }

  // cleanup
  uv_fs_close(loop, &open_req, fd, NULL);
  uv_fs_req_cleanup(&open_req);
  uv_fs_req_cleanup(&fstat_req);
  uv_fs_req_cleanup(&read_req);
  *out_buffer = buffer;
  return true;
}