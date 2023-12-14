#include <linux/init.h>/* Необходим для макросов */
#include <linux/module.h>/* Необходим всем модулям */
#include <linux/kernel.h>/* Необходим для pr_info() */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Skoriupin Artem");
MODULE_DESCRIPTION("A simple example Linux module.");


static int __init lkm_example_init(void) {
 printk(KERN_INFO "Welcome to the Tomsk State University!\n");
     /* Если вернётся не 0, значит, init_module провалилась; модули загрузить не получится. */

 return 0;
}
static void __exit lkm_example_exit(void) {
 printk(KERN_INFO "Tomsk State University forever!\n");
}
module_init(lkm_example_init);
module_exit(lkm_example_exit);
