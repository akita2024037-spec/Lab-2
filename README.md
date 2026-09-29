# IoT 2026 - Lab 2 Template

You need to finish following 4 exercises

### Setup Configuration in this Wokwi Project template:

- RED LED - `D26`
- Green LED - `D27`
- Blue LED - `D14`
- Yellow LED - `D12`


- Button (Active high) - `D25`
- Light sensor (analog) - `D33`

- LCD I2C - SDA: `D21`
- LCD I2C - SCL: `D22`

- Servo Motor: `D5`

- Buzzer: `D32`

![alt text](image.png)


## 1) LED chase (Knight Rider)
- Cycle through **RED (D26) → GREEN (D27) → YELLOW (D12) → BLUE (D14) → YELLOW (D12) → GREEN (D27)** and repeat, turning on one LED at a time for 150 ms while the others stay OFF.  
- Keep track of the step index in a variable (do not use `delay()` chains longer than one step - use a single `delay(150)` per loop iteration).  
- Serial: Print the name of the LED that just turned on, e.g. `chase=RED`.

---

## 2) Serial-only sensor statistics
- Every 1000 ms, take **10 back-to-back `analogRead()` samples** of **LIGHT (D33)** (no delay between the samples themselves).  
- Compute the **minimum**, **maximum** and **average** of those 10 samples.  
- No LEDs are used in this exercise.  
- Serial: Print one line per second in the format `min=120 max=340 avg=210`.

---

## 3) Sensor threshold alert with hysteresis
- Every 300 ms, read **LIGHT (D33)**.  
- Maintain a boolean `alertActive` state:  
  - If the reading rises **above 3000** and the alert is not already active, set it active.  
  - If the reading drops **below 2500** and the alert is active, clear it.  
  (This gap between 2500 and 3000 prevents rapid flickering.)  
- No LEDs are used in this exercise.  
- Serial: Print `ALERT=1` only the moment it becomes active, and `ALERT=0` only the moment it clears (not on every loop).

---

## 4) Button press counter with pattern
- Detect **BUTTON (D25)** presses using proper edge detection (act once per press, not once per loop while held).  
- Keep a press counter that increases by 1 on each press and wraps back to 0 after reaching 4.  
- Based on the counter value, light up exactly that many LEDs simultaneously, in this fixed order: **RED (D26)**, then **GREEN (D27)**, then **YELLOW (D12)**, then **BLUE (D14)** (e.g. counter=2 → RED and GREEN ON, YELLOW and BLUE OFF).  
- Serial: Print `count=<n>` every time the counter changes.
