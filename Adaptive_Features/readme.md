# 🚘 Adaptive Cruise Control and ADAS Integration

This project implements a **miniature Adaptive Driving Assistance System (ADAS)** that combines **lane and obstacle detection, speed sensing, automatic braking**, and **cruise control with regenerative braking**.  
It consists of two Arduino-based modules communicating via the **I²C protocol**.

---

## 🧩 System Overview

The project includes **two Arduino units** working together:

| Unit | Function | Communication |
|------|-----------|----------------|
| **Unit 1 – ADAS Sensor Module** | Detects vehicle speed, front obstacles, and seatbelt status | Acts as **I²C Slave** |
| **Unit 2 – Cruise Control & Regenerative Braking Module** | Controls vehicle throttle, cruise mode, and regenerative braking | Acts as **I²C Master** |

Both units are synchronized through I²C data exchange for dynamic throttle and brake management based on real-time sensor feedback.

---

## ⚙️ Hardware Components

### **Unit 1 – ADAS Sensor Module**
| Component | Purpose |
|------------|----------|
| Ultrasonic sensors (×6) | Measure obstacle distance around the vehicle |
| IR sensor | Detects wheel rotation pulses for RPM calculation |
| Hall effect sensor | Detects seatbelt magnetic latch |
| Arduino board | Sensor processing and I²C data communication |
| Brake relay | Activates emergency braking |

### **Unit 2 – Cruise Control & Regenerative Braking**
| Component | Purpose |
|------------|----------|
| Potentiometer (Throttle) | Simulates manual accelerator input |
| MCP4725 DAC module | Generates analog output to motor controller |
| Arduino board | Controls throttle, cruise mode, regen braking |
| Push buttons/switches | Manual brake, cruise, regen toggles |
| I²C connection | Receives obstacle distance from ADAS unit |

---

## 🧠 Algorithms and Technical Details

### 🔹 1. Speed Measurement (RPM Detection)
- The **IR sensor** detects wheel slots or reflective markers.
- Pulse counts over 1 second are converted to RPM:
  \[ RPM = \frac{\text{Pulses per second} \times 60}{8} \]
- Used to estimate vehicle speed and detect movement.

### 🔹 2. Distance Sensing (Ultrasonic Array)
- Six HC-SR04 ultrasonic sensors are placed around the front and sides of the vehicle.
- Distance data (in cm) is measured using the `HCSR04.h` library.
- The **minimum front distance** between two forward sensors determines the nearest obstacle.

### 🔹 3. ADAS Braking Logic
- If the nearest obstacle distance < `200 cm` **and** ADAS braking is enabled,  
  → The **brake relay** is triggered.  
- If both front sensors report `-1`, the system interprets no valid reading (sensor out of range).

### 🔹 4. Seatbelt Detection
- The **Hall effect sensor** reads magnetic closure.  
  - HIGH → Seatbelt fastened  
  - LOW → Not fastened

### 🔹 5. Cruise Control and Regenerative Braking
- The main controller reads accelerator input via **analog pin A3**.  
- The **MCP4725 DAC** outputs proportional voltage to motor control.  
- Cruise mode maintains constant speed, while:
  - If distance < 50 cm → throttle = 0  
  - If distance < 100 cm → throttle = 75% of manual value  
  - Otherwise → full throttle

### 🔹 6. Regenerative Braking
- Activated when accelerator < threshold and regen switch ON.  
- A digital pin enables braking energy recovery by toggling the **regen pin**.

### 🔹 7. I²C Communication
- ADAS module sends distance data to Cruise module via **I²C address 9**.
- The Cruise module requests one byte periodically:
  ```cpp
  Wire.requestFrom(9, 1);
  distance = Wire.read();
  ```
- The distance data directly influences the adaptive throttle output.

---

## 📊 Data Flow Diagram

```
          [ Ultrasonic Sensors ]       [ IR Sensor ]      [ Hall Sensor ]
                    │                        │                    │
                    └──────────────┬─────────┴──────────────┬─────┘
                                   │                        │
                             [ ADAS Arduino ] --(I²C)--> [ Cruise Control Arduino ]
                                                             │
                                                             ├── [ MCP4725 DAC Output ]
                                                             ├── [ Motor Controller ]
                                                             └── [ Regen / Brake Relay ]
```

---

## 🧪 Results and Observations

| Test Scenario | Expected Behavior | Observed Result |
|----------------|------------------|-----------------|
| Object < 50 cm | Emergency brake | Relay activates brake output |
| Object 50–100 cm | Reduced throttle | 75% output applied via DAC |
| Object > 100 cm | Normal cruise | Full throttle output |
| Accelerator released + Regen ON | Regenerative braking | Regen relay triggered |
| Seatbelt off | Warning in serial output | Correct detection |
| Wheel motion | RPM displayed on Serial | Accurate pulse-based RPM reading |

---

## 📈 Performance Highlights

- **Response time:** < 100 ms between obstacle detection and brake activation  
- **RPM measurement accuracy:** ±3%  
- **Cruise mode throttle stability:** Smooth analog output using MCP4725  
- **Sensor refresh rate:** 6 ultrasonic readings every 100 ms  

---

## 📽️ Demonstration

Attach your test video and sensor module image here:

- 🎥 **Video:** `ADAS_Cruise_Test.mp4`  
- 🖼️ **Image:** `ADAS_Setup.jpg`

---

## 🚀 Future Enhancements

- Integrate **CAN bus** for real vehicle interfacing.  
- Add **PID-based adaptive cruise control** logic.  
- Replace ultrasonic sensors with **mmWave radar** for higher accuracy.  
- Combine with **lane detection** for complete autonomous operation.

---

## 👨‍💻 Developed By

**Stefin Shiby George**  
M.Tech – Signal Processing, NIT Calicut  
Focus Areas: Embedded Systems, Intelligent Vehicles, and Computer Vision  
