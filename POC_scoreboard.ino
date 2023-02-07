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
byte color_key = 0;                    // Acts as a key for the color look-up table
byte program_mode = 0;                 // If 0, program mode for colors is disabled
byte num_colors = 14;                  // Total number of colors

class define_colors
{
  public: 
    uint32_t colors[14];
  
    define_colors() // Constructor for define_colors object
    {
      colors[0] = 0xFF0000; // Red
      colors[1] = 0x7D0D0D; // Maroon
      colors[2] = 0xF49F9F; // Pink
      colors[3] = 0xFF6900; // Orange
      colors[4] = 0x7E3509; // Brown
      colors[5] = 0xFFE800; // Yellow
      colors[6] = 0x75FF00; // Lime Green
      colors[7] = 0x346C05; // Forest Green
      colors[8] = 0x02FFA8; // Teal
      colors[9] = 0x02D7FF; // Baby Blue
      colors[10] = 0x0259FF; // Blue
      colors[11] = 0x2C3295; // Navy blue
      colors[12] = 0x902FF; // Purple
      colors[13] = 0xFF02E0; // Fuschia
    }

    ~define_colors() // Destrutor (not needed since no dynamic memory is allocated)
    {
      return; 
    }
};

define_colors colorObj = define_colors();

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

  else if (program_mode == 1) // Increment color key and apply new color (strip A)
  {
    color_key += 1;

    if(color_key >= num_colors) color_key = 0;

    displayA.setColorFont(colorObj.colors[color_key]);
    displayA.show();
    strip.show();
    
    Serial.println(color_key);
    Serial.println(program_mode);
    Serial.print("\n");

    return;
 
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
    return;
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

  else if (program_mode == 1) // Program mode enabled, increment color key and apply new color (strip B)
  {
    return;
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
    return;
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
  Serial.begin(9600);
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
