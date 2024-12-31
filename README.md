# ProxiCharge: Proximity-Based Charger Safety System 
![Enclosure_2024-Dec-31_02-07-59PM-000_CustomizedView21053023037_png](https://github.com/user-attachments/assets/5014bdc3-5336-44a1-b25b-96b3493d1a9b)

## Overview 📘

ProxiCharge is an automatic battery charger shutdown system designed to enhance safety in shared electronics workshops. It mitigates the risks of overcharging lithium-based batteries by ensuring supervision during the charging process.

### Problem Statement
In our university's electronics workshop, students sometimes leave lithium battery chargers unattended while charging, unaware of the potential risks of fire or chemical leakage due to charger malfunction. ProxiCharge ensures the chargers are automatically powered off when no one is present in the room.

## How It Works ❓

- **Initial Setup**: The user presses the power switch on the motion detector to activate the chargers. The system begins monitoring for human presence and lights up green.
- **Safety Measures**: If the room is vacant:
  - A 5-minute countdown begins before shutdown.
  - At the 4-minute mark, the system emits an audio alarm and cycles through light indicators: blue → yellow → red.
  - If no motion is detected in the room for 5 minutes, the motion detector uses the ESPNOW protocol to send a "turn off" command to the charger power supply controller.
- **Reset Mechanism**: If the user returns within 5 minutes, the system resets the countdown. If they return later, the switch must be pressed again to reactivate the chargers.
- **Outcome**: Chargers automatically power off if the room remains vacant, preventing unsupervised charging.

## Features ✅

- Motion detection with three PIR sensors.
- Remote control of charger power supply via ESPNOW protocol.
- Visual and audio alerts during the shutdown countdown.
- OTA firmware updates for motion detector.

## Technical Details 🔧

### Components
- **Motion Detector**:  
  - Three PIR motion sensors.  
  - ESP32-S2 microcontroller for prototyping (Lolin S2 Mini).  
  - Power switch for user interaction.
  - RGB LED Ring for visual feedback and buzzer for auditory feedback.  
  - USB-powered, OTA-capable.

- **Remote Power Supply Controller **:  
  - 24 volt 15 ampere SMPS power supply for powering the battery chargers.  
  - ESP32-S2 microcontroller for controlling the relay. 
  - Relay to switch the power to the battery chargers on/off.

### Design
- **Mechanical**:  
  - Motion sensor enclosure designed in Autodesk Fusion 360 and 3D printed.  
  - Reused metal enclosure for the power supply (custom enclosure planned).  

  **Fusion 360 Renders:**
  ![Enclosure_2024-Dec-31_02-20-23PM-000_CustomizedView51132567864_png](https://github.com/user-attachments/assets/c8fd60d5-4baf-4544-871b-d21d2d1ad18d)
  ![Enclosure_2024-Dec-31_02-07-59PM-000_CustomizedView21053023037_png](https://github.com/user-attachments/assets/9ebeaa09-779d-42ad-9e79-1f80b09adb34)

- **Electronics**:  
  - Circuit diagram of the motion detector
  - ![Schematic_Battery-Charging-Station-Auto-Shutdown_2025-01-01 (1)](https://github.com/user-attachments/assets/abbddfce-93d0-4d39-abc8-d0608e34a4a3)

The circuit diagram for the Remote Power Supply Controller will be uploaded shortly.

### Programming
- **Language**: Arduino  
- **Environment**: PlatformIO + Visual Studio Code  

## Final Results 📷
Here are some pictures of the final prototype:  
- **Motion Detector:**
- ![Motion Sensor-5 5](https://github.com/user-attachments/assets/e21efacf-cc81-4956-8fcd-aa7f318b4264)
- ![Motion Sensor-4](https://github.com/user-attachments/assets/44673d7f-9dc4-4123-a7aa-1089d5e88a5c)
- ![Motion Sensor-6](https://github.com/user-attachments/assets/5590017a-3632-4b28-b02a-ef29dd34a9bf)
- ![Motion Sensor-3](https://github.com/user-attachments/assets/d60fe017-d865-4d5c-a3d7-7d9c20e43332)
- ![Motion Sensor-8](https://github.com/user-attachments/assets/49cb04fb-28a5-4caf-a25a-9ae1df9a9e0c)
- ![Motion Sensor-2](https://github.com/user-attachments/assets/8eee04f6-4e2b-4cc4-8da7-040b5ab1b2a0)

- **Battery Charger Power Supply:**
- ![IMG_2580](https://github.com/user-attachments/assets/ba82bff6-7d33-478f-9fa2-8477b36bb225)
- ![IMG_2576 2](https://github.com/user-attachments/assets/93d78559-8927-431b-a62f-2f2fac9247e2)


## Current Plans 🎯

- Finalize testing and refine hardware/software for improved reliability.  

Stay tuned for updates, and check out the demonstration video! 

https://github.com/user-attachments/assets/e3da7332-e028-45b5-88b8-72c04bd070bb

---

