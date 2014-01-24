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

//! union to save random adc pattern and easier access
union {
   uint8_t  val8bit[2];
   uint16_t val16bit;
} adcValue;


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
   uint8_t  i = 0;

   adcValue.val16bit = 0xAFFE;

   initHardware();

   while(1)
   {
      // count to 10 - two times
      SET_PIN(SEG_CAT1);
      for(i = 0; i < NUM_OF_PATTERN; ++i)
      {
         EXP_PORT(SEG_PORT) = pattern[i];
         _delay_ms(500);
      }
      RESET_PIN(SEG_CAT1);
      _delay_ms(10);

      SET_PIN(SEG_CAT2);
      for(i = 0; i < NUM_OF_PATTERN; ++i)
      {
         EXP_PORT(SEG_PORT) = pattern[i];
         _delay_ms(500);
      }
      RESET_PIN(SEG_CAT2);
      _delay_ms(10);

      // random adc pattern
      for(i = 0; i < 1000; ++i)
      {
         show7Segment(adcValue.val8bit[0] ^ adcValue.val8bit[1], adcValue.val8bit[1]);
      }

      // show adc value on 7 segment display
      for(i = 0; i < 1000; ++i)
      {
         show7Segment(pattern[adcValue.val8bit[0]],
                      pattern[adcValue.val8bit[1]]);
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
   adcValue.val16bit = adc_get();
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
 * \param left  byte
 * \param right byte
 */
void show7Segment(uint8_t left, uint8_t right)
{
   SET_PIN(SEG_CAT1);
   EXP_PORT(SEG_PORT) = left;
   _delay_ms(20);
    RESET_PIN(SEG_CAT1);
   _delay_ms(1);

   SET_PIN(SEG_CAT2);
   EXP_PORT(SEG_PORT) = right;
   _delay_ms(20);
   RESET_PIN(SEG_CAT2);
   _delay_ms(1);
}