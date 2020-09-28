const int N=1;
const int S=2;
const int OFF=0;

// *************************************************************************************
// **************************** BEGIN EDITABLE AREA ************************************                        
// *************************************************************************************

void setup() {  
  pinMode(12,OUTPUT); // Coil A: DIR
  pinMode(3,OUTPUT);  // Coil A: CURRENT
  pinMode(9,OUTPUT);  // Coil A: BRAKE

  pinMode(13,OUTPUT); // Coil B: DIR
  pinMode(11,OUTPUT); // Coil B: CURRENT
  pinMode(8,OUTPUT);  // Coil B: BRAKE

  pinMode(2, INPUT);  // Pin 2 is reading the switch

  Serial.begin(115200);
 
  shutdown_all();
}


// ********************** USE THESE SETTINGS TO PREVENT OVERHEATING *********************
int power_stall=50;     // Just enough to move. Use 7 for half-stepping, 50 for full-stepping
int power=50;           // Set from 30 to 100 depending on required torque. Higher values make the motor shield hotter.
                        // Use values <= 50 for half-stepping (because both coils are on simultaneously)
int step_duration=5000; // For very long step_duration (>7000) use low power to prevent overheating the shield.
                        // If motor gets stuck (especially for step_duration around 5500), increase power.

                        // ALWAYS CHECK THE MOTOR SHIELD FOR OVERHEATING !!
                        
unsigned long int t0 = 0, t = 0;
float iA = 0; float iB = 0;
// **************************************************************************************

void loop() { 
  t0 = micros();
  
  int sw = 0;                           //Initializing switch variable
  sw = digitalRead(2);
  int ct = 0;
  
  while(sw == 0)
  {
      sw = digitalRead(2);              //Start the homing sequence when switch is pressed
  }

  //Serial.print(sw);
  delay(1000);                          //give the user 1 second to release the switch
  sw = digitalRead(2);
  
  while(sw == LOW)
  {
    sw = digitalRead(2);                //Start the homing sequence when switch is pressed
    coilPolarities(N, OFF);
    //delay(1);                         //1.5 ms is the smallest interval for fastest motion of this motor
    coilPolarities(OFF, S);
    //delay(1);
    coilPolarities(S, OFF);
    //delay(1);
    coilPolarities(OFF, N);
    //delay(1);

    ct++;
    
    iA = float(analogRead(0)) / 1023.0*5.0 / 1.65 * 1000.0;
    iB = float(analogRead(1)) / 1023.0*5.0 / 1.65 * 1000.0;
    
    /* //-----CURRENT MEASUREMENT
    Serial.print(String((micros()-t0)/1000000) + "\t" + String(ct*4));
    Serial.print("\t");
    Serial.print(iA);
    Serial.print("\t");
    Serial.println(iB);
    */
       //-----STEP MEASUREMENT
    Serial.println(String(micros()-t0)+"\t"+String(ct*4));
  }

  step_duration=10000; //reduce speed to half
  for (int ct=1;ct<=31;ct++){          //50 ct per revolution. 8 mm lead per revolution. Need 5mm so 0.625 rev, or 31.25
    coilPolarities(N, OFF);            //backwards direction
    //delay(2);                          //3 ms is half speed
    coilPolarities(OFF, N);
    //delay(2);
    coilPolarities(S, OFF);
    //delay(2);
    coilPolarities(OFF, S);
    //delay(2);


    /* //-----CURRENT MEASUREMENT
    iA= float(analogRead(0)) /1023.0*5.0 / 1.65 * 1000.0;
    iB= float(analogRead(1)) /1023.0*5.0 / 1.65 * 1000.0;
    Serial.print(String((micros()-t0)/1000000) + "\t" + String(ct*4));
    Serial.print("\t");
    Serial.print(iA);
    Serial.print("\t");
    Serial.println(iB);
    */

    //-----STEP MEASUREMENT
    Serial.println(String(micros()-t0)+"\t"+String(ct*4));


    t = micros()-t0;                    //elapsed time since crossing t0 = micros() line
   // Serial.print(t);
   // Serial.print("\t");
   // Serial.println(ct*4);         //printing "microseconds elapsed since started moving" on the left and number of steps on the right
  }
  
  sw = digitalRead(2);
  power_stall=7;                      //half-stepping
  power=30;
  ct = 0;

  step_duration=2500;                 //half speed, half step
  while(sw == LOW)
  {
    sw = digitalRead(2);                //Start the homing sequence when switch is pressed
    coilPolarities(N, OFF);
    delay(4);                         //1.5 ms is the smallest interval for fastest motion of this motor
    coilPolarities(OFF, S);
    delay(4);
    coilPolarities(S, OFF);
    delay(4);
    coilPolarities(OFF, N);
    delay(4);

    ct++;

    /* //-----CURRENT MEASUREMENT
    iA = float(analogRead(0)) / 1023.0*5.0 / 1.65 * 1000.0;
    iB = float(analogRead(1)) / 1023.0*5.0 / 1.65 * 1000.0;
    Serial.print(String((micros()-t0)/1000000) + "\t" + String(ct*4));
    Serial.print("\t");
    Serial.print(iA);
    Serial.print("\t");
    Serial.println(iB);
    */

    //-----STEP MEASUREMENT
    Serial.println(String(micros()-t0)+"\t"+String(ct*4));
    
  }

  step_duration=5000; //quarter speed, half step
  for (int ct=1;ct<=188;ct++){          //50 ct per revolution (half-step). 8 mm lead per revolution. Need 30mm so 3.75 rev, or 187.5
    coilPolarities(N, OFF);            //backwards direction
    delay(4);                          //3 ms is half speed
    coilPolarities(OFF, N);
    delay(4);
    coilPolarities(S, OFF);
    delay(4);
    coilPolarities(OFF, S);
    delay(4);

    /* //-----CURRENT MEASUREMENT
    iA = float(analogRead(0)) / 1023.0*5.0 / 1.65 * 1000.0;
    iB = float(analogRead(1)) / 1023.0*5.0 / 1.65 * 1000.0;
    Serial.print(String((micros()-t0)/1000000) + "\t" + String(ct*4));
    Serial.print("\t");
    Serial.print(iA);
    Serial.print("\t");
    Serial.println(iB);
    */

    //-----STEP MEASUREMENT
    Serial.println(String(micros()-t0)+"\t"+String(ct*4));
    
   t = micros()-t0;                    //elapsed time since crossing t0 = micros() line
   //Serial.print(t);
   //Serial.print("\t");
   //Serial.println(ct*4);         //printing "microseconds elapsed since started moving" on the left and number of steps on the right
  }
  shutdown_all();                   //turn everything off
  while(true);                      //sinkhole
}

// *************************************************************************************
// ***************************** END OF EDITABLE AREA **********************************
// *************************************************************************************




// *************************************************************************************
// *************************** DO NOT EDIT BELOW THIS LINE *****************************
// *************************************************************************************
void coilPolarities(int polarityA, int polarityB){
  int powerA=power, powerB=power;

  switch(polarityA){
    case N:
      digitalWrite(12, HIGH);
      break;
    case S:
      digitalWrite(12,LOW);
      break;
    default:
      digitalWrite(12,LOW);
      powerA=0;
  }
  switch(polarityB){
    case N:
      digitalWrite(13, HIGH);
      break;
    case S:
      digitalWrite(13,LOW);
      break;
    default:
      digitalWrite(13,LOW);
      powerB=0;
  }
  setCurrents(powerA, powerB);
}

void setCurrents(int powerA, int powerB){
  long int t0=0, PWM_cycle_duration=50;
  long int PWM_high_A=0, PWM_high_B=0, dutyA=0, dutyB=0;
  int first_pin=0, second_pin=0;
  long int first_time=0, second_time=0;
  
  if (powerA>100) powerA=100;
  if (powerA<0) powerA=0;
  if (powerB>100) powerB=100;
  if (powerB<0) powerB=0;

  dutyA=power_stall+int(powerA*(100-power_stall)/100);
  dutyB=power_stall+int(powerB*(100-power_stall)/100);
  
  PWM_high_A=int(PWM_cycle_duration*dutyA/100);
  PWM_high_B=int(PWM_cycle_duration*dutyB/100);

  if (PWM_high_A>=PWM_high_B){
    first_pin=11;
    second_pin=3;
    first_time=PWM_high_B;
    second_time=PWM_high_A;
  }
  else{
    first_pin=3;
    second_pin=11;
    first_time=PWM_high_A;
    second_time=PWM_high_B;
  }

  t0=micros();
  while(micros()-t0<step_duration){
    digitalWrite(first_pin, HIGH);
    digitalWrite(second_pin, HIGH);
    delayMicroseconds(first_time);
    digitalWrite(first_pin, LOW);
    delayMicroseconds(second_time-first_time);
    digitalWrite(second_pin, LOW);      
  }
}

void shutdown_all(){
  digitalWrite(12,LOW);
  digitalWrite(3,LOW);
  digitalWrite(9,LOW);

  digitalWrite(13,LOW);
  digitalWrite(11,LOW);
  digitalWrite(8,LOW);
}
// *************************************************************************************
// ********************************** THAT'S ALL ***************************************
// *************************************************************************************
