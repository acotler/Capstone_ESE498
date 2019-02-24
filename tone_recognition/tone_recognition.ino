//Libraries
#include <LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>
#include <FHT.h>

//Global Variables
LiquidCrystal lcd(8, 7, 6, 5, 3, 2);
int vibMotor = 9;
int mic = A5;

int maxVals[5] = {0, 0, 0, 0, 0};
int sampleVal = 0;

//delta timing variables
unsigned long displayTime = 0;
bool disp = false;
unsigned long sampleTime = 0;
//SD card variables
File myFile;
//FSM Variables
enum State { dataAquisition, FHTProcessing, recordTone };
State state = dataAquisition;
bool toneDetected = false;
bool FHTProcDone = true;
bool shouldRecordTone = false;
bool recordingTone = false;


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

void setup() {
  Serial.begin(9600); //begin serial connection
  //Initialize I/O
  pinMode(vibMotor, OUTPUT);
  //pinMode(mic, INPUT);
  //Initilize LCD
  //lcd.begin(16, 2); //start lcd
  //lcd.clear();
  //Initilize SD Card
  //SD.begin(4);  
}

void loop() {
  state = nextState(state);
  
  switch(state){
    case dataAquisition:
      if(millis()-sampleTime >= 1){
        //sampleVal = analogRead(mic);
        
        for(int j=4; j>=0; j--){
          if(sampleVal>maxVals[j]){
            for(int k=0; k<j+1; k++){
              maxVals[k] = maxVals[k+1];
            }
            maxVals[j] = sampleVal;
            break;
          }
        }
        Serial.println(analogRead(mic));
        sampleTime = millis();
      }
      if(millis()-displayTime>500){
        Serial.print("{");
        for(int i=0; i<5; i++){
          Serial.print(maxVals[i]);
          if(i<4){
            Serial.print(", "); 
          }
        }
        Serial.println("}");
        displayTime = millis();
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




  
