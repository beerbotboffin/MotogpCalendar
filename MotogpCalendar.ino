/*


     MODIFICATIONS
     ==================
     20200220 SF: Changed the daysTillNextRace method so that we cover a race weekend
     20210219 SF:
*/

//#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <WiFi.h>
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 rtc;

// This will need to change to the mobile access point in production
//const char* ssid     = "WiFi-2BAC";        // The SSID (name) of the Wi-Fi network you want to connect to
char ssid[]     = "WiFi-2BAC";        // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "10810491";        // The password of the Wi-Fi network
//String ssid = "WiFi-2BAC";
const char* host = "worldtimeapi.org";


//23-02-2021
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//String raceDates[] = {"20210302", "20210306", "20210319", "20210328", "20210404", "20210412", "20210418", "20210502", "20210516", "20210530", "20210606", "20210620", "20210627", "20210711", "20210815", "20210829", "20210912", "20210919", "20211003", "20211010", "20211024", "20211031", "20211114", "202100PPD", "202100PPD", "202100RES"};
//String circuitTLA[] = {"Jer", "Los", "Los", "Los", "Los", "Jer", "Alg", "Jer", "LMa", "Mug", "Cat", "Sac", "Ass", "Kym", "RBR", "Sil", "Ara", "Mis", "Mot", "Cha", "PIs", "Sep", "Ric", "Hon", "Ame", "Man"};
//String circuitAbbreviation[] = {" Losail"," Losail","Portugal","Spain"," France","Italy","Spain","Germany","Netherlands","Finland","Austria","Great Britain","Spain","Italy","Japan","Thailand","Australia","Malaysia"," Spain"};
//String countries[] = {"SPAIN", "QATAR", "QATAR", "QATAR", "QATAR", "SPAIN", "PORTUGAL", "SPAIN", "FRANCE", "ITALY", "SPAIN", "GERMANY", "NETHERLANDS", "FINLAND", "AUSTRIA", "GREAT BRITAIN", "SPAIN", "ITALY", "JAPAN", "THAILAND", "AUSTRALIA", "MALAYSIA", "SPAIN", "ARGENTINA", "UNITED STATES", "INDONESIA"};
//String circuits[] = {"Circuito de Jerez - Angel Nieto", "Losail International Circuit", "Losail International Circuit", "Losail International Circuit", "Losail International Circuit", "Circuito de Jerez - Angel Nieto", "Algarve International Circuit", "Circuito de Jerez - Angel Nieto", "Le Mans", "Autodromo Internazionale del Mugello", "Circuit de Barcelona-Catalunya", "Sachsenring", "TT Circuit Assen", "KymiRing", "Red Bull Ring - Spielberg", "Silverstone Circuit", "MotorLand Aragon", "Misano World Circuit Marco Simoncelli", "Twin Ring Motegi", "Chang International Circuit", "Phillip Island", "Sepang International Circuit", "Circuit Ricardo Tormo", "Termas de Rio Hondo", "Circuit Of The Americas", "Mandalika International Street Circuit"};
//String raceName[] = {"Jerez MotoE Official Test 1", "Qatar MotoGP Official Test", "Qatar Moto2 & Moto3 Official Test", "Grand Prix of Qatar", "Grand Prix of Doha", "Jerez MotoE Official Test 2", "Grande Prmio de Portugal", "Gran Premio Red Bull de Espaa", "SHARK Helmets Grand Prix de France", "Gran Premio d'Italia Oakley", "Gran Premi Monster Energy de Catalunya", "Liqui Moly Motorrad Grand Prix Deutschland", "Motul TT Assen", "Grand Prix of Finland", "Motorrad Grand Prix von sterreich", "British Grand Prix", "Gran Premio de Aragn", "Gran Premio Octo di San Marino e della Riviera di Rimini", "Motul Grand Prix of Japan", "OR Thailand Grand Prix", "Australian Motorcycle Grand Prix", "Malaysia Motorcycle Grand Prix", "Gran Premio Motul de la Comunitat Valenciana", "Gran Premio de la Repblica Argentina", "Grand Prix of the Americas"};

String raceDates[] = {"20221118", "20220205", "20220211", "20220219", "20220306", "20220320", "20220403", "20220410", "20220424", "20220501", "20220502", "20220515", "20220529", "20220605", "20220606", "20220619", "20220626", "20220807", "20220821", "20220904", "20220906", "20220918", "20220925", "20221002", "20221016", "20221023", "20221106"};
String circuitTLA[] = {"JER", "SEP", "MAN", "POR", "QAT", "IND", "ARG", "COT", "POR", "ESP", "JER", "FRA", "ITA", "CAT", "CTT", "GER", "ASS", "GBR", "AUS", "RIM", "MIS", "ARA", "JAP", "THA", "AUS", "MAL", "VAL"};
String circuitAbbreviation[] = {"Jerez", "Sepang", "Mandalika", "Portimao", "Qatar", "Pertamina", "Argentina", "Americas", "Portugal", "Red Bull", "Jerez", "LeMANS", "Mugello", "Catalunya", "Catalunya", "Deutschland", "Assen", "SILVERSTONE", "RED BULL", "MISANO", "MISANO TEST", "ARAGON", "MOTEGI", "THAILAND", "PHILLIP ISLAND", "MALAYSIA", "Valenciana"};
String countries[] = {"SPAIN", "MALAYSIA", "INDONESIA", "PORTUGAL", "QATAR", "INDONESIA", "ARGENTINA", "UNITED STATES", "PORTUGAL", "SPAIN", "SPAIN", "FRANCE", "ITALY", "SPAIN", "SPAIN", "GERMANY", "NETHERLANDS", "GREAT BRITAIN", "AUSTRIA", "ITALY", "ITALY", "SPAIN", "JAPAN", "THAILAND", "AUSTRALIA", "MALAYSIA", "SPAIN"};
String circuits[] = {"Circuito de Jerez - Angel Nieto", "Losail International Circuit", "Losail International Circuit", "Losail International Circuit", "Losail International Circuit", "Circuito de Jerez - Angel Nieto", "Algarve International Circuit", "Circuito de Jerez - Angel Nieto", "Le Mans", "Autodromo Internazionale del Mugello", "Circuit de Barcelona-Catalunya", "Sachsenring", "TT Circuit Assen", "KymiRing", "Red Bull Ring - Spielberg", "Silverstone Circuit", "MotorLand Aragon", "Misano World Circuit Marco Simoncelli", "Twin Ring Motegi", "Chang International Circuit", "Phillip Island", "Sepang International Circuit", "Circuit Ricardo Tormo", "Termas de Rio Hondo", "Circuit Of The Americas", "Mandalika International Street Circuit"};
String raceName[] = {"Jerez MotoE Official Test 1", "Qatar MotoGP Official Test", "Qatar Moto2 & Moto3 Official Test", "Grand Prix of Qatar", "Grand Prix of Doha", "Jerez MotoE Official Test 2", "Grande Prmio de Portugal", "Gran Premio Red Bull de Espaa", "SHARK Helmets Grand Prix de France", "Gran Premio d'Italia Oakley", "Gran Premi Monster Energy de Catalunya", "Liqui Moly Motorrad Grand Prix Deutschland", "Motul TT Assen", "Grand Prix of Finland", "Motorrad Grand Prix von sterreich", "British Grand Prix", "Gran Premio de Aragn", "Gran Premio Octo di San Marino e della Riviera di Rimini", "Motul Grand Prix of Japan", "OR Thailand Grand Prix", "Australian Motorcycle Grand Prix", "Malaysia Motorcycle Grand Prix", "Gran Premio Motul de la Comunitat Valenciana", "Gran Premio de la Repblica Argentina", "Grand Prix of the Americas"};

String dateToday = ""; 
String nextSession = "";

int Delay = 30000;
    
int dataIn = 15;
int load = 12;
int clockPin = 13;

int i = 0;
int e = 0;
int f = 0;
int c = 0;
int c2= 0;

int maxInUse = 8;    //here you have to change this varialbe to how many max 7219 you want to use
long randNumber;

 // define max7219 registers
byte max7219_reg_noop        = 0x00;
byte max7219_reg_digit0      = 0x01;
byte max7219_reg_digit1      = 0x02;
byte max7219_reg_digit2      = 0x03;
byte max7219_reg_digit3      = 0x04;
byte max7219_reg_digit4      = 0x05;
byte max7219_reg_digit5      = 0x06;
byte max7219_reg_digit6      = 0x08;
byte max7219_reg_digit7      = 0x01;
byte max7219_reg_decodeMode  = 0x09;
byte max7219_reg_intensity   = 0x0a;
byte max7219_reg_scanLimit   = 0x0b;
byte max7219_reg_shutdown    = 0x0c;
byte max7219_reg_displayTest = 0x0f;

byte A[] ={24,36,66,66,126,66,66,66};
byte B[] ={62,66,66,62,66,66,66,62};
byte C[] ={60,66,2,2,2,2,66,60};
byte D[] ={30,34,66,66,66,66,34,30};
byte E[] ={126,2,2,2,30,2,2,126};
byte F[] ={126,2,2,30,2,2,2,2};
byte G[] ={60,66,2,2,98,66,66,124};
byte H[] ={66,66,66,66,126,66,66,66};
byte I[] ={62,8,8,8,8,8,8,62};
byte J[] ={254,16,16,16,16,16,18,12};
byte K[] ={34,18,10,6,10,18,34,66};
byte L[] ={2,2,2,2,2,2,2,126};
byte M[] ={130,198,170,146,130,130,130,130};
byte N[] ={66,66,70,74,82,98,66,66};
byte O[] ={60,66,66,66,66,66,66,60};
byte P[] ={62,66,66,66,62,2,2,2};
byte Q[] ={60,66,66,66,74,82,98,60};
byte R[] ={62,66,66,66,62,18,34,66};
byte S[] ={60,66,2,60,64,64,66,60};
byte T[] ={127,8,8,8,8,8,8,8};
byte U[] ={66,66,66,66,66,66,66,60};
byte V[] ={130,130,130,130,68,68,40,16};
byte W[] ={66,66,66,66,66,90,102,66};
byte X[] ={66,66,36,24,36,66,66,66};
byte Y[] ={34,34,34,20,8,8,8,8};
byte Z[] ={126,64,32,16,8,4,2,126};
byte fullstop[] ={0,0,0,0,0,0,24,24};
byte n1[] ={16,24,16,16,16,16,16,56}; // 20200220 SF: Changed to closely match the lift numerals at Mineral House (though they use 5 x 7)
byte n2[] ={56,68,64,32,16,8,4,124};
byte n3[] ={56,68,64,56,64,64,68,56};
byte n4[] ={32,48,40,36,124,32,32,32};
byte n5[] ={124,4,4,60,64,64,68,56};
byte n6[] ={56,68,4,60,68,68,68,56};
byte n7[] ={124,64,64,32,16,16,16,16};
byte n8[] ={56,68,68,56,68,68,68,56};
byte n9[] ={56,68,68,120,64,64,68,56};
byte n0[] ={56,68,68,68,68,68,68,56};
byte slash[] ={128,64,32,16,8,4,2,1};
byte colon[] ={0,24,24,0,0,0,24,24};


String line;



void maxIni (byte reg, byte col) {    // put the initialisation to all the max 7219 in the system
     int c = 0;
     digitalWrite(load, HIGH);
     for ( c =1; c<= maxInUse; c++) {
          maxPut(reg, col);   // use all 8 columns
     }
     digitalWrite(load, LOW);
     digitalWrite(load,HIGH);
}

void putByte(byte data) {
     byte i = 8;
     byte mask;
     while(i > 0) {
          mask = 0x01 << (i - 1);  // get bitmask
          digitalWrite(clockPin, LOW);   // tick
          if (data & mask){        // choose bit
          digitalWrite(dataIn, HIGH); // send 1
     }else{
          digitalWrite(dataIn, LOW);  // send 0
     }
     
     digitalWrite(clockPin, HIGH);  // tock
     --i;                     // move to lesser bit
     }
}

void maxPut (byte reg, byte col) {    // use this
     //data = ((data & 0x01) * 256) + data1 >> 1;
     //data = (data << 1) + ((data >> 7) & 0x01);
     putByte(reg);  // specify register
     putByte(col);//((data & 0x01) * 256) + data >> 1); // put data
}

int dateDiff(int year1, int mon1, int day1, int year2, int mon2, int day2)
{
    int ref,dd1,dd2,i;
    ref = year1;
    if(year2<year1)
    ref = year2;
    dd1=0;
    dd1=dater(mon1);
    for(i=ref;i<year1;i++)
    {
        if(i%4==0)
        dd1+=1;
    }
    dd1=dd1+day1+(year1-ref)*365;
    dd2=0;
    for(i=ref;i<year2;i++)
    {
        if(i%4==0)
        dd2+=1;
    }
    dd2=dater(mon2)+dd2+day2+((year2-ref)*365);
    return dd2-dd1;
}

int dater(int x)
{ const int dr[]= { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
  return dr[x-1];
}

int getByteValue(String Character, int pos){
     char b = Character.charAt(0);
  
     if(int(b) >= 97){ // Convert lowercase ASCII to uppercase
          b = b - 32;
     }

     switch (int(b)) {
     case 32:
          return 0;
          break;
     case 46:
          return fullstop[pos];
          break;
     case 47:
          return slash[pos];
          break;          
     case 48:
          return n0[pos];
          break;          
     case 49:
          return n1[pos];
          break;
     case 50:
          return n2[pos];
          break;
     case 51:
          return n3[pos];
          break;
     case 52:
          return n4[pos];
          break;
     case 53:
          return n5[pos];
          break;
     case 54:
          return n6[pos];
          break;
     case 55:
          return n7[pos];
          break;
     case 56:
          return n8[pos];
          break;
     case 57:
          return n9[pos];
          break;
     case 58:
          return colon[pos];
          break;
     case 65:
          return A[pos];
          break;
     case 66:   
          return B[pos];
          break;
     case 67:   
          return C[pos];
          break;
     case 68:   
          return D[pos];
          break;
     case 69:   
          return E[pos];
          break;
     case 70:   
          return F[pos];
          break;
     case 71:   
          return G[pos];      
          break;
     case 72:   
          return H[pos];
          break;
     case 73:   
          return I[pos];
          break;
     case 74:   
          return J[pos];
          break;
     case 75:   
          return K[pos];
          break;
     case 76:   
          return L[pos];
          break;
     case 77:   
          return M[pos];
          break;
     case 78:   
          return N[pos];
          break;
     case 79:   
          return O[pos];
          break;
     case 80:   
          return P[pos];
          break;
     case 81:   
          return Q[pos];
          break;
     case 82:   
          return R[pos];
          break;
     case 83:   
          return S[pos];
          break;
     case 84:   
          return T[pos];
          break;
     case 85:   
          return U[pos];
          break;
     case 86:   
          return V[pos];
          break;
     case 87:   
          return W[pos];
          break;
     case 88:   
          return X[pos];
          break;
     case 89:   
          return Y[pos];
          break;
     case 90:   
          return Z[pos];
          break;
     default:
          return 0;
          break;
     }
}

void displayAString(String displayString) {
     // Added by Shane Feb 2020
     
     do // Pad with whitespace at the end if the string is less that 8 characters long
     {
         displayString = displayString + " ";
     } while (displayString.length() < 8);
     
     String currentLetter1 = displayString.substring(0,1);
     String currentLetter2 = displayString.substring(1,2);
     String currentLetter3 = displayString.substring(2,3);
     String currentLetter4 = displayString.substring(3,4);
     String currentLetter5 = displayString.substring(4,5);
     String currentLetter6 = displayString.substring(5,6);
     String currentLetter7 = displayString.substring(6,7);
     String currentLetter8 = displayString.substring(7,8);

     for (c=7; c>-1; c--){ 
           maxPut(8 - c, getByteValue(currentLetter8, c));
           maxPut(8 - c, getByteValue(currentLetter7, c));
           maxPut(8 - c, getByteValue(currentLetter6, c));
           maxPut(8 - c, getByteValue(currentLetter5, c));
           maxPut(8 - c, getByteValue(currentLetter4, c));
           maxPut(8 - c, getByteValue(currentLetter3, c));
           maxPut(8 - c, getByteValue(currentLetter2, c)); // These are in reverse order - ie you need to load right to left
           maxPut(8 - c, getByteValue(currentLetter1, c)); 
           digitalWrite(load, LOW);
           yield();
           delay(1);
           digitalWrite(load,HIGH);
           yield();
           delay(10);    
     }                       
}

int getNextRace(){
     int todayDay = dateToday.substring(0,2).toInt();
     int todayMonth = dateToday.substring(3,5).toInt();
     int todayYear = dateToday.substring(6,12).toInt();
     int numElements = (sizeof(raceDates) / sizeof(raceDates[0]));
     int i = 0;
     int raceFound = 0;
     int raceIndex = -1;
    
     do{
          int raceDay = raceDates[i].substring(0,2).toInt();
          int raceMonth = raceDates[i].substring(3,5).toInt();
          int raceYear = raceDates[i].substring(6,12).toInt();
          if(dateDiff(todayYear, todayMonth, todayDay, raceYear, raceMonth, raceDay ) < 1){
               i++;
          }else {
               raceFound = 1;
          }
     } while (raceFound == 0);

//     Serial.print("Index of race is ");
//     Serial.println(i);
//     Serial.print("Next Race TLA is  ");
//     Serial.println(circuitTLA[i]);
     
     return i;
}

int daysTillNextRace(){
     int daysTillNextRace = 0;
     int todayDay = dateToday.substring(0,2).toInt();
     int todayMonth = dateToday.substring(3,5).toInt();
     int todayYear = dateToday.substring(6,12).toInt();
     int numElements = (sizeof(raceDates) / sizeof(raceDates[0]));
     int i = 0;
     int raceFound = 0;
     int raceIndex = -1;
    
     do{
          int raceDay = raceDates[i].substring(0,2).toInt();
          int raceMonth = raceDates[i].substring(3,5).toInt();
          int raceYear = raceDates[i].substring(6,12).toInt();
          if(dateDiff(todayYear, todayMonth, todayDay, raceYear, raceMonth, raceDay ) < 1){
               i++;
          }else {
               raceFound = 1;
               daysTillNextRace = dateDiff(todayYear, todayMonth, todayDay, raceYear, raceMonth, raceDay );
          }
     } while (raceFound == 0);

     Serial.println(todayDay);
     Serial.println(todayMonth);
     Serial.println(todayYear);
     Serial.print("daysTillNextRace Value : ");
     Serial.println(daysTillNextRace);
     return daysTillNextRace - 2;
}

void ConnectToWifi(){
     delay(3000);
     Serial.println("Connecting to Wifi");
     Serial.println("wifi begin");
//     WiFi.begin(ssid, password);             // Connect to the network
     WiFi.begin("WiFi-2BAC", "10810491");             // Connect to the network
     
     Serial.println("connecting");
     Serial.print("Connecting to ");
     Serial.print(ssid); Serial.println(" ...");

     Serial.println("loop");
     int i = 0;
     while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
     yield();
     delay(1000);
     Serial.println("print ");
     Serial.print(++i); Serial.print(' ');
     }

     Serial.println("end wifi connect");
     Serial.println('\n');
     Serial.println("Connection established!");  
     Serial.print("IP address:\t");
     Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer     
}

void setup () {
     pinMode(dataIn, OUTPUT);
     pinMode(clockPin,  OUTPUT);
     pinMode(load,   OUTPUT);
     digitalWrite(13, HIGH);  
     Serial.begin(115200);
     Wire.begin();
//     rtc.adjust(DateTime(2022, 8, 27, 11, 11, 0));  // Use this to set the RTC if it loses time for some reason.
 
     //////////////////////////////////////////////initiation of the max 7219
     maxIni(max7219_reg_scanLimit, 0x07);      
     maxIni(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
     maxIni(max7219_reg_shutdown, 0x01);    // not in shutdown mode
     maxIni(max7219_reg_displayTest, 0x00); // no display test
     for (i=1; i<=8; i++) {    // empty registers, turn all LEDs off
          maxIni(i,0);
     }
     
     maxIni(max7219_reg_intensity, 0x0f & 0x0f);    // the first 0x0f is the value you can set                   

//     ConnectToWifi();

//     getDateFromWeb();
//     getNextRace();
}
 
 void loop () {    
//     DateTime now = rtc.now();
//     String thisDay = "";
//     String thisMonth = "";     
//     String dayVal = String(now.day());
//     String monthVal = String(now.month());
//     Serial.println("dayVal");
//     Serial.println(dayVal);
//     Serial.println("monthVal");
//     Serial.println(monthVal);
//     Serial.println(String(now.year()));
//     Serial.println("----------------------");


     Serial.println("-------------------------------------------------------");
     Serial.println("WORKING");
     Serial.println("-------------------------------------------------------");
     
     
//     if (now.day() < 10){
//          thisDay = "0" + dayVal;
//     } else {
//          thisDay = dayVal;
//     }
//     
//     if (now.month() < 10){
//          thisMonth = "0" + monthVal;
//     } else {
//          thisMonth = monthVal;
//     }
//    
//     dateToday = thisDay + "-" + thisMonth + "-" + String(now.year());
//          
//     String daysTillWUP = "";
//     displayAString(circuitAbbreviation[getNextRace()]);
//     delay(Delay);
//     if(daysTillNextRace() > 2){
//          daysTillWUP = String(daysTillNextRace()) + " days";
//     }else{
//          if(daysTillNextRace() == 1){
//               daysTillWUP = String(daysTillNextRace()) + " day";
//          } else {
//               daysTillWUP = "  FP1";
//          }
//     }
//     displayAString(daysTillWUP);
//     yield();
//     delay(Delay);

     delay(1500);
 }






 
