#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>


int row1, col1, row2, col2;
int x, y;
int ** a = NULL;
int ** b = NULL;
int ** c = NULL;
char* str1 = NULL;
char* str2 = NULL;
char* str3 = NULL;

struct dim{
	int x, y;
}typedef pair;

int ** allocateMatrix(int row, int col) {
	//allocate contiguous block of memeory for all the array
	int * vals = (int *) malloc(row * col * sizeof(int));

	// allocate array of int* each row is represended by (int *)
	int ** ptrs = (int **) malloc(row * sizeof(int *));

	//adjust pointers at the start of each row
	for (int i = 0; i < row; i++)
		ptrs[i] = &vals[i * col];

	return ptrs;
}

void readArrays(){

	FILE * fp1 = fopen(str1, "r");;

	//read dimensions from file
	char sizes1[50];
	char sizes2[50];
	fscanf(fp1, "%s %s", sizes1, sizes2);
	row1 = atoi(sizes1 + 4);
	col1 = atoi(sizes2 + 4);

	a = allocateMatrix(row1, col1);

	for (int i = 0; i < row1; i++)
		for (int j = 0; j < col1; j++)
			fscanf(fp1, "%d", &a[i][j]);

	fclose(fp1);

	//read dimensions from file

	FILE * fp2 = fopen(str2, "r");

	fscanf(fp2, "%s %s", sizes1, sizes2);
	row2 = atoi(sizes1 + 4);
	col2 = atoi(sizes2 + 4);

	if (col1 != row2){
		printf("wrong dimensions\n");
		exit(-1);
	}

	b = allocateMatrix(row2, col2);

	for (int i = 0; i < row2; i++)
		for (int j = 0; j < col2; j++)
			fscanf(fp2, "%d", &b[i][j]);

	fclose(fp2);

	c = allocateMatrix(row1, col2);
}

void writeAns(){
	//writing answer to answer file
	FILE * fp3 = fopen(str3, "w");

	for (int i = 0; i < row1; i++){

		for (int j = 0; j < col2 - 1; j++)
			fprintf(fp3 , "%d ", c[i][j]);

		fprintf(fp3 , "%d\n", c[i][col2 - 1]);
	}
	fclose(fp3);
}


void* oneThreadedCode(void * arg){
	for (int i = 0; i < row1; i++)
		for (int j = 0; j < col2; j++)
			c[i][j] = 0;

	for (int i = 0; i < row1; i++)
		for (int j = 0; j < col2; j++)
			for (int k = 0; k < col1; k++)
				c[i][j] += a[i][k] *b[k][j];
	return NULL;
}

void* rowThreadedCode(void * arg){
	int wantedRow = (long) arg;
	for (int i = 0; i < col2; i++)
		c[wantedRow][i] = 0;

	for (int j = 0; j < col2; j++)
		for (int k = 0; k < col1; k++)
			c[wantedRow][j] += a[wantedRow][k] * b[k][j];

	return NULL;
}

void* elementThreadedCode(void * arg){
	int wantedRow = x;
	int wantedCol = y;

	c[wantedRow][wantedCol] = 0;

	for (int k = 0; k < col1; k++)
		c[wantedRow][wantedCol] += a[wantedRow][k] * b[k][wantedCol];

	return NULL;
}

void oneThreadMode(){
	pthread_t id;

	//create only one thread to solve
	int rc = pthread_create(&id, NULL, oneThreadedCode, NULL);
	if (rc){
		printf("error, return code is %d", rc);
		exit(-1);
	}
}

void rowThreadMode(){
	int numOfThreads = row1;
	pthread_t threadsIds[numOfThreads];

	// loop over rows indeces and solve each row in thread
	for (long i = 0; i < numOfThreads; i++){
		int rc = pthread_create(&threadsIds[i], NULL, rowThreadedCode, (void *)i);
		if (rc){
			printf("error, return code is %d", rc);
			exit(-1);
		}
	}

	for (int i = 0; i < numOfThreads; i++)
		pthread_join(threadsIds[i], NULL);
}

void elementThreadMode(){
	int numOfThreads = row1 * col2;

	pthread_t threadsIds[numOfThreads];


	for (long i = 0; i < row1; i++)
		for (long j = 0; j < col2; j++){
			x = i;
			y = j;
			int rc = pthread_create(&threadsIds[i], NULL, elementThreadedCode, NULL);

			if (rc){
				printf("error, return code is %d", rc);
				exit(-1);
			}
		}

	for (long i = 0; i < numOfThreads; i++){
		int val = pthread_join(threadsIds[i], NULL);

		if (val){
			printf("error in joining threads pthread_join return value is %d \n", val);
			return;
		}
	}
}


int main(int argc, char *argv[]) {
	if (argc == 4){
		//if user entered names of files
		str1 = argv[1];
		str2 = argv[2];
		str3 = argv[3];
		char word[20] = ".txt";

		if (!strstr(str1, ".txt"))
			strncat(str1, word, 5);
		if (!strstr(str2, ".txt"))
			strncat(str2, word, 5);
		if (!strstr(str3, ".txt"))
			strncat(str3, word, 5);
	}
	else {
		//default files names
		str1 = malloc(sizeof(char *));
		str2 = malloc(sizeof(char *));
		str3 = malloc(sizeof(char *));

		strcpy(str1,"a.txt");
		strcpy(str2,"b.txt");
		strcpy(str3,"c.txt");
	}

	readArrays();

	free(str1);
	free(str2);


	struct timeval stop1, start1;


	gettimeofday(&start1, NULL); //start checking time
	oneThreadMode();
	gettimeofday(&stop1, NULL); //end checking time
	printf("number of threads is %d in addition to main thread\n", 1);
	printf("Seconds taken %lu\n", stop1.tv_sec - start1.tv_sec);
	printf("Microseconds taken: %lu\n", stop1.tv_usec - start1.tv_usec);


	gettimeofday(&start1, NULL);
	rowThreadMode();
	gettimeofday(&stop1, NULL);
	printf("number of threads is %d in addition to main thread\n", row1);
	printf("Seconds taken %lu\n", stop1.tv_sec - start1.tv_sec);
	printf("Microseconds taken: %lu\n", stop1.tv_usec - start1.tv_usec);


	gettimeofday(&start1, NULL);
	elementThreadMode();
	gettimeofday(&stop1, NULL);
	printf("number of threads is %d in addition to main thread\n", row1 * col2);
	printf("Seconds taken %lu\n", stop1.tv_sec - start1.tv_sec);
	printf("Microseconds taken: %lu\n", stop1.tv_usec - start1.tv_usec);

	//writing answer to the file
	writeAns();

	free(str3);
	free(a);
	free(b);
	free(c);
}

