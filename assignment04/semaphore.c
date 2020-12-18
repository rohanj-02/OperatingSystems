#include <pthread.h>
typedef struct my_blocking_semaphore sem_b;

// TODO Error handling

struct my_blocking_semaphore
{
	pthread_mutex_t lock;
	pthread_cond_t wait;
	int value;
	int num_waiters;
};

sem_b *init_sem_blocking(int initial_value)
{
	sem_b *my_sem;
	my_sem = (sem_b *)malloc(sizeof(sem_b));

	if (my_sem == NULL)
	{
		return NULL;
	}

	my_sem->value = initial_value;
	pthread_mutex_init(&(my_sem->lock), NULL);
	pthread_cond_init(&(my_sem->wait), NULL);
	my_sem->num_waiters = 0;

	return my_sem;
}

void signal_blocking(sem_b *s)
{
	pthread_mutex_lock(&(s->lock));
	s->value++;

	if (s->value <= 0)
	{
		pthread_cond_signal(&(s->wait));
	}

	pthread_mutex_unlock(&(s->lock));
	return;
}

void wait_blocking(sem_b *s)
{
	pthread_mutex_lock(&(s->lock));
	s->value--;

	while (s->value < 0)
	{
		if (s->num_waiters >= (-1 * s->value))
		{
			break;
		}
		else
		{
			s->num_waiters++;
			pthread_cond_wait(&(s->wait), &(s->lock));
			s->num_waiters--;
		}

		pthread_mutex_unlock(&(s->lock));
	}

	return;
}
