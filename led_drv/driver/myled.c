#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/platform_device.h>
#include "myled.h"

unsigned long *base_addr;	/* Vitual Base Address */
struct resource *res;		/* Device Resource Structure */
unsigned long remap_size;	/* Device Memory Size */

static ssize_t proc_myled_write(struct file *file, const char __user * buf,
								size_t count, loff_t * ppos)
{
	char myled_phrase[16];
	u32 myled_value;

	if (count < 11)
	{
		if (copy_from_user(myled_phrase, buf, count))
			return -EFAULT;
		myled_phrase[count] = '\0';
		myled_value = simple_strtoul(myled_phrase, NULL, 0);
	}
	else
		myled_value = 0;

	wmb();
	iowrite32(myled_value, base_addr);
	return count;
}

// write to seq file
static int proc_myled_show(struct seq_file *p, void *v)
{
	u32 myled_value;
	myled_value = ioread32(base_addr);
	seq_printf(p, "0x%x", myled_value);
	return 0;
}
static int proc_myled_open(struct inode *inode, struct file *file)
{
	unsigned int size = 16;
	char *buf;
	struct seq_file *m;
	int res;

	buf = (char *)kmalloc(size * sizeof(char), GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	res = single_open(file, proc_myled_show, NULL);

	if (!res)
	{
		m = file->private_data;
		m->buf = buf;
		m->size = size;
	}
	else
	{
		kfree(buf);
	}

	return res;
}

/* File Operations for /proc/myled */
static const struct file_operations proc_myled_operations =
{
	.open = proc_myled_open,
	.write = proc_myled_write,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release
};

static void myled_shutdown(struct platform_device *pdev)
{
	iowrite32(0, base_addr);
}

static int myled_remove(struct platform_device *pdev)
{
	myled_shutdown(pdev);
	/* Remove /proc/myled entry */
	remove_proc_entry(DRIVER_NAME, NULL);
	/* Release mapped virtual address */
	iounmap(base_addr);
	/* Release the region */
	release_mem_region(res->start, remap_size);
	return 0;
}

/*
 * Device Probe function for myled
 */
static int __devinit myled_probe(struct platform_device *pdev)
{
	struct proc_dir_entry *myled_proc_entry;
	int ret = 0;
	printk(KERN_INFO DRIVER_NAME " start probe\n");
	// get resource
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res)
	{
		dev_err(&pdev->dev, "No memory resource\n");
		return -ENODEV;
	}
	printk(KERN_INFO DRIVER_NAME " physical addr 0x%x\n", res->start);
	// request memory resource
	remap_size = res->end - res->start + 1;
	if (!request_mem_region(res->start, remap_size, pdev->name))
	{
		dev_err(&pdev->dev, "Cannot request IO\n");
		return -ENXIO;
	}
	// remap to virtual address
	base_addr = ioremap(res->start, remap_size);
	if (base_addr == NULL)
	{
		dev_err(&pdev->dev, "Couldn't ioremap memory at 0x%08lx\n",
			(unsigned long)res->start);
		ret = -ENOMEM;
		goto err_release_region;
	}
	// create procfs entry
	myled_proc_entry = proc_create(DRIVER_NAME, 0, NULL, &proc_myled_operations);
	if (myled_proc_entry == NULL)
	{
		dev_err(&pdev->dev, "Couldn't create proc entry\n");
		ret = -ENOMEM;
		goto err_create_proc_entry;
	}

	printk(KERN_INFO DRIVER_NAME " probed at va 0x%p\n", base_addr);

	return 0;

 err_create_proc_entry:
	iounmap(base_addr);
 err_release_region:
	release_mem_region(res->start, remap_size);

	return ret;
}

/* device match table to match with device node in device tree */
static const struct of_device_id myled_of_match[] __devinitconst =
{
	{.compatible = DEVICE_NAME},
	{},
};
MODULE_DEVICE_TABLE(of, myled_of_match);

/* platform driver structure for myled driver */
static struct platform_driver myled_driver =
{
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table = myled_of_match
	},
	.probe = myled_probe,
	.remove = __devexit_p(myled_remove),
	.shutdown = __devexit_p(myled_shutdown)
};

/* Register myled platform driver */
module_platform_driver(myled_driver);

MODULE_LICENSE("GPL");
MODULE_ALIAS(DRIVER_NAME);

