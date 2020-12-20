/*	Name: Rohan Jain
	Roll No.: 2019095 */

#include <pthread.h>
#include <errno.h>
#include <stdio.h>

typedef struct my_semaphore sem_b;

struct my_semaphore
{
	pthread_mutex_t lock;
	pthread_cond_t cond;
	int count;
	int num_waiters;
};

/**
 * @brief Initialises a counting semaphore
 * 
 * @param my_sem The semaphore to be initialised of type my_semaphore
 * @param initial_count The initial count of the counting semaphore
 * @return int 0 on success and -1 on error with errno set
 */
int init_sem(sem_b *my_sem, int initial_count)
{
	my_sem->count = initial_count;
	if (pthread_mutex_init(&(my_sem->lock), NULL) != 0)
	{
		perror("pthread_mutex_init(): error");
		return -1;
	}
	if (pthread_cond_init(&(my_sem->cond), NULL) != 0)
	{
		perror("pthread_cond_init(): error");
		return -1;
	}
	my_sem->num_waiters = 0;
	return 0;
}

/**
 * @brief The function signal() or V() for the my_semaphore structure. It is a blocking function.
 * 
 * @param s The semaphore to signal or increment
 * @return int 0 on success and -1 on error with errno set
 */
int signal_blocking(sem_b *s)
{
	if (pthread_mutex_lock(&(s->lock)) != 0)
	{
		perror("pthread_mutex_lock(): error");
		return -1;
	}
	s->count++;

	if (s->count <= 0)
	{
		if (pthread_cond_signal(&(s->cond)) != 0)
		{
			perror("pthread_cond_signal(): error");
			return -1;
		}
	}

	if (pthread_mutex_unlock(&(s->lock)) != 0)
	{
		perror("pthread_mutex_unlock(): error");
		return -1;
	}
	return 0;
}

/**
 * @brief The function wait() or P() for the my_semaphore structure. It is a blocking function.
 * 
 * @param s The semaphore to wait or decrement
 * @return int 0 on success and -1 on error with errno set
 */
int wait_blocking(sem_b *s)
{
	if (pthread_mutex_lock(&(s->lock)) != 0)
	{
		perror("pthread_mutex_lock(): error");
		return -1;
	}
	s->count--;

	while (s->count < 0)
	{
		if (s->num_waiters >= (-1 * s->count))
		{
			break;
		}
		else
		{
			s->num_waiters++;
			if (pthread_cond_wait(&(s->cond), &(s->lock)) != 0)
			{
				perror("pthread_cond_wait(): error");
				return -1;
			}

			s->num_waiters--;
		}
	}
	if (pthread_mutex_unlock(&(s->lock)) != 0)
	{
		perror("pthread_mutex_unlock(): error");
		return -1;
	}

	return 0;
}

/**
 * @brief Blocking Function to print the count of a semaphore of type my_semaphore.
 * 
 * @param s The semaphore whose count is to be printed
 * @return int 0 on success and -1 on error with errno set
 */
int print_count_blocking(sem_b *s)
{
	if (pthread_mutex_lock(&(s->lock)) != 0)
	{
		perror("pthread_mutex_lock(): error");
		return -1;
	}
	printf("count of semaphore: %d\n", s->count);
	if (pthread_mutex_unlock(&(s->lock)) != 0)
	{
		perror("pthread_mutex_unlock(): error");
		return -1;
	}
	return 0;
}

/**
 * @brief The function signal() or V() for the my_semaphore structure. It is a non blocking function.
 * 
 * @param s The semaphore to signal or increment
 * @return int 0 on success and -1 on error with errno set
 */
int signal_nonblocking(sem_b *s)
{
	int ret_val = 0;
	if (ret_val = pthread_mutex_trylock(&(s->lock)) != 0)
	{
		if (ret_val == EBUSY)
		{
			return EBUSY;
		}
		perror("pthread_mutex_trylock(): error");
		return -1;
	}
	s->count++;

	if (s->count <= 0)
	{
		if (pthread_cond_signal(&(s->cond)) != 0)
		{
			perror("pthread_cond_signal(): error");
			return -1;
		}
	}

	if (pthread_mutex_unlock(&(s->lock)) != 0)
	{
		perror("pthread_mutex_unlock(): error");
		return -1;
	}
	return 0;
}

/**
 * @brief The function wait() or P() for the my_semaphore structure. It is a non blocking function.
 * 
 * @param s The semaphore to wait or decrement
 * @return int 0 on success and -1 on error with errno set
 */
int wait_nonblocking(sem_b *s)
{
	// fprintf(stderr, "In wait non block \n");
	int ret_val = 0;
	ret_val = pthread_mutex_trylock(&(s->lock));
	if (ret_val != 0)
	{
		// fprintf(stderr, "Did not get sem non block :%d\n", ret_val);
		if (ret_val == EBUSY)
		{
			return EBUSY;
		}
		else
		{
			perror("pthread_mutex_trylock(): error");
			return -1;
		}
	}
	// fprintf(stderr, "Got sem of non block \n");
	s->count--;

	if (s->count < 0)
	{
		if (pthread_cond_wait(&(s->cond), &(s->lock)) != 0)
		{
			perror("pthread_cond_wait(): error");
			return -1;
		}
	}

	if (pthread_mutex_unlock(&(s->lock)) != 0)
	{
		perror("pthread_mutex_unlock(): error");
		return -1;
	}

	return 0;
}

/**
 * @brief Non blocking Function to print the count of a semaphore of type my_semaphore.
 * 
 * @param s The semaphore whose count is to be printed
 * @return int 0 on success and -1 on error with errno set
 */
int print_count_nonblocking(sem_b *s)
{
	int ret_val = 0;
	if (ret_val = pthread_mutex_trylock(&(s->lock)) != 0)
	{
		if (ret_val == EBUSY)
		{
			return EBUSY;
		}
		perror("pthread_mutex_lock(): error");
		return -1;
	}
	printf("count of semaphore: %d\n", s->count);
	if (pthread_mutex_unlock(&(s->lock)) != 0)
	{
		perror("pthread_mutex_unlock(): error");
		return -1;
	}
	return 0;
}
