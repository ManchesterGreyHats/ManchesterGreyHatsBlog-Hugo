#include "DigiKeyboard.h"
#define KEY_CAPS_LOCK 57

void setup() {
  // don't need to set anything up to use DigiKeyboard
  // Run Once Section
  pinMode(1, OUTPUT); // LED on Model A
}

void loop() {
  digitalWrite(1,HIGH);
  DigiKeyboard.delay(1000);
  digitalWrite(1,LOW);
  // put your main code here, to run repeatedly:
  DigiKeyboard.delay(1000);
  DigiKeyboard.sendKeyStroke(0);
  DigiKeyboard.sendKeyStroke(KEY_CAPS_LOCK);
  digitalWrite(1,HIGH);
  DigiKeyboard.delay(1000);
  DigiKeyboard.sendKeyStroke(0);
  DigiKeyboard.sendKeyStroke(KEY_CAPS_LOCK);
  digitalWrite(1,LOW);

}
