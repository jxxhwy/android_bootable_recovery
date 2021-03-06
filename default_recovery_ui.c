/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <linux/input.h>

#include "recovery_ui.h"
#include "common.h"
#include "extendedcommands.h"

/*
	to enable on-screen debug code printing set this to 1-6
	to disable on-screen debug code printing set this to 0
		TOUCH_CONTROL_DEBUG = 1  >>>>   Prints events code, type and value
		TOUCH_CONTROL_DEBUG = 2  >>>>   Prints gr_fb_width() and gr_fb_height() values
		TOUCH_CONTROL_DEBUG = 3  >>>>   Prints Touch X and Y co-ordinates
		TOUCH_CONTROL_DEBUG = 4  >>>>   Prints Action issued by touch events
		TOUCH_CONTROL_DEBUG = 5  >>>>   Prints Action issued for last two debugs
		TOUCH_CONTROL_DEBUG = 6  >>>>   Prints Action returned to recovery.c through ui_wait_key()
*/
int TOUCH_CONTROL_DEBUG = 0;


//Device specific boundaries for touch recognition
/*
	WARNING
	these might not be the same as resX, resY (from below)
	these have to be found by setting them to zero and then in debug mode
	check the values returned by on screen touch output by click on the
	touch panel extremeties
*/
//int maxX=1535;		//Set to 0 for debugging
//int maxY=2559;		//Set to 0 for debugging

/*
	the values of following two variables are dependent on specifc device resolution
	and can be obtained using the outputs of the gr_fb functions
*/
#define resX gr_fb_width()		//Value obtained from function 'gr_fb_width()'
#define resY gr_fb_height()		//Value obtained from function 'gr_fb_height()'

char* MENU_HEADERS[] = { "developed by Napstar",
			 "",
			NULL };

char* MENU_ITEMS[] = { "reboot system now",
                       "install zip",
                       "wipe data/factory reset",
                       "wipe cache partition",
                       "backup and restore",
                       "mounts and storage",
                       "advanced",
                       NULL };

void device_ui_init(UIParameters* ui_parameters) {
}

int device_recovery_start() {
    return 0;
}

int device_reboot_now(volatile char* key_pressed, int key_code) {
    return 0;
}

int device_perform_action(int which) {
    return which;
}

int device_wipe_data() {
    return 0;
}

int get_menu_icon_info(int indx1, int indx2) {
//ToDo: Following switch case should be replaced by array or structure

int caseN = indx1*4 + indx2;
/*
int MENU_ICON1[] = {
		{  1*resX/8,	(resY - MENU_MAX_HEIGHT/2), 0*resX/4, 1*resX/4 },
		{  3*resX/8,	(resY - MENU_MAX_HEIGHT/2), 1*resX/4, 2*resX/4 },
		{  5*resX/8,	(resY - MENU_MAX_HEIGHT/2), 2*resX/4, 3*resX/4 },
		{  7*resX/8,	(resY - MENU_MAX_HEIGHT/2), 3*resX/4, 4*resX/4 },
	};

*/

switch (caseN) {
	case 0:
		return 1*resX/8;
	case 1:
		return (resY - MENU_MAX_HEIGHT()/2);
	case 2:
		return 0*resX/4;
	case 3:
		return 1*resX/4;
	case 4:
		return 3*resX/8;
	case 5:
		return (resY - MENU_MAX_HEIGHT()/2);
	case 6:
		return 1*resX/4;
	case 7:
		return 2*resX/4;
	case 8:
		return 5*resX/8;
	case 9:
		return (resY - MENU_MAX_HEIGHT()/2);
	case 10:
		return 2*resX/4;
	case 11:
		return 3*resX/4;
	case 12:
		return 7*resX/8;
	case 13:
		return (resY - MENU_MAX_HEIGHT()/2);
	case 14:
		return 3*resX/4;
	case 15:
		return 4*resX/4;

}

return 0;
}


//For those devices which has skewed X axis and Y axis detection limit (Not similar to XY resolution of device), So need normalization
int MT_X(int fd, int x)
{
	int abs_store[6] = {0};

    	ioctl(fd, EVIOCGABS(ABS_MT_POSITION_X), abs_store);
    	int maxX = abs_store[2];

	int out;
	out = maxX ? (int)((float)x*gr_fb_width()/maxX) : x;

	return out;
}

int MT_Y(int fd, int y)
{
	int abs_store[6] = {0};
    	
    	ioctl(fd, EVIOCGABS(ABS_MT_POSITION_Y), abs_store);
    	int maxY = abs_store[2];

	int out;
	out = maxY ? (int)((float)y*gr_fb_height()/maxY) : y;

	return out;
}


//In this case MENU_SELECT icon has maximum possible height.
//Maximum allowed height for navigation icons
 int MENU_MAX_HEIGHT()
{
	return gr_get_height(gMenuIco[MENU_SELECT]);
}
