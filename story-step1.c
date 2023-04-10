#include <stdio.h>
#include <stdlib.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "usage: %s <story_template_file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  parse_story_template(argv[1], NULL, true);

  return EXIT_SUCCESS;
}
