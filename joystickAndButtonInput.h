#ifndef JOYSTICKANDBUTTONINPUT_H
#define JOYSTICKANDBUTTONINPUT_H

#include <Arduino.h>
#include "icons.h"
#include "ESP32BLECombo.h"

int previousX = 0, previousY = 0;
unsigned long leftClickTimer = 0, rightClickTimer = 0, lastMilisLeft = 0, lastMilisRight = 0;

void mouseControl(int xValue, int yValue, bool leftMouseClick, bool rightMouseClick, bool middleMouseClick, ESP32BLECombo &mouse){
  mouse.mouseMove((xValue - previousX) % 127,(yValue - previousY) % 127);
  previousX = xValue;
  previousY = yValue;
  if(leftMouseClick == true){
    mouse.mousePress(1);
  }
  else{
    leftClickTimer = millis();
    if(leftClickTimer - lastMilisLeft >= 50){
      mouse.mouseRelease(1);
      lastMilisLeft = leftClickTimer;
    }
  }
  if(rightMouseClick == true)
    mouse.mousePress(2);
  else{
    rightClickTimer = millis();
    if(rightClickTimer -  lastMilisRight>= 50){
      mouse.mouseRelease(2);
      lastMilisRight = rightClickTimer;
    }
  }
}

void keyboardControl(ESP32BLECombo &keyboard){
  //press(keyboard.keyboardKey);
}
#endif