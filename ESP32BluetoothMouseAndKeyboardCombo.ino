#include "icons.h"
#include "menu.h"
#include "joystickAndButtonInput.h"


// Pin definitions - change these to match your wiring
#define TFT_CS                  5
#define TFT_CS2                 25
#define TFT_DC                  2
#define TFT_RST                 4
#define colorReadPin          	14
#define JOYSTICK_Y              27
#define JOYSTICK_X              26
#define LEFT_MOUSE_CLICK        34
#define RIGHT_MOUSE_CLICK       35
#define PIXELS_PER_MOVEMENT     10      // max pixels per update at full push
#define JOYSTICK_DEADBAND       200    
#define JOYSTICK_HALF_RANGE     1900   // 12-bit ADC, distance from center to edge

bool isMouseOptionSelected = false, firstTimeMovement = true;
int JOYSTICK_X_DEADZONE = 1950, JOYSTICK_Y_DEADZONE = 2020, cursorX = 0, cursorY = 0, previousCursorX = 1, previousCursorY = 1, xComponentMovement = 0, yComponentMovement = 0;

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, -1);
ESP32BLECombo interface;

void calibrateJoystick(){
  long sx = 0, sy = 0;
  for(int i=0; i<200; i++){
    sx += analogRead(JOYSTICK_X);
    sy += analogRead(JOYSTICK_Y);
    delay(2);
  }
  JOYSTICK_X_DEADZONE = sx / 200;
  JOYSTICK_Y_DEADZONE = sy / 200;
}

void setup() {
  Serial.begin(115200);
  ESP32BLEComboConfig device;
  device.mode = ESP32BLEComboMode::KEYBOARD_MOUSE;
  device.deviceName = "Sokol Universal Interface Hub";
  device.manufacturer = "Sokol Electronics";
  device.batteryLevel = 100;
  device.appearance = ESP32BLEComboAppearance::AUTO;
  pinMode(JOYSTICK_Y, INPUT);
  pinMode(JOYSTICK_X, INPUT);
  pinMode(LEFT_MOUSE_CLICK, INPUT);
  pinMode(RIGHT_MOUSE_CLICK, INPUT);
  pinMode(TFT_RST, OUTPUT);
  digitalWrite(TFT_RST, HIGH); delay(10);
  digitalWrite(TFT_RST, LOW);  delay(20);
  digitalWrite(TFT_RST, HIGH); delay(150);

  tft.init(240, 320);
  tft.setSPISpeed(40000000);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
	tft.drawBitmap(0,0,epd_bitmap_seLOGO,320,240,ST77XX_WHITE);
  calibrateJoystick();
  delay(2000);
  tft.fillScreen(ST77XX_BLACK);
  interface.begin(device);
}

void loop() {
  if(previousCursorX != cursorX || previousCursorY !=cursorY){
    if(isMouseOptionSelected == false)
      displayMenuItems(tft);
    previousCursorX = cursorX;
    previousCursorY = cursorY;
  }
  menuNavigationControls();
  Serial.print(cursorX);
  Serial.print(" - ");
  Serial.println(cursorY);
}

int readAveraged(int pin){
  long sum = 0;
  for(int i=0; i<16; i++) sum += analogRead(pin);
  return sum / 16;
}

// returns -1.0 .. +1.0, and 0 inside the deadband
float axisToNorm(int raw, int center){
  int offset = raw - center;
  if(abs(offset) <= JOYSTICK_DEADBAND) return 0;
  offset -= (offset > 0) ? JOYSTICK_DEADBAND : -JOYSTICK_DEADBAND;
  return constrain((float)offset / (JOYSTICK_HALF_RANGE - JOYSTICK_DEADBAND), -1.0f, 1.0f);
}

void universalJoystickMovement(double maxMoveX, double maxMoveY){
  xComponentMovement = axisToNorm(readAveraged(JOYSTICK_X), JOYSTICK_X_DEADZONE) * maxMoveX;
  yComponentMovement = -1 * axisToNorm(readAveraged(JOYSTICK_Y), JOYSTICK_Y_DEADZONE) * maxMoveY;
}

void checkWhichInputOptionWasSelected(int a){
  if(digitalRead(LEFT_MOUSE_CLICK) == HIGH){
    if(a==1)
      isMouseOptionSelected = true;
    else
      isMouseOptionSelected = false;
  }
}

void moveCursor(int x, int y, int maximumXValue, int maximumYValue){
  cursorX = constrain(cursorX + x, 0.0, maximumXValue);
  cursorY = constrain(cursorY + y, 0.0, maximumYValue);
}

void drawCursor(){
  tft.fillCircle(cursorX, cursorY, 2, ST77XX_RED);
}

void menuNavigationControls(){
  universalJoystickMovement(PIXELS_PER_MOVEMENT, PIXELS_PER_MOVEMENT * 0.75);
  checkWhichInputOptionWasSelected(selctedMenuOption);
  if(isMouseOptionSelected==true){
    moveCursor(xComponentMovement,yComponentMovement,1920,1080);
    if(firstTimeMovement){
      delay(1500);
      for(int i = 0; i < 80; i++){
        interface.mouseMove(-127, -127);
        delay(8);                            
      }
      cursorX = 0;
      cursorY = 0;
      firstTimeMovement = false;
      tft.fillScreen(ST77XX_BLACK);
    }
    mouseOption(tft, digitalRead(LEFT_MOUSE_CLICK), digitalRead(RIGHT_MOUSE_CLICK));
    mouseControl(cursorX,cursorY,digitalRead(LEFT_MOUSE_CLICK),digitalRead(RIGHT_MOUSE_CLICK),false,interface);
  }
  else{
    moveCursor(xComponentMovement,yComponentMovement,320,240);
    checkWhichItemIsCursorHoveringOver(cursorX, cursorY, tft);
    drawCursor();
    firstTimeMovement = true;
  }
  xComponentMovement = 0;
  yComponentMovement = 0;
}
