#include <stdio.h>
#include <pthread.h> //нужен флаг -pthread при компиляции
 
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;//условвная переменная
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;//мьютекс
bool ready = false;
 
void *provide(void *){//поток-поставщик
    while(1){//имитация долгого выполнения программы, содержащей поток-поставщик
        pthread_mutex_lock(&lock); //блокировка мьютекса
        if(ready){
            pthread_mutex_unlock(&lock);
            continue;
        }
        ready = true; // действие для наступления события
        printf("provided\n");//уведомление для нас
        pthread_cond_signal(&cond1); // уведомление для потока-потребителя о наступлении события
        pthread_mutex_unlock(&lock); //освобождение мьютекса
        
    }
    return NULL;
}
 
void *consume(void *)
{
    while(1){//имитация долгого выполнения программы, содержащей поток-потребитель
        pthread_mutex_lock(&lock); //блокировка мьютекса
        if (!ready)//проверка наступления события
        {

            pthread_cond_wait(&cond1, &lock);//ожидание наступления события 
                                            //с временным освобождением мьютекса
            printf ("awoke\n");//уведомление для нас
        }
        ready = false; //обработка события
        printf ("consumed\n");//уведомление о проделанной операции для нас
        pthread_mutex_unlock(&lock); //окончательное освобождение мьютекса
    }
    return NULL;
}
 
int main() {
    pthread_t provider;
    pthread_t user;
 
    pthread_create(&provider, NULL, provide, NULL);//создание потока
    pthread_create(&user, NULL, consume, NULL);

    pthread_join(provider, NULL);//ожидает завершение потока
    pthread_join(user, NULL);
 
    return 0;
}
