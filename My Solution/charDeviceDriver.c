/*
 *  chardev.c: Creates a read-only char device that says how many times
 *  you've read from the dev file
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>    /* for put_user */
#include <charDeviceDriver.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");

/*
 * This function is called whenever a process tries to do an ioctl on our
 * device file. We get two extra parameters (additional to the inode and file
 * structures, which all device functions get): the number of the ioctl called
 * and the parameter given to the ioctl function.
 *
 * If the ioctl is write or read/write (meaning output is returned to the
 * calling process), the ioctl call returns the output of this function.
 *
 */


DEFINE_MUTEX  (devLock);

Node *node = NULL;

const size_t messageLength = 6*1024;
const size_t totalLength = 4*1024*1024;
size_t storedLength = 0;
int deviceWrite = 0;

static int counter = 0;


int init_module(void)
{
    Major = register_chrdev(0, DEVICE_NAME, &fops);

    if (Major < 0) {
      printk(KERN_ALERT "Registering char device failed with %d\n", Major);
      return Major;
    }

    printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
    printk(KERN_INFO "the driver, create a dev file with\n");
    printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
    printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
    printk(KERN_INFO "the device file.\n");
    printk(KERN_INFO "Remove the device file and module when done.\n");

    return SUCCESS;
}


void cleanup_module(void)
{
    Node *tempNode = NULL;
    unregister_chrdev(Major, DEVICE_NAME);
    while(node){
	tempNode = node;
	node = node -> previous;
	kfree(tempNode->kBuffer);
	kfree(tempNode);
	tempNode->kBuffer = NULL;
	tempNode = NULL;
    }
    printk(KERN_INFO "The driver has been removed successfully\n");
}


static int device_open(struct inode *inode, struct file *file)
{

    mutex_lock (&devLock);
    if (Device_Open) {
    mutex_unlock (&devLock);
    return -EBUSY;
    }
    Device_Open++;
    mutex_unlock (&devLock);
    printk(KERN_INFO "The file has been opened %d times\n", counter++);
    try_module_get(THIS_MODULE);

    return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
    mutex_lock (&devLock);
    Device_Open--;
    deviceWrite = 0;
    mutex_unlock (&devLock);
    module_put(THIS_MODULE);
    printk(KERN_INFO "File closed\n");

    return 0;
}

static ssize_t device_read(struct file *filp,   /* see include/linux/fs.h   */
               char *buffer,    /* buffer to fill with data */
               size_t length,   /* length of the buffer     */
               loff_t * offset)
{
    int bytes_read = 0;
    Node *currentNode = node;

    mutex_lock(&devLock);
    if(node==NULL){
	mutex_unlock(&devLock);
    	return -EAGAIN;
    }
    node = node->previous;

    bytes_read = currentNode->length;
    storedLength = storedLength - bytes_read;
    if(copy_to_user (buffer, currentNode->kBuffer, bytes_read)!=0){
	kfree(currentNode->kBuffer);
	kfree(currentNode);
	currentNode->kBuffer = NULL;
	currentNode = NULL;
	mutex_unlock(&devLock);
    	return -EFAULT;
    }
    mutex_unlock(&devLock);

    kfree(currentNode->kBuffer);
    kfree(currentNode);
    currentNode->kBuffer = NULL;
    currentNode = NULL;
    printk(KERN_INFO "Read successfully\n");
    return bytes_read;
}



static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
	Node *newNode;
	mutex_lock(&devLock);
	deviceWrite++;
	if(len>messageLength){
		printk(KERN_INFO "The length of the input exceeds the limit\n");
		mutex_unlock(&devLock);
		return -EINVAL;
	}
	if(len + storedLength > totalLength){
		printk(KERN_INFO "The total length of storage exceeds the limit\n");
		mutex_unlock(&devLock);
		return -EAGAIN;
	}

	newNode = kmalloc(sizeof(Node), GFP_KERNEL);
	if(!newNode){
		printk(KERN_INFO "Write failed: memory allocation failed\n");
		mutex_unlock(&devLock);
		return -ENOMEM;
	}

	newNode->kBuffer = kmalloc(len, GFP_KERNEL);
	if(!newNode->kBuffer){
		kfree(newNode);
		newNode = NULL;
		printk(KERN_INFO "Write failed: memory allocation failed\n");
		mutex_unlock(&devLock);
		return -ENOMEM;
	}

	if(copy_from_user(newNode->kBuffer,buff,len)!=0){
		kfree(newNode->kBuffer);
		kfree(newNode);
		newNode->kBuffer = NULL;
		newNode = NULL;
		printk(KERN_INFO "Write failed");
		mutex_unlock(&devLock);
		return -EFAULT;
	}
	newNode->length = len;
	if (deviceWrite == 1) {
		if (node == NULL) {
			newNode->previous = NULL;
			node = newNode;
		} else {
			newNode->previous = node;
			node = newNode;
		}
	}
	else{
		newNode->previous = node->previous;
		node->previous = newNode;
	}
	storedLength += len;
	mutex_unlock(&devLock);
	printk(KERN_INFO "Write successfully\n");
    return len;
}

