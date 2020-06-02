#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<string.h>
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
    }
    else{
        printf("Error: Can't add new data\n");
        exit(EXIT_FAILURE);
    }
    fclose(csv);
}

void findTemperature(GumboNode* node){
    if(node->type != GUMBO_NODE_ELEMENT)
        return;

    if(node->v.element.tag == GUMBO_TAG_B){
            GumboNode* textBetweenTagB = (GumboNode*)(node->v.element.children.data[0]);
        if(textBetweenTagB->type == GUMBO_NODE_TEXT || textBetweenTagB->type != GUMBO_NODE_WHITESPACE){
            if(z % 2 != 0){
                printf("max = %s\n", textBetweenTagB->v.text.text);
                weather[z].temperature = (char*)calloc(strlen(textBetweenTagB->v.text.text), sizeof(char*));
                strcpy(weather[z].temperature, (char*)textBetweenTagB->v.text.text);
            }
            else{
                printf("min = %s\n", textBetweenTagB->v.text.text);
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
            printf("%s\n", textBetweenTagLabel->v.text.text);

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
            printf("%s\n", textBetweenTagP->v.text.text);
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
                printf("%s\n", textBetweenTagA->v.text.text);
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
            printf("%s\n", textBetweenTagP->v.text.text);
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
                printf("%s\n", textBetweenTagP->v.text.text);
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

    copy = (char*)calloc(bytes + 1, sizeof(char*));
    copy[bytes + 1] = '\0';

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

    printf("\n>>> pause <<<\n");
    getchar();
    system("clear");    

    FILE* csv = NULL;    
    const char* csvFileName = "database.csv";

    createFile(csv, csvFileName);
    writeDataInCSV(csv, csvFileName, weather);

    return EXIT_SUCCESS;
}
