#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "search.h"
#include <time.h>
/*
*Author: Vera Borvinski
*The dfs file performs a depth-first search
*/

int main(){
    int passes = 1;
    int pathCost1 = 0;
    int totalPathCost = 0;
    char searchFor[100];
    char goal[100];
    char dataFile[100];

    printf("input root node title\n");
    scanf("%s", searchFor);
    printf("input data file\n");
    scanf("%s", dataFile);
    Movie movieClicked = searchForMovie(dataFile, searchFor);    
    printf("input goal state title\n");
    scanf("%s", goal);

    clock_t t2;
    t2 = clock();
    clock_t t1;
    t1 = clock();
    Movie *current = depthFirstSearch(dataFile, movieClicked, goal)->tail;
    pathCost1 = current->pathCost;

    t1 = clock() - t1;
    double time_taken = ((double)t1)/CLOCKS_PER_SEC;
    while(strcmp(current->Title, goal) != 0){
        totalPathCost += current->pathCost;
        current->visited = 1;
        current = current->previous;
        passes++;
    }
    t2 = clock() - t2;
    double time_taken2 = ((double)t2)/CLOCKS_PER_SEC;

    printf("finding the goal state took %d passes\nfinding the first movie took %f seconds with a path cost of %d\nfinding the goal state took %f seconds with a path cost of %d", passes, time_taken, pathCost1, time_taken2, totalPathCost);
    
    return 0;
}

//creates frontier
Frontier *depthFirstSearch(char dataFile[100], Movie movieClicked, char goalState[100]){
    Trie *newTrie = initFromFile(dataFile, movieClicked);
    Frontier *frontier = createFrontier();
    Movie *currentGenre = newTrie->root.child;
    while(currentGenre != NULL){
        Movie *currentScore = currentGenre->child;
        while(currentScore != NULL){
            Movie *currentYear = currentScore->child;
            while(currentYear != NULL){
                Movie *current = currentYear->child;
                while(current != NULL){
                    if(current->visited == 0)
                    {
                        addToFrontierReverse(frontier, current);
                        if(strcmp(current->Title, goalState)==0){
                            break;
                        }
                    }
                    current = current->next;
                }
                currentYear = currentYear->next;
            }
            currentScore = currentScore->next;
        }
        currentGenre = currentGenre->next;
    }
    return frontier;
}
