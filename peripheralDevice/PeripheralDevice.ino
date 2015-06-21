//This is the prototype code for the PIU (Player Interface Unit)
//for the MAGE 2 system. 

//Written by Jacob Dixon
//Date: 5/23/2015 | Last Updated: 5/28/2015
//Version: 1.0

//Display library provided by u8g at http://code.google.com/p/u8glib/

//Functionality: This first version of the program displays the MAGE 2 logo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#include <SoftwareSerial.h>
#include <PString.h>


#define DEBUG //comment this out to not run debug code
#define SLAVE //set to MASTER or SLAVE for Bluetooth mode operation
#define QUESTIONS //ask the bluetooth module some questions

SoftwareSerial BluetoothSerial(10,9); //initialize software serial on digital pins 9 and 10.


//Variables used
char strBuffer[50];
char bluetoothLogic = ' ';
bool deviceConnectFlag = false;
bool watchdogTimerEnable = false;
int watchdogTimer = 0;
PString bluetoothString(strBuffer,sizeof(strBuffer)); 

//Bluetooth Read from device command
void readBluetooth(void)
{
  delay(300);
  while(BluetoothSerial.available() > 0)
  {	
	Serial.write(BluetoothSerial.read());
  }
}

void setup(void) {
  
  Serial.begin(57600); //serial connection speed for communication to arduino


  //bluetooth AT command setup
  BluetoothSerial.begin(9600); //serial communication speed to bluetooth module
  
  //wake up the bluetooth module
  BluetoothSerial.println("I am iron man I am iron man I am iron man I am iron man I am iron man I am iron man"); //this is literally what the data-sheet says.
  
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
  BluetoothSerial.write("AT+BAUD0"); //sets serial BAUD communication rate
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
  
  
  #ifdef DEBUG  Serial.print("...100%\n");
  Serial.print("Finished training Bluetooth module restarting module...\n\n");

   // ~~~~~~ reboot sequence~~~~~~~
  BluetoothSerial.write("AT+START"); //Starts BT module operation

  readBluetooth();

  BluetoothSerial.write("AT+RESET"); //Resets module to apply changes

  readBluetooth();
  
   //~~~~~~~~Startup Parameters~~~~~~~~
  #ifdef QUESTIONS //queries the bluetooth chip and sends the info back to a serial terminal on host machine.
  Serial.write("====Startup Parameters====");
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
  #endif
  
  //~~~~~~Final startup sequence~~~~~~~
  // BluetoothSerial.write("AT+START"); //Starts BT module operation
  // BluetoothSerial.end();
  // BluetoothSerial.begin(19200);
  // readBluetooth();

  // BluetoothSerial.write("AT+RESET"); //Resets module to apply changes
  // readBluetooth();
  
bluetoothConnect();
  #endif
}

void loop(void) {

  // send updates to the console forever
  // if(BluetoothSerial.available())
  // {
	  // Serial.write(BluetoothSerial.read());
  // }
    if(BluetoothSerial.available())
  //while(0==1)
  {
	  bluetoothLogic = BluetoothSerial.read();
	  if(bluetoothLogic == 'O')
	  {
		  delay(300);
		  bluetoothLogic = BluetoothSerial.read();
		  if(bluetoothLogic == 'K')
		  {
			  delay(300);
			  bluetoothLogic = BluetoothSerial.read();
			  if(bluetoothLogic == '+')
			  {
				  delay(300);
				  bluetoothLogic = BluetoothSerial.read();
				  if(bluetoothLogic == 'L')
				  {
					  delay(300);
					  bluetoothLogic = BluetoothSerial.read();
					  if(bluetoothLogic == 'O')
					  {
						  delay(300);
						  bluetoothLogic = BluetoothSerial.read();
						  if(bluetoothLogic == 'S')
						  {
							  delay(300);
							  bluetoothLogic = BluetoothSerial.read();
							  if(bluetoothLogic == 'T')
							  {
								  Serial.print("Connection Lost\n");
								  bluetoothConnect();
							  }
						  }
					  }
				  }
			  }
		  }
	  }
	  Serial.print(bluetoothLogic);
  }
  if(Serial.available())
  {
	  BluetoothSerial.write(Serial.read());
  }
}

void bluetoothConnect(void)
{
  while(!BluetoothSerial.available())
  {
	  #ifdef DEBUG
	  delay(1000);
	  Serial.print("Waiting for master.\n");
	  #endif
  }

  while(BluetoothSerial.available())
  {
	  delay(300);
	  bluetoothLogic = BluetoothSerial.read();
	  if(bluetoothLogic == 'O')
	  {
		  delay(300);
		  bluetoothLogic = BluetoothSerial.read();
		  if(bluetoothLogic == 'K')
		  {
			  delay(300);
			  bluetoothLogic = BluetoothSerial.read();
			  if(bluetoothLogic == '+')
			  {
				  delay(300);
				  bluetoothLogic = BluetoothSerial.read();
				  if(bluetoothLogic == 'L')
				  {
					  delay(300);
					  bluetoothLogic = BluetoothSerial.read();
					  if(bluetoothLogic == 'O')
					  {
						  delay(300);
						  bluetoothLogic = BluetoothSerial.read();
						  if(bluetoothLogic == 'S')
						  {
							  delay(300);
							  bluetoothLogic = BluetoothSerial.read();
							  if(bluetoothLogic == 'T')
							  {
								  Serial.print("Connection Lost\n");
								  BluetoothSerial.print("AT+RESET");
								  delay(1000);
							  }
						  }
					  }
				  }
				  else if(bluetoothLogic == 'C')
				  {
					  delay(300);
					  bluetoothLogic = BluetoothSerial.read();
					  if(bluetoothLogic == 'O')
					  {
						  delay(300);
						  bluetoothLogic = BluetoothSerial.read();
						  if(bluetoothLogic == 'N')
						  {
							  delay(300);
							  bluetoothLogic = BluetoothSerial.read();
							  if(bluetoothLogic == 'N')
							  {
									delay(400);
									bluetoothLogic = BluetoothSerial.read();
									if(bluetoothLogic == 'A') //Connection "accepted" means searching
									{
										Serial.print("Device wants to connect...\n");
									}
									else if(bluetoothLogic == 'E')
									{
										Serial.print("Connection Error. Retrying...\n");
										bluetoothConnect();
									}
									else if(bluetoothLogic == 'F')
									{
										Serial.print("Connection Failed. Retrying...\n");
										bluetoothConnect();
									}
									else //connected or found device
									{
										if(deviceConnectFlag == true)
										{
											Serial.print("Connection Successful\n");
											deviceConnectFlag = false;
											watchdogTimerEnable = false;
											watchdogTimer = 0;
											//BluetoothSerial.write("Test Message from Master!\n");
										}
										else
										{
											Serial.print("Device seen\n");
											deviceConnectFlag = true;
											watchdogTimerEnable = true;
										}
									}
							  }
						  }
					  }
				  }
			  }
		  }
	  }
	// if(watchdogTimerEnable == true);
	// {
		// if(watchdogTimer >= 30)
		// {
			// Serial.write("watchdog triggered");
			// watchdogTimer = 0;
			// delay(1000);
			// setup();
		// }
		// watchdogTimer++;
	// }
  }
}