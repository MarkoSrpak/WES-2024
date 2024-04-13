idf_component_register(
    SRCS
        "gui.c"
        "app.c"

        "src/coms.c"

        "squareline/components/ui_comp_hook.c"

        "squareline/screens/ui_Screen1.c"

        "squareline/ui.c"
        "squareline/ui_helpers.c"

    INCLUDE_DIRS
        ""
        "squareline"
        "inc"

    REQUIRES
        lvgl
        lvgl_esp32_drivers
        json
        esp_timer
        wifi
        led_pwm
        button
        mqtt_driver
        i2c
)
