#include <Servo.h>

Servo motor;
Servo steering;
Servo scanning;

//RangeFinder Vars///
const int sensorPin = A0;  // the analog pin connected to the sensor
const long referenceMv = 5000; // long int to prevent overflow when multiplied
// the following is used to interpolate the distance from a table
// table entries are distances in steps of 250 millivolts
const int TABLE_ENTRIES = 12;
const int firstElement = 250; // first entry is 250 mV
const int INTERVAL  = 250; // millivolts between each element
int distance[] = {150,140,130,100,60,50,40,35,30,25,20,15};
boolean gaps[180];
int dist[160];
int sizeOfGap = 0;
int startOfGap = 0;
/////////
const int STOP_DISTANCE = 50;
const int BACK_STOP_DISTANCE = 50;
const int MAX_STRAY_ZEROS = 4;
/////////
int scanningPin = 3;
int motorPin = 6;
int steeringPin = 9;
int frontIRSensor = A1;
int backIRSensor = A5;
boolean stopped = false;
int stopCount = 0;
long unsigned int stopMillis = 0;

void setup()
{
    Serial.begin(1200);
    motor.attach(motorPin);        //Motor
    steering.attach(steeringPin);  //Turning Wheels
    scanning.attach(scanningPin);  //Servo mounted IR-sensor
    motor.writeMicroseconds(1500);
    steering.writeMicroseconds(1500);
    scanning.writeMicroseconds(0);
    delay(3000);
    //motor.writeMicroseconds(2000);
}

void loop()
{  
    scanning.writeMicroseconds(1500);
    
    //Stop when wall is reached
    if(readDistance(frontIRSensor) < STOP_DISTANCE || readDistance(backIRSensor) < STOP_DISTANCE + 20) 
    {
      if(!stopped)
      {
         stopCar();
         /*if(stopCount==0)
         {
           //stopMillis = (long unsigned int)millis;
         }*/
         //stopCount ++;
      }
      /*long unsigned int p = (long unsigned int)millis;
      if(stopCount > 3 && p-stopMillis<=30000)
      {
        stopCount = 0;
        //Back straight a bit
        steering.write(90);
        motor.writeMicroseconds(1200);
        delay(1000);
        motor.writeMicroseconds(1500);
        delay(50);
        //Turn one way
        steering.write(45);
        motor.writeMicroseconds(1200);
        delay(1000);
        motor.writeMicroseconds(1500);
        delay(50);
        //Turn the other way
        steering.write(135);
        motor.writeMicroseconds(1650);
        delay(1000);
        motor.writeMicroseconds(1500);
        delay(50);
        steering.write(90);
      }
      else if(stopCount>3)
      {
        stopCount = 0;
      }*/
      
      scanArea();
        
      delay(1000);
        
      removeStrayZeros();
      getBiggestGap();
      steering.write(180-getMiddleOfGap());
      motor.writeMicroseconds(1200);
      delay(reverseDelay(getMiddleOfGap()));
      motor.writeMicroseconds(1500);
      steering.writeMicroseconds(1500);
      
    }
    else {
      //scanning.write(0);
    }
    
    motor.writeMicroseconds(1650);
    delay(100);
    stopped = false;
      
      //Serial.println(readDistance(frontIRSensor));
      
//      if(readDistance(frontIRSensor) < STOP_DISTANCE) {
//        stopCar();
//        
//        scanArea();
//        
//        delay(1000);
//        
//        removeStrayZeros();
//        getBiggestGap();
//        
//        //Prints GapsArray + other crap
////        for(int i =0; i < 180; i++) 
////        {
////          
////          if(i == startOfGap) {
////            Serial.print('M');
////          }
////            
////            
////          Serial.print(gaps[i]);
////        }
////        Serial.println();
////        
////        Serial.println(startOfGap);
//
//        steering.write(getMiddleOfGap());
//      }
      
            
}
      

////////Scanning code for the rangefinder
//Use this
int readDistance(int pin) {
    int val = analogRead(pin);
    double mV = (val * referenceMv) / 1023;
    int cm = getDistance(mV);
    return cm;
}
/////////
//DONT DELETE AND DONT TOUCH
int getDistance(int mV)
{
    if( mV >  INTERVAL * TABLE_ENTRIES-1 )
        return distance[TABLE_ENTRIES-1];
    else
    {
        int index = mV / INTERVAL;
        float frac = (mV % 250) / (float)INTERVAL;
        return distance[index] - ((distance[index] - distance[index+1]) * frac);
    }
}
//////////
/////////
int reverseDelay(int midGap)
{
  
  int scaleFactor = abs(90-midGap);
  
  return 400+5*scaleFactor;
  
}

void scanArea() {
  
  int num = 0;
  
  scanning.write(0);
  
  for(int i=600;i<2400;i += 10)
  {
      scanning.writeMicroseconds(i);
      delay(15);
      
      if(readDistance(backIRSensor) > BACK_STOP_DISTANCE) {
        gaps[num]=1;
      } else {
        gaps[num]=0;
      }
      num++;
  }
  
  //Needed to reset the scan and give it enough time to reset
  scanning.write(90);
  delay(250);
}

void stopCar() {
  motor.writeMicroseconds(1000);
  delay(200);
  motor.writeMicroseconds(1500);
  stopped = true;
}

/////////(To/From Radians)
double radianConv(boolean to, float angle) {
    double pi = 3.14159;
    double result = 0;
    
    if(to) {
        result = (angle/180)*pi;
    }
    else{
        result = (angle/pi)*180;
    }
    
    return result;
}

/////////End Scanning Code


/////////Method to find size of gaps

double gapDistance(double x, double a, double b){
  
 //Since a and b will be entered as angles from the horizontal, must be converted to reflect this diagram:
 /*
                          x
         -------------|   |   | ----------------
                       | a|b | 
                        | | |
                         |||
                          |
*/
  //Assuming a is the first angle
    a = 90 - a;
    b = b - 90  ;
  
    //Using the cos rule (c^2 = a^2 + b^2 - 2ab*cos(C)) to find c
    
    int sideA = (sq(x))/sq(cos(radianConv(0,a)));
    int sideB = (sq(x))/(sq(cos(radianConv(0,b))));
    //Changed a-b to a+b to fix issue   
    int cosC = cos(radianConv(0,a+b));
  
    double distance = sqrt(sideA*sideA + sideB*sideB -2*sideA*sideB*cosC);
    return distance;
}

//Sets the two global varaibles sizeOfGap and startOfGap
void getBiggestGap()
{
  int num = 0;
  startOfGap = 0;
  sizeOfGap = 0;
  boolean gapStarted = false;

  for(int i = 0; i < 180; i++) 
  {
      if(gaps[i] == 1) {
          gapStarted = true;
          num++;
      }

      if(gapStarted && (gaps[i] == 0 || i+1 == 180)) 
      {
    
          if(num>sizeOfGap)
          {
              sizeOfGap = num;
              startOfGap = i-sizeOfGap;
          }
      }
    
      if(gaps[i] == 0) 
      {
          num = 0;
          gapStarted = false;
      }
  }   
}

//Returns the index of the middle of the gap
int getMiddleOfGap() {
    return (sizeOfGap/2) + startOfGap;
}



//////Stuff
boolean scanner2()
{
  scanning.write(0);
  delay(300);
  int leftreading = analogRead(backIRSensor);
  
  scanning.write(180);
  delay(300);
  int rightreading = analogRead(backIRSensor);
  
  scanning.write(90);
  
  if (leftreading > rightreading) 
  {
    return true;
  }
  
}

void removeStrayZeros() {
  
  int numOfStrayZeros = 0;
  boolean possibleStrayZero = false;
  
  for(int i = 0; i < 180; i++) {
    
    if(gaps[i] == 1) {
      possibleStrayZero = true;
    }
  
    if(possibleStrayZero && gaps[i] == 0) {
      numOfStrayZeros++;
    }
  
    if(numOfStrayZeros <= MAX_STRAY_ZEROS && gaps[i] == 1) {
      
      for(int x = 1; x <= numOfStrayZeros; x++) {
        gaps[i-x] = 1;
      }
      
      numOfStrayZeros = 0;
    }
    
    if(numOfStrayZeros >= 4) {
      possibleStrayZero = false;
      numOfStrayZeros = 0;
    }
  }
}
