#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "f2802x_common/include/clk.h"
#include "f2802x_common/include/flash.h"
#include "f2802x_common/include/gpio.h"
#include "f2802x_common/include/pie.h"
#include "f2802x_common/include/pll.h"
#include "f2802x_common/include/pwm.h"
#include "f2802x_common/include/wdog.h"
void pwm_Init_();
//void set_freq(int freq);
//void set_dutycycle(int duty);
#define TBPRD  12500  // Period register
#define CMPA   6250    //start the pwm at 50% duty cycle
#define CMPB   3125

CLK_Handle myClk;
GPIO_Handle myGpio;
PWM_Handle myPwm1;
PWM_Handle myPwm2;
//PWM_Handle myPwm3;
void main(void)
{
    CPU_Handle myCpu;
    PLL_Handle myPll;
    WDOG_Handle myWDog;
    // Initialize all the handles needed for this application
    myClk = CLK_init((void *)CLK_BASE_ADDR, sizeof(CLK_Obj));
    myCpu = CPU_init((void *)NULL, sizeof(CPU_Obj));
    myGpio = GPIO_init((void *)GPIO_BASE_ADDR, sizeof(GPIO_Obj));
    myPll = PLL_init((void *)PLL_BASE_ADDR, sizeof(PLL_Obj));
    myPwm1 = PWM_init((void *)PWM_ePWM1_BASE_ADDR, sizeof(PWM_Obj));
    myPwm2 = PWM_init((void *)PWM_ePWM1_BASE_ADDR, sizeof(PWM_Obj));
    //myPwm3 = PWM_init((void *)PWM_ePWM1_BASE_ADDR, sizeof(PWM_Obj));
    myWDog = WDOG_init((void *)WDOG_BASE_ADDR, sizeof(WDOG_Obj));
    // Perform basic system initialization
    WDOG_disable(myWDog);
    CLK_setOscSrc(myClk, CLK_OscSrc_Internal);
    PLL_setup(myPll, PLL_Multiplier_1, PLL_DivideSelect_ClkIn_by_4);
    CPU_disableGlobalInts(myCpu);
    CPU_clearIntFlags(myCpu);
    // Initalize GPIO
    GPIO_setMode(myGpio, GPIO_Number_0, GPIO_0_Mode_EPWM1A);
    GPIO_setMode(myGpio, GPIO_Number_1, GPIO_1_Mode_EPWM1B);
    //GPIO_setMode(myGpio, GPIO_Number_2, GPIO_2_Mode_EPWM2A);
    //set_freq(10);
    //set_dutycycle(75);
    CLK_disableTbClockSync(myClk);
    pwm_Init_();
    CLK_enableTbClockSync(myClk);
    while(1);
}

/*void set_freq(int freq)
{
 float Tpwm,c;
 Tpwm = 1/freq;
 c = Tpwm/0.000008;
 TBPRD = (int)12500.98;
}
void set_dutycycle(int duty)
{
 float div;
 div = 100/duty;
 CMPA = (int)(TBPRD/div);
 CMPA = (int)6250.123;
}*/
void pwm_Init_()
{
    CLK_enablePwmClock(myClk, PWM_Number_1);
    CLK_enablePwmClock(myClk, PWM_Number_2);// Note that the value of this is supposed to be initialised in the GPIO headerfile
    CLK_enablePwmClock(myClk, PWM_Number_3);
    // Setup TBCLK
    PWM_setPeriod(myPwm1, TBPRD);   // Set timer period 801 TBCLKs
    PWM_setPhase(myPwm1, 0x0000);   // Phase is 0
    PWM_setCount(myPwm1, 0x0000);   // Clear counter
    PWM_setPeriod(myPwm2, TBPRD);   // Set timer period 801 TBCLKs
    PWM_setPhase(myPwm2, 0x0000);   // Phase is 0
    PWM_setCount(myPwm2, 0x0000);   // Clear counter
    //PWM_setPeriod(myPwm3, TBPRD);   // Set timer period 801 TBCLKs
    //PWM_setPhase(myPwm3, 0x0000);   // Phase is 0
    //PWM_setCount(myPwm3, 0x0000);   // Clear counter
    // Set Compare values
    PWM_setCmpA(myPwm1, CMPA);  // Set compare A value
    PWM_setCmpB(myPwm2, CMPA); // Set compare A value
    //PWM_setCmpA(myPwm3, CMPC);
    // Setup counter mode
    PWM_setCounterMode(myPwm1, PWM_CounterMode_Up); // Count up and down
    PWM_disableCounterLoad(myPwm1);                     // Disable phase loading
    PWM_setHighSpeedClkDiv(myPwm1, PWM_HspClkDiv_by_10); // Clock ratio to SYSCLKOUT
    PWM_setClkDiv(myPwm1, PWM_ClkDiv_by_1);
    PWM_setCounterMode(myPwm2, PWM_CounterMode_UpDown); // Count up and down
    PWM_disableCounterLoad(myPwm2);                     // Disable phase loading
    PWM_setHighSpeedClkDiv(myPwm2, PWM_HspClkDiv_by_10); // Clock ratio to SYSCLKOUT
    PWM_setClkDiv(myPwm2, PWM_ClkDiv_by_1);
    //PWM_setCounterMode(myPwm3, PWM_CounterMode_UpDown); // Count up and down
    //PWM_disableCounterLoad(myPwm3);                     // Disable phase loading
    //PWM_setHighSpeedClkDiv(myPwm3, PWM_HspClkDiv_by_10); // Clock ratio to SYSCLKOUT
    //PWM_setClkDiv(myPwm3, PWM_ClkDiv_by_1);
    // Setup shadowing
    PWM_setShadowMode_CmpA(myPwm1, PWM_ShadowMode_Shadow);
    PWM_setLoadMode_CmpA(myPwm1, PWM_LoadMode_Zero);
    PWM_setShadowMode_CmpB(myPwm2, PWM_ShadowMode_Shadow);
    PWM_setLoadMode_CmpB(myPwm2, PWM_LoadMode_Zero);
    //PWM_setShadowMode_CmpB(myPwm3, PWM_ShadowMode_Shadow);
    //PWM_setLoadMode_CmpB(myPwm3, PWM_LoadMode_Zero);
    // Set actions
    PWM_setActionQual_CntUp_CmpA_PwmA(myPwm1, PWM_ActionQual_Clear);      // Set PWM1A on event A, up count
    PWM_setActionQual_CntDown_CmpA_PwmA(myPwm1, PWM_ActionQual_Set);
    //PWM_setActionQual_CntDown_CmpB_PwmB(myPwm2, PWM_ActionQual_Clear);      // Set PWM1B on event A, down count
    //PWM_setActionQual_CntUp_CmpB_PwmB(myPwm2, PWM_ActionQual_Set);
    PWM_setActionQual_CntUp_CmpA_PwmB(myPwm2, PWM_ActionQual_Set);
    PWM_setActionQual_CntDown_CmpB_PwmB(myPwm2, PWM_ActionQual_Set);
    //PWM_setActionQual_CntUp_CmpB_PwmB(myPwm2, PWM_ActionQual_Set);// Set PWM2A on event A, down count
    //PWM_setActionQual_CntDown_CmpB_PwmB(myPwm2, PWM_ActionQual_Clear);

    // Clear PWM1A on event A, down count
}

//===========================================================================
// No more.
