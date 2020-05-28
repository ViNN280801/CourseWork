#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct weather{
    char* weekDay;
    char* date;
    char* month;
    char* label;
    char* temperature;
};

void createFile(FILE* csv, const char* fileName){
    if((csv = fopen(fileName, "w"))){
        printf("File was successfully created\n");
    }
    else printf("Error: Can't create file [%s]\n", fileName);

    fclose(csv);
}

