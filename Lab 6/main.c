#include "DSP28x_Project.h"     // DSP28x Headerfile

#include "f2802x_common/include/clk.h"
#include "f2802x_common/include/pll.h"
#include "f2802x_common/include/wdog.h"
#include "f2802x_common/include/adc.h"
#include "f2802x_common/include/pie.h"

#ifdef _FLASH
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
#endif

float ADC_in;
int LoopCount;
int ADC_counter;

__interrupt void adc_isr(void);

ADC_Handle myAdc;
CLK_Handle myClk;
PLL_Handle myPll;
WDOG_Handle myWDog;

PIE_Handle myPie;
CPU_Handle myCpu;

int main(void)
{
    myWDog = WDOG_init((void *)WDOG_BASE_ADDR, sizeof(WDOG_Obj));
    myAdc = ADC_init((void *)ADC_BASE_ADDR, sizeof(ADC_Obj));
    myClk = CLK_init((void *)CLK_BASE_ADDR, sizeof(CLK_Obj));
    myPll = PLL_init((void *)PLL_BASE_ADDR, sizeof(PLL_Obj));

    myPie = PIE_init((void *)PIE_BASE_ADDR, sizeof(PIE_Obj));
    myCpu = CPU_init((void *)NULL, sizeof(CPU_Obj));
    WDOG_disable(myWDog);
    CLK_enableAdcClock(myClk);
    (*Device_cal)();

    CLK_setOscSrc(myClk, CLK_OscSrc_Internal);

    PLL_setup(myPll, PLL_Multiplier_12, PLL_DivideSelect_ClkIn_by_2);

    PIE_disable(myPie);
    PIE_disableAllInts(myPie);
    CPU_disableGlobalInts(myCpu);
    CPU_clearIntFlags(myCpu);

    PIE_setDebugIntVectorTable(myPie);
    PIE_enable(myPie);

    PIE_registerPieIntHandler(myPie, PIE_GroupNumber_10, PIE_SubGroupNumber_1,
                                      (intVec_t)&adc_isr);

    ADC_enableBandGap(myAdc);
    ADC_enableRefBuffers(myAdc);
    ADC_powerUp(myAdc);
    ADC_enable(myAdc);
    ADC_setVoltRefSrc(myAdc, ADC_VoltageRefSrc_Int);

    ADC_setIntPulseGenMode(myAdc, ADC_IntPulseGenMode_Prior);

    ADC_enableInt(myAdc, ADC_IntNumber_1);

    ADC_setIntMode(myAdc, ADC_IntNumber_1, ADC_IntMode_ClearFlag);

    ADC_setIntSrc(myAdc, ADC_IntNumber_1, ADC_IntSrc_EOC1);

    ADC_setSocChanNumber (myAdc, ADC_SocNumber_1, ADC_SocChanNumber_A0);

    ADC_setSocTrigSrc(myAdc, ADC_SocNumber_1, ADC_SocTrigSrc_CpuTimer_0);

    ADC_setSocSampleWindow(myAdc, ADC_SocNumber_1,
                                 ADC_SocSampleWindow_14_cycles);

    CPU_enableInt(myCpu, CPU_IntNumber_10);
    PIE_enableAdcInt(myPie, ADC_IntNumber_1);

    CPU_enableGlobalInts(myCpu);
    CPU_enableDebugInt(myCpu);


    LoopCount = 0;
    ADC_counter = 0;

    while (1)
      LoopCount++;

    return 0;
}


__interrupt void
adc_isr(void)
{
    ADC_in = (3.3/4096)*ADC_readResult(myAdc, ADC_ResultNumber_1);

    ADC_clearIntFlag(myAdc, ADC_IntNumber_1);
    PIE_clearInt(myPie, PIE_GroupNumber_10);
    ADC_counter++;
    return;
}
