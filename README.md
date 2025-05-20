# 🛹 Electric Skateboard with ESP32 (Master-Slave Architecture)

This project demonstrates a DIY electric skateboard controlled using a custom-built wireless remote. It leverages two ESP32 boards communicating over ESP-NOW, enabling real-time throttle, brake, and mode control. Designed with modularity and functionality in mind, this setup provides an exciting platform for experimenting with wireless control and motor drivers.

---

## 🔧 Features

- **Wireless Communication (ESP-NOW)**: Low-latency protocol between master and slave ESP32.
- **Joystick-Based Throttle**: X-axis controls forward and reverse motion.
- **Brake Control**: Inbuilt joystick button triggers braking.
- **Speed Modes**: Push button toggles between 30%, 60%, and 100% speed.
- **Reverse Toggle**: Slide switch changes drive direction.
- **Battery Level Indicator**: Uses a modified 1S indicator to display remote battery charge level.
- **OLED Display**: 1.96-inch screen shows speed mode, battery level, reverse status, and connection.
- **Level Shifting**: Bidirectional logic level converter ensures 3.3V ESP32 signals are safely converted to 5V for the ESC.
- **Dual-Port Charging/Programming**: Separate USB ports for ESP32 firmware updates and battery charging.
- **Main Power Switch**: Slide switch connected to battery terminal for full circuit control.

---

## ⚙️ Hardware Used (Remote + Reciever)

- 2 × ESP32 Dev Boards (Master + Slave)
- Dual-Axis Joystick Module with Push Button
- 1.96" OLED Display (4-pin, Blue)
- Bidirectional 4-Channel Logic Level Converter (3.3V to 5V)
- ESC (Electronic Speed Controller)
- 1S Battery Voltage Indicator Module (modified)
- 1 × Micro USB Port (for charging only)
- 1 × Micro USB Port (for programming)
- 1 × USB Type-C Port (optional)
- Slide Switches & Push Buttons
- Lithium-ion Battery
- Charging Module (with output to Vin/GND of ESP32)

---

## 📁 Repository Structure

```
/electric-skateboard
│
├── master/               # Remote control code (transmitter)
├── slave/                # Skateboard control code (receiver)
├── docs/                 # Schematics, diagrams, pinouts
├── firmware/             # Optional: precompiled binaries
├── assets/               # Images, videos, mockups
└── README.md
```

---

## 📷 Media

> Insert GIF or images of the working prototype here.

---

## 🎥 YouTube Tutorial Series

I’ve created a dedicated 3-part video series on YouTube to walk you through every part of this project:

1. [The Remote](https://youtu.be/eH9qokv9OXw?si=9TsH_yq5MPhmLzN8)  
2. [The Skateboard]
3. [The Field Test]

Watch the full playlist here: [Electric Skateboard Series](https://youtu.be/eH9qokv9OXw?si=9TsH_yq5MPhmLzN8)

---

## 🚀 Setup Instructions

1. Flash `master` code to the remote ESP32.
2. Flash `slave` code to the skateboard ESP32.
3. Ensure both boards are powered and within range.
4. Connect the output from the level shifter to your ESC’s throttle input.
5. Use the joystick and switches to control motion, speed, and brakes.

---

## 🔮 Future Plans

- Add steering capability via Y-axis of joystick.
- Integrate mobile telemetry for stats and tracking.
- Implement PID-based speed control.
- Add RPM feedback via Hall sensor.

---

## 🙌 Acknowledgements

Built with love by Vivek Dhariwal using open-source tools and ESP32 magic.

---

Feel free to fork, modify, and ride into innovation! 🛠️⚡
