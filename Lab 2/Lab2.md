### Lab 2, 25th Jan 2023

### Task

- Set GPIO 0 & 1 as input.
- Set GPIO 2 as output of GPIO 1 && GPIO 2.
- View registers: GPADIR, GPADAT

### Implementation

- Set GPIO 0, 1 as input & 2 as output.
- `Use GPIO_getData()` to get data from GPIO pins. 

### Learnings

- By default, all GPIO pins are inputs.

- GPADIR Register: GPIO Port A Direction
  - 0 Configures the GPIO pin as an input. (default)
  - 1 Configures the GPIO pin as an output.

- GPADAT Register: GPIO Port A Data
  - The GPIO data registers indicate the current status of the GPIO pin, irrespective of which mode the pin is in. 
  - Writing to this register will set the respective GPIO pin high or low if the pin is enabled as a GPIO output.
  - Typically the DAT registers are used for reading the current state of the pins. 
  - Reading a 0 indicates that the state of the pin is currently low, irrespective of the mode the pin is configured.
  - Reading a 1 indicates that the state of the pin is currently high irrespective of the mode the pin is configured.

