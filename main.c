#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct WordFrequency {
    const char word[5];
    float frequency;
} wordFreq;  

wordFreq wordFreqs[456976]; // 26^4 i.e space for every possible four letter word
float flor;

int hash(char *input) { // Treat the four letter quadgrams as base 26 numbers. We can find a hash code for every quadgram that way
    int key = 0;
    for(int i = 0; i < 4; i++) {
        if(input[i] >= 'A' && input[i] <= 'Z') { // Checking for only four letter capitalized words
            key += (input[i] - 'A' + 1) * pow(26, (3 - i));
        } else {
            return -1;
        }
    }
    return key - 18278;
}

void shuffle(char *input) {
    int i, j, len = strlen(input);;
    char temp;
    for(i = 0; i < len; i++) {
        j = rand()%len;
        temp = input[i];
        input[i] = input[j];
        input[j] = temp;
    }
}

void decipher(char *input, char *key) {
    char alpha[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i, j, lenI = strlen(input), lenK = strlen(key);
    for(i = 0; i < lenI; i++) { // Loops through length of message
        for(j = 0; j < lenK; j++) { // Loops through the give key
            if(input[i] == key[j]) {
                input[i] = alpha[j]; // Deciphers letter
                break;
            }
        }
    }
}

void swap(char *input) {
    char temp;
    int a, b;
    a = rand() % 26;
    b = rand() % 26;
    temp = input[a];
    input[a] = input[b];
    input[b] = temp;
}

void upper(char *input) {
    int i, len = strlen(input);
    for(i = 0; i < len; i++) {
        if (input[i] >= 'a' && input[i] <= 'z') // If the character is lowercase alphabetical
            input[i] -= 32;
    }
}

void openAndAssignQuadgrams() {
    int i = 0, lines = 0, key, max = pow(26, 4);
    unsigned int N = 0;
    char ch, buff[5];
    float freq;

    // Open quadgrams file and convert text to two arrays located in struct
    FILE *quadgramsFile;

    if((quadgramsFile = fopen("english_quadgrams.txt", "r")) == NULL) {
        perror("fopen:");
    } else
        printf("Opened english_quadgrams.txt\n");

    while((ch = fgetc(quadgramsFile)) != EOF) { // Loop to count the number of lines in the file to create arrays
        if(ch == '\n')
            lines++;
    } rewind(quadgramsFile);

    i = 0;
    while(i < lines) {
        fscanf(quadgramsFile, "%s %f\n", &buff, &freq);
        key = hash(buff);
        if(key == -1) {
            printf("Incorrect quadgram: %s at line: %d", buff, i);
            exit(0);
        }
        //printf("%s %f\n", buff, freq);
        strcpy(wordFreqs[key].word, buff);
        wordFreqs[key].frequency = freq;
        //printf("%d %s %f\n", key, wordFreqs[key].word, wordFreqs[key].frequency);
        i++;
    }

    for(i = 1; i < max+1; i++) {
        N += wordFreqs[i].frequency;
    }

    flor = log10(0.01/N);

    for(i = 1; i < max+1; i++) {
        if(wordFreqs[i].frequency != 0.00000) {
            wordFreqs[i].frequency = log10(wordFreqs[i].frequency/N); // Convert frequencies to a score which is log of probablity of quadgram to be found in english language
        } else {
            wordFreqs[i].frequency = flor; // Where there are no corresponding frequencies give it a default score
        }
    }

    fclose(quadgramsFile);
}


float quadgramScore(char *message) {
    double score = 0;
    int i, key;
    char buff[5];
    //memset(buff, '\0', sizeof(buff)); // Ensure terminating character at end of buff
    for(i = 0; i < (strlen(message)-3); i++) { // Loop through the array to the length of the message minus the length of an ngram minus 1
        strncpy(buff, &message[i], 4); // Gets current ngram e.g if message was "THIS IS A MESSAGE" we first get "THIS" then "HIS " then " IS " etc
        if((key = hash(buff)) == -1) {
            score += flor;
        } else {
            score += wordFreqs[key].frequency;
        }
    }
    return score;
}

void subCipher(char *message, char *outputMessage) {
    openAndAssignQuadgrams();
    int len = strlen(message);
    char maxKey[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    float maxScore = -99e9, score, testScore;
    char parentKey[strlen(maxKey)], childKey[strlen(maxKey)];
    strcpy(parentKey, maxKey); // Copies maxKey to parentKey
    char testText[len], lastText[len];
    double t, begin, end, time_taken, time_taken1;
    int count, i = 0, newBestIndex = 0;

    begin = clock(); // Flag for time
    while(i < 100) {
        t = clock(); // Flag for time
        printf("\r%.2f%% done", (((float)i)/100)*100);
        fflush(stdout);
        i++;
        strcpy(testText, message); // Copies message to testText

        shuffle(parentKey); // Randomly shuffle the key
        decipher(message, parentKey); // Decipher the message using the shuffled key
        score = quadgramScore(message); // Find the fitness(how close it is to real words) of the text

        strcpy(message, testText); // Resets message back to original version as decipher() directly changes string

        count = 0;
        while(count < 500) {
            strcpy(lastText, testText); // Used for resetting testText every iteration

            strcpy(childKey, parentKey);
            swap(childKey); // Randomly swaps two characters of the childKey
            decipher(testText, childKey); // Decipher testText using childKey
            testScore = quadgramScore(testText); // Get fitness of testText

            if(testScore > score) { // Check if testScore is higher than score and set that as the new score
                score = testScore;
                strcpy(parentKey, childKey); // Copy the new key to the parent key
                count = 0;
            }
            strcpy(testText, lastText); // Used for resetting testText iteration

            count++;
        }
        if(score > maxScore) {
            time_taken = ((double)t)/CLOCKS_PER_SEC; // calculate the elapsed time
            printf("\nTaken %f seconds so far\n", time_taken);

            newBestIndex = i;
            maxScore = score; // Set new high score ot maxScore
            strcpy(maxKey, parentKey);
            printf("New best key found on iteration %d!\n", i);
            printf("Best score so far: %f\n", maxScore);
            printf("with key: ABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
            printf("          %s\n", maxKey);
            strcpy(testText, message); // Used for resetting message
            decipher(message, maxKey);
            printf("plain text:\n%s\n", message);
            strcpy(message, testText); // Used for resetting message
        }

        if(i - newBestIndex > 15) { // Checks if no new best iteration in last 10
            printf("\nNo new iteration found in last 15. Skipping to end!");
            break;
        }
    }
    end = clock();
    time_taken = ((double)(end - begin)/CLOCKS_PER_SEC); // calculate the elapsed time
    printf("\nTook %f seconds to decrypt!\n", time_taken);
    decipher(message, maxKey);
    strcpy(outputMessage, message); // Copies message to message
}

int main(){
    int length = 0, i;
    char ch;

    FILE *input;
    FILE *output;

    if((input = fopen("input.txt", "r")) == NULL) { // Opens file and checks if there is an error opening
        perror("fopen:");
        return 0;
    } else
        printf("Opened input.txt\n");


    if((output = fopen("output.txt", "w")) == NULL) { // Opens file and checks if there is an error opening
        perror("fopen:");
        return 0;
    } else
        printf("Opened output.txt\n");

    while(fgetc(input) != EOF){
        length++;
    } rewind(input);

    char message[length];
    char outputMessage[length];

    message[length] = '\0';
    outputMessage[length] = '\0';

    i = 0;
    while(i < length){
        message[i] = fgetc(input);
        i++;
    }

    printf("%d\n",strlen(message));
    printf("key: %d\n", hash("ZZZZ"));

    upper(message);

    subCipher(message, outputMessage);

    fprintf(output, "%s", outputMessage);

    fclose(input);
    fclose(output);
    return 1;
}