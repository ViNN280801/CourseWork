#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
#include<utf8proc/utf8proc.h>
#include<gumbo.h>

#define BUFF_SIZE 256

void findWeather(GumboNode* node){
    GumboAttribute* className;
    const char* lost = "weather__content_tab-day";
    
    if(node->type != GUMBO_NODE_ELEMENT)
        return;

    GumboVector* child = &node->v.element.children;
    
    if(node->v.element.tag == GUMBO_TAG_P && 
        (className = gumbo_get_attribute(&node->v.element.attributes, "class")) && 
        !strcmp(className->value, lost)){
            GumboNode* textBetweenTagP = (GumboNode*)(node->v.element.children.data[0]);
                
            if(textBetweenTagP->type == GUMBO_NODE_TEXT || textBetweenTagP->type == GUMBO_NODE_WHITESPACE)
                printf("%s - %s\n", className->value, textBetweenTagP->v.text.text);
    }
    
    for(int i = 0; i < (child->length); i++){
        findWeather((GumboNode*)(child->data[i]));
    }
}

int main(){
    const char* fileName = "weather.html";
    FILE* fin;
    char* buffer = (char*)calloc(BUFF_SIZE, sizeof(char*));
    char* copy;
    long bytes = 0L;

    
    if((fin = fopen(fileName, "rb")) == NULL){
        perror("Error: Can't read file. Make sure of existanse this file.");

        return EXIT_FAILURE;
    }
    else{
        fseek(fin, 0L, SEEK_SET);
        while(fgets(buffer, sizeof(buffer), fin)){
            #ifdef PRINT_HTML
                printf("%s", buffer);
            #endif            
        }   
        bytes = ftell(fin);
        fseek(fin, bytes, SEEK_END);

        copy = (char*)calloc(bytes, sizeof(char));
        
        fseek(fin, 0L, SEEK_SET);
        while(fgets(buffer, sizeof(buffer), fin)){
            strcat(copy, buffer);
        }
        
        #ifdef PRINT_HTML
            printf("\nSuccessfully read %ld bytes\n", bytes);
        #endif

        free(buffer);
    }

    GumboOutput* out = gumbo_parse(copy);
    findWeather(out->root);
    gumbo_destroy_output(&kGumboDefaultOptions, out);

    free(copy);
    fclose(fin);

    return EXIT_SUCCESS;
}
