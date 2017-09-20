#ifndef PIN_CHANGE_INTERRUPT_H_
#define PIN_CHANGE_INTERRUPT_H_

/**
 * This pin is labeled "2" on the NakedMedga.
 */
#define ISR_PIN 2

/**
 * Use this variable in a main loop to detect when the button is pressed. Set it back
 * to false after detecting it as true.
 */
extern volatile bool ISR_PIN_STATE;

/**
 * Configure the interrupt and associated GPIO. Don't do anything to global 
 * interrupts - that's the caller's responsibility.
 */
void setupPinChangeInterrupt(bool rising);


/**
 * Disable the interrupt, again not touching global interrupts.
 */
void disablePinChangeInterrupt();

#endif
