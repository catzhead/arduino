#ifndef __LEDPANEL__
#define __LEDPANEL__

namespace LedPanel {

void init();

void print_at_pos(unsigned pos, unsigned int value);

void print(float value);

}

#endif