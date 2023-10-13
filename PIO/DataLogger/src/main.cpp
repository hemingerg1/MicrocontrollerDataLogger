#include <Arduino.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

int numSensors = 2;
int sensorSlopes[] = {625, 625};
float sensorOffsets[] = {-312.5, -312.5};

String data = "";
char stream = 0;

void setup()
{
  Serial.begin(115200);

  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  ads.begin();
}

void loop()
{
  if (stream)
  {
    for (int i = 0; i < numSensors; i++)
    {
      int16_t adc = ads.readADC_SingleEnded(i);
      float v = (adc * 0.1875) / 1000;
      int p = (sensorSlopes[i] * v) + sensorOffsets[i];

      data += p;
      data += ',';
    };

    Serial.println(data);
    data = "";
  }
}

void serialEvent()
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
