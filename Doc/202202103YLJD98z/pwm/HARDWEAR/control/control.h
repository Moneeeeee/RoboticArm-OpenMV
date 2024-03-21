#ifndef __CONTROL_H
#define __CONTROL_H

#include "sys.h"

void control_go(u16 FLAG);
u16 limit(u16 MOTOR);
void car_go(u16 FLAG);
 u16 limit_pwm(u16 pwm);

#endif


