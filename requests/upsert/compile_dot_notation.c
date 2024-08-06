#include "compile_dot_notation.h"

bool compile_dot_notation_change(sdb_http_response_t* http_response,
                                 sdb_stater_t* stater,
                                 sdb_query_params_t queries,
                                 char** string_to_save, char* new_value) {
  // remove the before the first dot and the first dot itself
  const char* json_id = strchr(queries.id, '.') + 1;

  char* schema_content = NULL;
  if (!load_schema(http_response, stater, &schema_content, queries.col)) {
    return false;
  }

  JSON_Value* request_json_val = json_parse_string_with_comments(new_value);
  if (request_json_val == NULL) {
    http_response->status = 400;
    s_compile(&http_response->body, "Invalid request JSON: %s", new_value);
    free(schema_content);
    return false;
  }

  JSON_Value* schema_json_val = json_parse_string_with_comments(schema_content);
  if (schema_json_val == NULL) {
    http_response->status = 500;
    s_compile(&http_response->body,
              "Unknown schema error (01) - please check the schema: \n%s\n",
              schema_content);
    free(schema_content);
    json_value_free(request_json_val);
    return false;
  }
  const JSON_Object* schema_object = json_value_get_object(schema_json_val);
  if (schema_object == NULL) {
    http_response->status = 500;
    s_compile(&http_response->body,
              "Unknown schema error (02) - please check the schema: \n%s\n",
              schema_content);
    free(schema_content);
    json_value_free(request_json_val);
    json_value_free(schema_json_val);
    return false;
  }

  const JSON_Value* schema_json_dot_value =
      json_object_dotget_value(schema_object, json_id);
  if (schema_json_dot_value == NULL) {
    http_response->status = 404;
    s_compile(&http_response->body, "Property not found in schema: %s",
              json_id);
    free(schema_content);
    json_value_free(request_json_val);
    json_value_free(schema_json_val);
    return false;
  }

  // need to get this validation working???
  if (json_value_get_type(schema_json_dot_value) !=
      json_value_get_type(request_json_val)) {
    http_response->status = 400;
    s_compile(&http_response->body, "Invalid schema - Should be:\n%s\n",
              schema_content);
    free(schema_content);
    json_value_free(request_json_val);
    json_value_free(schema_json_val);
    return false;
  }

  char* document_content = NULL;
  if (!load_document(http_response, stater, &document_content, queries.col,
                     queries.id)) {
    free(schema_content);
    json_value_free(request_json_val);
    json_value_free(schema_json_val);
    return false;
  }

  JSON_Value* document_json = json_parse_string_with_comments(document_content);
  if (document_json == NULL) {
    http_response->status = 500;
    s_compile(&http_response->body, "Failed to parse JSON");
    free(schema_content);
    json_value_free(request_json_val);
    json_value_free(schema_json_val);
    return false;
  }
  JSON_Object* document_json_object = json_value_get_object(document_json);
  if (document_json_object == NULL) {
    http_response->status = 500;
    s_compile(&http_response->body, "Failed to parse JSON");
    free(schema_content);
    json_value_free(request_json_val);
    json_value_free(schema_json_val);
    return false;
  }

  JSON_Value* document_value =
      json_object_dotget_value(document_json_object, json_id);
  if (document_value == NULL) {
    http_response->status = 404;
    s_compile(&http_response->body, "id not found in document: %s", queries.id);
    free(schema_content);
    json_value_free(request_json_val);
    json_value_free(schema_json_val);
    return false;
  }

  JSON_Status dot_set_status =
      json_object_dotset_value(document_json_object, json_id, request_json_val);
  if (dot_set_status != JSONSuccess) {
    http_response->status = 500;
    s_compile(&http_response->body, "Failed to set dot notation");
    free(schema_content);
    json_value_free(request_json_val);
    json_value_free(schema_json_val);
    return false;
  }

  *string_to_save = json_serialize_to_string_pretty(document_json);
  free(schema_content);
  json_value_free(request_json_val);
  json_value_free(schema_json_val);
  return true;
}
