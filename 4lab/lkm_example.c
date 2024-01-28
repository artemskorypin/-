#include <linux/module.h>/* Необходим всем модулям */
#include <linux/kernel.h>/* Необходим для pr_info() */
#include <linux/proc_fs.h>//для работы с файлами
#include <linux/time.h>//для работы со временем

#define PROC_NAME "tsulab"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Skoriupin Artem");
MODULE_DESCRIPTION("A simple example Linux module for time.");

static int read_count = 0;

static ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos) {

    struct timespec64 current_time;
    ktime_get_real_ts64(&current_time);
    
    // Получаем количество секунд с начала часа
    long long times = current_time.tv_sec;

    long seconds_since_midnight = (times) % (60*60);
	
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
    if (read_count % 2 == 0) {
        pr_info("%d of minutes since the beginning of the current hour\n", elapsed_minutes);
    } else {
        pr_info("%d of minutes remaining until the next hour\n", remaining_minutes);
    }

    read_count++;
    return 0;
}

static const struct proc_ops proc_ops = {
    .proc_read = proc_read,
};

static int proc_init(void) {//создаем файл для последующего чтения
    proc_create(PROC_NAME, 0, NULL, &proc_ops);
    pr_info( "/proc/%s created\n", PROC_NAME);
    return 0;
}

static void proc_exit(void) {//Удаляем файл из системы procfs
    remove_proc_entry(PROC_NAME, NULL);
    pr_info( "/proc/%s removed\n", PROC_NAME);
}

module_init(proc_init);
module_exit(proc_exit);
