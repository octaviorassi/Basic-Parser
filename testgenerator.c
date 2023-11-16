#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORD_LENGTH 256
#define MAX_WORDS 1000
#define LENGTH_VARIANCE 4

int main(int argc, char *argv[]) {
  // Check if the required command line arguments were provided
  if (argc < 4) {
    printf("Error: missing command line arguments\n");
    printf("Usage: %s DICT_FILE NUM_PHRASES PHRASE_LENGTH\n", argv[0]);
    return 1;
  }
  // Get the command line arguments
  char dict_file_name[1024];
  snprintf(dict_file_name, sizeof(dict_file_name), "dicts/%s", argv[1]);
  int num_phrases = atoi(argv[2]);
  int phrase_length = atoi(argv[3]);

  // Calculate the minimum and maximum phrase length
  int min_phrase_length = phrase_length - phrase_length / LENGTH_VARIANCE;
  int max_phrase_length = phrase_length + phrase_length / LENGTH_VARIANCE;

  // Seed the random number generator
  srand(time(NULL));

  // Open the input file
  FILE *input_file = fopen(dict_file_name, "r");
  if (input_file == NULL) {
    printf("Error: could not open input file\n");
    return 1;
  }
  // Read the words from the input file into an array
  char words[MAX_WORDS][MAX_WORD_LENGTH];
  int num_words = 0;
  while (fgets(words[num_words], MAX_WORD_LENGTH, input_file) != NULL) {
    // Remove the newline character from the end of the word and null-terminate it.
    words[num_words][strlen(words[num_words]) - 1] = '\0';

    num_words++;
  }

  // Close the input file
  fclose(input_file);

  // Remove the .txt extension from the dictionary file name
  char dict_name[100];
  strcpy(dict_name, argv[1]);
  char *dot = strrchr(dict_name, '.');
  if (dot != NULL && strcmp(dot, ".txt") == 0)  // If there is a dot followed by "txt", end the string there.
    *dot = '\0';

  // Generate the output file name
  char output_file_name[256];
  sprintf(output_file_name, "phrases/%s_%d_%d-%d.txt", dict_name, num_phrases,
          min_phrase_length, max_phrase_length);

  // Open the output file
  FILE *output_file = fopen(output_file_name, "w");
  if (output_file == NULL) {
    printf("Error: could not open output file\n");
    return 1;
  }
  // Generate the phrases and write them to the output file
  for (int i = 0; i < num_phrases; i++) {
    char phrase[max_phrase_length];
    phrase[0] = '\0';
    int current_phrase_length =
        min_phrase_length + rand() % (max_phrase_length - min_phrase_length +
                                      1);

    while ((int) strlen(phrase) < current_phrase_length) {
      // Choose a random word from the dictionary
      int word_index = rand() % num_words;
      char *word = words[word_index];

      // Choose a random prefix of the word
      int prefix_length = rand() % (strlen(word) + 1);

      // Check if adding the prefix would exceed the maximum phrase length
      int phrase_len = strlen(phrase);
      if (phrase_len + prefix_length > max_phrase_length)
        // If it would, only add as many characters as possible
        prefix_length = max_phrase_length - phrase_len;


      strncpy(phrase + phrase_len, word, prefix_length);
      phrase[phrase_len + prefix_length] = '\0';
    }

    fprintf(output_file, "%s\n", phrase);
  }

  fclose(output_file);

  return 0;
}
