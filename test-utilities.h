#ifndef TEST_UTILITIES_H
#define TEST_UTILITIES_H

#include "output.h"
#include <glib.h>

class StringOutput : public Output {
public:
  StringOutput();
  ~StringOutput();
  void output(const char *format, ...);
  const char *string();
private:
  GString *string_;
};

#endif
