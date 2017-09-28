/*
 * flow_meter_test_rx.ino 
 * 
 * Copyright 2017  Calvin A. Cherry (calvincherry@gmail.com)
 *                 calcium3000.wordpress.com
 * 
 * RF24 library courtesy of TMRh20.
 * http://tmrh20.github.io/RF24
 * 
 * LowPower library courtesy of RocketScream.
 * https://github.com/rocketscream/Low-Powerdfx
 * 
 * Use:
 *   This sofware is currently in development.  Further documentation is
 *   on the horizon.
 *   
 *   
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */
 
 // Includes
#include <SPI.h>
#include "RF24.h"


// Defines
#define NRF_CE      7
#define NRF_nCS     8


// Constants
RF24 radio(NRF_CE, NRF_nCS);
byte addresses[2][6] = {"80085","58008"};


// Setup
void setup()
{
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(1, addresses[0]);
  radio.startListening();

  Serial.begin(9600);
  Serial.println("Begin:");
}


// Main Program
void loop()
{
  byte received;
  
  if(radio.available())
  {
    while(radio.available())
    {
      radio.read(&received, sizeof(byte));
      Serial.println(received);
    }  
  }
}
