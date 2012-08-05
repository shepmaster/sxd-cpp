#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "document.h"

class FileOutput : public Output {
public:
  FileOutput(FILE *file) :
    file(file)
  {
  }

  void output(const char *format, ...) {
    va_list params;

    va_start(params, format);
    vfprintf(file, format, params);
    va_end(params);
  }
private:
  FILE *file;
};

int
main(int argc, char *argv[])
{
  FILE *file;
  char *data;
  size_t alloc_sz = 1024;
  size_t read_sz;
  Document *doc;
  GError *error = NULL;

  file = fopen("/tmp/a.xml", "rb");
  if (! file) {
    perror("Could not open file");
    return EXIT_FAILURE;
  }

  data = (char *)malloc(alloc_sz);
  read_sz = fread(data, 1, alloc_sz, file);
  data[read_sz] = '\0';

  doc = Document::parse(data, &error);
  free(data);
  if (error) {
    fprintf(stderr, "Could not parse: %s\n", error->message);
    g_error_free(error);
    return EXIT_FAILURE;
  }

  {
    FileOutput stdout_output(stdout);
    Element *root = doc->root();
    root->output(stdout_output);
  }

  delete doc;

  return EXIT_SUCCESS;
}

