#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "function.h"

size_t competitorSize = 0; //The size of the competitor array
int numCompetitor = 0; //The number of competitors
int numTracks = 0; //The number of tracks
char event[50]; //The event name
char date [20]; //The date of the event

//Calculates the total score of a competitor
int totalCalculator(char value[10], struct Competitor *competitors, int pos) {

    //Holds the time values
    char min[2];
    char sec[3];
    char mil[2];
    int a = 0;
    int b = 0;
    while (value[a] != ':') { //Finds the minutes
        min[b] = value[a];
        a++;
        b++;
    }
    a++;
    b = 0;
    while (value[a] != '.') { //Finds the seconds
        sec[b] = value[a];
        a++;
        b++;
    }
    a++;
    b = 0;
    while (value[a] != '\0') { //Finds the milliseconds
        mil[b] = value[a];
        a++;
        b++;
    }
    int trueMin = 0;
    int trueSec = 0;
    int trueMil = 0;
    //Converts the times into integers
    trueMin = atoi(min);
    trueSec = atoi(sec);
    trueMil = atoi(mil);

    return (((trueMin * 60) + (trueSec)) * 100) + trueMil; //Calculation for the total time in milliseconds
}

//Adds the value returned from totalCalculator to the total competitor time
void addTotalCal(char compResult[10], struct Competitor *competitors, int pos){
    competitors[pos].totalScore = competitors[pos].totalScore + totalCalculator(compResult, competitors, pos);
}

//Runs through all competitors and sets their total score
void setCompetitorTotal(struct Competitor *competitors, struct Track *listTracks){
    for(int i = 0; i < numCompetitor; i++) {
        competitors[i].totalScore = 0; //Sets the competitor score to 0
        int count = 0;
        for (int x = 0; x < numTracks; x++) { //Loops through the number of tracks
            track_pointer ptr = competitors[i].head; //Points to the competitors first track result
            while (ptr != NULL) {
                if (strcmp(ptr->trackName, listTracks[x].trackName) == 0) { //If the competitors track and the track in the list of tracks match
                    count++;
                    if (strcmp(ptr->result, "0:00.0") == 0) { //If their result is 0 minutes then reduce count
                        count--;
                        break;
                    }
                    addTotalCal(ptr->result, competitors, i); //Add result to total
                }
                ptr = ptr->next; //Next competitor track data
            }
        }
        if (count!=numTracks){ //If they don't have data for each track
            competitors[i].totalScore = 0; //Set score to 0
        }
    }
}

//Creates a new track to be added to the competitor
track_pointer makeTrackResult(char trackName[20], char compResult[10]) {
    track_pointer new_track; //Creates pointer for new track
    new_track = malloc(sizeof(track)); //Sets size of the track to be the size of a track
    strcpy(new_track->trackName, trackName); //Sets name
    strcpy(new_track->result, compResult); //Sets result
    new_track->next = NULL; //Makes the tracks next equal NULL
    return new_track; //Returns the new track
}

//Inserts the new track to the competitor
void insertAtTop(struct Competitor *competitors, int pos, track_pointer new_track){
    new_track->next = competitors[pos].head; //Makes the new track point at the current head
    competitors[pos].head = new_track; //Sets the new track to the top of the competitors linked list
}

//Compares the name of tracks to order them alphabetically
int cmpTrackName (void * name1, void * name2){
    //Creates temporary tracks
    track *n1;
    track *n2;
    n1 = (track *)name1;
    n2 = (track *)name2;
    char c1n[60];
    char c2n[60];

    //Gets the track names
    strcpy(c1n, n1->trackName);
    strcpy(c2n, n2->trackName);

    //Compares the track names
    if (strcmp(c1n, c2n) < 0){
        return -1;
    }
    else if (strcmp(c1n, c2n) > 0){
        return 1;
    }
    return 0;
}

//Compares the competitor total scores
int cmpScores (void * score1, void * score2){
    //Creates temporary competitors
    competitor *n1;
    competitor *n2;
    n1 = (competitor *)score1;
    n2 = (competitor *)score2;
    int c1s;
    int c2s;

    //Gets the total score from each competitor
    c1s = n1->totalScore;
    c2s = n2->totalScore;

    //Compares them
    if (c1s < c2s){
        //Sets the scores of 0 to return to the bottom
        if(c1s == 0){
            return 1;
        }
        return -1;
    }
    else if (c1s > c2s){
        //Sets the scores of 0 to return to the bottom
        if(c2s == 0){
            return -1;
        }
        return 1;
    }
    return 0;
}

//Makes a list of tracks
void makeTrackList(struct Competitor *competitors, struct Track *listTracks){
    for (int i = 0; i < numCompetitor; i++) { //Loops through each competitor
        track_pointer ptr = competitors[i].head; //Pointer to each competitor's head
        while (ptr != NULL) { //While they have tracks
            int included = 0;
            for (int x = 0; x < numCompetitor; x++) {
                if (strcmp(ptr->trackName, listTracks[x].trackName) == 0) { //Looks to see if track has been added to list
                    included = 1;
                    break;
                }
            }
            if (included == 0) { //If track is not in list
                strcpy(listTracks[numTracks].trackName, ptr->trackName); //Add track
                numTracks++; //Increase number of tracks
            }
            ptr = ptr->next; //Next track
        }
    }
}

//Prints out the competitors with their score on each track
void print(struct Competitor *competitors){
    struct Track *listTracks; //Makes an array of tracks
    listTracks = malloc(sizeof(char) * 30); //Sets the size of the track array
    int width = 10; //Exists as a way to determine the distance from the competitor name and id
    int max = 0; //The largest competitor name
    for (int i = 0; i < numCompetitor; i++){
        size_t num = strlen(competitors[i].name); //Get length of name
        if (num > max){ //Compare name length to max name length
            max = num; //Replaces name length
        }
    }
    printf("\nCompetition: %s  Date: %s\n", event, date);
    printf("NAME          Competitor   ");

    numTracks = 0; //Sets number of tracks to 0
    makeTrackList(competitors, listTracks); //Makes a list of tracks
    qsort(listTracks, numTracks, sizeof(struct Track), (__compar_fn_t) cmpTrackName); //Sorts the tracks by name
    setCompetitorTotal(competitors, listTracks); //Gets the total score of each competitor
    qsort(competitors, numCompetitor, sizeof(struct Competitor), (__compar_fn_t) cmpScores); //Sorts the scores by size

    for (int i = 0; i < numTracks; i++){
        printf("%s    ", listTracks[i].trackName); //Prints each track name out
    }
    printf("Total\n");
    printf("==================================");
    for (int i = 0; i < numTracks; i++){
        printf("========="); //Adds to the length of the heading line
    }

    for(int i = 0; i < numCompetitor; i++) {
        width = 5 + max - (strlen(competitors[i].name)); //Sets the width for competitor name and id
        printf("\n%s %*d", competitors[i].name, width, competitors[i].id); //Prints competitor name and id
        for (int x = 0; x < numTracks; x++) {
            track_pointer ptr = competitors[i].head; //Points to current competitor head
            while (ptr != NULL) {
                //prints data for each competitor track matching the track name
                if (strcmp(ptr->trackName, listTracks[x].trackName) == 0) {
                    printf("    %s", ptr->result);
                    break;
                }
                ptr = ptr->next; //Next track
            }
            //If no track data output 0
            if (ptr == NULL) {
                printf("    0:00.0");
            }
        }
        int min;
        int sec;
        int mil;
        //Calculations for each time value
        mil = competitors[i].totalScore % 100;
        sec = ((competitors[i].totalScore - mil) / 100) % 60;
        min = ((competitors[i].totalScore - mil) / 100) / 60;
        printf("    %d:%d.%d", min, sec, mil);

    }
}

//Lets user add results to a competitors track linked list manually
void manualResultAdd(struct Competitor *competitors) {
    print(competitors); //Shows user list of competitors
    int id = 0;
    char trackTemp[20]; //Temporary track name holder
    int pos = -1; //Position of the competitor
    char newResult[10]; //The new data to add
    printf("\n\nEnter the competitor ID: ");
    scanf("%d", &id);
    //Finds competitor user enters
    for (int i = 0; i < numCompetitor; i++) {
        if (competitors[i].id == id) {
            pos = i; //Sets position
            break;
        }
    }
    //If no competitor was found
    if (pos < 0) {
        printf("\nCouldn't Find competitor");
        return;
    }
    track_pointer ptr = competitors[pos].head; //Pointers to found competitors head
    printf("\nEnter the track name: ");
    scanf("%s", trackTemp);
    //Searches through the competitors tracks they've already got
    while (1) {
        if (ptr == NULL){ //If they have no data break
            break;
        }

        else if (strcmp(ptr->trackName, trackTemp) == 0) { //If the pointer's track name is the same as the track to be added
            if (strcmp(ptr->result, "0:00.0") != 0) { //If the data is not 0
                //Ask the user if they are sure they want to update the data
                int ans = 0;
                printf("\nThis competitor already has data for this track: %s", ptr->result);
                printf("\nDo you want to change the data? (1-yes, 0-no)\n");
                scanf("%d", &ans);
                if (ans == 0) {
                    return;
                }

            }
            break;
        } else if (ptr->next == NULL) { //Break if the next piece of data is empty
            break;
        }
        ptr = ptr->next; //Next track
    }

    //Allows user to enter new details
    char min[3];
    char sec[3];
    char secHolder[3];
    char mil[1];
    printf("\nEnter result minutes: ");
    scanf("%s", min);
    printf("\nEnter result secs: ");
    scanf(" %s", sec);
    strcpy(secHolder, sec);
    printf("\nEnter result mil: ");
    scanf("%s", mil);
    //Adds the symbols between the time values
    strcpy(newResult, (strcat(min, ":")));
    strcpy(newResult, (strcat(newResult, sec)));
    strcpy(newResult, (strcat(newResult, ".")));
    strcpy(newResult, (strcat(newResult, mil)));
    printf("%s", newResult); //Prints the new value
    //If track exists replace data
    if (ptr != NULL && strcmp(ptr->trackName, trackTemp) == 0) {
        strcpy(ptr->result, newResult);
    }
    //Else insert a new track at the top of the linked list
    else {
        insertAtTop(competitors, pos, makeTrackResult(trackTemp, newResult));
    }
}

//Load a track file
void loadTracks(FILE *fp, struct Competitor *competitors) {
    char track[20];
    char time[10];
    char line[100];
    fgets(track, 200, fp); //Gets the track name
    fgets(line, 200, fp);
    //Removes the new line from fgets
    track[strcspn(track, "\n")] = 0;
    line[strcspn(line, "\n")] = 0;

    if(strcmp(line, date) != 0){ //Compares the date of tracks and the competitors
        printf("\nEvent date and race date are different"); //Tells the user if they are different
    }
    fgets(time, 200, fp); //Sets the time the race happened
    time[strcspn(time, "\n")] = 0;

    while(fgets(line, 1000, fp)!=NULL){ //While there's a new line
        int i = 0;
        char tempCompID[10];
        int compID;
        char compResult[10];
        memset(tempCompID, 0, 10);
        memset(compResult, 0, 10);

        //Gets the data from each track result
        while (line[i] != ',') {
            tempCompID[i] = line[i];
            i++;
        }
        i++;
        int y = 0;
        while (line[i] != '\0'){
            compResult[y] = line[i];
            i++;
            y++;
        }
        compID = atoi(tempCompID); //converts the id to an integer
        compResult[strcspn(compResult, "\n")] = 0;

        for (int x = 0; x < numCompetitor; x++){ //Loops through each competitor
            if (competitors[x].id == compID) {
                track_pointer ptr = competitors[x].head; //Makes a pointer to the current competitors head
                int skip = 0; //If the data is not added
                while (ptr != NULL) {
                    //If competitor already has data
                    if (strcmp(ptr->trackName, track) == 0) {
                        skip = 1; //Skip make new track
                        if (strcmp(ptr->result, "0:00:0") == 0 || strcmp(ptr->result, compResult) == 0) {} //If data is equal to 0
                        else {
                            //Ask user if they want to replace the current data
                            int ans = 0;
                            printf("\nCompetitor %s already has data for this track: %s", competitors[x].name, ptr->result);
                            printf("\nDo you want to change the data to: %s? (1-yes, 0-no)\n", compResult);
                            scanf("%d", &ans);
                            if (ans == 0) {
                                break;
                            }
                        }
                        strcpy(ptr->result, compResult); //Replace current data
                        break;
                    }
                    ptr = ptr->next; //Next competitor track
                }
                if (skip == 0) {
                    insertAtTop(competitors, x, makeTrackResult(track, compResult)); //Make a new track
                    break;
                }
            }
        }
    }
}

//Load a competitor file
void loadCompetitors(FILE *fp, struct Competitor *competitors){
    struct Competitor temp; //Make a temporary competitor
    fgets(event, 200, fp); //Adds the event name to event
    fgets(date, 200, fp); //Adds the date to date
    //Removes the new line from fgets
    event[strcspn(event, "\n")] = 0;
    date[strcspn(date, "\n")] = 0;
    char firstName[60];
    char surname[60];
    char email[100];
    char flyID[20];
    while (!feof(fp)) { //While files not empty
        int i = 0; //The competitor position
        while (!feof(fp)) {
            //Gets data from file
            fgets(firstName, 60, fp);
            fgets(surname, 60, fp);
            firstName[strcspn(firstName, "\n")] = 0;
            surname[strcspn(surname, "\n")] = 0;
            //Creates a full name
            strcat(firstName, " ");
            strcpy(temp.name, strcat(firstName, surname));
            fgets(email, 100, fp);
            fscanf(fp, "%d ", &temp.id);
            fgets(flyID, 100, fp);
            temp.totalScore = 0; //Sets score to 0
            temp.head = NULL; //Sets their tracks to null
            competitors[i] = temp; //Adds competitor to array
            numCompetitor++; //Increases number of competitors
            i++; //Moves to next position
        }
    }
}

//Menu
void menu(FILE *fp, char *file, struct Competitor *competitors) {
    while (1) {
        int select;
        //Displays menu options
        printf("\n\nMENU");
        printf("\n1: Manually enter competition results\n");
        printf("2: Load competition results file\n");
        printf("3: Display results table\n");
        printf("0: Eit program\n");
        scanf("%d", &select);
        switch (select) { //Calls the requested case
            case 1:
                manualResultAdd(competitors);
                break;
            case 2:
                //Takes a race file name
                printf("Enter race file name: ");
                scanf("%s", file);
                if (access(file, F_OK) != 0){ //Checks if file exists
                    printf("\nFile not found");
                    break;
                }
                fp = fopen(file, "r"); //Opens file
                loadTracks(fp, competitors);
                break;
            case 3:
                print(competitors);
                break;
            default:
                printf("Goodbye");
                return;
        }
    }
}

int main() {

    char file[50];
    //Gets a competitor file
    printf("Enter competitor file name: ");
    scanf("%s", file);
    //Checks if file exists
    if (access(file, F_OK) != 0){
        printf("\nFile not found");
        exit(1);
    }

    FILE *fp;
    fp = fopen(file, "r"); //Opens file

    //Checks if file is empty
    if (fp == NULL) {
        printf("Empty file");
        exit(1);
    }
    //Skips the name and date
    fgetc(fp);
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

    struct Competitor* competitors;
    competitors = calloc(competitorSize, sizeof(struct Competitor)); //Make array the size of the number of competitors
    if (competitors == NULL){
        printf("Error implementing");
    }
    rewind(fp); //Resets the file pointer
    //Loads functions
    loadCompetitors(fp, competitors);
    menu(fp, file, competitors);
    free(competitors); //Closes the array
    return 0;
}
