set(COMPONENT_SRCS "mqtt_driver.c")
set(COMPONENT_ADD_INCLUDEDIRS ".")
set(COMPONENT_REQUIRES nvs_flash esp_netif driver esp_wifi mqtt gui)

register_component()
