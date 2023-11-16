#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "GQUEUES.h"
#include "trie.h"

#define INITIAL_BUFFER_SIZE 2048

/* handle_error: char* FILE* --> void
 * handle_error receives an error message and a file pointer. Prints the message, closes the file, and exits with status 1.
 */
void handle_error(char *error_msg, FILE * file) {
  printf("%s\n", error_msg);
  fclose(file);
  exit(1);
}


// #################### TRIE AND AUTOMATON CONSTRUCTION #####################


/* read_file_and_build_trie: char* --> Trie*
 * read_file_and_build_trie receives the name of a dict file, opens it, and returns its corresponding Trie.
 * Accounts for:    * Error when opening file.
                    * Empty lines.
                    * Spaces
                    * Non-alphabetic characters
                    * Empty file
 * IMPORTANT. Any of those results in an error message and the termination of the program.
 */
Trie *read_file_and_build_trie(char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL)
    handle_error("Error: could not open file", file);

  Trie *root = trie_create();
  char word[256];
  int word_len = 0;
  int line_num = 1;
  int c;

  while ((c = fgetc(file)) != EOF) {
    if (c == '\n') {            // End of word, checking for non-empty word
      if (word_len == 0) {
        sprintf(word, "Error: empty line at line %d in file %s", line_num,
                filename);
        handle_error(word, file);
      }

      word[word_len] = '\0';
      trie_insert(root, word);  // Non-empty word whose characters are all valid: we add it.

      word_len = 0;
      line_num++;
    } else if (isspace(c)) {    // Input file should not contain any spaces.
      sprintf(word, "Error: space character at line %d in file %s", line_num,
              filename);
      handle_error(word, file);

    } else if (!isalpha(c)) {   // Input file should contain alphabetic characters exclusively.
      sprintf(word, "Error: invalid character '%c' at line %d in file %s", c,
              line_num, filename);
      handle_error(word, file);

    } else {                    // Case: reading a still valid word. Continue.
      word[word_len++] = c;
    }
  }

  if (ftell(file) == 0)
    handle_error("Error: file is empty", file); // Checking for an empty .txt

  if (word_len > 0) {           // Adding the very last word, which does not end in \n therefore was not included.
    word[word_len] = '\0';
    trie_insert(root, word);
  }

  fclose(file);
  return root;
}


/* transform_trie_into_automaton: Trie* --> void
 * transform_trie_into_automaton receives an already generated Trie and constructs its fail and valid_suffix
 * pointers to represent its corresponding automaton using BFS.
 */
void transform_trie_into_automaton(Trie * root) {
  // Initialize fail pointers
  root->fail = NULL;
  for (int i = 0; i < ALPHABET_SIZE; i++)
    if (root->children[i] != NULL)
      root->children[i]->fail = root;

  // Breadth-first search to construct fail pointers
  Queue q = queue_create();
  for (int i = 0; i < ALPHABET_SIZE; i++)
    if (root->children[i] != NULL)
      queue_enqueue(q, root->children[i], id);

  while (!queue_is_empty(q)) {
    Trie *node = queue_start(q, id);
    queue_dequeue(q, skip);

    for (int i = 0; i < ALPHABET_SIZE; i++) {
      if (node->children[i] != NULL) {
        Trie *fail_node = node->fail;

        while (fail_node != NULL && fail_node->children[i] == NULL)
          fail_node = fail_node->fail;

        if (fail_node == NULL)
          node->children[i]->fail = root;

        else {
          node->children[i]->fail = fail_node->children[i];

          // Valid Suffix links construction
          Trie *vsuffix_node = node->children[i]->fail;
          while (vsuffix_node != root && vsuffix_node->word_length == 0)
            vsuffix_node = vsuffix_node->fail;

          if (vsuffix_node != root)
            node->children[i]->valid_suffix = vsuffix_node;
        }

        queue_enqueue(q, node->children[i], id);
      }
    }
  }

  queue_destroy(q, skip);
}


// ########################### PARSING ALGORITHM ############################
// ###########################     AUXILIARY     ############################


/* open_output_file: char* --> FILE*
 * open_output_file receives the name of the output file, opens it in write
 * mode and returns its FILE pointer. 
 */
FILE *open_output_file(char *output_filename) {
  FILE *output_file = fopen(output_filename, "w");
  if (output_file == NULL)
    handle_error("Error: could not open output file", output_file);
  return output_file;
}


/* open_phrases_file: char* --> FILE*
 * open_phrases_file receives the name of the phrases file, opens it in read mode and returns its FILE pointer. 
 */
FILE *open_phrases_file(char *phrases_filename) {
  FILE *phrases_file = fopen(phrases_filename, "r");
  if (phrases_file == NULL)
    handle_error("Error: could not open phrases file", phrases_file);
  return phrases_file;
}


/* char_to_index: char --> int
 * char_to_index receives a char and returns its index according
 * to our representation ('a' = 0, 'b' = 1, ...).
 */
int char_to_index(char c) {
  return (int) tolower(c) - 'a';
}


/* update_longest_word: int[] Trie* int --> void
 * update_longest_word receives the int array used to represent the longest word in text beginning at index i, a node
 * and the integer i corresponding to the current index. If the length of the word is greater than the current
 * value stored at (i - wordlen + 1), it gets updated. Otherwise, no changes are done.
*/
void update_longest_word(int *longest_word_arr, Trie * node, int i) {
  longest_word_arr[i - node->word_length + 1] =
      node->word_length > longest_word_arr[i - node->word_length + 1] ?
      node->word_length : longest_word_arr[i - node->word_length + 1];
}


/* print_parsed_texto_into_file: int[] char* char* int FILE* --> int
 * print_parsed_texto_into_file iterates over each position in the text and checks if a word was found at that position
 * by looking at the value of longest_word_arr[i].
 *  If a word was found, the function writes the characters of the word to the output file and
 * increments the loop index by the length of the word.
 *  If no word was found at the current position, the function increments a counter of skipped characters and stores the 
 * skipped character on the errors[] array.
 * 
 * Returns the number of characters skipped.
 */
int print_parsed_text_into_file(int *longest_word_arr, char *text,
                                char errors[], int n, FILE * fp_out) {
  int skippedCharsCount = 0;
  for (int i = 0; i < n; i++) {
    if (longest_word_arr[i] > 0) {
      for (int j = i; j < i + longest_word_arr[i]; j++) {
        fputc(text[j], fp_out);
      }
      fputc(' ', fp_out);       // Notice an unwanted space will be generated when there are no errors.
      i += longest_word_arr[i] - 1;
    } else {
      errors[skippedCharsCount] = text[i];
      skippedCharsCount++;
    }
  }

  return skippedCharsCount;
}


/* print_error_message_to_file: char[] int FILE* int --> void
 * print_error_message_to_file receives the errors array, characters skipped count, a pointer to the output file,
 * and the length of the text, and prints the corresponding error message, if required.
 */
void print_error_message_to_file(char errors[], int charSkippedCount,
                                 FILE * output_file, int n) {
  if (charSkippedCount > 0) {
    char lastLine[32];
    int len =
        snprintf(lastLine, sizeof(lastLine), "| %d char%c skipped | Errors: ",
                 charSkippedCount, (charSkippedCount == 1) ? ' ' : 's');
    lastLine[len] = '\0';

    fprintf(output_file, "%s", lastLine);

    for (int i = 0; i < charSkippedCount; i++)
      fputc(errors[i], output_file);
    fputc('\n', output_file);

  } else {                      // If we have no skipped chars, we must have printed an unnecesary space which we must remove
    // except when n == 0, i. e. the line is empty.
    if (n > 0)
      fseek(output_file, -1, SEEK_CUR);
    fputc('\n', output_file);
  }
}


// ###########################      PARSING      ############################

/* parse_string: Trie* char* FILE* --> void
 * parse_string receives the automaton (Trie) which represents our dictionary, a string which we want to parse, and 
 * the FILE* where we will store the resulting string. 
 * parse_string creates an int array of size n, where n is the length of the string. It proceeds by filling out the i-index
 * of the array with a number representing the maximum possible length of a valid word starting at the i index of the string.
 * 
 * EXAMPLE. If our string were "dolaredolaress" with dictionary {dolar, dolares} the resulting array would be
 *                                          [5, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0].
 *          Then, parse_string takes the words text[0:5) and text[6: 6 + 7), printing the text "dolar dolares"
 *          to the file, and returns 2 due to the skipped 'e' and 's'.
 *
 * EXAMPLES
 * Dictionary: {quien, deposito, dolar, dolares, recibira}
 * text: "quiendepositodolaresrecibiradolares"     -- prints -->   "quien deposito dolares recibira dolares"
 * text: "xquienwdepositodolareszxrecibiradolares" -- prints -->   "quien deposito dolares recibira dolares | 4 chars skipped | Errors: xwzx"
 * text: "invalidtext"                             -- prints -->   "| 11 chars skipped | Errors: invalidtext"
 * text: ""                                        -- prints -->   ""
 * text: "qquiendepositodolaredolaress"            -- prints -->   "quien deposito dolar dolares | 3 chars skipped | Errors: qss"
 */
void parse_string(Trie * root, char *text, FILE * fp_out) {
  int n = strlen(text);
  int longest_word[n];
  memset(longest_word, 0, sizeof(longest_word));

  Trie *node = root;

  for (int i = 0; i < n; i++) {
    int index = char_to_index(text[i]);

    while (node != NULL && node->children[index] == NULL)
      node = node->fail;

    if (node == NULL)
      node = root;

    else {
      node = node->children[index];
      Trie *vsuffix_node = node;

      while (vsuffix_node) {
        update_longest_word(longest_word, vsuffix_node, i);
        vsuffix_node = vsuffix_node->valid_suffix;
      }
    }
  }

  char errors[n];
  int skippedCharsCount =
      print_parsed_text_into_file(longest_word, text, errors, n, fp_out);

  print_error_message_to_file(errors, skippedCharsCount, fp_out, n);
}


/* read_line: FILE* char** size_t* --> char*
 * read_line receives a pointer to a FILE, a pointer to a buffer and a pointer to a size_t variable.
 * Returns a pointer to the buffer with the contents of the next line of the file.
 * Pointers are used in case that the a memory reallocation is required for our buffer.
 */
char *read_line(FILE * file, char **buffer, size_t *buffer_size) {
  size_t length = 0;

  // Keeps trying to read the entire line until it does fit into the length defined.
  while (fgets(*buffer + length, *buffer_size - length, file) != NULL) {
    length += strlen(*buffer + length);

    if ((*buffer)[length - 1] == '\n') {        // Case: the line read did fit in the current buffer size. We return it.
      (*buffer)[length - 1] = '\0';
      return (*buffer);
    }
    // The line was too long to fit in the buffer, so we need to increase its size
    *buffer_size *= 2;
    *buffer = realloc(*buffer, *buffer_size);

    if (*buffer == NULL) {      // Checks for possible errors allocating memory.
      free(*buffer);
      return NULL;
    }
  }

  return NULL;
}


/* process_file: Trie* FILE* FILE* --> void
 * process_file receives the root of a Trie (Automaton), and pointers to the phrases file and output file. It parses the
 * phrases file line by line and prints the resulting strings to the output file.
 */
void process_file(Trie * root, FILE * phrases_file, FILE * output_file) {
  size_t buffer_size = INITIAL_BUFFER_SIZE;

  char *buffer = malloc(buffer_size);
  if (buffer == NULL)
    return;

  char *line;
  while ((line = read_line(phrases_file, &buffer, &buffer_size)) != NULL) {
    parse_string(root, line, output_file);
  }

  free(buffer);
}


/* process_phrases: Trie* char* char* --> void
 * process_phrases receives a Trie (Automaton) and the names of the phrases and output files.
 * It opens both files, invokes process_file and then closes the files.
 */
void process_phrases(Trie * root, char *phrases_file_name,
                     char *output_file_name) {
  FILE *output_file = open_output_file(output_file_name);
  FILE *phrases_file = open_phrases_file(phrases_file_name);

  process_file(root, phrases_file, output_file);

  fclose(output_file);
  fclose(phrases_file);
}


// ###########################       EXTRA       ############################


/* print_file_content: char* --> void
 * print_file_content receives the name of a file, opens it in read mode, and prints it line per line.
 */
void print_file_content(char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf
        ("Error: failed to open the file when trying to print its contents.\n");
    return;
  }

  char line[1024];
  while (fgets(line, sizeof(line), file) != NULL) {
    printf("%s", line);
  }

  fclose(file);
}


/* print_results: Trie* char* --> void
 * print_results receives the Trie generated during the main program execution
 * and the name of the output file, and prints the trie and parsing results.
 */
void print_results(Trie * root, char *filename) {
  printf
      ("------------------------------------- TRIE -------------------------------------\n");
  trie_print(root);

  printf
      ("-------------------------------- PARSING RESULTS -------------------------------\n");
  print_file_content(filename);
  printf
      ("-------------------------------- --------------- -------------------------------\n");
}
