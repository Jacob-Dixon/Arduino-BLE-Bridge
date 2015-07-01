//This is bluetooth HM-10 module testing code
//with a arduino MCU interface.

//Written by Jacob Dixon
//Date: 6/26/2015 | Last Updated: 7/1/2015
//Version: 1.4

#include <SoftwareSerial.h> //required if your arduino has only 1 hardware serial connection

#define DEBUG //comment this out to not run debug code
#define SLAVE //set to MASTER or SLAVE for Bluetooth mode operation
#define QUESTIONS //ask the bluetooth module some questions on startup

//comment these out if you don't want these messages
#define TEST_MESSAGE // send a message to the other side every (user defined) interval
#define TEST_MESSAGE_INTERVAL = 100

SoftwareSerial BluetoothSerial(10,9); //initialize software serial on digital pins 9(TX) and 10(RX).


//Variables used
#ifdef TEST_MESSAGE
int counter = 0;
#endif


//Bluetooth Read from device command
void readBluetooth(void)
{
  delay(1000); //the HM-10 is fairly finicky, you can try lowering this delay, but you may experience unexpected behavior.
  while(BluetoothSerial.available() > 0)
  {	
	Serial.write(BluetoothSerial.read());
  }
}

void setup(void) {
  
  Serial.begin(9600); //serial connection speed for communication to arduino and PC COM port.


    BluetoothSerial.begin(9600); //on Arduino, I have never been able to get it to work on any other BAUD rate. Let me know if you are able to get it to work on anything faster.
  //wake up the bluetooth module
  BluetoothSerial.println("I am iron man I am iron man I am iron man I am iron man I am iron man I am iron man"); //this is literally what the data-sheet says. This may or may not be necessary. 
 
  
  BluetoothSerial.write("AT+RENEW"); //resets settings to factory defaults
  readBluetooth();
  
  
  //~~~~~~~Training Bluetooth Module~~~~~~~~
  Serial.print("\nTraining Bluetooth Module...\n");
  #ifdef MASTER
  BluetoothSerial.write("AT+ROLE1"); //Sets role to master (Slave role is AT+ROLE1)
  #endif
  #ifdef SLAVE
  BluetoothSerial.write("AT+ROLE0"); //Sets role to slave (Master role is AT+ROLE0)
  #endif
  readBluetooth();
  BluetoothSerial.write("AT+NAMEMAGE2_PIU"); //sets the name of the bluetooth device
  readBluetooth();
  #ifdef DEBUG
  Serial.print("...17%\n");
  #endif
  BluetoothSerial.write("AT+BAUD0"); //sets serial BAUD communication rate to 115.2k BAUD
  delay(200);
  readBluetooth();
  BluetoothSerial.write("AT+NOTI1"); //sets BT module to notify of connection changes
  readBluetooth();
  #ifdef DEBUG
  Serial.print("...34%\n");
  #endif
  BluetoothSerial.write("AT+PWRM1"); //sets BT module sleep state to auto-sleep
  readBluetooth();
  #ifdef DEBUG
  Serial.print("...51%\n");
  #endif
  BluetoothSerial.write("AT+MODE2"); //per data sheet, allows control of BT module via UART after the AT commands have been entered.
  readBluetooth();
  
  BluetoothSerial.write("AT+POWE3"); //sets to max power mode "6dbm"
  readBluetooth();
  #ifdef DEBUG
  Serial.print("...68%\n");
  #endif
  BluetoothSerial.write("AT+SHOW1"); //Sets discovery to "on"
  readBluetooth();
  #ifdef DEBUG
  Serial.print("...85%\n");
  #endif
  // BluetoothSerial.write("AT+IMME1");
  // readBluetooth();
  BluetoothSerial.write("AT+TYPE0"); //sets the bonding to not need a PIN code from other end.
  readBluetooth();
 
  #ifdef DEBUG
  Serial.print("...100%\n");
  #endif
  Serial.print("Finished training Bluetooth module restarting module...\n\n");

   // ~~~~~~ reboot sequence~~~~~~~
  BluetoothSerial.write("AT+START"); //Starts BT module operation

  readBluetooth();

  BluetoothSerial.write("AT+RESET"); //Resets module to apply changes

  readBluetooth();
  
   //~~~~~~~~Startup Parameters~~~~~~~~
  #ifdef QUESTIONS //queries the bluetooth chip and sends the info back to a serial terminal on host machine.
  Serial.write("====Startup Parameters====");
  delay(500);//to let the reset process finish
  BluetoothSerial.write("AT+ADDR?"); //gets MAC address
  Serial.write("\nMAC Addr:");
  readBluetooth();

  
  BluetoothSerial.write("AT+BAUD?"); //gets BAUD information
  Serial.write("\nBAUD Rate:");
  readBluetooth();

  
  BluetoothSerial.write("AT+GAIN?"); //gets gain information
  Serial.write("\nGain Parameter:");
  readBluetooth();
  
  BluetoothSerial.write("AT+MODE?"); //gets mode information... though this may not work and may need to be role instead.
  Serial.write("\nOperating Mode:");
  readBluetooth();

  BluetoothSerial.write("AT+POWE?"); //gets antenna power information
  Serial.write("\nAntenna Power State:");
  readBluetooth();

  BluetoothSerial.write("AT+PWRM?"); //gets sleep mode information
  Serial.write("\nSleep Mode State:");
  readBluetooth();
  
  BluetoothSerial.write("AT+TEMP?"); //gets IC Temperature
  Serial.write("\nIC Temperature:");
  readBluetooth();
  Serial.write(" Celsius\n");
  #endif //end QUESTIONS
}

void loop(void) {

  // send updates to the console forever
  if(BluetoothSerial.available())
  {
	 Serial.write(BluetoothSerial.read());
  }

  if(Serial.available())
  {
	  BluetoothSerial.write(Serial.read());
  }
  #ifdef TEST_MESSAGE
  if(counter >= TEST_MESSAGE_INTERVAL) //fires test message every TEST_MESSAGE_INTERVAL iterations. Feel free to go faster than 100 or so, however I experienced some bytes dropped. (possible due to 9600 BAUD rate)
  {
	  BluetoothSerial.write("Packet Received!!\n"); //test message to send
	  counter = 0;
  }
  counter++;
  #endif
}

