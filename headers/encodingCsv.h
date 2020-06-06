#include<stdio.h>
#include<stdlib.h>

void encodeFile(){
    FILE* csv;
    const char* csvFileName = "database.csv";
    char* fileCopy = NULL;
    long sizeOfFileCopy = 0L;
    int ch = 0;

    if((csv = fopen(csvFileName, "r+")) != NULL){
        fseek(csv, 0L, SEEK_END);
        sizeOfFileCopy = ftell(csv);
        fseek(csv, 0L, SEEK_SET);

        fileCopy = (char*)calloc(sizeOfFileCopy, sizeof(char));
        if(fileCopy != NULL){
            fread(fileCopy, sizeof(char), sizeOfFileCopy, csv);

            fseek(csv, 0L, SEEK_SET);
            while(true){
                ch = fgetc(csv);

                if(ch == EOF)
                    break;
                
                fprintf(csv, "%01X", ch ^ 0xBF);
            }

            printf("File was successfuly encoded\n");
            printf("Do you want to decode the file? Yes(1) No(2): ");
            scanf("%d", &ch);
            
            switch (ch)
            {
            case 1:
                system("clear");
                fseek(csv, 0L, SEEK_SET);
                fprintf(csv, "%s", fileCopy);
                printf("File was successfully decoded\n");
                break;
            case 2:
                system("clear");
                exit(EXIT_SUCCESS);
                break;
            default:
                printf("Error: choosing error\n");
                exit(EXIT_FAILURE);
                break;
            }
        }
        else{
            printf("Error: calloc return NULL:encodingCsv.h:17");
            exit(EXIT_FAILURE);
        }
    }
    else{
        printf("Error: can't open file for encode data\n");
        exit(EXIT_FAILURE);
    }
    fclose(csv);
}