#include "test.h"

void create_file(char *filename) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    fprintf(stderr, "ERROR: Could not open file: %s\n", filename);
    exit(1);
  }
  fclose(file);
}

void delete_file(char *filename) {
  if (remove(filename) != 0) {
    fprintf(stderr, "ERROR: Could not delete file: %s\n", filename);
    exit(1);
  }
}

void write_to_file(char *filename, char *data, int size) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    fprintf(stderr, "ERROR: Could not open file: %s\n", filename);
    exit(1);
  }

  fwrite(data, 1, size, file);
  fclose(file);
}

void read_from_file(char *filename, char *data, int size) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "ERROR: Could not open file: %s\n", filename);
    exit(1);
  }

  size_t fread_res = fread(data, 1, size, file);
  if ((int)fread_res != size) {
    fprintf(stderr, "ERROR: Read was not successful\n");
    exit(0);
  }
  fclose(file);
}

void reset_all_files(char *filenames[], int size) {
  for (int i = 0; i < size; i++) {
    write_to_file(filenames[i], "", 0);
  }
}

void print_test_result(int result) {
  if (result) {
    printf(GREEN "[PASSED]\n\n" RESET "");
  } else {
    printf(RED "[FAILED]\n\n" RESET "");
  }
}

void execute_tests() {
  printf(
      "\nThis is the test script for the implementation of the project, using "
      "standard key, iv, input/output files\n"
      "Time will be measured over 10 000 iterations and the output will be "
      "compared to the expected output\n"
      "By encrypting and then decrypting the input file, the output should "
      "be the same as the initial input\n\n");

  uint32_t key[4] = {0xACDF12D9, 0xE9FCBC8A, 0x8B0DFFEC, 0x7B0E1F3C};
  uint32_t iv[2] = {0xF0E1D2C3, 0xB4A5F6E7};

  char *input = malloc(60000);
  char *decrypted = malloc(60000);
  if (input == NULL || decrypted == NULL) {
    fprintf(stderr, "ERROR: Memory allocation failed\n");
    exit(1);
  }

  create_file("input");
  create_file("encrypted");
  create_file("decrypted");
  char *filenames[3] = {"input", "encrypted", "decrypted"};
  int amount_of_files = 3;

  // test only xtea
  printf("[1.0] Testing: XTEA\n");
  uint32_t v[2] = {0x11223344, 0x55667788};
  xtea_encrypt_block(v, key);
  xtea_decrypt_block(v, key);
  print_test_result(v[0] == 0x11223344 && v[1] == 0x55667788);

  // alphabet
  write_to_file("input", "abcdefghijklmnopqrstuvwxyz", 26);
  printf("[2.1] Testing: v0 with Alphabet\n");

  execute(0, 0, key, iv, 0, "input", "encrypted");
  execute(0, 0, key, iv, 1, "encrypted", "decrypted");

  read_from_file("input", input, 26);
  read_from_file("decrypted", decrypted, 26);
  print_test_result(!memcmp(input, decrypted, 26));

  // length 1 to 100
  printf("[2.2] Testing: v0 with Input Files with lengths 1 to 100\n");
  int res = 1;
  for (int i = 1; i < 100; i++) {
    reset_all_files(filenames, amount_of_files);

    for (int j = 0; j < i; j++) {
      input[j] = 'a';
    }
    write_to_file("input", input, i);

    execute(0, 0, key, iv, 0, "input", "encrypted");
    execute(0, 0, key, iv, 1, "encrypted", "decrypted");

    read_from_file("input", input, i);
    read_from_file("decrypted", decrypted, i);
    if (memcmp(input, decrypted, i)) {
      res = 0;
      break;
    }
  }
  print_test_result(res);

  // test special
  reset_all_files(filenames, amount_of_files);
  char *special = "\0\n\0\n\t\t\t\0\0\n\n\0\0\0\t\t\0\n\0\0\0\0\0\t\0\0";
  write_to_file("input", special, 27);
  printf("[2.3] Testing: v0 with Special Input\n");

  execute(0, 0, key, iv, 0, "input", "encrypted");
  execute(0, 0, key, iv, 1, "encrypted", "decrypted");

  read_from_file("input", input, 27);
  read_from_file("decrypted", decrypted, 27);
  print_test_result(!memcmp(input, decrypted, 27));

  // lorem
  reset_all_files(filenames, amount_of_files);
  write_to_file(
      "input",
      "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam "
      "nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam",
      131);
  printf("[3.1] Testing: v1 with Lorem Ipsum\n");

  execute(1, 0, key, iv, 0, "input", "encrypted");
  execute(1, 0, key, iv, 1, "encrypted", "decrypted");

  read_from_file("input", input, 131);
  read_from_file("decrypted", decrypted, 131);
  print_test_result(!memcmp(input, decrypted, 131));

  // arabic lorem
  reset_all_files(filenames, amount_of_files);
  write_to_file("input",
                "الألم في حد ذاته مهم للغاية ، سيتم إنشاء الإليتر الساعد ، لكن "
                "قطره غير ممدود مثل الكدح والألم ",
                170);
  printf("[3.2] Testing: v1 with Arabic Lorem Ipsum\n");

  execute(1, 0, key, iv, 0, "input", "encrypted");
  execute(1, 0, key, iv, 1, "encrypted", "decrypted");

  read_from_file("input", input, 170);
  read_from_file("decrypted", decrypted, 170);
  print_test_result(!memcmp(input, decrypted, 170));

  // long lorem
  reset_all_files(filenames, amount_of_files);
  write_to_file(
      "input",
      "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam "
      "nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, "
      "sed diam voluptua. At vero eos et accusam et justo duo dolores et ea "
      "rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem "
      "ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing "
      "elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna "
      "aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo "
      "dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus "
      "est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur "
      "sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et "
      "dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam "
      "et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea "
      "takimata sanctus est Lorem ipsum dolor sit amet.   Duis autem vel eum "
      "iriure dolor in hendrerit in vulputate velit esse molestie consequat, "
      "vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et "
      "iusto odio dignissim qui blandit praesent luptatum zzril delenit augue "
      "duis dolore te feugait nulla facilisi. Lorem ipsum dolor sit amet, "
      "consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt "
      "ut laoreet dolore magna aliquam erat volutpat.   Ut wisi enim ad minim "
      "veniam, quis nostrud exerci tation ullamcorper suscipit lobortis nisl "
      "ut aliquip ex ea commodo consequat. Duis autem vel eum iriure dolor in "
      "hendrerit in vulputate velit esse molestie consequat, vel illum dolore "
      "eu feugiat nulla facilisis at vero eros et accumsan et iusto odio "
      "dignissim qui blandit praesent luptatum zzril delenit augue duis dolore "
      "te feugait nulla facilisi.   Nam liber tempor cum soluta nobis eleifend "
      "option congue nihil imperdiet doming id quod mazim placerat facer "
      "possim assum. Lorem ipsum dolor sit amet, consectetuer adipiscing elit, "
      "sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam "
      "erat volutpat. Ut wisi enim ad minim veniam, quis nostrud exerci tation "
      "ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo consequat.  "
      " Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse "
      "molestie consequat, vel illum dolore eu feugiat nulla facilisis.   At "
      "vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd "
      "gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. "
      "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam "
      "nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, "
      "sed diam voluptua. At vero eos et accusam et justo duo dolores et ea "
      "rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem "
      "ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing "
      "elitr, At accusam aliquyam diam diam dolore dolores duo eirmod eos "
      "erat, et nonumy sed tempor et et invidunt justo labore Stet clita ea et "
      "gubergren, kasd magna no rebum. sanctus sea sed takimata ut vero "
      "voluptua. est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, "
      "consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut "
      "labore et dolore magna aliquyam erat.   Consetetur sadipscing elitr, "
      "sed diam nonumy eirmod tempor invidunt ut labore et dolore magna "
      "aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo "
      "dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus "
      "est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur "
      "sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et "
      "dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam "
      "et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea "
      "takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit "
      "amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor "
      "invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At "
      "vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd "
      "gubergren, no sea takimata sanctus.   Lorem ipsum dolor sit amet, "
      "consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut "
      "labore et dolore magna aliquyam erat, sed diamEND.",
      4095);
  printf("[3.3] Testing: v1 with Long Lorem Ipsum\n");

  execute(1, 0, key, iv, 0, "input", "encrypted");
  execute(1, 0, key, iv, 1, "encrypted", "decrypted");

  read_from_file("input", input, 4095);
  read_from_file("decrypted", decrypted, 4095);
  print_test_result(!memcmp(input, decrypted, 4095));

  reset_all_files(filenames, amount_of_files);
  printf("[3.4] Testing: v1 with 32MiB File\n");
  execute(1, 0, key, iv, 0, "test_input_32mib", "encrypted");
  execute(1, 0, key, iv, 1, "encrypted", "decrypted");

  char *input_32mib = (char *)malloc(32 * 1024 * 1024);
  char *decrypted_32mib = (char *)malloc(32 * 1024 * 1024);
  read_from_file("test_input_32mib", input_32mib, 32 * 1024 * 1024);
  read_from_file("decrypted", decrypted_32mib, 32 * 1024 * 1024);
  print_test_result(!memcmp(input_32mib, decrypted_32mib, 32 * 1024 * 1024));
  free(input_32mib);
  free(decrypted_32mib);

  printf("\n");

  // cleanup
  free(input);
  free(decrypted);
  delete_file("input");
  delete_file("encrypted");
  delete_file("decrypted");
}
