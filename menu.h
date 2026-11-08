#ifndef MENU_H
#define MENU_H
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <Arduino.h>
#include "icons.h"

#define TEXT_SIZE           2
#define buttonPin           34
int selctedMenuOption = 0;
bool doesScreenNeedToReset = true, hasPictureChanged = true, resetMouseScreen = false;
unsigned long timer1 = 0, timer2 = 0;

typedef struct{
  const char* name;
  const unsigned char *icon;
}Menu;

Menu items[] = {
    {"Keyboard",NULL},
    {"Mouse",NULL},
};
const char menuItemCount = sizeof(items) / sizeof(Menu);

void displayMenuItems(Adafruit_ST7789 &display){
  display.fillScreen(ST77XX_BLACK);
  display.setTextSize(TEXT_SIZE);
  for(int i=0; i<menuItemCount; i++){
    display.setCursor(0,i*16 + 2);
    display.print(items[i].name);
  }
}

void mouseOption(Adafruit_ST7789 &display, bool lefMousePress, bool rightMousePress){
  if(doesScreenNeedToReset){
    display.fillScreen(ST77XX_BLACK);
    doesScreenNeedToReset = false;
  }
  if(lefMousePress || rightMousePress){
    timer1 = millis();
    if(timer1-timer2>=50){
      timer2 = timer1;
      hasPictureChanged = false;
    }
  }
  else if(lefMousePress == false && rightMousePress == false && resetMouseScreen == false){
    display.fillScreen(ST77XX_BLACK);
    display.drawBitmap(0,0,epd_bitmap_esp32MOUSE,320,240,ST77XX_WHITE);
    resetMouseScreen = true;
  }
  if(hasPictureChanged == false && resetMouseScreen == true){
    display.fillScreen(ST77XX_BLACK);
    if(lefMousePress)
      display.drawBitmap(0,0,epd_bitmap_esp32MOUSELeftClick,320,240,ST77XX_WHITE);
    else if(rightMousePress)
      display.drawBitmap(0,0,epd_bitmap_esp32MOUSERightClick,320,240,ST77XX_WHITE);
    hasPictureChanged = true;
    resetMouseScreen = false;
  }
}

void keyboardOption(){

}

void checkWhichItemIsCursorHoveringOver(int &cursorX, int &cursorY, Adafruit_ST7789 &display){
  for(int i=0; i<menuItemCount; i++){
    if(cursorY<=(i+1)*16 + 2){
      display.drawRect(0, i*16 + 2, 320, 16, ST77XX_WHITE);
      selctedMenuOption = i;
      break;
    }
  }
}
#endif