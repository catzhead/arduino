#ifndef __SENSOR__
#define __SENSOR__

/*
 * attach_sensor: attach the function to the pin to which the sensor
 *                is connected. 
 *                   
 * For info, pin 21 works on Arduino Mega
 */

void attach_sensor(int pin);

/*
 * detect: function called when something is detected on the sensor
 */

void detect();

/*
 * get_wheel_speed: returns the wheel speed in rpm
 */

float get_wheel_speed();

#endif
