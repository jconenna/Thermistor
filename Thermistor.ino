#include <math.h>

// Steinhart Hart equation coefficients determined experimentally:
// Use a calibrated temperature sensor or thermommeter with the thermistor attached to a multimeter
// set to read resistance. Measure the resistance of the thermistor at three different temperatures.
// Plug each value of T and Rt into 1/T = a + b*ln(Rt) + c[ln(Rt)]^3 and solve the system of three linear equations
// for a, b, and c using algebra, or a computer tool such at matlab, wolfram, or an online matrix equation calculator. 
// Here are values I calculated using an Accurite temperature sensor and calibrating my thermistor to it at room temp,
// refrigerator, and freezer temperatures. 
float a = 1.520E-3;
float b = 1.657E-4;
float c = 4.075E-7;

// Value of R1 in voltage divider circuit
float const R1 = 10000.0;

void setup() {
  // Pin A0 is analog input for voltage across thermistor in voltage divider circuit.
  pinMode(A0, INPUT);
  // Begin serial for output readings 
  Serial.begin(9600);
  // Set reference voltage for ADC to 1.1V which for myself was measured as 1.067 V across AREF and GND pins.
  // The lower voltage reference is used to lower current levels through the thermistor to lessen self heating 
  // effects (joule heating, I^2C heating).
  analogReference(INTERNAL);
}

void loop() {
 // Read in analog input from A0 for voltage across thermistor 
 float Vt = analogRead(A0);
 
 // Scale analog input for thermistor voltage to actual voltage
 Vt = (1.067*Vt) / 1023.0;
 
 // Solve for resistance of thermistor (Rt) from voltage divider eqn: Vt = 5V * [ Rt / (R1 + Rt) ] 
 float Rt = (Vt * R1) / (1.067 - Vt);
 
 // Solve for abolute temperature of thermistor in K from Steinhart-Hart eqn: 1/T = a + b*ln(Rt) + c[ln(Rt)]^3
 float T = pow( a + b*log(Rt) + c*pow(log(Rt), 3), -1);
 
 // Convert temperature to your regional scale, fahrenheit here in the states
 float Tf = 1.8*(T-273) + 32;
 
 // Round temperature to nearest tenth
 Tf = roundf(Tf * 10.0) / 10.0;  
 
 // Print temperature to screen
 Serial.println(Tf);
 
 // 500 ms between temperature readings
 delay(500);
}
