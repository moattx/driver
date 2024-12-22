/*-
 * gpio, a beaglebone gpio driver for Linux.
 * Copyright (C) 2022 moatx
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * or see <https://www.gnu.org/licenses/>.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/slab.h>

/*
 * mknod -m 666 /dev/wgpio c MAJOR MINOR
 */

#define BUF_SIZE 1080

/* global vars */
dev_t dev = 0;
static char *BUF_STRING;
static struct class *dev_class;
static struct cdev wgpio_cdev;

static int wgpio_open(struct inode *inode, struct file *file);
static int wgpio_release(struct inode *inode, struct file *file);
static ssize_t wgpio_read(struct file *filp, char __user *buf, size_t len,
			  loff_t *off);
static ssize_t wgpio_write(struct file *filp, const char *buf, size_t len,
			   loff_t *off);

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = wgpio_read,
	.write = wgpio_write,
	.open = wgpio_open,
	.release = wgpio_release,
};

static int wgpio_open(struct inode *inode, struct file *file)
{
	pr_info("Driver Open Function Called...!!!\n");
	return 0;
}

static int wgpio_release(struct inode *inode, struct file *file)
{
	pr_info("Driver Release Function Called...!!!\n");
	return 0;
}

static ssize_t wgpio_read(struct file *filp, char __user *buf, size_t len,
			  loff_t *off)
{
	pr_info("Driver Read Function Called...!!!\n");
	if (copy_to_user(buf, BUF_STRING, BUF_SIZE) != 0)
		return -EFAULT;
	return BUF_SIZE;
}

static ssize_t wgpio_write(struct file *filp, const char __user *buf,
			   size_t len, loff_t *off)
{
	pr_info("Driver Write Function Called...!!!\n");
	if (copy_from_user(BUF_STRING, buf, len) != 0)
		return -EFAULT;
	return len;
}

static __init int wgpio_init(void)
{
	/* Allocating Major number */
	if ((alloc_chrdev_region(&dev, 0, 1, "wgpio")) < 0) {
		pr_err("Cannot allocate major number for device\n");
		return -1;
	}

	pr_info("Major = %d Minor = %d \n", MAJOR(dev), MINOR(dev));
	/*Creating cdev structure*/
	cdev_init(&wgpio_cdev, &fops);

	/*Adding character device to the system*/
	if ((cdev_add(&wgpio_cdev, dev, 1)) < 0) {
		pr_err("Cannot add the device to the system\n");
		goto out_unregister_class;
	}

	/* Creating struct class */
	dev_class = class_create(THIS_MODULE, "wgpio");
	if (IS_ERR(dev_class)) {
		pr_err("Cannot create the struct class for device\n");
		goto out_unregister_class;
	}

	/* Creating device */
	if (IS_ERR(device_create(dev_class, NULL, dev, NULL, "wgpio"))) {
		pr_err("Cannot create the Device\n");
		goto out_destroy_device;
	}

	BUF_STRING = kmalloc(sizeof(BUF_STRING) * BUF_SIZE, GFP_KERNEL);
	pr_info("Kernel driver Inserted Successfully...\n");

	return 0;

out_destroy_device:
	class_destroy(dev_class);
out_unregister_class:
	unregister_chrdev_region(dev, 1);
	return -1;
}

static __exit void wgpio_exit(void)
{
	kfree(BUF_STRING);
	device_destroy(dev_class, dev);
	class_destroy(dev_class);
	cdev_del(&wgpio_cdev);
	unregister_chrdev_region(dev, 1);
	pr_info("Kernel driver Removed Successfully...\n");
}

module_init(wgpio_init);
module_exit(wgpio_exit);

/* meta information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("moatx");
MODULE_DESCRIPTION("A driver that controls beaglebone's gpio from user space");
