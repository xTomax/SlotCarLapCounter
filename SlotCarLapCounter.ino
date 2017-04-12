// This is a demo of the RBBB running as webserver with the Ether Card
// 2010-05-28 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

#include <EtherCard.h>

// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
//static byte myip[] = { 192,168,4,4 };

byte Ethernet::buffer[500];
BufferFiller bfill;
const byte inPin4 = 7;
const byte inPin3 = 6;
const byte inPin2 = 5;
const byte inPin1 = 4;

unsigned long readMillis = 0;
//Position 0 will be reserved on the array. It helps with the counters.
unsigned long track[4][20] = {
                                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                             };
String stringResults[4];
char track1String[300];
char track2String[300];
char track3String[300];
char track4String[300];
//Need to remove 1 on this to display the proper trackcounter
unsigned int trackcounter[4] = {0,0,0,0};

//Keeps the temporary read of every track at a moment.
byte reader[4] = {0,0,0,0};

void setup () {
  Serial.begin(115200);
  if (ether.begin(sizeof Ethernet::buffer, mymac, 3) == 0)
    Serial.println(F("Failed to access Ethernet controller"));
  else
    Serial.println(F("Ethernet controller OK!"));
  
  Serial.println(F("Setting up DHCP"));
  if (!ether.dhcpSetup())
    Serial.println(F("DHCP failed"));
  
  ether.printIp("My IP: ", ether.myip);
  ether.printIp("Netmask: ", ether.netmask);
  ether.printIp("GW IP: ", ether.gwip);
  ether.printIp("DNS IP: ", ether.dnsip);
  pinMode(inPin1, INPUT);   // initialize the button pin as a input
  pinMode(inPin2, INPUT);   // initialize the button pin as a input
  pinMode(inPin3, INPUT);   // initialize the button pin as a input
  pinMode(inPin4, INPUT);   // initialize the button pin as a input
  Serial.println("Setup Done!");
}

static word homePage() {
  /**long t = millis() / 1000;
  word h = t / 3600;
  byte m = (t / 60) % 60;
  byte s = t % 60;
  */
  stringResults[0].toCharArray(track1String,280);
  stringResults[1].toCharArray(track2String,280);
  stringResults[2].toCharArray(track3String,280);
  stringResults[3].toCharArray(track4String,280);
  bfill = ether.tcpOffset();
  bfill.emit_p(PSTR(
    "HTTP/1.0 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Pragma: no-cache\r\n"
    "\r\n"
    "<meta http-equiv='refresh' content='1'/>"
    "<title>SlotCar TrackCounter Arduino</title>" 
    "<table><tbody><tr><td>"
    "<h1>Track 1 Laps:</h1>"
    "$F"
    "| |</td><td>| |"
    "<h1>Track 2 Laps:</h1>"
    "$F"
    "</td></tr><tr><td>"
    "<h1>Track 3 Laps:</h1>"
    "$F"
    "| |</td><td>| |"
    "<h1>Track 4 Laps:</h1>"
    "$F"
    "</td></tr></tbody></table>"),"track1String","track2String","track3String","track4String");
  return bfill.position();
}

void loop () {
  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);
  
  if (pos)  // check if valid tcp data is received
    ether.httpServerReply(homePage()); // send web page data


  readAll();
  readMillis = millis();
  for (int i=0;i<4;i++){
    if (reader[i]==1){
      trackcounter[i]+=1;
      track[i][trackcounter[i]]= readMillis - trackcounter[i-1];
      buildStringResults();
    }
  }
  //printTracks();
}

void readAll(){
    //Not that much clean, but get the job done quick (I guess)
    reader[0] = digitalRead(inPin1);
    reader[1] = digitalRead(inPin1);
    reader[2] = digitalRead(inPin1);
    reader[3] = digitalRead(inPin1);
}
void buildStringResults(){
  for (int j=0;j<4;j++){
    for (int i=0;i<20;i++){
      char buf[16];
      sprintf(buf, "<h2>%lu</h2>", track[j][i]);
      stringResults[j]+=buf;
    }
  }
}
