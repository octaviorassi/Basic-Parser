#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "parser.h"

#define MAX_FILE_NAME_SIZE 256

int compare_files(FILE * fp1, FILE * fp2) {
  char ch1 = getc(fp1);
  char ch2 = getc(fp2);
  int error = 0, pos = 0, line = 1;
  while (ch1 != EOF && ch2 != EOF) {
    pos++;

    if (ch1 == '\n' && ch2 == '\n') {
      line++;
      pos = 0;
    } else if (ch1 == '\n')
      ch1 = getc(fp1);
    else if (ch2 == '\n')
      ch2 = getc(fp2);

    if (ch1 != ch2) {
      error++;
      printf
          ("Line Number : %d \tError Position: %d | File1 = %c | File2 = %c\n",
           line, pos, ch1, ch2);
    }

    ch1 = getc(fp1);
    ch2 = getc(fp2);
  }

  return (ch1 == EOF && ch2 == EOF) ? error : -1;
}

/* test_duhalde uses duhalde_dict to parse test_duhalde_phrases and
 * compares the resulting file with  the expected results per our
 * requirements, test_duhalde_expected_output.txt, using compare_files.
 * 
 * test_duhalde_phrases contains a series of phrases, each associated
 * with a certain border case for our program.
 */
void test_duhalde() {
  char *phrases_file_name = "phrases/test_duhalde_phrases.txt";
  char *dict_file_name = "dicts/duhalde_dict.txt";
  char *output_file_name = "outputs/test_duhalde_output.txt";

  Trie *root = read_file_and_build_trie(dict_file_name);

  transform_trie_into_automaton(root);

  process_phrases(root, phrases_file_name, output_file_name);

  FILE *actual_output = fopen(output_file_name, "r");
  FILE *expected_output =
      fopen("outputs/test_duhalde_expected_output.txt", "r");

  assert(compare_files(actual_output, expected_output) == 0);

  // print_results(root, output_file_name);
  fclose(actual_output);
  fclose(expected_output);
  trie_destroy(root);

  printf("Passed test_duhalde!\n");
}

/* test_update_longest_word tests the function on the three main cases,
 *  updating a position which initially had no length stored
 *  updating a position which previously had a shorter length stored
 *  not updating a position after receiving a new length smaller than the
 * already stored.
*/
void test_update_longest_word() {
  int longest_word_arr[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  Trie *node1 = trie_create();
  node1->word_length = 3;

  Trie *node2 = trie_create();
  node2->word_length = 4;

  Trie *node3 = trie_create();
  node3->word_length = 2;

  update_longest_word(longest_word_arr, node1, 4);      // Case: update from 0
  assert(longest_word_arr[0] == 0);
  assert(longest_word_arr[1] == 0);
  assert(longest_word_arr[2] == 3);
  assert(longest_word_arr[3] == 0);
  assert(longest_word_arr[4] == 0);
  assert(longest_word_arr[5] == 0);
  assert(longest_word_arr[6] == 0);
  assert(longest_word_arr[7] == 0);
  assert(longest_word_arr[8] == 0);
  assert(longest_word_arr[9] == 0);

  update_longest_word(longest_word_arr, node2, 5);      // Case: update from existing value
  assert(longest_word_arr[0] == 0);
  assert(longest_word_arr[1] == 0);
  assert(longest_word_arr[2] == 4);
  assert(longest_word_arr[3] == 0);
  assert(longest_word_arr[4] == 0);
  assert(longest_word_arr[5] == 0);
  assert(longest_word_arr[6] == 0);
  assert(longest_word_arr[7] == 0);
  assert(longest_word_arr[8] == 0);
  assert(longest_word_arr[9] == 0);

  update_longest_word(longest_word_arr, node3, 3);      // Case: no update
  assert(longest_word_arr[0] == 0);
  assert(longest_word_arr[1] == 0);
  assert(longest_word_arr[2] == 4);
  assert(longest_word_arr[3] == 0);
  assert(longest_word_arr[4] == 0);
  assert(longest_word_arr[5] == 0);
  assert(longest_word_arr[6] == 0);
  assert(longest_word_arr[7] == 0);
  assert(longest_word_arr[8] == 0);
  assert(longest_word_arr[9] == 0);


  trie_destroy(node1);
  trie_destroy(node2);
  trie_destroy(node3);

  printf("Passed test_update_longest_word!\n");
}

/* test_trie_insert tests the insertion of two words sharing a prefix.
*/
void test_trie_insert() {
  Trie *root = trie_create();

  trie_insert(root, "test");
  assert(root->children['t' - 'a'] != NULL);
  assert(root->children['t' - 'a']->children['e' - 'a'] != NULL);
  assert(root->children['t' - 'a']->children['e' - 'a']->children['s' - 'a'] !=
         NULL);
  assert(root->children['t' - 'a']->children['e' - 'a']->
         children['s' - 'a']->children['t' - 'a'] != NULL);
  assert(root->children['t' - 'a']->children['e' - 'a']->
         children['s' - 'a']->children['t' - 'a']->word_length == 4);

  trie_insert(root, "tea");
  assert(root->children['t' - 'a'] != NULL);
  assert(root->children['t' - 'a']->children['e' - 'a'] != NULL);
  assert(root->children['t' - 'a']->children['e' - 'a']->children['s' - 'a'] !=
         NULL);
  assert(root->children['t' - 'a']->children['e' - 'a']->
         children['s' - 'a']->children['t' - 'a'] != NULL);
  assert(root->children['t' - 'a']->children['e' - 'a']->
         children['s' - 'a']->children['t' - 'a']->word_length == 4);
  assert(root->children['t' - 'a']->children['e' - 'a']->children['a' - 'a'] !=
         NULL);
  assert(root->children['t' - 'a']->children['e' - 'a']->
         children['a' - 'a']->word_length == 3);

  trie_destroy(root);
  printf("Passed test_trie_insert!\n");
}

int main() {
  test_duhalde();
  test_update_longest_word();
  test_trie_insert();
  return 0;
}
