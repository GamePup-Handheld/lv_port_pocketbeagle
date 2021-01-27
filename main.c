#include "lvgl/lvgl.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_examples/lv_examples.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include "lv_examples/src/lv_ex_get_started/lv_ex_get_started.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#define DISP_BUF_SIZE (LV_VER_RES_MAX* LV_HOR_RES_MAX)


#include "lv_drivers/indev/keyboard.h"
#include "lv_drivers/indev/mousewheel.h"


static lv_group_t*  g;
static lv_obj_t* roller;

typedef struct coordinates
{
    int x;
    int y;
}coordinates;

static void roller_event_cb(lv_obj_t* obj, lv_event_t event);
uint32_t custom_tick_get(void);

void lv_ex_get_started_1(void);

int main(void)
{
    //system("sudo echo 0 | sudo tee -a /sys/class/graphics/fbcon/cursor_blink");
    int fd = open("/sys/class/graphics/fbcon/cursor_blink", O_WRONLY);
    write(fd, "0", 1);
    /*LittlevGL init*/
    lv_init();

    /*Linux frame buffer device init*/
    fbdev_init();

    /*A small buffer for LittlevGL to draw the screen's content*/
    static lv_color_t buf[DISP_BUF_SIZE];

    /*Initialize a descriptor for the buffer*/
    static lv_disp_buf_t disp_buf;
    lv_disp_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);

    /*Initialize and register a display driver*/
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.buffer   = &disp_buf;
    disp_drv.flush_cb = fbdev_flush;
    lv_disp_drv_register(&disp_drv);

    /*Create a Demo*/
    //lv_demo_widgets();
    lv_ex_get_started_1();

    /*Handle LitlevGL tasks (tickless mode)*/
    while(1) {
        lv_task_handler();
        usleep(5000);
    }

    return 0;
}

void lv_ex_get_started_1(void)
{


     LV_THEME_DEFAULT_INIT(lv_theme_get_color_primary(), lv_theme_get_color_primary(),
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



/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if(start_ms == 0) {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;
    return time_ms;
}

static void roller_event_cb(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED) {
        char buf[32];
        lv_roller_get_selected_str(obj, buf, sizeof(buf));
        printf("Selected game: %s\n", buf);
    }
}


// #include "lvgl/lvgl.h"
// #include "lv_drivers/display/fbdev.h"
// #include "lv_examples/lv_examples.h"
// #include <unistd.h>
// #include <pthread.h>
// #include <time.h>
// #include <sys/time.h>
// #include "lv_examples/src/lv_ex_get_started/lv_ex_get_started.h"
// #include <stdlib.h>
// #include <stdio.h>
// #include <fcntl.h>
// #define DISP_BUF_SIZE (LV_VER_RES_MAX* LV_HOR_RES_MAX)



// typedef struct coordinates
// {
//     int x;
//     int y;
// }coordinates;
// void create_button_test (lv_obj_t* btn, lv_obj_t* label, char* text, coordinates position, coordinates size);
// static void btn_event_cb(lv_obj_t * btn, lv_event_t event);
// void lv_ex_get_started_1(void);

// int main(void)
// {
//     //system("sudo echo 0 | sudo tee -a /sys/class/graphics/fbcon/cursor_blink");
//     int fd = open("/sys/class/graphics/fbcon/cursor_blink", O_WRONLY);
//     write(fd, "0", 1);
//     /*LittlevGL init*/
//     lv_init();

//     /*Linux frame buffer device init*/
//     fbdev_init();

//     /*A small buffer for LittlevGL to draw the screen's content*/
//     static lv_color_t buf[DISP_BUF_SIZE];

//     /*Initialize a descriptor for the buffer*/
//     static lv_disp_buf_t disp_buf;
//     lv_disp_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);

//     /*Initialize and register a display driver*/
//     lv_disp_drv_t disp_drv;
//     lv_disp_drv_init(&disp_drv);
//     disp_drv.buffer   = &disp_buf;
//     disp_drv.flush_cb = fbdev_flush;
//     lv_disp_drv_register(&disp_drv);

//     /*Create a Demo*/
//     //lv_demo_widgets();
//     lv_ex_get_started_1();

//     /*Handle LitlevGL tasks (tickless mode)*/
//     while(1) {
//         lv_task_handler();
//         usleep(5000);
//     }

//     return 0;
// }

// void lv_ex_get_started_1(void)
// {

    

    
//     char names[6][10] = {
//                          "Joshua",
//                          "Jaelyn",
//                          "Andrew",
//                          "Bibartan",
//                          "Darius",
//                          "Eeshan"
//                      };
                     

//     coordinates position = {10,10};
//     coordinates size = {120,18};
    
//     for(int i = 0; i < 6; i++){
        
//         lv_obj_t* btn;     /*Add a button the current screen*/
//         lv_obj_t* label;
        
//         create_button_test(btn,label,names[i],position,size);
//         position.y += size.y;

//     }
    
// }


// void create_button_test (lv_obj_t* btn, lv_obj_t* label, char* text, coordinates position,  coordinates size){
    
    
//     btn = lv_btn_create(lv_scr_act(), NULL);     /*Add a button the current screen*/
//     lv_obj_set_pos(btn, position.x, position.y);                            /*Set its position*/
//     lv_obj_set_size(btn, size.x, size.y);                          /*Set its size*/
//     lv_obj_set_event_cb(btn, btn_event_cb);                 /*Assign a callback to the button*/

//     label = lv_label_create(btn, NULL);          /*Add a label to the button*/
//     lv_label_set_text(label,text);                     /*Set the labels text*/
    
// }

// /*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
// uint32_t custom_tick_get(void)
// {
//     static uint64_t start_ms = 0;
//     if(start_ms == 0) {
//         struct timeval tv_start;
//         gettimeofday(&tv_start, NULL);
//         start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
//     }

//     struct timeval tv_now;
//     gettimeofday(&tv_now, NULL);
//     uint64_t now_ms;
//     now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

//     uint32_t time_ms = now_ms - start_ms;
//     return time_ms;
// }

// static void btn_event_cb(lv_obj_t * btn, lv_event_t event)
// {
//     if(event == LV_EVENT_CLICKED) {
//         static uint8_t cnt = 0;
//         cnt++;

//         /*Get the first child of the button which is the label and change its text*/
//         lv_obj_t * label = lv_obj_get_child(btn, NULL);
//         lv_label_set_text_fmt(label, "Button: %d", cnt);
//     }
// }
