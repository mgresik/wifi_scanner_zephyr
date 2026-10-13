#include "scanner.h"

#include <zephyr/sys/printk.h>

int main()
{
    printk("Starting application...\n");

    WifiScanner scanner;
    const int ret = scanner.start();

    printk("Wi-Fi scanner stopped with error: %d\n", ret);

    return ret;
}