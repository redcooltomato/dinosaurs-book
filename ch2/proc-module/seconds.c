#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/jiffies.h>
#include <asm/param.h>

#define BUFFER_SIZE 128

#define PROC_NAME "seconds"

static ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);

static int proc_init(void);
static void proc_exit(void);

unsigned long jiffies_on_init;

static const struct proc_ops proc_ops = {
    .proc_read = proc_read,
};

static int proc_init(void) {
    proc_create(PROC_NAME, 0, NULL, &proc_ops);

    printk(KERN_INFO "proc %s created", PROC_NAME);

    jiffies_on_init = jiffies;

    return 0;
}

static void proc_exit(void) {
    remove_proc_entry(PROC_NAME, NULL);

    printk( KERN_INFO "proc %s removed\n", PROC_NAME);
}

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos) {
    int return_value = 0;
    char buffer[BUFFER_SIZE];

    static int safeguard = 0; // in case of several parallel runs? idk
    if (safeguard) {
        safeguard = 0;
        return 0;
    }
    safeguard = 1;

    return_value = sprintf(buffer, "seconds passed since proc initialization: %lu\n", 
        (jiffies - jiffies_on_init) / (unsigned long)HZ);
    
    if (copy_to_user(usr_buf, buffer, return_value)) {
        return -EFAULT;
    }

    return return_value;
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("simple module");
MODULE_AUTHOR("john module maker");