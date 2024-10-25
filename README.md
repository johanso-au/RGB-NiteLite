RGB LED Strip Motion Activated Night Light
==================================================

Pictures: http://imgur.com/a/TQshe
Video: http://youtu.be/ay8awluwBV4
  
  -----------------------
  
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
  
  The PCB includes on-board buttons and a RGB led for testing 
  without additional hardware.
  
  Limitations:
  ULM2003 is limited to 500mA per output, meaning each colour. LED
  strips are typically 115mA/Metre, but check with a multi-meter
  first.
  
  Inputs are designed for normally open contacts, but this can be
  changed in the code.
  
  The RGB Fade in-out code is by Clay Shirky <clay.shirky@nyu.edu>
  via http://arduino.cc/en/Tutorial/ColorCrossfader
  
  The rest of the code is released here under the Creative Commons 
  CC BY-NC-SA License http://creativecommons.org/licenses/by-nc-sa/3.0/au/
