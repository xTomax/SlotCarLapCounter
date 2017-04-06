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

//Need to remove 1 on this to display the proper trackcounter
unsigned int trackcounter[4] = {0,0,0,0};

//Keeps the temporary read of every track at a moment.
byte reader[4] = {0,0,0,0};

void setup() {
  Serial.begin(115200);         // Use serial for debugging
 
  pinMode(inPin1, INPUT);   // initialize the button pin as a input
  pinMode(inPin2, INPUT);   // initialize the button pin as a input
  pinMode(inPin3, INPUT);   // initialize the button pin as a input
  pinMode(inPin4, INPUT);   // initialize the button pin as a input
}

void loop() {
  //TODO: The reader must only start counting after the trackCounter > 0
  readAll();
  readMillis = millis();
  for (int i=0;i<4;i++){
    if (reader[i]==1){
      trackcounter[i]+=1;
      track[i][trackcounter[i]]= readMillis - trackcounter[i-1];
    }
  }
  printTracks();
}

void readAll(){
    //Not that much clean, but get the job done quick (I guess)
    reader[0] = digitalRead(inPin1);
    reader[1] = digitalRead(inPin1);
    reader[2] = digitalRead(inPin1);
    reader[3] = digitalRead(inPin1);
}
void printTracks(){
  
  Serial.println("Track 1 Laps\t\t\t\tTrack 2 Laps");
 
  for (int i=0;i<20;i++){
    Serial.print(track[0][i]);
    Serial.print("\t\t\t\t");
    Serial.println(track[1][i]);
  }
  
  Serial.println("Track 3 Laps\t\t\t\tTrack 4 Laps");
  for (int i=0;i<20;i++){
    Serial.print(track[2][i]);
    Serial.print("\t\t\t\t");
    Serial.println(track[3][i]);
  }
}

// I'm putting text "Rangefiders"  at row 5, col 1. And moving cursor
// to column 16 to output next text there.
//Serial.print("\e[5;1HRangefinder: \e[5;16H");

