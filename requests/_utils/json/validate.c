#include "validate.h"

bool validate_json_against_schema(sdb_http_response_t* http_response,
                                  sdb_stater_t* stater,
                                  const char* content_string,
                                  const char* collection) {
  char* schema_path = NULL;
  if (!get_path_schema(http_response, stater, &schema_path, collection)) {
    return false;
  }

  char* schema_file_content = NULL;
  if (!fs_file_load(http_response, stater, &schema_file_content, schema_path)) {
    free(schema_path);
    return false;
  }

  JSON_Value* schema_json_val =
      json_parse_string_with_comments(schema_file_content);
  if (schema_json_val == NULL) {
    http_response->status = 500;
    s_compile(&http_response->body,
              "Unknown schema error (01) - please check the schema: \n%s\n",
              schema_file_content);
    free(schema_path);
    free(schema_file_content);
    return false;
  }

  JSON_Value* content_json_val =
      json_parse_string_with_comments(content_string);
  if (content_json_val == NULL) {
    http_response->status = 500;
    s_compile(&http_response->body,
              "Unknown schema error (02) - please check the schema: \n%s\n",
              schema_file_content);
    free(schema_path);
    free(schema_file_content);
    json_value_free(schema_json_val);
    return false;
  }

  // if schema_json_val has four properties, and content_json_val has five,
  // as long as the four of the five are valid, this still returns success
  if (json_validate(schema_json_val, content_json_val) != JSONSuccess) {
    http_response->status = 400;
    s_compile(&http_response->body, "Invalid schema - Should be:\n%s\n",
              schema_file_content);
    free(schema_path);
    free(schema_file_content);
    json_value_free(schema_json_val);
    json_value_free(content_json_val);
    return false;
  }
  // so we have to go both ways, to ensure the schema is a thurough match
  if (json_validate(content_json_val, schema_json_val) != JSONSuccess) {
    http_response->status = 400;
    s_compile(&http_response->body, "Invalid schema - Should be:\n%s\n",
              schema_file_content);
    free(schema_path);
    free(schema_file_content);
    json_value_free(schema_json_val);
    json_value_free(content_json_val);
    return false;
  }

  return true;
}
