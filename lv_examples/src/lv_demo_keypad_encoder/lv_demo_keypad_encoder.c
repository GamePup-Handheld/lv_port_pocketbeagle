/**
 * @file lv_demo_keypad_encoder.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "../../lv_examples.h"
#include "lv_demo_keypad_encoder.h"
#if LV_EX_KEYBOARD
#include "lv_drivers/indev/keyboard.h"
#endif
#if LV_EX_MOUSEWHEEL
#include "lv_drivers/indev/mousewheel.h"
#endif

#if LV_USE_DEMO_KEYPAD_AND_ENCODER


/**********************
 *  STATIC VARIABLES
 **********************/
static lv_group_t*  g;
static lv_obj_t* roller;

static void roller_event_cb(lv_obj_t* roller, lv_event_t e);

LV_EVENT_CB_DECLARE(dd_enc)
{
    if(e == LV_EVENT_VALUE_CHANGED) {
        /*printf("chg\n");*/
    }
}

void lv_demo_keypad_encoder(void)
{
    LV_THEME_DEFAULT_INIT(lv_theme_get_color_primary(), lv_theme_get_color_secondary(),
        LV_THEME_MATERIAL_FLAG_DARK,
        lv_theme_get_font_small(), lv_theme_get_font_normal(), lv_theme_get_font_subtitle(), lv_theme_get_font_title());

    g = lv_group_create();

#if LV_EX_KEYBOARD
    lv_indev_drv_t kb_drv;
    lv_indev_drv_init(&kb_drv);
    kb_drv.type = LV_INDEV_TYPE_KEYPAD;
    kb_drv.read_cb = keyboard_read;
    lv_indev_t * kb_indev = lv_indev_drv_register(&kb_drv);
    lv_indev_set_group(kb_indev, g);
#endif

    lv_obj_reset_style_list(lv_layer_top(), LV_OBJ_PART_MAIN);
    lv_obj_set_click(lv_layer_top(), false);
    lv_event_send(lv_scr_act(), LV_EVENT_REFRESH, NULL);

    lv_obj_reset_style_list(lv_layer_top(), LV_OBJ_PART_MAIN);
    lv_obj_set_click(lv_layer_top(), false);
    lv_event_send(lv_scr_act(), LV_EVENT_REFRESH, NULL);

    roller = lv_roller_create(lv_scr_act(), NULL);

    lv_roller_set_auto_fit(roller, false);
    lv_roller_set_visible_row_count(roller, 4);
    lv_obj_set_width(roller, 128);
    lv_obj_set_height(roller, 160);
    lv_roller_set_options(roller, "Pacman\nRobby\nGridlee\nCircus\nCarpolo", LV_ROLLER_MODE_NORMAL);
    lv_obj_set_event_cb(roller, roller_event_cb);
    lv_group_add_obj(g, roller);
}


static void roller_event_cb(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED) {
        char buf[32];
        lv_roller_get_selected_str(obj, buf, sizeof(buf));
        printf("Selected game: %s\n", buf);
    }
}


#endif
