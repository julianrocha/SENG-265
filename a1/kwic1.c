#include <stdlib.h>
#include <stdio.h>
#include <string.h>			//string functions
#include <ctype.h> 			//toupper()

const int NUM_LINES = 100;		//at most input has 100 lines
const int NUM_EXCLUSIONS = 20;	//at most input has 20 exclusion words
const int LINE_LENGTH = 71;		//at most input line is 69 chars long (+ '\n' + '\0')
const int EXCLUSION_LENGTH = 22;//at most input exclusion word is 20 chars long (+ '\n' + '\0')

typedef struct { 			//~Struct to store keywords along with other valuable info
	char word[70]; 			//~Keyword (at most 69 chars)
	int charLocation[100];  //~Location of first letter in line (second index in lines[][])
							// stored in an array in case keyword is repeated for all 100 lines
	int lineLocation[100];  //~Line number where keyword is found (first index in lines[][])
							// stored in an array in case keyword is repeated for all 100 lines
	int numOccurences;		//~Number of times a keyword is repeated (at most 100 times)
} indexedKeyword;

//This function stores the exlusion words and lines from stdin into arrays
void readInput(char exclusions[][EXCLUSION_LENGTH], char lines[][LINE_LENGTH], int *numExclusions, int *numLines){

	char buffer[LINE_LENGTH];	//~Temporarily store input line
	int exclusionCounter = 0;	//~Keep track of place in exclusions[][]
	int linesCounter = 0;		//~Keep track of place in lines[][]
	
	fgets(buffer, LINE_LENGTH, stdin);	//Discard input file version number
	fgets(buffer, LINE_LENGTH, stdin);	//Discard first colons in input file

	while(fgets(buffer, EXCLUSION_LENGTH, stdin) > 0){									//loop to read exclusion words
		if(buffer[0] == '\n')															//skip line if blank
			continue;
		if(strcmp(buffer, "::\n") == 0)													//break loop once colons reached
			break;
		strncpy(exclusions[exclusionCounter], buffer, EXCLUSION_LENGTH);				//add exclusion word
		exclusions[exclusionCounter][strlen(exclusions[exclusionCounter]) - 1] = '\0';	//replalce '\n' with NULL
		exclusionCounter++;
	}
	while(fgets(buffer, LINE_LENGTH, stdin) > 0){										//loop to read lines
		if(buffer[0] == '\n')															//skip line if blank
			continue;
		strncpy(lines[linesCounter], buffer, LINE_LENGTH);								//add line
		lines[linesCounter][strlen(lines[linesCounter]) - 1] = '\0';					//replace '\n' with NULL
		linesCounter++;
	}

	*numExclusions = exclusionCounter;	//pass back numExclusions to main
	*numLines = linesCounter;			//pass back numLines to main
}

//This function stores keywords from lines[][] into an array
void grabKeywords(indexedKeyword keywords[], char exclusions[][EXCLUSION_LENGTH], 
	char linesToBeTokenized[][LINE_LENGTH], int numExclusions, int numLines, int *numKeywords){
	
	for(int i = 0; i < numLines; i++){						//loop through lines (first index of lines[][])
	
		char *word = strtok(linesToBeTokenized[i], " ");	//~word points to a single word from a line at a time
		int charNum = 0;									//~keeps track of char location in line (second index of lines[][])
		
		while(word != NULL){								//tokenize all words in a line
			int j;											//~general purpose loop counter
			for(j = 0; j < numExclusions; j++){				
				if(strcmp(word, exclusions[j]) == 0)
					break;
			}
			if(j != numExclusions){							//if word was found in exclusions[][], move to next word in line
				charNum+= strlen(word) + 1;
				word = strtok(NULL, " ");
				continue;
			}

			for(j = 0; j < *numKeywords; j++){
				if(strcmp(word, keywords[j].word) == 0)
					break;
			}
			if(j != *numKeywords){							//if word is a repeat keyword, update info in struct indexedKeyword
				keywords[j].charLocation[keywords[j].numOccurences] = charNum;
				keywords[j].lineLocation[keywords[j].numOccurences] = i;
				keywords[j].numOccurences++;
				charNum+= strlen(word) + 1;
				word = strtok(NULL, " ");
				continue;
			}
															//if word is a new keyword, add it to keywords[]
			strncpy(keywords[*numKeywords].word, word, LINE_LENGTH - 1);
			keywords[*numKeywords].charLocation[keywords[*numKeywords].numOccurences] = charNum;
			keywords[*numKeywords].lineLocation[0] = i;
			keywords[*numKeywords].numOccurences = 1;
			*numKeywords = *numKeywords + 1;
			charNum+= strlen(word) + 1;
			word = strtok(NULL, " ");
		}
	}
}

//This function is used by qsort to sort an array of struct indexedKeyword
int compareFunction(const void * a, const void * b){
	indexedKeyword *keyA = (indexedKeyword *)a;
	indexedKeyword *keyB = (indexedKeyword *)b;
	return strcmp(keyA->word, keyB->word);
}

//This function prints all keywords capatalized in the lines they were read from to stdout
void outputKeywords(indexedKeyword keywords[], int numKeywords, char lines[][LINE_LENGTH]){
	int i;
	for(i = 0; i < numKeywords; i++){								//loop through keywords[] (i)
		int y;
		for(y = 0; y < keywords[i].numOccurences; y++){				//loop through occurences of a keyword (y)
			int z;
			for(z = 0; z < keywords[i].charLocation[y]; z++){		//loop through chars before keyword, in same line (z)
				printf("%c", lines[keywords[i].lineLocation[y]][z]);
			}
			for(z = 0; z < strlen(keywords[i].word); z++){			//loop through chars in keyword (z)
				printf("%c", toupper(keywords[i].word[z]));			//print keyword in uppercase
			}
			for(z = keywords[i].charLocation[y] + strlen(keywords[i].word); z < LINE_LENGTH; z++){//loop through chars after keyword (z)
				if(lines[keywords[i].lineLocation[y]][z] == '\0')	//stop printing once end of line is reached
					break;
				printf("%c", lines[keywords[i].lineLocation[y]][z]);
			}
			printf("\n");											//move to next line for next occurence/keyword
		}
	}
}


int main(int argc, char* argv[]){
	char exclusions[NUM_EXCLUSIONS][EXCLUSION_LENGTH];	//~Store exclusion words
														// at most, 20 words each of length 20 chars
	char lines[NUM_LINES][LINE_LENGTH];					//~Store lines
														// at most 100 lines, each of length 69 chars
	char linesToBeTokenized[NUM_LINES][LINE_LENGTH];	//~Copy of lines[][] so that they can be tokenized in grabKeywords()
	int numExclusions = 0;								//~Number of exclusion words
	int numLines = 0;									//~Number of lines
	int numKeywords = 0;								//~Number of keywords (at most 100) 
	indexedKeyword keywords[NUM_LINES];					//~Store indexedKeywords
														// at most 100 words, at most 69 chars each
	readInput(exclusions, lines, &numExclusions, &numLines);

	for(int i = 0; i < numLines; i++){		//Copy lines[][] to linesToBeTokenized[][]
		strncpy(linesToBeTokenized[i], lines[i], LINE_LENGTH);
	}

	grabKeywords(keywords, exclusions, linesToBeTokenized, numExclusions, numLines, &numKeywords);
	qsort(keywords, numKeywords, sizeof(indexedKeyword), compareFunction);
	outputKeywords(keywords, numKeywords, lines);
}