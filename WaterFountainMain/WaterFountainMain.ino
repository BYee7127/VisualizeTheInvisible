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
  AudioMemory(13);
#if defined(__IMXRT1062__)
  set_arm_clock(600000000);
#endif
  setupLED();
  amp1.gain(35.0);
}

void loop() {
  if (fft256.available())  //verify fft has output data
  {
    averageFFT();
    for (int i = 0; i < 12; i++)  //for every fft bin set the pump
    {
      // Serial.print((double)FFTarray[i], 3);  //print fft bin for testing
      // Serial.print(" ");
      setPump(i, FFTarray[i]);
    }
    // Serial.println();  //new line
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
  // fullColorWipe(60);
  cycleAllPatterns();

  // pumpsOff();
  // pumpsHigh();
}

/*
 * Sets given pump based on value from FFT
 */
void setPump(int pump, float val) {
  float newVal;
  if (pump == 0)  //pump12
  {
    if (FFTarray[pump] < 0.10)  //lower threshold
    {
      analogWrite(pump, 0);
    } else {
      newVal = (val)*70 + 125;
      analogWrite(pump, newVal);
    }
  }

  if (pump == 1)  //pump11
  {
    if (FFTarray[pump] < 0.10) {
      analogWrite(pump, 0);
    } else {
      newVal = (val * 2) * 55 + 175;
      analogWrite(pump, newVal);
    }
  }

  if (pump == 2)  //pump10
  {
    if (FFTarray[pump] < 0.10) {
      analogWrite(pump, 0);
    } else {
      newVal = (val * 2) * 55 + 190;
      analogWrite(pump, newVal);
    }
  }

  if (pump == 3)  //pump9
  {
    if (FFTarray[pump] < 0.10) {
      analogWrite(pump, 0);
    } else {
      newVal = (val * 3) * 70 + 175;
      analogWrite(pump, newVal);
    }
  }

  if (pump == 4)  //pump8
  {
    if (FFTarray[pump] < 0.10) {
      analogWrite(pump, 0);
    } else {
      newVal = (val * 2) * 65 + 160;
      analogWrite(pump, newVal);
    }
  }

  if (pump == 5)  //pump7
  {
    if (FFTarray[pump] < 0.10) {
      analogWrite(pump, 0);
    } else {
      newVal = (val * 4) * 40 + 205;
      analogWrite(pump, newVal);
    }
  }

  if (pump == 6)  //pump6
  {
    if (FFTarray[pump] < 0.08) {
      analogWrite(pump, 0);
    } else {
      newVal = (val * 6) * 60 + 180;
      analogWrite(pump, newVal);
    }
  }

  if (pump == 7)  //pump5
  {
    if (FFTarray[pump] < 0.08) {
      analogWrite(pump, 0);
    } else {
      newVal = (val * 8) * 65 + 180;
      analogWrite(pump, newVal);
    }
  }

  if (pump == 8)  //pump4
  {
    if (FFTarray[pump] < 0.07) {
      analogWrite(pump, 0);
    } else {
      newVal = (val * 12) * 50 + 185;
      analogWrite(pump, newVal);
    }
  }

  if (pump == 9)  //pump3
  {
    if (FFTarray[pump] < 0.10) {
      analogWrite(pump, 0);
    } else {
      newVal = (val * 2) * 70 + 170;
      analogWrite(pump, newVal);
    }
  }

  if (pump == 10)  //pump2
  {
    if (FFTarray[pump] < 0.06) {
      analogWrite(pump, 0);
    } else {
      newVal = (val * 12) * 55 + 200;
      analogWrite(pump, newVal);
    }
  }

  if (pump == 11)  //pump1
  {
    if (FFTarray[pump] < 0.04) {
      analogWrite(pump, 0);
    } else {
      newVal = (val * 20) * 90 + 130;
      analogWrite(pump, newVal);
    }
  }
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
 * for testing pumps
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
  analogWrite(9, 225);   // pump 03
  analogWrite(10, 255);  // pump 02
  analogWrite(11, 220);  // pump 01
}

/*
 * Averages the first 36 out of 128 bins in the FFT to 12 and places the values into FFTarrary[] for the pumps
 * to access.
 */
void averageFFT() {
  for (int i = 0; i < 12; i++) {
    if (i == 11)  // (4 Khz - 6 khz | presence range of frequencies)
    {
      FFTarray[i] = fft256.read(23, 35) / 12;
    } 
    else if (i == 10) //(2 Khz - 3.8 khz | upper mid range frequencies)
    {
      FFTarray[i] = fft256.read(10, 22) / 12;
    }
    else  //all other bins from 0-10 are not averaged (344 hz - 1.9 Khz | low to mid range frequencies)
    {
      FFTarray[i] = fft256.read((i - 1) + 1);
    }
  }
}