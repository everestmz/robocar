//This is a very weird method which turns the car (roughly) 180 degrees after it has stopped too many times within a period of time

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
      
      
//Code written to test the IR sensor when it wasn't working
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
    
///Code written to find the width of a gap
/////////Method to find size of gaps

    //double gapDistance(double x, double a, double b){
    //  
    // //Since a and b will be entered as angles from the horizontal, must be converted to reflect this diagram:
    // /*
    //                          x
    //         -------------|   |   | ----------------
    //                       | a|b | 
    //                        | | |
    //                         |||
    //                          |
    //*/
    //  //Assuming a is the first angle
    //    a = 90 - a;
    //    b = b - 90  ;
    //  
    //    //Using the cos rule (c^2 = a^2 + b^2 - 2ab*cos(C)) to find c
    //    
    //    int sideA = (sq(x))/sq(cos(radianConv(0,a)));
    //    int sideB = (sq(x))/(sq(cos(radianConv(0,b))));
    //    //Changed a-b to a+b to fix issue   
    //    int cosC = cos(radianConv(0,a+b));
    //  
    //    double distance = sqrt(sideA*sideA + sideB*sideB -2*sideA*sideB*cosC);
    //    return distance;
    //}
    //
    //      
    //
    //     
   
//No clue what this is, some kinda scanner method. Doesn't do anything we need.
    ///////Stuff
    //boolean scanner2()
    //{
    //  scanning.write(0);
    //  delay(300);
    //  int leftreading = analogRead(backIRSensor);
    //  
    //  scanning.write(180);
    //  delay(300);
    //  int rightreading = analogRead(backIRSensor);
    //  
    //  scanning.write(90);
    //  
    //  if (leftreading > rightreading) 
    //  {
    //    return true;
    //  }
    //  
    //}
    // 
