#include <stdio.h>
#include "platform.h"
#include "xsysmon.h"
#include "xparameters.h"
#include "xstatus.h"
#include "sleep.h"

#define SYSMON_DEVICE_ID 	XPAR_SYSMON_0_DEVICE_ID

static int SysMonPolledPrintfExample(u16 SysMonDeviceId);
static int SysMonFractionToInt(float FloatNum);

static XSysMon SysMonInst;      /* System Monitor driver instance */

int main(void)
{
	int Status;

	while(1)
	{
		Status = SysMonPolledPrintfExample(SYSMON_DEVICE_ID);
		if (Status != XST_SUCCESS)
			return XST_FAILURE;
		sleep(10);
	}
}

/**
* This function runs a test on the System Monitor/ADC device using the
* driver APIs.
* This function does the following tasks:
*	- Initiate the System Monitor device driver instance
*	- Run self-test on the device
*	- Setup the sequence registers to continuously monitor on-chip temperature, VCCINT and VCCAUX
*	- Setup configuration registers to start the sequence
*	- Read the latest on-chip temperature, VCCINT and VCCAUX
*/
int SysMonPolledPrintfExample(u16 SysMonDeviceId)
{
	int Status;
	XSysMon_Config *ConfigPtr;
	u32 TempRawData;
	u32 VccAuxRawData;
	u32 VccIntRawData;
	float TempData;
	float VccAuxData;
	float VccIntData;
	float MaxData;
	float MinData;
	XSysMon *SysMonInstPtr = &SysMonInst;

	printf("\r\nEntering the SysMon Polled Example. \r\n");

	/*
	 * Initialize the SysMon driver.
	 */
	ConfigPtr = XSysMon_LookupConfig(SysMonDeviceId);
	if (ConfigPtr == NULL)
		return XST_FAILURE;
	XSysMon_CfgInitialize(SysMonInstPtr, ConfigPtr, ConfigPtr->BaseAddress);

	/*
	 * Self Test the System Monitor/ADC device
	 */
	Status = XSysMon_SelfTest(SysMonInstPtr);
	if (Status != XST_SUCCESS)
		return XST_FAILURE;

	/*
	 * Disable the Channel Sequencer before configuring the Sequence
	 * registers.
	 */
	XSysMon_SetSequencerMode(SysMonInstPtr, XSM_SEQ_MODE_SINGCHAN);

	/*
	 * Disable all the alarms in the Configuration Register 1.
	 */
	XSysMon_SetAlarmEnables(SysMonInstPtr, 0x0);

	/*
	 * Setup the Averaging to be done for the channels in the
	 * Configuration 0 register as 16 samples:
	 */
	XSysMon_SetAvg(SysMonInstPtr, XSM_AVG_16_SAMPLES);

	/*
	 * Setup the Sequence register for 1st Auxiliary channel
	 * Setting is:
	 *	- Add acquisition time by 6 ADCCLK cycles.
	 *	- Bipolar Mode
	 *
	 * Setup the Sequence register for 16th Auxiliary channel
	 * Setting is:
	 *	- Add acquisition time by 6 ADCCLK cycles.
	 *	- Unipolar Mode
	 */
	Status = XSysMon_SetSeqInputMode(SysMonInstPtr, XSM_SEQ_CH_AUX00);
	if (Status != XST_SUCCESS)
		return XST_FAILURE;

	Status = XSysMon_SetSeqAcqTime(SysMonInstPtr, XSM_SEQ_CH_AUX15 |
						XSM_SEQ_CH_AUX00);
	if (Status != XST_SUCCESS)
		return XST_FAILURE;

	/*
	 * Enable the averaging on the following channels in the Sequencer
	 * registers:
	 * 	- On-chip Temperature, VCCINT/VCCAUX  supply sensors
	 * 	- 1st/16th Auxiliary Channels
	  *	- Calibration Channel
	 */
	Status = XSysMon_SetSeqAvgEnables(SysMonInstPtr, XSM_SEQ_CH_TEMP |
						XSM_SEQ_CH_VCCINT |
						XSM_SEQ_CH_VCCAUX |
						XSM_SEQ_CH_AUX00  |
						XSM_SEQ_CH_AUX15  |
						XSM_SEQ_CH_CALIB);
	if (Status != XST_SUCCESS)
		return XST_FAILURE;

	/*
	 * Enable the following channels in the Sequencer registers:
	 * 	- On-chip Temperature, VCCINT/VCCAUX supply sensors
	 * 	- 1st/16th Auxiliary Channel
	 *	- Calibration Channel
	 */
	Status =  XSysMon_SetSeqChEnables(SysMonInstPtr, XSM_SEQ_CH_TEMP |
						XSM_SEQ_CH_VCCINT |
						XSM_SEQ_CH_VCCAUX |
						XSM_SEQ_CH_AUX00  |
						XSM_SEQ_CH_AUX15  |
						XSM_SEQ_CH_CALIB);
	if (Status != XST_SUCCESS)
		return XST_FAILURE;

	/*
	 * Set the ADCCLK frequency equal to 1/32 of System clock for the System
	 * Monitor/ADC in the Configuration Register 2.
	 */
	XSysMon_SetAdcClkDivisor(SysMonInstPtr, 32);

	/*
	 * Set the Calibration enables.
	 */
	XSysMon_SetCalibEnables(SysMonInstPtr,
				XSM_CFR1_CAL_PS_GAIN_OFFSET_MASK |
				XSM_CFR1_CAL_ADC_GAIN_OFFSET_MASK);

	/*
	 * Enable the Channel Sequencer in continuous sequencer cycling mode.
	 */
	XSysMon_SetSequencerMode(SysMonInstPtr, XSM_SEQ_MODE_CONTINPASS);

	/*
	 * Wait till the End of Sequence occurs
	 */
	XSysMon_GetStatus(SysMonInstPtr); /* Clear the old status */
	while ((XSysMon_GetStatus(SysMonInstPtr) & XSM_SR_EOS_MASK) != XSM_SR_EOS_MASK);

	/*
	 * Read the on-chip Temperature Data (Current/Maximum/Minimum)
	 */
	TempRawData = XSysMon_GetAdcData(SysMonInstPtr, XSM_CH_TEMP);
	TempData = XSysMon_RawToTemperature(TempRawData);
	printf("\r\nThe Current Temperature is %0d.%03d Centigrades.\r\n",
				(int)(TempData), SysMonFractionToInt(TempData));

	TempRawData = XSysMon_GetMinMaxMeasurement(SysMonInstPtr, XSM_MAX_TEMP);
	MaxData = XSysMon_RawToTemperature(TempRawData);
	printf("The Maximum Temperature is %0d.%03d Centigrades. \r\n",
				(int)(MaxData), SysMonFractionToInt(MaxData));

	TempRawData = XSysMon_GetMinMaxMeasurement(SysMonInstPtr, XSM_MIN_TEMP);
	MinData = XSysMon_RawToTemperature(TempRawData);
	printf("The Minimum Temperature is %0d.%03d Centigrades. \r\n",
				(int)(MinData), SysMonFractionToInt(MinData));

	/*
	 * Read the on-chip VCCINT Data (Current/Maximum/Minimum)
	 */
	VccIntRawData = XSysMon_GetAdcData(SysMonInstPtr, XSM_CH_VCCINT);
	VccIntData = XSysMon_RawToVoltage(VccIntRawData);
	printf("\r\nThe Current VCCINT is %0d.%03d Volts. \r\n",
			(int)(VccIntData), SysMonFractionToInt(VccIntData));

	VccIntRawData = XSysMon_GetMinMaxMeasurement(SysMonInstPtr, XSM_MAX_VCCINT);
	MaxData = XSysMon_RawToVoltage(VccIntRawData);
	printf("The Maximum VCCINT is %0d.%03d Volts. \r\n",
			(int)(MaxData), SysMonFractionToInt(MaxData));

	VccIntRawData = XSysMon_GetMinMaxMeasurement(SysMonInstPtr, XSM_MIN_VCCINT);
	MinData = XSysMon_RawToVoltage(VccIntRawData);
	printf("The Minimum VCCINT is %0d.%03d Volts. \r\n",
			(int)(MinData), SysMonFractionToInt(MinData));

	/*
	 * Read the VccAux Votage Data (Current/Maximum/Minimum)
	 */
	VccAuxRawData = XSysMon_GetAdcData(SysMonInstPtr, XSM_CH_VCCAUX);
	VccAuxData = XSysMon_RawToVoltage(VccAuxRawData);
	printf("\r\nThe Current VCCAUX is %0d.%03d Volts. \r\n",
			(int)(VccAuxData), SysMonFractionToInt(VccAuxData));

	VccAuxRawData = XSysMon_GetMinMaxMeasurement(SysMonInstPtr, XSM_MAX_VCCAUX);
	MaxData = XSysMon_RawToVoltage(VccAuxRawData);
	printf("The Maximum VCCAUX is %0d.%03d Volts. \r\n",
				(int)(MaxData), SysMonFractionToInt(MaxData));


	VccAuxRawData = XSysMon_GetMinMaxMeasurement(SysMonInstPtr, XSM_MIN_VCCAUX);
	MinData = XSysMon_RawToVoltage(VccAuxRawData);
	printf("The Minimum VCCAUX is %0d.%03d Volts. \r\n\r\n",
				(int)(MinData), SysMonFractionToInt(MinData));

	printf("Exiting the SysMon Polled Example. \r\n");

	return XST_SUCCESS;
}

/**
* @param	FloatNum is the floating point number.
* @return	Integer number to a precision of 3 digits.
*****************************************************************************/
int SysMonFractionToInt(float FloatNum)
{
	float Temp;
	Temp = FloatNum;
	if (FloatNum < 0)
		Temp = -(FloatNum);

	return( ((int)((Temp -(float)((int)Temp)) * (1000.0f))));
}
