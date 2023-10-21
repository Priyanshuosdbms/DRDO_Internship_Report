#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#define ui_Size 100

int i_Buffer1[ui_Size];
int i_Buffer2[ui_Size];

pthread_mutex_t tu_Mutex;
sem_t tu_Semaphore1, tu_Semaphore2;

void *sr_Thread1(void *arg)
{
    int ui_I, ui_J, ui_Loopcounter, i_Array1[4];
    ui_J = 0;
    ui_Loopcounter = 0;

    // loop running indefintely , has to stopped by the user
    while (1)
    {
        for (ui_I = 0; ui_I < 4; ui_I++)
        {
            i_Array1[ui_I] = ui_Loopcounter;
        }
        // Locking the mutex
        pthread_mutex_lock(&tu_Mutex);

        for (ui_I = 0; ui_I < 4; ui_I++)
        {
            i_Buffer1[ui_J] = i_Array1[ui_I]; // Transferring data from array1 to the buffer assigned for transferring to Thread 2
            ui_J++;
        }

        pthread_mutex_unlock(&tu_Mutex);
        // Unlock a mutex.
        if (ui_J >= ui_Size)
        {
            ui_J = 0;
        }
        printf("LoopCounter of thread A :%d\n------------------------\n", ui_Loopcounter);
        ui_Loopcounter++;
        usleep(250000);
        sem_post(&tu_Semaphore1);
    }
    pthread_exit(NULL);
}

void *sr_Thread2(void *arg)
{
    int ui_I, ui_J, ui_Loopcounter, i_Array2[4];
    ui_J = 0;
    ui_Loopcounter = 0;

    while (1)
    {
        sem_wait(&tu_Semaphore1);
        // Locking the mutex
        pthread_mutex_lock(&tu_Mutex);

        for (ui_I = 0; ui_I < 4; ui_I++)
        {
            i_Array2[ui_I] = i_Buffer1[ui_J]; // Transferring data from i_Buffer1 to array2
            i_Buffer2[ui_J] = i_Array2[ui_I]; // Transferring data from array2 to the buffer assigned for transferring to Thread 3
            ui_J++;
        }

        pthread_mutex_unlock(&tu_Mutex);
        // Unlocking the mutex
        if (ui_J >= ui_Size) // Buffer reached its max ui_Size

        {
            ui_J = 0; // Re-initializing the value of j and over-writing data in it
        }

        for (ui_I = 0; ui_I < 4; ui_I++)
        {
            printf("| Array from A to B: %d|\n", i_Array2[ui_I]);
        }
        printf("------------------------\nLoopCounter of thread B :%d\n------------------------\n", ui_Loopcounter);
        ui_Loopcounter++;
        usleep(250000);
        sem_post(&tu_Semaphore2);
    }
    pthread_exit(NULL);
}

void *sr_Thread3(void *arg)
{
    int ui_I, ui_J, ui_Loopcounter, i_Array3[4];
    ui_Loopcounter = 0;
    ui_J = 0;

    while (1)
    {
        sem_wait(&tu_Semaphore2);
        // Locking the mutex
        pthread_mutex_lock(&tu_Mutex);

        for (ui_I = 0; ui_I < 4; ui_I++)
        {
            i_Array3[ui_I] = i_Buffer2[ui_J]; // Transferring data from i_Buffer2 to array3
            ui_J++;
        }

        pthread_mutex_unlock(&tu_Mutex);
        // Unlocking the mutex
        if (ui_J >= ui_Size) // Buffer reached its max ui_Size

        {
            ui_J = 0; // Re-initialijing the value of j and over-writing data in it
        }

        for (ui_I = 0; ui_I < 4; ui_I++)
        {
            printf("| Array from B to C: %d|\n", i_Array3[ui_I]);
        }
        printf("-------------------------\nLoopCounterof thread C :%d\n-------------------------\n", ui_Loopcounter);
        ui_Loopcounter++;
        usleep(250000);
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t ul_Thread1, ul_Thread2, ul_Thread3; // Three threads are taken here

    pthread_mutex_init(&tu_Mutex, NULL); // Initializing mutex attributes to NULL

    sem_init(&tu_Semaphore1, 0, 0); // initialize the semaphore w.r.t the parameter if its being shared among processes and its initial value
    sem_init(&tu_Semaphore2, 0, 0);

    pthread_create(&ul_Thread1, NULL, sr_Thread1, NULL); // The pthread_create() function starts a new thread in the calling process
    pthread_create(&ul_Thread2, NULL, sr_Thread2, NULL);
    pthread_create(&ul_Thread3, NULL, sr_Thread3, NULL);

    pthread_join(ul_Thread1, NULL); // The pthread_join() function waits for the thread specified by thread to terminate
    pthread_join(ul_Thread2, NULL);
    pthread_join(ul_Thread3, NULL);

    pthread_mutex_destroy(&tu_Mutex); // Destroying the mutex to free memory
    sem_destroy(&tu_Semaphore1);      // Free resources associated with semaphore object tu_Semaphore1
    sem_destroy(&tu_Semaphore2);

    return 0;
}