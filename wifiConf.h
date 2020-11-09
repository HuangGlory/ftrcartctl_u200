#ifndef WIFICONF_H
#define WIFICONF_H

#define WPA_SUPPLICANT_CONF_FILE_NAME tr("/etc/wpa_supplicant/wpa_supplicant.conf")

#define WPA_SUPPLICANT_CONF tr(\
"ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev\n\
update_config=1\n\
country=TW\n\
\n\
network={\n\
    ssid=\"test\"\n\
    psk=\"12345678\"\n\
    key_mgmt=WPA-PSK\n\
}\n\
\n\
network={\n\
    ssid=\"FTR Systems AGV-AP\"\n\
    psk=\"ftr3699510a\"\n\
    key_mgmt=WPA-PSK\n\
    priority=5\n\
}\n\
\n\
network={\n\
    ssid=\"FTR4Outdoor\"\n\
    psk=\"12345678\"\n\
    key_mgmt=WPA-PSK\n\
    priority=10\n\
}\n")

#define WPA_SUPPLICANT_CONF_HEADER tr(\
"ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev\n\
update_config=1\n\
country=TW\n\
\n\
network={\n\
    ssid=\"test\"\n\
    psk=\"12345678\"\n\
    key_mgmt=WPA-PSK\n\
}\n\n")

#endif // WIFICONF_H
