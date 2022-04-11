#include <math.h>
#define NOTE_D4 294 //buzzer note

const int xPin = A0;
const int yPin = A1;
const int zPin = A2;
const int buzledPin = 13;
const int buttonPin = 12;
const int gSelectPin = 8;

const float acc_volt = 3.3; //accelerometer volt supply
const float sensitivity = 0.206; //accelerometer sensitivity

//Take multiple samples to reduce false positives
const int sampleSize = 20;
const int AveragingSize = 10;

void setup()
{
  Serial.begin(9600);
  pinMode(buzledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(gSelectPin, OUTPUT);
  digitalWrite(gSelectPin, HIGH);
  analogReference(EXTERNAL);
  Serial.begin(9600);
}

void loop()
{
  int xAccel, yAccel, zAccel;
  float sum = 0;
  for (int j = 0; j < AveragingSize; j++)
  {
    int xRaw = 0;
    int yRaw = 0;
    int zRaw = 0;
    int arr[3] = { xPin, yPin, zPin};
    ReadAxis(arr, &xRaw, &yRaw, &zRaw);

    // Converting raw accelerometer output to gF
    float xActual = (xRaw-
    511.5)*acc_volt/(sensitivity*1023);

    float yActual = (yRaw-
    511.5)*acc_volt/(sensitivity*1023);

    float zActual = (zRaw-
    511.5)*acc_volt/(sensitivity*1023);

    xAccel = round(xActual);
    yAccel = round(yActual);
    zAccel = round(zActual);

    float TotalGf = sqrt(xAccel*xAccel + yAccel*yAccel +
    zAccel*zAccel);
    sum += TotalGf;
    delay(20);
  }
  
  float avgTotalGf = sum/AveragingSize;
  // Triggers the buzzer/LED if gF crosses the threshold.
  if (avgTotalGf >= 2)
  {
    digitalWrite(buzledPin, HIGH);
    tone(buzledPin, NOTE_D4);
  }

  // Resets the buzzer/LED when button is pressed.
  if (digitalRead(buttonPin) == HIGH)
  {
    digitalWrite(buzledPin, LOW);
    noTone(buzledPin);
  }
  
  Serial.print("X, Y, Z :: ");
  Serial.print(xAccel);
  Serial.print("G, ");
  Serial.print(yAccel);
  Serial.print("G, ");
  Serial.print(zAccel);
  Serial.print("G :: ");
  Serial.print("Total: ");
  Serial.print(avgTotalGf);
  Serial.println("G ");
}

void ReadAxis(int arr[], int* xRaw, int* yRaw, int* zRaw)
{
  long xReading = 0;
  long yReading = 0;
  long zReading = 0;
  for (int i = 0; i < sampleSize; i++)
  {
    xReading += analogRead(arr[0]);
    yReading += analogRead(arr[1]);
    zReading += analogRead(arr[2]);
  }
  *xRaw = xReading/sampleSize;
  *yRaw = yReading/sampleSize;
  *zRaw = zReading/sampleSize;
}
