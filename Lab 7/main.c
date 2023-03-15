#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

#include "common/include/clk.h"
#include "common/include/flash.h"
#include "common/include/gpio.h"
#include "common/include/pie.h"
#include "common/include/pll.h"
#include "common/include/pwm.h"
#include "common/include/wdog.h"

//
// Defines
//

//
// Configure which EPwm timer interrupts are enabled at the PIE level:
// 1 = enabled,  0 = disabled

#define PWM1_INT_ENABLE  1
#define PWM2_INT_ENABLE  1

// Defines that configure the period for each timer
#define PWM1_TIMER_TBPRD   1000
#define PWM2_TIMER_TBPRD     500
//
// Make this long enough so that we can see an LED toggle
//
//#define DELAY 1000000L

__interrupt void epwm1_isr(void);
void InitEPwmTimer(void);
//
// Globals
//
uint32_t  EPwm1TimerIntCount;
uint32_t  LoopCount;

CLK_Handle myClk;
FLASH_Handle myFlash;
GPIO_Handle myGpio;
PIE_Handle myPie;
PWM_Handle myPwm1,myPwm2;

//
// Main
//
void main(void)
{
    CPU_Handle myCpu;
    PLL_Handle myPll;
    WDOG_Handle myWDog;

    //
    // Initialize all the handles needed for this application
    //
    myClk = CLK_init((void *)CLK_BASE_ADDR, sizeof(CLK_Obj));
    myCpu = CPU_init((void *)NULL, sizeof(CPU_Obj));
    myFlash = FLASH_init((void *)FLASH_BASE_ADDR, sizeof(FLASH_Obj));
    myGpio = GPIO_init((void *)GPIO_BASE_ADDR, sizeof(GPIO_Obj));
    myPie = PIE_init((void *)PIE_BASE_ADDR, sizeof(PIE_Obj));
    myPll = PLL_init((void *)PLL_BASE_ADDR, sizeof(PLL_Obj));
    myPwm1 = PWM_init((void *)PWM_ePWM1_BASE_ADDR, sizeof(PWM_Obj));
    myPwm2 = PWM_init((void *)PWM_ePWM1_BASE_ADDR, sizeof(PWM_Obj));
    myWDog = WDOG_init((void *)WDOG_BASE_ADDR, sizeof(WDOG_Obj));

    //
    // Perform basic system initialization
    //
    WDOG_disable(myWDog);
    CLK_enableAdcClock(myClk);
    (*Device_cal)();
    CLK_disableAdcClock(myClk);

    // Select the internal oscillator 1 as the clock source
    CLK_setOscSrc(myClk, CLK_OscSrc_Internal);

    // Setup the PLL for x10 /2 which will yield 50Mhz = 10Mhz * 10 / 2
    PLL_setup(myPll, PLL_Multiplier_2, PLL_DivideSelect_ClkIn_by_1);

    PIE_disable(myPie);
    PIE_disableAllInts(myPie);
    CPU_disableGlobalInts(myCpu);
    CPU_clearIntFlags(myCpu);


    PIE_setDebugIntVectorTable(myPie);
    PIE_enable(myPie);

    PIE_registerPieIntHandler(myPie, PIE_GroupNumber_3, PIE_SubGroupNumber_1,(intVec_t)&epwm1_isr);






    CLK_disableTbClockSync(myClk);
    // Initialize the EPwm Timers used in this example
    InitEPwmTimer();
    CLK_enableTbClockSync(myClk);


#ifdef _FLASH

    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
#endif

    //
    // Initialize counters
    //
    EPwm1TimerIntCount = 0;

    LoopCount = 0;

    //
    // Enable CPU INT3 which is connected to EPwm1-3 INT
    //
    CPU_enableInt(myCpu, CPU_IntNumber_3);
    // Enable EPwm INTn in the PIE: Group 3 interrupt 1-3.

    PIE_enablePwmInt(myPie, PWM_Number_1);
    PIE_enablePwmInt(myPie, PWM_Number_2);
    // Enable global Interrupts and higher priority real-time debug events
    CPU_enableGlobalInts(myCpu);
    CPU_enableDebugInt(myCpu);


    PWM_setCmpA(myPwm1,500);
    PWM_setCmpB(myPwm2,250);

    for(;;)
    {
        //
        // This loop will be interrupted, so the overall
        // delay between pin toggles will be longer.
        //
//        DELAY_US(DELAY);
        LoopCount++;

        //
        // Toggle GPIO
        //
    }
}

//
// InitEPwmTimer -
//
void
InitEPwmTimer()
{
    CLK_disableTbClockSync(myClk);
    CLK_enablePwmClock(myClk, PWM_Number_1);
    GPIO_setPullUp(myGpio, GPIO_Number_0, GPIO_PullUp_Disable);
    GPIO_setMode(myGpio, GPIO_Number_0, GPIO_0_Mode_EPWM1A);


    CLK_enablePwmClock(myClk, PWM_Number_2);
    GPIO_setPullUp(myGpio, GPIO_Number_1, GPIO_PullUp_Disable);
        GPIO_setMode(myGpio, GPIO_Number_1, GPIO_1_Mode_EPWM1B);
    //
    // Allow each timer to be sync'ed
    //
    PWM_enableCounterLoad(myPwm1);
    PWM_enableCounterLoad(myPwm2);

    // Set the phase
    PWM_setPhase(myPwm1, 0);
    PWM_setPhase(myPwm2, 0);

    PWM_setPeriod(myPwm1, PWM1_TIMER_TBPRD);
    PWM_setCounterMode(myPwm1, PWM_CounterMode_Up);         // Count up
    PWM_disableCounterLoad(myPwm1);


    PWM_setPeriod(myPwm2, PWM2_TIMER_TBPRD);
    PWM_setCounterMode(myPwm2, PWM_CounterMode_UpDown);         // Count up
    PWM_disableCounterLoad(myPwm2);
    //
    // Select INT on Zero event
    //
    PWM_setIntMode(myPwm1, PWM_IntMode_CounterEqualZero);
    PWM_enableInt(myPwm1);                        // Enable INT

    PWM_setIntMode(myPwm2, PWM_IntMode_CounterEqualZero);
    PWM_enableInt(myPwm2);
    //
    // Generate INT on 1st event
    //
    PWM_setIntPeriod(myPwm1, PWM_IntPeriod_FirstEvent);
    PWM_setActionQual_Zero_PwmA(myPwm1, PWM_ActionQual_Set);
    PWM_setActionQual_CntUp_CmpA_PwmA(myPwm1, PWM_ActionQual_Clear);
    PWM_setClkDiv(myPwm1,PWM_ClkDiv_by_1);
    PWM_setHighSpeedClkDiv(myPwm1, PWM_HspClkDiv_by_1);

    PWM_setIntPeriod(myPwm2, PWM_IntPeriod_SecondEvent);
    PWM_setActionQual_Zero_PwmB(myPwm2, PWM_ActionQual_Set);
    PWM_setActionQual_CntUp_CmpB_PwmB(myPwm2, PWM_ActionQual_Clear);
    PWM_setActionQual_CntDown_CmpB_PwmB(myPwm2, PWM_ActionQual_Set);
    PWM_setClkDiv(myPwm2,PWM_ClkDiv_by_1);
    PWM_setHighSpeedClkDiv(myPwm2, PWM_HspClkDiv_by_1);
}


__interrupt void
epwm1_isr(void)
{


    EPwm1TimerIntCount++;
    //
    // Clear INT flag for this timer
    //
    PWM_clearIntFlag(myPwm1);
    PWM_clearIntFlag(myPwm2);

    //
    // Acknowledge this interrupt to receive more interrupts from group 3
    //
    PIE_clearInt(myPie, PIE_GroupNumber_3);
}
