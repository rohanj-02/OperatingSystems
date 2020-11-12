# OS-Assignment-3

## Modifying CFS Scheduler

### Error Handling
1. EINVAL thrown when pid < 0 or soft_realtime < 0.
2. ESRCH thrown when task_struct corresponding to pid not found.
