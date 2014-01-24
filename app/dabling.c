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

#include "util/util.h"
#include "adc/adc.h"
#include "timer/timer.h"
#include "dabling.h"

#include <util/delay.h>
#include <avr/interrupt.h>

//! number of pattern in the pattern array
#define NUM_OF_PATTERN 16

//! pattern array for the 7 segment display
uint8_t pattern[NUM_OF_PATTERN] = { SEG_NUM_0,
                                    SEG_NUM_1,
                                    SEG_NUM_2,
                                    SEG_NUM_3,
                                    SEG_NUM_4,
                                    SEG_NUM_5,
                                    SEG_NUM_6,
                                    SEG_NUM_7,
                                    SEG_NUM_8,
                                    SEG_NUM_9,
                                    SEG_NUM_A,
                                    SEG_NUM_B,
                                    SEG_NUM_C,
                                    SEG_NUM_D,
                                    SEG_NUM_E,
                                    SEG_NUM_F };

//! value of the adc pin
uint16_t adcVal = 0xAFFE;

// === MAIN LOOP =============================================================

/**
 * \brief main loop
 **/
#ifdef __DOXYGEN__
int main(void)
#else
int __attribute__((OS_main)) main(void)
#endif
{
   uint16_t i = 0;
   uint16_t j = 0;

   initHardware();

   while(1)
   {
      // count to 10 - two times
      for(i = 0; i < NUM_OF_PATTERN; ++i)
      {
         // j * wait time in show7Segment() = time to display pattern
         for(j = 0; j < 23; ++j)
         {
            show7Segment(pattern[i], ~(pattern[i]));
         }
      }
      _delay_ms(10);

      for(i = 0; i < NUM_OF_PATTERN; ++i)
      {
         // j * wait time in show7Segment() = time to display pattern
         for(j = 0; j < 23; ++j)
         {
            show7Segment(~(pattern[i]), pattern[i]);
         }
      }
      _delay_ms(10);

      // random adc pattern
      for(i = 0; i < 1500; ++i)
      {
         show7Segment(adcVal & 0xFF,
                      adcVal >> 8);
      }
   }
}

// === ISR ===================================================================

/**
 * \brief timer 0 overflow interrupt service routine
 * Every overflow, the adc value of the unconnected port pin is read
 * to get a random value.
 */
ISR(TIMER0_OVF_vect)
{
   adcVal = adc_get();
   TOGGLE_PIN(BLUE_PIN);
}

// === HELPERS ===============================================================

/**
 * \brief initialization for hardware
 * The port and pins for the 7 segment displays are setup as
 * well as timer(s) and adc.
 */
void initHardware(void)
{
   // block select (common cathode of 7 segment blocks)
   PIN_SET_OUTPUT(SEG_CAT1);
   RESET_PIN(SEG_CAT1);

   PIN_SET_OUTPUT(SEG_CAT2);
   RESET_PIN(SEG_CAT2);

   // init port for segments (all outputs) and set to 0
   EXP_DDR(SEG_PORT) = 0xFF;
   EXP_PORT(SEG_PORT) = 0;

   // init port pin of blue LED
   PIN_SET_OUTPUT(BLUE_PIN);
   RESET_PIN(BLUE_PIN);

   // approx. 262ms @ 1MHz
   initTimer0(/* TimerOverflow */);
   adc_init();

   // enable all (configured) interrupts
   sei();

   adc_enable();
   startTimer0();
}

/**
 * \brief show values on left and right 7 segment display
 * \param left  byte and left display side
 * \param right byte and right display side
 */
void show7Segment(uint8_t left, uint8_t right)
{
   SET_PIN(SEG_CAT1);
   EXP_PORT(SEG_PORT) = right;
   _delay_ms(10);
    RESET_PIN(SEG_CAT1);
   _delay_ms(1);

   SET_PIN(SEG_CAT2);
   EXP_PORT(SEG_PORT) = left;
   _delay_ms(10);
   RESET_PIN(SEG_CAT2);
   _delay_ms(1);
}
