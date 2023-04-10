# Custom Mad Libs game in C++

In this project, I have developed a program to tell a random
story (kind of like Mad Libs) that generates a story, given a story
template and a list of words to choose from.

For example, if you had a story like:

Once upon a time, there was a _animal_ who lived in a very _adjective_
_place_.

And some words to choose from like:

animal:dragon
animal:walrus
place:cave
adjective:peculiar
adjective:scary
adjective:peaceful

The program might produce:

Once upon a time, there was a walrus who lived in a very peaceful
cave.

The story template will be provided in an input file, where the "blanks"
are the category name preceded and followed by an underscore
("_"). Eventually, the program should replace this blank with a word from
the word list of the appropriate category. The categories and word choices
are provided in a separate input file with the category name, colon (":"),
then word. The program prints the story to stdout. 
