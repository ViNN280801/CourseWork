#include"headers/htmlThief.h"
#include"headers/htmlParser.h"

int main(){
    printf("*** *** sinoptik.com scrapping... *** ***\n\n");
    system("./htmlThief https://sinoptik.com.ru/%D0%BF%D0%BE%D0%B3%D0%BE%D0%B4%D0%B0-%D1%81%D0%B0%D0%BD%D0%BA%D1%82-%D0%BF%D0%B5%D1%82%D0%B5%D1%80%D0%B1%D1%83%D1%80%D0%B3");
    printf("\n>>> pause <<<\n");
    getchar();
    system("clear");
    htmlParse();

    return EXIT_SUCCESS;
}