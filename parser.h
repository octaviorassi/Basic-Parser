#ifndef _PARSER_H_
#define _PARSER_H_
#include <stdio.h>
#include "trie.h"

void handle_error(char *error_msg, FILE * file);

FILE *open_output_file(char *output_filename);

FILE *open_phrases_file(char *phrases_filename);

int char_to_index(char c);

void update_longest_word(int *longest_word_arr, Trie * node, int i);

int print_parsed_text_into_file(int *longest_word_arr, char *text,
                                char errors[], int n, FILE * fp_out);

void print_error_message_to_file(char errors[], int charSkippedCount,
                                 FILE * output_file, int n);

char *read_line(FILE * file, char **buffer, size_t *buffer_size);

void process_file(Trie * root, FILE * phrases_file, FILE * output_file);

void process_phrases(Trie * root, char *phrases_file_name,
                     char *output_file_name);

void print_file_content(char *filename);

void print_results(Trie * root, char *filename);

Trie *read_file_and_build_trie(char *filename);

void transform_trie_into_automaton(Trie * root);

void parse_string(Trie * root, char *text, FILE * output_file);

#endif
