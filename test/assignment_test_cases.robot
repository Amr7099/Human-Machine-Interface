*** Settings ***
Documentation    Embedded Systems Project Test Cases

*** Test Cases ***

Test Case 1: System Initialization
    [Documentation]    Verify that the system initializes correctly when the program starts.
    # Arrange
    Connect the Arduino Uno to the computer and upload the program.
    Connect the LM35 sensor, LCD, buttons, and LED according to the circuit connections.
    # Act
    Start the program.
    # Assert
    Verify that the LCD displays Temperature HMI and System Starting.
    Verify that the startup messages are displayed in the Serial Monitor.
    Verify that the initial temperature limits are set to LH = 30 C and LL = 20 C.
    Verify that the LED is initially OFF.
    Result: PASS

Test Case 2: UART Initialization
    [Documentation]    Verify that UART communication is initialized correctly.
    # Arrange
    Connect the Arduino Uno to the computer using USB.
    Open the Serial Monitor and select 9600 baud.
    # Act
    Start the program.
    # Assert
    Verify that the startup messages and temperature readings are displayed correctly in the Serial Monitor without communication errors.
    Result: PASS

Test Case 3: ADC Initialization
    [Documentation]    Verify that the ADC is configured correctly for reading the LM35 sensor.
    # Arrange
    Connect the LM35 output to PC0 (ADC0).
    Ensure that AVCC is used as the ADC voltage reference.
    # Act
    Start the program and allow the ADC to perform conversions.
    # Assert
    Verify that the ADC initializes successfully and produces valid 10-bit readings between 0 and 1023.
    Result: PASS

Test Case 4: LM35 Temperature Measurement
    [Documentation]    Verify that the LM35 sensor provides temperature readings correctly.
    # Arrange
    Connect the LM35 sensor to the Arduino Uno.
    Place the sensor in an environment with a known temperature.
    # Act
    Start the program and allow the system to read the sensor.
    # Assert
    Verify that the measured temperature is displayed on the LCD and in the Serial Monitor.
    Verify that the temperature reading is reasonably close to the actual environmental temperature.
    Result: PASS

Test Case 5: LCD Initialization
    [Documentation]    Verify that the 16x2 LCD initializes and displays information correctly.
    # Arrange
    Connect the LCD to the Arduino Uno using the configured 4-bit interface.
    Connect the potentiometer to adjust the LCD contrast.
    # Act
    Start the program.
    # Assert
    Verify that the LCD displays the startup message and then displays the temperature, status, and configured limits after initialization.
    Result: PASS

Test Case 6: LCD Temperature Display
    [Documentation]    Verify that the measured temperature is displayed with one decimal place.
    # Arrange
    Start the system and ensure that the LM35 sensor is connected correctly.
    # Act
    Allow the system to read and display the temperature.
    # Assert
    Verify that the first LCD row displays the temperature in the format T:XX.X.
    Verify that the displayed temperature is updated approximately every 500 ms.
    Result: PASS

Test Case 7: Initial Temperature Limits
    [Documentation]    Verify that the system starts with the default high and low temperature limits.
    # Arrange
    Start the program without pressing either button.
    # Act
    Allow the startup sequence to finish.
    # Assert
    Verify that the LCD displays LH:30 and LL:20.
    Verify that the Serial Monitor displays Initial LH: 30 C and Initial LL: 20 C.
    Result: PASS

Test Case 8: High Limit Increase
    [Documentation]    Verify that a short press of the LH button increases the high temperature limit.
    # Arrange
    Start the system with the high limit set to 30 C.
    Ensure that the LH button is connected to PB0 with an external pull-down resistor.
    # Act
    Press and quickly release the LH button.
    # Assert
    Verify that the high temperature limit increases from 30 C to 31 C.
    Verify that the new high limit is displayed on the LCD and the Serial Monitor.
    Result: PASS

Test Case 9: High Limit Decrease
    [Documentation]    Verify that a long press of the LH button decreases the high temperature limit.
    # Arrange
    Start the system with the high limit set to 30 C.
    Ensure that the LH button is connected correctly.
    # Act
    Press and hold the LH button for approximately one second.
    # Assert
    Verify that the high temperature limit decreases from 30 C to 29 C.
    Verify that the new high limit is displayed on the LCD and the Serial Monitor.
    Result: PASS

Test Case 10: Low Limit Increase
    [Documentation]    Verify that a short press of the LL button increases the low temperature limit.
    # Arrange
    Start the system with the low limit set to 20 C.
    Ensure that the LL button is connected to PB1 with an external pull-down resistor.
    # Act
    Press and quickly release the LL button.
    # Assert
    Verify that the low temperature limit increases from 20 C to 21 C.
    Verify that the new low limit is displayed on the LCD and the Serial Monitor.
    Result: PASS

Test Case 11: Low Limit Decrease
    [Documentation]    Verify that a long press of the LL button decreases the low temperature limit.
    # Arrange
    Start the system with the low limit set to 20 C.
    Ensure that the LL button is connected correctly.
    # Act
    Press and hold the LL button for approximately one second.
    # Assert
    Verify that the low temperature limit decreases from 20 C to 19 C.
    Verify that the new low limit is displayed on the LCD and the Serial Monitor.
    Result: PASS

Test Case 12: High Limit Maximum Restriction
    [Documentation]    Verify that the high temperature limit cannot exceed its maximum allowed value.
    # Arrange
    Start the system and increase the high temperature limit until it reaches 150 C.
    # Act
    Press and quickly release the LH button again.
    # Assert
    Verify that the high temperature limit remains at 150 C and does not increase further.
    Verify that the low temperature limit remains unchanged.
    Result: PASS

Test Case 13: Low Limit Minimum Restriction
    [Documentation]    Verify that the low temperature limit cannot decrease below zero.
    # Arrange
    Start the system and decrease the low temperature limit until it reaches 0 C.
    # Act
    Press and hold the LL button again for approximately one second.
    # Assert
    Verify that the low temperature limit remains at 0 C and does not decrease further.
    Verify that the high temperature limit remains unchanged.
    Result: PASS

Test Case 14: Temperature Limit Separation
    [Documentation]    Verify that the system maintains at least a one-degree difference between the high and low limits.
    # Arrange
    Start the system with LH = 30 C and LL = 20 C.
    Adjust the limits until the high limit is exactly one degree above the low limit.
    # Act
    Attempt to decrease the high limit using a long press of the LH button.
    Attempt to increase the low limit using a short press of the LL button.
    # Assert
    Verify that neither operation allows the high and low limits to become equal or overlap.
    Verify that the high limit always remains at least one degree above the low limit.
    Result: PASS

Test Case 15: Button Debouncing
    [Documentation]    Verify that the system prevents false button actions caused by mechanical button bouncing.
    # Arrange
    Start the system and ensure that both buttons are released.
    # Act
    Press and release the LH button while producing normal mechanical contact bouncing.
    Repeat the same procedure with the LL button.
    # Assert
    Verify that each physical press and release is detected as a single button action.
    Verify that unwanted repeated limit changes are prevented.
    Result: PASS

Test Case 16: Long Press Repeated Decrease
    [Documentation]    Verify that holding a button continuously decreases its corresponding temperature limit at approximately one-second intervals.
    # Arrange
    Start the system with LH = 30 C and LL = 20 C.
    Ensure that the LH button is released.
    # Act
    Press and hold the LH button for approximately three seconds.
    # Assert
    Verify that the high temperature limit decreases approximately once every second while the button remains held.
    Verify that the limit decreases without being treated as a short press when the button is released.
    Result: PASS

Test Case 17: Button Release After Long Press
    [Documentation]    Verify that releasing a button after a long press does not generate an additional increase action.
    # Arrange
    Start the system with the high temperature limit set to 30 C.
    Ensure that the LH button is released.
    # Act
    Press and hold the LH button for approximately one second, then release it.
    # Assert
    Verify that the high temperature limit decreases to 29 C.
    Verify that releasing the button does not increase the limit back to 30 C.
    Result: PASS

Test Case 18: Temperature Within Configured Limits
    [Documentation]    Verify that the system reports normal operation when the measured temperature is within the configured limits.
    # Arrange
    Start the system with LH = 30 C and LL = 20 C.
    Ensure that the LM35 sensor is exposed to a temperature between 20 C and 30 C.
    # Act
    Allow the system to measure the temperature.
    # Assert
    Verify that the LCD displays OK.
    Verify that the LED remains OFF.
    Verify that the Serial Monitor displays NORMAL.
    Result: PASS

Test Case 19: Temperature Above High Limit
    [Documentation]    Verify that the system activates the alert when the measured temperature exceeds the high limit.
    # Arrange
    Start the system with the high temperature limit set to 30 C.
    Place the LM35 sensor in an environment where the measured temperature exceeds 30 C.
    # Act
    Allow the system to measure the temperature.
    # Assert
    Verify that the LED turns ON.
    Verify that the LCD displays NOK.
    Verify that the Serial Monitor displays ALERT - ABOVE HIGH LIMIT.
    Result: PASS

Test Case 20: Temperature Below Low Limit
    [Documentation]    Verify that the system activates the alert when the measured temperature falls below the low limit.
    # Arrange
    Start the system with the low temperature limit set to 20 C.
    Place the LM35 sensor in an environment where the measured temperature is below 20 C.
    # Act
    Allow the system to measure the temperature.
    # Assert
    Verify that the LED turns ON.
    Verify that the LCD displays NOK.
    Verify that the Serial Monitor displays ALERT - BELOW LOW LIMIT.
    Result: PASS