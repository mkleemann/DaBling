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
#include "leds/leds.h"
#include "timer/timer.h"
#include "dabling.h"

#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/cpufunc.h>

//! number of pattern in the segmentPattern array
#define NUM_OF_SEGMENT_PATTERN 16

//! pattern array for the 7 segment display
uint8_t segmentPattern[NUM_OF_SEGMENT_PATTERN] =
   { SEG_NUM_0,
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

//! trigger value of flasher
uint16_t flashTrigger = 1;

//! trigger matrix pattern
uint16_t matrixTrigger = 0;

//! how far to count the trigger until the pattern changes
#define WAIT_FOR_NEXT_PATTERN_STEP 45

//! pattern type for matrix
typedef struct { //! row pattern
                 uint8_t rows;
                 //! column patter
                 uint8_t cols;
               } matrix_t;

//! max. number of matrix pattern
#define NUM_OF_MATRIX_PATTERN 4

//! number of single pattern steps in array
#define NUM_OF_MATRIX_PATTERN_STEPS 40

//! define steps per different pattern corresponding with matrixPattern array
uint8_t stepsPerPattern[NUM_OF_MATRIX_PATTERN] = { 4, 12, 8, 16 };

//! define offset per different pattern corresponding with matrixPattern array
uint8_t offsetPerPattern[NUM_OF_MATRIX_PATTERN] = { 0, 4, 16, 24 };

//! simple pattern for matrix
matrix_t matrixPattern[NUM_OF_MATRIX_PATTERN_STEPS][MATRIX_MAX_ROW] =
   { { { 1, 2 }, { 2, 2 }, { 4, 2 } },    // mill
     { { 1, 1 }, { 2, 2 }, { 4, 4 } },
     { { 2, 1 }, { 2, 2 }, { 2, 4 } },
     { { 1, 4 }, { 2, 2 }, { 4, 1 } },
     //-------------------------------    next pattern
     { { 1, 1 }, { 0, 0 }, { 0, 0 } },    // from upper corner (l) to lower corner (r)
     { { 1, 2 }, { 2, 2 }, { 2, 1 } },
     { { 1, 4 }, { 2, 4 }, { 4, 7 } },
     { { 2, 4 }, { 4, 2 }, { 4, 4 } },
     { { 0, 0 }, { 0, 0 }, { 4, 4 } },
     { { 0, 0 }, { 0, 0 }, { 0, 0 } },
     { { 0, 0 }, { 1, 4 }, { 0, 0 } },    // from upper corner (r) to lower corner (l)
     { { 1, 2 }, { 2, 2 }, { 2, 4 } },
     { { 1, 1 }, { 2, 1 }, { 4, 7 } },
     { { 2, 1 }, { 4, 1 }, { 4, 2 } },
     { { 0, 0 }, { 4, 1 }, { 0, 0 } },
     { { 0, 0 }, { 0, 0 }, { 0, 0 } },
     //-------------------------------    next pattern
     { { 1, 7 }, { 0, 0 }, { 0, 0 } },    // wandering bar (rows)
     { { 2, 7 }, { 0, 0 }, { 0, 0 } },
     { { 4, 7 }, { 0, 0 }, { 0, 0 } },
     { { 0, 0 }, { 0, 0 }, { 0, 0 } },
     { { 1, 1 }, { 2, 1 }, { 4, 1 } },    // wandering bar (cols)
     { { 1, 2 }, { 2, 2 }, { 4, 2 } },
     { { 1, 4 }, { 2, 4 }, { 4, 4 } },
     { { 0, 0 }, { 0, 0 }, { 0, 0 } },
     //-------------------------------    next pattern
     { { 1, 2 }, { 0, 0 }, { 0, 0 } },    // wandering point at border
     { { 1, 4 }, { 0, 0 }, { 0, 0 } },
     { { 2, 4 }, { 0, 0 }, { 0, 0 } },
     { { 4, 4 }, { 0, 0 }, { 0, 0 } },
     { { 4, 2 }, { 0, 0 }, { 0, 0 } },
     { { 4, 1 }, { 0, 0 }, { 0, 0 } },
     { { 2, 1 }, { 0, 0 }, { 0, 0 } },
     { { 1, 1 }, { 0, 0 }, { 0, 0 } },
     { { 1, 2 }, { 0, 0 }, { 0, 0 } },    // wandering point as 8
     { { 1, 4 }, { 0, 0 }, { 0, 0 } },
     { { 2, 4 }, { 0, 0 }, { 0, 0 } },
     { { 2, 2 }, { 0, 0 }, { 0, 0 } },
     { { 2, 1 }, { 0, 0 }, { 0, 0 } },
     { { 4, 1 }, { 0, 0 }, { 0, 0 } },
     { { 4, 2 }, { 0, 0 }, { 0, 0 } },
     { { 2, 2 }, { 0, 0 }, { 0, 0 } }
   };

//! sets pattern active to show
uint8_t matrixPatternInProgress = 0;

//! wait cycles (in main loop) for taking the AVR to sleep
#define WAIT_CYCLES_FOR_SLEEP 4

//! time to be in sleep mode: approximately in ms
#define SLEEPING_TIME 30000

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
   int8_t wait4Sleep = 0;

   initHardware();

   while(1)
   {
      // count to 10 - two times
      for(i = 0; i < NUM_OF_SEGMENT_PATTERN; ++i)
      {
         // j * wait time in show7Segment() = time to display pattern
         for(j = 0; j < 23; ++j)
         {
            show7Segment(segmentPattern[i], ~(segmentPattern[i]));
         }
      }
      nextMatrixPattern();
      _delay_ms(10);

      for(i = 0; i < NUM_OF_SEGMENT_PATTERN; ++i)
      {
         // j * wait time in show7Segment() = time to display pattern
         for(j = 0; j < 23; ++j)
         {
            show7Segment(~(segmentPattern[i]), segmentPattern[i]);
         }
      }
      nextMatrixPattern();
      _delay_ms(10);

      // random adc pattern
      for(i = 0; i < 2000; ++i)
      {
         show7Segment(adcVal & 0xFF,
                      adcVal >> 8);
      }
      nextMatrixPattern();

      // sleep now..or wait another cycle
      ++wait4Sleep;
      if(WAIT_CYCLES_FOR_SLEEP == wait4Sleep)
      {
         sleepNow();
         wait4Sleep = 0;
      }
   }
}

// === ISR ===================================================================

/**
 * \brief timer0 overflow interrupt service routine
 * Every overflow, the adc value of the unconnected port pin is read
 * to get a random value.
 */
ISR(TIMER0_OVF_vect)
{
   adcVal = adc_get();
}

/**
 * \brief timer1 compare interrupt service routine
 */
ISR(TIMER1_CAPT_vect)
{
   // (trigger/wait for pattern) counts up to select the pattern
   // and its modulo keeps it in range of the array. The row selection
   // within the pattern is the simple modulo of the trigger with the
   // number of rows.
   uint8_t patternStepActive =
      ((matrixTrigger / WAIT_FOR_NEXT_PATTERN_STEP) % stepsPerPattern[matrixPatternInProgress]) +
      offsetPerPattern[matrixPatternInProgress];

   showMatrixPattern(patternStepActive, (matrixTrigger % MATRIX_MAX_ROW));
   ++matrixTrigger;
}

/**
 * \brief timer2 compare interrupt service routine
 */
ISR(TIMER2_COMP_vect)
{
   flashTrigger <<= 1;

   flashLed(flashBlue1, 0x0333);
   flashLed(flashBlue2, 0x0249);
   // reset trigger
   if(flashTrigger & 0x8000)
   {
      flashTrigger = 1;
   }
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

   // set 3x3 LED matrix rows and columns as output
   EXP_DDR(MATRIX_ROW_PORT) |= MATRIX_ROW_MASK;
   EXP_PORT(MATRIX_ROW_PORT) &= ~MATRIX_ROW_MASK;
   EXP_DDR(MATRIX_COL_PORT) |= MATRIX_COL_MASK;
   EXP_PORT(MATRIX_COL_PORT) &= ~MATRIX_COL_MASK;

   // approx. 262ms @ 1MHz
   initTimer0(/* TimerOverflow */);
   // approx. 5ms @ 1MHz
   initTimer1(TimerCompare);
   // approx. 25ms @ 1MHz
   initTimer2(TimerCompare);

   adc_init();
   led_init();

   // enable all (configured) interrupts
   sei();

   adc_enable();
   startTimer0();
   startTimer1();
   startTimer2();
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

/**
 * \brief routine to flash leds using a trigger pattern
 * \param led definition in leds_config.h
 * \param trigger pattern
 */
void flashLed(eLED led, uint16_t trigger)
{
   // only flash if pattern is met
   if(flashTrigger & trigger)
   {
      led_on(led);
   }
   else
   {
      led_off(led);
   }
}

/**
 * \brief show pattern in 3x3 matrix
 * The pattern is defined by the rows and columns selected.
 * \param rows to be selected
 * \param columns to be selected
 */
void showMatrix(uint8_t rows, uint8_t columns)
{
   EXP_PORT(MATRIX_ROW_PORT) |= (rows << MATRIX_ROW_SHIFT);
   EXP_PORT(MATRIX_COL_PORT) |= (columns << MATRIX_COL_SHIFT);
}

/**
 * \brief switch 3x3 matrix off
 */
void hideMatrix(void)
{
   EXP_PORT(MATRIX_ROW_PORT) &= ~MATRIX_ROW_MASK;
   EXP_PORT(MATRIX_COL_PORT) &= ~MATRIX_COL_MASK;
}

/**
 * \brief show the pattern defined multiplexing the rows
 * \param pattern (array index) to be shown
 * \param select row within pattern
 */
void showMatrixPattern(uint8_t pattern, uint8_t select)
{
   // remove old pattern first
   hideMatrix();
   _delay_us(100);
   // show new pattern
   showMatrix(matrixPattern[pattern][select].rows,
              matrixPattern[pattern][select].cols);
}

/**
 * \brief change matrix pattern to next
 */
void nextMatrixPattern(void)
{
      // change to next matrix pattern
      ++matrixPatternInProgress;
      if(matrixPatternInProgress == NUM_OF_MATRIX_PATTERN)
      {
         matrixPatternInProgress = 0;
      }
}

/**
 * \brief set AVR to sleep
 *
 * In this special case, only the idle mode is possible. We don't use an external
 * clock, so we need the internal clock source for timer 2 wakeup call. External
 * interrupts are also blocked.
 *
 * \code
 * SM2 SM1 SM0 Sleep Mode
 *   0   0   0 Idle
 *   0   0   1 ADC Noise Reduction
 *   0   1   0 Power-down
 *   0   1   1 Power-save
 *   1   1   0 Standby
 * \endcode
 *
 * AVR enters sleep mode and also wakes up in this state, so some intial
 * steps to set wakeup interrupt need to be done here.
 *
 * The three \c _NOP(); instructions are a safety, since older AVRs may
 * skip the next couple of instructions after sleep mode.
 */
void sleepNow(void)
{
   cli();

   stopTimer0();
   stopTimer2();
   adc_disable();
   led_all_off();
   hideMatrix();

   // set wakeup call
   setTimer1Count(SLEEPING_TIME);

   // let's sleep...
   set_sleep_mode(SLEEP_MODE_IDLE);
   // sleep_mode() has a possible race condition in it, so splitting it
   sleep_enable();
   sei();
   sleep_cpu();
   sleep_disable();

   // just in case...
   _NOP();
   _NOP();
   _NOP();

   // ...and restart
   adc_enable();
   startTimer0();
   startTimer2();
}
