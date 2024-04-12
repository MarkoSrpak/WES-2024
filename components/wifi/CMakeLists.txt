set(COMPONENT_SRCS "wifi.c")
set(COMPONENT_ADD_INCLUDEDIRS ".")
set(COMPONENT_REQUIRES esp_wifi nvs_flash wifi_provisioning)

register_component()
