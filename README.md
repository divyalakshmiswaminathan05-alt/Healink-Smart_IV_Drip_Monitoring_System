#  Smart IV Drip Monitoring System with Healink App

A compact IoT-based IV monitoring device in the form of a small box, designed to continuously monitor IV drip status using weight-based analysis and integrated with the **Healink app** for real-time visualization and alerts.

---

##  Problem Statement
In hospitals, IV drip monitoring is mostly done manually by nurses, which can lead to:
- Delays in replacing empty IV bottles  
- Risk of air entering the bloodstream  
- IV line blockages going unnoticed  
- Blood backflow conditions  
- Increased workload for healthcare staff  

---

##  Objective
To design a system that:
- Monitors IV bottle weight in real time  
- Tracks drip flow continuously  
- Displays patient data digitally  
- Shows device battery status  
- Detects medical conditions automatically  
- Sends all data to the Healink app  
- Alerts medical staff before critical situations  

---

##  Features
- Small box-like portable design  
- Real-time IV drip monitoring  
- Weight-based medical condition detection  
- Live patient data visualization  
- Drip flow rate display  
- Device battery level monitoring  
- Live data visualization in Healink app  
- Threshold-based alert system  
- Visual and audio notifications  

---

##  Hardware Components
- ESP32 / Arduino Microcontroller  
- Load Cell + HX711 Amplifier  
- IV Bottle Hook (integrated with device)  
- Buzzer / LED Indicator  
- Battery Module + Charging Circuit  
- Enclosure (3D printed / plastic box)  

---

##  Software & Tools
- Arduino IDE  
- Embedded C  
- Healink Mobile / Web App  
- Node-RED / Web Dashboard  
- Wi-Fi Communication  
- Firebase / ThingSpeak (optional)  

---

##  How It Works

- The IV bottle is hung on a **hook provided by the device**  
- A **load cell** continuously measures the weight of the IV bottle  
- Drip flow rate is calculated from weight change over time  
- Patient details are entered in the Healink app  
- Device battery level is monitored by the microcontroller  
- All data is transmitted via Wi-Fi to the cloud  

### Decision Logic
- If weight remains **constant**  
  → IV line is **blocked / no flow**

- If weight **increases**  
  → Possible **blood backflow / reverse flow**

- If weight **decreases normally**  
  → IV flow is **proper**

- If weight decreases to a **very low value**  
  → IV bottle is **nearly empty**

### System Response
- Device triggers **buzzer / LED alert**  
- Healink app displays:
  - Patient details  
  - Current bottle weight  
  - Drip flow rate  
  - Device battery level  
  - Flow status  
  - Alert notifications  

---

##  Applications
- Hospital wards  
- ICU and emergency rooms  
- Dialysis centers  
- Elderly care centers  
- Home healthcare systems  

---

##  Future Scope
- Fully wireless rechargeable device  
- Nurse call system integration  
- Multi-patient dashboard in Healink  
- AI-based infusion prediction  
- Hospital inventory integration  


