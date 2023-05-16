#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xfae95743, "module_layout" },
	{ 0xeb02705f, "i2c_del_driver" },
	{ 0xf01958ca, "i2c_register_driver" },
	{ 0x55ef4e8b, "device_destroy" },
	{ 0x45999f19, "_dev_info" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x9f3810fb, "cdev_del" },
	{ 0xb82e53c3, "class_destroy" },
	{ 0x4455988c, "_dev_err" },
	{ 0x67c481f5, "device_create" },
	{ 0xf66f1eec, "__class_create" },
	{ 0x33b77350, "cdev_add" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xb7ce238f, "cdev_init" },
	{ 0x5a97d022, "devm_kmalloc" },
	{ 0x37a0cba, "kfree" },
	{ 0x3c56d48f, "i2c_smbus_write_byte" },
	{ 0xe2e8065e, "memdup_user" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Carduino-i2c");
MODULE_ALIAS("of:N*T*Carduino-i2cC*");

MODULE_INFO(srcversion, "017F05AA489FE51BB81EE16");
