#include "DigiKeyboard.h"

void setup() {
  // put your setup code here, to run once:
  DigiKeyboard.delay(5000);
  DigiKeyboard.update();

  DigiKeyboard.sendKeyStroke(KEY_R, MOD_GUI_LEFT); // meta+r
  DigiKeyboard.delay(100);

  DigiKeyboard.println("firefox -fs http://fakeupdate.net/windows98/index.html");
}

void loop() {
  // put your main code here, to run repeatedly:

}
