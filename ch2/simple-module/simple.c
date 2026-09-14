#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/jiffies.h>
#include <asm/param.h>

static int simple_init(void);
static void simple_exit(void);

unsigned long jiffies_on_init = 0;

static int simple_init(void)
{
    printk(KERN_INFO "loading kernerl module\n");
    
    printk(KERN_INFO "HZ = %d", HZ);
    printk(KERN_INFO "jiffies = %lu", jiffies);
    jiffies_on_init = jiffies;

    return 0;
}

static void simple_exit(void)
{
    printk(KERN_INFO "jiffies = %lu", jiffies);
    printk(KERN_INFO "module lasted for %lu seconds", (jiffies - jiffies_on_init) / (unsigned long long)HZ);

    printk(KERN_INFO "removing kernel module\n");
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("simple module, says how long it lasted upon removal");
MODULE_AUTHOR("john module");