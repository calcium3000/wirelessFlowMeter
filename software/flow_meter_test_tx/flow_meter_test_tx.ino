/*
 * flow_meter_test_tx.ino 
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
#include <LowPower.h>
#include <SPI.h>
#include "RF24.h"


// Defines
#define FLOW_GND    8
#define FLOW_PIN    3
#define NRF_CE      9
#define NRF_nCS     10


// Constants
volatile unsigned int count;
unsigned int flow_count;

RF24 radio(NRF_CE, NRF_nCS);
byte address[6] = "80085";


// Setup
void setup(void)
{
  pinMode(FLOW_PIN, INPUT);

  pinMode(FLOW_GND, OUTPUT);
  digitalWrite(FLOW_GND, LOW);

  attachInterrupt(digitalPinToInterrupt(FLOW_PIN), found_rise, RISING);

  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(address);

  noInterrupts();
}


// Main Program
void loop(void)
{
  flow_count = measure_flow();
  radio.write(&flow_count, sizeof(int));

  radio.powerDown();
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  radio.powerUp();
}


// ISR
void found_rise(void)
{
  count++;
}


/* Measure flow */
unsigned int measure_flow(void)
{
  // Reset counter
  count = 0;

  // Enable flow sensor
  digitalWrite(FLOW_GND, HIGH);
  // Enable interrupts
  interrupts();
  // Delay for 133ms to determine flow in L/min
  // (f [Hz] = Q [L/min] * 7.5)
  // (1000ms / 7.5 = 133ms)
  delay(133);
  // Disable interrupts
  noInterrupts();
  // Disable flow meter
  digitalWrite(FLOW_GND, LOW);
  
  return count;
}

