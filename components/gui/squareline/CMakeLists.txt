SET(SOURCES components/ui_comp_background1.c
    components/ui_comp.c
    screens/ui_Screen1.c
    screens/ui_Screen2.c
    screens/ui_Screen3.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    ui_events.c
    images/ui_img_bg_space1_png.c
    images/ui_img_logo3_png.c
    images/ui_img_dashboard3_png.c
    images/ui_img_x_icon2_png.c
    images/ui_img_o_icon3_png.c
    images/ui_img_logo2_png.c
    images/ui_img_logo4_png.c)

add_library(ui ${SOURCES})
