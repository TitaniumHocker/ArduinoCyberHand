#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 0
#define DELTAY 2
double count = 0;

#define LOGO16_GLCD_HEIGHT 32
#define LOGO16_GLCD_WIDTH 128

void setup()
{
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000);
  display.clearDisplay();
}

void loop()
{
  String vString =  String(count, 3);// using a float and the 
  display.clearDisplay();
  robojaxText("Voltage:          ", 4, 3, 1, false);
  robojaxText(vString, 72, 3, 1, false);
  robojaxText("V", 110, 3, 1, false);
  robojaxText("Temprature: 32C", 4, 11, 1, false);
  robojaxText("Capacity:   92.86L", 4, 21, 1, false);
  //display.drawLine(1, 37, 100, 37, WHITE);
  display.drawRect(1, 1, 126,31, WHITE);
  //display.drawCircle(63,31, 31, WHITE);
  //display.startscrollright(0x00, 0x0F);
  display.display();
  count +=0.173;
  delay(2000); 
}

void robojaxText(String text, int x, int y,int size, boolean d) 
{
  display.setTextSize(size);
  display.setTextColor(WHITE);
  display.setCursor(x,y);
  display.println(text);
  if(d){
    display.display();
  }
  
  delay(100);
}
