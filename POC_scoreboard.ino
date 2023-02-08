//    ****************************************************************************************************
//    ******************************PROOF OF COMCEPT, TWO PLAYER SCOREBOARD*******************************
//    ****************************************************************************************************   


//   ********************ASSIGN CONSTANT VALUES********************
const byte ledPin = 12;                // Arduino pin connected to pixel strip
const byte numDigits = 2;              // quantity of digits on each display
const byte pixelPerDigit = 18;         // all pixels, including decimal point pixels if available at each digit
const byte pixelPerPlayer = pixelPerDigit * 2; 
const byte addPixels = 0;              // other pixels
const byte startPixelA = 0;            // start pixel of display A
const byte startPixelB = 36;           // start pixel of display B 
const uint32_t ledCount(pixelPerDigit * numDigits * 2 + addPixels);


//    **********ASSIGN VARIABLE VALUES*********
byte counterA;                         // counts player A
byte counterB;                         // counts player B
int colorKey = 0;                    // Acts as a key for the color look-up table
byte program_mode = 0;                 // If 0, program mode for colors is disabled

// **********Color Values*********
int white[3] = {255,255,255};
int red[3] = {255, 0, 0};
int salmon[3] = {224, 109, 109};
int maroon[3] = {128, 0, 0};
int crimson[3] = {220,20,60};
int orange[3] = {255, 120, 0};
int brown[3] = {117, 55, 0};
int yellow[3] = {255, 255, 0};
int gold[3] = {255, 195, 0};
int yellowGreen[3] = {113, 255, 0};
int green[3] = {0, 255, 0};
int forestGreen[3] = {0, 141, 20};
int aquamarine[3] = {0, 255, 186};
int teal[3] = {0,128,128};
int cyan[3] = {0, 255, 255};
int blue[3] = {0, 0, 255};
int navyBlue[3] = {0, 0, 128};
int purple[3] = {128, 0, 128};
int indigo[3] = {75, 0, 130};
int fuschia[3] = {255, 0, 255};
int hotPink[3] = {255, 0, 128};


int* colors[21] = 
{
  white,
  red,
  salmon,
  maroon,
  crimson,
  orange,
  brown,
  yellow,
  gold,
  yellowGreen,
  green,
  forestGreen,
  aquamarine,
  teal,
  cyan,
  blue,
  navyBlue,
  purple,
  indigo,
  fuschia,
  hotPink
};

int numColors = floor(sizeof(colors)/2);

//    **********link button names to pins*********
const byte buttonUpApin = A0;            // player A increment  
const byte buttonDnApin = A1;            // player A decrement
const byte buttonUpBpin = A3;            // player B increment
const byte buttonDnBpin = A2;            // player B decrement
const byte buttonResetPin = A4;          // button to reset scores, active LOW; Will also act as 'program mode' toggle for colors


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


//    **********INCLUDES AND NEOPIXEL OBJECT DECLARATIONS**********
#include <Adafruit_NeoPixel.h>                                       
Adafruit_NeoPixel strip(ledCount, ledPin, NEO_GRB + NEO_KHZ800);
uint32_t strip_color;    

#include <Noiasca_NeopixelDisplay.h>                                      
Noiasca_NeopixelDisplay displayA(strip, segment, numDigits, pixelPerDigit, startPixelA);  
Noiasca_NeopixelDisplay displayB(strip, segment, numDigits, pixelPerDigit, startPixelB);  

#include <OneButton.h>  
#include <math.h>

//    **********Set up onebutton**********            
OneButton buttonAup(buttonUpApin, true);
OneButton buttonAdn(buttonDnApin, true);
OneButton buttonBup(buttonDnBpin, true);
OneButton buttonBdn(buttonUpBpin, true);
OneButton buttonReset(buttonResetPin, true);


//    ********************FUNCTIONS********************

// Pass the display and counter by reference - allows the changes made to continue outside the scope of the function
void clickUpColor(Noiasca_NeopixelDisplay& currDisplay, byte currCounter, int& key)
{
  if(key >= numColors) key = 0;
    
  int* currColor = colors[key];

  strip_color = strip.Color(colors[key][0],colors[key][1],colors[key][2], 0);

  currDisplay.setColorFont(strip_color);

  if (currCounter < 10) currDisplay.print(" ");
    
  currDisplay.print(currCounter);     
    
  key++;
}

void clickDownColor(Noiasca_NeopixelDisplay& currDisplay, byte currCounter, int& key)
{
  key--;
  
  if(colorKey < 0) key = numColors-1;
    
  int* currColor = colors[key];

  strip_color = strip.Color(colors[key][0],colors[key][1],colors[key][2], 0);

  currDisplay.setColorFont(strip_color);

  if (currCounter < 10) currDisplay.print(" ");
    
  currDisplay.print(currCounter);     
}

// ********************Strip A********************
void clickAup()                                     //Increments counter A
{
  if(program_mode == 0)
  {
    if (counterA == 99) return;                     //Check for overflow
    
    counterA++;                                     //Increase counter by one
    
    displayA.setCursor(0);                          //Position cursor
    
    if (counterA < 10) displayA.print(" ");         //Values under 10 get a blank space before digit
    
    displayA.print(counterA);                       //Send new value to display
  }

  else // Increment color key and apply new color (strip A)
  {
    clickUpColor(displayA, counterA, colorKey);
  }
}

void clickAdn()                                   //Decrements counter A
{
  if(program_mode == 0)
  {
    if (counterA == 0) return;                      //Check for underflow
    counterA--;                                     //Decrease counter by one
    displayA.setCursor(0);                          //Position cursor
    if (counterA < 10) displayA.print(" ");         //Values under 10 get a blank space before digit
    displayA.print(counterA);                       //Send new value to display
  }

  else // Decrement color key and apply new color (strip A)
  {
    clickDownColor(displayA, counterA, colorKey);
  }
}

// ********************Strip B********************

void clickBup()
{
  if(program_mode == 0) // If program mode disabled, increment counter (strip B)
  {
    if (counterB == 99) return;
    counterB++;
    displayB.setCursor(0);
    if (counterB < 10) displayB.print(" ");
    displayB.print(counterB);
  }

  else // Program mode enabled, increment color key and apply new color (strip B)
  {
    clickUpColor(displayB, counterB, colorKey);
  }
}

void clickBdn()
{
  if(program_mode == 0) // If program mode disabled, decrement counter
  {
    if (counterB == 0) return;
    counterB--;
    displayB.setCursor(0);
    if (counterB < 10) displayB.print(" ");
    displayB.print(counterB);
  }

  else // If program mode enabled, decrement color key and apply new color (strip B)
  {
    clickDownColor(displayB, counterB, colorKey);
  }
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

void toggle_program_mode()
{
  if(program_mode == 0)
  {
    program_mode = 1;
  }

  else program_mode = 0;
}

// ********************End Button Programming********************

void setup()
{
  Serial.begin(9600); // Mostly used for debugging purposes - allows stuff to be printed directly to the COM3 output port
  strip.begin();                        // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();                         // Turn OFF all pixels
  strip.setBrightness(100);             // Set BRIGHTNESS  (max = 255)
  strip.clear();

  displayA.setColorFont(0x888888);      // Display A Color 
  displayB.setColorFont(0x888888);      // Display B Color

  displayA.print(" 0");                 //Show a 0 on display A
  displayB.print(" 0");                 //Show a 0 on display B
  
//link button functions  
// 'Attach' functions to specific button actions (double click, long click, etc)
  buttonAup.attachClick(clickAup);      
  buttonAdn.attachClick(clickAdn);
  buttonBup.attachClick(clickBup);
  buttonBdn.attachClick(clickBdn);
  buttonReset.attachLongPressStart(resetScore);
  buttonReset.attachDoubleClick(toggle_program_mode);
  //Serial.end(); // Disable serial communication - done for redudancy sake to prevent memory leaks or unnecessary memory usage
}

void loop()
{
  // put here other code which needs to run:
  buttonReset.tick();
  buttonAup.tick();
  buttonAdn.tick();
  buttonBup.tick();
  buttonBdn.tick();
}
