#ifndef __SENSOR__
#define __SENSOR__

#include "config.hpp"

/*
 * attach_sensor: attach the function to the pin to which the sensor
 *                is connected.
 *
 * For info, pin 21 works on Arduino Mega
 */

void attach_sensor(int pin);


/*
 * time_since_last_detection: compute the time since the last time
 *                            we detected something
 */

unsigned long time_since_last_detection();

/*
 * detect: function called when something is detected on the sensor
 */

void detect();

/*
 * get_wheel_speed: returns the wheel speed in rpm
 */

float get_wheel_speed();

/*
 * detection: variable set to true by detect()
 */
extern char detection;

#endif
