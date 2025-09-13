#include <linux/module.h>
#include <linux/poll.h>

#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/gfp.h>
#include <linux/gpio.h>
#include <linux/gpio/consumer.h>
#include <linux/platform_device.h>
#include <linux/of_gpio.h>
#include <linux/of_irq.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/slab.h>
#include <linux/fcntl.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <asm/current.h>
#include <linux/delay.h>

static int major;
static struct class *dht11_class;
static int dht11_data_pin = 115;  /* gpio4_19, (4-1)*32+19-115 */
static wait_queue_head_t dht11_wq;

static void dht11_start(void)
{
	gpio_direction_output(dht11_data_pin, GPIOD_OUT_LOW);
	mdelay(18);
	gpio_direction_output(dht11_data_pin, GPIOD_OUT_HIGH);
	udelay(40);	
	gpio_direction_input(dht11_data_pin);
}

static int dht11_wait_for_ready(void)
{
	int timeout_us = 200;

	/* 等待低电平 */
	while (gpio_get_value(dht11_data_pin) && --timeout_us)
	{
		udelay(1);
	}
	if (!timeout_us)
	{
		return -1;
	}

	/* 现在是低电平 */
	/* 等待高电平 */
	timeout_us = 200;
	while (!gpio_get_value(dht11_data_pin) && --timeout_us)
	{
		udelay(1);
	}
	if (!timeout_us)
	{
		return -1;
	}

	/* 现在是高电平 */
	/* 等待低电平 */
	timeout_us = 200;
	while (gpio_get_value(dht11_data_pin) && --timeout_us)
	{
		udelay(1);
	}
	if (!timeout_us)
	{
		return -1;
	}
	
	return 0;	
}


static int dht11_read_byte(unsigned char *buf)
{
	int i;
	int us = 0;
	unsigned char data = 0;
	int timeout_us = 200;
	
	for (i = 0; i <8; i++)
	{
		/* 现在是低电平 */
		/* 等待高电平 */
		timeout_us = 200;
		while (!gpio_get_value(dht11_data_pin) && --timeout_us)
		{
			udelay(1);
		}
		if (!timeout_us)
		{
			return -1;
		}

		/* 现在是高电平 */
		/* 等待低电平,累加高电平的时间 */
		timeout_us = 200;
		us = 0;
		while (gpio_get_value(dht11_data_pin) && --timeout_us)
		{
			udelay(1);
			us++;
		}
		if (!timeout_us)
		{
			return -1;
		}
		if (us > 40)
		{
			/* get bit 1 */
			data = (data << 1) | 1;
		}
		else
		{
			/* get bit 0 */
			data = (data << 1) | 0;
		}
		
	}

	*buf = data;
	return 0;
}

/* 实现对应的open/read/write等函数，填入file_operations结构体                   */
static ssize_t dht11_drv_read (struct file *file, char __user *buf, size_t size, loff_t *offset)
{
	unsigned long flags;
	int i;
	unsigned char data[5];
	int ret;

	if (size != 4)
		return -EINVAL;
	
	local_irq_save(flags);	  // 关中断

	/* 1. 发送高脉冲启动DHT11 */
	dht11_start();

	/* 2. 等待DHT11就绪 */
	if (dht11_wait_for_ready())
	{
		local_irq_restore(flags); // 恢复中断
		return -EAGAIN;
	}

	/* 3. 读5字节数据 */
	for (i = 0; i < 5; i++)
	{
		if (dht11_read_byte(&data[i]))
		{
			local_irq_restore(flags); // 恢复中断
			return -EAGAIN;
		}
	}
			
	local_irq_restore(flags); // 恢复中断

	/* 4. 根据校验码验证数据 */
	if (data[4] != (data[0] + data[1] + data[2] + data[3]))
		return -1;

	/* 5. copy_to_user */	
	/* data[0]/data[1] : 湿度 */
	/* data[2]/data[3] : 温度 */
	ret = copy_to_user(buf, data, 4);
	(void)ret;
	return 4;
}

static unsigned int dht11_drv_poll(struct file *fp, poll_table * wait)
{
//	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
//	poll_wait(fp, &dht11_wait, wait);
	return 0;
}



/* 定义自己的file_operations结构体                                              */
static struct file_operations dht11_fops = {
	.owner	 = THIS_MODULE,
	.read    = dht11_drv_read,
	.poll    = dht11_drv_poll,
};


static const struct of_device_id ask100_dht11[] = {
    { .compatible = "100ask,dht11" },
    { },
};


/* 2. 在入口函数注册platform_driver */
static int __init dht11_init(void)
{
    int err = 0;
    
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

	/* 注册file_operations 	*/
	major = register_chrdev(0, "dht11", &dht11_fops);  

	dht11_class = class_create(THIS_MODULE, "dht11_class");
	if (IS_ERR(dht11_class)) {
		printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
		unregister_chrdev(major, "dht11");
		return PTR_ERR(dht11_class);
	}

	init_waitqueue_head(&dht11_wq);

	device_create(dht11_class, NULL, MKDEV(major, 0), NULL, "dht11");

		
	return err;
}

/* 3. 有入口函数就应该有出口函数：卸载驱动程序时，就会去调用这个出口函数
 *     卸载platform_driver
 */
static void __exit dht11_exit(void)
{
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

	class_destroy(dht11_class);
	unregister_chrdev(major, "dht11");
}


/* 7. 其他完善：提供设备信息，自动创建设备节点                                     */

module_init(dht11_init);
module_exit(dht11_exit);

MODULE_LICENSE("GPL");


