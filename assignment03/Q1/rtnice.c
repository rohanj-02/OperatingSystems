#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <asm/errno.h>

SYSCALL_DEFINE2(rtnice, pid_t, pid, long long, srt_val)
{
	struct task_struct *task;

	//Error EINVAL 22 Invalid Argument
	if (pid < 0 || srt_val < 0)
	{
		return -1 * EINVAL;
	}

	//Returns -ve on error and 0 on success
	printk(KERN_INFO "Running rtnice syscall\n");

	//Getting task_struct from PID
	task = pid_task(find_vpid(pid), PIDTYPE_PID);

	//Error ESRCH 3 No such process
	if (task == NULL)
	{
		printk(KERN_ERR "Process with PID = %d was not found!\n", pid);
		return -1 * ESRCH;
	}

	//Set soft_realtime_value of pid's task to given value
	task->se.soft_realtime_nice = (u64)(srt_val);
	printk("Value of soft_realtime_nice: %llu\n", task->se.soft_realtime_nice);
	return 0;
}
