#include <WS2812B.h>

#include "./Inc/menu.h"

#include "./Inc/renderer.h"
#include "./Inc/adc.h"
#include "./Inc/AI.h"
#include "./Inc/rtc.h"
#include "./Inc/ws2812.h"
#include "./Inc/led.h"
#include "./Inc/keyBoard.h"
#include "./Inc/adc.h"


void setup() 
{
  Serial.begin(115200);
  menu_init();
  Serial.println("Init finished!\r\n");
}


void loop() 
{ 
  menu_task(NULL);
}
