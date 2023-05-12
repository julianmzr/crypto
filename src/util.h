#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct data_struct {
  uint32_t *txt; // pointer to first text block
  uint32_t size; // points to last char of text
};

/**
 * @brief Reads a file
 *
 * @param fileName Name of file
 * @param data Data struct, stores content and size of file

 * @return int Exit code
 */
int read_text_file(char *fileName, struct data_struct *data);

/**
 * @brief Writes a file
 *
 * @param fileName Name of file
 * @param data Data struct, stores content and size of file
 * @return int Exit code
 */
int write_text_file(char *fileName, struct data_struct *data);
