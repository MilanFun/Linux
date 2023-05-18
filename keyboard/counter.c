#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/timer.h>


MODULE_LICENSE("GPL");
static struct timer_list timer_counter;
static unsigned int count = 0;
static int device = 0;

static irqreturn_t irq_handler(int irq, void *dev_id)
{
    static unsigned int init = 0;
    if (init == 0) {
        count++;
        printk(KERN_INFO "KEYBOARD_HANDLER: pressed & released = %d\n", count);
        init = 1;
    } else {
        init = 0;
    }

    /**
     * Probably will be very good use next logic
     * scancode - unsigned char which equal to inb(0x60) (asm/io lib)
     * *((char *)scancode) & 0x80 == True  => Released 
     * *((char *)scancode) & 0x80 == False => Pressed
    */
    return IRQ_HANDLED;
}

static void timer_call(struct timer_list * data)
{
    printk(KERN_INFO "TIMER_HANDLER: pressed = %d\n", count);
    count = 0;
    // Reset timer handler and counter for keyboard handler
    mod_timer(&timer_counter, jiffies + msecs_to_jiffies(60000));
}


static int __init irq_ex_init(void)
{
    // Set up configuration for using irq line
    int res; 
    free_irq(1, &device);
    enable_irq(1);
    res = request_irq (1, irq_handler, IRQF_SHARED, "keyboard_irq_handler", &device);

    // Set up timer handler
    timer_setup(&timer_counter, timer_call, 0);
    mod_timer(&timer_counter, jiffies + msecs_to_jiffies(60000));

    printk (KERN_INFO "INIT_HANDLER: request_irq return = %d code\n", res);
    return res;
}

static void __exit irq_ex_exit(void)
{
    del_timer(&timer_counter);

    // Free irq with dev_id
    free_irq(1, &device);
    printk (KERN_INFO "EXIT_HANDLER: exit\n");
}

module_init(irq_ex_init);
module_exit(irq_ex_exit);