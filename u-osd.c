/*u-osd - A simple open source osd for G-OSD
Copyright (C) 2013 Alex Maximchuk

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <string.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "defines.h"
#include "config.h"

#include <util/delay.h>

#include "types.h"
#include "vars.h"

#include "oem6x8.h"

#include "math.h"
#include "utils.h"
#include "calcs.h"

#ifdef GPS_ENABLED
#	include "gps.h"
#endif // GPS_ENABLED

#include "layout.h"

#include "vars.h"
#include "setup.h"
#include "sensors.h"

static void update_time() {
	++g_time_tick;
	
	if (g_time_tick % (TIME_FRAMES_PER_SEC / 2) == 0) {
		g_blink		= !g_blink;
	}
	if (g_time_tick >= TIME_FRAMES_PER_SEC) {
		g_time_tick = 0;
		g_time.sec++;
	}
	if (g_time.sec >= 60) {
		g_time.sec = 0;
		g_time.min++;
	}
	if (g_time.min >= 60) {
		g_time.min = 0;
#ifdef TIME_HOUR_ENABLED
		g_time.hour++;
#endif //TIME_HOUR_ENABLED
	}
#ifdef TIME_HOUR_ENABLED
	if (g_time.hour >= 99) {
		g_time.hour = 0;
	}
#endif //TIME_HOUR_ENABLED
}

__attribute__((unused))
static void reset_time() {
	g_time_tick = 0;
	g_time.sec = 0;
	g_time.min = 0;
#ifdef TIME_HOUR_ENABLED
	g_time.hour = 0;
#endif //TIME_HOUR_ENABLED
}	

static void on_frame() {
	if (g_key_pressed) {
		++g_key_press_time;
	}

	if (g_time_tick % (TIME_FRAMES_PER_SEC / SENSOR_UPDATES_PER_SEC) == 0) {
		measure_analog	();
		update_sensors	();
		calc_distance_traveled();
	}
	if (g_time_tick % (TIME_FRAMES_PER_SEC / SCREEN_UPDATES_PER_SEC) == 0) {
		update_layout	();
	}

	update_time	();
}

__attribute__ ((noreturn))
void main(void) {
	setup				();

	while (1) {
#ifdef GPS_ENABLED		
		if(UCSR0A & (1<<RXC0)) {
			decode_gps	(UDR0);
		}
#endif // GPS_ENABLED		
    
		if((PIND & KEY) != KEY) {
			g_key_pressed = 1;
			if (g_key_press_time > TIME_FRAMES_PER_SEC) {
				PORTD	|= LED; // long press!
#ifdef HOME_SET_WITH_BUTTON
				if (g_gps_data.chksum_valid && g_gps_data.fix) { // (Improved thanks to Yury Smirnov)
					g_key_press_time = 0;
					set_home_pos();
					reset_time	();
				}
#endif //HOME_SET_WITH_BUTTON
			}
		} else {
			if (g_key_pressed) {
				PORTD		&= ~LED;  // led off
			}				
			g_key_pressed	= 0;
			g_key_press_time= 0;
		}

		if (g_frame_sync == 1) {
			g_frame_sync	= 0;
			on_frame		();
		}
	}
}

ISR(INT0_vect) {
	TCNT1		= 0;	// Reset sync lost timeout.
	update_line	();
}

ISR(TIMER1_OVF_vect)
{
	static u8 last_line = 0;
	if (last_line == g_active_line) {
		PORTD	^= LED; // Toggle the LED
	}
	last_line	= g_active_line;
}

