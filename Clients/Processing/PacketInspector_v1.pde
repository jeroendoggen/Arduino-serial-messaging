String typedText = "";
 
PFont font;
import processing.serial.*;

Serial myPort;  // Create object from Serial class

int portNumber = 0;
byte[] inBuffer = new byte[17];

byte msgType;
byte nodeID;
byte sensorID;
byte payload;
byte parity;
byte command;

int lf=10;

void setup() {
  size(440, 80);
  font = createFont("Arial-Black-48.vlw", 10);
  
  String portName = Serial.list()[portNumber];
  myPort = new Serial(this, portName, 115200);
  myPort.bufferUntil(lf); 
  myPort.clear();
  
  background(245);
}

 
void draw() {
  drawduobutton(10,10,220,0,"","Hex","Decimal");
  drawduobutton(80,10,220,0,"MsgType",msgType,int(msgType));
  drawduobutton(150,10,220,0,"NodeID",nodeID,int(nodeID));
  drawduobutton(220,10,220,0,"SensorID",sensorID,int(sensorID));
  drawduobutton(290,10,220,0,"Payload",payload,int(payload));
  drawduobutton(360,10,220,0,"Parity",parity,int(parity));
}

void drawduobutton(int right, int down, int bgcolor, int textcolor, String buttontext, byte dataByte,int buttondata){
drawbutton(right, down, bgcolor, textcolor, buttontext,70);
drawbutton(right, down+20, bgcolor+30, textcolor, dataByte,70);
drawbutton(right, down+40, bgcolor+30, textcolor, buttondata,70);
}

void drawduobutton(int right, int down, int bgcolor, int textcolor, String buttontext1, String buttontext2, String buttontext3){
drawbutton(right, down, bgcolor, textcolor, buttontext1,70);
drawbutton(right, down+20, bgcolor+30, textcolor, buttontext2,70);
drawbutton(right, down+40, bgcolor+30, textcolor, buttontext3,70);
}

void drawbutton(int right, int down, int bgcolor, int textcolor, String buttontext, int width){
  int height=20;
  if(bgcolor==1) fill(255,0,0);
  else if(bgcolor==2)fill(0,255,0);
  else if(bgcolor==3)fill(0,0,255);
  else fill(bgcolor);
  
  rect(right, down, width, height);
  textAlign(CENTER);
  fill(textcolor);
  text(buttontext, right+(width/2), down+(height/1.2));
}

void drawbutton(int right, int down, int bgcolor, int textcolor, int buttondata, int width){
  int height=20;
  if(bgcolor==1) fill(255,0,0);
  else if(bgcolor==2)fill(0,255,0);
  else if(bgcolor==3)fill(0,0,255);
  else fill(bgcolor);
  
  rect(right, down, width, height);
  textAlign(CENTER);
  fill(textcolor);
  text(int(buttondata), right+(width/2), down+(height/1.2));
}

void drawbutton(int right, int down, int bgcolor, int textcolor, byte buttondata, int width){
  int height=20;
  if(bgcolor==1) fill(255,0,0);
  else if(bgcolor==2)fill(0,255,0);
  else if(bgcolor==3)fill(0,0,255);
  else fill(bgcolor);
  
  rect(right, down, width, height);
  textAlign(CENTER);
  fill(textcolor);
  text(hex(buttondata), right+(width/2), down+(height/1.2));
}

void serialEvent(Serial thisPort) {
  
  while (thisPort.available() > 0) {
    int byteCount = thisPort.readBytesUntil(lf, inBuffer);
  }
  
  msgType=byte(Character.getNumericValue(inBuffer[1])*16 + Character.getNumericValue(inBuffer[2]));
  nodeID=byte(Character.getNumericValue(inBuffer[4])*16 + Character.getNumericValue(inBuffer[5]));
  sensorID=byte(Character.getNumericValue(inBuffer[7])*16 + Character.getNumericValue(inBuffer[8]));
  payload=byte(Character.getNumericValue(inBuffer[10])*16 + Character.getNumericValue(inBuffer[11]));
  parity=byte(Character.getNumericValue(inBuffer[13])*16 + Character.getNumericValue(inBuffer[14]));
}
