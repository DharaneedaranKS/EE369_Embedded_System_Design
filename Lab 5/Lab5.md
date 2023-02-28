# JTAG Theory 

JTAG (Joint Test Action Group); Test Access Port and Boundary-Scan Architecture. This is a serial bus with four signals: Test Clock (TCK), Test Mode Select (TMS), Test Data Input (TDI), and Test Data Output (TDO). The bus is used as a test bus for the 'Boundary-Scan' of ICs, as in Design-For-Testability (DFT). To use JTAG, during the design, you most select JTAG compatible devices. ICs supporting JTAG will have the four additional pins listed above. Devices reside on the bus in a daisy chain, with TDO of one device feeding TDI of the next device. In addition to having the pins listed above each device most have a Boundary-Scan Register. The Boundary-Scan Register may be used to test the interconnection between ICs [Chip-to-Chip] or test with in the IC. Boundary-scan tests can be used to check continuity between devices. Continuity checks on PWB nets may be performed by sending out a know pattern and receiving that same pattern at the input to another IC(s). Not receiving the test signal or pattern would indicate a broken PWB trace, a failed IC, or cold solder joint.

## TCK: [Test Clock] 
has noting to do with the board or system clock. The Test Clock is used to load the test mode data from the TMS pin, and the test data on the TDI pin [on the rising edge]. On the falling edge test clock outputs the test data on the TDO pin. As with any clock pin this line needs to be terminated in order to reduce reflections. The termination should be a 68 ohm resistor in series with a 100pF capacitor to ground. The TCK signal is bused to all Integrated Circuits [IC] in the JTAG chain. The signal may require buffering or be fanned out by multiple drivers depending on the distance and number of devices in the chain. Using multiple drivers would also require a termination resistor on each TCK line.

## TMS: [Test Mode Select Input] 
controls the operation of the test logic, by receiving the incoming data]. The value at the input on the rising edge of the clock controls the movement through the states of the TAP controller. The TMS line has an internal pull-up, so the input is high with no input. The TMS signal is bused to all ICs in the JTAG chain. The TMS line should have a 10k pull-up resistor on the line.

## TDI: [Test Data Input] 
receives serial input data which is either feed to the test data registers or instruction register, but depends on the state of the TAP controller. The TDI line has an internal pull-up, so the input is high with no input. The TDI signal is feed to the TDI pin of the first IC in the JTAG chain. The TDO signal from that IC is then sent to the TDI pin of the next IC in the chain or sent back out to the JTAG header. The TDI line should have a 10k pull-up resistor on the line.

## TDO: [Test Data Output] 
outputs serial data which comes from either the test data registers or instruction register, but depends on the state of the TAP controller. Data applies to the TDI pin will appear at the TDO pin but may be shifted of a number of clock cycles, depending on the length of the internal register. The TDO pin is high-Impedance. The TDO signal is the output from a JTAG device that feed the TDI input of another JTAG device. The TDO line should have a 10k pull-up resistor on the line. The TDO signal should also include a 22 ohm series resistor placed near the last device in the JTAG chain.

##TRST: [Test Rest] 
will asynchronously reset the JTAG test logic. The logic is reset (with TRST) regardless of the state of TMS or TCLK. The TRST signal is bused to all ICs in the JTAG chain. The TRST signal should include a pull-down resistor when possible to reduce the chance the signal floats.

# JTAG pins 

Monitor the JTAG pins when the switch related to JTAG is switched on and off. Pins to observe 
- TCK
- TDO
- TMS

## Results 

![results](https://github.com/DharaneedaranKS/EE369_Embedded_System/blob/main/Lab%205/JTAG_results.jpeg)

From the top, the waveforms are those of :
- TCK
- TMS
- TDO
