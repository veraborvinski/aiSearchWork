#include <stdio.h>
#include <string.h>
/*
*Author: Vera Borvinski
*The header file for this project, contains trie functionality
*/

//The movie struct provides structure for movies, treated as nodes in a trie
typedef struct movie{
    char Title[100];
    char Genre[100];
    int Score;
    int Year;
    int pathCost;
    struct movie* next;
    struct movie* previous;
    struct movie* parent;
    struct movie* child;
    int visited;
} Movie;

typedef struct frontier{
    Movie *head;
    Movie *tail;
} Frontier;

//The trie struct is used to create the structure of the movie trie
typedef struct trie{
	Movie root;
} Trie;

//Initiating functions
Frontier *breadthFirstSearch(char dataFile[100], Movie movieClicked);
Frontier *depthFirstSearch(char dataFile[100], Movie movieClicked);
Frontier *aStarSearch(char dataFile[100], Movie movieClicked);
Trie *constructTrie(Movie lastMovieClicked);
void addToTrie(Trie *trie, char t[100],  char g[100], int s, int y);
void printTrie(Trie *trie);
void printMovieInfo(Movie *movie);
Trie *initFromFile(char dataFile[100], Movie root);
Movie searchForMovie(char dataFile[100], char title[100]);
Frontier *createFrontier();
void addToFrontier(Frontier *frontier, Movie *movie);
void printFrontier(Frontier *frontier);
void printFrontierReverse(Frontier *frontier);
Movie *movcpy(Movie *movie1, Movie *movie2);
Frontier *heuristicFunction(Frontier *frontier, Movie *root);
Frontier *pathCostFunction(Frontier *frontier);
Frontier *aStarQueueConstructor(char dataFile[100], Movie movieClicked);

//Allocates memory for tree
Trie *constructTrie(Movie lastMovieClicked){
    Trie *trie = (Trie*) malloc(sizeof(Trie));
    trie->root = lastMovieClicked;
    trie->root.child = NULL;
    trie->root.parent = NULL;
    trie->root.next = NULL;
    trie->root.previous = NULL;
    trie->root.visited = 1;
    return trie;
}

//Creates new movie and adds to the frontier
void addToTrie(Trie *trie, char t[100],  char g[100], int s, int y){
    Movie *newMovie = (Movie*) malloc(sizeof(Movie));
    strcpy(newMovie->Title, t);
    strcpy(newMovie->Genre, g);
    newMovie->Score = s;
    newMovie->Year = y;
    newMovie->pathCost = 0;
    newMovie->next = NULL;
    newMovie->previous = NULL;
    newMovie->child = NULL;
    newMovie->visited = 0;
    
    if (trie->root.child == NULL){
        trie->root.child = newMovie;
        trie->root.child->parent = &trie->root;
        trie->root.child->pathCost = 1;

        addToTrie(trie, newMovie->Title,  newMovie->Genre, newMovie->Score, newMovie->Year);
        addToTrie(trie, newMovie->Title,  newMovie->Genre, newMovie->Score, newMovie->Year);
        addToTrie(trie, newMovie->Title,  newMovie->Genre, newMovie->Score, newMovie->Year);
    }
    else{  
        Movie *currentGenre = (Movie*) malloc(sizeof(Movie));
        currentGenre = trie->root.child;
                
        while(currentGenre->next != NULL){
            if(strcmp(currentGenre->Genre, g) == 0){
                break;
            }
            currentGenre = currentGenre->next;

        }             
        if(strcmp(currentGenre->Genre, g) == 0){
            if(currentGenre->child == NULL){
                currentGenre->child = (Movie*) malloc(sizeof(Movie));
                currentGenre->child = newMovie;
                currentGenre->child->parent = currentGenre;
                currentGenre->child->pathCost = 2;
            }
            else{
                Movie *currentScore = currentGenre->child;
        
                while(currentScore->next != NULL){

                    if(currentScore->Score == s){
                        break;
                    }
                    currentScore = currentScore->next;
                }
                
                if(currentScore->Score == s){
                    if(currentScore->child == NULL){
                        currentScore->child = (Movie*) malloc(sizeof(Movie));
                        currentScore->child = newMovie;
                        currentScore->child->parent = currentScore;
                        currentScore->child->pathCost = 3;
                    }
                    else{
                        Movie *currentYear = currentScore->child;
                      
                        while(currentYear->next != NULL){

                            if(currentYear->Year == y){
                                break;
                            } 
                            currentYear = currentYear->next;
                        }   
                        
                        if(currentYear->Year == y){
                            if(currentYear->child == NULL){
                                currentYear->child = (Movie*) malloc(sizeof(Movie));
                                currentYear->child = newMovie;
                                currentYear->child->parent = currentYear;
                                currentYear->child->pathCost = 4;
                            }
                            else{
                                Movie *current = currentYear->child;
                                while(current->next != NULL){
                                    current = current->next;
                                }
                                current->next = (Movie*) malloc(sizeof(Movie));
                                current->next = newMovie;
                                current->next->parent = current->parent;
                                current->next->pathCost = 4;
                            }
                        }
                        else if(currentYear->next == NULL){
                            currentYear->next = (Movie*) malloc(sizeof(Movie));
                            currentYear->next = newMovie;
                            addToTrie(trie, newMovie->Title,  newMovie->Genre, newMovie->Score, newMovie->Year);
                            currentYear->next->parent = currentYear->parent;
                            currentYear->next->pathCost = 3;
                        }
                    }
                }
                else if(currentScore->next == NULL){
                    currentScore->next = (Movie*) malloc(sizeof(Movie));
                    currentScore->next = newMovie;
                    addToTrie(trie, newMovie->Title,  newMovie->Genre, newMovie->Score, newMovie->Year);
                    addToTrie(trie, newMovie->Title,  newMovie->Genre, newMovie->Score, newMovie->Year);
                    currentScore->next->parent = currentScore->parent;
                    currentScore->next->pathCost = 2;
                }
            }
        }
        else if(currentGenre->next == NULL){
            currentGenre->next = (Movie*) malloc(sizeof(Movie));
            currentGenre->next = newMovie;
            addToTrie(trie, newMovie->Title,  newMovie->Genre, newMovie->Score, newMovie->Year);
            addToTrie(trie, newMovie->Title,  newMovie->Genre, newMovie->Score, newMovie->Year);
            addToTrie(trie, newMovie->Title,  newMovie->Genre, newMovie->Score, newMovie->Year);
            currentGenre->next->parent = &trie->root;
            currentGenre->next->pathCost = 1;
        }    
    }
}

//Prints the fields of a movie that are useful for a viewer
void printMovieInfo(Movie *movie){
    printf("%s, %s, %d, %d\n", movie->Title, movie->Genre, movie->Score, movie->Year);
}

//Prints the entire trie with formating
void printTrie(Trie *trie){
    printf("The last movie you saw was :");
    printMovieInfo(&trie->root);
    Movie *currentGenre = trie->root.child;
    while(currentGenre != NULL){
        printf("\nGenre: %s\n", currentGenre->Genre);
        Movie *currentScore = currentGenre->child;
        while(currentScore != NULL){
            printf("\tScore: %d\n", currentScore->Score);
            Movie *currentYear = currentScore->child;
            while(currentYear != NULL){
                printf("\t \tYear: %d\n", currentYear->Year);
                Movie *current = currentYear->child;
                while(current != NULL){
                    if(current->visited == 0)
                    {
                        printf("\t \t \t");
                        printMovieInfo(current);
                        current = current->next;
                    }
                }
                currentYear = currentYear->next;
            }
            currentScore = currentScore->next;
        }
        currentGenre = currentGenre->next;
    }
}

//Fills the trie with data from a text file
Trie *initFromFile(char dataFile[100], Movie root){
    Trie *newTrie = constructTrie(root);
    FILE *fp = fopen( dataFile, "r" );
    char film[100], genre[100];
    int score, year;
    if (fp!=NULL) {
        while ((fscanf(fp, "%s , %s , %d , %d", film, genre, &score, &year)>0))
        {
            if(strcmp(root.Title, film) != 0){
                addToTrie(newTrie, film, genre, score, year);
            }
        }
        fclose (fp);
    }
    return newTrie;
}

//searches for a movie in file by title
Movie searchForMovie(char dataFile[100], char title[100]){
    FILE *fp = fopen( dataFile, "r" );
    char film[100], genre[100];
    int score, year;
    Movie *found = (Movie*) malloc(sizeof(Movie));
    if (fp!=NULL) {
        while ((fscanf(fp, "%s , %s , %d , %d", film, genre, &score, &year)>0))
        {
            if(strcmp(title, film) == 0){
                strcpy(found->Title, film);
                strcpy(found->Genre, genre);
                found->Score = score;
                found->Year = year;
            }
        }
        fclose (fp);
    }
    return *found;
}

//initialises space for frontier
Frontier *createFrontier(){
    Frontier *frontier = (Frontier*) malloc(sizeof(Frontier));
    frontier->head = NULL;
    frontier->tail = NULL;
    return frontier;
}

//adds to frontier
void addToFrontier(Frontier *frontier, Movie *movie){
    if(movie->visited == 0)
    {
        if(frontier->head != NULL){
            frontier->tail->next = movie;
            frontier->tail = movie;
        }
        else{
            frontier->head = movie;
            frontier->tail = movie;
        }
    }
}

//adds to frontier
void addToFrontierReverse(Frontier *frontier, Movie *movie){
    if(movie->visited == 0)
    {
        if(frontier->head != NULL){
            movie->previous = frontier->tail;
            frontier->tail = movie;
        }
        else{            
            frontier->head = movie;
            frontier->tail = movie;
        }
    }
}

//copies movie data from one movie to another
Movie *movcpy(Movie *movie1, Movie *movie2){
    strcpy(movie1->Title, movie2->Title);
    strcpy(movie1->Genre, movie2->Genre);
    movie1->Score = movie2->Score;
    movie1->Year = movie2->Year;
    movie1->pathCost = movie2->pathCost;
    return movie1;
}

//prints frontier
void printFrontier(Frontier *frontier){
    Movie *current = frontier->head;
    while(current != NULL){
        printMovieInfo(current);
        current = current->next;
    }
}

//prints frontier for movies that have been added in reverse
void printFrontierReverse(Frontier *frontier){
    Movie *current = frontier->tail;
    while(current != NULL){
        printf("%s\n", current->Title);
        current = current->previous;
    }
}

//sorts by similarity to root
Frontier *heuristicFunction(Frontier *frontier, Movie *root){
    Frontier *sorted = createFrontier();
    Movie *current = frontier->head;
                   
    while(current != NULL){
        if(strcmp(current->Genre, root->Genre) == 0 && current->Score == root->Score && current->Year == root->Year && current->visited == 0){
            Movie *temp = (Movie*) malloc(sizeof(Movie));
            temp = movcpy(temp, current);
            addToFrontier(sorted, temp);
            current->visited = 1;
        }
        current = current->next;
    }

    current = frontier->head;
    while(strcmp(current->Genre, root->Genre) == 0 && current->Score == root->Score && current != NULL){
        if(current->visited == 0){
            Movie *temp = (Movie*) malloc(sizeof(Movie));
            temp = movcpy(temp, current);
            addToFrontier(sorted, temp);
            current->visited = 1;
        }
        current = current->next;
    }

    current = frontier->head;
    while(strcmp(current->Genre, root->Genre) == 0 && current != NULL){
        if(current->visited == 0){
            Movie *temp = (Movie*) malloc(sizeof(Movie));
            temp = movcpy(temp, current);
            addToFrontier(sorted, temp);
            current->visited = 1;
        }
        current = current->next;
    }

    current = frontier->head;
    while(current->Score == root->Score &&current != NULL){
        if(current->visited == 0){
            Movie *temp = (Movie*) malloc(sizeof(Movie));
            temp = movcpy(temp, current);
            addToFrontier(sorted, temp);
            current->visited = 1;
        }
        current = current->next;
    }

    current = frontier->head;
    while(current->Year == root->Year &&current != NULL){
        if(current->visited == 0){
            Movie *temp = (Movie*) malloc(sizeof(Movie));
            temp = movcpy(temp, current);
            addToFrontier(sorted, temp);
            current->visited = 1;
        }
        current = current->next;
    }

    current = frontier->head;
    while(current != NULL){
        if(current->visited == 0){
            Movie *temp = (Movie*) malloc(sizeof(Movie));
            temp = movcpy(temp, current);
            addToFrontier(sorted, temp);
            current->visited = 1;
        }
        current = current->next;
    }
    
    return sorted;
}

//always choses parent over child
Frontier *pathCostFunction(Frontier *frontier){
    int sorted;
    do
    {
        int swaps = 0;
        Movie *current = frontier->head;

        while(current != NULL &&current->next != NULL){
            Movie *currentNext = current->next;
            if(current->pathCost > currentNext->pathCost){
                Movie temp = { .Title = "", .Genre = "", .Score = 0, .Year = 0};
                temp = *movcpy(&temp, current);
                current = movcpy(current, currentNext);
                currentNext = movcpy(currentNext, &temp);

                swaps++;
            }
            current = current->next;
        }
        if(swaps == 0){
            sorted = 1;
        }
    }
    while(sorted == 0);
    return frontier;
}
