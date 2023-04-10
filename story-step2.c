#include <stdio.h>
#include <stdlib.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "usage: %s <categories and words file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  catarray_t * catarray = parse_categories_words(argv[1]);
  printWords(catarray);
  free_catarray(catarray);

  return EXIT_SUCCESS;
}
