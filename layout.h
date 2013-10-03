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

#ifndef LAYOUT_H_
#define LAYOUT_H_

//0                   1                   2                   3
//0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4
//----------------------------------------------------------------------//
//0 0 0 : 0 0       T 9 9 9 9 m             M 9 9 9 9 m         A 9 9 % // 0 
//K B D 9 9 9                                                   S 0 3   // 1 
//                                                                      //
//  9 9 9   |                                               |   9 9 9 9 // 2 
//          |                 . . . . . . .                 |           // 3 
//          |               . . . . . . . . .               |           // 4 
//          |             . . . . . . . . . . .             |           // 5 
//          |             . . . . . . . . . . .             |           // 6 
//          |             . . . . . . . . . . .             |           // 7 
//  9 9 9 > |             . . . . . O . . . . .             | < 9 9 9 9 // 8 
//          |             . . . . . . . . . . .             |           // 9 
//          |             . . . . . . . . . . .             |           //10 
//          |             . . . . . ^ . . . . .             |           //11 
//          |               . . . . . . . . .               |           //12 
//          |                 . . . . . . .                 |           //13 
//                                                                      // 
//                                                                      //
//P 0 7 . 5 2 V                     H                   P 0 9 . 0 0 A   //14 
//V 0 7 . 5 2 V                 < 1 7 9 >               P 9 9 9 9 m A h //15 
//N 0 0 . 0 0 . 0 0 0 0         9 9 9 9 m       E 0 0 0 . 0 0 . 0 0 0 0 //16  
//----------------------------------------------------------------------//
//0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4
//0                   1                   2                   3

__attribute__((noinline))										
pstr str_ptr(u8 row, u8 col) {
	return			g_text_rows[row].str + col;
}	

__attribute__((noinline))
void str_shadowed(str_text *str) {
	if (!str->shadowed) return;
	pstr dst		= str_ptr(str->p.y,str->p.x);
	pcstr dst_end	= dst + str->length;
	while (dst < dst_end) {
		if (0 != *dst)
			*dst	|= CHAR_SH;
		dst++;
	}		
}

__attribute__((noinline))
void str_clear_tail(pstr ptr, pcstr ptr_end) {
	while (ptr < ptr_end)
		*ptr++		= 0;
}	

__attribute__((noinline))
void str_fill(str_text *str, char ch) {
	pstr dst		= str_ptr(str->p.y,str->p.x);
	pcstr dst_end	= dst + str->length;
	while (dst < dst_end)
		*dst++		= ch;
}	

__attribute__((noinline))
pstr print_text(str_text *str, pcstr src, BOOL clear_tail) {
	pstr dst		= str_ptr(str->p.y,str->p.x);
	pcstr dst_end	= dst + str->length;
	while (*src && dst < dst_end)
		*dst++		= (*src++);
	str_shadowed	(str);
	if (clear_tail)	
		str_clear_tail	(dst,dst_end);
	return			dst;
}

__attribute__((noinline))
void print_float(str_text *str, u8 high, u8 hwidth, u8 hmin, u8 low, u8 lwidth, u8 lmin, char prefix, char suffix, char delim) {
	pstr dst		= str_ptr(str->p.y,str->p.x);
	pcstr dst_end	= dst + str->length;

	if (prefix)	
		*dst++		= prefix;
	dst				= utoa16(high,dst,hwidth,hmin);
	if (delim)
		*dst++		= delim;
	dst				= utoa16(low,dst,lwidth,lmin);
	if (suffix)	
		*dst++		= suffix;

	str_shadowed	(str);
	str_clear_tail	(dst, dst_end);
}	

__attribute__((noinline))
void print_decimal8(str_text *str, u8 val, u8 m, char prefix, char suffix) {
	pstr dst		= str_ptr(str->p.y,str->p.x);
	pcstr dst_end	= dst + str->length;

	u8 w			= str->length;
	if (prefix) {
		*dst++		= prefix;
		w--;
	}
	if (suffix) 	w--;
	dst				= utoa16(val,dst,w,m);
	if (suffix)		*dst++		= suffix;

	str_shadowed	(str);
	str_clear_tail	(dst, dst_end);
}	

__attribute__((noinline))
pstr print_decimal(str_text *str, u16 val, u8 m, char prefix, char suffix) {
	pstr dst		= str_ptr(str->p.y,str->p.x);
	pcstr dst_end	= dst + str->length;

	u8 w			= str->length;
	if (prefix) {
		*dst++		= prefix;
		w--;
	}
	if (suffix) 	w--;
	dst				= utoa16(val,dst,w,m);
	if (suffix)		*dst++		= suffix;
	
	str_shadowed	(str);
	str_clear_tail	(dst, dst_end);
	return			dst;
}	

__attribute__((noinline))
void print_gps_coord(str_text *str, TGpsCoord *val, BOOL lat) {
	pstr dst		= str_ptr(str->p.y,str->p.x);

	u8	h			= val->hour;
	u8	m			= val->min; 
	u16	dm			= val->sec100;
	s8	neg			= val->neg;
	
	dm				*= 10;
	dm				/= 6;//div6_u16(dm);
	
	if (lat) 		*dst++	= neg ? 'S' : 'N';
	else {
		if (h < 100)*dst++	= 0;
		*dst++		= neg ? 'W' : 'E';
	}			
	dst				= utoa16(h,dst,3,2);
	*dst++			= '\'';
	dst				= utoa16(m,dst,2,2);
	*dst++			= '.';
	dst				= utoa16(dm,dst,4,4);
	
	str_shadowed	(str);
}	

__attribute__((noinline))
void print_scale(str_scale *str, u16 new_val, char c00, char c01, char c1) {
	//val				+= 2;	
	u16 val			= str->last_val;
	val				= (5 * val	+ 5 * new_val)	/ 10;
	str->last_val	= val;
	
	pstr ptr		= 0;
	u8 rem			= val - ((val >> 2) << 2);
	u8 a, b;
	if (0 == rem)		{	a = c1;  b = c00;	}
	else if (1 == rem)	{	a = c1;  b = c01;	}
	else if (2 == rem)	{	a = c00; b = c1;	}
	else				{	a = c01; b = c1;	}

	u8 x			= str->p.x;
	u8 y			= str->p.y;
	for (u8 i = 0; i < TEXT_SCALE_HEIGHT; ++i) { 
		u8 c		= i & 1 ? a : b;
		ptr			= str_ptr(y+i,x);
		*ptr		= c;
	}
}

__attribute__((noinline))
u8 radar_calc_pos(u8 r, u16 angle, s8 *rx, s8 *ry) {
	s8 x			=  ((r * cosT(angle + 90)) / 256);
	s8 y			=  ((r * sinT(angle + 90)) / 256);
	*rx	= x; *ry = y;
	return x * x + y * y;
}

void print_location(str_radar *loc, str_text *course, str_text *home, u16 bearing, u16 angle, u16 dist, u8 speed, u8 home_set, u8 b1Hz) {
	u8 r;
	if (dist > TEXT_RADAR_DIST_5)		r = 5;
	else if (dist > TEXT_RADAR_DIST_4)	r = 4;
	else if (dist > TEXT_RADAR_DIST_3)	r = 3;
	else if (dist > TEXT_RADAR_DIST_2)	r = 2;
	else if (dist > TEXT_RADAR_DIST_1)	r = 1;
	else								r = 0;

	u8 lim			= ((2*r + 1) * (2*r + 1)) >> 2;
	u8 jit			= 0;
	s8				rx,ry;
	do {
		if (radar_calc_pos(r + 1, bearing + jit, &rx, &ry) <= lim)
			break;
		if (radar_calc_pos(r + 1, bearing - jit, &rx, &ry) <= lim)
			break;
	} while (++jit < 8);

	u8 cx			= loc->c.x;
	u8 cy			= loc->c.y;

	rx				+= cx;
	ry				+= cy;

	pstr ptr		= 0;
	u8 angle_idx	= ((angle + 1 + 22) * 91) >> 12;	// x / 45
	if (angle_idx >= 8) angle_idx	= 0;

	// radar
	u8 lx			= loc->last_pos.x;
	u8 ly			= loc->last_pos.y;
	char rs			= (speed > 1) ? 0x5B + angle_idx : 0x6C;
	ptr				= str_ptr(ly,lx);	*ptr = 0;
	ptr				= str_ptr(cy,cx);	*ptr = 0x21;
	ptr				= str_ptr(ry,rx);	*ptr = rs;

	loc->last_pos.x	= rx;
	loc->last_pos.y	= ry;

	// course
	s16 rth			= (bearing - angle);
	if (rth < 0)	rth += 360;
	u8 rth_idx		= ((rth + 1 + 22) * 91) >> 12;		// x / 45
	if (rth_idx >= 8) rth_idx	= 0;
	rs				= 0x5B + rth_idx;
	print_decimal	(course, rth,	3, ' ',' ');
	if (rth < 180)	print_decimal	(course, rth,		3, ' ',rs);
	else			print_decimal	(course, 360-rth,	3, rs,' ');

	// home symbol	
	pstr dst		= str_ptr(home->p.y,home->p.x);
	char sh			= home->shadowed ? CHAR_SH : 0;
	*dst			= sh | (home_set ? ')' : b1Hz ? '(' : ' ');
}

static void update_layout() {
	u8 blink			= g_blink;

	if (g_gps_data.fix) {
		if (g_home_set)	PORTD |= LED;
		else {
			if (blink)	PORTD |= LED;
			else		PORTD &=~LED;
		}			
	} else {
		PORTD		^= LED;
	}
	if (g_home_set) {
		u32			distance;
		calc_home	(&g_gps_valid_data.pos, &g_home_pos, &distance, &g_home_bearing);
		if (distance > g_stat_max_distance) {
			g_stat_max_distance = distance;
		}
		g_home_distance	= distance;
	}

	// smoothed
	s16 alt_rel		= g_gps_valid_data.pos.altitude - g_home_pos.altitude;
	u8 speed		= g_gps_valid_data.speed;
	
	u8 sat_fix		= g_gps_valid_data.fix;
	
	// alarms
	u8 aSpeed		= speed > ALARM_SPEED_HIGH; 
	u8 aAltitude	= alt_rel > ALARM_ALTITUDE_HIGH || alt_rel < ALARM_ALTITUDE_LOW;
	u8 aDistToHome	= (g_home_distance > ALARM_DISTANCE_HIGH);
	u8 aBatt1		= g_sensor_voltage1.high&& (((g_sensor_voltage1.high * 100) + g_sensor_voltage1.low) < ALARM_BATT1_LOW_INT);
	u8 aBatt2		= g_sensor_voltage2.high&& (((g_sensor_voltage2.high * 100) + g_sensor_voltage2.low) < ALARM_BATT2_LOW_INT);
	u8 aRssi		= g_sensor_rssi			&& (g_sensor_rssi < ALARM_RSSI_LOW);
	
	// print values
	print_float		(&g_text_time,	g_time.min, 3,2, g_time.sec, 2,2, 0, 0, ':');

	if (sat_fix) {
		print_gps_coord	(&g_text_lon,	&g_gps_valid_data.pos.longitude,0);
		print_gps_coord	(&g_text_lat,	&g_gps_valid_data.pos.latitude,	1);
	} else {
		str_fill	(&g_text_lon, '-');
		str_fill	(&g_text_lat, '-');
	}		
	if (sat_fix || blink)	print_decimal	(&g_text_sat, g_gps_valid_data.sats,2, '&', "-?;/"[g_gps_stat_packet]);
	else					str_fill		(&g_text_sat, ' ');

	if (!aBatt1 || blink) 	print_float	(&g_text_volt1, g_sensor_voltage1.high, 2, 1, g_sensor_voltage1.low, 2, 2, 0x2A, 'V', '.');
	else					str_fill	(&g_text_volt1, ' ');
	if (!aBatt2 || blink) 	print_float	(&g_text_volt2, g_sensor_voltage2.high, 2, 1, g_sensor_voltage2.low, 2, 2, 0x2B, 'V', '.');
	else					str_fill	(&g_text_volt2, ' ');
	if (!aRssi || blink)	print_decimal(&g_text_rssi,g_sensor_rssi, 2, 0x40, '%');
	else					str_fill	(&g_text_rssi, ' ');

	print_float		(&g_text_current,			g_sensor_current.high, 2, 1, g_sensor_current.low, 2, 2, 0x3D, 'A', '.');	
	{
		pstr str	=		print_decimal	(&g_text_power_usage, g_sensor_power_usage >> 10,	4,	0x3D, 'm');	
		u8 sh		= g_text_power_usage.shadowed ? CHAR_SH : 0;
		*str++		= sh | 'A';
		*str++		= sh | 'h';
	}
	
	print_decimal	(&g_text_stat_max_speed,	g_stat_max_speed,		3, 0, 0);
	print_decimal	(&g_text_stat_max_alt,		g_stat_max_altitude,	4, 0, 0);
	print_decimal	(&g_text_stat_trip,			g_stat_dist_traveled,	4, 0x69,TEXT_LENGTH_UNIT);
	print_decimal	(&g_text_stat_max_dist,		g_stat_max_distance,	4, 0x6A,TEXT_LENGTH_UNIT);
	
	// dbg
#ifdef DEBUG_TEXT
	print_decimal	(&g_text_stat_trip,			g_home_bearing,			4, 'B', 0);
	print_decimal	(&g_text_stat_max_dist,		g_gps_valid_data.angle,	4, 'A', 0);
	print_decimal	(&g_text_gps_total,			g_gps_total_packet,		5, 'T', 0);
	print_decimal	(&g_text_gps_bad,			g_gps_bad_packet,		5, 'B', 0);
#endif //DEBUG_TEXT
	
	
	if (!aDistToHome || blink)	print_decimal	(&g_text_dist_to_home,	g_home_distance,	4, 0,TEXT_LENGTH_UNIT);
	else						str_fill		(&g_text_dist_to_home, ' ');
	
	if (alt_rel < 0)			alt_rel = 0;
	if (!aAltitude || blink)	print_decimal	(&g_text_alt,	alt_rel, 5,'<',0);
	else						str_fill		(&g_text_alt,	' ');

	if (!aSpeed || blink)		print_decimal	(&g_text_speed,	speed,	 4, 0, '>');
	else						str_fill		(&g_text_speed, ' ');
	
	// draw scales
	print_scale		(&g_text_alt_scale,		alt_rel,0x22,0x23,0x24);
	print_scale		(&g_text_speed_scale,	speed,	0x63,0x64,0x65);
	
	print_location	(&g_text_radar, &g_text_bear_to_home, &g_text_home, g_home_bearing,  g_gps_valid_data.angle, g_home_distance, speed, g_home_set, blink);
}

static void draw_line(u8 row, u8 line)
{	
	// 1185 cycles
	_delay_us			(0.7);
	pcstr str			= g_text_rows[row].str;
	u16 line_offs		= (u16)oem6x8 + CHAR_LENGTH * line;
	
	u8 it				= TEXT_ROW_MAX_CHARS;
	do {
//		DELAY_1_NOP		();
		// 28 cycles
		u8 out;
		BOOL sh;
		const u8 ch		= *str++;
		if (ch) {
			u8 of		= ch;
			BIT_CLEAR	(of,CHAR_SH_BIT);
			of			-= CHAR_OFFSET;
			u16 addr	= line_offs + of;
			out			= pgm_read_byte_near(addr);
			sh			= ch & CHAR_SH;
		} else {
			sh			= FALSE;
			out			= 0;
			DELAY_9_NOP();
		}				
		if (sh)			{	BIT_SET(DDRB, BLACK_OUT);					}
		else			{	BIT_CLEAR(DDRB, BLACK_OUT);	DELAY_1_NOP();	}
		SPDR			= out;

		DELAY_9_NOP	();
		DELAY_1_NOP		();
//		DELAY_1_NOP		();
	} while (--it);					
	DELAY_10_NOP		();
	DELAY_7_NOP			();
	SPDR				= 0x00;
	BIT_CLEAR			(DDRB, BLACK_OUT);
}

static void update_line() {
	_delay_us(4.5);					// wait 4.5 us to see if H or V sync
	if(!BIT_TEST(PIND, LTRIG_IN)) { // H sync
		u8 rnum		= g_text_row;
		if (rnum < TEXT_ROWS) {
			u8 line		= g_active_line;
			u8 r_line	= g_text_rows[rnum].line;
			u8 l_end	= r_line + TEXT_CHAR_HEIGHT;
			if (line >= r_line && line < l_end) {
				u8 pmline	= (line - r_line);
				draw_line	(rnum,pmline);
			}
			line++;
			if (line == 255)	line--;
			if (line == l_end)	rnum++;
			g_text_row		= rnum;
			g_active_line	= line;
		
			if (line == FRAME_SYNC_LINE)
				g_frame_sync = 1;
		}
	} else { // V sync
		if(g_active_line > 200) {
			g_active_line	= 0;
			g_text_row		= 0;
		}
	}
}

#endif /* LAYOUT_H_ */