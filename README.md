# Embedded Systems Project Report: Temperature Monitoring and Human Machine Interface (HMI)

## 1. Introduction

This project implements a Temperature Monitoring and Human Machine Interface (HMI) system using an Arduino Uno based on the ATmega328P microcontroller and an LM35 temperature sensor.

The system measures the surrounding temperature through the ADC interface and displays the measured temperature, its status, and the configured High (LH) and Low (LL) temperature limits on a 16×2 LCD.

Two push buttons allow the user to adjust the temperature limits. A short press increases the corresponding limit by 1°C, while a long press decreases it by 1°C. Software debouncing is implemented to prevent unwanted button readings caused by mechanical bouncing.

An LED acts as an alert indicator whenever the measured temperature falls outside the configured limits. UART communication is also implemented to transmit temperature readings, configured limits, and system status to the Serial Monitor.

The project uses native C programming and direct AVR register manipulation without relying on pre-developed Arduino libraries for the main system functionalities.

## 2. Project Code

**GitHub Repository:**

[GitHub Repository URL](https://github.com/Amr7099/Human-Machine-Interface.git)

**Local Root Directory:**

[Project Source Code](./code/A2_main.ino)

The source code is organized into modular files:

- `A2_main.ino` – Main program and system control.
- `uart.ino` / `uart.h` – UART initialization and communication.
- `adc.ino` / `adc.h` – ADC configuration and analog readings.
- `lm35.ino` / `lm35.h` – LM35 temperature measurement and conversion.
- `lcd.ino` / `lcd.h` – 16×2 LCD initialization and display functions.
- `Input_Output_define.ino` / `Input_Output_define.h` – Digital input/output pin configuration.

## 3. Test Cases

Test cases are documented in plain English using a Robot Framework-style test case format. They describe the expected behavior of the system and cover temperature measurement, LCD operation, button functionality, temperature limit adjustment, and LED alert indication.

**Test Cases:**

[Test Cases File](./test/assignment_test_cases.robot)

## 4. Bill of Materials (BOM)

**BOM:**

[Bill of Materials](./bom/bom.csv)

**Optimized BOM:**

[Optimized Bill of Materials](./bom/optimized_bom.csv)

## 5. Video Demonstration

[Recorded Video](./video/sample_video.mp4)

## 6. Design

[Project Design Document](./design/project_design_tpl.md)
