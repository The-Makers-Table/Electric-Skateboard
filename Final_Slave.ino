#include <esp_now.h>
#include <WiFi.h>
#include <driver/ledc.h>

#define PWM_PIN 25
#define REVERSE_PIN 5  

const int PWM_FREQ = 1000;
const int PWM_CHANNEL = 0;
const int PWM_RESOLUTION = 8;

unsigned long lastReceivedTime = 0;
const unsigned long TIMEOUT_MS = 500; 

void setup() {
    Serial.begin(115200);
    
    
    ledcAttach(PWM_PIN, PWM_FREQ, PWM_RESOLUTION);
    ledcWrite(PWM_PIN, 0);
    
   
    pinMode(REVERSE_PIN, OUTPUT);
    digitalWrite(REVERSE_PIN, LOW);

    
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW init failed");
        while(1);
    }
    esp_now_register_recv_cb(onDataReceived);
}

void onDataReceived(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len) {
    if (len > 0) {
        int8_t receivedValue = data[0];
        lastReceivedTime = millis();

        
        bool reverseState = (receivedValue & 0x80);
        digitalWrite(REVERSE_PIN, reverseState ? HIGH : LOW);

       
        uint8_t throttle = receivedValue & 0x7F;
        throttle = constrain(throttle, 0, 100);

        
        int pwmValue = map(throttle, 0, 100, 0, 255);
        ledcWrite(PWM_PIN, pwmValue);

        
        Serial.printf("Throttle: %d%%, Reverse: %s, PWM: %d\n", 
                     throttle, 
                     reverseState ? "ON" : "OFF", 
                     pwmValue);
    }
}

void loop() {
    
    if (millis() - lastReceivedTime > TIMEOUT_MS) {
        ledcWrite(PWM_PIN, 0);
    }
}