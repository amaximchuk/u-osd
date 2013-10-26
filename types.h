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

#ifndef TYPES_H_
#define TYPES_H_

// ADC
typedef struct {
	u8			high;
	u8			low;
} TAnalogValue;

// Time
typedef struct {
	u8			sec;
	u8			min;
#ifdef TIME_HOUR_ENABLED
	u8			hour;
#endif //TIME_HOUR_ENABLED
} TTime;

// GPS
typedef struct {
	union {
		struct {
			u8	hour;
			u8	min	: 7;
			u8	neg	: 1;
			u16	sec100;
		};
		u32		data;			
	};		
} TGpsCoord;

typedef struct {
	TGpsCoord	latitude;
	TGpsCoord	longitude;
	s16			altitude;
} TGpsPos;

typedef struct {
	TGpsPos		pos;
	u16			angle;
	u8			speed;
	u8			sats;
	u8			fix;
	u8			chksum_valid;
} TGpsData;

// Text
typedef struct {
	u8			line;
	char		str[TEXT_ROW_MAX_CHARS];
}	str_row;

typedef struct {
	u8			x,y;
}	pos;

typedef struct {
	pos			p;
	struct {
		u8		length		: 7;
		u8		shadowed	: 1;
	};
}	str_text;

typedef struct {
	pos			p;
	u16			last_val;
}	str_scale;

typedef struct {
	pos			c;
	pos			last_pos;
}	str_radar;	

#endif /* TYPES_H_ */