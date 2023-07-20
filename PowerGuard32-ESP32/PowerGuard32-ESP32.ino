#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>



// Service and Characteristic UUIDs
#define SERVICE_UUID        "bcbe96b5-795d-4d06-a4e6-9dd718910290"
#define CHARACTERISTIC_UUID "63cec759-d14e-4c97-bcdd-06b59a04346f"

int chargeLimit = 80;  // to store the current charge limit
int currentCharge;

BLECharacteristic *pCharacteristic;




class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string value = pCharacteristic->getValue();

        if (value.length() > 0) {
            Serial.println("Received write request:");
            Serial.println(value.c_str());

            char dataType = value[0];
            int dataValue = atoi(value.substr(1).c_str());

            if (dataType == 'L') {
                // Handle charge limit
                chargeLimit = dataValue;
                Serial.println("Charge limit: " + String(chargeLimit));

                
            } 
            else if (dataType == 'B') {
                // Handle battery status
                currentCharge = dataValue;
                Serial.println("Battery status: " + String(dataValue));
              
            }
        }
    }
};


void setup() {
  Serial.begin(115200);
  pinMode(4, OUTPUT);
  Serial.println("Starting BLE...");



  BLEDevice::init("Charge32");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_WRITE
  );

  // Enable notifications and indications
  pCharacteristic->setNotifyProperty(true);
  pCharacteristic->setIndicateProperty(true);

  pCharacteristic->setCallbacks(new MyCallbacks());

  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();
}

void loop() {

  
      if(currentCharge < chargeLimit-1){
      digitalWrite(4,HIGH);
      
    }
    else{
      digitalWrite(4,LOW);
    }

}

