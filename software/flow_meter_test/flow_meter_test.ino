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

