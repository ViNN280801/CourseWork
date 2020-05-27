#include<stdio.h>
#include<stdlib.h>

void createFile(FILE* csv, const char* fileName){
    if((csv = fopen(fileName, "w"))){
        printf("File was successfully created\n");
    }
    else printf("Error: Can't create file [%s]\n", fileName);
}