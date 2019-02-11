//Libraries
#include <LiquidCrystal.h>
//#include <SPI.h>
#include <SD.h>
#include <FHT.h>

//Global Variables
LiquidCrystal lcd(8, 7, 6, 5, 3, 2);
int vibMotor = 9;
int mic = A5;
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
  pinMode(mic, INPUT);
  //Initilize LCD
  lcd.begin(16, 2); //start lcd
  lcd.clear();
  //Initilize SD Card
  SD.begin(4);

  
  //Write to a file
  myFile = SD.open("test.csv", FILE_WRITE);
  myFile.print(0);
  myFile.println(",");
  delay(5000);
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

  
}

void loop() {
  /*if(i<500){
    myFile.print(micros());
    myFile.println("");
    i++;
  }
  else {
    myFile.close();
  }*/
  
  if(millis()<15000){

    //if(micros()-sampleTime > 6000){
      
      myFile.print(analogRead(mic));
      myFile.println(",");
      sampleTime = micros();
      //Serial.println(analogRead(mic));
      delayMicroseconds(400);
    //}
  }
  else {
    myFile.close();
  }
  
  /*
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
}





  
