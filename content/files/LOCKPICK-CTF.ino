#define GREET1 "Hello"
#define GREET2 "OWASP CTF"
#define FLAG1 "flag{s1lly_l3ft_0v3r_fl4g}"
#define FLAG2 "flag2"

#define leftswitch 7 // Pin D4
#define rightswitch 9 // Pin D6


// Simple I2C test for 128x32 oled.
// Use smaller faster AvrI2c class in place of Wire.
// Edit AVRI2C_FASTMODE in SSD1306Ascii.h to change the default I2C frequency.
//
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

// Define proper RST_PIN if required.
#define RST_PIN -1

SSD1306AsciiAvrI2c oled;
//------------------------------------------------------------------------------
void setup() {

// Setup the inputs
pinMode(leftswitch, INPUT);
digitalWrite(leftswitch, HIGH);
pinMode(rightswitch, INPUT);
digitalWrite(rightswitch, HIGH);

#if RST_PIN >= 0
  oled.begin(&Adafruit128x32, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
  oled.begin(&Adafruit128x32, I2C_ADDRESS);
#endif // RST_PIN >= 0
  // Call oled.setI2cClock(frequency) to change from the default frequency.

  oled.setFont(Adafruit5x7);

  uint32_t m = micros();

  oled.clear();

  // first row
//  oled.println("set1X test");

  // second row
  oled.set2X();
  oled.println(GREET1);

  // third row
//  oled.set1X();
//  oled.print("micros: ");
//  oled.print(micros() - m);

//  delay(2000);
//  oled.clear();
  // second row
  oled.set2X();
  oled.println(GREET2);

  delay(2000);
  oled.clear();


}
//------------------------------------------------------------------------------
void loop() {

  if ((digitalRead(leftswitch)) == LOW ) {
    // second row
    oled.set2X();
    oled.println(GREET2);
    oled.set2X();
    oled.println(FLAG1);
    delay(5000);
    oled.clear();
     
  }

  if ((digitalRead(rightswitch)) == LOW ) {
    // second row
    oled.set2X();
    oled.println(GREET2);
    oled.set2X();
    oled.println(FLAG2);
    delay(5000);
    oled.clear();
    
  }

  delay(10);
}
