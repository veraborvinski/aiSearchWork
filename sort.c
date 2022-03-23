#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void selectionSort(int a[], int n);
void insertionSort(int a[], int n);
void bubbleSort(int a[], int n);
void printArray(int a[], int n);
int sortStyle = 2;
int showSteps = 1;
int arrayType = 0;

int main(){
	int size;
	printf("select size\n");
	scanf("%d", &size);

	printf("select sort style (selection = 0, insertion = 1, bubble = 3)\n");
	scanf("%d", &sortStyle);

	printf("show steps? (no = 0, yes = 1)\n");
	scanf("%d", &showSteps);

	printf("type of array? (increasing = 0, decreasing = 1, random = 2, manual = 3)\n");
	scanf("%d", &arrayType);

	int a[size];
	if(arrayType == 0){
		for(int i = 0; i < size; i++){
			a[i] = i;
		}
	}
	else if(arrayType == 1){
		for(int i = size-1; i > -1; i--){
			a[i] = i;
		}
	}
	else if(arrayType == 2){
		for(int i = 0; i < size; i++){
			a[i] = rand();
		}
	}
	else{
		for(int i = 0; i < size; i++){
			scanf("%d", &a[i]);
		}
	}
	if(sortStyle == 0){
		clock_t t1;
	    t1 = clock();
		selectionSort(a, size);
		t1 = clock() - t1;
	    double time_taken = ((double)t1)/CLOCKS_PER_SEC; // calculate the elapsed time
	    printf("The program took %f seconds to execute", time_taken);
    }
    else if(sortStyle == 1){
		clock_t t1;
	    t1 = clock();
		insertionSort(a, size);
		t1 = clock() - t1;
	    double time_taken = ((double)t1)/CLOCKS_PER_SEC; // calculate the elapsed time
	    printf("The program took %f seconds to execute", time_taken);

    }
    else{
    	clock_t t1;
	    t1 = clock();
		insertionSort(a, size);
		t1 = clock() - t1;
	    double time_taken = ((double)t1)/CLOCKS_PER_SEC; // calculate the elapsed time
	    printf("The program took %f seconds to execute", time_taken);
    }

	return 0;
}

void selectionSort(int a[], int n){
	for(int i = 0; i < n; i++){
		int j = i;
		for(int k = (i + 1); k <= n; k++){
			if(a[k] < a[j]){
				j = k;
			}
		}
		int t = a[i];
		a[i] = a[j];
		a[j] = t;
		if(showSteps == 1) printArray(a, n);
	}
	if(showSteps == 0) printArray(a, n);
}

void insertionSort(int a[], int n){
	for(int i = 2; i < n; i++){
		int j = i;
		while(j > 1 && a[j] < a[j-1]){
			int t = a[j];
			a[j] = a[j-1];
			a[j-1] = t;
			j--;
		}
		if(showSteps == 1) printArray(a, n);
	}
	if(showSteps == 0) printArray(a, n);
}

//this one doesn't work properly
void bubbleSort(int a[], int n){
	for(int i = 0; i < n-1; i++){

		for(int j = 0; j < (n-i-1); j++){

 			if(a[j] > a[j + 1]){

				int t = a[j]; 
				a[j] = a[j + 1];
				a[j + 1] = t;
 			}
			if(showSteps == 1) printArray(a, n);
		}
	}
	if(showSteps == 0) printArray(a, n);
}

void printArray(int a[], int n){
    int i;
    for (i=0; i < n; i++)
        printf("%d ", a[i]);
    printf("\n");
}