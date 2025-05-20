#include <esp_now.h>
#include <WiFi.h>
#include <Adafruit_SSD1306.h>


#define JOYSTICK_VERTICAL_PIN 34
#define MODE_BUTTON_PIN 32
#define BRAKE_LIMIT_SWITCH_PIN 33
#define REVERSE_TOGGLE_PIN 25


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

uint8_t slaveMacAddress[] = {0x34, 0x5F, 0x45, 0xE7, 0x6D, 0xC0};

uint8_t currentMode = 1; 
bool isConnected = false;
bool isReversed = false;

// Battery level pins
#define BATTERY_PIN_25 12
#define BATTERY_PIN_50 13
#define BATTERY_PIN_75 14
#define BATTERY_PIN_100 15


void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    isConnected = (status == ESP_NOW_SEND_SUCCESS);
}

void onDataReceived(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len) {
    
}

int smoothThrottle(int joystickValue) {
    const int JOYSTICK_FORWARD = 0;
    const int JOYSTICK_CENTER = 1880;
    const int JOYSTICK_BACKWARD = 4095;
    const int deadZone = 50;

    if (abs(joystickValue - JOYSTICK_CENTER) < deadZone) return 0;

    int throttleValue = 0;
    
    if (!isReversed) {
        if (joystickValue < JOYSTICK_CENTER) {
            throttleValue = map(joystickValue, JOYSTICK_FORWARD, JOYSTICK_CENTER, 100, 0);
        }
    } else {
        if (joystickValue > JOYSTICK_CENTER) {
            throttleValue = map(joystickValue, JOYSTICK_CENTER, JOYSTICK_BACKWARD, 0, 100);
        }
    }

    switch(currentMode) {
        case 1: throttleValue *= 0.5; break; 
        case 2: throttleValue *= 0.7; break; 
        case 3: break; 
    }

    return constrain(throttleValue, 0, 100);
}

void sendThrottleValue() {
    int verticalJoystickValue = analogRead(JOYSTICK_VERTICAL_PIN);
    int throttleValue = smoothThrottle(verticalJoystickValue);

    if (digitalRead(BRAKE_LIMIT_SWITCH_PIN) == LOW) throttleValue = 0;

    int8_t sendValue = throttleValue;
    if (isReversed) sendValue |= 0x80;

    esp_now_send(slaveMacAddress, (uint8_t *)&sendValue, sizeof(sendValue));

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.printf("Mode  : %s\n", currentMode == 1 ? "Eco +" : currentMode == 2 ? "Eco" : "Beast");
    display.printf("Speed : %d%%\n", throttleValue);
    display.printf("Reverse: %s\n", isReversed ? "ON" : "OFF");

    int batteryLevel = digitalRead(BATTERY_PIN_100) ? 100 :
                     digitalRead(BATTERY_PIN_75) ? 75 :
                     digitalRead(BATTERY_PIN_50) ? 50 : 25;
    display.printf("Battery: %d%%\n", batteryLevel);
    display.printf("Board : %s\n", isConnected ? "Connected" : "Not Found");
    display.display();
}

void handleModeChange() {
    static unsigned long lastPress = 0;
    if (millis() - lastPress > 300) {
        lastPress = millis();
        currentMode = (currentMode % 3) + 1;
    }
}

void setup() {
    Serial.begin(115200);

    pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
    pinMode(BRAKE_LIMIT_SWITCH_PIN, INPUT_PULLUP);
    pinMode(REVERSE_TOGGLE_PIN, INPUT_PULLUP);
    pinMode(BATTERY_PIN_25, INPUT);
    pinMode(BATTERY_PIN_50, INPUT);
    pinMode(BATTERY_PIN_75, INPUT);
    pinMode(BATTERY_PIN_100, INPUT);
    

    attachInterrupt(digitalPinToInterrupt(MODE_BUTTON_PIN), handleModeChange, FALLING);

    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) return;

    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, slaveMacAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    esp_now_add_peer(&peerInfo);


    esp_now_register_send_cb(onDataSent);
    esp_now_register_recv_cb(onDataReceived);



    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) while(1);
    display.clearDisplay();
    display.display();
}

void loop() {
    isReversed = (digitalRead(REVERSE_TOGGLE_PIN) == LOW);
    sendThrottleValue();
    delay(50);
}