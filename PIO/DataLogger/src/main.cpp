#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;
#define I2C_SDA 40 // define SCA pin number
#define I2C_SCL 38 // define SCL pin number
// Connect ADDR pin to GND for address of 0x48
TwoWire I2C = TwoWire(0); // create a I2C bus called 'I2C'

int numSensors = 2;
int sensorSlopes[] = {625, 625};
float sensorOffsets[] = {-312.5, -312.5};

String data = "";
int dataSum;
char stream = 0;
void serialRead();

void setup()
{
  Serial.begin(115200);
  I2C.begin(I2C_SDA, I2C_SCL);

  // Can set gain here not wanting the default setting
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV

  ads.begin(0x48, &I2C);

  /*
  if (!ads.begin(0x48, &I2C))
  {
    Serial.println("Failed to initialize ADS.");
    while (1)
      ;
  }
  */

  pinMode(15, OUTPUT);
}

void loop()
{
  if (stream)
  {
    digitalWrite(15, HIGH); // turn LED on
    dataSum = 0;
    for (int i = 0; i < numSensors; i++)
    {
      // int16_t adc = ads.readADC_SingleEnded(i);
      // float v = (adc * 0.1875) / 1000;
      int p = 3 + i; //(sensorSlopes[i] * v) + sensorOffsets[i];

      data += p;
      data += ',';
      dataSum += p;
    };

    data += dataSum;
    data += ',';
    Serial.println(data);
    data = "";
  }
  else
  {
    digitalWrite(15, LOW); // turn LED off
  }

  delay(1);

  if (Serial.available())
  {
    serialRead();
  }
}

void serialRead()
{
  while (Serial.available())
  {
    char inChar = Serial.read();
    if (inChar == '?')
    {
      Serial.print("#!#");
      Serial.print(numSensors);
      Serial.println('#');
      stream = 0;
    }
    if (inChar == 'A')
    {
      stream = 1;
    }
    if (inChar == 'S')
    {
      stream = 0;
    }
  }
}