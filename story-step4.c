#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rand_story.h"

void cmd_fail(char * argv0);

int main(int argc, char ** argv) {
  if (argc != 3 && argc != 4) {
    cmd_fail(argv[0]);
  }

  bool can_reuse = true;
  char * cat_f = NULL;
  char * template_f = NULL;

  if (argc == 3) {
    cat_f = argv[1];
    template_f = argv[2];
  }
  else if (!strcmp("-n", argv[1])) {
    can_reuse = false;
    cat_f = argv[2];
    template_f = argv[3];
  }
  else {
    cmd_fail(argv[0]);
  }
  catarray_t * catarray = parse_categories_words(cat_f);
  parse_story_template(template_f, catarray, can_reuse);
  free_catarray(catarray);

  return EXIT_SUCCESS;
}

void cmd_fail(char * argv0) {
  fprintf(stderr,
          "usage: %s <-n (optional)> <categories and words file> <story template file>\n",
          argv0);
  exit(EXIT_FAILURE);
}
