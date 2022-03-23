#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "search.h"
/*
*Author: Vera Borvinski
*The frontier file is used as tester for the frontier struct
*/
Frontier *pathCostFunction(Frontier *frontier);

Frontier *heuristicFunction(Frontier *frontier, Movie *root);

int main(){
	Movie root = { .Title = "TheProposal", .Genre = "Comedy", .Score = 7, .Year = 2009, .pathCost = 1, .next = NULL, .previous = NULL, .parent = NULL, .child = NULL, .visited = 0};
    
    Movie newMovie1 = { .Title = "EHH", .Genre = "Comedy", .Score = 7, .Year = 2009, .pathCost = 1, .next = NULL, .previous = NULL, .parent = NULL, .child = NULL, .visited = 0};
    Movie newMovie2 = { .Title = "vrrom", .Genre = "Comedy", .Score = 7, .Year = 2008, .pathCost = 3, .next = NULL, .previous = NULL, .parent = NULL, .child = NULL, .visited = 0};
    Movie newMovie3 = { .Title = "xx", .Genre = "Comedy", .Score = 7, .Year = 2009, .pathCost = 2, .next = NULL, .previous = NULL, .parent = NULL, .child = NULL, .visited = 0};
    
    Movie newMovie4 = { .Title = "EHraf", .Genre = "Comedy", .Score = 5, .Year = 2008, .pathCost = 1, .next = NULL, .previous = NULL, .parent = NULL, .child = NULL, .visited = 0};
    Movie newMovie5 = { .Title = "vrroaerfd", .Genre = "Rom", .Score = 6, .Year = 2008, .pathCost = 3, .next = NULL, .previous = NULL, .parent = NULL, .child = NULL, .visited = 0};
    Movie newMovie6 = { .Title = "xfvx", .Genre = "Cf", .Score = 7, .Year = 2001, .pathCost = 2, .next = NULL, .previous = NULL, .parent = NULL, .child = NULL, .visited = 0};
    
    Frontier *frontier = createFrontier();

    addToFrontier(frontier, &newMovie1);
    addToFrontier(frontier, &newMovie2);
    addToFrontier(frontier, &newMovie3);
    addToFrontier(frontier, &newMovie4);
    addToFrontier(frontier, &newMovie5);
    addToFrontier(frontier, &newMovie6);

    printFrontier(heuristicFunction(pathCostFunction(frontier), &root));

    return 0;
}