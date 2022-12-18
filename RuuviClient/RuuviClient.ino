/*
  This is an example script in how to interact with a RuuviTag Environmental Sensor
  to read BLE Advertisements.

  Board used: MKR WiFi 1010
*/

#include <ArduinoBLE.h>

float temperature, humidity;
bool new_data = false;
bool debug = true;

void blePeripheralDiscoveredHandler(BLEDevice central) {
  if(central.hasManufacturerData() && central.hasAdvertisementData()) {
    // Check PDF From: https://www.bluetooth.com/specifications/assigned-numbers/
    // 0x0499 Ruuvi Innovations Ltd.
    // Decimal: 4 153
    uint8_t manufacturer[central.manufacturerDataLength()];
    central.manufacturerData(manufacturer, central.manufacturerDataLength());
    if((int)manufacturer[0] == 153 && (int)manufacturer[1] == 4) {
      Serial.print("Discovered event from RuuviTag with Mac: ");
      Serial.println(central.address());
    } else {
      if (debug) {
        Serial.print("Advertisement is from an unknown Device with Manufacturer Id: ");
        Serial.print((int)manufacturer[1], HEX);
        Serial.print((int)manufacturer[0], HEX);
        Serial.println();
      }
      return;
    }
  } else {
    // no manufacturer Data available
    if(debug) {
      Serial.print("Discovered Device with mac: ");
      Serial.print(central.address());
      Serial.println(" but does no advertised data or manufacturer data");
    }
    return;
  }

  // Load the Advertised Data and store the values
  int l = central.advertisementDataLength();
  uint8_t value[l];
  central.advertisementData(value, l);

  int l_payload = (int)value[3];
  int format = (int)value[7];
  int payload_start = 8;

  if (format == 5) { // Data Format 5 Protocol Specification (RAWv2)
    // https://github.com/ruuvi/ruuvi-sensor-protocols/blob/master/dataformat_05.md

    Serial.print(value[payload_start], HEX);
    Serial.print(" ");
    Serial.print(value[payload_start+1], HEX);
    Serial.println();

    temperature = (((signed char)value[payload_start] << 8) | ((signed char)value[payload_start + 1])) * 0.005; // Celcius
    humidity = (((signed char)value[payload_start+2] << 8) | ((signed char)value[payload_start + 3])) * 0.0025; // Percent
    new_data = true;
  } else {
    Serial.print("Unknown Data Format from RuuviTag received: ");
    Serial.println(format);
  }
}

void print_data() {
  if(!new_data)
    return;

  new_data = false;
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("C ");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("% ");
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");
    while (1);
  }
  Serial.println("Ruuvi Prototype to listen for Ruuvi Advertisements and Decode data");
  Serial.println("------------------------------------------------------------------");
  BLE.setEventHandler(BLEDiscovered, blePeripheralDiscoveredHandler);
  BLE.scan();
  //BLE.scanForAddress("f2:40:f1:bc:69:e0", true);
}

void loop() {
  BLE.poll();
  print_data();
}
