//
// Created by jayst on 14/12/2021.
//

#ifndef CDP_FUNCTION_H
#define CDP_FUNCTION_H

typedef struct Track * track_pointer; //Makes a pointer to a track

//Used for making a new track
typedef struct Track {
    char trackName[20]; //Track name
    char result[10]; //Result on each track
    track_pointer next; //Pointer to a track
} track;

//Used for making a new competitor
typedef struct Competitor {
    char name[120]; //Competitor name
    int id; //Competitor id
    int totalScore; //Score from all tracks added up
    track_pointer head; //Pointer to a track
} competitor;

#endif //CDP_FUNCTION_H
