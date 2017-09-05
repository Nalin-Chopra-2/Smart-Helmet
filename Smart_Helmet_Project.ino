#include <Wire.h>

long accelX, accelY, accelZ;
float gForceX, gForceY, gForceZ;

long gyroX, gyroY, gyroZ;
float rotX, rotY, rotZ;

int conversionrad = 180;
int hits_counter = 0;
int linThresh = 0.9 *1.2;
int rotThresh = 200*1.5/ conversionrad;
int linThreshZ = 1.6*1.2;
int avgCount = 0;
int numMs = 15;

int conversionlin = 10;


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void setup() {
  Serial.begin(9600);
  Wire.begin();
  setupMPU();
}


void loop() {
  recordAccelRegisters();
  recordGyroRegisters();
  takeAvg();
//  checkDiag();
//  printData();
  plotData();
//  hit_counter();
}

void setupMPU(){
  Wire.beginTransmission(0b1101000); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
  Wire.endTransmission();  
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1B); //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4) 
  Wire.write(0x00000000); //Setting the gyro to full scale +/- 250deg./s 
  Wire.endTransmission(); 
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1C); //Accessing the register 1C - Acccelerometer Configuration (Sec. 4.5) 
  Wire.write(0b00000000); //Setting the accel to +/- 2g
  Wire.endTransmission(); 
}

void recordAccelRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x3B); //Starting register for Accel Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Accel Registers (3B - 40)
  while(Wire.available() < 6);
  accelX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  accelY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  accelZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  processAccelData();
}

void processAccelData(){
  gForceX = accelX / 16384.0;
  gForceY = accelY / 16384.0; 
  gForceZ = accelZ / 16384.0;
}

void recordGyroRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x43); //Starting register for Gyro Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Gyro Registers (43 - 48)
  while(Wire.available() < 6);
  gyroX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  gyroY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  gyroZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  processGyroData();
}

void processGyroData() {
  rotX = gyroX / 131.0 / conversionrad;
  rotY = gyroY / 131.0 / conversionrad; 
  rotZ = gyroZ / 131.0 / conversionrad;
}

void printData() {
  Serial.print("Gyro (deg)");
  Serial.print(" X=");
  Serial.print(rotX);
  Serial.print(" Y=");
  Serial.print(rotY);
  Serial.print(" Z=");
  Serial.print(rotZ);
  Serial.print(" Accel (g)");
  Serial.print(" X=");
  Serial.print(gForceX);
  Serial.print(" Y=");
  Serial.print(gForceY);
  Serial.print(" Z=");
  Serial.println(gForceZ);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 void plotData(){
  Serial.print(linThresh);
  Serial.print(" ");
  Serial.print(rotY);
  Serial.print(" ");
  Serial.print(gForceX);
  Serial.print(" ");
  Serial.print(rotX);
  Serial.print(" ");
  Serial.print(rotZ);
  Serial.print(" ");
  Serial.print(gForceY);
  Serial.print(" ");
  Serial.print(gForceZ);
  Serial.print(" ");
  Serial.print(rotThresh);
  Serial.print(" ");
  Serial.println(linThreshZ);
  }
/*
void plotData() {
  plotgFX();
  plotgFY();
  plotgFZ();
  plotrotX();
  plotrotY();
  plotrotZ();
  plotlinThresh();
  plotlinThreshZ();
  plotrotThresh();
}

void plotgFX(){
  Serial.println(gForceX);
  }

void plotgFY(){
  Serial.println(gForceY);
  }

void plotgFZ(){
  Serial.println(gForceZ);
  }
  
void plotrotX(){
  Serial.println(rotX);
  }

void plotrotY(){
  Serial.println(rotY);
  }

void plotrotZ(){
  Serial.println(rotZ);
  }
  
void plotlinThresh(){
  Serial.println(linThresh);
  }

void plotlinThreshZ(){
  Serial.println(linThreshZ);
  }

void plotrotThresh(){
  Serial.println(rotThresh);
  }
*/
//+++++++++++++++++++++++++++++++++++++++++++++
void hit_counter() {
  if ((( rotX or rotY or rotZ) > rotThresh) or ((rotX or rotY or rotZ)) < -rotThresh){
    hits_counter += 1;
  }
/*  if ((( gForceX or gForceY) > linThresh)or ((gForceX or gForceY) < -linThresh)){
    hits_counter += 1;
  }
  if ((gForceZ > linThreshZ)or (gForceZ < -linThreshZ)){
    hits_counter += 1;
    }*/
  if (/*(hits_counter % 5) == 0 and*/ hits_counter != 0) { 
    Serial.print("WARNING: REPEATED HITS TAKEN");
    Serial.print("PLAYER, HITS TAKEN:");
    Serial.print(hits_counter);
    Serial.println();
    delay(15);
  }
}  

void checkDiag(){
  if ((gForceX > linThresh) or (gForceX < -linThresh)){
    Serial.println("CONCUSSED in X Lin");
    }
  if ((gForceY > linThresh) or (gForceY < -linThresh)){
    Serial.println("CONCUSSED in Y Lin");
    }
  if ((gForceZ > linThreshZ) or (gForceZ < -linThreshZ)){
    Serial.println("CONCUSSED in Z Lin");
    }
  if ((rotX > rotThresh) or (rotX < -rotThresh)){
    Serial.println("CONCUSSED in X Rot");
    }  
  if ((rotY > rotThresh) or (rotY < -rotThresh)){
    Serial.println("CONCUSSED in Y Rot");
    }
  if ((rotZ > rotThresh) or (rotX < -rotThresh)){
    Serial.println("CONCUSSED in Z Rot");
    }    
  }

//+++++++++++++
//Arrays //lists of "saved" force snapshots
float gForceXArray[15];
float gForceYArray[15];
float gForceZArray[15];
float rotXArray[15];
float rotYArray[15];
float rotZArray[15];

float gFXAsum = 0;
float gFYAsum = 0;
float gFZAsum = 0;
float rXAsum = 0;
float rYAsum = 0;
float rZAsum = 0;


void takeAvg(){
  int n = avgCount;
  if (n < 14){
    gForceXArray[n] = gForceX;
    gForceYArray[n] = gForceY;
    gForceZArray[n] = gForceZ;
    rotXArray[n] = rotX;
    rotYArray[n] = rotY;
    rotZArray[n] = rotZ;
    avgCount = avgCount + 1;
    }
  else {
    for (int i = 0; i <= (numMs - 1); i++){
      gFXAsum += gForceXArray[i];
      gFYAsum += gForceYArray[i];
      gFZAsum += gForceZArray[i];
      rXAsum += rotXArray[i];
      rYAsum += rotYArray[i];
      rZAsum += rotZArray[i];
      }

      float gForceX = (gFXAsum/numMs)*2.5;
      float gForceY = (gFYAsum/numMs)*2.5;
      float gForceZ = (gFZAsum/numMs)*2.5;
      float rotX = (rXAsum/numMs)*2.5;
      float rotY = (rYAsum/numMs)*2.5;
      float rotZ = (rZAsum/numMs)*2.5;
      avgCount = 0;}
  }
