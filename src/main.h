#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

#include "crypto.h" // util.h is included in crypto.h
#include "test.h"

/**
 * @brief Displays help information
 *
 * @param name Name of executable
 */
void help_info(char *name);

/**
 * @brief Encrypts or decrypts a file
 *
 * @param v Version of implementation
 * @param key Key (Optional)
 * @param iv Initialization vector (Optional)
 * @param d Decrypt if 1, encrypt if 0
 * @param data Data struct, stores content and size of file
 * @param b Number of iterations for time measurement, 0 if no time measurement
 */
void crypto(int v, uint32_t key[4], uint32_t iv[2], int d,
            struct data_struct *data, int b);

/**
 * @brief Executes the program
 *
 * @param v Version of implementation
 * @param b Number of iterations for time measurement
 * @param key Key (Optional)
 * @param iv Initialization vector (Optional)
 * @param d Decrypt if 1, encrypt if 0
 * @param input_file Input file
 * @param output_file Output file
 */
void execute(int v, int b, uint32_t key[4], uint32_t iv[2], int d,
             char *input_file, char *output_file);

/**
 * @brief Main function
 *
 * @param argc Number of arguments
 * @param argv Arguments
 * @return int: Exit code
 */
int main(int argc, char *argv[]);
