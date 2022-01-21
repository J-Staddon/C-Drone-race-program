#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "function.h"

int numCompetitor = 0; //The number of competitors
size_t competitorSize = 0; //The size of the competitor array
char event[50]; //The event name
char date[20]; //The date of the event
int quit = 0; //Becomes 1 if user quits
int max = 0; //The largest full name

//Used to compare the surnames of competitors and order them
int cmpName (void * name1, void * name2){

    competitor *n1; //two different competitor pointers
    competitor *n2;
    n1 = (competitor *)name1;
    n2 = (competitor *)name2;
    char p1n[60];
    char p2n[60];

    strcpy(p1n, n1->surname); //Gets the surnames
    strcpy(p2n, n2->surname);

    //Compares their ascii size
    if (strcmp(p1n, p2n) < 0){
        return -1;
    }
    else if (strcmp(p1n, p2n) > 0){
        return 1;
    }
    return 0;
}

//EXTENDED FEATURES//

//Prints the competitors in a list style
void printList(struct Competitor competitors){
    printf("Competitor %d Name: %s, %s\n", competitors.id, competitors.surname, competitors.firstName);
    printf("Email Address: %s\n", competitors.email);
    printf("Flyer ID:  %s\n\n", competitors.flyerid);
}

//Prints the competitors in a table style
void printTable(struct Competitor competitors){
    int width = 5 + max - (strlen(competitors.firstName) + strlen(competitors.surname)); //Finds distance the competitor id has to be from the name
    printf("%s %s %*d   %s   %s\n", competitors.firstName, competitors.surname, width, competitors.id, competitors.flyerid, competitors.email);
}

//Traverses the array of competitors
void printTraversal(struct Competitor *competitors, void function()){
    qsort(competitors, numCompetitor, sizeof(struct Competitor), (__compar_fn_t) cmpName); //Sorts the list of competitors
    for (int i = 0; i < numCompetitor; i++){
        function(competitors[i]);
    }
}
//EXTENDED FEATURES//

//Prints the competitors in a table format
void print(struct Competitor *competitors){
    qsort(competitors, numCompetitor, sizeof(struct Competitor), (__compar_fn_t) cmpName); //Sorts the list of competitors
    for (int i = 0; i < numCompetitor; i++){
        size_t num = strlen(competitors[i].firstName) + strlen(competitors[i].surname); //Takes length of both first and surname
        if (num > max){ //Compares the largest name to the current one
            max = num; //Sets a new maximum
        }
    }
    printf("\nCompetition: %s  Date: %s\n", event, date);
    printf("NAME          Competitor   Flyer ID       Email\n");
    printf("======================================================\n");
    for(int i = 0; i < numCompetitor; i++) {
        int width = 5 + max - (strlen(competitors[i].firstName) + strlen(competitors[i].surname)); //Finds distance the competitor id has to be from the name
        printf("%s %s %*d   %s   %s\n", competitors[i].firstName, competitors[i].surname, width, competitors[i].id, competitors[i].flyerid, competitors[i].email);
    }
}

//Adds a competitor to the array
void addCompetitor(struct Competitor *c){
    struct Competitor temp; //Holds data for a new competitor
    char flyeridHolder[40]; //Holds a flyer id
    char emailHolder[100]; //Holds a email

    printf("Please fill in the below details\n");
    printf("First name: ");
    scanf("%c", (char *) stdin); //A buffer line
    fgets(temp.firstName, 60, stdin); //Input for the first name
    printf("Surname: ");
    fgets(temp.surname, 60, stdin); //Input for the surname
    printf("Flyer ID: ");
    fgets(flyeridHolder, 40, stdin); //Input for a flyer id
    int y = 0;
    int a = 0;
    while (flyeridHolder[y] != '\0'){ //While the string isn't at end
        if(flyeridHolder[y] != ' '){ //While string character isn't a space
            temp.flyerid[a] = flyeridHolder[y]; //Set value
            a++;
        }
        y++;
    }
    printf("Email: ");
    fgets(emailHolder, 100, stdin); //Input for a email
    y = 0;
    a = 0;
    while (emailHolder[y] != '\0'){
        if(emailHolder[y] != ' '){
            temp.email[a] = emailHolder[y];
            a++;
        }
        y++;
    }
    for (int i = 1; i <= numCompetitor; i++){ //Loops through the number of competitors
        int found = 0;
        for (int x = 0; x < numCompetitor; x++){ //Gets each competitor
            if(c[x].id == i){ //If the number is one of the competitor ids loop
                found = 1;
                break;
            }
        }
        if(found == 0){ //If id not found
            temp.id = i; //Add to competitor id
            break;
        }
    }
    //Removes \n from the fgets
    temp.firstName[strcspn(temp.firstName, "\n")] = 0;
    temp.surname[strcspn(temp.surname, "\n")] = 0;
    temp.flyerid[strcspn(temp.flyerid, "\n")] = 0;
    temp.email[strcspn(temp.email, "\n")] = 0;
    c[numCompetitor] = temp; //Adds temp competitor to array
    numCompetitor++; //Increases number of competitors
}

//Removes competitor
void removeCompetitor(struct Competitor *competitors){
    int idVal;
    print(competitors);
    printf("\nEnter the competitor ID of who you want to remove: ");
    scanf("%d", &idVal); //Takes an id
    for (int i = 0; i < numCompetitor; i++){ //Searches through competitor array
        if(competitors[i].id == idVal){ //If competitor id matches entered id
            for(int x = i; x < numCompetitor; x++){
                competitors[x] = competitors[x + 1]; //Reassign each competitor above the removed one to one position below
            }
            numCompetitor--; //Reduce number of competitors
            break;
        }
    }
}

//Make a new event file
void newEvent(){
    char file[50];
    printf("Enter a file name: ");
    scanf("%s", file); //Take in file name
    if (access(file, F_OK) == 0){ //If file already exists
        int choice;
        printf("\nFile already exists\n");
        printf("Would you like to remake file? (1-Yes, 0-No)\n");
        scanf("%d", &choice); //Gives user choice if they want to replace the file
        if(choice == 0) {
            return;
        }
    }
    FILE *fp;
    fp = fopen(file, "w"); //Makes file
    char line[100];
    printf("Enter the event name: ");
    scanf("%c", (char *) stdin); //Buffer
    fgets(line, 100, stdin);  //Input for the event name
    line[strcspn(line, "\n")] = 0;
    fputs(line, fp); //Puts data in file
    printf("Enter the event date 'dd/mm/yyyy': ");
    fgets(line, 100, stdin); //Input for the date
    line[strcspn(line, "\n")] = 0;
    fputs("\n", fp); //Puts a new line in file
    fputs(line, fp); //Puts data in file
}

//Saves the competitor array to the file
void saveTOFile(char *file, struct Competitor* competitors) {
    FILE *fp;
    fp = fopen(file, "w"); //Make file writable
    fputs(event, fp); //Puts event in file
    fputs("\n", fp);
    fputs(date, fp); //Puts date in file
    for (int i = 0; i < numCompetitor; i++) { //Loops through competitors and puts all their data
        fputs("\n", fp);
        fputs(competitors[i].firstName, fp);
        fputs("\n", fp);
        fputs(competitors[i].surname, fp);
        fputs("\n", fp);
        fputs(competitors[i].email, fp);
        fputs("\n", fp);
        fprintf(fp, "%d", competitors[i].id);
        fputs("\n", fp);
        fputs(competitors[i].flyerid, fp);
    }
}

//Loads Competitors
void loadCompetitors(FILE *fp, struct Competitor *competitors) {

    struct Competitor temp; //Makes a temp competitor
    fgets(event, 200, fp); //Gets event
    fgets(date, 200, fp); //Gets data
    event[strcspn(event, "\n")] = 0;
    date[strcspn(date, "\n")] = 0;
    while (!feof(fp)) { //While file is not empty
        int i = 0;
        while (!feof(fp)) {
            fgets(temp.firstName, 60, fp); //Gets remaining data
            fgets(temp.surname, 60, fp);
            fscanf(fp, "%s %d %s ", temp.email, &temp.id, temp.flyerid);
            temp.firstName[strcspn(temp.firstName, "\n")] = 0;
            temp.surname[strcspn(temp.surname, "\n")] = 0;
            competitors[i] = temp; //Puts temp in competitor array
            numCompetitor++; //Increases number of competitors
            i++; //Next competitor
        }
    }
    printf("\nFile loaded\n");
}

//Menu
void menu(char *file, struct Competitor *competitors) {
    while (1) {
        int select;
        printf("\nMENU"); //Displays menu options
        printf("\n1: List competitors\n");
        printf("2: Add competitor\n");
        printf("3: Remove competitor\n");
        printf("4: Save changes\n");
        printf("5: Create new event\n");
        printf("6: 'EXTENDED FEATURE' List competitors\n");
        printf("7: 'EXTENDED FEATURE' List competitor contact details\n");
        printf("0: Eit program\n");
        scanf("%d", &select);
        switch (select) { //Calls the requested case
            case 1:
                print(competitors);
                break;
            case 2:
                return;
            case 3:
                removeCompetitor(competitors);
                break;
            case 4:
                saveTOFile(file , competitors);
                break;
            case 5:
                newEvent();
                break;
            case 6:
                for (int i = 0; i < numCompetitor; i++){ //Runs the size maximum name calculator
                    size_t num = strlen(competitors[i].firstName) + strlen(competitors[i].surname);
                    if (num > max){
                        max = num;
                    }
                }
                printf("\nCompetition: %s  Date: %s.\n", event, date); //Prints the title
                printf("NAME          Competitor   Flyer ID       Email\n");
                printf("======================================================\n");
                printTraversal(competitors, printTable);
                break;
            case 7:
                printf("\nCompetition: %s  Date: %s.\n", event, date); //Prints the title
                printf("Competitor Contact Details.\n\n");
                printTraversal(competitors, printList);
                break;
            default:
                printf("Goodbye");
                quit = 1;
                return;
        }
    }
}



int main() {

    struct Competitor* competitors;
    char file[50];

    printf("Type a file\n");
    scanf("%s", file); //Takes a competitor file input

    if (access(file, F_OK) != 0){ //If file doesn't exist exit
        printf("\nFile not found");
        exit(1);
    }

    FILE *fp; //Makes a file pointer
    fp = fopen(file, "r"); //Open file

    if (fp == NULL) { //If file empty exit
        printf("Empty file");
        exit(1);
    }
    fgetc(fp); //Skips the name and date
    fgetc(fp);
    int i = 0;

    //Counts the number of competitors in the file
    while(!feof(fp))
    {
        int character = fgetc(fp);
        if(character == '\n')
        {
            i++;
        }
        if(i == 5){
            competitorSize++;
            i = 0;
        }
    }
    competitors = calloc(competitorSize, sizeof(struct Competitor)); //Make array the size of the number of competitors
    if (competitors == NULL){
        printf("Error implementing");
    }
    rewind(fp); //Resets the file pointer
    //Loads functions
    loadCompetitors(fp, competitors);
    menu(file, competitors);

    //Increases the size of the competitor array
    while(quit == 0) {
        if (numCompetitor == competitorSize) {
            competitorSize = competitorSize + 1;
            competitors = realloc(competitors, competitorSize * sizeof(struct Competitor));
        }
        addCompetitor(competitors);
        menu(file, competitors); //Opens up the menu again
    }
    free(competitors); //Closes the array

    return 0;
}
