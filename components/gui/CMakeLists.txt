idf_component_register(
    SRCS
        "gui.c"
        "app.c"

        "src/coms.c"
        "src/btnqueue.c"
        "src/tictac.c"

        "squareline/components/ui_comp_hook.c"
        "squareline/components/ui_comp_background1.c"
        "squareline/components/ui_comp.c"

        "squareline/screens/ui_Screen1.c"
        "squareline/screens/ui_Screen2.c"
        "squareline/screens/ui_Screen3.c"
        "squareline/screens/ui_Screen4.c"

        "squareline/images/ui_img_bg_space1_png.c"
        "squareline/images/ui_img_dashboard3_png.c"
        "squareline/images/ui_img_logo2_png.c"
        "squareline/images/ui_img_logo3_png.c"
        "squareline/images/ui_img_logo4_png.c"
        "squareline/images/ui_img_o_icon3_png.c"
        "squareline/images/ui_img_x_icon2_png.c"

        "squareline/ui.c"
        "squareline/ui_helpers.c"
        "squareline/ui_events.c"

    INCLUDE_DIRS
        ""
        "squareline"
        "squareline/components"
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
        accelerometer
        i2c
        joy
)
