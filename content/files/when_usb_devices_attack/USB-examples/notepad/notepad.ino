#include "DigiKeyboard.h"

void setup() {
  // put your setup code here, to run once:
  DigiKeyboard.delay(5000);
  DigiKeyboard.update();
}

void loop() {
  // put your main code here, to run repeatedly:
  DigiKeyboard.delay(10000);
  DigiKeyboard.update();
  DigiKeyboard.delay(100);

  // meta+r, delete content, start notepad
  DigiKeyboard.sendKeyStroke(KEY_R, MOD_GUI_LEFT); // meta+r
  DigiKeyboard.delay(100);
  DigiKeyboard.sendKeyStroke(76); // Clean it up
  DigiKeyboard.delay(50);
  DigiKeyboard.println("notepad");
  DigiKeyboard.delay(200);
  DigiKeyboard.println("Hello Greyhats!");

}
