#include <avr/wdt.h>
#include <avr/sleep.h>

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  wdsetup();
}

void clign(int n) {
  for(int i=0; i<n; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);
    digitalWrite(LED_BUILTIN, LOW);
    delay(300);
  }
}

void loop() {
  clign(1);
  wdt_reset();

  // mode Power-down
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  // mode Power-save
//  set_sleep_mode(SLEEP_MODE_PWR_SAVE);
  // mode standby
//  set_sleep_mode(SLEEP_MODE_STANDBY);
  // mode Extended Standby
//  set_sleep_mode(SLEEP_MODE_EXT_STANDBY);
  // mode IDLE
//  set_sleep_mode(SLEEP_MODE_IDLE);
  // mode ADC Noise Reduction
//  set_sleep_mode(SLEEP_MODE_ADC);
  // Désactivation BOD (brown-out detection)
//  sleep_bod_disable();

  // fais dodo !
  sleep_mode();

  clign(3);
  delay(1500);
}

// Configuration WDT
void wdsetup(void) {
  cli();
  wdt_reset();
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  // 8s (max) + interruption + pas de reset
  WDTCSR = (1<<WDIE)|(0<<WDE)|(1<<WDP3)|(1<<WDP2)|(1<<WDP1)|(1<<WDP0);
  sei();
}

ISR(WDT_vect){
  // faire des trucs
}
