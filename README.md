# ESP32 Earthquake Response Simulator

### Description
ESP32-based prototype for simulating and detecting different levels of vibration.

The system utilizes a DC motor controlled with an ultrasonic sensor's distance reading as a virtual remote for the vibration simulator. An ADXL345 accelerometer then samples the changing motion and reports the estimated vibration level through an OLED display, status LEDs, a buzzer, as well as WhatsApp notifications.

> Please note that this is an educational prototype, not a calibrated seismograph or certified earthquake-warning system. The displayed `RS` levels are heuristic/predicted labels and should not be interpreted as measured Richter magnitudes.

### Features
- Controls vibration intensity using distance measurements (with hand)
- Measures acceleration along X, Y, and Z axes
- Classifies vibration into several simulated `RS` levels
- Displays distance, system state, and vibration level on an OLED
- Uses green, yellow, and red LEDs to indicate severity
- Activates a buzzer as an emergency signal for stronger vibrations
- Sends rate-limited WhatsApp alerts through an HTTP endpoint (once every minute)
- Provides detailed readings through Serial Monitor for debugging
- Includes a physical enable switch for the vibration motor
