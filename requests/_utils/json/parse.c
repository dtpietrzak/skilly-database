#include "parse.h"

bool parse_to_value(sdb_http_response_t* http_response, sdb_stater_t* stater,
                    JSON_Value** out_buffer, const char* content,
                    const char* keys) {
  stater->error_body = "JSON value parsing failed";
  stater->error_status = 500;

  if (contains_periods(keys)) {
    // remove the part before the first dot and the first dot itself
    const char* json_key = strchr(keys, '.') + 1;
    char* json_key_decoded = url_decode(json_key);

    // parse the json to get its value
    const JSON_Value* temp_json_val = json_parse_string_with_comments(content);
    if (temp_json_val == NULL) {
      apply_state(http_response, stater, " - Document error (01)");
      out_buffer = NULL;
      free(json_key_decoded);
      return false;
    }
    // get the object as a JSON_Object type
    const JSON_Object* json_obj = json_value_get_object(temp_json_val);
    if (json_obj == NULL) {
      apply_state(http_response, stater, " - Object error (02)");
      out_buffer = NULL;
      free(json_key_decoded);
      return false;
    }
    // get the dot value from the object
    JSON_Value* json_val = json_object_dotget_value(json_obj, json_key_decoded);
    if (json_val == NULL) {
      stater->error_status = 404;
      apply_state(http_response, stater, " - Key not found in document: %s",
                  keys);
      out_buffer = NULL;
      free(json_key_decoded);
      return false;
    }

    free(json_key_decoded);
    *out_buffer = json_val;
    return true;
  } else {
    // get the whole object, but filter out any comments
    // from the JSON and minify it (done via parson)
    JSON_Value* json_val = json_parse_string_with_comments(content);
    if (json_val == NULL) {
      apply_state(http_response, stater, " - Document error (03)");
      out_buffer = NULL;
      return false;
    }

    *out_buffer = json_val;
    return true;
  }
}

bool parse_to_array(sdb_http_response_t* http_response, sdb_stater_t* stater,
                    JSON_Array** out_buffer, const char* content) {
  stater->error_body = "JSON array parsing failed";
  stater->error_status = 500;

  // parse the json to get its value
  const JSON_Value* temp_json_val = json_parse_string_with_comments(content);
  if (temp_json_val == NULL) {
    apply_state(http_response, stater, " - Value error (04)");
    return false;
  }

  *out_buffer = json_value_get_array(temp_json_val);
  if (*out_buffer == NULL) {
    apply_state(http_response, stater, " - Array error (05)");
    return false;
  }
  return true;
}