# Batch20_Group31
## IoT Based Smart Lighting System

## A description of project

<p align="justify">
The IoT-based Smart Lighting System is designed to enhance energy efficiency and automation by intelligently managing lighting based on ambient conditions and user preferences. The system incorporates an ESP8266 NodeMCU microcontroller, which serves as the central control unit, processing data from sensors and managing remote communication via IoT. A Light Dependent Resistor (LDR) is utilized to detect ambient light intensity, ensuring that lights are turned on or off dynamically based on real-time environmental conditions. The 10kΩ resistor is used in conjunction with the LDR in a voltage divider circuit to generate a readable signal for the microcontroller.  

A relay module is integrated to act as an electronic switch, enabling the ESP8266 to control high-power lighting loads safely. Since the relay operates at 5V, a level shifter is employed to maintain proper voltage levels between the ESP8266 (which operates at 3.3V) and the relay. The system is powered by a 5V power supply, ensuring stable operation of all components. A breadboard and connecting wires are used to facilitate easy prototyping and assembly.  

The system architecture is designed to support both automated and manual operation. Through a mobile application, users can monitor the lighting status in real-time and manually switch lights on or off as needed. Additionally, users can manage the lighting threshold, adjusting the sensitivity of the system to ambient light conditions. This feature provides flexibility and ensures user control even when automatic lighting conditions may not align with specific requirements.  

Overall, this smart lighting solution offers a practical and efficient approach to home and industrial lighting management, contributing to energy conservation and user comfort.
</p>


## Components
1. **ESP8266 NodeMCU**  
2. **5V Power Supply**  
3. **10kΩ Resistor**  
4. **Relay Module**  
5. **LDR (Light Dependent Resistor)**  
6. **Level Shifter**  
7. **Breadboard and Connecting Wires**  

## Circuit Diagram
![Circuit Diagram](https://github.com/user-attachments/assets/efc04f69-148d-4073-882d-5ba62a0bcb7f)

