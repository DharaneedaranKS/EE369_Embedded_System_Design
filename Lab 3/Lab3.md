### Lab 3, 1st Feb 2023

### Task
Disable PLL

### Observation
- When we did PLL * 12 /2 = 60MHz, and delay 500 000, we observed, 0.5s on & 0.5 off signal.
- When we did PLL * 1 / 1 = 10MHz, and delay 500 000, we observed, 3s on & 3s off signal.
- When we disable PLL, and delay 500 000, we observed, 3s on & 3s off signal. 
- PLLCR: **0x000**
- PLLSTS: **0x0185** (0000000110000101b)

### Learnings
- `DELAY_US` will give us the count till which the processor counts to generate corresponding delay.

- PLLCR: PLL Control Register 
  - The DIV field (Bits 3 to 0 in PLLCR) in the PLLCR register controls whether the PLL is bypassed or not and sets the PLL clocking ratio when it is not bypassed. PLL bypass is the default mode after reset. (Check table in user manual)

- PLLSTS: PLL Status   
  - 8th bit:7th bit **DIVSEL** Divide Select: This bit selects between /4, /2, and /1 for CLKIN to the CPU.
    - The configuration of the DIVSEL bit is as follows:
    - 00, 01 Select Divide By 4 for CLKIN
    - 10 Select Divide By 2 for CLKIN
    - 11 Select Divide By 1 for CLKIN

  - 2nd bit **PLLOFF** PLL Off Bit: This bit turns off the PLL. This is useful for system noise testing. 
  This mode must only be used when the PLLCR register is set to 0x0000.
    - 0 PLL On (default)
    - 1 PLL Off. While the PLLOFF bit is set the PLL module will be kept powered down.
  - 0th bit **PLLLOCKS** PLL Lock Status Bit.
    - 0 Indicates that the PLLCR register has been written to and the PLL is currently locking. The CPU is clocked by OSCCLK/2 until the PLL locks.
    - 1 Indicates that the PLL has finished locking and is now stable.

  > The device must be in PLL bypass mode (PLLCR = 0x0000) before writing a 1 to PLLOFF. While the PLL is turned off (PLLOFF = 1), do not write a non-zero value to the PLLCR.

- Conclusion: 
  - With DIV field as 0000, we select the first row as given in user manual
  - As PLLSTS 2nd bit is 1, it indicates PLLOFF
  - Finally with PLLSTS 8th bit:7th bit = 1:1, we select the column corresponding to Divide By 1 for CLKIN = **10MHz**