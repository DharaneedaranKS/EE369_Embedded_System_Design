### Lab 1, 18th Jan 2023

### Task
Make LEDs on launchpad glow.

### Implementation & Learnings

- LEDs are connected to GPIO pin 0, 1, 2, 3.
- Set GPIO to general purpose mode.
- Set GPIO direction to output.
- Setting GPIO to *high*  makes to **not** glow. (Schematic can be checked in datasheet for the reason.)
- Consequently, setting GPIO to *low* makes it glow.
- `DELAY_US` function is used with parameter 1 000 000 to delay for 1s.
- But this delay actually depends on clock speed. We have set PLL to **60MHz**, using the `PLL_Multiplier_12` & `PLL_DivideSelect_ClkIn_by_2`.
- Base clock in **10MHz**, so with these PLL settings, it'll set it to 10MHz * 12 / 2 = **60MHz**, which is ***max*** frequency this device can operate at.
