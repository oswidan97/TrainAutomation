

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "train.h"


int main() {
    Station* station1=malloc(sizeof(Station));//allocate the station
    int number_of_trains,number_of_passengers;
    printf("enter the number of trains ");
    scanf("%d",&number_of_trains);
    int number_of_seats[number_of_trains];
    printf("enter the number of seats of each train ");
    for (int i = 0; i < number_of_trains; ++i)
        scanf("%d",&number_of_seats[i]);

    printf("enter the number of passengers ");
    scanf("%d",&number_of_passengers);
    station_init(station1);//call the station initializer
    //declare the threads of the passengers and the trains
    pthread_t trains[number_of_trains],passengers[number_of_passengers];

    for (int i = 0; i < number_of_passengers; ++i)
        pthread_create(&passengers[i],NULL,passenger,station1);

    //create an array of arguments each for a train
    train_args args[number_of_trains];
    for (int i = 0; i < number_of_trains; ++i) {
        args[i].station=station1;
        args[i].number_of_seats=number_of_seats[i];
        pthread_create(&trains[i], NULL, train, &args[i]);

    }

    for (int i = 0; i < number_of_passengers; ++i)
        pthread_join(passengers[i],NULL);
    for (int i = 0; i < number_of_trains; ++i)
        pthread_join(trains[i],NULL);

    free(station1);
    return 0;
}