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
 * \file leds_config.h
 *
 * \date Created: 28.11.2011 18:18:23
 * \author Matthias Kleemann
 **/


#ifndef LEDS_CONFIG_H_
#define LEDS_CONFIG_H_

/**
 * \enum eLED
 * \brief define LEDs connected to your controller
 *
 * Name them as you wish, but keep in mind that it matches the
 * port/pin definitions below.
 */
typedef enum
{
   //! error on CAN1
   flashBlue1  = 0,
   //! error on CAN2
   flashBlue2  = 1,
   //! maximum number of LEDs
   NUM_OF_LEDS = 2         // always the last one
} eLED;

/**
 * \def P_LEDS
 * \brief defines for global array to access port pins directly via port address
 *
 * Note: The definitions here correspond to the LED names in eLED. They also
 *       correspond to the type definition of ledType in leds.h.
 */
#define P_LEDS    SET_PORT_PTR(B,1), \
                  SET_PORT_PTR(B,2)

#endif /* LEDS_CONFIG_H_ */
