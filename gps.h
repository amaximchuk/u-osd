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

Credit to Carl Ljungstrцm.
*/

#ifndef GPS_H_
#define GPS_H_

static void set_home_pos() {
	// reset stats
	g_stat_dist_traveled= 0;
	g_stat_max_speed	= 0;
	g_stat_max_altitude	= 0;
	g_stat_max_distance	= 0;

	g_home_pos	= g_gps_valid_data.pos;

#ifdef DEBUG_TEXT
	g_gps_packets_total	= 0;
	g_gps_packets_bad	= 0;
#endif // DEBUG_TEXT

	g_home_set	= 1;
}

static void finish_gps_decoding() {
	if (g_gps_data.chksum_valid) {
		if (++g_gps_stat_packet > 3) 
			g_gps_stat_packet = 0;
		
		g_gps_valid_data	= g_gps_data;
		
		if (g_home_set == 0) {
			if (g_gps_valid_data.fix) {
#ifdef HOME_SET_AT_FIX
				if (g_home_fix_count >= HOME_SET_FIX_COUNT) {
					if (g_gps_valid_data.sats >= HOME_SET_MIN_SATS) { // (Improved thanks to Yury Smirnov)
						set_home_pos();
					}				  
				} else {
				    ++g_home_fix_count;
			    }
#endif //HOME_FIRST_FIX
#ifdef HOME_AUTO_SET
				if (g_gps_valid_data.speed >= HOME_FIX_MIN_SPEED) {
					set_home_pos();
				}
#endif //HOME_AUTO_SET
			}
		} else {
			if (g_gps_valid_data.speed > g_stat_max_speed) {
				g_stat_max_speed = g_gps_valid_data.speed;
			}
			if (g_gps_valid_data.pos.altitude - g_home_pos.altitude > g_stat_max_altitude) {
				g_stat_max_altitude = g_gps_valid_data.pos.altitude - g_home_pos.altitude;
			}
		}
	}
#ifdef DEBUG_TEXT
	g_gps_packets_total++;
	if (g_gps_data.chksum_valid)
		g_gps_packets_bad++;
#endif // DEBUG_TEXT
}

#if defined(GPS_DIYD)
#	include "gps-diyd.h"
#else
#	include "gps-nmea.h"
#endif

#endif /* GPS_H_ */