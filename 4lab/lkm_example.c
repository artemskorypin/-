#include <linux/module.h>//нужен для всех модулей
#include <linux/fs.h>//для работы с файлами
#include <linux/time.h>//для работы с временем
#include <linux/delay.h>//для задержек

#define FILENAME "example.txt"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Artem Skoriupin");
MODULE_DESCRIPTION("A simple example Linux module for time.");

static int file_read_count = 0;

static int __init my_init(void) {
    struct file *file;

    while(file_read_count < 3)
    {
    	file = filp_open(FILENAME, O_RDONLY, 0);//открыли файл
    	if (IS_ERR(file)) {//проверили, что открылся
        	printk("Error opening file\n");
        	return PTR_ERR(file);
    	}

    	long day = (24 * 60 * 60);   
    	struct timespec64 current_time;
    	ktime_get_real_ts64(&current_time);//текущее время с начала эпохи
    
    	long long times = current_time.tv_sec;
	    
	// Получаем количество секунд с начала дня
    	long seconds_since_midnight = (times) % day;
	
    	// Вычисляем количество минут
    	int minutes_since_midnight = seconds_since_midnight / 60;

    	int elapsed_minutes = minutes_since_midnight + 11*60;
    	int remaining_minutes = 48*60 - elapsed_minutes;
		
    	// Четное чтение файла
        // Вычисляем количество минут, прошедших с 13:00 прошлого дня
        // и выводим это значение
        // ...

        // Нечетное чтение файла
        // Вычисляем количество минут, оставшихся до 13:00 следующего дня
        // и выводим это значение
        // ...
    	if (file_read_count % 2 == 0) {
    	    pr_info("%d minutes since 13:00 yesterday\n", elapsed_minutes);
    	} else {
    	    pr_info("%d minutes until 13:00 tomorrow\n", remaining_minutes);
    	}
    
    	file_read_count++;

    	filp_close(file, NULL);
    	
    	msleep(20000); // Приостановка выполнения на 2000 миллисекунд (2 секунды)
    }
    
    return 0;
}

static void __exit my_exit(void) {
    printk("Module unloaded\n");
}

module_init(my_init);
module_exit(my_exit);
