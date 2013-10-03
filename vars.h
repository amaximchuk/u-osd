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

Credit to Carl Ljungström.
*/

#ifndef VARS_H_
#define VARS_H_

// Key
static u8				g_key_pressed		= 0;
static u8				g_key_press_time	= 0;

// ADC
static u16				g_adc_raw[ANALOG_IN_NUMBERS];
static u8				g_adc_scale[ANALOG_IN_NUMBERS] = {ANALOG_IN_SCALE};

#ifdef SENSOR_BATTERY_PERCENTAGE_ENABLED
static u8				g_sensor_battery_percentage = 0;
#endif // SENSOR_BATTERY_PERCENTAGE_ENABLED

#ifdef SENSOR_RSSI_ENABLED
static u8				g_sensor_rssi		= 0;
#endif // SENSOR_RSSI_ENABLED

static TAnalogValue		g_sensor_voltage1;
static TAnalogValue		g_sensor_voltage2;

#ifdef SENSOR_CURRENT_ENABLED
static	u32				g_sensor_power_usage= 0;
static	TAnalogValue	g_sensor_current;
#endif // SENSOR_CURRENT_ENABLED

#ifdef SENSOR_COMPASS_ENABLED
static u8				g_sensor_compass_direction = 0;
#endif // SENSOR_COMPASS_ENABLED

// Time
static volatile u8		g_time_tick			= 0;
static volatile TTime	g_time				= {};
static volatile u8		g_blink				= 0;

// Stats
static volatile u16		g_stat_dist_traveled= 0;
static volatile u8		g_stat_max_speed	= 0;
static volatile s16		g_stat_max_altitude	= 0;
static volatile u16		g_stat_max_distance	= 0;

// GPS
static TGpsData			g_gps_valid_data;
static TGpsData			g_gps_data;
static TGpsPos			g_gps_stat_pos;

static BOOL				g_gps_stat_set		= 0;
static u8				g_gps_stat_packet	= 0;

#ifdef DEBUG_TEXT
static u16				g_gps_packets_bad	= 0;
static u16				g_gps_packets_total	= 0;
#endif //DEBUG_TEXT

static u32				g_home_distance		= 0;	// Distance to home
static u16				g_home_bearing		= 0;	// Direction to home

static TGpsPos			g_home_pos; 
static u8				g_home_set			= 0;

#ifdef HOME_SET_AT_FIX
static u8				g_home_fix_count	= 0;
#endif //HOME_SET_AT_FIX

// Text & Layout
static volatile u8		g_frame_sync		= 0;
static u8				g_active_line		= 0;

// rows
static u8				g_text_row;
static str_row			g_text_rows[TEXT_ROWS] = { 
	{25,	""},
	{37,	""},
		
	{82,	""},
	{90,	""},
	{98,	""},
	{106,	""},
	{114,	""},
	{122,	""},
	{130,	""},
	{138,	""},
	{146,	""},
	{154,	""},
	{162,	""},
	{170,	""},
		
	{216,	""},
	{228,	""},
	{240,	""},
};	

static str_text			g_text_sign				= {{0,	1	},	{16,1}};
static str_text			g_text_volt1			= {{0,	14	},	{7,	1}};
static str_text			g_text_volt2			= {{0,	15	},	{7,	1}};
static str_text			g_text_current			= {{27,	14	},	{7,	1}};
static str_text			g_text_power_usage		= {{27,	15	},	{8,	1}};
static str_text			g_text_time				= {{0,	0	},	{6,	1}};
static str_text			g_text_alt				= {{30,	8	},	{5,	1}};
static str_text			g_text_speed			= {{1,	8	},	{4,	1}};
static str_text			g_text_lat				= {{0,	16	},	{11,1}};
static str_text			g_text_lon				= {{23,	16	},	{12,1}};
static str_text			g_text_rssi				= {{31,	0	},	{4,	1}};
static str_text			g_text_sat				= {{31,	1	},	{4,	1}};
static str_text			g_text_bear_to_home		= {{15,	15	},	{5,	1}};
static str_text			g_text_dist_to_home		= {{15,	16	},	{6,	1}};
static str_text			g_text_stat_max_speed	= {{1,	2	},	{3,	1}};
static str_text			g_text_stat_max_dist	= {{21,	0	},	{7,	1}};
static str_text			g_text_stat_trip		= {{8,	0	},	{7,	1}};
static str_text			g_text_stat_max_alt		= {{31,	2	},	{4,	1}};
static str_scale		g_text_speed_scale		= {{5,	2	},	0};
static str_scale		g_text_alt_scale		= {{29,	2	},	0};
static str_radar		g_text_radar			= {{17,	8	},	{15,8}};
static str_text			g_text_home				= {{17,	14	},	{1,	1}};

#endif /* VARS_H_ */