/**
 * @file gui_app.c
 *
 * @brief This file runs the UI.
 *
 * COPYRIGHT NOTICE: (c) 2023 Byte Lab Grupa d.o.o.
 * All rights reserved.
 */

//--------------------------------- INCLUDES ----------------------------------
#include "gui_app.h"
#include <stdio.h>

/* Littlevgl specific */
#include "lvgl.h"
#include "lvgl_helpers.h"
//---------------------------------- MACROS -----------------------------------

//-------------------------------- DATA TYPES ---------------------------------

//---------------------- PRIVATE FUNCTION PROTOTYPES --------------------------
/**
 * @brief This function handles events that happen on LVGL objects.
 *
 * @param [in] p_event Pointer to the event type.
 */
static void _button_1_event_handler(lv_event_t *p_event);

//------------------------- STATIC DATA & CONSTANTS ---------------------------

//------------------------------- GLOBAL DATA ---------------------------------

//------------------------------ PUBLIC FUNCTIONS -----------------------------
void gui_app_init(void)
{
    /* Create buttons */
    lv_obj_t *p_label1;
    lv_obj_t *p_btn1 = lv_btn_create(lv_scr_act());
    lv_obj_align_to(p_btn1, NULL, LV_ALIGN_CENTER, 0, -80);
    p_label1 = lv_label_create(p_btn1);
    lv_label_set_text(p_label1, "Button 1");

    // Add button 1 callback
    (void)lv_obj_add_event_cb(p_btn1, _button_1_event_handler, LV_EVENT_CLICKED,
                              NULL);
}

//---------------------------- PRIVATE FUNCTIONS ------------------------------
static void _button_1_event_handler(lv_event_t *p_event)
{
    if (LV_EVENT_CLICKED == p_event->code) {
        printf("Button 1 clicked\n");
    } else {
        /* Do nothing.*/
    }
}

//---------------------------- INTERRUPT HANDLERS -----------------------------
