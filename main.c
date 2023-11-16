#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

#define MAX_FILE_NAME_SIZE 256

/* check_arguments: int char** --> void
 * check_arguments receives the argc and argv values and checks for correct command line execution
 */
void check_arguments(int argc, char *argv[]) {
  if (argc < 3 || argc > 4) {
    printf
        ("Insufficient arguments.\nUsage:  %s <phrases_file> <dict_file> [output_file]\n",
         argv[0]);
    exit(1);
  }
}


/* parse_arguments int char** char** char** char** --> void
 * Receives the argc and argv arguments from the main function, as well as pointers to character arrays for the
 * phrases_file_name, dict_file_name, and output_file_name.
 
 * Checks the validity of the command-line arguments, constructs the full file paths for the input and
 * output files by concatenating the directory paths of the phrases, dicts, and outputs folders with the file names,
 * and stores these file paths in the character arrays pointed to by the phrases_file_name, dict_file_name, and
 * output_file_name pointer.
 */
void parse_arguments(int argc, char *argv[], char phrases_file_name[],
                     char dict_file_name[], char output_file_name[]) {
  check_arguments(argc, argv);

  snprintf(phrases_file_name, MAX_FILE_NAME_SIZE, "phrases/%s", argv[1]);

  snprintf(dict_file_name, MAX_FILE_NAME_SIZE, "dicts/%s", argv[2]);

  if (argc == 4) {
    snprintf(output_file_name, MAX_FILE_NAME_SIZE, "outputs/%s", argv[3]);
  } else {
    snprintf(output_file_name, MAX_FILE_NAME_SIZE,
             "outputs/parsing_output.txt");
  }
}


/* USAGE   ./aho <phrases_file> <dict_file> [output_file]
 *
 * IMPORTANT! The files <phrases_file.txt> and <dict_file.txt> must be located in the ‘phrases’
 * and ‘dicts’ directories, respectively. Otherwise an error will occur when trying to open them.
 * 
 * OUTPUT. The third argument, <output_file.txt>, is optional. If not explicitly indicated, an output
 * with the name ‘parsing_output.txt’ will be generated. Both in this case and if a name is provided, the
 * output files will stored in the ‘outputs’ directory.* 
 */
int main(int argc, char *argv[]) {
  char phrases_file_name[MAX_FILE_NAME_SIZE];
  char dict_file_name[MAX_FILE_NAME_SIZE];
  char output_file_name[MAX_FILE_NAME_SIZE];

  parse_arguments(argc, argv, phrases_file_name, dict_file_name,
                  output_file_name);

  Trie *root = read_file_and_build_trie(dict_file_name);

  transform_trie_into_automaton(root);

  process_phrases(root, phrases_file_name, output_file_name);

  // print_results(root, output_file_name);

  trie_destroy(root);

  return 0;
}
