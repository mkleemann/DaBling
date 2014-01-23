/**
 * ----------------------------------------------------------------------------
 *
 * "THE ANY BEVERAGE-WARE LICENSE" (Revision 42 - based on beer-ware license):
 * <dev@layer128.net> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a be(ve)er(age) in return. (I don't
 * like beer much.)
 *
 * Matthias Kleemann
 *
 * ----------------------------------------------------------------------------
 *
 * \file dabling.c
 *
 * \date Created: 21.01.2014 22:55:56
 * \author Matthias Kleemann
 **/

#include "dabling.h"
#include "util/util.h"
#include "timer/timer.h"
#include "adc/adc.h"

#include <util/delay.h>
#include <avr/interrupt.h>

#define NUM_OF_PATTERN 10

uint8_t pattern[NUM_OF_PATTERN] = { SEG_NUM_0,
                                    SEG_NUM_1,
                                    SEG_NUM_2,
                                    SEG_NUM_3,
                                    SEG_NUM_4,
                                    SEG_NUM_5,
                                    SEG_NUM_6,
                                    SEG_NUM_7,
                                    SEG_NUM_8,
                                    SEG_NUM_9 };

// === MAIN LOOP =============================================================

/**
 * \brief main loop
 **/
int __attribute__((OS_main)) main(void)
{
   int i = 0;

   initPorts();

   // approx. 262ms @ 1MHz
   initTimer0(/* TimerOverflow */);

   adc_init();

   while(1)
   {
      SET_PIN(SEG_CAT1);
      for(i = 0; i < NUM_OF_PATTERN; ++i)
      {
         EXP_PORT(SEG_PORT) = pattern[i];
         _delay_ms(500);
      }
      RESET_PIN(SEG_CAT1);
      _delay_ms(100);

      SET_PIN(SEG_CAT2);
      for(i = 0; i < NUM_OF_PATTERN; ++i)
      {
         EXP_PORT(SEG_PORT) = pattern[i];
         _delay_ms(500);
      }
      RESET_PIN(SEG_CAT2);
      _delay_ms(100);

   }
}

// === ISR ===================================================================

/**
 * \brief timer 0 overflow interrupt service routine
 */
ISR(TIMER0_OVF_vect)
{
}

// === HELPERS ===============================================================

/**
 * \brief port initialization for leds
 */
void initPorts(void)
{
   // block select (common cathode of 7 segment blocks)
   PIN_SET_OUTPUT(SEG_CAT1);
   RESET_PIN(SEG_CAT1);

   PIN_SET_OUTPUT(SEG_CAT2);
   RESET_PIN(SEG_CAT2);

   // init port for segments (all outputs) and set to 0
   EXP_DDR(SEG_PORT) = 0xFF;
   EXP_PORT(SEG_PORT) = 0;
}

