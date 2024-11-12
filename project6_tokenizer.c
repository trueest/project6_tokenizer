/*
* Name: Hunter Hawkins - U13938229
* Email: hunterhawkins@usf.edu 
* Assignment: Project 6: Task 1
* This C program will
*/

// ADDING LIBRARIES
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Initializing our max character count per requirements.
#define MAX_LEN 10000

// Comparison function for qsort.
int compare_strings(const void *a, const void *b) {
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
    char buffer[MAX_LEN + 1] = ""; // our input buffer
    char text[MAX_LEN + 1] = ""; // our array of text, concatenated by strcat and read line by line by fgets
    char *delimiters = ". \n";
    char *token;

    // Read the file line by line with fgets and adding to our buffer and text arrays.
    while (!feof(rFile) && !ferror (rFile)) {
        while(fgets(buffer, MAX_LEN, rFile)) {
            strcat(text, buffer);
        }
    } 

    char *words[MAX_LEN + 1];
    int word_count = 0;

    //next we will tokenize our text array in *tok
    token = strtok(text, delimiters);
    while(token != NULL) {
        int unique = 1;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                unique = 0;
                break;
            }
        }

        if (unique) {
            words[word_count] = malloc(strlen(token) + 1);
            strcpy(words[word_count], token);
            word_count++;
        }
        token = strtok(NULL, delimiters);
    }

    // Using the qsort function with our compare_strings function to alphabetically sort our unique words.
    qsort(words, word_count, sizeof(char *), compare_strings);

    // Print or write the sorted words
    fprintf(wFile, "%d\n", word_count);
    for (int i = 0; i < word_count; i++) {
        fprintf(wFile, "%s\n", words[i]);  // Write each word to output file
    }

    // Create a map of word to index
    int word_indices[MAX_LEN];
    for (int i = 0; i < word_count; i++) {
        word_indices[i] = i + 1;  // Assign numbers starting from 1
    }

    // Reset text and re-read it for tokenizing sentences
    rewind(rFile);
    text[0] = '\0';  // Clear text for re-use
    while (fgets(buffer, MAX_LEN, rFile)) {
        strcat(text, buffer);
    }

    char *sentence_token;
    sentence_token = strtok(text, ".");
    while (sentence_token != NULL) {
        token = strtok(sentence_token, " \n");
        while (token != NULL) {
            // Find the index of the current word
            for (int i = 0; i < word_count; i++) {
                if (strcmp(words[i], token) == 0) {
                    fprintf(wFile, "%d ", word_indices[i]);
                    break;
                }
            }
            token = strtok(NULL, " \n");
        }
        fprintf(wFile, "\n");  // New line after each sentence
        sentence_token = strtok(NULL, ".");
    }

    // Free memory
    for (int i = 0; i < word_count; i++) {
        free(words[i]);
    }

    fclose(rFile);
    fclose(wFile);
}