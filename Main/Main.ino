#include <Servo.h>

Servo motor;
Servo steering;
Servo scanning;

//RangeFinder Vars
const int sensorPin = A0;  // the analog pin connected to the sensor
const long referenceMv = 5000; // long int to prevent overflow when multiplied
// the following is used to interpolate the distance from a table
// table entries are distances in steps of 250 millivolts
const int TABLE_ENTRIES = 12;
const int firstElement = 250; // first entry is 250 mV
const int INTERVAL  = 250; // millivolts between each element
int distance[] = {150,140,130,100,60,50,40,35,30,25,20,15};
boolean gaps[160];
int dist[160];
int sizeOfGap = 0;
int startOfGap = 0;

const int STOP_DISTANCE = 70;

int motorPin = 6;
int steeringPin = 9;
int scanningPin = 3;
int frontIRSensor = A1;
int backIRSensor = A0;

void setup()
{
    Serial.begin(9600);
  
    motor.attach(motorPin);        //Motor
    steering.attach(steeringPin);  //Turning Wheels
    scanning.attach(scanningPin);  //Servo mounted IR-sensor
    motor.writeMicroseconds(1500);
    steering.writeMicroseconds(1500);
    delay(3000);
    //motor.writeMicroseconds(2000);
}

void loop()
{
  steering.writeMicroseconds(1500);
  
  //Stop when wall is reached
    if(readDistance(frontIRSensor) < STOP_DISTANCE) 
    {
      stopCar();

      if(scanner2() == 1)
      {
        steering.writeMicroseconds(1000);
      }
      else {
        steering.writeMicroseconds(2000);
      }
      
      
      motor.writeMicroseconds(1500);
      delay(100);
      motor.writeMicroseconds(1200);
      delay(500);
      //scanner();
      
      
    }
    else {
      //scanning.write(0);
    }
    
    motor.writeMicroseconds(1650);      
       
}

////////Scanning code for the rangefinder
//EVEREST'S STUFF

//Use this
int readDistance(int pin) {
    int val = analogRead(pin);
    double mV = (val * referenceMv) / 1023;
    int cm = getDistance(mV);
    return cm;
}

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


void scanAround() {

    for(int i=0;i<160;i++)
    {
        scanning.write(i+10);
        
        if(readDistance(backIRSensor)>100) {
            gaps[i]=true;
            }
        else {
                gaps[i]=false;
            }
    }
}

void stopCar() {
  motor.writeMicroseconds(1200);
  delay(200);
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
    double distance = sqrt(((sq(x))/sq(cos(radianConv(0,a))))+((sq(x))/(sq(cos(radianConv(0,b)))))-((2*sq(x)*cos(radianConv(0,a-b)))/(cos(radianConv(0,b))*cos(radianConv(0,a)))));
        return distance;
}

//Sets the two global varaibles sizeOfGap and startOfGap
void getBiggestGap(){
    int count = 0;

    if(gaps[0] && gaps[1]){
        count++;
    }

    for(int i = 1; i < 160; i++){
        if(gaps[i]){
            count++;
        }
        if(!gaps[i]){
            if(count>sizeOfGap){
                sizeOfGap = count;
                startOfGap = i;
            }
            count = 0;
        }       
    }
}

//Returns the index of the middle of the gap
int getMiddlePositionOfGap() {
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
