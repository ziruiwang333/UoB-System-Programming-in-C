# University of Birmingham System Programming in C/C++

## Assignment 3

Assignment 3 (Released on November 20th at 10:00 am)

 

* **7 points in total, worth 7% of the final grade.**
* **Due December 6th at noon (12:00pm) — HARD DEADLINE**
* **You must submit electronically using Canvas.**

Write a device driver for a character device which implements a simple way of message passing. The kernel maintains a list of messages. To limit memory usage, we impose a limit of 6KB = 6*1024 bytes for each message, and also impose a limit of the size of all messages, which is initially 4MB = 4*1024*1024 bytes.

Your device driver should perform the following operations:

* Creating the device, which has to be /dev/opsysmem, creates an empty list of messages.
* Removing the device deallocates all messages and removes the list of messages.
* Reading from the device returns one message, and removes this message from the kernel list. If the list of messages is empty, the reader returns -EAGAIN.
* Writing to the device stores the message in kernel space and adds it to the list if the message is below the maximum size, and the limit of the size of all messages wouldn't be surpassed with this message. If the message is too big, -EINVAL is returned, and if the limit of the size of all messages was surpassed, -EAGAIN is returned.
* The kernel module which implements this driver must be called charDeviceDriver.ko.

You need to ensure that your code deals with multiple attempts at reading and writing at the same time. The reader should obtain the messages a LIFO (last in first out) manner.

**Marking Criteria**

2 points for correctly handling the list of messages

2 points for correctly handling opening, closing, reading, writing to the devise.

2 points for correctly handling the concurrency

1 Point for correctly handling the memory leak

**Testing**

Here you can find a project template to use. Do not modify or add files in the repository that start with test, since we use those files for scripts for testing. The test script expects the kernel module at module loading time to add the major number to /var/log/syslog, as done in the example code. The scripts used for marking will expect this output as well. A simple test script is provided in the project template.

**Submission**

You should put all your files into a .zip directory named exercise3. The directory exercise3 must be at the top level of your project for the marking scripts to work. We will run the command make in the directory exercise3 in order to obtain all required binaries. Do not use any files or directory with the prefix "test", since we will use such files for running tests. It is worth trying out submission well before the deadline even if the solution is not correct yet.

For marking we will use additional, more advanced, test scripts which check whether your program satisfies the specification. If the provided test scripts fail, all the more advanced test scripts are likely to fail as well. Any code which does not compile on the virtual machine provided will be awarded 0 marks and not be reviewed.

**Explanation of the example code**

The code provided is an example, in init_module, it will register a char device. so it will appears in /proc/devices as "chardev".

There is a mutex lock named as "devLock" just showing how to use mutex lock in kernel programming.

In device_read function, it shows how to copy message to user space.

You'll need to modify this charDeviceDriver.c and charDeviceDriver.h to complete the assignment.

**Useful commands for this assignment:**
1. "cat /proc/devices" check the regisitered device name
2. "sudo insmod charDeviceDriver.ko" insert the module to kernel
3. "sudo rmmod charDeviceDriver.ko" remove the kernel module
4. "tail -f /var/log/syslog" check the kernel log printed by "printk()"
5. "mknod /dev/chardev c <major> <minor>" make a device to talk to (check syslog for this, do this before you read or write to device )
6. "echo <message> > /dev/<devicename>" send message to the device (use "sudo su" change to root user before execute this)
7. "head -n 1 < /dev/<devicename>" read one message from the device(use "sudo su" change to root user before execute this)