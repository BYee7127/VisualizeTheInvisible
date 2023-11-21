/**************************************************************************************
 *
 * File:      WaterFountainMain.ino
 * Author:    Alex Gray & Beverly Yee
 * Date:      November 08, 2023
 * 
 * Calls to the different parts of the Water Fountain are made here. 
 * To add a sketch to be a part of the project, make sure the sketch is saved in the
 * Sketches folder of WaterFountainMain. Go to Sketch -> Add File..., and navigate to 
 * the sketch to add.
 *
 * DO NOT call setup() or loop() in any of the other sketches. If it is necessary to
 * do some setup, call it setup[object]() where [object] is the thing to setup.
 *        i.e: setupLEDStrip() or setupFFT()
 * 
 **************************************************************************************/
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

AudioInputAnalog adcL(A2);
AudioInputAnalog adcR(A3);
AudioMixer4 mixer1;
AudioAmplifier amp1;       
AudioRecordQueue queue1;
AudioAnalyzeFFT256 fft256;
AudioConnection patchCord1(adcL, 0, mixer1, 0);
AudioConnection patchCord2(adcR, 0, mixer1, 1);
AudioConnection patchCord3(mixer1, amp1);
AudioConnection patchCord4(amp1, queue1);
AudioConnection patchCord5(amp1, fft256);

static float FFTarray[12];

#if defined(__IMXRT1062__)
extern "C" uint32_t set_arm_clock(uint32_t frequency);
#endif

void setup() {
  Serial.begin(9600);
  AudioMemory(12);
#if defined(__IMXRT1062__)
  set_arm_clock(600000000);
  Serial.print("F_CPU_ACTUAL=");
  Serial.println(F_CPU_ACTUAL);
#endif
  setupLED();
  amp1.gain(100.0);
}

void loop() {
  if (fft256.available())  //verify fft has output data
  {
    averageFFT();
    for (int i = 0; i < 12; i++)  //test fft results (12 numbers, each should be a float between 0 and 1)
    {
      Serial.print((double)FFTarray[i],3);
      Serial.print(" ");
    }
    Serial.println();  //new line
  }

  // lineUpTest();
  // cycleAllColors(1000);
  // breatheStatic(3, 5);
  // showRandomColor(1000);
  // incrementFillToBlackStatic(2, 60);
  // incrementFillToBlackRandom(60);
  // incrementFillRandom(60);
  // singleColorWipeStatic(5, 60);
  // doubleColorWipe(1,3,60);
  //fullColorWipe(60);

  pumpsOff();
  // pumpsHigh();

}

/*
 * Turns off all the pumps. Essential if we want to test things without the pumps running.
 */
void pumpsOff() {
  analogWrite(0, 0);   // pump 12
  analogWrite(1, 0);   // pump 11
  analogWrite(2, 0);   // pump 10
  analogWrite(3, 0);   // pump 09
  analogWrite(4, 0);   // pump 08
  analogWrite(5, 0);   // pump 07
  analogWrite(6, 0);   // pump 06
  analogWrite(7, 0);   // pump 05
  analogWrite(8, 0);   // pump 04
  analogWrite(9, 0);   // pump 03
  analogWrite(10, 0);  // pump 02
  analogWrite(11, 0);  // pump 01
}

/*
 * Sets the max values of all pumps to the pump with the lowest height on the max value.
 * 
 */
void pumpsHigh() {
  analogWrite(0, 220);   // pump 12
  analogWrite(1, 255);   // pump 11
  analogWrite(2, 245);   // pump 10
  analogWrite(3, 245);   // pump 09
  analogWrite(4, 225);   // pump 08
  analogWrite(5, 255);   // pump 07
  analogWrite(6, 220);   // pump 06
  analogWrite(7, 245);   // pump 05
  analogWrite(8, 235);   // pump 04
  analogWrite(9, 255);   // pump 03
  analogWrite(10, 255);  // pump 02
  analogWrite(11, 220);  // pump 01
}

/*
 * Averages the 128 bins in the FFT to 12 and places the values into FFTarrary[] for the pumps
 * to access. This may not be ideal since all bins will be used?
 */
void averageFFT()  //averages the 128 bins of fft down to 12 and places the values in FFTarray[]
{
  for (int i = 0; i < 12; i++) {
    if (i == 0)  //first bin averages over 14
    {
      FFTarray[i] = fft256.read(0,13) / 14;
    } 
    else if (i == 11)  //last bin averages over 14
    {
      FFTarray[i] = fft256.read(114,127)/14;
    } 
    else  //all other bins average over 10
    {
      FFTarray[i] = fft256.read((1+((i-1)*10)+13),(10+((i-1)*10)+13))/10;
    }
  }
}