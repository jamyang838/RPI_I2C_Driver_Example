#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#define ARDUINO_DEV_NAME KBUILD_MODNAME
static struct class *arduino_class;
static dev_t devno;

struct arduino_i2c_cdev
{
    struct i2c_client *client;
    struct device *dev;
    struct cdev cdev;
};

static int arduino_i2c_open(struct inode *inode, struct file *filp)
{
    struct arduino_i2c_cdev *arduino =
        container_of(inode->i_cdev, struct arduino_i2c_cdev, cdev);
    filp->private_data = arduino->client;
    return 0;
};

static ssize_t arduino_i2c_write(struct file *filp, const char __user *buf, size_t count, loff_t *offset)
{
    char *tmp;
    int err = 0;
    int nbyte = 0;
    int i = 0;
    struct i2c_client *client = filp->private_data;
    count = count > 8192 ? 8192 : count;
    tmp = memdup_user(buf, count);
    if (IS_ERR(tmp))
    {
        return PTR_ERR(tmp);
    }
    for (i = 0; i < count; i++)
    {
        err = i2c_smbus_write_byte(client, tmp[i]);
        nbyte += err ? 0 : 1;
    }
    kfree(tmp);
    return nbyte;
};

struct file_operations arduino_i2c_fops = {
    .open = arduino_i2c_open,
    .write = arduino_i2c_write};

static int arduino_i2c_probe(struct i2c_client *client)
{
    int err = 0;
    struct device *dev = &client->dev;
    struct arduino_i2c_cdev *arduino;

    arduino = devm_kzalloc(dev, sizeof(struct arduino_i2c_cdev), GFP_KERNEL);
    if (IS_ERR(arduino))
    {
        dev_err(dev, "failed to allocate memory");
        err = PTR_ERR(arduino);
        goto out_oom;
    }
    arduino->client = client;
    arduino->cdev.owner = THIS_MODULE;

    cdev_init(&arduino->cdev, &arduino_i2c_fops);
    err = alloc_chrdev_region(&devno, 0, 1, ARDUINO_DEV_NAME);
    if (err < 0)
    {
        dev_err(dev, "failed to get dev_t");
        goto out_alloc_chrdev;
    }

    err = cdev_add(&arduino->cdev, devno, 1);
    if (err)
    {
        dev_err(dev, "failed to register cdev");
        goto out_cdev_add;
    }

    arduino_class = class_create(THIS_MODULE, ARDUINO_DEV_NAME);
    if (IS_ERR(arduino_class))
    {
        dev_err(dev, "failed to create sysfs class");
        err = PTR_ERR(arduino_class);
        goto out_class;
    }

    arduino->dev = device_create(arduino_class, NULL, devno, NULL, ARDUINO_DEV_NAME);
    if (IS_ERR(arduino->dev))
    {
        dev_err(dev, "failed to create device");
        err = PTR_ERR(arduino->dev);
        goto out_device;
    }

    i2c_set_clientdata(client, arduino);
    dev_info(dev, "arduino propbed");
    return 0;

out_device:
    class_destroy(arduino_class);
out_class:
    cdev_del(&arduino->cdev);
out_cdev_add:
    unregister_chrdev_region(devno, 1);
out_alloc_chrdev:
out_oom:
    return err;
};

static int arduino_i2c_remove(struct i2c_client *client)
{
    struct arduino_i2c_cdev *arduino;
    struct device *dev;

    dev = &client->dev;
    arduino = i2c_get_clientdata(client);

    device_destroy(arduino_class, devno);
    class_destroy(arduino_class);
    cdev_del(&arduino->cdev);
    unregister_chrdev_region(devno, 1);

    dev_info(dev, "arduino, removed");
    return 0;
}

static struct of_device_id arduino_ids[] = {
    {
        .compatible = "arduino-i2c",
    },
    {}
    };
MODULE_DEVICE_TABLE(of, arduino_ids);

static struct i2c_driver arduino_i2c_drv = {
    .probe_new = arduino_i2c_probe,
    .remove = arduino_i2c_remove,
    .driver = {
        .name = KBUILD_MODNAME,
        .owner = THIS_MODULE,
        .of_match_table = arduino_ids,
    },
};

module_i2c_driver(arduino_i2c_drv);
MODULE_LICENSE("GPL");