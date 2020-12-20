/*	Name: Rohan Jain
	Roll No.: 2019095 */

#include <pthread.h>
#ifndef SEMAPHORE_H
#define SEMAPHORE_H

struct my_semaphore
{
    pthread_mutex_t lock;
    pthread_cond_t cond;
    int count;
    int num_waiters;
};

typedef struct my_semaphore sem_b;

int init_sem(sem_b *my_sem, int initial_count);
int signal_blocking(sem_b *s);
int wait_blocking(sem_b *s);
int print_count_blocking(sem_b *s);
int signal_nonblocking(sem_b *s);
int wait_nonblocking(sem_b *s);
int print_count_nonblocking(sem_b *s);

#endif
