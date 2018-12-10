#include <pthread.h>
#include <stdio.h>
#include "train.h"

void station_init(Station * station)
{
    //initialize the condition variables, the mutex lock and the state variables
    pthread_mutex_init(&station->station_lock , NULL);
    pthread_cond_init(&station->train_in_station_cond, NULL);
    pthread_cond_init(&station->train_loaded,NULL);
    pthread_cond_init(&station->next_train,NULL);
    station->train_in_station=0;
    station->waitingPassengersNum=0;
}
void *train(void *args) {

    Station* station1=(Station*)((struct train_args*) args)->station;
    int number_of_seats=(int)((struct train_args*) args)->number_of_seats;

    //call the function to start loading the train with passengers
    station_load_train(station1,number_of_seats);
    pthread_exit(NULL);
}
void station_load_train(Station *station, int count)
{
    pthread_mutex_lock(&station->station_lock);
    while( station->train_in_station == 1)//if there's a train in station, then wait for it to leave
        pthread_cond_wait(&(station->next_train), &(station->station_lock));//wait on the condition variable of the next train

    station->availableTrainSeatNum = count;//assign the number of seats
    station->train_in_station = 1;//a train is in the station
    printf("train arrived with %d seats available\n",count);

    pthread_cond_broadcast(&(station->train_in_station_cond));//signal all waiting passengers that the train has arrived

    printf("signaled passengers\n");
    //if there's still available seats left and waiting passengers then the train should wait on the condition variable for them to board
    while( (station->availableTrainSeatNum > 0)  && (station->waitingPassengersNum > 0))
        pthread_cond_wait(&(station->train_loaded),&station->station_lock);

    station->train_in_station = 0;//there're no seats left or waiting passengers so the train would leave
    printf("train leaving station with seats left =%d\n",station->availableTrainSeatNum);

    pthread_cond_signal(&(station->next_train));//signal the next train's condition varaible
    pthread_mutex_unlock(&station->station_lock);//unlock the mutex for either a train or a mutex to acquire it


}
void *passenger(void *args) {

    Station* station1=(struct Station*)args;

    station_wait_for_train(station1);//call the function to wait for the train's arrival
    station_on_board(station1);
    pthread_exit(NULL);
}
void station_wait_for_train(Station *station)
{
    pthread_mutex_lock(&station->station_lock);
    station->waitingPassengersNum++;//inncrement the number of waiting passengers
    printf("waiting passneger %d\n",station->waitingPassengersNum);
    //if there's no train in the station or no available seats, then wait on the train in station condition variable
    while( station->train_in_station == 0 || station->availableTrainSeatNum==0)
        pthread_cond_wait(&(station->train_in_station_cond), &(station->station_lock));

   // if(station->waitingPassengersNum > 0)
        station->waitingPassengersNum--;//the train  is in the station and there're available seats then decrement the waiting passengers number

}

void station_on_board(Station *station)
{

    station->availableTrainSeatNum--;//decrement the available seat number as a passenger has boarded
    printf("passenger boarded new available seats=%d and waiting passengers=%d\n",station->availableTrainSeatNum,station->waitingPassengersNum);

    pthread_cond_signal(&(station->train_loaded));
    pthread_mutex_unlock(&station->station_lock);
}