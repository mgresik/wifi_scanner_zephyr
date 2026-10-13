#include "scanner.h"

#include <zephyr/kernel.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/sys/printk.h>

#include <errno.h>

K_SEM_DEFINE(scan_done, 0, 1);

static void wifi_event_handler(struct net_mgmt_event_callback *cb, uint64_t event, struct net_if *iface)
{
    ARG_UNUSED(iface);

    switch (event)
    {
    case NET_EVENT_WIFI_SCAN_RESULT:
    {
        const auto *result = static_cast<const wifi_scan_result *>(cb->info);
        if (result == nullptr)
        {
            break;
        }

        printk("SSID: %.*s | RSSI: %d dBm | Channel: %u | Security: %d\n",
               result->ssid_length,
               reinterpret_cast<const char *>(result->ssid),
               result->rssi,
               result->channel,
               static_cast<int>(result->security));

        break;
    }

    case NET_EVENT_WIFI_SCAN_DONE:
        printk("Wi-Fi scan completed\n");
        k_sem_give(&scan_done);
        break;

    default:
        break;
    }
}

int WifiScanner::start()
{
    struct net_if *iface = net_if_get_default();

    if (iface == nullptr)
    {
        printk("ERROR: No network interface found\n");
        return -ENODEV;
    }

    static struct net_mgmt_event_callback wifi_cb;

    net_mgmt_init_event_callback(&wifi_cb, wifi_event_handler, NET_EVENT_WIFI_SCAN_RESULT | NET_EVENT_WIFI_SCAN_DONE);
    net_mgmt_add_event_callback(&wifi_cb);

    int ret = net_if_up(iface);
    if (ret != 0 && ret != -EALREADY)
    {
        printk("ERROR: net_if_up failed: %d\n", ret);
        return ret;
    }

    printk("ESP32-S3 Wi-Fi scanner started\n");

    while (true)
    {
        printk("\nScanning Wi-Fi networks...\n");

        ret = net_mgmt(NET_REQUEST_WIFI_SCAN, iface, nullptr, 0);
        if (ret != 0)
        {
            printk("ERROR: Wi-Fi scan request failed: %d\n", ret);
        }
        else
        {
            ret = k_sem_take(&scan_done, K_SECONDS(30));

            if (ret != 0)
            {
                printk("WARNING: Wi-Fi scan timed out\n");
            }
        }

        k_sleep(K_SECONDS(10));
    }

    return 0;
}
