#include "util.h"
#include <string.h>

int read_text_file(char *fileName, struct data_struct *data) {
  FILE *f = fopen(fileName, "r");
  if (f == NULL) {
    fprintf(stderr, "ERROR: Could not open file: %s\n", fileName);
    return 1;
  }

  fseek(f, 0, SEEK_END);
  data->size = ftell(f);

  if (data->size == 0) {
    fprintf(stderr, "ERROR: File is empty\n");
    return 1;
  }

  rewind(f);

  // allocate memory for text and padding
  data->txt = malloc(sizeof(uint64_t) * (data->size / 8 + 1));

  if (data->txt == NULL) {
    fprintf(stderr, "ERROR: Memory allocation failed\n");
    return 1;
  }

  size_t fread_res = fread(data->txt, sizeof(uint32_t), data->size, f);

  if (fread_res != data->size / 4) {
    fprintf(stderr, "ERROR: Read was not successful\n");
    return 1;
  }

  fclose(f);

  return 0;
}

int write_text_file(char *fileName, struct data_struct *data) {
  FILE *f = fopen(fileName, "w");

  if (f == NULL) {
    fprintf(stderr, "ERROR: Could not open file: %s\n", fileName);
    return 1;
  }

  // char pointer to write to file in correct byte order
  char *tmp = malloc(data->size * sizeof(char));
  if (tmp == NULL) {
    fprintf(stderr, "ERROR: Memory allocation failed\n");
    return 1;
  }

  memcpy(tmp, data->txt, data->size);
  fwrite(tmp, sizeof(char), data->size, f);

  free(data->txt);
  free(tmp);

  fclose(f);
  return 0;
}
