

#ifndef TIMER_INCLUDED
#define TIMER_INCLUDED

#include "alloween.h"
#include <time.h>


void	t_TimerProcess();

double	FloatTime();
void	IniTimer();
void	GetRealTime(char *buf, double d_time);
void	DrawTimer();
void	UpdateTimers();
hBool	SlowWorldTime(float msec);



#endif // TIMER_INCLUDED
