#include <ESP8266WiFi.h>        //����������� ����������
#include <PubSubClient.h>

// ���������� ����������
String _ssid     = "TP-Link_1F20";                       // �������� WiFi
String _password = "60354341";                    // ������ �� WiFi 

const char *mqtt_server = "broker.hivemq.com"; // ��� ������� MQTT
const int mqtt_port =   1883; // ���� ��� ����������� � ������� MQTT
const char *mqtt_user = "4d78899845734a858cdfc2e436f22b29.s1.eu.hivemq.cloud"; // ���� �� ������
const char *mqtt_pass = ""; // ������ �� �������
int sum_led = 0;
int time_send = millis();

//�������� ����� �������
int SEC = 0;
int MIN = 0;
unsigned long timer;

const int motionSensor = D1;                                      // PIR ������ �����
bool motionDetected = false ;//

const int motionSensor1 = D2;                                      // PIR ������2 �������
bool motionDetected1 = false;//

const int motionSensor2 = D3;                                      // PIR ������2 ��� �������� ��������
bool motionDetected2 = false;//

const int motionSensor3 = D4;                                      // PIR ������2 ������ ���
bool motionDetected3 = false;//

String motor = "0" ;//
String yroven = "0" ;//
String presure = "0" ;//
String RKF = "0" ;//


bool skid = false ;//

//char OCMOCMS[6] = {'h', 'e', 'l', 'l', 'o'};

 

void callback(const MQTT::Publish& pub)
{
  Serial.print(pub.topic());   // ������� � ������ ���� �������� ������
  Serial.print(" => ");
  Serial.print(pub.payload_string()); // ������� � ������ ���� �������� ���������� ������
  
  String payload = pub.payload_string();
  
  if(String(pub.topic()) == "Led") // ��������� �� ������� �� ��� ������ ������ ������ 
  {
  int stled = payload.toInt(); // ����������� ���������� ������ � ��� integer
  /*if (stled == 1){
    digitalWrite(13, HIGH); 
  }else{digitalWrite(13, LOW); }
  digitalWrite(13,stled);*/  //  �������� ��� ��������� ��������� � ���������� �� ���������� �������� ������
 
  sum_led=sum_led+1;

  
  }
}







WiFiClient wclient;      
PubSubClient client(wclient, mqtt_server, mqtt_port);


void setup() {
  Serial.begin(115200); //������������� ��������
  Serial.println("");


 pinMode(motionSensor, INPUT_PULLUP);                          // ���������� �������� PIR  INPUT_PULLUP
 pinMode(motionSensor1, INPUT_PULLUP);  
 pinMode(motionSensor2, INPUT_PULLUP);
 pinMode(motionSensor3, INPUT_PULLUP); 

    
 // pinMode(13, OUTPUT);
  
   // ----------------����������� � WiFi-----------))
  WiFi.mode(WIFI_STA);                  
  WiFi.begin(_ssid.c_str(), _password.c_str());      //�������  ������������ � WiFi
  while ( WiFi.status() != WL_CONNECTED){            //���� ����� �� ���������� � ���� ���������:
   delay(1000);                                      //���� 1 ���
   Serial.print("*");                                //   ������� �� ����� ���������
} 
  if (WiFi.status() == WL_CONNECTED){                //��������� ������ ���������� � ����, ���� ����� ������������, ��:
    Serial.println("");
    Serial.println("WiFi connected");                //��������� �� ����;
    Serial.println("IP address: ");                  //������� IP �����..
    Serial.println(WiFi.localIP());                             //..����� �����
  }
  //--------------------------------------------------//
  
}






void loop() {
  //---------����������� � MQTT ------------------------//
    if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      Serial.println("Connecting to MQTT server");
      if (client.connect(MQTT::Connect("arduinoClient2")
                         .set_auth(mqtt_user, mqtt_pass))) {

                          
        Serial.println("Connected to MQTT server");
        client.set_callback(callback);
        client.subscribe("Led"); // �������������� �� ����� � ������� ��� ����������
        
      } else {
        Serial.println("Could not connect to MQTT server");   
      }
    }

    if (client.connected()){
      client.loop();
      Send();
  }
  
}






//������� ���� ������

 
bool btnState = !digitalRead(D1);
   bool btnState1 = !digitalRead(D2);
   bool btnState2 = !digitalRead(D3);
   bool btnState3 = !digitalRead(D4);


  if (btnState && !motionDetected) {  // ���������� �������
    motionDetected = true;
    Serial.println("press");
  //  bot.sendMessage(CHAT_ID, "�����_������", "");
   Serial.print("�����_������");
    motor = "1" ;//
    if (skid){
//bot.sendMessage(CHAT_ID, "����� ��೿", "");
    skid = false ;
    }
    

  }
  if (!btnState && motionDetected) {  // ���������� ����������
    motionDetected = false;  
     //bot.sendMessage(CHAT_ID, "�����_����", "");
       motor = "0" ;//
       Serial.print("�����_����");
  }

//�������
  if (btnState1 && !motionDetected1) {  // ���������� �������
    motionDetected1 = true;
    Serial.println("press");
  //  bot.sendMessage(CHAT_ID, "���_����", "");
  Serial.print("���_����");
     yroven = "1" ;//
  }
  if (!btnState1 && motionDetected1) {  // ���������� ����������  ��� �� ���� � ����� ���� = ���� (� �����)
    motionDetected1 = false;  
     //bot.sendMessage(CHAT_ID, "���_��_����", "");
      Serial.print("���_������");
      yroven = "0" ;//
  }

//���
   if (btnState3 && !motionDetected3) {  // ���������� �������
    motionDetected3 = true;
    RKF = "1" ;
    Serial.println("press");
    //bot.sendMessage(CHAT_ID, "������ ���", "");
  }
  if (!btnState3 && motionDetected3) {  // ���������� ����������
    motionDetected3 = false;  
    RKF = "0" ;
    // bot.sendMessage(CHAT_ID, "������� �", "");
  }




//��� ��� ����

 if (btnState2 && !motionDetected2) {  // ���������� �������
    motionDetected2 = true;
    Serial.println("press");
    presure = "1" ;
   // bot.sendMessage(CHAT_ID, "���_���_����", "");
  }
  if (!btnState2 && motionDetected2) {  // ���������� ����������
    motionDetected2 = false;  
    presure = "0" ;
    // bot.sendMessage(CHAT_ID, "���_���������������", "");
  }






  /*
   if (!yroven && !motor) {  // ����� �� �������  ����� ����
   
     if (millis() - timer > 1000) {
      timer = millis();
      SEC = SEC + 1;
      if (SEC > 600) { SEC = 0; MIN = MIN + 1;
      // bot.sendMessage(CHAT_ID, "������ (��� �� ����, ����� ����)", "");
       skid = true ;
       }
      // ������� ������� ������� ������� �� ������� �����
    // else
     // SEC = 0;
   }
  }

*/









//����� ���� ������


}



// ������� �������� ��������� 
void Send(){
  if ((millis() - time_send)>1000){
    
  
  client.publish("SumLed",String(sum_led)); // ���������� � ����� ��� ���������� ���������� ���������
  //Serial.println(String(sum_led));
  delay(10); 

client.publish("MOTOR_S",String(motor)); // ���������� � ����� ��� ���������� ���������� ���������
//  Serial.println(String(sum_led));
 delay(10);

client.publish("LEVEL_S",String(yroven)); // ���������� � ����� ��� ���������� ���������� ���������
  Serial.println(String(yroven));
  delay(10);

client.publish("PRESURE_S",String(presure)); // ���������� � ����� ��� ���������� ���������� ���������
  Serial.println(String(yroven));
  delay(10);

client.publish("RKF_S",String(RKF)); // ���������� � ����� ��� ���������� ���������� ���������
  Serial.println(String(RKF));
  delay(10);

  
  time_send = millis();
  } 
}