#include <FastLED.h>
#include <Wire.h>  // This is the Ard/RIOduino's I2C library.  

#define LED_PIN 5 //DIO pin used for control of LEDs
#define NUM_LEDS 60 //number of LEDs in LED strip
int LEDmode = 0;
int lastMode = 0;
int x = 0;
uint8_t gHue = 0;
boolean MVRTSetup;

CRGB leds[NUM_LEDS];

void setup()  //Arduino Init loop.
{
  delay(3000);
  Serial.begin(9600); //So we can watch serial monitor.  Use lots of print statments when debugging.
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  Wire.begin(8);  //Start I2C object, assigned to port #8. 
}

void loop()
{
  //Wire.onReceive(dataReceived); //Looks for data send over I2C, begins when any is detected, no matter the size. 
  RunLEDs(LEDmode); //If data is received, this function passes that information to the RunLEDs method.
}

void dataReceived(int howMany)   //Method started by data on the bus.  
{
  String LED = "";  //Create string object LED
  while (Wire.available())  //Continue through whole length of message
  {
    char n = (char)Wire.read(); //Convert the received bytes back into characters.
    if (((int)n) > ((int)(' '))) //Check that incoming characters are actual characters and not data commands.
    {
      //Serial.print(n); //Debug line for viewing incoming data
      LED += n;  // Add new character to previous character, thus creating the new string.
    }

  } 
  
  //Bad practice if/else chain.  Will be replaced by a switch case soon. 
  if (LED == "DISABLED") //The string that is passed.  This is written in the Java code. 
  {
    LEDmode = 1;  //Assign a number to the LEDmode.  This is passed to the Run LEDs function after this method ends. 
  } 
  else if (LED == "DRIVING") 
  {
    LEDmode = 2;
  } 
  else if (LED == "OTHER") 
  {
    LEDmode = 3;
  } 
  else 
  {
    LEDmode = 0;  //Diagnostic else statment.  If no I2C data is being passed, or if you are reading it incorrectly, this is useful.
  }
}

void RunLEDs(int mode) //This is passed the LEDmode object from the previous method.  
{
  //Once again, bad practice, this will be replaced by another switch case. 
  if (mode == 1) //If this mode is received, this function will happen.   
  {
    colorWipe(CRGB(255, 0, 0)); // Red for disabled 
    lastMode = 1;
  } 
  else if (mode == 2) 
  {
    
    if (!MVRTSetup) MVRT(); 
    else rotate(); // MVRT for enabled
    lastMode = 2;
  } 
  else if (mode == 3) 
  {
    rainbow(); // Rainbow for other
    lastMode = 3;
  }
}


//Standard color functions from the StrandTest example in neopixel library. doubleTheaterChase is a modification of my own.   
void colorWipe(CRGB color) 
{
  for(int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = color;
  }
  FastLED.show();
  delay(10);
  lastMode = 1;
}

//MVRT color reel
void MVRT()
{
  boolean color = true;
  for(int i = 0; i < NUM_LEDS; i+=4)
  {
    for(int j = 0; j < 4; j++)
    {
      if(color) leds[i + j] = CRGB(128,0,128); //suposed to be purple;
      else leds[i + j] = CRGB(255,200,0); //supposed to be gold;
      
    }
    color = !color;
  }
  FastLED.show();
  delay(10);
  MVRTSetup = true;
  lastMode = 2;
}

void rotate()
{
  CRGB temp = leds[0];
  for(int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = leds[i + 1];
  }

  leds[NUM_LEDS - 1] = temp;

  FastLED.show();
  FastLED.delay(100);
  lastMode = 2;
}

void rainbow() {
  fill_rainbow(leds, NUM_LEDS, gHue, 7);
  FastLED.show();
  FastLED.delay(5);
  gHue++;
  lastMode = 3;
}
