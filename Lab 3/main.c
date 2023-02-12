// Lab 3, Feb 1st 2023
/* Observation
 * This lab, we tried to disable PLL.
 * Delay_US will give us the count till which the processor counts to generate corresponding delay.
 * When we did PLL * 12 /2 = 60MHz, and delay 500000, we observed, 0.5s on & 0.5 off signal.
 * When we did PLL * 1 / 1 = 10MHz, and delay 500000, we observed, 3s on & 3s off signal.
 * When we disabled PLL, we thought we'll see 10MHz, but we observed 6s on & 6s off signal. Reasoning for the unexpected behaviour:
   We checked PLL register: PLLCR & PLLSTS. Values: PLLCR - 0x0001. PLLSTS - 0x0105
	 So, PLLCR tell us which row to select in data sheet, & PLLSTS 8th bit : 7th bit, gives us DIVSEL = 1:0 -> 2
	 So, it selected the row & column corresponding to OSCCLK/2. OSSCLK is 10MHz, according to datasheet.
	 So, it indicates, and justify the observation of 6s on & 6s off, since clock is at 10MHz/2 = 5MHz.
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
  WDOG_disable(myWDog);

  CLK_Handle myClk;
  PLL_Handle myPll;
  myClk = CLK_init((void *)CLK_BASE_ADDR, sizeof(CLK_Obj));
  myPll = PLL_init((void *)PLL_BASE_ADDR, sizeof(PLL_Obj));

  // Using internal clock 1
  // For using internal clock 2: CLK_Osc2Src_Internal
  CLK_setOscSrc(myClk, CLK_OscSrc_Internal);

  // Base clock is 10Mhz, so PLL will do 10 * 12 / 2 = 60Mhz, which is max for this device
  // PLL_setup(myPll, PLL_Multiplier_12, PLL_DivideSelect_ClkIn_by_2);

  // Base clock is 10Mhz, so PLL will do 10 * 1 / 1 = 10Mhz
  // PLL_setup(myPll, PLL_Multiplier_1, PLL_DivideSelect_ClkIn_by_1);

	// PLL disable
  PLL_disable(myPll);

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

  while(1) {
    // set low is glow
    GPIO_setLow(myGpio, GPIO_Number_0);
    // Delay in clock cycles. 1000000 microseconds & 60 Mhz = 1 sec
    DELAY_US(500000);
    // set high is off
    GPIO_setHigh(myGpio, GPIO_Number_0);
    DELAY_US(500000);
  }

  return 0;
}
