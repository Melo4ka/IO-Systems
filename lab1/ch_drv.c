#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/slab.h>

#define BUF_SIZE 256

static dev_t first;
static struct cdev c_dev;
static struct class * cl;

char ibuf[BUF_SIZE];

typedef struct ModuleData {
    int spaces;
    struct ModuleData *next;
} ModuleData;

static ModuleData *firstData, *lastData;

static int my_open(struct inode *i, struct file *f)
{
    printk(KERN_INFO "Driver: open()\n");
    return 0;
}

static int my_close(struct inode *i, struct file *f)
{
    printk(KERN_INFO "Driver: close()\n");
    return 0;
}

static int space_count(const char *str) {
    int size = strlen(str);
    int spaces = 0;
    int i = 0;
    while (i != size) {
        if (str[i] == ' ') {
            ++spaces;
        }
        ++i;
    }
    return spaces;
}

static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
ModuleData *now;
int count;
int ptr = 0;//strlen(ibuf) - 1;

now = firstData;
while (now) {
ptr += sprintf(ibuf + ptr, "%d ", now->spaces);
now = now->next;
}

ibuf[ptr++] = '\n';
ibuf[ptr++] = '\0';

count = strlen(ibuf);

printk(KERN_INFO "Driver: read()\n");

if (*off > 0 || len < count) {
return 0;
}

if (copy_to_user(buf, ibuf, count) != 0) {
return -EFAULT;
}

*off = count;

return count;
}

static ssize_t my_write(struct file *f, const char __user *buf,  size_t len, loff_t *off)
{
ModuleData *data;

printk(KERN_INFO "Driver: write()\n");

if(len > BUF_SIZE)
return 0;

if (copy_from_user(ibuf, buf, len) != 0) {
return -EFAULT;
}

data = (ModuleData *) kmalloc(sizeof(ModuleData), GFP_KERNEL);

if (!data) {
printk(KERN_ERR "Can not allocate memory for driver data.\n");
return 0;
}

data->spaces = space_count(ibuf);

if (!lastData) {
firstData = data;
lastData = data;
} else {
lastData->next = data;
lastData = data;
}

return len;
}

static struct file_operations mychdev_fops =
        {
                .owner = THIS_MODULE,
                .open = my_open,
                .release = my_close,
                .read = my_read,
                .write = my_write
        };

static int __init ch_drv_init(void)
{
    printk(KERN_INFO "Hello!\n");
    if (alloc_chrdev_region(&first, 0, 1, "ch_dev") < 0)
    {
        return -1;
    }
    if ((cl = class_create(THIS_MODULE, "chardrv")) == NULL)
    {
        unregister_chrdev_region(first, 1);
        return -1;
    }

    if (device_create(cl, NULL, first, NULL, "var4") == NULL)
    {
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return -1;
    }


    cdev_init(&c_dev, &mychdev_fops);
    if (cdev_add(&c_dev, first, 1) == -1)
    {
        device_destroy(cl, first);
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return -1;
    }
    return 0;
}

static void __exit ch_drv_exit(void)
{
    cdev_del(&c_dev);
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    printk(KERN_INFO "Bye!!!\n");
}

module_init(ch_drv_init);
module_exit(ch_drv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aleksandr Dolgikh and Andrienko Sergei");
MODULE_DESCRIPTION("IO Systems Lab #1");