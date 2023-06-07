// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * adruino_i2c.c - handle Arduino MCU as I2C slave mode
 *
 * Copyright (C) 2023 Simon Fan, Pegatron
 */

#include <linux/module.h>
#include <linux/kernel.h>
// #include <linux/init.h>
#include <linux/i2c.h>
#include <linux/string.h>
#include <linux/fs.h>
// #include <linux/slab.h>
#include <linux/cdev.h>
// #include <linux/device.h>

#define ARDUINO_DEV_NAME "arduino"
static struct class *arduino_class = NULL;
static dev_t devno = 0;

struct arduino_i2c_cdev {
    struct i2c_client *client;
    struct device *dev;
    struct cdev cdev;
};

static ssize_t arduino_i2c_open(struct inode *inode, struct file *filp)
{
    struct arduino_i2c_cdev *arduino = container_of(inode->i_cdev, struct arduino_i2c_cdev, cdev);
    if (!arduino) {
    	pr_err("Cannot extrace aduino structure from i_cdev.\n");
	return -EINVAL;
    }
    filp -> private_data = arduino -> client;
    return 0;
}

static ssize_t arduino_i2c_write(struct file *filp, const char __user *buf, size_t count,
		loff_t *offset)
{
    struct i2c_client *client = filp -> private_data;
    int i = 0;
    int err =0;
    int nbyte = 0;
    char *tmp;
    
    if (!client) {
        pr_err("Failed to get struct i2c_client.\n");
        return -EINVAL;
    }

    count = count > 8192 ? 8192 : count;
    tmp = memdup_user(buf, count);

    if (IS_ERR(tmp))
    {
	return PTR_ERR(tmp);
    }		    
    
    for (i = 0; i < count; i++) {
        err = i2c_smbus_write_byte(client, buf[i]);
	nbyte += err ? 0 : 1;
    }

    kfree(tmp);
    return nbyte;
}

static ssize_t arduino_i2c_read(struct file *filp, char __user *buf, size_t count,
		loff_t *offset)
{
    return 0;
}	

struct file_operations arduino_i2c_fops = {
    .open = arduino_i2c_open,
    .write = arduino_i2c_write,
    .read = arduino_i2c_read,
};

static int arduino_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    //char *info = "Hello, Arduino!";
    //for (int i = 0; i < strlen(info); i++) {
    //	i2c_smbus_write_byte (client, info[i]);
    //}

    int err = 0;
    struct arduino_i2c_cdev *arduino;
    struct device *dev = &client->dev;
    pr_info("Arduino i2c is being probed.\n");

    err = alloc_chrdev_region(&devno, 0, 1, ARDUINO_DEV_NAME);
    if (err < 0) {
        pr_err ("Failed in alloc_chrdev_reion for arduino.\n");
	goto out_alloc_chrdev;
    }

    arduino_class = class_create(THIS_MODULE, ARDUINO_DEV_NAME);
    if (!arduino_class) {
    	pr_err ("Failed to create sysfs class.\n");
	goto out_sysfs_class;
    }

    arduino = devm_kzalloc(dev, sizeof(struct arduino_i2c_cdev), GFP_KERNEL);
    if (!arduino) {
	pr_err("Failed to allocate memory.\n");
    	goto out_oom;
    }
    arduino -> client = client;

    cdev_init(&(arduino -> cdev), &arduino_i2c_fops);
    arduino->cdev.owner = THIS_MODULE;
    err = cdev_add(&(arduino -> cdev), devno, 1);
    if (err) {
	pr_err("Failed to register cdev.\n");
    	goto out_cdev_add;
    }

    arduino->dev = device_create(arduino_class, NULL, devno, NULL, ARDUINO_DEV_NAME);
    if (IS_ERR(arduino->dev)) {
    	pr_err("Failed to create device entry under sysfs.\n");
	goto out_device;
    }
    i2c_set_clientdata(client, arduino);
    return 0;

out_device:
    cdev_del(&arduino->cdev);
out_cdev_add:
    kfree(arduino);
out_oom:
    class_destroy(arduino_class);
out_sysfs_class:
    unregister_chrdev_region(devno, 1);
out_alloc_chrdev:
    return err;    
}

static int arduino_i2c_remove(struct i2c_client *client)
{
    struct arduino_i2c_cdev *arduino;
    struct device *dev;
    pr_info("Arduino i2c is removing.\n");

    dev = &client->dev;
    arduino = i2c_get_clientdata(client);
    device_destroy(arduino_class, devno);
    cdev_del(&(arduino->cdev));
    kfree(arduino);
    class_destroy(arduino_class);
    unregister_chrdev_region(devno, 1);
    return 0;
}

static const struct of_device_id arduino_of_ids[] = {
        { .compatible = "arduino-i2c", },
        { }
};
MODULE_DEVICE_TABLE(of, arduino_of_ids);

static const struct i2c_device_id arduino_id[] = {
        { "arduino", 0 },
        { },
};
MODULE_DEVICE_TABLE(i2c, arduino_id);


static struct i2c_driver arduino_i2c_driver = {
        .driver = {
                .owner = THIS_MODULE,
                .name = "arduino",
                .of_match_table = arduino_of_ids,
        },
        .probe = arduino_i2c_probe,
        .remove = arduino_i2c_remove,
        .id_table = arduino_id,
};

module_i2c_driver(arduino_i2c_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SimonFan");
MODULE_DESCRIPTION("This driver for Arduino MCU as I2C slave mode!");

