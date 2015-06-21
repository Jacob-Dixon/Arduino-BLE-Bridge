//This is a program that configures the HM-10/11 module to 
//act as a central device. Note: the MAC address is hard
//coded and will need to be changed for another application.

//Written by Jacob Dixon
//Date: 5/28/2015
//Version: 1.0


#include <SoftwareSerial.h>
#include <PString.h>


#define DEBUG //comment this out to not run debug code
#define MASTER //set to MASTER or SLAVE for Bluetooth mode operation
#define QUESTIONS //ask the bluetooth module some questions

SoftwareSerial BluetoothSerial(10,9); //I setup software serial pins on digital pins 9 and 10.


//Variables used

char bluetoothLogic = ' ';
char strBuffer[50];
bool deviceConnectFlag = false;
bool watchdogTimerEnable = false;
int watchdogTimer = 0;
int ConnectionTries = 0;
//PString bluetoothString(strBuffer,sizeof(strBuffer)); 

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
  
  #ifdef DEBUG
  Serial.begin(57600);
  #endif
  
  //bluetooth AT command setup
  BluetoothSerial.begin(9600); //communication speed to the bluetooth module.
  //wake up the bluetooth module
  
  bluetoothSetup();
 
  // Serial.write("\nInitial Device Scan");
  // BluetoothSerial.write("AT+DISC?"); //Starts device discovery scan
  // readBluetooth();
  // delay(5000);
  // Serial.write("\nReading Devices");
  // readBluetooth();
  // Serial.write("\nAttempting to connect to the first device seen...");
  // BluetoothSerial.write("AT+CONN0");
  // delay(500);
  // readBluetooth();
  // delay(1000);
  // readBluetooth();

}

void loop(void) {

  //Nasty condition tree for bluetooth
  if(BluetoothSerial.available())
  //while(0==1)
  {
	  
	  bluetoothLogic = BluetoothSerial.read(); //peak reads the char but does not pull it from the buffer
	  Serial.write(bluetoothLogic);
	  if(bluetoothLogic == 'O')
	  {
		  bluetoothLogic = BluetoothSerial.read();
		  if(bluetoothLogic == 'K')
		  {
			  bluetoothLogic = BluetoothSerial.read();
			  if(bluetoothLogic == '+')
			  {
				  bluetoothLogic = BluetoothSerial.read();
				  if(bluetoothLogic == 'L')
				  {
					  bluetoothLogic = BluetoothSerial.read();
					  if(bluetoothLogic == 'O')
					  {
						  bluetoothLogic = BluetoothSerial.read();
						  if(bluetoothLogic == 'S')
						  {
							  bluetoothLogic = BluetoothSerial.read();
							  if(bluetoothLogic == 'T')
							  {
								  Serial.print("Connection Lost\nAttempting to re-connect...");
								  bluetoothConnect();
							  }
						  }
					  }
				  }
			  }
		  }
	  }
	}
  
	 // bluetoothString = BluetoothSerial.read();

  if(Serial.available())
  {
	  BluetoothSerial.write(Serial.read());
  }
  // if(BluetoothSerial.available())
  // {
	  // Serial.write(BluetoothSerial.read());
  // }
  // if(inputData == "OK+DISCS") //device scan started
  // {
	  // Serial.write("\nScanning for devices...\n");
  // }
  // if(inputData == "OK+DISCE") //device scan has now ended, try to connect to device
  // {
	  // Serial.write("\nDevice scan completed. Attempting to connect to the first device...");
	  // BluetoothSerial.write("AT+CONN0");
	  // delay(300);
  // }
  // if(inputData == "OK+CONNE") //device is unable to connect, need to retry the device scan
  // {
	  // Serial.write("\nConnection Error. Retrying device scan...");
	  // BluetoothSerial.write("AT+DISC?");
	  // delay(300);
  // }
  // if(inputData == "OK+CONNF") // device is unable to connect, need to retry the device scan
  // {
	  // Serial.write("\nLink Failed. Retrying device scan...");
	  // BluetoothSerial.write("AT+DISC?");
	  // delay(300);
  // }
  // if(inputData == "OK+CONN") // device has connected successfully
  // {
	  // Serial.write("\nDevice connected successfully!");
  // }
  //if(inputData == "OK+LOST");
  //{
	//  Serial.write("\nConnection to device has failed. Rescanning for devices...");
	//  BluetoothSerial.write("AT+DISC?");
	//  delay(300);
  //}
  // index = 0;
  // memset(inputData,0,sizeof(inputData)); //reset char array for next iteration
  
  
  
  

}

void bluetoothSetup(void)
{
	//bluetooth AT command setup
  BluetoothSerial.begin(9600);
  //wake up the bluetooth module
  
  BluetoothSerial.println("I am iron man I am iron man I am iron man I am iron man I am iron man I am iron man");
  
  BluetoothSerial.write("AT+RENEW"); //resets settings to factory defaults
  delay(300);

  //~~~~~~~Training Bluetooth Module~~~~~~~~
  Serial.print("\nTraining Bluetooth Module...\n");
  //per manufacturer, IMME 1 should go here...
  BluetoothSerial.write("AT+IMME1");
  readBluetooth();
  #ifdef MASTER
  BluetoothSerial.write("AT+ROLE1"); //Sets role to master (Slave role is AT+ROLE1)
  #endif
  #ifdef SLAVE
  BluetoothSerial.write("AT+ROLE0"); //Sets role to slave (Master role is AT+ROLE0)
  #endif
  readBluetooth();


  #ifdef DEBUG
  Serial.print("...17%\n");
  #endif
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
  BluetoothSerial.write("AT+NAMEMAGE2_TestWP"); //sets the name of the bluetooth device
  readBluetooth();
  
  
  #ifdef DEBUG
  Serial.print("...100%\n");

  Serial.print("Finished training Bluetooth module\n\n====Startup Parameters====");
  #endif
  
   //~~~~~~~~Startup Questions~~~~~~~~
  #ifdef QUESTIONS //queries the bluetooth chip and sends the info back to a serial terminal on host machine.
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
  
  BluetoothSerial.write("AT+VERS?"); //gets sleep mode information
  Serial.write("\nFirmware Revision:");
  readBluetooth();
  #endif
  
  //~~~~~~Final startup sequence~~~~~~~
  BluetoothSerial.write("AT+START"); //Starts BT module operation
  delay(300);

  BluetoothSerial.write("AT+RESET"); //Resets module to apply changes
  delay(1000);
  
  bluetoothConnect();
}

void bluetoothSwiftStartup(void) //use if the settings have already been set
{
	BluetoothSerial.write("AT+RENEW"); //resets settings to factory defaults
	delay(300);
	
	 //~~~~~~~Training Bluetooth Module~~~~~~~~
  Serial.print("\nTraining Bluetooth Module...\n");
  //per manufacturer, IMME 1 should go here...
  BluetoothSerial.write("AT+IMME1");
  delay(300);
  #ifdef MASTER
  BluetoothSerial.write("AT+ROLE1"); //Sets role to master (Slave role is AT+ROLE1)
  #endif
  #ifdef SLAVE
  BluetoothSerial.write("AT+ROLE0"); //Sets role to slave (Master role is AT+ROLE0)
  #endif
  delay(300);


  BluetoothSerial.write("AT+NOTI1"); //sets BT module to notify of connection changes
  delay(300);

  BluetoothSerial.write("AT+PWRM1"); //sets BT module sleep state to auto-sleep
  delay(300);

  BluetoothSerial.write("AT+MODE2"); //per data sheet, allows control of BT module via UART after the AT commands have been entered.
  delay(300);

  BluetoothSerial.write("AT+SHOW1"); //Sets discovery to "on"
  delay(300);

  BluetoothSerial.write("AT+NAMEMAGE2_TestWP"); //sets the name of the bluetooth device
  delay(300);
  
  BluetoothSerial.write("AT+START"); //Starts BT module operation
  delay(300);

  BluetoothSerial.write("AT+RESET"); //Resets module to apply changes
  delay(1000);
  
  bluetoothConnect();
	
}

void bluetoothConnect()
{
	Serial.write("\nConnecting via MAC address\n");
	BluetoothSerial.write("AT+CON5C313EFE589B"); //specific slave address
	delay(1000);
	
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
								  Serial.print("Connection Lost\nAttempting to re-connect...");
								  bluetoothConnect();
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
									delay(300);
									bluetoothLogic = BluetoothSerial.read();
									if(bluetoothLogic == 'A') //Connection "accepted" means searching
									{
										Serial.print("Searching for device...\n");
										bluetoothConnect();
									}
									else if(bluetoothLogic == 'E')
									{
										Serial.print("Connection Error. Retrying...\n");
										ConnectionTries++;
										bluetoothSetup();
									}
									else if(bluetoothLogic == 'F')
									{
										Serial.print("Connection Failed. Retrying...\n");
										ConnectionTries++;
										bluetoothSetup();
									}
									else //connected or found device
									{
										Serial.print("Connection Successful\n");
										delay(300);
										
										// if(deviceConnectFlag == true)
										// {
											// Serial.print("Connection Successful\n");
											// deviceConnectFlag = false;
											// watchdogTimerEnable = false;
											// watchdogTimer = 0;
											// delay(300);
											//BluetoothSerial.write("Test Message from Master!\n");
										// }
										// else
										// {
											// Serial.print("Device found\n");
											// deviceConnectFlag = true;
											// watchdogTimerEnable = true;
										// }
									}
							  }
						  }
					  }
				  }
			  }
		  }
	  }
	}
 }