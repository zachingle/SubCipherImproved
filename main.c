#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void subCipher(char *message);

int main(){
    int length = 0;
    char *message = malloc(sizeof(char)); // Allocate 1 byte to array

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


    while(!feof(input)){
        message[length] = fgetc(input); // Assign character to length spot in message array
        message = realloc(message, sizeof(char)); // Reallocate another byte to message
        length++;
    }

    message[length-1] = '\0';

    printf("%s\n", message);
    printf("%d\n",strlen(message));

    subCipher(message);

    free(message);
    fclose(input);
    fclose(output);
    return 1;
}

char **ngramsArray = NULL;
float *ngramsFreqArray = NULL;

void subCipher(char *message){
    char **tempC = NULL;
    float *tempF = NULL;

    char buff[128];
    char buffNgram[5];

    float freq;
    
    int i = 0;

    FILE *ngramsFile;

    if((ngramsFile = fopen("english_quadgrams.txt", "r")) == NULL){
        perror("fopen:");
    } else
        printf("Opened english_quadgrams.txt\n");

    while(!feof(ngramsFile)){
        tempC = realloc(ngramsArray, (i + 1) * 4);
        if(tempC == NULL)
            perror("Temp C realloc:");
        ngramsArray = tempC;

        ngramsArray[i] = malloc(5);
        if(ngramsArray[i] == NULL)
            perror("malloc:");

        tempF = realloc(ngramsFreqArray, sizeof(float));
        if(tempF == NULL)
            perror("Temp F realloc:");
        ngramsFreqArray = tempF;
       
        

        fgets(buff, 128, ngramsFile);
        printf("buff: %s", buff);
        memset(buffNgram, '\0', sizeof(buffNgram));
        strncpy(buffNgram, buff, 4);
        strcpy(ngramsArray[i], buffNgram);
        printf("buffNgram: %s\n", ngramsArray[i]);
        freq = strtof(&buff[5], NULL);
        ngramsFreqArray[i] = freq;
        printf("freq: %f\n", ngramsFreqArray[i]);
        i++;
    }

    for(i = 0; i < 100; i++){
        printf("%s %f\n", ngramsArray[i], ngramsFreqArray[i]);
    }

    free(ngramsArray);
    free(ngramsFreqArray);
    fclose(ngramsFile);
}