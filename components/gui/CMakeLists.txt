idf_component_register(
    SRCS
        "gui.c"
        "app.c"

        "squareline/components/ui_comp_hook.c"

        "squareline/screens/ui_Screen1.c"

        "squareline/ui.c"
        "squareline/ui_helpers.c"

    INCLUDE_DIRS
        ""
        "squareline"

    REQUIRES
        lvgl
        lvgl_esp32_drivers
        esp_timer
)
