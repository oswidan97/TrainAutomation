
#include <sys/types.h>

typedef struct Station
{
    int waitingPassengersNum;
    int availableTrainSeatNum;
    int train_in_station;
    pthread_mutex_t station_lock;
    pthread_cond_t train_in_station_cond;
    pthread_cond_t train_loaded;
    pthread_cond_t next_train;


}Station;
typedef struct pair{
    void * first;
    void * second;
}pair;
void station_init(Station * station);
void station_load_train(Station *station, int count);
void station_wait_for_train(Station *station);
void station_on_board(Station *station);
void* train(void* args);
void* passenger(void* args);

