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
    struct movie* next;
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
Movie depthFirstSearch(char dataFile[100], Movie movieClicked);
Movie aStarSearch(char dataFile[100], Movie movieClicked);
Trie *constructTrie(Movie lastMovieClicked);
void addToTrie(Trie *trie, char t[100],  char g[100], int s, int y);
void printTrie(Trie *trie);
void printMovieInfo(Movie *movie);
Trie *initFromFile(char dataFile[100], Movie root);
Movie searchForMovie(char dataFile[100], char title[100]);
Frontier *createFrontier();
void addToFrontier(Frontier *frontier, Movie *movie);

//Allocates memory for tree
Trie *constructTrie(Movie lastMovieClicked){
    Trie *trie = (Trie*) malloc(sizeof(Trie));
    trie->root = lastMovieClicked;
    trie->root.child = NULL;
    trie->root.parent = NULL;
    trie->root.next = NULL;
    return trie;
}

//Creates new movie and adds to the frontier
void addToTrie(Trie *trie, char t[100],  char g[100], int s, int y){
    Movie *newMovie = (Movie*) malloc(sizeof(Movie));
    strcpy(newMovie->Title, t);
    strcpy(newMovie->Genre, g);
    newMovie->Score = s;
    newMovie->Year = y;
    newMovie->next = NULL;
    newMovie->child = NULL;
    newMovie->visited = 0;
    
    if (trie->root.child == NULL){
        trie->root.child = newMovie;
        trie->root.child->parent = &trie->root;

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
                            }
                            else{
                                Movie *current = currentYear->child;
                                while(current->next != NULL){
                                    current = current->next;
                                }
                                current->next = (Movie*) malloc(sizeof(Movie));
                                current->next = newMovie;
                                current->next->parent = current->parent;
                            }
                        }
                        else if(currentYear->next == NULL){
                            currentYear->next = (Movie*) malloc(sizeof(Movie));
                            currentYear->next = newMovie;
                            addToTrie(trie, newMovie->Title,  newMovie->Genre, newMovie->Score, newMovie->Year);
                            currentYear->next->parent = currentYear->parent;
                        }
                    }
                }
                else if(currentScore->next == NULL){
                    currentScore->next = (Movie*) malloc(sizeof(Movie));
                    currentScore->next = newMovie;
                    addToTrie(trie, newMovie->Title,  newMovie->Genre, newMovie->Score, newMovie->Year);
                    addToTrie(trie, newMovie->Title,  newMovie->Genre, newMovie->Score, newMovie->Year);
                    currentScore->next->parent = currentScore->parent;
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
            if(strcmp(title, film) != 0){
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
