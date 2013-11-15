/*
  Jess's Night Light (it's more for Sarah, actually)
  
  Idea: Use an RGB led strip
  (http://www.ladyada.net/products/rgbledstrip/), powered from a
  ULN2003 chip, to automatically light up the room with a soft
  light when a person enters the room at night.
  
  A PIR (12V, dry contact - security alarm type) will trigger the
  lights to fade-in with a soft ambient light. An off switch 
  will be used to fade the light off when exiting the room.
  
  An ambient light sensor (light dependant resistor) will monitor
  the light in the room so the night light will not operate
  during the day, or when the main lights are turned on.
  
  Limitations:
  ULM2003 is limited to 500mA per output, meaning each colour. LED
  strips are typically 115mA/Metre, but check with a multi-meter
  first.
  
  Inputs are designed for normally open contacts, but this can be
  changed in the code.
  
*/

const int RedLed = 9;           // Red LED Output Pin (Digital 9)
const int GrnLed = 11;          // Blue LED Output Pin (Digital 11)
const int BluLed = 10;          // Green LED Output Pin (Digital 10)
const int RedOut = 3;           // Red LED Strip Output Pin (Digital 9)
const int GrnOut = 5;          // Blue LED Strip Output Pin (Digital 11)
const int BluOut = 6;          // Green LED Strip Output Pin (Digital 10)
const int PIRin = 12;           // PIR Input Pin (Digital 8)
const int OFFin = 2;            // Off Button Input Pin (Digital 6)
const int LightSense = A1;      // Light Sensor Input Pin (Analog 1)

int PIRstate = 0;              // Flag for state of PIR (Is someone in the room?)
int OFFswstate = 0;            // Flag for the off button (Has someone requested the light be turned off?)

boolean LightIsOff = true;     // State of the night light (Is is currently on or off?)

// Settings

int wait = 10;                   // Colour fade step delay, smaller delay, faster fading.
int RoomBrightnessLimit = 600;  // Brightness level from 0 to 1023. Adjust according to ambient light in room when main lights are off.

// Colour settings
int LightColour[3] = { 0xF1, 0x40, 0x30 };  //F14030 = Salmon / Pink - Can be generated from any HTML colour chart
int Black[3]  = { 0, 0, 0 };    // Black, or Lights Off - Remember, Common Anode means 255 = LED Off

// Set initial color
int redVal = Black[0];
int grnVal = Black[1]; 
int bluVal = Black[2];

// Initialize color variables
int prevR = redVal;
int prevG = grnVal;
int prevB = bluVal;


void setup()
  {
  Serial.begin(9600);
  pinMode(RedLed, OUTPUT);
  digitalWrite(RedLed, HIGH);     
  pinMode(BluLed, OUTPUT);
  digitalWrite(BluLed, HIGH);
  pinMode(GrnLed, OUTPUT);
  digitalWrite(GrnLed, HIGH);
  pinMode(RedOut, OUTPUT);
  pinMode(BluOut, OUTPUT);
  pinMode(GrnOut, OUTPUT);
  pinMode(PIRin, INPUT_PULLUP);   // Not needed if pull-up resistors used (V1.3) - Use 'INPUT' instead of 'INPUT_PULLUP'
  pinMode(OFFin, INPUT_PULLUP);   // Not needed if pull-up resistors used (V1.3)
   
}
  
  
void loop()
  {
    if (LightIsOff == true) {                       // Is the light currently off? Yes!
    //Serial.println("Light is Off" ); 
    //delay(500);    
    PIRstate = digitalRead(PIRin);    // Check the PIR
    //Serial.print("PIRState is: " ); 
    //Serial.println(PIRstate);
    //delay(500); 
    int BrightnessLevel = analogRead(LightSense);   // Check the rooms ambient light
      //Serial.print("Brightness level is: " ); 
      //Serial.println(BrightnessLevel);
      //delay(500); 
      // If there is someone in the room AND the room is dark...
      if (PIRstate == HIGH && BrightnessLevel <= RoomBrightnessLimit) {
        //Serial.println("Fading Lights On Now " ); 
        //delay(500); 
        crossFade(LightColour);       // Fade lights on
        LightIsOff = false;           // The lights are now on
        delay(500);                  // Wait half a second for... umm... 
      }
      else {        // There is no-one in the room, or the room is already bright enough.
        //Serial.println("There is no-one in the room, or room not bright enough" ); 
        //delay(500); 
      delay(500);   // Wait half a second... this may need to be longer.
      }
    }
    else {                                // The light must be on
      OFFswstate = digitalRead(OFFin);    // Check the Off button
        //Serial.println("Running OFF loop - Waiting for Off button" ); 
        //delay(500); 
      if (OFFswstate == HIGH) {     // Button has been pressed - !!! Make this HIGH if using a N/C button
        //Serial.println("Off Button Pressed - Dimming Lights to black" ); 
        //delay(500); 
        crossFade(Black);          // Fade lights off
        delay(10000);               // Wait 10 seconds (enough time to clear the room)
        LightIsOff = true;         // The lights are now off
      }
      else {    // Button hasn't been pressed. Is this ELSE necessary??
      }
    }
}


/* Below is all the fade functions from *** Insert reference here****
/ It's some hardcore maths. This could be optimised if the colour chosen was a 
/ known variable, as you could then manually calculate the steps... but this 
/ makes it easier to test and choose a colour.
*/

int calculateStep(int prevValue, int endValue) {
  int step = endValue - prevValue; // What's the overall gap?
  if (step) {                      // If its non-zero, 
    step = 1020/step;              //   divide by 1020
  } 
  return step;
}


int calculateVal(int step, int val, int i) {

  if ((step) && i % step == 0) { // If step is non-zero and its time to change a value,
    if (step > 0) {              //   increment the value if step is positive...
      val += 1;           
    } 
    else if (step < 0) {         //   ...or decrement it if step is negative
      val -= 1;
    } 
  }
  // Defensive driving: make sure val stays in the range 0-255
  if (val > 255) {
    val = 255;
  } 
  else if (val < 0) {
    val = 0;
  }
  return val;
}


void crossFade(int color[3]) {
  // Convert to 0-255
  //int R = (color[0] * 255) / 100;
  //int G = (color[1] * 255) / 100;
  //int B = (color[2] * 255) / 100;

  int stepR = calculateStep(prevR, color[0]);
  int stepG = calculateStep(prevG, color[1]); 
  int stepB = calculateStep(prevB, color[2]);

  for (int i = 0; i <= 1020; i++) {
    redVal = calculateVal(stepR, redVal, i);
    grnVal = calculateVal(stepG, grnVal, i);
    bluVal = calculateVal(stepB, bluVal, i);

    
    analogWrite(RedOut, redVal);   // Write current values to Output pins
    analogWrite(GrnOut, grnVal);      
    analogWrite(BluOut, bluVal);
    redVal = ~redVal;              // Invert the values for the Common Anode RGB LED
    grnVal = ~grnVal;
    bluVal = ~bluVal;
    analogWrite(RedLed, redVal);   // Write current values to LED pins
    analogWrite(GrnLed, grnVal);      
    analogWrite(BluLed, bluVal);
    redVal = ~redVal;              // Invert them back to normal before exiting loop
    grnVal = ~grnVal;
    bluVal = ~bluVal;
    //Serial.print("Red: " );
    //Serial.print(redVal);
    //Serial.print(" - Green: " );
    //Serial.print(grnVal);
    //Serial.print(" - Blue: " );
    //Serial.println(bluVal);

    if (redVal >= 1 && grnVal == 0 && bluVal == 0)
      {
        //Serial.println("Red Too High - Speed Fade!");
      }
      else if (redVal == 0 && grnVal >= 1 && bluVal == 0)
      {
        //Serial.println("Green Too High - Speed Fade!");
      }
      else if (redVal == 0 && grnVal == 0 && bluVal >= 1)
      {
        //Serial.println("Blue Too High - Speed Fade!");
      }
      else
      {
      delay(wait); // Pause for 'wait' milliseconds before resuming the loop
      }
  }
  // Update current values for next loop
  prevR = redVal; 
  prevG = grnVal; 
  prevB = bluVal;
}

