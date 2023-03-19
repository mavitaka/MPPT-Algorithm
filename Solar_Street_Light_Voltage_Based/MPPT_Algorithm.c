/*
 * MPPT_Algorithm.c
 *
 *  Created on: Oct 24, 2015
 *      Author: OS
 */

#include "MPPT_Algorithm.h"

uint16_t Hill_Climbing(uint16_t duty_cycle, float diff)
{
  float increment;

  // calculate increment
  if (((diff<=1) && (diff>=-1)) || (diff>5) || (diff<-20))
	  increment = 0;
  else if (diff > 0)
	  increment = -diff*4;
  else if (diff < 0)
	  increment = -diff;

  float new_duty_cycle = duty_cycle + increment;

  // saturation condition
  if(new_duty_cycle >= 800)
      return 800;
  else if (new_duty_cycle <= 300)
      return 300;
  else
      return (uint16_t)new_duty_cycle;
}








