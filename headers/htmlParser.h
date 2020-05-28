#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
#include<utf8proc/utf8proc.h>
#include<gumbo.h>

#define BUFF_SIZE 256

int counter = 0;

void findTemperature(GumboNode* node){
    if(node->type != GUMBO_NODE_ELEMENT)
        return;

    if(node->v.element.tag == GUMBO_TAG_B){
            GumboNode* textBetweenTagB = (GumboNode*)(node->v.element.children.data[0]);
            if(textBetweenTagB->type == GUMBO_NODE_TEXT || textBetweenTagB->type != GUMBO_NODE_WHITESPACE){
                if(counter % 2 == 0){
                    printf("max = %s\n", textBetweenTagB->v.text.text);
                }    
                else 
                    printf("min = %s\n", textBetweenTagB->v.text.text);

                counter++;
            }
    }

    GumboVector* child = &node->v.element.children;
    for(int i = 0; i < (child->length); i++){
        findTemperature((GumboNode*)(child->data[i]));

        if(counter == 21){
            i = child->length;
        }
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
                printf("%s\n", textBetweenTagLabel->v.text.text);
                counter++;
            }

        
    }
    GumboVector* child = &node->v.element.children;
    for(unsigned int i = 0; i < (child->length); i++){
        findLabel((GumboNode*)(child->data[i]), lostLabel);
        if(counter == 7){
            i = child->length;
        }
    }
}

void findCurDay(GumboNode* node, const char* curDay){
    GumboAttribute* className;
        
    if(node->type != GUMBO_NODE_ELEMENT)
        return;

    if(node->v.element.tag == GUMBO_TAG_P && 
        (className = gumbo_get_attribute(&node->v.element.attributes, "class")) && 
        !strcmp(className->value, curDay)){
            GumboNode* textBetweenTagP = (GumboNode*)(node->v.element.children.data[0]);
                
            if(textBetweenTagP->type == GUMBO_NODE_TEXT || textBetweenTagP->type != GUMBO_NODE_WHITESPACE)
                printf("%s\n", textBetweenTagP->v.text.text);
    }

    GumboVector* child = &node->v.element.children;
    for(int i = 0; i < (child->length); i++){
        findCurDay((GumboNode*)(child->data[i]), curDay);
    }
}

void findWeekDay(GumboNode* node, const char* lostWeekDay){
    GumboAttribute* className;
        
    if(node->type != GUMBO_NODE_ELEMENT)
        return;

    if(node->v.element.tag == GUMBO_TAG_A && 
        (className = gumbo_get_attribute(&node->v.element.attributes, "class")) && 
        !strcmp(className->value, lostWeekDay)){
            GumboNode* textBetweenTagA = (GumboNode*)(node->v.element.children.data[0]);
                
            if(textBetweenTagA->type == GUMBO_NODE_TEXT || textBetweenTagA->type == GUMBO_NODE_WHITESPACE)
                printf("%s\n", textBetweenTagA->v.text.text);
    }

    GumboVector* child = &node->v.element.children;
    for(int i = 0; i < (child->length); i++){
        findWeekDay((GumboNode*)(child->data[i]), lostWeekDay);
    }
}

void findWeather(GumboNode* node, const char* lost){
    GumboAttribute* className;
        
    if(node->type != GUMBO_NODE_ELEMENT)
        return;

    if(node->v.element.tag == GUMBO_TAG_P && 
        (className = gumbo_get_attribute(&node->v.element.attributes, "class")) && 
        !strcmp(className->value, lost)){
            GumboNode* textBetweenTagP = (GumboNode*)(node->v.element.children.data[0]);
                
            if(textBetweenTagP->type == GUMBO_NODE_TEXT || textBetweenTagP->type == GUMBO_NODE_WHITESPACE)
                printf("%s\n", textBetweenTagP->v.text.text);
    }

    GumboVector* child = &node->v.element.children;
    for(int i = 0; i < (child->length); i++){
        findWeather((GumboNode*)(child->data[i]), lost);
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

    copy = (char*)calloc(bytes + 1, sizeof(char));
    copy[bytes + 1] = '\0';

    fseek(fin, 0L, SEEK_SET);

    fread(copy, sizeof(char), bytes, fin);

    #ifdef PRINT_HTML
        printf("\n%s\n", copy);
        printf("\nSuccessfully read %ld bytes\n", bytes);
    #endif

    free(buffer);

    char* curDay;
    const char* curDay = "weather__content_tab-day";
    const char* lostWeekDay = "weather__content_tab_a";
    const char* lostDay = "weather__content_tab-date day_red";
    const char* lostMonth = "weather__content_tab-month";
    const char* lostLabel = "show-tooltip";

    GumboOutput* out = gumbo_parse(copy);

    findCurDay(out->root, curDay);
    findWeekDay(out->root, lostWeekDay);
    findWeather(out->root, lostDay);
    findWeather(out->root, lostMonth);
    findLabel(out->root, lostLabel);
    findTemperature(out->root);

    gumbo_destroy_output(&kGumboDefaultOptions, out);

    free(copy);
    fclose(fin);

    return EXIT_SUCCESS;
}
