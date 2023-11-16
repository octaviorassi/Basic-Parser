#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "trie.h"

/* trie_create: void --> Trie*
 * trie_create returns a pointer to a Trie, with its children, fail,
 * and valid_suffix initialized to NULL and word_length set to 0.
 */
Trie *trie_create() {
  Trie *trieNode = malloc(sizeof(Trie));

  for (int i = 0; i < ALPHABET_SIZE; i++)
    trieNode->children[i] = NULL;

  trieNode->word_length = 0;
  trieNode->fail = NULL;
  trieNode->valid_suffix = NULL;

  return trieNode;
}

/* trie_insert: Trie* char* --> void
 * trie_insert receives a pointer to a Trie root and a string, and adds the
 * corresponding nodes to represent the word on the Trie (if required)
 */
void trie_insert(Trie * root, char *word) {
  // We assume root != NULL, i.e. inserting into an already existing trie.
  Trie *tmp = root;
  int cPos;

  int i;
  for (i = 0; word[i] != '\0'; i++) {
    cPos = (int) tolower(word[i]) - 'a';

    if (tmp->children[cPos] == NULL)
      tmp->children[cPos] = trie_create();

    tmp = tmp->children[cPos];
  }

  // Now we are at the end of the word, therefore the last node is a word.
  tmp->word_length = i;
}

/* trie_is_word: Trie* --> int
 * trie_is_word receives a Trie* node and returns 1 if it is a word (i.e. its length is positive) or 0 if not.
 */
int trie_is_word(Trie * node) {
  return node->word_length > 0;
}

/* trie_print_word_level: Trie* char* int --> void
 * trie_print_word_level receives a pointer to the root of a trie, a character buffer and 
 * an int representing the current level.
 * trie_print_word_level recursively performs a depth-first traversal of the Trie, printing valid words only.
 */
void trie_print_word_level(Trie * root, char *buffer, int level) {
  if (root == NULL)
    return;
  if (trie_is_word(root)) {
    buffer[level] = '\0';
    printf("%s\n", buffer);
  }

  for (int i = 0; i < ALPHABET_SIZE; i++) {
    if (root->children[i]) {
      buffer[level] = i + 'a';
      trie_print_word_level(root->children[i], buffer, level + 1);
    }
  }

}

/* trie_print_word_level_indented: Trie* char* int --> void
 * trie_print_word_level receives a pointer to the root of a trie, a character buffer and
 * an int representing the current level.
 * trie_print_word_level recursively performs a depth-first traversal of the Trie, printing
 * valid words only along with an  indentation to help visualize Trie levels.
 */
void trie_print_word_level_indented_prefixes(Trie * root, char *buffer,
                                             int level) {
  if (root == NULL)
    return;

  // Print the current node
  buffer[level] = '\0';
  printf("%*s%s\n", level * 2, "", buffer);

  // Recursively print the children
  for (int i = 0; i < ALPHABET_SIZE; i++) {
    if (root->children[i]) {
      buffer[level] = i + 'a';
      trie_print_word_level_indented_prefixes(root->children[i], buffer,
                                              level + 1);
    }
  }
}

/* trie_print: Trie* --> void
 * trie_print receives a pointer to the root of a trie and prints the words of the dictionary it represents.
 */
void trie_print(Trie * root) {
  char buffer[256];
  trie_print_word_level(root, buffer, 0);
  // trie_print_word_level_indented_prefixes(root, buffer, 0);
}

/* trie_destroy: Trie* --> void
 * trie_destroy receives a pointer to the root of a trie and frees its nodes recursively.
 */
void trie_destroy(Trie * root) {
  if (root == NULL)
    return;

  for (int i = 0; i < ALPHABET_SIZE; i++) {
    if (root->children[i])
      trie_destroy(root->children[i]);
  }

  free(root);
}
