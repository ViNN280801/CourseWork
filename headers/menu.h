#include"htmlParserAndFileWorkFuncs.h"
#include"encodingCsv.h"

#define BUFF_SIZE 256

void showMenu(){
    FILE* csv = NULL;
    const char* csvFileName = "database.csv";

    int choose = 0, counter = 0;
    char* username = NULL;
    char* psswd = NULL;

    printf("Username: ");

    username = (char*)calloc(BUFF_SIZE, sizeof(char));
    
    if(username != NULL){
        scanf("%[^\n]", username);
        

        username[strlen(username)] = '\0';

        if(!strcmp(username, "admin")){
            psswd = (char*)calloc(BUFF_SIZE, sizeof(char));

            if(psswd != NULL){ 
                passwd:
                printf("Password: ");

                fflush(stdin);
                getchar();
                scanf("%[^\n]", psswd);                

                psswd[strlen(psswd)] = '\0';

                if(!strcmp(psswd, "admin")){
                    choose = 0;

                    system("clear");
                    printf("\033[0;32m");
                    printf("You have successfully logged in under admin\n\n\n\n");
                    printf("\033[0m");
                
                    choosing:
                    counter++;

                    printf("1. Create file\n");
                    printf("2. Output info to terminal\n");
                    printf("3. Add record in end of file\n");
                    printf("4. Add record in random line\n");
                    printf("5. Modify record\n");
                    printf("6. Delete record\n");
                    printf("\033[0;36m");
                    printf("7. Encode the file\n");
                    printf("\033[0m");
                    printf("8. Exit from program\n");

                    printf("Your choosing: ");
                    scanf("%d", &choose);

                    switch(choose)
                    {
                    case 1:
                        system("clear");
                        createFile(csv, csvFileName);
                        writeDataInCSV(csv, csvFileName, weather);
                        goto choosing;
                        break;
                    case 2:
                        system("clear");
                        char* copy = NULL;
                        long fileSize = 0L;

                        if((csv = fopen(csvFileName, "r"))){
                            fseek(csv, 0L, SEEK_END);
                            fileSize = ftell(csv);
                            fseek(csv, 0L, SEEK_SET);

                            copy = (char*)calloc(fileSize, sizeof(char));

                            for(int i = 0; i < fileSize; i++){
                                copy[i] = fgetc(csv);

                                if(copy[i] == '\0')
                                    copy[i] = '.';
                            }

                            for(int i = 0; i < fileSize; i++){
                                if(copy[i] == ',')
                                    copy[i] = '\t';
                                if(copy[i] != '.'){
                                    printf("%c", copy[i]);
                                }
                            }
                            free(copy);
                        }
                        else{
                            printf("Error: Can't read file\n");
                        }
                        fclose(csv);
                        goto choosing;
                        break;
                    case 3:
                        system("clear");
                        addRecord(csv, csvFileName);
                        goto choosing;
                        break;
                    case 4:
                        system("clear");
                        addRecordInPosition(csv, csvFileName);
                        createFile(csv, csvFileName);
                        writeDataInCSV(csv, csvFileName, weather);
                        goto choosing;
                        break;
                    case 5:
                        system("clear");
                        addRecordInPosition(csv, csvFileName);
                        modifiedRecord(csv, csvFileName);
                        createFile(csv, csvFileName);
                        writeDataInCSV(csv, csvFileName, weather);
                        goto choosing;
                        break;
                    case 6:
                        system("clear");
                        deleteRecord(csv, csvFileName);
                        goto choosing;
                        break;
                    case 7:
                        system("clear");
                        encodeFile();
                        goto choosing;
                        break;
                    case 8:
                        system("clear");
                        exit(EXIT_SUCCESS);
                        break;
                    default:
                        system("clear");
                        printf("Invalid choosing\n");
                        exit(EXIT_FAILURE);
                    }
                }
                else{
                    printf("Wrong password\n");
                    goto passwd;
                }
            }
            else{
                printf("Error: calloc returns NULL:menu.h:22");
                exit(EXIT_FAILURE);
            }
        }
        else{
            system("clear");
            printf("\033[0;33m");
            printf("You have successfully logged in under user\n\n\n\n");
            printf("\033[0m");
            
            printf("1. Read data from file\n");
            printf("2. Exit from program\n");
            printf("Your choose: ");
            scanf("%d", &choose);

            if(choose == 1){
                system("clear");
                printf("%s\n", readDataFromCSV(csv, csvFileName));
            }
            else if(choose == 2){
                system("clear");
                exit(EXIT_SUCCESS);
            }
            else{
                system("clear");
                printf("Error: invalid choosing\n");
                exit(EXIT_FAILURE);
            }
        }
        free(psswd);
    }
    else{
        printf("Error: calloc returns NULL:menu.h:16");
        exit(EXIT_FAILURE);
    }
    free(username);
}