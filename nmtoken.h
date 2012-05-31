#ifndef NMTOKEN_H
#define NMTOKEN_H

class NMToken {
public:
  NMToken(const char * const name);
  ~NMToken();

  const char *ns();
  const char *name();
private:
  char *ns_;
  char *name_;
};

#endif
