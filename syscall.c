#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/file.h>
#include <linux/fcntl.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>
#include <asm/errno.h>

#define MY_SIZE_TEMP 256
#define MY_SIZE_FILENAME 256
#define MY_SIZE_OUTPUT 1024

static int errno;

int check_file_path(char *filename){
	//Function to check if filename given is a valid filename or not.
	//Invalid filename can be a filename with / as it denotes a directory in linux
	int i = 0;
	for(i = 0; filename[i] != '\0'; i++){
		if(filename[i] == '/' || filename[i] == '\\'){
			return -1;
		}
	}
	return 0;
}


int set_output(char *output, struct task_struct *task){

	//Function to set the output string to print to kernel log and in file!
	
	int len;
	char temp[MY_SIZE_TEMP];
	len = 0;
	
	//PID
	//type pid_t or int
	len += scnprintf(output, MY_SIZE_OUTPUT, "PID: %d\n", task->pid);

    //STATE 
    //TYPE: volatile long 
	switch(task->state){
		case -1:
    		len += scnprintf(temp, MY_SIZE_TEMP, "State: %ld (Unrunnable)\n", task->state);
    		break;
    	case 0:
    		len += scnprintf(temp, MY_SIZE_TEMP, "State: %ld (TASK_RUNNING)\n", task->state);
    		break;
    	case 1:
    		len += scnprintf(temp, MY_SIZE_TEMP, "State: %ld (TASK_INTERRUPTIBLE)\n", task->state);
    		break;
    	case 2:
    		len += scnprintf(temp, MY_SIZE_TEMP, "State: %ld (TASK_UNINTERRUPTIBLE)\n", task->state);
    		break;
    	case 4: 
    		len += scnprintf(temp, MY_SIZE_TEMP, "State: %ld (TASK_ZOMBIE)\n", task->state);
    		break;
    	case 8:
    		len += scnprintf(temp, MY_SIZE_TEMP, "State: %ld (TASK_STOPPED)\n", task->state);
    		break;
    	default:
    		len += scnprintf(temp, MY_SIZE_TEMP, "State: %ld (Unknown)\n", task->state);
			break;
	}
  	strncat(output, temp, MY_SIZE_TEMP);

    //REALTIME PRIORITY
    //type: unsigned int
    len += scnprintf(temp, MY_SIZE_TEMP, "Real Time Priority: %u\n", task->rt_priority);
  	strncat(output, temp, MY_SIZE_TEMP);
    // printk(KERN_INFO "Real time Priority: %u\n", task->rt_priority);
    
    //SCHEDULING POLICY
    //type: unsigned int
    switch(task->policy){
    	case 0:
    		len += scnprintf(temp, MY_SIZE_TEMP, "Scheduling Policy: %u (SCHED_NORMAL)\n", task->policy);
    		break;
  		case 1:
    		len += scnprintf(temp, MY_SIZE_TEMP, "Scheduling Policy: %u (SCHED_FIFO)\n", task->policy);
  			break;
  		case 2:
    		len += scnprintf(temp, MY_SIZE_TEMP, "Scheduling Policy: %u (SCHED_RR)\n", task->policy);
  			break;
  		case 3:
    		len += scnprintf(temp, MY_SIZE_TEMP, "Scheduling Policy: %u (SCHED_BATCH)\n", task->policy);
    		break;
  		default:
    		len += scnprintf(temp, MY_SIZE_TEMP, "Scheduling Policy: %u (Unknown)\n", task->policy);
    		break;
    }
    strncat(output, temp, MY_SIZE_TEMP);
    // printk(KERN_INFO "Policy: %u\n", task->policy);

  	//NUMBER OF CPUs ALLOWED
	//type: int
    len += scnprintf(temp, MY_SIZE_TEMP,"Number of CPUs allowed: %d\n", task->nr_cpus_allowed);
  	strncat(output, temp, MY_SIZE_TEMP);
    // printk(KERN_INFO "Number of CPUs allowed: %d\n", task->nr_cpus_allowed);
    

    // char comm[16] for comand name
    
    //CONTEXT SWITCH COUNT
    //type: unsigned long
    len += scnprintf(temp, MY_SIZE_TEMP,"Number of context switch done: %lu\n", task->nvcsw);
  	strncat(output, temp, MY_SIZE_TEMP);
    // printk(KERN_INFO "Number of context switch done: %lu\n", task->nvcsw);
  	return len;
}

// int open_file(struct file *fileptr, char *filepath){
	
// 	//Error ENAMETOOLONG 36 File name too long
// 	if(strlen(filepath) >= 255) {
//     	printk(KERN_ERR "File name too long!");
//     	errno = ENAMETOOLONG;
//     	return -1 * errno;
//     }
//     //Error ENOENT 2 No such file or directory
//     if(check_file_path(filepath) == -1){
//     	printk(KERN_ERR "File cannot be opened!\n");
//     	errno = ENOENT;
//     	return -1 * errno;
//     }
//     //File Open
//     fileptr = filp_open(filepath, O_WRONLY | O_CREAT, 0666);
//     //Error ENOENT 2 No such file or directory
//     if(fileptr == NULL){
//     	printk(KERN_ERR "File cannot be opened!\n");
//     	errno = ENOENT;
//     	return -1 * errno;     	
//     }
//     return 0;
// }

SYSCALL_DEFINE2(sh_task_info, pid_t, pid, char*, file_name)
{
	
	struct task_struct *task;
    char filepath[MY_SIZE_FILENAME], output[MY_SIZE_OUTPUT];
    int len, x;
	struct file *fileptr;
    loff_t pos;
	ssize_t returnVal;
	
	//Returns -ve on error and 0 on success	
	printk(KERN_INFO "Running sh_task_info syscall\n");
	
	//Getting task_struct from PID
	task = pid_task(find_vpid(pid), PIDTYPE_PID); 
	
	//Error ESRCH 3 No such process
	if (task == NULL)
	{
		printk(KERN_ERR "Process with PID = %d was not found!\n", pid);
		errno = ESRCH;
		return -1 * errno;
	}

	//Generate output string
	len = set_output(output, task);
	
	//Print output to kernel log
	printk(KERN_INFO "%s\n", output);

    copy_from_user(filepath, file_name, 256);
    
    // FILE OPEN
    //Error ENAMETOOLONG 36 File name too long
	if(strlen(filepath) >= 255) {
    	printk(KERN_ERR "File name too long!");
    	errno = ENAMETOOLONG;
    	return -1 * errno;
    }
    //Error ENOENT 2 No such file or directory
    if(check_file_path(filepath) == -1){
    	printk(KERN_ERR "File cannot be opened!\n");
    	errno = ENOENT;
    	return -1 * errno;
    }
    fileptr = filp_open(filepath, O_WRONLY | O_CREAT, 0666);
    //Error ENOENT 2 No such file or directory
    if(fileptr == NULL){
    	printk(KERN_ERR "File cannot be opened!\n");
    	errno = ENOENT;
    	return -1 * errno;     	
    }
	// x = open_file(fileptr, filepath);
 //    //Error in file opening
 //    if(x != 0){
 //    	return x;
 //    }

    //FILE WRITE
    pos = 0;

	// for(filepath_len = 0; filepath[filepath_len] != '\0'; i++);
    
    // printk(KERN_INFO "Opened File Successfully\n");
    
    // len = scnprintf(data, 500, "State: %ld\nReal time Priority: %u\nPolicy: %u\nNumber of CPUs allowed: %d\nPID: %d\nNumber of context switch done:%lu\n", task->state, task-> rt_priority, task->policy, task->nr_cpus_allowed, task->pid, task-> nvcsw);
    
    returnVal = kernel_write(fileptr, output, len, &pos);
    
    //Error EIO 5 I/O Error, Could not write full data to file!
    if(returnVal != len){
    	printk(KERN_ERR "Unable to write all data to given file");
    	errno = EIO;
    	return -1 * errno;
    }
    
    //Close File
    filp_close(fileptr, NULL);
    // printk(KERN_INFO "Return Val: %ld", returnVal);
    printk(KERN_EMERG "Hello! Did I just print on my terminal?");
    printk(KERN_INFO "");
    printk(KERN_INFO "hello");
	/* Monotonic time in nsecs: */
	// u64				start_time;
    return 0;
}
