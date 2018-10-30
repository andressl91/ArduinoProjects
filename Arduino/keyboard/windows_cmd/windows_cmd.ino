#include <Keyboard.h>


void setup() {
  Keyboard.begin();
  delay(1000);
  Keyboard.press(KEY_LEFT_GUI);
  delay(600);
  Keyboard.press('r');
  delay(600);
  Keyboard.releaseAll();
  delay(600);
  Keyboard.write("cmd");
  
}

void loop() {
  
}
