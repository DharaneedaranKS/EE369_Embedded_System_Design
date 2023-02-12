// Lab 4, Feb 8, 2023
/*
  Watchdog:
  --------
  The watchdog module generates an output pulse, 512 oscillator-clocks (OSCCLK) wide whenever the 8-bit 
  watchdog up counter has reached its maximum value. To prevent this, either disable the counter or the software 
  must periodically write a 0x55 + 0xAA sequence into the watchdog key register, which resets the watchdog 
  counter. 

  The WDRST and XRS signals are driven low for 512 OSCCLK cycles when a watchdog reset occurs. Likewise, if the watchdog interrupt
  is enabled, the WDINT signal will be driven low for 512 OSCCLK cycles when an interrupt occurs

  The watchdog can be configured in the SCSR register to either reset the device (WDRST) or assert an interrupt 
  (WDINT) if the watchdog counter reaches its maximum value. The behavior of each condition is described 
  below:

  • Reset mode:
    If the watchdog is configured to reset the device, then the WDRST signal will pull the device reset (XRS) pin 
    low for 512 OSCCLK cycles when the watchdog counter reaches its maximum value.

  • Interrupt mode:
    If the watchdog is configured to assert an interrupt, then the WDINT signal will be driven low for 512 OSCCLK 
    cycles, causing the WAKEINT interrupt in the PIE to be taken if it is enabled in the PIE module.
*/

#include "DSP28x_Project.h"     // DSP28x Headerfile

#include "f2802x_common/include/clk.h"
#include "f2802x_common/include/gpio.h"
#include "f2802x_common/include/pll.h"
#include "f2802x_common/include/wdog.h"


#ifdef _FLASH
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
#endif

int main(void) {
  WDOG_Handle myWDog;
  myWDog = WDOG_init((void *)WDOG_BASE_ADDR, sizeof(WDOG_Obj));

  CLK_Handle myClk;
  myClk = CLK_init((void *)CLK_BASE_ADDR, sizeof(CLK_Obj));

  PLL_Handle myPll;
  myPll = PLL_init((void *)PLL_BASE_ADDR, sizeof(PLL_Obj));

  // Using internal clock 1
  // For using internal clock 2: CLK_Osc2Src_Internal
  CLK_setOscSrc(myClk, CLK_OscSrc_Internal);

  // Base clock is 10Mhz, so PLL will do 10 * 12 / 2 = 60Mhz, which is max for this device
  PLL_setup(myPll, PLL_Multiplier_12, PLL_DivideSelect_ClkIn_by_1);

  GPIO_Handle myGpio;
  myGpio = GPIO_init((void *)GPIO_BASE_ADDR, sizeof(GPIO_Obj));

  // Set GPIO 0, 1, 2, 3 to output
  GPIO_setMode(myGpio, GPIO_Number_0, GPIO_0_Mode_GeneralPurpose);
  GPIO_setDirection(myGpio, GPIO_Number_0, GPIO_Direction_Output);
  GPIO_setMode(myGpio, GPIO_Number_1, GPIO_1_Mode_GeneralPurpose);
  GPIO_setDirection(myGpio, GPIO_Number_1, GPIO_Direction_Output);
  GPIO_setMode(myGpio, GPIO_Number_2, GPIO_2_Mode_GeneralPurpose);
  GPIO_setDirection(myGpio, GPIO_Number_2, GPIO_Direction_Output);
  GPIO_setMode(myGpio, GPIO_Number_3, GPIO_3_Mode_GeneralPurpose);
  GPIO_setDirection(myGpio, GPIO_Number_3, GPIO_Direction_Output);

  // Set all of those to high, which means to NOT glow
  GPIO_setHigh(myGpio, GPIO_Number_0);
  GPIO_setHigh(myGpio, GPIO_Number_1);
  GPIO_setHigh(myGpio, GPIO_Number_2);
  GPIO_setHigh(myGpio, GPIO_Number_3);

  /* Lab 5 starts here */
  // Watchdog interrupt enable
  WDOG_enableInt(myWDog);

  /* -- TO CHECK IF INTERRUPT IS WORKING
  // CPU write access to the flash configuration registers can be enabled only by executing the EALLOW instruction. 
  EALLOW;
  // This function resets the watchdog timer.
  SysCtrlRegs.WDKEY = 0x0055;
  SysCtrlRegs.WDKEY = 0x00AA;
  if (SysCtrlRegs.SCSR & 0x04) {
    GPIO_setLow(myGpio, GPIO_Number_0);
    DELAY_US(5000000);
    GPIO_setHigh(myGpio, GPIO_Number_0);
  }
  // Disable watchdog timer
  // SysCtrlRegs.WDCR= 0x0068;
  // Write access is disabled when the EDIS instruction is executed.
  EDIS;
  -- TO CHECK IF INTERRUPT IS WORKING */

  WDOG_disable(myWDog);

  WDOG_setPreScaler(myWDog, WDOG_PreScaler_OscClk_by_512_by_2);

  while (1) {
    if(WDOG_getIntStatus(myWDog))
      GPIO_setLow(myGpio,GPIO_Number_0);
    else
      GPIO_setHigh(myGpio,GPIO_Number_0);
    // Check in oscilloscope. 512 clock cycles * 1/10MHZ = 51.2us delay
  }

  return 0;
}


