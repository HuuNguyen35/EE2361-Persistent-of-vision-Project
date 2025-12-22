# EE2361 Persistent-of-Vision (POV) Fan Display  
**EE2361 – Fall 2025 Final Project**

This project implements a **Persistent-of-Vision (POV) display** using a
**PIC24FJ64GA002 microcontroller**, a **DC motor–driven rotating fan**, and a
**WS2812B RGB LED strip** mounted on the fan blade. By synchronizing LED updates
with the fan’s rotation, the system creates the illusion of a stable image or
text in mid-air.

An **IR reflective sensor** measures the fan’s RPM, and an **HTML-based pattern
generator** allows users to design custom RGB patterns that are exported
directly into firmware-ready C arrays.

---

## Demo
https://screenpal.com/content/video/cTl336nY7Au

---

## Key Features
- PIC24FJ64GA002–based embedded control
- 28-LED WS2812B RGB LED strip mounted on a rotating fan blade
- IR sensor–based RPM measurement
- Optional LCD display for live RPM readout
- HTML pixel-art tool to generate POV patterns
- Custom mechanical chassis designed in SolidWorks
- Slip ring used to route power/signals to the rotating blade

---

## System Overview
1. A DC motor spins the fan blade with the LED strip attached.
2. An IR reflective sensor mounted beneath the fan detects each rotation.
3. The PIC24 measures the rotation period and calculates timing for each angular
   column.
4. LED colors are updated at precise times during the rotation.
5. Due to persistence of vision, the viewer perceives a stable image or text.

---

## Hardware
- **Microcontroller:** PIC24FJ64GA002  
- **LEDs:** WS2812B RGB LED strip (28 LEDs total)  
- **Sensor:** TCRT5000 IR reflective sensor module  
- **Motor:** DC motor  
- **Mechanical:**  
  - Custom chassis designed in SolidWorks  
  - Slip ring for rotating electrical connections  
- **Optional:** LCD display for RPM readout  

### Power
- **5 V:** WS2812B LED strip and DC motor  
- **3.3 V:** PIC24FJ64GA002 and IR sensor  
- Power is provided via the **Microchip SNAP board**

---

## Firmware
- **IDE:** MPLAB X  
- **Target MCU:** PIC24FJ64GA002  
- The repository includes a ready-to-build **`.X` MPLAB project**
- No special configuration bits or external libraries required

### Firmware Responsibilities
- Read IR sensor signals to determine rotation timing (RPM)
- Divide each rotation into fixed angular columns
- Output RGB data to the WS2812B LED strip
- Optionally update an LCD with real-time RPM values

---

## Pattern Generator (HTML Tool)
The project includes an HTML-based **circular pixel art generator** used to
design POV display patterns.

### Capabilities
- Paint RGB patterns on a polar grid (radial pixels × angular columns)
- Render text using a built-in 3×5 pixel font
- Export patterns in multiple formats

### Output
- **C source output for PIC24**, including:
  - Pattern dimensions
  - RGB array formatted for direct inclusion in firmware

### How to Use
1. Open the HTML file in a web browser.
2. Create a pattern or render text.
3. Click **Export C (PIC24)**.
4. Copy the generated output into:
