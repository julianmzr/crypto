#include "main.h"

void help_info(char *name) {
  printf("Usage: %s [OPTION] ... [FILE] ... \n", name);
  printf(
      "Performs cryptographic encryption and decryption using XTEA (Extended "
      "Tiny Encryption Algorithm).\n\n"
      "With FILE as input file, additional files will be ignored.\n\n"
      "Options:\n"
      "  -V NUM\t implementation version that should be used; default: main "
      "implementation: -V 0\n"
      "  -B (NUM)\t if set, outputs run time of implementation with NUM "
      "repetitions\n\t\t NUM is optional, (default: 10_000)\n"
      "  -k NUM\t selects NUM as key, should be 128 bit\n"
      "  -i NUM\t selects NUM as initialization vector, should be 64 bit\n"
      "  -d\t\t if set, decrypt, otherwise encrypt\n"
      "  -o FILE\t set output file as FILE\n"
      "  -h, --help\t display this help text and exit\n");
  printf("EXAMPLES\n"
         "  %s -V 0 -B 100000 -o output input\n"
         "  %s -V 1 -i 1234567890ABCDEF input\n"
         "  %s -V 1 -B -k 1234567890ABCDEF0123456789ABCDEF input\n"
         "  %s -V 0 -B 10000 -k 1234567890ABCDEF0123456789ABCDEF -i "
         "1234567890ABCDEF -d -o output input\n"
         "  %s\n",
         name, name, name, name, name);
}

void crypto(int v, uint32_t key[4], uint32_t iv[2], int d,
            struct data_struct *data, int b) {

  void (*encrypt_v[4])(struct data_struct * data, const uint32_t key[4],
                       uint32_t iv[2]) = {encrypt_v0, encrypt_v1};
  void (*decrypt_v[4])(struct data_struct * data, const uint32_t key[4],
                       uint32_t iv[2]) = {decrypt_v0, decrypt_v1};

  if (b) {
    clock_t start, end;
    double sum = 0.0;

    for (int j = 0; j < b; j++) {

      if (d) {
        start = clock();
        (*decrypt_v[v])(data, key, iv);
        end = clock();

        remove_padding(data);
        add_padding(data);

        (*encrypt_v[v])(data, key, iv);

      } else {
        add_padding(data);

        start = clock();
        (*encrypt_v[v])(data, key, iv);
        end = clock();

        (*decrypt_v[v])(data, key, iv);
        remove_padding(data);
      }

      sum += (double)(end - start) / CLOCKS_PER_SEC;
    }

    printf("\nDurchschnittliche Laufzeit: %.3f Mikroskunden über %i "
           "Durchläufe.\n\n",
           sum / b * 1e6, b);
  }

  if (d) {
    (*decrypt_v[v])(data, key, iv);
    remove_padding(data);
  } else {
    add_padding(data);
    (*encrypt_v[v])(data, key, iv);
  }
}

void execute(int v, int b, uint32_t key[4], uint32_t iv[2], int d,
             char *input_file, char *output_file) {

  struct data_struct *data = malloc(sizeof(struct data_struct));
  if (data == NULL) {
    fprintf(stderr, "ERROR: Memory allocation failed\n");
    exit(1);
  }

  if (read_text_file(input_file, data)) {
    free(data);
    exit(1);
  }

  crypto(v, key, iv, d, data, b);

  if (write_text_file(output_file, data)) {
    free(data);
    exit(1);
  }
  free(data);
}

int main(int argc, char *argv[]) {

  long v = 0, b = 0, d = 0;
  __int128_t key = 0;
  int64_t iv = 0;
  char *input_file = NULL;
  char *output_file = NULL;
  int option;
  char *endptr;
  errno = 0;
  while ((option = getopt(argc, argv, ":V:B:k:i:do:ht")) != -1) {
    switch (option) {
    case 'V':
      v = strtol(optarg, &endptr, 10);
      if (endptr == optarg || *endptr != '\0') {
        fprintf(stderr, "%s contains non-number characters\n", optarg);
        return EXIT_FAILURE;
      } else if (errno == ERANGE) {
        fprintf(stderr, "%s is not a number in range of long\n", optarg);
        return EXIT_FAILURE;
      }

      if (v < 0 || v > 1) {
        printf("Wrong implementation option (-v)\n");
        printf("\tAvailable versions: V%i, V%i\n", 0, 1);
        return -1;
      }
      break;
    case 'B':
      if (optarg == NULL) {
        break;
      }
      b = strtol(optarg, &endptr, 10);

      if (endptr == optarg || *endptr != '\0') {
        fprintf(stderr, "%s contains non-number characters\n", optarg);
        return EXIT_FAILURE;
      } else if (errno == ERANGE) {
        fprintf(stderr, "%s is not a number in range of long\n", optarg);
        return EXIT_FAILURE;
      }

      if (b < 0) {
        fprintf(stderr, "Wrong runtime measurement option (-B)\n"
                        "\tMust be greater than 0\n");
        return -1;
      }
      break;
    case 'k':
      key = strtoll(optarg, &endptr, 10);

      if (endptr == optarg || *endptr != '\0') {
        fprintf(stderr, "%s contains non-number characters\n", optarg);
        return EXIT_FAILURE;
      } else if (errno == ERANGE) {
        fprintf(stderr, "%s is not a number in range of long\n", optarg);
        return EXIT_FAILURE;
      }
      break;
    case 'i':
      iv = strtol(optarg, &endptr, 10);

      if (endptr == optarg || *endptr != '\0') {
        fprintf(stderr, "%s contains non-number characters\n", optarg);
        return EXIT_FAILURE;
      } else if (errno == ERANGE) {
        fprintf(stderr, "%s is not a number in range of long long\n", optarg);
        return EXIT_FAILURE;
      }
      break;
    case 'd':
      d = 1;
      break;
    case 'o':
      output_file = optarg;
      break;
    case 'h':
      help_info(*argv);
      return 0;
    case 't':
      execute_tests();
      return 0;
    case ':':
      if (optopt == 'B') {
        b = 10000; // default value for B
      } else {
        fprintf(stderr, "Option -%c is missing a value!\n", optopt);
      }
      break;
    case '?':;
      const char *unknown = argv[optind];
      if (!strcmp(unknown, "--help")) {
        help_info(*argv);
        return 0;
      } else {
        fprintf(stderr,
                "%s: missing file operand '%c'\n"
                "Try '%s --help' for more information.\n",
                argv[0], optopt, argv[0]);
        return -1;
      }
    }
  }
  while (optind < argc) {
    if (input_file == NULL) {
      input_file = argv[optind++];
    }
  }
  if (input_file == NULL)
    input_file = "input";

  if (output_file == NULL)
    output_file = "output";

  uint32_t tmp_k[4] = {0};
  uint32_t tmp_i[2] = {0};
  for (int i = 0; i < 4; i++) {
    tmp_k[i] = key;
    key >>= 32;
  }
  for (int i = 0; i < 2; i++) {
    tmp_i[i] = iv;
    iv >>= 32;
  }

  if (tmp_k[0] == 0 && tmp_k[1] == 0 && tmp_k[2] == 0 && tmp_k[3] == 0) {
    tmp_k[0] = 0xACDF12D9;
    tmp_k[1] = 0xE9FCBC8A;
    tmp_k[2] = 0x8B0DFFEC;
    tmp_k[3] = 0x7B0E1F3C;
  }

  if (tmp_i[0] == 0 && tmp_i[1] == 0) {
    tmp_i[0] = 0xF0E1D2C3;
    tmp_i[1] = 0xB4A5F6E7;
  }
  execute(v, b, tmp_k, tmp_i, d, input_file, output_file);
  return 0;
}
