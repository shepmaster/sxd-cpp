#ifndef TO_STREAM_H
#define TO_STREAM_H

#include <iostream>

/**
 * Provides a base implementation of writing an object to a stream.
 */
class ToStream {
public:
  /**
   * Writes this object to a stream.
   *
   * This is programmer-friendly output, and is not intended for
   * end-user consumption.
   */
  virtual std::ostream &to_stream(std::ostream &os) const;
};

std::ostream &operator<<(std::ostream &os, const ToStream &obj);

#endif
