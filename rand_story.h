#ifndef __RAND_STORY_H__
#define __RAND_STORY_H__

#include <stdbool.h>
#include <stdio.h>

#include "provided.h"

//any functions you want your main to use
void parse_story_template(char * story_template, catarray_t * catarray, bool can_reuse);
catarray_t * parse_categories_words(char * categories);
void free_catarray(catarray_t * catarray);
void free_category(category_t * category);
#endif
