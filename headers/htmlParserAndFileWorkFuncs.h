#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
#include<gumbo.h>

#define BUFF_SIZE 256

struct sinoptik{
    char* weekDay;
    char* date;
    char* month;
    char* label;
    char* temperature;
};

struct sinoptik weather[BUFF_SIZE];

int counter = 0, k = 0, j = 0, z = 0, a = 0;

int size = 0;
int modi = 0;

void createFile(FILE* csv, const char* csvFileName){
    if((csv = fopen(csvFileName, "w"))){
        printf("File was successfully created\n");
    }
    else{
        printf("Error: Can't create file [%s]\n", csvFileName);
        exit(EXIT_FAILURE);
    }

    fclose(csv);
}

void writeDataInCSV(FILE* csv, const char* csvFileName, struct sinoptik weather[]){
    if((csv = fopen(csvFileName, "w"))){
        fprintf(csv, "Weekday, Date, Month, General characteristic, Temperature\n,,,,Min, Max");
        for(int i = 0; i < 14; i++){
            if(weather[i].weekDay != NULL && weather[i].date != NULL && weather[i].month != NULL && 
                weather[i].label != NULL){
                fprintf(csv, "%s,%s,%s,%s,%s,%s\n", weather[i].weekDay, weather[i].date,
                    weather[i].month, weather[i].label, weather[i+i].temperature, weather[i+i+1].temperature);
            }
        }
        printf("Data has been successfully written\nDatabase saved\n");
    }
    else{
        printf("Error: Can't create or write data in file\n");
        exit(EXIT_FAILURE);
    }
    fclose(csv);
}

char* readDataFromCSV(FILE* csv, const char* csvFileName){
    char* copy = NULL;
    long fileSize = 0L;

    if((csv = fopen(csvFileName, "r"))){
        fseek(csv, 0L, SEEK_END);
        fileSize = ftell(csv);
        fseek(csv, 0L, SEEK_SET);

        copy = (char*)calloc(fileSize, sizeof(char));

        fread(copy, sizeof(char), fileSize, csv);

        for(int i = 0; i < fileSize; i++){
            if(copy[i] == ',')
                copy[i] = '\t';
        }

        return copy;
        free(copy);
    }
    else{
        printf("Error: Can't read file\n");
    }
    fclose(csv);

    return NULL;
}

void addRecord(FILE* csv, const char* csvFileName){
    if((csv = fopen(csvFileName, "a+"))){
        int count = 0;
        printf("How much records do you want to write? ");
        scanf("%d", &count);

        for(int i = 0; i < count; i++){
            printf("Enter weekday: ");
            getchar();
            fflush(stdin);
            scanf("%[^\r\n]", weather[i].weekDay);
            printf("Enter date: ");
            getchar();
            fflush(stdin);
            scanf("%[^\r\n]", weather[i].date);
            printf("Enter month: ");
            getchar();
            fflush(stdin);
            scanf("%[^\r\n]", weather[i].month);
            printf("Enter general characteristics: ");
            getchar();
            fflush(stdin);
            scanf("%[^\r\n]", weather[i].label);
            printf("Enter minimal temperature: ");
            getchar();
            fflush(stdin);
            scanf("%[^\r\n]", weather[i+i].temperature);
            printf("Enter maximal temperature: ");
            getchar();
            fflush(stdin);
            scanf("%[^\r\n]", weather[i+i+1].temperature);

            fprintf(csv, "%s,%s,%s,%s,%s°,%s°\n", weather[i].weekDay, weather[i].date,
                    weather[i].month, weather[i].label, weather[i+i].temperature, weather[i+i+1].temperature);

            printf("Changes were saved successfully\n");
        }
    }
    else{
        printf("Error: Can't open file for write record\n");
    }
    fclose(csv);
}

void addRecordInPosition(FILE* csv, const char* csvFileName){
    char* oldFileCopy = NULL;
    long num = 0L, bytes = 0L, fpos1 = 0L, fpos2 = 0L;
    int choose = 0, counter = 0;
    char* tmp = NULL;
    char* temp = NULL;
    char* newCopyOfFile = NULL;
    char* newString = NULL;
    long newSize = 0L, newStringSize = 0L;

    if((csv = fopen(csvFileName, "r+")) != NULL){  
        fseek(csv, 0L, SEEK_END);
        bytes = ftell(csv);
        fseek(csv, 0L, SEEK_SET);

        temp = (char*)calloc(BUFF_SIZE, sizeof(char));
        oldFileCopy = (char*)calloc(bytes, sizeof(char));

        oldFileCopy = readDataFromCSV(csv, csvFileName);

        if(temp != NULL){
            temp = readDataFromCSV(csv, csvFileName);

            for(int i = 0; i < bytes; i++)
                if(temp[i] == '\n')
                    counter++;
        }
        else{
           printf("Error: calloc returns NULL pointer\n");
            exit(EXIT_FAILURE);
        }

        printf("Enter the record position to enter new data: ");
        scanf("%ld", &num);

        modi = num;

        if(num == 0){
            printf("Error: you can't' add new data to main rows\n");
            exit(EXIT_FAILURE);
        }

        if(num <= counter){
            printf("1. You want to add record? (1)\n2. Exit program (2)\nYour choose: ");
            scanf("%d", &choose);
            if(choose == 1){
                int i = 0;
                tmp = (char*)calloc(BUFSIZ, sizeof(char));
                if(tmp != NULL){
                    for(int j = 0; j < bytes; j++){
                        tmp[j] = temp[j];
                        fpos1 = (long)strlen(tmp);
                        if(tmp[j] == '\n'){
                            i++;
                            if(i == num){
                                if((fpos1 - fpos2) < 0){
                                    fpos2 = fpos1;
                                    size = fpos2;

                                    fseek(csv, 0L, SEEK_SET);
                                }
                                else{
                                    size = fpos1 - fpos2;
                                    fseek(csv, size, SEEK_SET);
                                }

                                fpos2 = (long)strlen(tmp);

                                fflush(stdin);
                                printf("Enter weekday: ");
                                getchar();
                                scanf("%[^\n]", weather[i].weekDay);
                                newStringSize += strlen(weather[i].weekDay);
                                printf("Enter date: ");
                                getchar();
                                scanf("%[^\n]", weather[i].date);
                                newStringSize += strlen(weather[i].date);
                                printf("Enter month: ");
                                getchar();
                                scanf("%[^\n]", weather[i].month);
                                newStringSize += strlen(weather[i].month);
                                printf("Enter general characteristics: ");
                                getchar();
                                scanf("%[^\n]", weather[i].label);
                                newStringSize += strlen(weather[i].label);
                                printf("Enter minimal temperature: ");
                                getchar();
                                scanf("%[^\n]", weather[i+i].temperature);
                                newStringSize += strlen(weather[i+i].temperature);
                                printf("Enter maximal temperature: ");
                                getchar();
                                scanf("%[^\n]", weather[i+i+1].temperature);
                                newStringSize += strlen(weather[i+i+1].temperature);

                                newString = (char*)calloc(newStringSize, sizeof(char));
                                if(newString != NULL){
                                    strcpy(newString, weather[i].weekDay);
                                    strcat(newString, ",");
                                    strcat(newString, weather[i].date);
                                    strcat(newString, ",");
                                    strcat(newString, weather[i].month);
                                    strcat(newString, ",");
                                    strcat(newString, weather[i].label);
                                    strcat(newString, ",");
                                    strcat(newString, weather[i+i].temperature);
                                    strcat(newString, "°,");
                                    strcat(newString, weather[i+i+1].temperature);
                                    strcat(newString, "°\n");

                                    newStringSize = strlen(newString);

                                    fprintf(csv, "%s", newString);
                                }
                                else{
                                    printf("Error: calloc returns NULL pointer\n");
                                    exit(EXIT_FAILURE);
                                }
                            }
                            else if(i != num){
                                if(weather[i].weekDay != NULL && weather[i].date != NULL && 
                                    weather[i].label != NULL && weather[i].month != NULL &&
                                    weather[i+i].temperature != NULL && 
                                    weather[i+i+1].temperature != NULL){
                                        
                                    fseek(csv, newStringSize + fpos2, SEEK_END);
                                    newSize = ftell(csv);
                                    fseek(csv, newStringSize + fpos2, SEEK_SET);

                                    for(int k = size; k < newSize; k++){
                                        fprintf(csv, "%c", oldFileCopy[k]);
                                    }
                                }
                                continue;
                            }
                            else{
                                printf("Error: i != num:htmlParser.h:173");
                            }
                        }
                    }
                }
                else{
                    printf("Error: calloc returns NULL pointer\n");
                    exit(EXIT_FAILURE);
                }
            }
            else if(choose == 2){
                exit(EXIT_SUCCESS);
            }
            else{
                printf("Error: invalid choosing\n");
                exit(EXIT_FAILURE);
            }
            free(newCopyOfFile);
        }
        else{
            printf("Error: invalid number of record\n");
            exit(EXIT_FAILURE);
        }
        free(tmp);
    }
    else{
        printf("Error: can't add record\n");
        exit(EXIT_FAILURE);
    }
    free(temp);
    free(oldFileCopy);
    fclose(csv);
}

void modifiedRecord(FILE* csv, const char* csvFileName){
    char* fileCopy = readDataFromCSV(csv, csvFileName);
    long sizeOfFileCopy = strlen(fileCopy);
    int numberOfTransferCh = 0, whereIsN = 0, whereIsNVersion2 = 0;
    
    for(int i = 0; i < sizeOfFileCopy; i++){
        if(fileCopy[i] == '\n'){
            numberOfTransferCh++;
        }
    }

    if((csv = fopen(csvFileName, "r+")) != NULL){
        for(int i = 0; i < numberOfTransferCh; i++){
            if(modi == i){
                for(int k = size; k < size + 50; k++){
                    if(fileCopy[k] == '\n'){
                        whereIsN = k;
                    }
                }
            }
        }

        for(int i = whereIsN; i < whereIsN + 150; i++){
            if(fileCopy[i] == '\n'){
                whereIsNVersion2 = i;
            }
        }
        
        fseek(csv, (long)(whereIsN), SEEK_SET);
        for(int i = whereIsN; i < whereIsNVersion2; i++){
            fputc(fileCopy[i], csv);
        }

        printf("Changes were saved successfully\n");
    }
    else{
        printf("Error: can't open file for modify\n");
        exit(EXIT_FAILURE);
    }
}

void deleteRecord(FILE* csv, const char* csvFileName){
    char* fileCopy = readDataFromCSV(csv, csvFileName);
    long sizeOfFileCopy = strlen(fileCopy);
    int numberOfTransferCh = 0;
    int deleteLine = 0;
    char* buf = (char*)calloc(BUFF_SIZE, sizeof(char));
    int counter = 0, newSize = 0, newSize2 = 0;

    if(buf != NULL){
        printf("Enter line which you want to delete: ");
        scanf("%d", &deleteLine);
        
        for(int i = 0; i < sizeOfFileCopy; i++){
            if(fileCopy[i] == '\n'){
                numberOfTransferCh++;
            }
        }

        for(int i = 0; i < numberOfTransferCh; i++){
            if(deleteLine == i){
                for(int j = 0; j < sizeOfFileCopy; j++){
                    if(fileCopy[j] == '\n'){
                        counter++;
                        if(counter == deleteLine)
                            newSize = j;
                    }                        
                }
            }           
        }

        for(int i = newSize + 1; i < newSize + 100; i++){
            if(fileCopy[i] == '\n')
                newSize2 = i;
        }

        if((csv = fopen(csvFileName, "r+")) != NULL){
            fseek(csv, newSize + 1, SEEK_SET);
                
            for(int i = newSize + 1; i < newSize2; i++){
                fileCopy[i] = '\0';
                fputc(fileCopy[i], csv);
            }
                
            fseek(csv, newSize2, SEEK_SET);
            fputs("|", csv);
        }
        else{
            printf("Error: can't delete data\n");
            exit(EXIT_FAILURE);
        }
        fclose(csv);
    }
    else{
        printf("Error: calloc returns NULL pointer\n");
        exit(EXIT_FAILURE);
    }
}

void findTemperature(GumboNode* node){
    if(node->type != GUMBO_NODE_ELEMENT)
        return;

    if(node->v.element.tag == GUMBO_TAG_B){
            GumboNode* textBetweenTagB = (GumboNode*)(node->v.element.children.data[0]);
        if(textBetweenTagB->type == GUMBO_NODE_TEXT || textBetweenTagB->type != GUMBO_NODE_WHITESPACE){
            if(z % 2 != 0){
                #ifdef PRNT
                    printf("max = %s\n", textBetweenTagB->v.text.text);
                #endif
                weather[z].temperature = (char*)calloc(strlen(textBetweenTagB->v.text.text), sizeof(char*));
                strcpy(weather[z].temperature, (char*)textBetweenTagB->v.text.text);
            }
            else{
                #ifdef PRNT
                    printf("min = %s\n", textBetweenTagB->v.text.text);
                #endif
                weather[z].temperature = (char*)calloc(strlen(textBetweenTagB->v.text.text), sizeof(char*));
                strcpy(weather[z].temperature, (char*)textBetweenTagB->v.text.text);
            }
            z++;
        }
    }

    GumboVector* child = &node->v.element.children;
    for(int i = 0; i < (child->length); i++){
        findTemperature((GumboNode*)(child->data[i]));

        if(z == 14)
            i = child->length;
    }
}

void findLabel(GumboNode* node, const char* lostLabel){
    GumboAttribute* className;
    
    if(node->type != GUMBO_NODE_ELEMENT)
        return;

    if(node->v.element.tag == GUMBO_TAG_LABEL && 
        (className = gumbo_get_attribute(&node->v.element.attributes, "class")) && 
        !strcmp(className->value, lostLabel)){
        GumboNode* textBetweenTagLabel = (GumboNode*)(node->v.element.children.data[0]);
                
        if(textBetweenTagLabel->type == GUMBO_NODE_TEXT 
            || textBetweenTagLabel->type != GUMBO_NODE_WHITESPACE){
            char* tmp = (char*)calloc(strlen(textBetweenTagLabel->v.text.text), sizeof(char*));

            strcpy(tmp, textBetweenTagLabel->v.text.text);
            #ifdef PRNT
                printf("%s\n", textBetweenTagLabel->v.text.text);
            #endif

            if(tmp[0] == '\n')
                tmp[0] = ' ';

            for(int v = 0; v < strlen(tmp); v++){
                if(tmp[v] == ',')
                    tmp[v] = ' ';
            }

            weather[j].label = (char*)calloc(strlen(textBetweenTagLabel->v.text.text), sizeof(char*));
            strcpy(weather[j].label, tmp);
        }
        j++;
    }
    GumboVector* child = &node->v.element.children;
    for(int i = 0; i < (child->length); i++){
        findLabel((GumboNode*)(child->data[i]), lostLabel);
        if(j == 7){
            i = child->length;
        }
    }
}

void findWeekDay(GumboNode* node, const char* lostWeekDay, const char* curDay){
    GumboAttribute* className;
        
    if(node->type != GUMBO_NODE_ELEMENT)
        return;

    if(node->v.element.tag == GUMBO_TAG_P && 
        (className = gumbo_get_attribute(&node->v.element.attributes, "class")) && 
        !strcmp(className->value, curDay)){
        GumboNode* textBetweenTagP = (GumboNode*)(node->v.element.children.data[0]);
                
        if(textBetweenTagP->type == GUMBO_NODE_TEXT || textBetweenTagP->type != GUMBO_NODE_WHITESPACE){
            #ifdef PRNT
                printf("%s\n", textBetweenTagP->v.text.text);
            #endif
            weather[a].weekDay = (char*)calloc(strlen(textBetweenTagP->v.text.text), sizeof(char*));

        if(weather[a].weekDay != NULL && (a == 0 || a == 1)){
            strcpy(weather[a].weekDay, textBetweenTagP->v.text.text);
            a++;
            }
        }
    }

    if(node->v.element.tag == GUMBO_TAG_A && 
        (className = gumbo_get_attribute(&node->v.element.attributes, "class")) && 
        !strcmp(className->value, lostWeekDay)){
            GumboNode* textBetweenTagA = (GumboNode*)(node->v.element.children.data[0]);
                
            if(textBetweenTagA->type == GUMBO_NODE_TEXT || textBetweenTagA->type != GUMBO_NODE_WHITESPACE){
                #ifdef PRNT
                    printf("%s\n", textBetweenTagA->v.text.text);
                #endif
                weather[k+a].weekDay = (char*)calloc(strlen(textBetweenTagA->v.text.text), sizeof(char*));
                strcpy(weather[k+a].weekDay, textBetweenTagA->v.text.text);
            }
            k++;
            counter++;
    }

    GumboVector* child = &node->v.element.children;
    for(int i = 0; i < (child->length); i++){
        findWeekDay((GumboNode*)(child->data[i]), lostWeekDay, curDay);
        if(counter == 7){
            i = child->length;
        }
    }
}

int t = 0, f = 0;

void findDate(GumboNode* node, const char* lost){
    GumboAttribute* className;
    
    if(node->type != GUMBO_NODE_ELEMENT)
        return;

    if(node->v.element.tag == GUMBO_TAG_P && 
        (className = gumbo_get_attribute(&node->v.element.attributes, "class")) && 
        !strcmp(className->value, lost)){
        GumboNode* textBetweenTagP = (GumboNode*)(node->v.element.children.data[0]);
                
        if(textBetweenTagP->type == GUMBO_NODE_TEXT || textBetweenTagP->type != GUMBO_NODE_WHITESPACE){
            #ifdef PRNT
                printf("%s\n", textBetweenTagP->v.text.text);
            #endif
            weather[t].date = (char*)calloc(strlen(textBetweenTagP->v.text.text), sizeof(char*));
            strcpy(weather[t].date, textBetweenTagP->v.text.text);
        }
        t++;
    }

    GumboVector* child = &node->v.element.children;
    for(int i = 0; i < (child->length); i++){
        findDate((GumboNode*)(child->data[i]), lost);
    }
}

void findMonth(GumboNode* node, const char* lost){
    GumboAttribute* className;
    
    if(node->type != GUMBO_NODE_ELEMENT)
        return;

    if(node->v.element.tag == GUMBO_TAG_P && 
        (className = gumbo_get_attribute(&node->v.element.attributes, "class")) && 
        !strcmp(className->value, lost)){
            GumboNode* textBetweenTagP = (GumboNode*)(node->v.element.children.data[0]);
                
            if(textBetweenTagP->type == GUMBO_NODE_TEXT || textBetweenTagP->type != GUMBO_NODE_WHITESPACE){
                #ifdef PRNT
                    printf("%s\n", textBetweenTagP->v.text.text);
                #endif
                weather[f].month = (char*)calloc(strlen(textBetweenTagP->v.text.text), sizeof(char*));
                strcpy(weather[f].month, textBetweenTagP->v.text.text);
            }
            f++;
    }

    GumboVector* child = &node->v.element.children;
    for(int i = 0; i < (child->length); i++){
        findMonth((GumboNode*)(child->data[i]), lost);
    }
}

int htmlParse(){
    const char* fileName = "weather.html";
    FILE* fin;
    char* buffer = (char*)calloc(BUFF_SIZE, sizeof(char*));
    char* copy;
    long bytes = 0L;

    if((fin = fopen(fileName, "rb")) == NULL){
        perror("Error: Can't read file. Make sure of existanse this file.");

        return EXIT_FAILURE;
    }
    fseek(fin, 0L, SEEK_END);
    bytes = ftell(fin);
    fseek(fin, 0L, SEEK_SET);

    copy = (char*)calloc(bytes, sizeof(char));

    fseek(fin, 0L, SEEK_SET);

    fread(copy, sizeof(char*), bytes, fin);

    #ifdef PRINT_HTML
        printf("\n%s\n", copy);
        printf("\nSuccessfully read %ld bytes\n", bytes);
    #endif

    free(buffer);

    const char* curDay = "weather__content_tab-day";
    const char* lostWeekDay = "weather__content_tab_a";
    const char* lostDay = "weather__content_tab-date day_red";
    const char* lostMonth = "weather__content_tab-month";
    const char* lostLabel = "show-tooltip";

    GumboOutput* out = gumbo_parse(copy);

    findWeekDay(out->root, lostWeekDay, curDay);
    findDate(out->root, lostDay);
    findMonth(out->root, lostMonth);
    findLabel(out->root, lostLabel);
    findTemperature(out->root);

    gumbo_destroy_output(&kGumboDefaultOptions, out);

    free(copy);
    fclose(fin);
    
    system("clear");

    return EXIT_SUCCESS;
}
