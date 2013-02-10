#ifndef TO_STREAM_H
#define TO_STREAM_H

#include <iostream>

class ToStream {
public:
  virtual std::ostream &to_stream(std::ostream &os) const;
};

std::ostream &operator<<(std::ostream &os, const ToStream &obj);

#endif
