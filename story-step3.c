#include <stdio.h>
#include <stdlib.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 3) {
    fprintf(
        stderr, "usage: %s <categories and words file> <story template file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  catarray_t * catarray = parse_categories_words(argv[1]);
  parse_story_template(argv[2], catarray, true);
  free_catarray(catarray);

  return EXIT_SUCCESS;
}
