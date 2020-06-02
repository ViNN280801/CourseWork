#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<c++/10/curl/curl.h>

static size_t writeData(void* ptr, size_t size, size_t nmemb, void* stream){
  size_t written = fwrite(ptr, size, nmemb, (FILE*)stream);

  return written;
}

int htmlThief(int argc, char* argv[]){
    CURL* curlHandle;
    const char* pageFileName = "weather.html";
    FILE* pageFile;

    if(argc < 2){
        printf("Usage: %s<URL>\n", argv[0]);

        return EXIT_FAILURE;
    }

    curl_global_init(CURL_GLOBAL_ALL); // flag CURL_GLOBAL_ALL - 
                                        // initializing everything is possible (http, ssl, etc.)

    curlHandle = curl_easy_init(); //initializing the curl session
    curl_easy_setopt(curlHandle, CURLOPT_URL, argv[1]); //setting up the URL
    curl_easy_setopt(curlHandle, CURLOPT_VERBOSE, 1L); //switch on full protocol/debug output while test
    curl_easy_setopt(curlHandle, CURLOPT_NOPROGRESS, 0L); //enable progress bar
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, writeData);

    if((pageFile = (fopen(pageFileName, "wb"))) != NULL){
        curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, pageFile);
        curl_easy_perform(curlHandle); //perform the transfer as descripted in options

        fclose(pageFile);
    }
    else 
        perror("Error write data in file");

    curl_easy_cleanup(curlHandle);
    curl_global_cleanup();

    return EXIT_SUCCESS;
}