#include <Arduino.h>
#include <Bounce2.h>
#include <Encoder.h> 

Encoder enc(26, 25); //create Encoder Object
Bounce bank_button = Bounce(); // Create button object

const byte number_of_banks = 4; // Constants for banks and encoders
const byte number_of_encs = 1;

int current_value[16]; // Arrays for previous and current values
int previous_value[16];

byte current_bank = 0; // Variable for current bank

int buttonPin = 2;

void setup() 
{
  pinMode(13, OUTPUT); // Set LED as output

  pinMode(buttonPin, INPUT_PULLUP); // Set button pin as input

  bank_button.attach(buttonPin); // Attach digital pin to bounce object
  bank_button.interval(5); // Set interval of bounce object
}

void loop() 
{
  bank_button.update();

  
if (bank_button.fell() == true) 
{ // If button is pressed, then update the bank selection
    current_bank = current_bank + 1;
    if (current_bank >= number_of_banks) 
    {
      current_bank = 0;
    }

  for (int i = 0; i < (current_bank + 1); i ++)
  { // blink LED for Bank Number
    digitalWrite(13, HIGH);
    delay(50);
    digitalWrite(13, LOW);
    delay(50);
  }

   enc.write(previous_value[(current_bank * number_of_encs) + 0]); // Retrieve current values and write to encs
}
 
current_value[(number_of_encs * current_bank) + 0] = enc.read(); // Read encs into current values
 
  if (current_value[(number_of_encs * current_bank) + 0] < 0) { // Constrain all four encs to 0 - 127... 
    current_value[(number_of_encs * current_bank) + 0] = 0;
    enc.write(0);
  }
  else if (current_value[(number_of_encs * current_bank) + 0] > 127) {
    current_value[(number_of_encs * current_bank + 0)] = 127;
    enc.write(127);
  }

for (int i = 0; i < number_of_encs; i ++) // Compare previous to current readings, and send MIDI data if changed, take into consideration different banks
{ 
    if (current_value[(number_of_encs * current_bank) + i] != previous_value[(number_of_encs * current_bank) + i] || bank_button.fell() == true) 
    {
      previous_value[(number_of_encs * current_bank) + i] = current_value[(number_of_encs * current_bank) + i];
      usbMIDI.sendControlChange(1 + (number_of_encs * current_bank) + i, current_value[(number_of_encs * current_bank) + i], 1);
    }
}
}

