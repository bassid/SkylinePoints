
/* COMP10002 Assignment 2: Named-entity recognition (NER)

   Name: Danish Bassi 	Student ID: 867811	 Last updated: 24/05/17
   
   This C code is a solution to the NER process, where an alphabetically sorted
   dictionary and a sentence is input and the output is the sentence's words 
   labelled depending on their probabilties of being first names, last names or
   non-names. 
*/






/* Import headers/libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Stage numbers */
#define STAGE_ONE 1
#define STAGE_TWO 2
#define STAGE_THREE 3
#define STAGE_FOUR 4
#define STAGE_FIVE 5

/* Constants */
#define MAXLETTERS 30
#define MAXWORDS 100

/* Structure for a word */
typedef struct name {
	char word[MAXLETTERS+1];
	int first_name, last_name, non_name;
	struct name *next;
} name_t;
		


/* Function prototypes */
void stage1(name_t *dictionary);
void stage2(name_t *dictionary, int num_words);
name_t *stage3(name_t *dictionary, name_t *head);
void stage4(name_t *dictionary, int num_words, name_t *head);

void print_stage_heading(int stage);
void populate_dict(name_t *dictionary);
name_t *add_word(name_t *previous_word, char *word);
int binary_search(name_t *dictionary, char *item, int num_words);






/* Main program binds it all together */
int main(int argc, char *argv[])
{			
	// Storage for the input data
	name_t dictionary[MAXWORDS];
	name_t *head = NULL;
	
	// Call function to read in the data and store it
	populate_dict(dictionary);
	
	// Calculates the total number of names that are stored
	int num_words = 0;
	int char_zero = 0;
	while(dictionary[num_words].word[char_zero] != '\0')
	{
		num_words++;
	}
	
	
	// Call stage 1
	stage1(dictionary);
	
	// Call stage 2
	stage2(dictionary, num_words);
	
	// Call stage 3
	head = stage3(dictionary, head);
	
	// Call stage 4
	stage4(dictionary, num_words, head);
	
	// Return 0 to exit the program successfully
	return 0;
}




/* Print out a stage heading */
void print_stage_heading(int stage)
{
	// Prints 25 '=' characters on each side of the the stage heading
	int i;
	int num_of_equal_signs = 50;
	for(i = 0; i < num_of_equal_signs; i++)
	{
		if(i == (num_of_equal_signs/2))
		{
			printf("Stage %d", stage);
		}
		printf("=");	
	}
	printf("\n");
}




/* Read in the data and store it in the dictionary[] array */
void populate_dict(name_t *dictionary)
{
	char word[MAXLETTERS+1]; // +1 to account for the null byte
	char c;
	int first_name, last_name, non_name;
	int i = 0;
	
	/* Get the first character of the input file. Use while loop to check if 
	   char is equal to '#'. If it is, scan the content for the word and store 
	   it in the dictionary[] array
	   */
	c = getchar();
	while(c != '%')
	{
		if(c == '#')
		{
			scanf("%s", word);
			strcpy(dictionary[i].word, word);
		
			scanf("%d", &first_name);
			dictionary[i].first_name = first_name;
		
			scanf("%d", &last_name);
			dictionary[i].last_name = last_name;
		
			scanf("%d", &non_name);
			dictionary[i].non_name = non_name;
			
			i++;	
		}
		c = getchar();
	}
}	



/* Adds a word struct to the end of the linked list 

   ( Function skeleton from YouTube video by Bluefever Software. 
     URL: https://www.youtube.com/watch?v=SPHcwzn6Jds )   
*/
name_t *add_word(name_t *previous_word, char *word)
{
	name_t *new_word = malloc(sizeof(name_t));
	
	strcpy(new_word->word, word);
	new_word->next = NULL;
	
	/* Checks if the input struct is the first in the list. If it isn't, then
	   assigns the memory address of the new_word struct to the *next pointer
	   of the previous_word struct 
	*/
	if(previous_word != NULL)
	{
		previous_word->next = new_word;	
	}
	
	return new_word;
}




/* Performs binary search for a word to check if it is in the dictionary[] 
   array. If it is, then returns the index of the word, otherwise returns -1 

   ( Binary search function from COMP10002 Semester 1 2017 Lecture 5 ) 
*/
int binary_search(name_t *dictionary, char *item, int num_words)
{
	int start = 0;
	int end = num_words;
	int middle = (start + end) / 2;
	
	while (start < end)
	{		
		middle = (start + end) / 2;
		
		if(strcmp(dictionary[middle].word, item) > 0)
		{
			end = middle;
		}
		else if(strcmp(dictionary[middle].word, item) < 0)
		{
			start = middle+1;
		}
		else
		{
			return middle;
		}
	}
	
	return -1;
}





/* Stage 1 - Print out the first word in the dictionary and it's probabilities 
*/
void stage1(name_t *dictionary)
{
	print_stage_heading(STAGE_ONE);	
	
	int word_zero = 0;
	printf("Word %d: %s\n", word_zero, dictionary[word_zero].word);
	printf("Label probabilities: %d%% %d%% %d%%\n\n",
		dictionary[word_zero].first_name,
		dictionary[word_zero].last_name,
		dictionary[word_zero].non_name);	
}





/* Stage 2 - Print the total num of words stored and calculate the average 
   number of characters 
*/
void stage2(name_t *dictionary, int num_words)
{
	print_stage_heading(STAGE_TWO);	
	
	int i, j, char_count;
	double average, total;
	char c;
	
	
	printf("Number of words: %d\n", num_words);
	
	// For loop counts the total amount of characters in the dictionary
	for(i = 0; i < num_words; i++)
	{
		j = 0;
		char_count = 0;
		c = dictionary[i].word[j];
		while(c != '\0')
		{
			c = dictionary[i].word[j];
			j++;
			++char_count;	
		}
		--char_count;

		total += char_count;	
	}
	
	average = total / num_words;
	printf("Average number of characters per word: %.2lf\n\n", average);
}





/* Stage 3 - Creates a linked list to store the words from the input sentence
   and prints the words
*/
name_t *stage3(name_t *dictionary, name_t *head)
{
	print_stage_heading(STAGE_THREE);	
	
	
	name_t *foot = NULL;
	char word[MAXLETTERS+1]; // +1 to account for the null byte
	
	scanf("%s", word);	
	
	// Creates linked list 
	while(scanf("%s", word) != EOF)
	{
		if (head == NULL)
		{
			head = add_word(NULL, word);
			foot = head;
		}
		else
		{
			foot = add_word(foot, word);	
		}
	}
	
	
	name_t *current_word = head;
	
	// Prints the sentence words on a new line using the linked list
	while(current_word != NULL)
	{
		printf("%s\n", current_word->word);
		current_word = current_word->next;
	}
	
	printf("\n");
	
	return head;
}





/* Stage 4 - Prints the sentence words from the linked list created in stage 3
   and labels them according to their probabilities
*/
void stage4(name_t *dictionary, int num_words, name_t *head)
{
	print_stage_heading(STAGE_FOUR);
	
	
	name_t *sentence_word ;
	int index;
	int first_index = 0;
	
	// Use while loop to check all nodes in the linked list
	sentence_word = head;
	while (sentence_word != NULL)
	{
		// Left justified formatting
		printf("%-32s", sentence_word->word);
		
		/* Binary search used to check if sentence_word is in the dictionary[]
	       array
		*/
		index = binary_search(dictionary, sentence_word->word, num_words);
		
		if(index >= first_index)
		{
			int first = dictionary[index].first_name;
			int last = dictionary[index].last_name;
			
			// If word is found, label according to it's probabilities
			if((first > first_index) && (last > first_index))
			{
				printf("%s, %s", "FIRST_NAME", "LAST_NAME");	
			}
			else if(first > first_index)
			{
				printf("%s", "FIRST_NAME");	
			}
			else if(last > first_index)
			{
				printf("%s", "LAST_NAME");
			}
					
			printf("\n");	
		}
		else
		{
			printf("%s\n", "NOT_NAME");	
		}
		
		sentence_word = sentence_word->next;
	}
	printf("\n");
}
