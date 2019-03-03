//Libraries
#include <LiquidCrystal.h>
#include <SPI.h>
//#include <SD.h>
#include <arduinoFFT.h>
#include <math.h>

//Global Variables
LiquidCrystal lcd(8, 7, 6, 5, 3, 2);
int vibMotor = 9;
int mic = A5;

//SD card variables
//File myFile;
//FSM Variables
enum State { dataAquisition, FHTProcessing, recordTone };
State state = dataAquisition;

bool toneDetected = false;
bool FHTProcDone = true;
bool shouldRecordTone = false;
bool recordingTone = false;

//FFT Sampling Variables
const uint16_t samples = 128;
const double samplingFrequency = 8000;

double maxVals[2] = {0,0};
double lastX = 0.0;

double vReal[samples];
double vImag[samples];

arduinoFFT FFT = arduinoFFT(vReal, vImag, samples, samplingFrequency); /* Create FFT object */

int i = 0;


//A function to determine state transitions
State nextState(State currentState){
  switch(currentState){
    case dataAquisition:
      FHTProcDone = false; //reset FHTProcDone
      recordingTone = false; //reset recordingTone
      if(toneDetected) {
        state = FHTProcessing;
      }
      else {
        state = dataAquisition;
      }

      if(shouldRecordTone){
        state = recordTone;
      }
      break;
    case FHTProcessing:
      if(!FHTProcDone) {
        state = FHTProcessing;
      }
      else {
        state = dataAquisition;
        FHTProcDone = true;
        toneDetected = false;
      }
      break;
    case recordTone:
      if(recordingTone) {
        state = recordTone;
      }
      else {
        state = dataAquisition;
        shouldRecordTone = false;
      }
      break;
  }
}

double distance(double x1, double x2){
  return abs(x2-x1);
}


void setup() {
  Serial.begin(115200); //begin serial connection
  //Initialize I/O
  pinMode(vibMotor, OUTPUT);
  //Initilize LCD
  lcd.begin(16, 2); //start lcd
  lcd.clear();
  //Initilize SD Card
  //SD.begin(4);  
}

void loop() {
  state = nextState(state);
  
  switch(state){
    case dataAquisition:
      if(i<128){
        vReal[i] = analogRead(A5);
        vImag[i] = 0.0;
        i++;
        delayMicroseconds(130);
      }
      else {
        FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Weigh data */
        FFT.Compute(vReal, vImag, samples, FFT_FORWARD); /* Compute FFT */
        FFT.ComplexToMagnitude(vReal, vImag, samples); /* Compute magnitudes */
        FFT.DCRemoval(vReal, samples);

        for(int j=0; j<128; j++){
          Serial.print(vReal[j]);
          if(j!=127){
            Serial.print(",");
          }
        }
        Serial.println();
        
        double x = FFT.MajorPeak(vReal, samples, samplingFrequency) * 0.50;  //0.616;
      
        if(distance(x, lastX)>1.0){
          for(int j=1; j>=0; j--){
            if(x>maxVals[j]){ //discourages multiple
              for(int k=0; k<j+1; k++){
                maxVals[k] = maxVals[k+1];
              }
              maxVals[j] = x;
              break;
            }
          }
        }
        lastX = x;
        //Serial.print(maxVals[0]);
        //Serial.print(", ");
       // Serial.println(maxVals[1]);
        i=0;
      }
      

      
      //toneDetected = true;
      break;
    case FHTProcessing: //Take FHT and compare to files
      break;       
    case recordTone: //Record new tones to a file
      break;
  }
}

/*
  //Write to a file
  myFile = SD.open("test.csv", FILE_WRITE);
  myFile.print(0);
  myFile.println(",");
  delay(5000);
  */
  /*if (myFile) {
    myFile.println("This is a test...");
    myFile.close();
  } 
  else {
    Serial.println("error opening file");
  }

  //Read from a file
  myFile = SD.open("test.txt");
  if (myFile) {
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    myFile.close();
  } 
  else {
    Serial.println("error opening file");
  }*/
  
  /*
  if(millis()-displayTime >= 1000){
    lcd.clear();
    if(disp){
      lcd.setCursor(0, 0);
      lcd.print("Tone Detected:");
      lcd.setCursor(0, 1);
      lcd.print("Doorbell");
    }
    else {
      lcd.setCursor(0, 0);
      lcd.print("Rrriiiiinggg");
      lcd.setCursor(0, 1);
      lcd.print("Rrriiiiinggg");
    }
    disp = !disp;
    displayTime = millis();
  }
*/




  
