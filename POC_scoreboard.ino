
//    ****************************************************************************************************
//    ******************************PROOF OF COMCEPT, TWO PLAYER SCOREBOARD*******************************
//    ****************************************************************************************************   


//   ********************ASSIGN CONSTANT VALUES********************
const byte ledPin = 12;                // Arduino pin connected to pixel strip
const byte numDigits = 2;              // quantity of digits on each display
const byte pixelPerDigit = 18;         // all pixels, including decimal point pixels if available at each digit
const byte addPixels = 0;              // other pixels
const byte startPixelA = 0;            // start pixel of display A
const byte startPixelB = 36;           // start pixel of display B 
const uint32_t ledCount(pixelPerDigit * numDigits * 2 + addPixels);


//    **********ASSIGN VARIABLE VALUES*********
byte counterA;                         // counts player A
byte counterB;                         // counts player B


//    **********link button names to pins*********
const byte buttonUpApin = A0;            // player A increment  
const byte buttonDnApin = A1;            // player A decrement
const byte buttonUpBpin = A3;            // player B increment
const byte buttonDnBpin = A2;            // player B decrement
const byte buttonResetPin = A4;          // button to reset scores, active LOW


//    **********Define pixel to segment map**********  
typedef uint32_t segsize_t;                              
const segsize_t segment[8] {
  0b00000000000000000000000000000011,  // SEG_A
  0b00000000000000000000000000011100,  // SEG_B
  0b00000000000000000000000011100000,  // SEG_C
  0b00000000000000000000001100000000,  // SEG_D
  0b00000000000000000001110000000000,  // SEG_E
  0b00000000000000001110000000000000,  // SEG_F
  0b00000000000000110000000000000000,  // SEG_G
  0b00000000000000000000000000000000   // SEG_DP
};


//    **********INCLUDES**********
#include <Adafruit_NeoPixel.h>                                       
Adafruit_NeoPixel strip(ledCount, ledPin, NEO_GRB + NEO_KHZ800);     

#include <Noiasca_NeopixelDisplay.h>                                      
Noiasca_NeopixelDisplay displayA(strip, segment, numDigits, pixelPerDigit, startPixelA);  
Noiasca_NeopixelDisplay displayB(strip, segment, numDigits, pixelPerDigit, startPixelB);  

#include <OneButton.h>  


//    **********Set up onebutton**********            
OneButton buttonAup(buttonUpApin, true);
OneButton buttonAdn(buttonDnApin, true);
OneButton buttonBup(buttonDnBpin, true);
OneButton buttonBdn(buttonUpBpin, true);
OneButton buttonReset(buttonResetPin, true);


//    ********************FUNCTIONS********************
void clickAup()                                   //Increments counter A
{
  if (counterA == 99) return;                     //Check for overflow
  counterA++;                                     //Increase counter by one
  displayA.setCursor(0);                          //Position cursor
  if (counterA < 10) displayA.print(" ");         //Values under 10 get a blank space before digit
  displayA.print(counterA);                       //Send new value to display
}

void clickAdn()                                   //Decrements counter A
{
  if (counterA == 0) return;                      //Check for underflow
  counterA--;                                     //Decrease counter by one
  displayA.setCursor(0);                          //Position cursor
  if (counterA < 10) displayA.print(" ");         //Values under 10 get a blank space before digit
  displayA.print(counterA);                       //Send new value to display
}

void clickBup()
{
  if (counterB == 99) return;
  counterB++;
  displayB.setCursor(0);
  if (counterB < 10) displayB.print(" ");
  displayB.print(counterB);
}

void clickBdn()
{
  if (counterB == 0) return;
  counterB--;
  displayB.setCursor(0);
  if (counterB < 10) displayB.print(" ");
  displayB.print(counterB);
}

void resetScore()
{
  counterA = 0;
  counterB = 0;
  displayA.clear();
  displayA.print(" 0");
  displayB.clear();
  displayB.print(" 0");
}

void setup()
{
  strip.begin();                        // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();                         // Turn OFF all pixels
  strip.setBrightness(100);             // Set BRIGHTNESS  (max = 255)
  strip.clear();

  displayA.setColorFont(0x888888);      // Display A Color 
  displayB.setColorFont(0x888888);      // Display B Color

  displayA.print(" 0");                 //Show a 0 on display A
  displayB.print(" 0");                 //Show a 0 on display B
  
//link button functions  
  buttonAup.attachClick(clickAup);      
  buttonAdn.attachClick(clickAdn);
  buttonBup.attachClick(clickBup);
  buttonBdn.attachClick(clickBdn);
  buttonReset.attachLongPressStart(resetScore);
}

void loop()
{
  // put here other code which needs to run:
  buttonAup.tick();
  buttonAdn.tick();
  buttonBup.tick();
  buttonBdn.tick();
  buttonReset.tick();
}
