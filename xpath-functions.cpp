#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "xpath-internal.h"

static void
not_implemented(void)
{
  abort();
}

/* 4.1 - Node Set Functions */

xpath_result_t
xpath_fn_last(xpath_evaluation_context_t *context, GArray *parameters_unused)
{
  xpath_result_t result;

  result.type = XPATH_RESULT_TYPE_NUMERIC;
  result.value.numeric = nodeset_count(context->nodeset);

  return result;
}

xpath_result_t
xpath_fn_position(xpath_evaluation_context_t *context, GArray *parameters_unused)
{
  xpath_result_t result;
  int i;

  result.type = XPATH_RESULT_TYPE_NUMERIC;
  result.value.numeric = 0;

  for (i = 0; i < nodeset_count(context->nodeset); i++) {
    if (context->node == nodeset_get(context->nodeset, i)) {
      /* Position is one-indexed, not zero */
      result.value.numeric = i + 1;
      break;
    }
  }

  return result;
}

xpath_result_t
xpath_fn_count(xpath_evaluation_context_t *context_unused, GArray *parameters)
{
  xpath_result_t *nodeset;
  xpath_result_t result;

  if (parameters->len != 1) {
    not_implemented();
  }

  nodeset = &g_array_index(parameters, xpath_result_t, 0);
  if (nodeset->type != XPATH_RESULT_TYPE_NODESET) {
    not_implemented();
  }

  result.type = XPATH_RESULT_TYPE_NUMERIC;
  result.value.numeric = nodeset_count(nodeset->value.nodeset);

  return result;
}

/* 4.2 - String Functions */

xpath_result_t
xpath_fn_concat(xpath_evaluation_context_t *context_unused, GArray *parameters)
{
  GString *str;
  xpath_result_t *param;
  xpath_result_t result;
  int i;

  if (parameters->len < 2) {
    not_implemented();
  }

  param = &g_array_index(parameters, xpath_result_t, 0);
  if (param->type != XPATH_RESULT_TYPE_STRING) {
    not_implemented();
  }
  str = g_string_new(param->value.string);

  for (i = 1; i < parameters->len; i++) {
    param = &g_array_index(parameters, xpath_result_t, i);
    if (param->type != XPATH_RESULT_TYPE_STRING) {
      not_implemented();
    }
    g_string_append(str, param->value.string);
  }

  result.type = XPATH_RESULT_TYPE_STRING;
  result.value.string = g_string_free(str, FALSE);
  return result;
}

xpath_result_t
xpath_fn_starts_with(xpath_evaluation_context_t *context_unused, GArray *parameters)
{
  xpath_result_t *string;
  xpath_result_t *prefix;
  xpath_result_t result;

  if (parameters->len != 2) {
    not_implemented();
  }

  string = &g_array_index(parameters, xpath_result_t, 0);
  prefix = &g_array_index(parameters, xpath_result_t, 1);

  if (string->type != XPATH_RESULT_TYPE_STRING) {
    not_implemented();
  }

  if (prefix->type != XPATH_RESULT_TYPE_STRING) {
    not_implemented();
  }

  result.type = XPATH_RESULT_TYPE_BOOLEAN;
  result.value.boolean = g_str_has_prefix(string->value.string, prefix->value.string);
  return result;
}

typedef struct {
  xpath_result_t *haystack;
  xpath_result_t *needle;
} find_substring_t;

static const char *
find_substring(find_substring_t *find, GArray *parameters)
{
  if (parameters->len != 2) {
    not_implemented();
  }

  find->haystack = &g_array_index(parameters, xpath_result_t, 0);
  find->needle = &g_array_index(parameters, xpath_result_t, 1);

  if (find->haystack->type != XPATH_RESULT_TYPE_STRING) {
    not_implemented();
  }

  if (find->needle->type != XPATH_RESULT_TYPE_STRING) {
    not_implemented();
  }

  return g_strstr_len(find->haystack->value.string, -1, find->needle->value.string);
}

xpath_result_t
xpath_fn_contains(xpath_evaluation_context_t *context_unused, GArray *parameters)
{
  find_substring_t find;
  xpath_result_t result;

  result.type = XPATH_RESULT_TYPE_BOOLEAN;
  result.value.boolean = (find_substring(&find, parameters) != NULL);
  return result;
}

xpath_result_t
xpath_fn_substring_before(xpath_evaluation_context_t *context_unused, GArray *parameters)
{
  find_substring_t find;
  const char *location;
  xpath_result_t result;

  result.type = XPATH_RESULT_TYPE_STRING;

  location = find_substring(&find, parameters);
  if (location) {
    int len = location - find.haystack->value.string;
    result.value.string = g_strndup(find.haystack->value.string, len);
  } else {
    result.value.string = g_strdup("");
  }

  return result;
}

xpath_result_t
xpath_fn_substring_after(xpath_evaluation_context_t *context_unused, GArray *parameters)
{
  find_substring_t find;
  const char *location;
  xpath_result_t result;

  result.type = XPATH_RESULT_TYPE_STRING;

  location = find_substring(&find, parameters);
  if (location) {
    const char *start = location + strlen(find.needle->value.string);
    result.value.string = g_strdup(start);
  } else {
    result.value.string = g_strdup("");
  }

  return result;
}

static char *
utf8_strndup(const char *str, gsize n)
{
  const char *end_byte;
  int len;
  char *result;

  end_byte = g_utf8_offset_to_pointer(str, n);
  len = end_byte - str;

  result = (char *)malloc(len + 1);
  memcpy(result, str, len);
  result[len] = '\0';

  return result;
}

xpath_result_t
xpath_fn_substring(xpath_evaluation_context_t *context_unused, GArray *parameters)
{
  xpath_result_t *string;
  xpath_result_t *start;
  xpath_result_t *end = NULL;
  int start_offset;
  const char *start_byte;
  xpath_result_t result;

  if (parameters->len < 2) {
    not_implemented();
  }

  if (parameters->len > 3) {
    not_implemented();
  }

  string = &g_array_index(parameters, xpath_result_t, 0);
  start = &g_array_index(parameters, xpath_result_t, 1);
  if (parameters->len == 3) {
    end = &g_array_index(parameters, xpath_result_t, 2);
  }

  if (string->type != XPATH_RESULT_TYPE_STRING) {
    not_implemented();
  }

  if (start->type != XPATH_RESULT_TYPE_NUMERIC) {
    not_implemented();
  }

  if (end && end->type != XPATH_RESULT_TYPE_NUMERIC) {
    not_implemented();
  }

  result.type = XPATH_RESULT_TYPE_STRING;

  /* Start is one-indexed, not zero */
  start_offset = start->value.numeric - 1;
  start_byte = g_utf8_offset_to_pointer(string->value.string, start_offset);
  if (end) {
    result.value.string = utf8_strndup(start_byte, end->value.numeric);
  } else {
    result.value.string = g_strdup(start_byte);
  }

  return result;
}

/* 4.3 - Boolean Functions */

xpath_result_t
xpath_fn_boolean(xpath_evaluation_context_t *context_unused, GArray *parameters)
{
  xpath_result_t *value;
  xpath_result_t result;

  if (parameters->len != 1) {
    not_implemented();
  }

  value = &g_array_index(parameters, xpath_result_t, 0);

  result.type = XPATH_RESULT_TYPE_BOOLEAN;
  switch (value->type) {
  case XPATH_RESULT_TYPE_BOOLEAN:
    result.value.boolean = value->value.boolean;
    break;
  case XPATH_RESULT_TYPE_NUMERIC:
    result.value.boolean = (value->value.numeric != 0 &&
			    isfinite(value->value.numeric));
    break;
  case XPATH_RESULT_TYPE_STRING:
    result.value.boolean = (value->value.string[0] != '\0');
    break;
  case XPATH_RESULT_TYPE_NODESET:
    result.value.boolean = (nodeset_count(value->value.nodeset) > 0);
    break;
  }

  return result;
}

xpath_result_t
xpath_fn_not(xpath_evaluation_context_t *context_unused, GArray *parameters)
{
  xpath_result_t *value;
  xpath_result_t result;

  if (parameters->len != 1) {
    not_implemented();
  }

  value = &g_array_index(parameters, xpath_result_t, 0);
  if (value->type != XPATH_RESULT_TYPE_BOOLEAN) {
    not_implemented();
  }

  result.type = XPATH_RESULT_TYPE_BOOLEAN;
  result.value.boolean = ! value->value.boolean;

  return result;
}

xpath_result_t
xpath_fn_true(xpath_evaluation_context_t *context_unused, GArray *parameters_unused)
{
  xpath_result_t result;
  result.type = XPATH_RESULT_TYPE_BOOLEAN;
  result.value.boolean = TRUE;
  return result;
}

xpath_result_t
xpath_fn_false(xpath_evaluation_context_t *context_unused, GArray *parameters_unused)
{
  xpath_result_t result;
  result.type = XPATH_RESULT_TYPE_BOOLEAN;
  result.value.boolean = FALSE;
  return result;
}

/* 4.4 - Number Functions */

xpath_result_t
xpath_fn_floor(xpath_evaluation_context_t *context_unused, GArray *parameters)
{
  xpath_result_t *value;
  xpath_result_t result;

  if (parameters->len != 1) {
    not_implemented();
  }

  value = &g_array_index(parameters, xpath_result_t, 0);
  if (value->type != XPATH_RESULT_TYPE_NUMERIC) {
    not_implemented();
  }

  result.type = XPATH_RESULT_TYPE_NUMERIC;
  result.value.numeric = floor(value->value.numeric);

  return result;
}

xpath_result_t
xpath_fn_ceiling(xpath_evaluation_context_t *context_unused, GArray *parameters)
{
  xpath_result_t *value;
  xpath_result_t result;

  if (parameters->len != 1) {
    not_implemented();
  }

  value = &g_array_index(parameters, xpath_result_t, 0);
  if (value->type != XPATH_RESULT_TYPE_NUMERIC) {
    not_implemented();
  }

  result.type = XPATH_RESULT_TYPE_NUMERIC;
  result.value.numeric = ceil(value->value.numeric);

  return result;
}

xpath_result_t
xpath_fn_round(xpath_evaluation_context_t *context_unused, GArray *parameters)
{
  xpath_result_t *value;
  xpath_result_t result;

  if (parameters->len != 1) {
    not_implemented();
  }

  value = &g_array_index(parameters, xpath_result_t, 0);
  if (value->type != XPATH_RESULT_TYPE_NUMERIC) {
    not_implemented();
  }

  result.type = XPATH_RESULT_TYPE_NUMERIC;
  if (isfinite(value->value.numeric)) {
    /* The standard says we need to round towards positive infinity,
     * but that is not the default. It seems as if we should be able to
     * use fesetround(), but I can't determine the thread safety of that
     * function.
     */
    double truncated = (int) value->value.numeric;

    if (value->value.numeric < 0) {
      if (value->value.numeric - truncated >= -0.5) {
	result.value.numeric = truncated;
      } else {
	result.value.numeric = truncated - 1;
      }
    } else {
      result.value.numeric = round(value->value.numeric);
    }
  } else {
    result.value.numeric = value->value.numeric;
  }

  return result;
}
