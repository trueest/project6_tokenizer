/*
* Name: Hunter Hawkins - U13938229
* Email: hunterhawkins@usf.edu 
* Assignment: Project 6: Task 1
* This C program will act as a tokenizer to convert an input of words into tokens. 
* It will achieve this by receiving a text file as input and a text file for output in the command-line.
* Then, it will read the input file, guaranteed to have at most 10,000 characters.
* Once read, it will use C library functions to tokenize the input, create a unique array of words from the input,
* sort the input in alphabetical order, and finally create corresponding numbers, or token values, for each unique word.
* Finally, it will output all of this information to a write file per requirements.
*/

// ADDING LIBRARIES
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Initializing our max character count per requirements.
#define MAX_LEN 10000

// Comparison function to be used for qsort. Because qsort takes void pointers as arguments, they will be casted to char.
int compare_words(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int main (int argc, char *argv[]) {

    // Verifying valid user input. If the amount of arguments is not valid, display error to user and quit program.
    if (argc != 3) {
        printf("Invalid number of arguments. 3 expected.");
        return 1;
    }

    // From the command line, open the first file name in read permissions.
    FILE *rFile = fopen(argv[1], "r");

    // From the command line, open the second file name in write permissions.
    FILE *wFile = fopen(argv[2], "w");

    // Verifying valid user file input name. If the file is not found, display file not found and quit program.
    if (rFile == NULL) {
        printf("File not found.");
        return 1;
    }

    /* Declare various string variables to hold our buffer text from our read file, 
    * our main text string array, then our delimiters for the strtok function and 
    * tokenized array for strtok.
    */
    char buffer[MAX_LEN + 1] = ""; // Our input buffer
    char text[MAX_LEN + 1] = ""; // Our array of text, concatenated by strcat and read line by line by fgets.
    char *delimiters = ". \n"; // Our delimiters that will be used for the first call of strtok.
    char *token; // Our tokenized text array.

    // Read the file line by line with fgets and adding to our buffer and text arrays.
    while (!feof(rFile) && !ferror (rFile)) {
        while(fgets(buffer, MAX_LEN, rFile)) {
            // Adding our text from our file/buffer to our text array.
            strcat(text, buffer);
        }
    } 

    // Initializing our array that will hold unique words.
    char *words[MAX_LEN + 1];
    // Initializing a word_count variable to hold the number of words and be referenced when creating token values for words.
    int word_count = 0;

    // Tokenizing our text with the declared delimiters.
    token = strtok(text, delimiters);
    // While loop to continue tokenizing until a NULL pointer is returned.
    while(token != NULL) {
        // Unique flag for the current word.
        int unique = 1;
        // If the word is already in our words array, then it is not unique and will not be added.
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                unique = 0;
                break;
            }
        }
        // If the word is unique, it will be added to the words array.
        if (unique) {

            // Use malloc to allocate memory for the unique word + null character due to the use of strtok.
            words[word_count] = malloc(strlen(token) + 1);

            // Copy the unique word from token into our words array at the word_count index.
            strcpy(words[word_count], token);

            // Increment word count.
            word_count++;
        }
        token = strtok(NULL, delimiters);
    }

    // Using the library qsort function with our compare_words function to alphabetically sort our unique words.
    qsort(words, word_count, sizeof(char *), compare_words);

    // Write each word to the output file.
    fprintf(wFile, "%d\n", word_count);
    for (int i = 0; i < word_count; i++) {
        fprintf(wFile, "%s\n", words[i]);
    }

    // In order to create an index, or integer value for each word, a word index array will be used.
    int word_indices[MAX_LEN];

    for (int i = 0; i < word_count; i++) {

        // Since our token values will begin from 1, begin assigning values in our word_indices array at 1.
        word_indices[i] = i + 1;
    }

    // Reset the file stream position to the beginning of the file to begin reading the input again.
    rewind(rFile);

    // Reset our text array.
    text[0] = '\0';

    // Read the file line by line with fgets and adding to our buffer and text arrays.
    while (fgets(buffer, MAX_LEN, rFile)) {
        strcat(text, buffer);
    }

    // Process each sentence by finding periods
    char *sentence_start = text;
    char *period_position;
    while ((period_position = strchr(sentence_start, '.')) != NULL) {
        // Temporarily end the sentence at the period
        *period_position = '\0';

        // Tokenize words within the sentence
        char *word_token = strtok(sentence_start, " \n");
        while (word_token != NULL) {
            // Find index of the word in the sorted list
            for (int i = 0; i < word_count; i++) {
                if (strcmp(words[i], word_token) == 0) {
                    fprintf(wFile, "%d ", word_indices[i]);
                    break;
                }
            }
            word_token = strtok(NULL, " \n");
        }
        fprintf(wFile, "\n");  // Newline after each sentence's word numbers

        // Move to the next sentence
        sentence_start = period_position + 1;  // Start after the period
    }

    // Free memory
    for (int i = 0; i < word_count; i++) {
        free(words[i]);
    }

    fclose(rFile);
    fclose(wFile);
}
