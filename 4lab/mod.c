#include <linux/kernel.h>/* Необходим для pr_info() */
#include <linux/module.h> /* Необходим всем модулям */
#include <linux/proc_fs.h>//для работы с файлами
#include <linux/uaccess.h>//для чтения и записи данных между пользовательским и ядром пространством
#include <linux/version.h>//для определения версии ядра
#include <linux/ktime.h>//для работы со временем

#define procfs_name "tsulab"
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Skoriupin Artem");
MODULE_DESCRIPTION("A simple example a Linux module for time.");
static struct proc_dir_entry *our_proc_file = NULL;

static int read_count = 0;


static ssize_t procfile_read(struct file *file_pointer, 
    char __user *buffer, size_t buffer_length, loff_t* offset) {

    struct timespec64 current_time;
    ktime_get_real_ts64(&current_time);

    // Получаем количество секунд с начала часа

    int seconds_since_midnight = (current_time.tv_sec) % (60*60);
	
    // Вычисляем количество минут
    int minutes_since_midnight = seconds_since_midnight / 60;

    int elapsed_minutes = minutes_since_midnight;
    int remaining_minutes = 60 - elapsed_minutes;
    
    // Четное чтение файла
    // Вычисляем количество минут, прошедших с начала текущего часа
    // и выводим это значение
    // ...
    // Нечетное чтение файла
    // Вычисляем количество минут, оставшихся до следующего часа
    // и выводим это значение
    // ...	
	char message[80];
	ssize_t ret;    
	
    if (read_count % 2 == 0) {
        ret = snprintf(message, sizeof(message), "прошло(a) %d минут(a) с текущего часа\n", elapsed_minutes);

    } else {
        ret = snprintf(message, sizeof(message), "осталось(ась) %d минут(a) до следующего часа\n", remaining_minutes);
    }

	if (*offset >= ret || copy_to_user(buffer, message, ret)) { 
		pr_info("copy_to_user failed\n");
        ret = 0; 
    } else { 
        pr_info("procfile was read on %s\n", file_pointer->f_path.dentry->d_name.name); 
        pr_info("read %d %d %d \n", read_count, elapsed_minutes, remaining_minutes);
    	
        *offset += 2*ret; 
        read_count++;        
	}
	
    return ret; 
};

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
static const struct proc_ops proc_file_fops = {
    .proc_read = procfile_read,
};
#else
static const struct file_operations proc_file_fops = {
    .read = procfile_read,
};
#endif

static int __init procfs1_init(void) {//создаем файл для последующего чтения
    our_proc_file = proc_create(procfs_name, 0644, NULL, &proc_file_fops);
    if (NULL == our_proc_file) { 
        pr_alert("Error:Could not initialize /proc/%s\n", procfs_name); 
        return -ENOMEM; 
    } 
 
    pr_info("proc %s was created\n", procfs_name);

    return 0;
}

static void __exit procfs1_exit(void) {//Удаляем файл из системы procfs
    proc_remove(our_proc_file);
    pr_info("proc %s was removed\n", procfs_name);
}

module_init(procfs1_init);
module_exit(procfs1_exit);
