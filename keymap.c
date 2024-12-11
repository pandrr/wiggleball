/* Copyright 2021 Colin Lam (Ploopy Corporation)
 * Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
 * Copyright 2019 Hiroyuki Okada
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
/*
#define MATRIX_ROWS 1
#define MATRIX_COLS 1
#define DIRECT_PINS {    { D0 }  }
*/
// Dummy
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = { [0]=LAYOUT(MOUSE_BTN1) };

// enum custom_keycodes {
//     DRAG_SCROLL = SAFE_RANGE,
// };

bool set_scrolling = false;



unsigned long time;


bool PloopyAcceleration = false;
bool PloopyNumlockScroll = false;
int16_t PloopyNumlockScrollVDir = 1;

int scrollX=0;
int scrollY=0;

int countShake=0;
bool lastDir=false;
int lastShake=0;
int lastShakeSwitch=0;

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {

    // wiggle ball detection
    if(timer_read()-lastShakeSwitch>250)
    {

        if(timer_read()-lastShake>150)countShake=0;

        if(mouse_report.x> 1 && mouse_report.y<3 && !lastDir)
        {
            countShake++;
            lastShake=timer_read();
            lastDir=!lastDir;
        }
        if(mouse_report.x<-1 && mouse_report.y<3 && lastDir)
        {
            countShake++;
            lastShake=timer_read();
            lastDir=!lastDir;
        }

        if(countShake>3)
        {
            set_scrolling=!set_scrolling;
            countShake=0;
            lastShake=timer_read();
            lastShakeSwitch=timer_read();
        }
    }

    bool doScroll = set_scrolling || host_keyboard_led_state().caps_lock|| host_keyboard_led_state().num_lock;


    if (doScroll) {
        // Scroll is very sensitive if you use the default values.
        // We can't divide it by anything to reduce the sensitivity, cause that would zero out small input values.
        // Instead we simply want either a 0, 1, or -1 depending on the input value's sign.
        // x = (x > 0 ? 1 : (x < 0 ? -1 : 0));
        // y = PloopyNumlockScrollVDir * (y > 0 ? 1 : (y < 0 ? -1 : 0));
        // mouse_report.v = x;
        // mouse_report.h = y;

		scrollX+=mouse_report.x;
		scrollY+=mouse_report.y;

		if(scrollX>150)
			{
				mouse_report.h=1;
				scrollX=0;
			}
		if(scrollX<-150)
			{
				mouse_report.h=-1;
				scrollX=0;
			}
		if(scrollY>150)
			{
				mouse_report.v=-1;
				scrollY=0;
			}
		if(scrollY<-150)
			{
				mouse_report.v=1;
				scrollY=0;
			}

        // if(mouse_report.x!=0)
        // {
        //     if(mouse_report.x>0)mouse_report.h=step;
        //     else mouse_report.h=-step;
        // }

        // if(mouse_report.y!=0)
        // {
        //     if(mouse_report.y>0)mouse_report.v=-step;
        //     else mouse_report.v=step;
        // }

	    mouse_report.x = 0;
	    mouse_report.y = 0;


        return mouse_report;
    }

    // if (PloopyAcceleration) {
    //     // Testing revealed the max reasonable x/y values were ~16.
    //     // `x*x/16 + x` results in ~2x speed at the max value, while maintaining 1x speed at the minimum.
    //     // But the x*x cancels the sign, so we need to negate it if the input value is negative.
    //     x = (x > 0 ? x*x/16+x : -x*x/16+x);
    //     y = (y > 0 ? y*y/16+y : -y*y/16+y);
    // }

    // mouse_report.x = x;
    // mouse_report.y = y;

    return mouse_report;
}


// Function to handle key events and enable/disable drag scrolling
bool process_record_user(uint16_t keycode, keyrecord_t *record) {


    switch (keycode) {
        case MOUSE_BTN1:
        	set_scrolling = !set_scrolling;//record->event.pressed;
            break;
        default:
            break;
    }
    return true;
}











