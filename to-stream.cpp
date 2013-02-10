#include "to-stream.h"

std::ostream &
ToStream::to_stream(std::ostream& os) const {
  return os << typeid(this).name() << " <" << this << ">";
}

std::ostream &
operator<<(std::ostream &os, const ToStream &obj)
{
  return obj.to_stream(os);
}
