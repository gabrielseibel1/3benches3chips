#include <stdlib.h>
#include <stdio.h>

#define SIZE_OF_WORD 4

/**
 * Gets number of word to parse from program arguments
 */
size_t get_n_words_in_input(int argc, char **argv);

/**
 * Creates a random collection of words that are acceptable by the parser
 */
char *build_input(char *input, size_t n_chars);

/**
 * Creates random word acceptable by the parser
 * RegEx would look like: [a|b][c|d][e|f][g|h]
 * E.g "aceh", "bcfg" etc.
 */
char *get_random_word();

/**
 * Parses an input that has word like [a|b][c|d][e|f][g|h] and spaces
 */
void parse_input(char* input, size_t n_chars);

void main() {
  size_t n_words = 5;
  size_t n_chars = n_words * SIZE_OF_WORD + n_words /*spaces and \0*/;
  char input[5 * SIZE_OF_WORD + 5];

  srand(7);
  build_input(input, n_chars);

  //printf("Input: %s\n", input);
  parse_input(input, n_words * SIZE_OF_WORD);
  printf("Parsed %ld words of length %d successfully.\n", (long) n_words, SIZE_OF_WORD);

}

char *build_input(char *input, size_t n_chars) {
  int i;

  for (i = 0; i < n_chars; i += SIZE_OF_WORD + 1) {
    if (i + SIZE_OF_WORD + 1 < n_chars)
      sprintf(input + i, "%s ", get_random_word());
    else
      sprintf(input + i, "%s\0", get_random_word());
  }

  return input;
}

char *get_random_word() {
  //create words like [a|b][c|d][e|f][g|h]
  switch (rand() % 16) {
    default:
    case 0:   return "aceg";
    case 1:   return "aceh";
    case 2:   return "acfg";
    case 3:   return "acfh";
    case 4:   return "adeg";
    case 5:   return "adeh";
    case 6:   return "adfg"; //Dig in the dancing queen
    case 7:   return "adfh";
    case 8:   return "bceg";
    case 9:   return "bceh";
    case 10:  return "bcfg";
    case 11:  return "bcfh";
    case 12:  return "bdeg";
    case 13:  return "bdeh";
    case 14:  return "bdfg";
    case 15:  return "bdfh";
  }
}

void parse_input(char* input, size_t n_chars) {
  int state = 0;
  int i;
  for (i = 0; i < n_chars; ++i) {
    switch (state) {
      default:
      case 0: //first letter of a word
        if (input[i] == 'a' || input[i] == 'b') state = 1;
        else return;
        break;
      case 1: //second letter of a word
        if (input[i] == 'c' || input[i] == 'd') state = 2;
        else return;
        break;
      case 2: //third letter of a word
        if (input[i] == 'e' || input[i] == 'f') state = 3;
        else return;
        break;
      case 3: //forth letter of a word
        if (input[i] == 'g' || input[i] == 'h') state = 4;
        else return;
        break;
      case 4: //expects space character
        if (input[i] == ' ') state = 0;
        else if (input[i] == '\0') return;
        else return;
        break;
    }
  }
}