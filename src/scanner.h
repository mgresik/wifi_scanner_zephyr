#pragma once

#include <cstdint>
#include <zephyr/kernel.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_mgmt.h>

class WifiScanner
{
public:
    WifiScanner();

    int start();
    int getNetworkCount();

private:
    static void wifi_event_handler(struct net_mgmt_event_callback *cb, uint64_t mgmt_event, struct net_if *iface);

    int network_count;
    struct k_mutex count_mutex;
    struct net_mgmt_event_callback wifi_cb;
};
