//#include "TimerOne.h"

void setup(){
pinMode(7, OUTPUT);          // pins safe mode
Serial.begin(9600);
Serial.print("setuped\n");
digitalWrite(7, HIGH); //repos state
}



void wait_us(int us){
        delayMicroseconds(us);
}



void send_start(){               //Send 18ms pull down signal

  int watchdog = 0;
  
    pinMode(7, OUTPUT);          // Set pin to output mode
    digitalWrite(7, LOW);       // Set pull down signal
    delay(18);                  // wait for 18ms
    pinMode(7, INPUT);          // Set pin to safe input mode
    
    while( ! digitalRead(7)){   // wait until signal pulled up
      watchdog++;
      wait_us(10);
        if( watchdog >100){      // test if signal don't kept down
        Serial.print("Signal kept down\n");
        return;                  // else quit function with error
         }

    }
         watchdog = 0;
         
    while( digitalRead(7)){    // wait until DHT pull signal down (normally 20-40us)
      watchdog++;
      wait_us(10);
    if( watchdog >100){      // test if DHT commute corretly
        Serial.print("DHT doesn't pull signal down\n");
        return;                  // else quit function with error
         }
    }

    // If here, signal PUTED down again
    watchdog = 0;

        while( ! digitalRead(7)){     // Wait for DHT release the signal again
      wait_us(10);
      watchdog++;
    if( watchdog > 100){      // test if DHT commute corretly to 0
        Serial.print("DHT don't begin to send data\n");
        return;                  // else quit function with error
         }
    //DHT start next bit transmisson
    while(digitalRead(7)); //synchro
    }
}


int measure_time(){
    int measure_loop = 0;               // init counting
   // timer1_flag = 0;                    // Sync the timer
   
    while( ! digitalRead(7));           // Wait while DHT stop holding down the signal (50ms normally)
    
    while( digitalRead(7)){          // Wait while timer flag doesn't reach 20us
        wait_us(10);                    // Start counting 10us packets
        measure_loop ++;                // Count n*10us
        if(measure_loop > 100){         // Arbitrary error-exit value
            Serial.print("Watchdog measure reached\n"); // Error message
            return -1;                                      // Quit function
                               }
                        }
    // Bits value determination
    if ( (measure_loop <= 3) && (measure_loop != 0)){
        return 0;
    } else if ((measure_loop > 3  ) && (measure_loop <= 9)){
        return 1;
    }else{
         Serial.print("Can't determine bit value\n"); // Error message
         return -1;
    }

}


int receive(int * integral_RH, int * decimal_rh, int * integral_T, int * decimal_T, int * checksum){

    int packet_length = 8;      // Trame_length = 8 bits
    int number_of_packets = 5;      // Trame received
    int bit_received = 0;       // Last bit received
    int trame = 0;              // Constructing trame

    
    while(number_of_packets){
        trame = 0;          // RAZ trame
        packet_length = 8;
        
    while(packet_length){
            trame = trame << 1;                     // Décalage bit à bit, le 0 de départ sera éjecté au 8e bit
            bit_received = measure_time();  // Stockage value
            if(bit_received == -1) return -1;  
            trame = trame | bit_received;
            packet_length --;
            }
    if(number_of_packets == 5) *integral_RH = trame;
    if(number_of_packets == 4) *decimal_rh = trame;
    if(number_of_packets == 3) *integral_T = trame;
    if(number_of_packets == 2) *decimal_T = trame;
    if(number_of_packets == 1) *checksum = trame;

    number_of_packets--;
    }
  return 0;
}

void loop(){

int lol = 0;
int integral_RH = 0, decimal_rh = 0, integral_T = 0, decimal_T = 0, checksum = 0;

while(lol <5){
Serial.print("Send start\n"); // Error message
send_start();
if(receive(&integral_RH, &decimal_rh, &integral_T, &decimal_T, &checksum) != -1){
Serial.print("RH integral :");Serial.println(integral_RH, BIN);
Serial.print("RH decimal :");Serial.println(decimal_rh, BIN);
Serial.print("Integral T :");Serial.println(integral_T, BIN);
Serial.print("Decimal T :");Serial.println(decimal_T, BIN);
Serial.print("checksum :");Serial.println(checksum, BIN);
}else{
  Serial.print("ERROR : received returned -1\n");
  Serial.print("RH integral :");Serial.println(integral_RH, BIN);
Serial.print("RH decimal :");Serial.println(decimal_rh, BIN);
Serial.print("Integral T :");Serial.println(integral_T, BIN);
Serial.print("Decimal T :");Serial.println(decimal_T, BIN);
Serial.print("checksum :");Serial.println(checksum, BIN);
}

lol++;
delay(1000);
}

return;

/*//Serial.print("Send high\n"); // Error message
digitalWrite(7, HIGH);
delay(18);
//Serial.print("Send low\n"); // Error message
digitalWrite(7, LOW);
delay(18);*/

}
