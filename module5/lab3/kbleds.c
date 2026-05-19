// clang-format off
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/string.h>
#include <linux/timer.h>
#include <linux/tty.h>
#include <linux/tty_port.h> 
#include <linux/kd.h>
#include <linux/vt.h>
#include <linux/console_struct.h>
#include <linux/vt_kern.h>
// clang-format on

MODULE_DESCRIPTION("module5 lab3. kernelblink + sysfs");
MODULE_LICENSE("GPL");

struct timer_list my_timer;
struct tty_driver *my_driver;

static struct kobject *example_kobject;

static int _kbledstatus = 0;

static int test = 7;

#define BLINK_DELAY HZ / 5
#define ALL_LEDS_ON 0x07
#define RESTORE_LEDS 0xFF
#define SYS_FILE "systest"

static ssize_t show(struct kobject *kobj, struct kobj_attribute *attr,
					char *buff) {
	return sprintf(buff, "%d\n", test);
}

static ssize_t store(struct kobject *kobj, struct kobj_attribute *attr,
					 const char *buff, size_t count) {
	sscanf(buff, "%du", &test);
	return count;
}

static struct kobj_attribute attribute = __ATTR(test, 0660, show, store);

static void my_timer_func(struct timer_list *ptr) {
	int *pstatus = &_kbledstatus;
	if (*pstatus == test)
		*pstatus = RESTORE_LEDS;
	else
		*pstatus = test;
	(my_driver->ops->ioctl)(vc_cons[fg_console].d->port.tty, KDSETLED,
							*pstatus);
	my_timer.expires = jiffies + BLINK_DELAY;
	add_timer(&my_timer);
}

static int __init kbleds_init(void) {
	pr_debug("Module initialized successfully \n");
	int error = 0;
	example_kobject = kobject_create_and_add(SYS_FILE, kernel_kobj);
	if (!example_kobject) return -ENOMEM;

	error = sysfs_create_file(example_kobject, &attribute.attr);
	if (error) {
		pr_debug("failed to create the foo file in /sys/kernel/%s \n",
				 SYS_FILE);
	}

	my_driver = vc_cons[fg_console].d->port.tty->driver;

	timer_setup(&my_timer, my_timer_func, 0);

	my_timer.expires = jiffies + BLINK_DELAY;
	add_timer(&my_timer);
	return 0;
}

static void __exit kbleds_cleanup(void) {
	pr_debug("Module un initialized successfully \n");

	kobject_put(example_kobject);
	del_timer(&my_timer);
	(my_driver->ops->ioctl)(vc_cons[fg_console].d->port.tty, KDSETLED,
							RESTORE_LEDS);
}

module_init(kbleds_init);
module_exit(kbleds_cleanup);
