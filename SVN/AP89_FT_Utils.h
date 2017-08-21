/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*                                                                            
*	Source title:		AP89_FT_Utils.h                                    
*		Created from Maxim Template                         
*       Template Written and maintained by:								 
*			Patrick Hayes (Patrick.Hayes@MaximIntegrated.com)	                 
*			Mark Bruening (Mark.Bruening@MaximIntegrated.com)                      
*		  
*	URL:				$URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/AP89_FT_Utils.h $
*   Last Modified by:	$LastChangedBy: govit.pimthong $                                       
*	Date:				$Date: 2017-07-31 06:41:45 +0700 (Mon, 31 Jul 2017) $                                                
*   Current Rev:		$Revision: 67435 $
*                                                                            
*	Description:  General Purpose User Functions for this product
*                                                                            	
*
*   User Revision History                                                    
*                                                                            
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/

namespace DUT
{
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
//		declare user functions here
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	void Initiate_TestMode();

	namespace BSTUV{

		SiteInt SetSlaveAddress (int iSlaveAddress);
		SiteInt Write (int iReg, SiteInt siData);
		SiteInt Write (int iReg, int iData);
		SiteInt Write (int iAddr, int iReg, SiteInt siData);
		SiteInt Write (int iAddr, int iReg, int iData);	

	}
}

extern PinListData VCC_MeasV();
extern PinListData NDRV_MeasV();
extern PinListData CS_MeasV();

extern void ScratchPad_XY_ReadBack(int &DSIndex, int iWaferAddr = 0xD0, int iXAddr = 0xD1, int iYAddr = 0xD2);
extern void ScratchPad_XY_Write(int &DSIndex, int iWaferAddr = 0xC0, int iXAddr = 0xC1, int iYAddr = 0xC2);

extern PinListData MeasureTime(PME_INT QtmuPin, int* SourcePins, int* ReceivePins, LPCTSTR ArmSource, int NumSamples, void (*FunctionCall)(LPCTSTR, int), LPCTSTR PatternLabel, 
	int QTMU_Timeout, int StartPinType, int StopPinType, int StartSlope, int StopSlope, int StartHoldOffCount, int StopHoldOffCount, double StartTrig, double StopTrig,
	int StartRetriggerHoldoff, int StopRetriggerHoldoff);

extern PinListData APU_AWGThreshold(PME_INT Force_Pin, LPCTSTR AwgName, int Force_VRange, int Force_IRange, 
								  double Start_Force, double Stop_Force, double Step_Size, int MCU_Divider, PME_INT Meas_Pin, double Compare_Val, 
								  int Threshold_Direction, int settling_time, int forcemode, int adcmode, int connectmode = APU12_PIN_TO_VI, int iSampeToSkip = 0, bool bFromRight = false);

extern PinListData Test_Ramp_Threshold_Char(double dStart_Force, double dStop_Force, double dStep_Size, SiteDouble sdCompare_Val, int iThreshold_Direction, void (*SetFunc)(double), PinListData (*MeasFunc)(void));

extern PinListData pldFreqLow;
extern PinListData pldFreqHigh;
extern SiteDouble compute_ss(double center_freq = 400e3);
extern PinListData Binary_Search(double dBegin, double dEnd, double dAccu, int Direction, SiteDouble sdThres, void (*SetFunc)(SiteDouble), PinListData (*MeasFunc)(void));
