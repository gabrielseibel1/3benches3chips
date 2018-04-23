#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define SIZE_OF_WORD 4

/**
 * Gets number of word to parse from program arguments
 */
size_t get_n_words_in_input(int argc, int argv);

/**
 * Creates a random collection of words that are acceptable by the parser
 */
void build_input(size_t n_chars, size_t n_chars2);

/**
 * Creates random word acceptable by the parser
 * RegEx would look like: [a|b][c|d][e|f][g|h]
 * E.g "aceh", "bcfg" etc.
 */
char *get_random_word(void);

/**
 * Parses an input that has word like [a|b][c|d][e|f][g|h] and spaces
 */

int main() {
  int argc; 
  int argv;
  argc = 2;
  argv = 5000;	
  
  srand((unsigned int) time(NULL));

  size_t n_words = get_n_words_in_input(argc, argv);
  size_t n_chars = n_words * SIZE_OF_WORD + n_words /*spaces and \0*/;
  
  build_input(n_chars, n_words * SIZE_OF_WORD);
 
  printf("Parsed %ld words of length %d successfully.\n", (long) n_words, SIZE_OF_WORD);

  return 0;
}

size_t get_n_words_in_input(int argc, int argv) {
  if (argc != 2) {
    fprintf(stderr, "Expected args: n_words_to_parse\n");
    exit(EXIT_FAILURE);
  }

  return argv;
}

void build_input(size_t n_chars, size_t n_chars2) {
	 
  char input[n_chars];
  char *aux;
  
  int i, j; 
  int state = 0;
  int n;
  j=0;
  
  for (i = 0; i<=n_chars; i++) {
  	aux = get_random_word();
  	
  	if(i < n_chars){
  		while(aux[j] != '\0'){
  			input[i] = aux[j];
  			i++;
  			j++;
  		}
  		j=0;
  		input[i] = ' ';
  			
  	}
  	else{
  		input[i] = '\0';
  	}
  }

  printf("input = %s", input);
  
  for(n=0; n<10; n++){
  	for (i = 0; i < n_chars2; ++i) {
    		switch (state) {
      		default:
      		case 0: //first letter of a word
        		if (input[i] == 'a' || input[i] == 'b') state = 1;
        		else exit(EXIT_FAILURE);
        		break;
      		case 1: //second letter of a word
        		if (input[i] == 'c' || input[i] == 'd') state = 2;
        		else exit(EXIT_FAILURE);
        		break;
      		case 2: //third letter of a word
        		if (input[i] == 'e' || input[i] == 'f') state = 3;
        		else exit(EXIT_FAILURE);
        		break;
      		case 3: //forth letter of a word
        		if (input[i] == 'g' || input[i] == 'h') state = 4;
        		else exit(EXIT_FAILURE);
        		break;
      		case 4: //expects space character
        		if (input[i] == ' ') state = 0;
        		else if (input[i] == '\0') return;
        		else exit(EXIT_FAILURE);
        		break;
    		}
  	}
  }
  
}

char *get_random_word(void) {
  //create words like [a|b][c|d][e|f][g|h]
  switch (rand() % 16) {
    default:
    case 0:   return "aceg";
    case 1:   return "aceh";
    case 2:   return "acfg";
    case 3:   return "acfh";
    case 4:   return "adeg";
    case 5:   return "adeh";
    case 6:   return "adfg"; 
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


