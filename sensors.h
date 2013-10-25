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

#ifndef SENSORS_H_
#define SENSORS_H_

__attribute__((noinline))
static void calc_adc(TAnalogValue *r, u8 input) {
	u16 raw		= g_adc_raw[input];
	u8 scale	= g_adc_scale[input];
	raw			*= scale;
	u8 high		= raw >> 10;			// / 1024	
	raw			= raw - ((high) << 10);	// * 1024
	u8 low		= (raw * 25) >> 8;		// * 100 / 1024	
	r->high		= high;
	r->low		= low;
}

__attribute__((noinline,unused))
static u16 calc_level(u8 input, u16 in_min, u16 in_max, u16 out_min, u16 out_max, BOOL reversed) {
	TAnalogValue v; calc_adc(&v, input);
	u32 level	= v.high * 100 + v.low;
	if (level > in_max) {
		level	= out_max;
	} else if (level > in_min) {
		level	-= in_min;
		level	*= out_max - out_min;
		level	/= in_max - in_min;
		level	+= out_min;
	} else {
		level	= out_min;
	}
	if (reversed)
		level	= out_max - level + out_min;
	return		level;
}

static void update_sensors() {
    calc_adc	(&g_sensor_voltage1, ANALOG_IN_1);
    calc_adc	(&g_sensor_voltage2, ANALOG_IN_2);

#ifdef SENSOR_CURRENT_ENABLED
	u16 curr			= calc_level(CURRENT_INPUT, CURRENT_MIN_VOLTAGE_INT, CURRENT_MAX_VOLTAGE_INT, 0, CURRENT_MAX_AMPS_INT, FALSE);
	// non-linear correction (magic numbers)
	curr				= curr ? (((u32)curr * 93 / 100) + 18) : 0;
	g_sensor_power_usage	+= (((u32)(curr) * 10 << 10) / SENSOR_UPDATES_PER_SEC / 3600);
	g_sensor_current.high	= curr / 100;
	g_sensor_current.low	= (curr - (g_sensor_current.high * 100));
#endif		// SENSOR_CURRENT_ENABLED

#ifdef SENSOR_BATTERY_PERCENTAGE_ENABLED
   g_sensor_battery_percentage = calc_level(SENSOR_BATTERY_PERCENTAGE_INPUT, BATT_MIN_VOLTAGE_INT, BATT_MAX_VOLTAGE_INT, 0, 100);
#endif		// SENSOR_BATTERY_PERCENTAGE_ENABLED

#ifdef SENSOR_RSSI_ENABLED
	g_sensor_rssi = calc_level(RSSI_INPUT, RSSI_MIN_VOLTAGE_INT, RSSI_MAX_VOLTAGE_INT, 0, 100, RSSI_REVERSED);
#endif		// SENSOR_RSSI_ENABLED

#ifdef SENSOR_COMPASS_ENABLED
   g_sensor_compass_direction = calc_level(SENSOR_COMPASS_INPUT, COMPASS_MIN_VOLTAGE_INT, COMPASS_MAX_VOLTAGE_INT, 0, 359);
#endif		// SENSOR_COMPASS_ENABLED
}  

static void measure_analog() {
	for (u8 i = 0; i < ANALOG_IN_NUMBERS; ++i) {
		ADMUX		&= 0xF0;					// Clear mux
		ADMUX		|= (i + ADC_OFFSET);		// Setup adc mux
		ADCSRA		|= (1<<ADEN) | (1<<ADATE);	// ADC enable & ADC auto trigger enable
		ADCSRA		|= (1<<ADSC);				// Start measure
		while ((ADCSRA & (1<<ADIF)) == 0);		// Wait to finish
		ADCSRA		|= (1<<ADIF);				// Clear ADC interrupt flag with a 1
		ADCSRA		&= ~(1<<ADEN) & ~(1<<ADATE);// ADC disabled & ADC auto trigger disabled
		g_adc_raw[i]= ADCW;
	}    
}

#endif /* SENSORS_H_ */