

#include <pthread.h>
#include <stdio.h>
#include "train.h"


int main() {
    Station station1;
    int number_of_trains,number_of_passengers;
    printf("enter the number of trains ");
    scanf("%d",&number_of_trains);
    int number_of_seats[number_of_trains];
    printf("enter the number of seats of each train ");
    for (int i = 0; i < number_of_trains; ++i)
        scanf("%d",&number_of_seats[i]);

    printf("enter the number of passengers ");
    scanf("%d",&number_of_passengers);
    station_init(&station1);
    pthread_t trains[number_of_trains],passengers[number_of_passengers];

    for (int i = 0; i < number_of_passengers; ++i)
        pthread_create(&passengers[i],NULL,passenger,&station1);

    for (int i = 0; i < number_of_trains; ++i)
        pthread_create(&trains[i],NULL,train,&station1);

    for (int i = 0; i < number_of_passengers; ++i)
        pthread_join(passengers[i],NULL);
    for (int i = 0; i < number_of_trains; ++i)
        pthread_join(trains[i],NULL);

    return 0;
}