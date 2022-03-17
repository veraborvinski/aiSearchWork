#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "search.h"
/*
*Author: Vera Borvinski
*The trie file is used as an access point for the project
*/

int main(){
    Movie newMovie = { .Title = "TheProposal", .Genre = "Comedy", .Score = 7, .Year = 2009, .next = NULL, .parent = NULL, .child = NULL, .visited = 1};
    Trie *newTrie = initFromFile("mov.txt", newMovie);
    printTrie(newTrie);
    return 0;
}