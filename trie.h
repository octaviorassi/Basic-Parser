#ifndef _TRIE_
#define _TRIE_

#define ALPHABET_SIZE 26

typedef struct _Trie {
  struct _Trie *children[ALPHABET_SIZE];
  struct _Trie *fail;
  struct _Trie *valid_suffix;
  int word_length;
} Trie;

Trie *trie_create();

void trie_insert(Trie * root, char *word);

int trie_is_word(Trie * node);

void trie_print_word_level(Trie * root, char *buffer, int level);

void trie_print_word_level_indented_prefixes(Trie * root, char *buffer,
                                             int level);

void trie_print(Trie * root);

void trie_destroy(Trie * root);

#endif
