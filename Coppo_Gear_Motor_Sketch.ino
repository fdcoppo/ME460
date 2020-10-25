void setup() {
    pinMode(12,OUTPUT); // Dir
    pinMode(3,OUTPUT);  // Current
    pinMode(9,OUTPUT);  // Break
  
    pinMode(7,INPUT);
    pinMode(10, INPUT); //Switch Input
    Serial.begin(115200); //send 115200 bits per second to the serial monitor

    digitalWrite(9,LOW);  //DISABLE BREAK

    attachInterrupt(digitalPinToInterrupt(2), count, RISING);
      
}

volatile long int pulses = 0; //variable in RAM memory for fast task memory allocation
                              //the motor does 374 pulses per revolution

int v_motor = 420;                //set motor voltage
double current_rpm = 0;
int target = getRpm(35);
int k = 1;

void loop() {
  
  if (v_motor > 450)
    v_motor = 450;
  else if (v_motor < 0)
    v_motor = 0;
  
  int t0 = micros();
  
  int sw = 0;
  sw = digitalRead(10);

  while(sw == 0)
    sw = digitalRead(10);  //Do not start homing sequence until switch is pressed
  
  delay(1000);             //give the user 1 second to release the switch
  sw = digitalRead(10);

///////////////////////////////////////////MOVE TO TARGET at 35mm/s////////////////////////////
  
  current_rpm = getSpeed(100000);
  target = getRpm(35);
  Serial.println(String(millis()) + "\t" + String(int(v_motor)) + "\t" + String(current_rpm));
  
  while(sw == LOW)         //move towards switch at 35mm/s
  {
    
      if(current_rpm > target)
        v_motor = v_motor - k*1;
      else if(current_rpm < target)
        v_motor = v_motor + k*1;
        
      digitalWrite(12, HIGH); //Set direction: High is CCW looking down from the shaft of the motor
      analogWrite(3, v_motor);  //Turn On the Voltage. Use analog to set intermediate value.

      analogWrite(3, v_motor);
      delay(100);
      current_rpm = getSpeed(100000);

      //Print out Time [us], Control Voltage [0-255], Speed [rpm]
      Serial.println(String(millis()) + "\t" + String(int(v_motor)) + "\t" + String(current_rpm));

      sw = digitalRead(10);

  }

  int  pulsestatus1 = pulses;
  sw = digitalRead(10);
  current_rpm = getSpeed(100000);
  v_motor = 78;
  k = 1;

////////////////////////////////RETRACT 10 mm at 20mm/s/////////////////////////////////////////
  target = getRpm(20);
  while(abs(pulses - pulsestatus1) < getPulses(10))
  {
    if(current_rpm > target)
        v_motor = v_motor - k*1;
    else if(current_rpm < target)
        v_motor = v_motor + k*1;

    digitalWrite(12, LOW); //Set direction: High is CCW looking down from the shaft of the motor
    analogWrite(3, v_motor);  //Turn On the Voltage. Use analog to set intermediate value.

    analogWrite(3, v_motor);
    delay(100);
    current_rpm = getSpeed(100000);

    //Print out Time [us], Control Voltage [0-255], Speed [rpm]
    Serial.println(String(millis()) + "\t" + String(int(v_motor)) + "\t" + String(current_rpm*(-1)));

    sw = digitalRead(10);
  }

  int  pulsestatus2 = pulses;
  sw = digitalRead(10);
  current_rpm = getSpeed(10000);
  v_motor = 45;
  
////////////////////////////////Move back to Switch at 6mm/s (minimum)/////////////////////////////////////////
  target = getRpm(6);
  while(sw == LOW)         //move towards switch at 20mm/s
  {
    
      if(current_rpm > target)
        v_motor = v_motor - k*1;
      else if(current_rpm < target)
        v_motor = v_motor + k*1;
        
      digitalWrite(12, HIGH); //Set direction: High is CCW looking down from the shaft of the motor
      analogWrite(3, v_motor);  //Turn On the Voltage. Use analog to set intermediate value.

      analogWrite(3, v_motor);
      delay(100);
      current_rpm = getSpeed(100000);

      //Print out Time [us], Control Voltage [0-255], Speed [rpm]
      Serial.println(String(millis()) + "\t" + String(int(v_motor)) + "\t" + String(current_rpm));

      sw = digitalRead(10);

  }

  int  pulsestatus3 = pulses;
  sw = digitalRead(10);
  current_rpm = getSpeed(100000);
  v_motor = 43;
  k = 1;
  
////////////////////////////////RETRACT 30 mm at 10mm/s/////////////////////////////////////////
  target = getRpm(10);
  while(abs(pulses - pulsestatus3) < getPulses(30))
  {
    if(current_rpm > target)
        v_motor = v_motor - k*1;
    else if(current_rpm < target)
        v_motor = v_motor + k*1;

    digitalWrite(12, LOW); //Set direction: High is CCW looking down from the shaft of the motor
    analogWrite(3, v_motor);  //Turn On the Voltage. Use analog to set intermediate value.

    analogWrite(3, v_motor);
    delay(100);
    current_rpm = getSpeed(10000);

    //Print out Time [us], Control Voltage [0-255], Speed [rpm]
    Serial.println(String(millis()) + "\t" + String(int(v_motor)) + "\t" + String(current_rpm*(-1)));

    sw = digitalRead(10);
  }

  
  shutdown_all();                   //turn everything off
  while(true);                      //sinkhole
}

void shutdown_all(){
  digitalWrite(12,LOW);
  digitalWrite(3,LOW);
  digitalWrite(9,LOW);
}

void count()
{
  if (digitalRead(7) == LOW)
    pulses++;
  else
    pulses--;  
}

double getSpeed(long int us)
{
  unsigned long int t0 = 0, initialPulses = 0, delta_pulses = 0, delta_t;
  double rpm = 0;
  initialPulses = pulses;   //Set initial snapshot of the pulse count
  t0 = micros();              //Set the initial time in us
  while(micros() - t0 < us){}   //Allow sampling time in microseconds to elapse
  delta_t = us;                 //Duration of the sample, in microseconds
  if(pulses < initialPulses)
    delta_pulses = initialPulses - pulses;
  else if(pulses > initialPulses)
    delta_pulses = pulses - initialPulses;  //Pulse increase during sample period

  rpm = double(delta_pulses)/double(delta_t)*1E+6 / 374.0 * 60.0;

  return(rpm);  
}

float getRpm(int vel) //returns the desired rpm value given linear velocity in mm/s
{
  float myrpm = vel*7.5;
  return myrpm;
}

float getPulses(int dist)
{
  float myrevs = 46.75*dist;
  return myrevs;
}
