/*
  MONITOR
  subsystem for MR15
  Reads sensors and relays output over serial to Pi
*/

/* --- Libraries --- */
#include <SoftwareSerial.h> // needed for RFID module.
#include "stdio.h" // needed for dtostrf()
#include "DHT.h"

/* --- Pins --- */
#define SEAT_PIN A0
#define HITCH_PIN A1
#define BUTTON_PIN A2
#define GUARD_PIN A3
#define BRAKE_PIN A4
#define IGNITION_PIN A5
#define RFID_TX_PIN 2
#define RFID_RX_PIN 3
#define DHT_PIN 4

/* --- Constants --- */
#define BAUD 9600
#define DHT_TYPE 22
#define READ 0x02

/* --- Objects --- */
SoftwareSerial rfid(RFID_RX_PIN, RFID_TX_PIN);
DHT dht(DHT_PIN, DHT_TYPE);

/* --- Variables --- */
volatile int SEAT = 0;
volatile int HITCH = 0;
volatile int IGNITION = 0;
volatile int BUTTON = 0;
volatile int GUARD = 0;
volatile int BRAKE = 0;
volatile int RFID = 0;
volatile int TEMP = 0;
volatile int HUMIDITY = 0;

/* --- Buffers --- */
char SENSORS[128];

/* --- Setup --- */
void setup() {

  // Initialize On/Off Pins
  pinMode(SEAT_PIN, INPUT);
  pinMode(HITCH_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(GUARD_PIN, INPUT);
  pinMode(BRAKE_PIN, INPUT);
  pinMode(IGNITION_PIN, INPUT);
  
  // Set resistor for On/Off Pins
  digitalWrite(SEAT_PIN, LOW);
  digitalWrite(HITCH_PIN, LOW);
  digitalWrite(BUTTON_PIN, LOW);
  digitalWrite(GUARD_PIN, LOW);
  digitalWrite(BRAKE_PIN, LOW);
  digitalWrite(IGNITION_PIN, LOW);

  // Initialize DHT
  dht.begin();
  
  // Initialize Serial
  Serial.begin(BAUD);
  
  // Initialize RFID
  rfid.begin(BAUD);
}

/* --- Loop --- */
void loop() {
  
  // Read Switches
  SEAT = digitalRead(SEAT_PIN);
  HITCH = digitalRead(HITCH_PIN);
  BUTTON = digitalRead(BUTTON_PIN);
  GUARD = digitalRead(GUARD_PIN);
  BRAKE = digitalRead(BRAKE_PIN);
  IGNITION = digitalRead(IGNITION_PIN);
  
  // Read Sensors
  RFID = check_rfid();
  TEMP = check_temp();
  HUMIDITY = check_humidity();

  // Convert to string and send over serial
  sprintf(SENSORS, "{'seat':%d,'hitch':%d,'button':%d,'brake':%d,'guard':%d,'ignition':%d, 'rfid':%d, 'temp':%d, 'humidity':%d}",SEAT,HITCH,BUTTON,BRAKE,GUARD,IGNITION,RFID, TEMP, HUMIDITY);
  Serial.println(SENSORS);
  Serial.flush();
}

/* --- Methods --- */
int check_rfid() {
  rfid.write(READ);
  delay(20);
  if (rfid.read() == -1) {
    return 1;
    delay(500);
  }
  else {
    return 0;
  }
}

int check_temp() {
  return 0;
}

int check_humidity() {
  return 0;
}
