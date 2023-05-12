#pragma once
#include <stdint.h>
#include <stdio.h>

#include "main.h"

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"

/**
 * @brief Create a file, handles errors: exits if error
 *
 * @param filename name of file
 */
void create_file(char *filename);

/**
 * @brief Delete a file, handles errors: exits if error
 *
 * @param filename name of file
 */
void delete_file(char *filename);

/**
 * @brief Writes data to file, handles errors: exits if error
 *
 * @param filename name of file
 * @param data data to write
 * @param size size of data
 */
void write_to_file(char *filename, char *data, int size);

/**
 * @brief Reads data from file, handles errors: exits if error
 *
 * @param filename name of file
 * @param data data to write
 * @param size size of data
 */
void read_from_file(char *filename, char *data, int size);

/**
 * @brief Resets all files, handles errors: exits if error
 */
void reset_all_files();

/**
 * @brief Prints test result
 */
void print_test_result(int result);

/**
 * @brief Testing function executing all tests
 */
void execute_tests();