#include <Adafruit_MLX90614.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

LiquidCrystal_I2C lcd(0x27, 16, 2);

SoftwareSerial espSerial(5, 6);

const int motionLed = 2;
const int distanceMotionLed =  11;
const int alertLed = 13;

int buzzerPin = 8;

int trigPin = 4;    // Trigger
int echoPin = 3;    // Echo
int trigPin1 = 10;    // Trigger
int echoPin1 = 9;    // Echo

long duration, cm, inches;
long duration1, cm1, inches1;

bool detected = false;
bool detected1 = false;
bool firstTime = true;
bool goInsideLoop = false;

int loopCounter = 0;

void setup() {
  Serial.begin(9600);// setup Serial Monitor to display information
  espSerial.begin(115200);
  
  lcd.begin();
  
  while (!Serial);

  if (!mlx.begin()) {
    Serial.println("Error connecting to MLX sensor. Check wiring.");
    while (1);
  };

  pinMode(motionLed, OUTPUT);
  pinMode(distanceMotionLed, OUTPUT);
  pinMode(alertLed, OUTPUT);
  
  pinMode(7, INPUT);// Input from sensor

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  
  pinMode(buzzerPin, OUTPUT);
}

void loop() {

  if(loopCounter == 15){
     loopCounter = 0;
  } 
  
  int motion =digitalRead(7);
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  cm = (duration/2) / 29.1;
  
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);

  pinMode(echoPin1, INPUT);
  duration1 = pulseIn(echoPin1, HIGH);
  cm1 = (duration1/2) / 29.1;

  if(firstTime == false){

     if(cm1 <= 20 ){
      digitalWrite(distanceMotionLed, HIGH);
    }
  
    else{
      digitalWrite(distanceMotionLed, LOW);
    }
  
    if(motion){
      digitalWrite(motionLed, HIGH);
    }
  
     else{
      digitalWrite(motionLed, LOW);
    }
  
    if(cm <= 20){
       digitalWrite(alertLed, HIGH);
    }
  
     else{
      digitalWrite(alertLed, LOW);
    }
    
    if(((motion && cm1 <= 20) && cm > 20) || ((motion && cm1 < 20) && cm <= 20)){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Detected");
  
        delay(2000);
  
        lcd.clear();
        
        lcd.setCursor(0,0);
        lcd.print("Analyzing");
        
        delay(2000);
  
        digitalWrite(trigPin1, LOW);
        delayMicroseconds(5);
        digitalWrite(trigPin1, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin1, LOW);
  
        pinMode(echoPin1, INPUT);
        duration1 = pulseIn(echoPin1, HIGH);
  
        cm1 = (duration1/2) / 29.1;

        motion =digitalRead(7);

        digitalWrite(trigPin, LOW);
        delayMicroseconds(5);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
      
        pinMode(echoPin, INPUT);
        duration = pulseIn(echoPin, HIGH);
        cm = (duration/2) / 29.1;

        if(cm1 <= 20 ){
          digitalWrite(distanceMotionLed, HIGH);
        }
      
        else{
          digitalWrite(distanceMotionLed, LOW);
        }
      
        if(motion){
          digitalWrite(motionLed, HIGH);
        }
      
         else{
          digitalWrite(motionLed, LOW);
        }
      
        if(cm <= 20){
           digitalWrite(alertLed, HIGH);
        }
      
         else{
          digitalWrite(alertLed, LOW);
        }
        
        if(cm1 <= 20 && motion){
          lcd.clear();

          int loopDisplay = 0;
          
          for(;;){
            if(loopDisplay == 15){
              loopDisplay = 0;
            }
              
            digitalWrite(trigPin1, LOW);
            delayMicroseconds(5);
            digitalWrite(trigPin1, HIGH);
            delayMicroseconds(10);
            digitalWrite(trigPin1, LOW);
      
            pinMode(echoPin1, INPUT);
            duration1 = pulseIn(echoPin1, HIGH);
      
            cm1 = (duration1/2) / 29.1;

            motion =digitalRead(7);

            digitalWrite(trigPin, LOW);
            delayMicroseconds(5);
            digitalWrite(trigPin, HIGH);
            delayMicroseconds(10);
            digitalWrite(trigPin, LOW);
          
            pinMode(echoPin, INPUT);
            duration = pulseIn(echoPin, HIGH);
            cm = (duration/2) / 29.1;

            if(cm1 <= 20 ){
              digitalWrite(distanceMotionLed, HIGH);
            }
          
            else{
              digitalWrite(distanceMotionLed, LOW);
            }
          
            if(motion){
              digitalWrite(motionLed, HIGH);
            }
          
             else{
              digitalWrite(motionLed, LOW);
            }
          
            if(cm <= 20){
               digitalWrite(alertLed, HIGH);
            }
          
             else{
              digitalWrite(alertLed, LOW);
            }
            
            if(cm1 > 20 && !motion){
              break;
            }

            if(loopDisplay == 0){
              lcd.setCursor(0,0);
              lcd.print("Occupied");
            }

              digitalWrite(trigPin, LOW);
              delayMicroseconds(5);
              digitalWrite(trigPin, HIGH);
              delayMicroseconds(10);
              digitalWrite(trigPin, LOW);
        
              pinMode(echoPin, INPUT);
              duration = pulseIn(echoPin, HIGH);
        
              cm = (duration/2) / 29.1;
              
              String celcius = String(mlx.readObjectTempC());

              if(loopDisplay == 0){
                 espSerial.println(String(celcius + "%1%" + cm + "$"));
              }

              delay(1000);

              loopDisplay++;
          }
        }
  
        else{
          lcd.clear();
          
          lcd.setCursor(0,0);
          lcd.print("Empty");
          
          digitalWrite(trigPin, LOW);
          delayMicroseconds(5);
          digitalWrite(trigPin, HIGH);
          delayMicroseconds(10);
          digitalWrite(trigPin, LOW);
    
          pinMode(echoPin, INPUT);
          duration = pulseIn(echoPin, HIGH);
    
          cm = (duration/2) / 29.1;
          
          String celcius = String(mlx.readObjectTempC());
        }
      
      detected = true;
      detected1 = false;

      digitalWrite(trigPin, LOW);
        delayMicroseconds(5);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        
        pinMode(echoPin, INPUT);
        duration = pulseIn(echoPin, HIGH);
        
        cm = (duration/2) / 29.1;

        String celcius = String(mlx.readObjectTempC());
    
        if(loopCounter == 0){
           espSerial.println(String(celcius + "%0%" + cm + "$"));
        } 
  }
  
  else{
        if(cm <= 20 && (cm1 > 20 && !motion)){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Detected");
    
          delay(2000);
    
          lcd.clear();
          
          lcd.setCursor(0,0);
          lcd.print("Analyzing");
          
          delay(2000);
    
          digitalWrite(trigPin, LOW);
          delayMicroseconds(5);
          digitalWrite(trigPin, HIGH);
          delayMicroseconds(10);
          digitalWrite(trigPin, LOW);
    
          pinMode(echoPin, INPUT);
          duration = pulseIn(echoPin, HIGH);
    
          cm = (duration/2) / 29.1;

          motion =digitalRead(7);

          digitalWrite(trigPin1, LOW);
          delayMicroseconds(5);
          digitalWrite(trigPin1, HIGH);
          delayMicroseconds(10);
          digitalWrite(trigPin1, LOW);
        
          pinMode(echoPin1, INPUT);
          duration1 = pulseIn(echoPin1, HIGH);
          cm1 = (duration1/2) / 29.1;

          if(cm1 <= 20 ){
            digitalWrite(distanceMotionLed, HIGH);
          }
        
          else{
            digitalWrite(distanceMotionLed, LOW);
          }
        
          if(motion){
            digitalWrite(motionLed, HIGH);
          }
        
           else{
            digitalWrite(motionLed, LOW);
          }
        
          if(cm <= 20){
             digitalWrite(alertLed, HIGH);
          }
        
           else{
            digitalWrite(alertLed, LOW);
          }
  
          
          if(cm <= 20 && (cm1 > 20 && !motion)){
            int loopDispay = 0;
            
            for(;;){
              if(loopDispay == 3){
                loopDispay = 0;
              }
              
              digitalWrite(trigPin, LOW);
              delayMicroseconds(5);
              digitalWrite(trigPin, HIGH);
              delayMicroseconds(10);
              digitalWrite(trigPin, LOW);
        
              pinMode(echoPin, INPUT);
              duration = pulseIn(echoPin, HIGH);
        
              cm = (duration/2) / 29.1;

              motion =digitalRead(7);

              digitalWrite(trigPin1, LOW);
              delayMicroseconds(5);
              digitalWrite(trigPin1, HIGH);
              delayMicroseconds(10);
              digitalWrite(trigPin1, LOW);
            
              pinMode(echoPin1, INPUT);
              duration1 = pulseIn(echoPin1, HIGH);
              cm1 = (duration1/2) / 29.1;

              if(cm1 <= 20 ){
                digitalWrite(distanceMotionLed, HIGH);
              }
            
              else{
                digitalWrite(distanceMotionLed, LOW);
              }
            
              if(motion){
                digitalWrite(motionLed, HIGH);
              }
            
               else{
                digitalWrite(motionLed, LOW);
              }
            
              if(cm <= 20){
                 digitalWrite(alertLed, HIGH);
              }
            
               else{
                digitalWrite(alertLed, LOW);
              }
  
              if(cm > 20 || (cm <= 20 && (cm1 < 20 && motion))){
                detected1 = false;
                goInsideLoop = true;
                
                break;
              }

              if(loopDispay == 0){
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("Person collapsed");
              }
              
              String celcius = String(mlx.readObjectTempC());

              if(loopDispay == 0){
                espSerial.println(String(celcius + "%0%" + cm + "$"));
              }

              analogWrite(buzzerPin, 255); 
              delay(3000);//wait for 1ms
              digitalWrite(buzzerPin,LOW);
              delay(3000);//wait for 1ms

              loopDispay++;
            }
            
          }

          else{
            lcd.clear();
              
            lcd.setCursor(0,0);
            lcd.print("Empty");

            digitalWrite(trigPin, LOW);
            delayMicroseconds(5);
            digitalWrite(trigPin, HIGH);
            delayMicroseconds(10);
            digitalWrite(trigPin, LOW);
        
            pinMode(echoPin, INPUT);
            duration = pulseIn(echoPin, HIGH);
        
            cm = (duration/2) / 29.1;

            String celcius = String(mlx.readObjectTempC());
            
            //espSerial.println(String(celcius + "%0%" + cm + "$"));
          }   
        }
        
        else{
            if(detected1 == false){
            lcd.clear();
            
            lcd.setCursor(0,0);
            lcd.print("Empty");

            digitalWrite(trigPin, LOW);
            delayMicroseconds(5);
            digitalWrite(trigPin, HIGH);
            delayMicroseconds(10);
            digitalWrite(trigPin, LOW);
        
            pinMode(echoPin, INPUT);
            duration = pulseIn(echoPin, HIGH);
        
            cm = (duration/2) / 29.1;

            String celcius = String(mlx.readObjectTempC());
    
            //espSerial.println(String(celcius + "%0%" + cm + "$"));
          }
        }   

      detected = false;

      if(goInsideLoop == false){
        detected1 = true;
        goInsideLoop = false;
      }
      
        goInsideLoop = false;

       digitalWrite(trigPin, LOW);
        delayMicroseconds(5);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        
        pinMode(echoPin, INPUT);
        duration = pulseIn(echoPin, HIGH);
        
        cm = (duration/2) / 29.1;

        String celcius = String(mlx.readObjectTempC());

        if(loopCounter == 0){
           espSerial.println(String(celcius + "%0%" + cm + "$"));
        } 
  }
}

  firstTime = false;

  loopCounter++;
  delay(1000);
}
