#include "rand_story.h"

#include <stdbool.h>
#include <string.h>

void trim_newline(char * str) {
  char * c = strchr(str, '\n');
  if (c != NULL) {
    *c = '\0';
  };
}

FILE * open_file(char * file_name) {
  FILE * f = fopen(file_name, "r");
  if (f == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }
  return f;
}

void close_file(FILE * file) {
  if (fclose(file) != 0) {
    perror("fclose");
    exit(EXIT_FAILURE);
  }
}

void remove_used(char * category, catarray_t * catarray, category_t * used_words) {
  //Find the category in the catarray
  //If found, for each word in used_words, remove word from catarray
  for (size_t i = 0; i < catarray->n; i++) {
    if (!strcmp(category, (*(catarray->arr + i)).name)) {
      for (size_t j = 0; j < used_words->n_words; j++) {
        for (size_t k = 0; k < (*(catarray->arr + i)).n_words; k++) {
          if (!strcmp(used_words->words[j], (*(catarray->arr + i)).words[k])) {
            free((*(catarray->arr + i)).words[k]);
            size_t pos = k;
            while (pos + 1 < (*(catarray->arr + i)).n_words) {
              (*(catarray->arr + i)).words[pos] = (*(catarray->arr + i)).words[pos + 1];
              pos++;
            }
            (*(catarray->arr + i)).n_words--;
            break;
          }
        }
      }
      break;
    }
  }
}

void print_for_blank(char * category,
                     catarray_t * catarray,
                     category_t * used_words,
                     bool can_reuse,
                     FILE * template_f) {
  char * endptr = NULL;
  long back_ref = strtol(category, &endptr, 10);
  const char * word;
  if (*endptr != '\0' || back_ref <= 0) {
    if (!can_reuse)
      remove_used(category, catarray, used_words);
    word = chooseWord(category, catarray);
  }
  else if (back_ref > 0 && ((long)used_words->n_words - back_ref) >= 0) {
    word = used_words->words[(long)used_words->n_words - back_ref];
  }
  else {
    fprintf(stderr,
            "Back reference must be a positive number and not more than used words "
            "size!\n");
    free(category);
    free_catarray(catarray);
    free_category(used_words);
    close_file(template_f);
    exit(EXIT_FAILURE);
  }

  // print the word and add to used_words
  printf("%s", word);
  used_words->words =
      realloc(used_words->words, (used_words->n_words + 1) * sizeof(*used_words->words));
  used_words->words[used_words->n_words] =
      malloc((strlen(word) + 1) * sizeof(*used_words->words[used_words->n_words]));
  strcpy(used_words->words[used_words->n_words], word);
  used_words->n_words++;
}

void parse_story_template(char * story_template, catarray_t * catarray, bool can_reuse) {
  FILE * template_f = open_file(story_template);
  char * category = NULL;
  bool blank_ends = true;
  int c = 0;
  size_t i = 0;
  category_t * used_words = malloc(sizeof(*used_words));
  used_words->name = NULL;
  used_words->words = NULL;
  used_words->n_words = 0;
  while ((c = fgetc(template_f)) != EOF) {
    if (c == '_') {
      if (!blank_ends) {
        category = realloc(category, (i + 1) * sizeof(*category));
        category[i] = '\0';
        print_for_blank(category, catarray, used_words, can_reuse, template_f);
        blank_ends = true;
        free(category);
        category = NULL;
        i = 0;
      }
      else {
        blank_ends = false;
      }
    }
    else if (blank_ends) {
      fputc(c, stdout);
    }
    else {
      category = realloc(category, (i + 1) * sizeof(*category));
      category[i] = c;
      i++;
    }
  }
  free(category);

  if (!blank_ends) {
    fprintf(stderr, "Matching underscore not found!\n");
    exit(EXIT_FAILURE);
  }
  close_file(template_f);
  free_category(used_words);
}

catarray_t * parse_categories_words(char * categories) {
  FILE * categories_f = open_file(categories);
  catarray_t * catarray = NULL;
  char * curr = NULL;
  size_t sz = 0;
  char * name = NULL;
  char * word = NULL;
  while (getline(&curr, &sz, categories_f) >= 0) {
    char * temp = curr;
    name = strsep(&temp, ":");
    word = temp;
    if (word == NULL) {
      fprintf(stderr, "Delimiter ':' not found!\n");
      exit(EXIT_FAILURE);
    }
    trim_newline(word);
    if (catarray != NULL) {
      //if category found, add the word to the words array
      for (size_t i = 0; i < catarray->n; i++) {
        if (!strcmp((*(catarray->arr + i)).name, name)) {
          (*(catarray->arr + i)).words =
              realloc((*(catarray->arr + i)).words,
                      ((*(catarray->arr + i)).n_words + 1) *
                          sizeof(*(*(catarray->arr + i)).words));
          (*(catarray->arr + i)).words[(*(catarray->arr + i)).n_words] = malloc(
              (strlen(word) + 1) *
              sizeof(*(*(catarray->arr + i)).words[(*(catarray->arr + i)).n_words]));
          strcpy((*(catarray->arr + i)).words[(*(catarray->arr + i)).n_words], word);
          (*(catarray->arr + i)).n_words++;
          break;
        }
        //if category not word, create category and add word to words array
        if (i == catarray->n - 1) {
          catarray->arr =
              realloc(catarray->arr, (catarray->n + 1) * sizeof(*catarray->arr));
          (*(catarray->arr + i + 1)).name =
              malloc((strlen(name) + 1) * sizeof(*(*(catarray->arr + i + 1)).name));
          strcpy((*(catarray->arr + i + 1)).name, name);
          (*(catarray->arr + i + 1)).words =
              malloc(sizeof(*(*(catarray->arr + i + 1)).words));
          (*(catarray->arr + i + 1)).words[0] =
              malloc((strlen(word) + 1) * sizeof(*(*(catarray->arr + i + 1)).words[0]));
          strcpy((*(catarray->arr + i + 1)).words[0], word);
          (*(catarray->arr + i + 1)).n_words = 1;
          catarray->n++;
          break;
        }
      }
    }
    //if catarray is NULL, create new catarray and create category and add word to words array
    else {
      catarray = malloc(sizeof(*catarray));
      catarray->arr = malloc(sizeof(*catarray->arr));
      (*(catarray->arr)).name =
          malloc((strlen(name) + 1) * sizeof(*(*(catarray->arr)).name));
      strcpy((*(catarray->arr)).name, name);
      (*(catarray->arr)).words = malloc(sizeof(*(*(catarray->arr)).words));
      (*(catarray->arr)).words[0] =
          malloc((strlen(word) + 1) * sizeof(*(*(catarray->arr)).words[0]));
      strcpy((*(catarray->arr)).words[0], word);
      (*(catarray->arr)).n_words = 1;
      catarray->n = 1;
    }
    name = NULL;
    word = NULL;
  }
  free(curr);
  close_file(categories_f);
  return catarray;
}

void free_catarray(catarray_t * catarray) {
  for (size_t i = 0; i < catarray->n; i++) {
    free((*(catarray->arr + i)).name);
    for (size_t j = 0; j < (*(catarray->arr + i)).n_words; j++)
      free((*(catarray->arr + i)).words[j]);
    free((*(catarray->arr + i)).words);
  }
  free(catarray->arr);
  free(catarray);
}

void free_category(category_t * category) {
  for (size_t i = 0; i < category->n_words; i++) {
    free(category->words[i]);
  }
  free(category->words);
  free(category);
}
