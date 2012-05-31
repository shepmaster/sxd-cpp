#ifndef OUTPUT_H
#define OUTPUT_H

class Output {
public:
  virtual void output(const char * format, ...) = 0;
};

#endif
