#include <Arduino.h>
// #include <SPI.h>
// #include <LoRa.h>

// void setup() {
//   Serial.begin(9600);
//   while (!Serial);

//   Serial.println("LoRa Receiver");

//   if (!LoRa.begin(915E6)) {
//     Serial.println("Starting LoRa failed!");
//     while (1);
//   }
//   Serial.println("LoRa Initiation Successful!");
// }

// void loop() {
//   // try to parse packet
//   int packetSize = LoRa.parsePacket();
//   if (packetSize) {
//     // received a packet
//     Serial.print("Received packet '");

//     // read packet
//     while (LoRa.available()) {
//       Serial.print((char)LoRa.read());
//     }

//     // print RSSI of packet
//     Serial.print("' with RSSI ");
//     Serial.println(LoRa.packetRssi());
//   }
// }

// ***-----------------------------------------------

#include <SPI.h>
#include <LoRa.h>

int counter = 0;
// const int csPin = 7;          // LoRa radio chip select
// const int resetPin = 6;       // LoRa radio reset
// const int irqPin = 1;         // change for your board; must be a hardware interrupt pin

String outgoing;              // outgoing message

byte msgCount = 0;            // count of outgoing messages
byte localAddress = 0xFF;     // address of this device
byte destination = 0xBB;      // destination to send to
long lastSendTime = 0;        // last send time
int interval = 2000;          // interval between sends

void setup() {
  Serial.begin(9600);

  Serial.println("Initializing LoRa Reciever");

// override the default CS, reset, and IRQ pins (optional)
  // LoRa.setPins(csPin, resetPin, irqPin);// set CS, reset, IRQ pin

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  Serial.println("Setting spreading factor");
  LoRa.setSpreadingFactor(9);          // ranges from 6-12,default 7 see API docs

  Serial.println("LoRa init succeeded.");
}


void readCANMessage() {
  uint8_t packetSize = LoRa.parsePacket();

  if (packetSize == 0) return;          // if there's no packet, return

  // uint8_t msgReceived = LoRa.read();
  uint8_t receivedMessage[11];              // Array to store the received message
  uint8_t i = 0;                            // Index for the array

  // Read each byte into the array
  while (LoRa.available() && i < 11) {
    receivedMessage[i] = LoRa.read();
    i++;
  }

  // Now `receivedMessage` contains the 11-byte CAN message
  Serial.print("*&");
  for (int j = 0; j < i; j++) {
    Serial.print(receivedMessage[j], HEX);
    Serial.print(" ");
  }
  Serial.print("&*");
}

void loop() {

  readCANMessage();

}



// **** ------ EXAMPLE below----- ***** 
//   // if (millis() - lastSendTime > interval) {
//   //   String message = "HeLoRa World! ";   // send a message
//   //   message += msgCount;
//   //   sendMessage(message);
//   //   Serial.println("Sending " + message);
//   //   lastSendTime = millis();            // timestamp the message
//   //   interval = random(2000) + 1000;    // 2-3 seconds
//   //   msgCount++;
//   // }

//   // parse for a packet, and call onReceive with the result:
//   onReceive(LoRa.parsePacket());
// }

// void onReceive(int packetSize) {
//   if (packetSize == 0) return;          // if there's no packet, return

//   // read packet header bytes:
//   int recipient = LoRa.read();          // recipient address
//   byte sender = LoRa.read();            // sender address
//   byte incomingMsgId = LoRa.read();     // incoming msg ID
//   byte incomingLength = LoRa.read();    // incoming msg length

//   String incoming = "";

//   while (LoRa.available()) {
//     incoming += (char)LoRa.read();
//   }

//   if (incomingLength != incoming.length()) {   // check length for error
//     Serial.println("error: message length does not match length");
//     return;                             // skip rest of function
//   }

//   // if the recipient isn't this device or broadcast,
//   if (recipient != localAddress && recipient != 0xFF) {
//     Serial.println("This message is not for me.");
//     return;                             // skip rest of function
//   }

//   // if message is for this device, or broadcast, print details:
//   Serial.println("Received from: 0x" + String(sender, HEX));
//   Serial.println("Sent to: 0x" + String(recipient, HEX));
//   Serial.println("Message ID: " + String(incomingMsgId));
//   Serial.println("Message length: " + String(incomingLength));
//   Serial.println("Message: " + incoming);
//   Serial.println("RSSI: " + String(LoRa.packetRssi()));
//   Serial.println("Snr: " + String(LoRa.packetSnr()));
//   Serial.println();
// } 

