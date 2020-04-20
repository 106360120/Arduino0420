#include <Wire.h>
#include <TimerOne.h>
#define SLAVE_ADDRESS 0x04    // 設定Arduino開發板I2C的位址
#define A1 31
#define A2 32
#define A3 33
#define A4 34
#define B1 35
#define B2 36
#define C1 37
#define C2 38
#define C3 39
#define C4 40
#define EchoPin 41  // 超音波模組的接收腳
#define TrigPin 42  // 超音波模組的觸發腳
#define Takepicture 43 //拍照顯示燈

int number = 0;
volatile boolean CarCheck_state = 1;  // 儲存超音波感測器偵測車體的狀態

//const byte A = 24;  // 馬達A的致能接腳
//const byte pin1 = 22;  // 馬達A的正反轉接腳
//const byte pin3 = 23;  // 馬達A的正反轉接腳
//const byte speed = 200;


long duration, cm, inches;
long ToStart = 0;
const long interval = 1000*1000/0.016959999; //  1 sec


void setup() {
  //pinMode(pin1, OUTPUT);    // 觸發腳設定成「輸出」
  //pinMode(pin3, OUTPUT);    // 接收腳設定成「輸入」
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(B1, OUTPUT);
  pinMode(B2, OUTPUT);
  pinMode(C1, OUTPUT);
  pinMode(C2, OUTPUT);
  pinMode(C3, OUTPUT);
  pinMode(C4, OUTPUT);
      
  pinMode(TrigPin, OUTPUT);    // 觸發腳設定成「輸出」
  pinMode(EchoPin, INPUT);    // 接收腳設定成「輸入」
  pinMode(Takepicture, OUTPUT); 
  
  //attachInterrupt(digitalPinToInterrupt(2), CarCheck_state, RISING);   // 
  Serial.begin(9600);   // Serial通訊埠通訊設為9600速率
  Wire.begin(SLAVE_ADDRESS);  // 初始化Arduino的I2C位址

  //Wire.onReceive(receiveData); //I2C訊號接收時，啟用函式
  Wire.onRequest(sendData);  //主機要求讀取時，啟用函式

  Serial.println("Ready!");
  Timer1.initialize(interval); // 初始化,以 micro sec 為單位
  
 
}

void loop() {
  
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(TrigPin, HIGH);     // 給 Trig 高電位，持續 10微秒
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  pinMode(EchoPin, INPUT);             // 讀取 echo 的電位
  duration = pulseIn(EchoPin, HIGH);   // 收到高電位時的時間
 
  cm = (duration/2) / 29.1;         // 將時間換算成距離 cm 或 inch  
  inches = (duration/2) / 74; 

  Serial.print("Distance : ");  
  Serial.print(inches);
  Serial.print("in,   ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.print(ToStart);
  Serial.println();
  delay(1000);
  
  if(cm<5){
    CarCheck_state = 0;
    Timer1.attachInterrupt(CarCheck); // 設定讓定時器每隔1秒，自動執行CarCheck函數
  }else{
    CarCheck_state = 1;
  }
  
  /*if(CarCheck_state == 0){
    sendData();
    Serial.print("send");
    //
    CarCheck_state = 1;
    
  }*/

    
}

// callback for received data
/*void receiveData(int byteCount){
while(Wire.available()) {  //當I2C的buffer中有資料時進入迴圈
  number = Wire.read();   //指定nubmer 等於讀取的訊息
  Serial.print("data received: ");
  Serial.println(number);

  if (number == 5){  
    if(state == 0){
      digitalWrite(A1, HIGH);
      state = 1;
    }else{
      digitalWrite(A1, LOW);
      state = 0;
    }

  }
 }
}*/

// callback for sending data
void sendData(){
  Wire.write(10);
  
}

//讀取是否有車子
void CarCheck(){
  switch(CarCheck_state)
  case 0:
      Serial.println("do");
        ToStart++;
        if(ToStart>4){
          digitalWrite(Takepicture,HIGH);
          CarCheck_state = 1;
          ToStart=0;
        }
          break;
   case 1:
        digitalWrite(Takepicture,LOW);
        ToStart=0;
   break;
}
