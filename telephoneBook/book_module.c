#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include "user_data.h"
#include <linux/init.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");

int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
static long device_ioctl(struct file *, unsigned int, unsigned long);
void print_array(void);

#define SUCCESS 0
#define DEVICE_NAME "chardev"
#define DEL_USER _IOW('a','a', char*)
#define GET_USER _IOR('g','b', char*)

static int Major;
static int Device_Open = 0;
static int count = 0;
static struct user_data *array;	
char *value;

static struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release,
	.unlocked_ioctl = device_ioctl
};


void print_array()
{
	for (int i = 0; i < count; i++) {
		printk(KERN_INFO "Name: %s\t Surname: %s\t Age: %d\t Email: %s\t Phone: %s\n", 
			array[i].name, array[i].surname, array[i].age, array[i].email, array[i].phone_number);
	}
}

void remove(int index)
{
    for(int i = index; i < count - 1; i++) {
		array[i] = array[i + 1];
   	}
	kfree(&array[count]);
	count--; 
}

static int device_open(struct inode *inode, struct file *file)
{

	if (Device_Open) {
		return -EBUSY;
	}

	Device_Open++;
	try_module_get(THIS_MODULE);
	printk(KERN_INFO "OPEN: open symbolic device");
	return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
	Device_Open--;
	printk(KERN_INFO "CLOSE: close symbolic device\n");
	module_put(THIS_MODULE);
	return 0;
}

static ssize_t device_read(struct file *filp, char *buffer,	size_t length, loff_t * offset)
{
	
	print_array();
	return length;
}

static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{	
	char tmp[len];
	if (buff) {
		printk(KERN_INFO "WRITE: save user.");
		if (copy_from_user(tmp, buff, len) != 0) {
			return -EFAULT;
		} else {
			struct user_data *user = (struct user_data*) ((void*) tmp);
			printk(KERN_INFO "Name: %s\t Surname: %s\t Age: %d\t Email: %s\t Phone: %s\n", 
				user->name, user->surname, user->age, user->email, user->phone_number);
			array[count] = *user;
			count++;
		}
	}
    	
	return len;
}

static long device_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	char *val = kmalloc(16, GFP_KERNEL);
	switch(cmd) {
		case DEL_USER:
			printk(KERN_INFO "DEL_USER: Delete user by surname!\n");
			if(copy_from_user(val ,(char*) arg, sizeof(val)) )
			{
				printk(KERN_ALERT "READ: Panic!\n");
			}
			int del = 0;
			for (int i = 0; i < count; i++) {
				if (strncmp(array[i].surname, val, 16) == 0) {
					printk(KERN_INFO "DEL_USER: Found user with surname %s\n", val);
					remove(i);
					del++;
					break;
				}
			}
			print_array();
			if (del == 0) {
				printk(KERN_INFO "DEL_USER: User with surname %s not found\n", val);
			} 
			break;
		case GET_USER:
			printk(KERN_INFO "GET_USER: Get user by surname!\n");
			if(copy_from_user(val ,(char*) arg, sizeof(val)) )
			{
				printk(KERN_ALERT "GET_USER: Panic!\n");
			}
			int found = 0;
			for (int i = 0; i < count; i++) {
				if (strncmp(array[i].surname, val, 16) == 0) {
					printk(KERN_INFO "GET_USER: Found user with surname %s\n", val);
					if (copy_to_user((char*) arg, &array[i], sizeof(array[i])))
					{
						printk(KERN_ALERT "COPY_TO_USER_SPACE: Panic!\n");
						break;
					}
					printk(KERN_INFO "GET_USER: Name: %s\t Surname: %s\t Age: %d\t Email: %s\t Phone: %s\n", 
						array[i].name, array[i].surname, array[i].age, array[i].email, array[i].phone_number);
					found++;
					break;
				}
			}
			if (found == 0) {
				printk(KERN_INFO "GET_USER: User with surname %s not found\n", val);
			} 
			break;
		default:
			break;
	}
	return 0;
}

static int __init start_module(void)
{
	Major = register_chrdev(0, DEVICE_NAME, &fops);
	array = kmalloc_array(1024, sizeof(user_data), GFP_KERNEL);

	if (Major < 0) {
	  printk(KERN_ALERT "START_MODULE: register char device failed");
	  return Major;
	}

	printk(KERN_INFO "START_MODULE: char device successfully create with major=%d\n", Major);
	return 0;
}

void __exit finish_module(void)
{
	unregister_chrdev(Major, DEVICE_NAME);
	kfree(array);
	kfree(value);
	printk(KERN_INFO "FINISH_MODULE: exit module\n");
}

module_init(start_module);
module_exit(finish_module);