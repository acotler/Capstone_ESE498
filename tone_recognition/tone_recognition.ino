//Libraries
#include <LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>
#include <FHT.h>

//Global Variables
LiquidCrystal lcd(12, 11, 6, 5, 3, 2);
int vibMotor = 13;
int mic = A0;
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
bool recordTone = false;
bool recordingTone = false;

//A function to determine state transitions
State nextState(State currentState){
  switch(currentState){
    case dataAquisiton:
      FHTProcDone = false; //reset FHTProcDone
      recordingTone = false; //reset recordingTone
      if(toneDetected) {
        state = FHTProcessing;
      }
      else {
        state = dataAquistion;
      }

      if(recordTone){
        state = recordTone;
      }
      break;
    case FHTProcessing:
      if(!FHTProcDone) {
        state = FHTProcessing;
      }
      else {
        state = dataAquisiton;
        FHTProcDone = true;
        toneDetected = false;
      }
      break;
    case recordTone:
      if(recordingTone) {
        state = recordTone;
      }
      else {
        state = dataAquistion;
        recordTone = false;
      }
      break;
  }
}

void setup() {
  Serial.begin(9600); //begin serial connection
  lcd.begin(16, 2); //start lcd
  //Initialize I/O
  pinMode(vibMotor, OUTPUT);
  pinMode(mic, INPUT);
  //Initilize SD Card
  SD.begin(4);

  /*
  //Write to a file
  myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) {
    myFile.println("testing 1, 2, 3.");
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
  }
  */ 
}

void loop() {
  state = nextState(state);
  
  switch(state){
    case dataAquisition: //take 256 samples and see if tone detected
      if(millis()-sampleTime >= 100){
        //store sample
        sampleTime = millis();
      }
      //toneDetected = true;
      break;
    case FHTProcessing: //Take FHT and compare to files
      break;       
    case recordTone: //Record new tones to a file
      break;
  }
  
  
  /*if(millis()-displayTime >= 1000){
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
  }*/
}





  
