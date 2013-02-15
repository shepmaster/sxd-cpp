#include <iostream>
#include <string.h>

#include "nmtoken.h"

NMToken::NMToken(const char * const name)
{
  const char * colon;

  colon = strchr(name, ':');

  if (colon) {
    char *modified = strdup(name);
    modified[colon - name] = '\0';
    ns_ = modified;
    name_ = strdup(colon+1);
  } else {
    ns_ = nullptr;
    name_ = strdup(name);
  }
}

NMToken::~NMToken()
{
  free(ns_);
  free(name_);
}

const char *
NMToken::name()
{
  return name_;
}

const char *
NMToken::ns()
{
  return ns_;
}
