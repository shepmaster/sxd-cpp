#include "test-utilities.h"

StringOutput::StringOutput() :
  string_(g_string_new(NULL))
{}

StringOutput::~StringOutput()
{
  g_string_free(string_, TRUE);
}

void
StringOutput::output(const char *format, ...)
{
  va_list params;

  va_start(params, format);
  g_string_append_vprintf(string_, format, params);
  va_end(params);
}

const char *
StringOutput::string()
{
  return string_->str;
}
