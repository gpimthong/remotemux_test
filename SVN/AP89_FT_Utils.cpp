/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*                                                                            
*	Source title:		AP89_FT_Utils.cpp                                     
*		Created from Maxim Template                         
*		Template Written and maintained by:								 
*			Patrick Hayes (Patrick.Hayes@MaximIntegrated.com)	                 
*			Mark Bruening (Mark.Bruening@MaximIntegrated.com)                      
*
*	URL:				$URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/AP89_FT_Utils.cpp $
*	Last Modified by:	$LastChangedBy: govit.pimthong $                                       
*	Date:				$Date: 2017-07-31 06:41:45 +0700 (Mon, 31 Jul 2017) $                                                
*	Current Rev:		$Revision: 67435 $
*
*	Description:  General Purpose User Functions for this product
*
*
*	User Revision History                                                    
*
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/

#include "AP89_FT.h"

PinListData VCC_MeasV();
PinListData NDRV_MeasV();
PinListData CS_MeasV();

namespace DUT
{
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// 		define user functions
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+



	/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
	* Initiate_TestMode
	*
	*   Purpose: Put the device into test mode
	*
	*	Detail: Test Mode can be entered by pulling FSEN below ground while giving three consecutive rising edges on DIM, 
	*		Once the Test Mode condition is entered it is latched.
	*		In Test Mode the device is forced to operate in I2C mode even though Stand Alone mode was previously set.
	*		In this way it is possible to access at the AP89 register map. Also in Test Mode the timing of SS_WAIT and SS_RUN are accelerated by 8X.
	*		This allows to save test time during functional checks.                
	**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
	void Initiate_TestMode(){


		//DIM at 0V
		dpinforce("DIM", "0", 3.0, 0.1, MS_ALL);
		cbitclose(K40); //DIM to DPU
		
		apu12set(FSEN, APU12_FV, 0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI);
		lwait (1 ms_f);

		apu12set(FSEN, APU12_FI, -2 mA_f, APU12_10V, APU12_10MA, APU12_PIN_TO_VI);
		lwait (1 ms_f);
		
		//3 rising edge at DIM
		dpinforce("DIM", "1", 3.0, 0.1, MS_ALL);
		dpinforce("DIM", "0", 3.0, 0.1, MS_ALL);
		dpinforce("DIM", "1", 3.0, 0.1, MS_ALL);
		dpinforce("DIM", "0", 3.0, 0.1, MS_ALL);
		dpinforce("DIM", "1", 3.0, 0.1, MS_ALL);
		dpinforce("DIM", "0", 3.0, 0.1, MS_ALL);
		lwait (500 us_f);

		apu12set(FSEN, APU12_FV, 0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI);
		dpinforce("DIM", "1", 3.0, 0.1, MS_ALL);

	}



	namespace TestMode
	{

		//Test mode for BG measurement
		void OTP_BG(){

		}




	}


	namespace BSTUV
	{

		SiteInt SetSlaveAddress (int iSlaveAddress)
		{
			static int iPrevAddress = -1000;
			//Check to see if different than last address
			if (iPrevAddress == iSlaveAddress)
				return SiteInt(0);

			SiteInt siStatus;
			int iStatus;
			__int64 i64SlaveAddress[7];

			for (int i=0, j=6; i < 7; i++, j--)
				if( 1<<i & iSlaveAddress )
					i64SlaveAddress[j]=0xFFFF;
				else
					i64SlaveAddress[j]=0x0000;

			iStatus = dpinvecblockwrite ( "SDA", "i2c_slave_id_w_bst_uv", DPIN_DRVDATA, 0, 7, i64SlaveAddress, MS_ALL );
			for (int iSite = 0; iSite<giNumSites; iSite++)
				siStatus[iSite] = iStatus;
			iStatus = dpinvecblockwrite ( "SDA", "i2c_slave_id_r_bst_uv", DPIN_DRVDATA, 0, 7, i64SlaveAddress, MS_ALL );
			for (int iSite = 0; iSite<giNumSites; iSite++)
				siStatus[iSite] = int(siStatus[iSite]) | iStatus;

			return siStatus;
		}
	
		
		SiteInt SendCommand (int iCommand)	// multi-site
		{
			SiteInt siStatus;
			int iStatus;
			int	i, j;
			__int64 i64FailPins;
			__int64 i64Comm[8];

			for (i=0, j=7; i < 8; i++, j--)
				if( 1<<i & iCommand )
					i64Comm[j]=0xFFFF;
				else
					i64Comm[j]=0x0000;
			iStatus = dpinvecblockwrite ( "SDA", "i2c_send_command_bst_uv", DPIN_DRVDATA, 3, 8, i64Comm, MS_ALL );

			iStatus |= mcurun( NULL, "i2c_send_command_bst_uv");

			for (int iSite=0; iSite<giNumSites; iSite++)
				if (msSiteStat(iSite))
					siStatus[iSite] = iStatus | dpingetfailpins( "SDA", DPIN_ALL_PINS, &i64FailPins, iSite );

			return siStatus;
		}

		SiteInt SendCommand (int iAddr, int iCommand)	// multi-site
		{

			DUT::BSTUV::SetSlaveAddress(iAddr);
			return DUT::BSTUV::SendCommand(iCommand);
		}

		
		SiteInt Write (int iReg, SiteInt siData)	// multi-site
		{
			SiteInt siStatus;
			int iStatus;
			int	i, j;
			__int64 i64FailPins;
			__int64 i64Addr[8];
			__int64 i64Data[8];

			for (i=0, j=7; i < 8; i++, j--)
				if( 1<<i & iReg )
					i64Addr[j]=0xFFFF;
				else
					i64Addr[j]=0x0000;
			iStatus = dpinvecblockwrite ( "SDA", "i2c_write_1byte_bst_uv", DPIN_DRVDATA, 3, 8, i64Addr, MS_ALL );

			for(int iSite = 0; iSite < giNumSites; iSite++ )
				if (msSiteStat(iSite))
				{
					for (i=0, j=7; i < 8; i++, j--)
						if( 1<<i & siData[iSite] )
							i64Data[j]=0x1;
						else
							i64Data[j]=0x0;

					siStatus[iSite] = iStatus | dpinvecblockwrite ( "SDA", "i2c_write_1byte_bst_uv", DPIN_DRVDATA, 12, 8, i64Data, iSite );
				}

				iStatus = mcurun( NULL, "i2c_write_1byte_bst_uv");

				for (int iSite=0; iSite<giNumSites; iSite++)
					if (msSiteStat(iSite))
						siStatus[iSite] = int(siStatus[iSite]) | iStatus | dpingetfailpins( "SDA", DPIN_ALL_PINS, &i64FailPins, iSite );

				return siStatus;
		}

		SiteInt Write (int iReg, int iData)	// multi-site
		{
			SiteInt siStatus;
			int iStatus;
			int	i, j;
			__int64 i64FailPins;
			__int64 i64Addr[8];
			__int64 i64Data[8];

			for (i=0, j=7; i < 8; i++, j--)
				if( 1<<i & iReg )
					i64Addr[j]=0xFFFF;
				else
					i64Addr[j]=0x0000;
			iStatus = dpinvecblockwrite ( "SDA", "i2c_write_1byte_bst_uv", DPIN_DRVDATA, 3, 8, i64Addr, MS_ALL );

			for (i=0, j=7; i < 8; i++, j--)
				if( 1<<i & iData )
					i64Data[j]=0xFFFF;
				else
					i64Data[j]=0x0000;
			iStatus |= dpinvecblockwrite ( "SDA", "i2c_write_1byte_bst_uv", DPIN_DRVDATA, 12, 8, i64Data, MS_ALL );

			iStatus |= mcurun( NULL, "i2c_write_1byte_bst_uv");

			for (int iSite=0; iSite<giNumSites; iSite++)
				if (msSiteStat(iSite))
					siStatus[iSite] = int(siStatus[iSite]) | iStatus | dpingetfailpins( "SDA", DPIN_ALL_PINS, &i64FailPins, iSite );

			dpinforce("DIM", "1", 3.0, 0.1, MS_ALL);

			return siStatus;
		}

		SiteInt Write (int iAddr, int iReg, SiteInt siData)	// multi-site
		{
			DUT::BSTUV::SetSlaveAddress(iAddr);
			return DUT::BSTUV::Write(iReg, siData);
		}

		SiteInt Write (int iAddr, int iReg, int iData)	// multi-site
		{
			DUT::BSTUV::SetSlaveAddress(iAddr);
			return DUT::BSTUV::Write(iReg,iData);
		}

	}

}



/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
* MeasureTime
*   Purpose: This functions will perform Time Measurements and return the value in ns
*	Arguments:	
*		[QtmuPin]		QTMU pin as defined in Pin Map Editor
*		[SourcePins]	The source pin which the stimulus is going to be applied on (if less than 0, use Direct method).
*		[ReceivePins]	The source pin which the delay of the stimulus is going to be measured on (if less than 0, use Direct method).
*		[ArmSource]		The input signal to connect to the arm channel of the TMU. This can be a PSQ group name defined in the Vector Editor.
*		[NumSamples]	Number of samples to be captured.
*		[FunctionCall]	Pointer to function to be called. It can be specified as NULL
*		[PatternLabel]	String of pattern label to be called. It can be specified as NULL
*		[QTMU_Timeout]	TMU time out.
*		[StartPinType]	The actual input from the specified Mode for qtmustart().  
*		[StopPinType]	The actual input from the specified Mode for qtmustop().  
*		[StartSlope]	The slope of the source signal for qtmustart(). If unspecified, will default to QTMU_POS.  
*		[StopSlope]		The slope of the source signal for qtmustop(). If unspecified, will default to QTMU_POS.  
*		[StartHoldOffCount]	The number of events to be ignored for start signal
*		[StopHoldOffCount]	The number of events to be ignored for stop signal
*		[StartTrig]		The qtmustart() trigger level. Only valid for direct sources.  Range is �50 V. 
*		[StopTrig]		The qtmustop() trigger level. Only valid for direct sources.  Range is �50 V. 
*		[StartRetriggerHoldoff]	The lowpass filter value in nsec for start signal. Default is 0
*		[StopRetriggerHoldoff]	The lowpass filter value in nsec for stop signal. Default is 0
*	Returns:
*		[PinListData]  Measured Times is returned as PinListData.
*   Example usage (4 sites per sector):
*			PinListData sdResults;
*			int NumSamples = 10;
*			int din1[NUM_SITES] = {12, 8, 4, 0};		// Digital Channels connected to pin din1 for each site
*			int din2[NUM_SITES] = {13, 9, 5, 1};		// Digital Channels connected to pin din2 for each site
*			int direct_v[NUM_SITES] = {-1, -1, -1, -1}	// To do direct QTMU measurements
*
*			// Period Measurement on Digital Line
*			sdResults = MeasureTime(TMU_GP, din1, din1, "AUTO", 1, NULL, "", 100 us_f, QTMU_RCVL, QTMU_RCVH, QTMU_NEG, QTMU_NEG, 0, 0, 1.62, 1.24);
*			Maxim::DatalogAll(DSIndex, sdResults);
*
*			// Propagation Delay Measurement - Digital Line to Digital Line
*			sdResults = MeasureTime(TMU_GP, din1, din2, "AUTO", 1, propagation_delay_trigger, "", 100 ms_f, QTMU_RCVL, QTMU_RCVL, QTMU_NEG, QTMU_NEG, 0, 0, 0.2, 1.5);
*			Maxim::DatalogAll(DSIndex, sdResults);
*		
*			// Pulse Measurement on Digital Line
*			sdResults = MeasureTime(TMU_GP, din1, din1, "AUTO", 1, NULL, "", 100 us_f, QTMU_RCVL, QTMU_RCVH, QTMU_NEG, QTMU_NEG, 0, 0, 1.62, 1.24);
*			Maxim::DatalogAll(DSIndex, sdResults);
*
*			// Period Measurement Direct QTMU
*			sdResults = MeasureTime(TMU_GP, direct_v, direct_v, "AUTO", 1, NULL, "", 100 us_f, QTMU_DIRECTA, QTMU_DIRECTB, QTMU_NEG, QTMU_NEG, 0, 0, 1.62, 1.24);
*			Maxim::DatalogAll(DSIndex, sdResults);
*
*			// Propagation Delay Measurement - Direct QTMU to Digital Line
*			sdResults = MeasureTime(TMU_GP, direct_v, din2, "AUTO", 1, propagation_delay_trigger, "", 1 ms_f, QTMU_DIRECTA, QTMU_RCVL, QTMU_NEG, QTMU_NEG, 0, 0, 0.2, 1.5);
*			Maxim::DatalogAll(DSIndex, sdResults);
*		
*			// Pulse Measurement using Direct QTMU connection
*			sdResults = MeasureTime(TMU_GP, direct_v, direct_v, "AUTO", 1, NULL, "", 100 us_f, QTMU_DIRECTA, QTMU_DIRECTB, QTMU_NEG, QTMU_NEG, 0, 0, 1.62, 1.24);
*			Maxim::DatalogAll(DSIndex, sdResults);
*
*   Revision History:
*		3/17/2012: Initial Release (Angelo Portelli) based on Jeffrey Jonathan function
*                                                                            
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
int din1[NUM_SITES] = {13, 9, 5, 1};		// Digital Channels connected to pin DPU_OUT for each site
int din2[NUM_SITES] = {12, 8, 4, 0};		// Digital Channels connected to pin SHARED_DPU for each site
int direct_v[NUM_SITES] = {-1, -1, -1, -1};	// To do direct QTMU measurements

PinListData MeasureTime(PME_INT QtmuPin, int* SourcePins, int* ReceivePins, LPCTSTR ArmSource, int NumSamples, void (*FunctionCall)(LPCTSTR, int), LPCTSTR PatternLabel, 
	int QTMU_Timeout, int StartPinType, int StopPinType, int StartSlope, int StopSlope, int StartHoldOffCount, int StopHoldOffCount, double StartTrig, double StopTrig,
	int StartRetriggerHoldoff, int StopRetriggerHoldoff)
{
	PinListData pldTemp;
	PinListData pldResult;		//((GME_INT)QtmuPin);
	int TotalLoop;			
	char buffer[20];
	int TmuTempGroupSet = 30000;	//Select an large arbitrary that is probably not being used.
	int isite;

	RESULTS_STR* Results;
	int ResultsSize = giNumSites;	//PME_INT only has one pin
	Results = new RESULTS_STR[ResultsSize];
	for (isite = 0; isite < NUM_SITES; isite++) 
		if (msSiteStat(isite)) 
			Results[isite].site = isite;
		else
			Results[isite].site = -2;
	pldResult = PinListData(Results, 1, ResultsSize);	//PME_INT only has one pin

	double start_level_high, start_level_low;
	double stop_level_high, stop_level_low;
	int code_sites = 0;

	if (SourcePins[0] < 0 && ReceivePins[0] < 0)
	{
		Maximqtmustart( QtmuPin, "OFF", QTMU_RCVH, QTMU_POS, 0, QTMU_NOFILTER, 0.0 );
		Maximqtmustop( QtmuPin, "OFF", QTMU_RCVH, QTMU_POS, 0, QTMU_NOFILTER, 0.0 );	
		Maximqtmumode ( QtmuPin, QTMU_TIMER, QTMU_ARMOUTOFF );
		lwait( 1 ms_f );		
		Maximqtmustart( QtmuPin, "Direct", QTMU_DIRECTA, StartSlope, StartHoldOffCount, StartRetriggerHoldoff, StartTrig );
		Maximqtmustop( QtmuPin, "Direct", QTMU_DIRECTB, StopSlope, StopHoldOffCount, StopRetriggerHoldoff, StopTrig );

		//Arm the tmu based on the ArmSource
		Maximqtmuarm  ( QtmuPin, ArmSource, NumSamples );
		lwait( 100 us_f );		

		//Pointer to external function.
		if (FunctionCall != NULL) FunctionCall(PatternLabel, 0xF);
		//End trigger

		//Measure the time between triggers.
		pldTemp = Maximqtmumt( QtmuPin, QTMU_READTIME, QTMU_Timeout, 1, 1, 1 );
		for (isite = 0; isite < NUM_SITES; isite++) pldResult[isite] = pldTemp[isite];

		Maximqtmumode( QtmuPin, QTMU_OFF );
	}
	else
	{
		if (SourcePins == ReceivePins)
		{
			if (StartTrig > StopTrig)
			{
				start_level_high = stop_level_high = StartTrig;
				start_level_low = stop_level_low = StopTrig;
			}
			else
			{
				start_level_low = stop_level_low = StartTrig;
				start_level_high = stop_level_high = StopTrig;
			}
		}
		else
		{
			switch(StartPinType)
			{
			case QTMU_RCVL:
				start_level_high = StartTrig + 0.5;
				start_level_low = StartTrig;
				break;
			case QTMU_RCVH:
				start_level_high = StartTrig;
				start_level_low = StartTrig - 0.5;
				break;
			}

			switch(StopPinType)
			{
			case QTMU_RCVL:
				stop_level_high = StopTrig + 0.5;
				stop_level_low = StopTrig;
				break;
			case QTMU_RCVH:
				stop_level_high = StopTrig;
				stop_level_low = StopTrig - 0.5;
				break;
			}
		}
		// Determine the total number of looping required
		TotalLoop = ResultsSize / 2;

		if (TotalLoop > 0)
		{
			for(int Loop = 0; Loop<TotalLoop; Loop++)
			{
				if (msSiteStat(Loop*2)+msSiteStat(Loop*2+1)) 
				{
					code_sites = 0x3 << (Loop*2);
					// Put TMU in timer mode and software armed				
					if ((msSiteStat(Loop*2)) && !(msSiteStat(Loop*2+1)))
						groupset( TmuTempGroupSet, "TMU_TEMP", "QTMU%d", QtmuPin[Loop*2]);
					else if (!(msSiteStat(Loop*2)) && (msSiteStat(Loop*2+1)))
						groupset( TmuTempGroupSet, "TMU_TEMP", "QTMU%d", QtmuPin[Loop*2+1]);
					else
						groupset( TmuTempGroupSet, "TMU_TEMP", "QTMU%d, QTMU%d", QtmuPin[Loop*2], QtmuPin[Loop*2+1]);
					Maximqtmustart( TmuTempGroupSet, "OFF", QTMU_RCVH, QTMU_POS, 0, StartRetriggerHoldoff, 0.0 );
					Maximqtmustop( TmuTempGroupSet, "OFF", QTMU_RCVH, QTMU_POS, 0, StopRetriggerHoldoff, 0.0 );	
					Maximqtmumode ( TmuTempGroupSet, QTMU_TIMER, QTMU_ARMOUTOFF );

					// If source pins or receive pins are less than 0, use Direct method.
					if ((SourcePins[Loop*2] < 0) || (SourcePins[Loop*2+1] < 0))
					{
						if (msSiteStat(Loop*2))
							Maximqtmustart( QtmuPin[Loop*2], "Direct", StartPinType, StartSlope, StartHoldOffCount, StartRetriggerHoldoff, StartTrig );
						if (msSiteStat(Loop*2+1))
							Maximqtmustart( QtmuPin[Loop*2+1], "Direct", StartPinType, StartSlope, StartHoldOffCount, StopRetriggerHoldoff, StartTrig );
					}
					else 
					{
						if (msSiteStat(Loop*2))
						{
							sprintf_s( buffer, "IO%d", SourcePins[Loop*2] );
							Maximdpinrcvset( buffer, DPIN_RCV_SPLIT, start_level_high, start_level_low, MS_ALL );
							Maximqtmustart( QtmuPin[Loop*2], buffer, StartPinType, StartSlope, StartHoldOffCount, StartRetriggerHoldoff);
						}
						if (msSiteStat(Loop*2+1))
						{
							sprintf_s( buffer, "IO%d", SourcePins[Loop*2+1] );
							Maximdpinrcvset( buffer, DPIN_RCV_SPLIT, start_level_high, start_level_low, MS_ALL );
							Maximqtmustart( QtmuPin[Loop*2+1], buffer, StartPinType, StartSlope, StartHoldOffCount, StartRetriggerHoldoff);
						}
					}

					if ((ReceivePins[Loop*2] < 0) || (ReceivePins[Loop*2+1] < 0))
					{
						if (msSiteStat(Loop*2))
							Maximqtmustop( QtmuPin[Loop*2], "Direct", QTMU_DIRECTB, StopSlope, StopHoldOffCount, StopRetriggerHoldoff, StopTrig );
						if (msSiteStat(Loop*2+1))
							Maximqtmustop( QtmuPin[Loop*2+1], "Direct", QTMU_DIRECTB, StopSlope, StopHoldOffCount, StopRetriggerHoldoff, StopTrig );
					}
					else 
					{
						if (msSiteStat(Loop*2))
						{
							sprintf_s( buffer, "IO%d", ReceivePins[Loop*2] );
							Maximdpinrcvset( buffer, DPIN_RCV_SPLIT, stop_level_high, stop_level_low, MS_ALL );
							Maximqtmustop( QtmuPin[Loop*2], buffer, StopPinType, StopSlope, StopHoldOffCount, StopRetriggerHoldoff);
						}
						if (msSiteStat(Loop*2+1))
						{
							sprintf_s( buffer, "IO%d", ReceivePins[Loop*2+1] );
							Maximdpinrcvset( buffer, DPIN_RCV_SPLIT, stop_level_high, stop_level_low, MS_ALL );
							Maximqtmustop( QtmuPin[Loop*2+1], buffer, StopPinType, StopSlope, StopHoldOffCount, StopRetriggerHoldoff);
						}
					}

					//Arm the tmu based on the ArmSource
					Maximqtmuarm  ( TmuTempGroupSet, ArmSource, NumSamples );
					lwait( 100 us_f );		

					//Pointer to external function.
					if (FunctionCall != NULL) FunctionCall(PatternLabel, code_sites);
					//End trigger

					//Measure the time between triggers.
					pldTemp = Maximqtmumt( TmuTempGroupSet, QTMU_READTIME, QTMU_Timeout, 1, 1, 1 );
					pldResult[Loop*2] = pldTemp[Loop*2];
					pldResult[Loop*2+1] = pldTemp[Loop*2+1];

					Maximqtmumode( TmuTempGroupSet, QTMU_OFF );
				}
			}
		}
		else
		{
			int Loop = 0;
			if (msSiteStat(Loop*2)) 
			{
				code_sites = 0x3 << (Loop*2);
				// Put TMU in timer mode and software armed				
				if (msSiteStat(Loop*2)) groupset( TmuTempGroupSet, "TMU_TEMP", "QTMU%d", QtmuPin[Loop*2]);
				Maximqtmustart( TmuTempGroupSet, "OFF", QTMU_RCVH, QTMU_POS, 0, StartRetriggerHoldoff, 0.0 );
				Maximqtmustop( TmuTempGroupSet, "OFF", QTMU_RCVH, QTMU_POS, 0, StopRetriggerHoldoff, 0.0 );	
				Maximqtmumode ( TmuTempGroupSet, QTMU_TIMER, QTMU_ARMOUTOFF );

				// If source pins or receive pins are less than 0, use Direct method.
				if ((SourcePins[Loop*2] < 0))
				{
					if (msSiteStat(Loop*2)) Maximqtmustart( QtmuPin[Loop*2], "Direct", QTMU_DIRECTA, StartSlope, StartHoldOffCount, StartRetriggerHoldoff, StartTrig );
				}
				else 
				{
					if (msSiteStat(Loop*2))
					{
						sprintf_s( buffer, "IO%d", SourcePins[Loop*2] );
						Maximdpinrcvset( buffer, DPIN_RCV_SPLIT, start_level_high, start_level_low, MS_ALL );
						Maximqtmustart( QtmuPin[Loop*2], buffer, StartPinType, StartSlope, StartHoldOffCount, StartRetriggerHoldoff);
					}
				}

				if ((ReceivePins[Loop*2] < 0))
				{
					if (msSiteStat(Loop*2))Maximqtmustop( QtmuPin[Loop*2], "Direct", QTMU_DIRECTB, StopSlope, StopHoldOffCount, StopRetriggerHoldoff, StopTrig );
				}
				else 
				{
					if (msSiteStat(Loop*2))
					{
						sprintf_s( buffer, "IO%d", ReceivePins[Loop*2] );
						Maximdpinrcvset( buffer, DPIN_RCV_SPLIT, stop_level_high, stop_level_low, MS_ALL );
						Maximqtmustop( QtmuPin[Loop*2], buffer, StopPinType, StopSlope, StopHoldOffCount, StopRetriggerHoldoff);
					}
				}

				//Arm the tmu based on the ArmSource
				Maximqtmuarm  ( TmuTempGroupSet, ArmSource, NumSamples );
				lwait( 100 us_f );		

				//Pointer to external function.
				if (FunctionCall != NULL) FunctionCall(PatternLabel, code_sites);
				//End trigger

				//Measure the time between triggers.
				pldTemp = Maximqtmumt( TmuTempGroupSet, QTMU_READTIME, QTMU_Timeout, 1, 1, 1 );
				pldResult[Loop*2] = pldTemp[Loop*2];

				Maximqtmumode( TmuTempGroupSet, QTMU_OFF );
			}
		}
	}
	//Reset qtmu state
	Maximqtmustart( QtmuPin, "Off", StartPinType, StartSlope, 0, QTMU_NOFILTER);
	Maximqtmustop( QtmuPin, "Off", StopPinType, StopSlope, 0, QTMU_NOFILTER );

	return pldResult;
}




/// <summary>
/// ScratchPad_XY_ReadBack, Read all data from 3 registers.
/// Extract sign bit from wafer register and add to X-Y coordinate then datalog.
/// </summary>
/// <param name="DSIndex">Index of the ds.</param>
/// <param name="iWaferAddr">Register address of wafer number</param>
/// <param name="iXAddr">Register address of X coordinate</param>
/// <param name="iYAddr">Register address of Y coordinate</param>
void ScratchPad_XY_ReadBack(int &DSIndex, int iWaferAddr, int iXAddr, int iYAddr){
  
    SiteInt siWaferID, siX, siY, siEngSample, siSerial;
  
  
    //Read data from registers
    Maxim::I2C::Read(iWaferAddr,siWaferID);
    Maxim::I2C::Read(iXAddr,siX);
    Maxim::I2C::Read(iYAddr,siY);

	siSerial = siWaferID;
	siSerial = 0;
     
    //Extract sign bit of X and Y  from Wafer register
    //-------------------------------------------------------------------------
    // |   bit-7    |  bit-6 |  bit-5 | bit-4 | bit-3 | bit-2 | bit-1 | bit-0 |
    // | ENG_SAMPLE | X Sign | Y Sign |   W4  |   W3  |   W2  |   W1  |   W0  |
    //-------------------------------------------------------------------------
 
    for (int iSite = 0; iSite < NUM_SITES; iSite++)
    {
        if (msSiteStat(iSite)){

			siSerial[iSite] = siSerial[iSite] + siY[iSite] + (siX[iSite] * 10000);

             
            if (siWaferID[iSite] & 0x40){
				siSerial[iSite] = siSerial[iSite] + 10000000;
                siX[iSite] = -siX[iSite];
			}
  
            if (siWaferID[iSite] & 0x20){
				siSerial[iSite] = siSerial[iSite] + 1000;
                siY[iSite] = -siY[iSite];
			}
  
            if (siWaferID[iSite] & 0x80)
                siEngSample[iSite] = 1;
  
            //Get rid of bit [7:5] from waferID
            siWaferID[iSite] = siWaferID[iSite] & 0x1F;
  
        }
    }  
  
  
    //Datalog      
    Maxim::DatalogAll(DSIndex, siEngSample ); //Engineering sample indicator
    Maxim::DatalogAll(DSIndex, siWaferID ); //Wafer number
    Maxim::DatalogAll(DSIndex, siX ); //X Coordinate
    Maxim::DatalogAll(DSIndex, siY ); //Y Coordinate
	Maxim::DatalogAll(DSIndex, siSerial ); //Serial Number


  
}



/// <summary>
/// ScratchPad_XY_Write, Write all data to 3 registers and datalog
/// </summary>
/// <param name="DSIndex">Index of the ds.</param>
/// <param name="iWaferAddr">Register address of wafer number</param>
/// <param name="iXAddr">Register address of X coordinate</param>
/// <param name="iYAddr">Register address of Y coordinate</param>
/// <param name="bEngSample">Flag to indicate engineering sample</param>
void ScratchPad_XY_Write(int &DSIndex, int iWaferAddr, int iXAddr, int iYAddr){

	SiteInt siX, siY, siXtemp, siYtemp, siWaferID, siWaferID2, siEngSample;
	int iSite, iWaferID;


	//If in production, SUBLOT_NUMBER=wafer number
	//else in engineering, decide what to do
	if (Maxim::Operator())
	{
		char buffer[10];
		int iRet;

		iRet = GetETSVariable(SUBLOT_NUMBER, buffer, sizeof(buffer));
		buffer[iRet] = 0;
		iWaferID = atoi(buffer);
		//atoi can return 0 if not convertible.  Handle this?

		siWaferID = iWaferID;          
		siWaferID2 = iWaferID;     
		

		//Get x-y coordinate
		for  (iSite = 0; iSite < Maxim::GetNumSites(); iSite++)
		{
			int iSite2;
			iSite2 = iSite + Maxim::GetNumSites() * (GetStationNumber() - 1);

			//grab the xy coords from the driver
			int iXCoord, iYCoord;
			hpctrl(HNDLR_COORDS, iSite2, &iXCoord, &iYCoord);

			siX[iSite] = iXCoord;
			siY[iSite] = iYCoord;

			//Save for datalog later
			siXtemp[iSite] = iXCoord;
			siYtemp[iSite] = iYCoord;

			//-------------------------------------------------------------------------
			// |   bit-7    |  bit-6 |  bit-5 | bit-4 | bit-3 | bit-2 | bit-1 | bit-0 |
			// | ENG_SAMPLE | X Sign | Y Sign |   W4  |   W3  |   W2  |   W1  |   W0  |
			//-------------------------------------------------------------------------

			// Add sign bit to wafer register
			if (iXCoord < 0){
				siX[iSite] = abs(iXCoord);
				siWaferID[iSite] = siWaferID[iSite] | 0x40;
			}
			if (iYCoord < 0){
				siY[iSite] = abs(iYCoord);
				siWaferID[iSite] = siWaferID[iSite] | 0x20;
			}

			//Assign Engineering sample bit
			if (gbSample) siWaferID[iSite] = siWaferID[iSite] | 0x80;

		}


		// Write data to scratch pad register
		Maxim::I2C::Write(iWaferAddr,siWaferID);
		Maxim::I2C::Write(iXAddr,siX);
		Maxim::I2C::Write(iYAddr,siY);


		//Datalog      
		Maxim::DatalogAll(DSIndex, gbSample); //Engineering sample indicator
		Maxim::DatalogAll(DSIndex, siWaferID2); //Wafer number
		Maxim::DatalogAll(DSIndex, siXtemp); //X Coordinate
		Maxim::DatalogAll(DSIndex, siYtemp); //Y Coordinate


	}
	else
	{
		//Could ask Engineer for wafer number or query SE-Probe every insertion?
		iWaferID = 31;
		siWaferID2 = iWaferID;     

		char buffer[BUFSIZ];
		SiteInt DeviceNum;

		//Get actual DUT serial number.//Code from PI27
		for(int isite = 0; isite<NUM_SITES; isite++) 
		{
			if(msSiteStat(isite))
			{
				msGetSerialNumberString(isite,buffer,BUFSIZ);
				DeviceNum[isite] = strtol(buffer,NULL,10);
				siX[isite] = DeviceNum[isite] & 0xFF;
				siY[isite] = DeviceNum[isite] & 0xFF00;
				siY[isite] = siY[isite] >> 8;
			}
		}

		//Store Random batch number in 3rd scratch pad register (0xC0)
		//Also electronically mark as sample by set MSB bit to '1' , if not using production OI
		if (gbSample)
			iWaferID = giRandomBatchCode | 0x80;
		else
			iWaferID = giRandomBatchCode & 0x7F; //mask out MSB , set '0'


		siWaferID2 = giRandomBatchCode & 0x7F;


		// Write data to scratch pad register
		Maxim::I2C::Write(iWaferAddr,iWaferID);
		Maxim::I2C::Write(iXAddr,siX);
		Maxim::I2C::Write(iYAddr,siY);


		//Datalog      
		Maxim::DatalogAll(DSIndex, gbSample); //Engineering sample indicator
		Maxim::DatalogAll(DSIndex, siWaferID2); //Wafer number
		Maxim::DatalogAll(DSIndex, siX); //X Coordinate
		Maxim::DatalogAll(DSIndex, siY); //Y Coordinate


	}



}





PinListData APU_AWGThreshold(PME_INT Force_Pin, LPCTSTR AwgName, int Force_VRange, int Force_IRange, 
	double Start_Force, double Stop_Force, double Step_Size, int MCU_Divider, PME_INT Meas_Pin, double Compare_Val, 
	int Threshold_Direction, int settling_time, int forcemode, int adcmode, int connectmode , int iSampeToSkip, bool bFromRight)
{
	PinListData results;
	int Numb_Steps = abs((int)((Stop_Force - Start_Force ) / Step_Size)); //--- Force cast to an int to prevent compile warnings
	int isite;


	RESULTS_STR* temp_results;
	int ResultsSize = giNumSites;	//PME_INT only has one pin
	temp_results = new RESULTS_STR[ResultsSize];
	for (isite = 0; isite < NUM_SITES; isite++) 
		if (msSiteStat(isite)) 
			temp_results[isite].site = isite;
		else
			temp_results[isite].site = -2;
	results = PinListData(temp_results, 1, ResultsSize);	//PME_INT only has one pin


	//Create AWG pattern if not already done.
	if (!Maxim::AWG::IsAWGPatternLoaded(AwgName))
	{
		gdAWGStart = Start_Force;
		gdAWGStop = Stop_Force;
		gdAWGStepSize = Step_Size;
		giAWGVectorSize = 1 + int(( gdAWGStop - gdAWGStart ) / gdAWGStepSize );
		Maxim::AWG::apu12awgloader( Force_Pin, (char*)(AwgName), forcemode, Force_VRange, Force_IRange, Maxim::AWG::CreateLinearRampVector( gdAWGStart, gdAWGStop, giAWGVectorSize ));
		mcusequence( MCU_CH0, AwgName, "%d ON", giAWGVectorSize );
	}



	//--- Setup the MCU Clock based off the force rate
	mcumode( MCU_CH0, MCU_CLK, MCU_Divider, MCU_CLKA, MCU_TRIGNORMAL );
	mcuconnect( MCU_CH0, "Group%d, Group%d", (int)Force_Pin, (int)Meas_Pin  ); //--- (int) casting is required becaus_fe of issue in 2011A release.  Should not need eventually to do that.

	apu12adcmode( Meas_Pin, adcmode, 1 );
	apu12setwhereat( Meas_Pin, 1 + iSampeToSkip, Numb_Steps+1, Compare_Val, Threshold_Direction );


	//--- Setup the force condition
	Maximapu12setcapload( Force_Pin, forcemode+2, Start_Force, Force_VRange, Force_IRange, 1, connectmode );
	lwait( settling_time );

	//--- Run pattern
	apu12awgselect( Force_Pin, AwgName );
	mcurun( AwgName, MCU_NODIGITAL );


	//---- Disconnect the MCU
	mcuconnect( MCU_CH0, "OFF" );

	//--- Find the whereat location and datalog the results
	apu12getwhereat( Meas_Pin, APU12_THRESHOLD1 );
	apu12setwhereat( Meas_Pin, -1, -1, 0.0, APU12_BELOW_THRESH, -1, -1, 0.0, APU12_BELOW_THRESH );  //--- Turn off the whereat
	groupgetresults( temp_results, NUM_SITES );


	//Find first trigger point from the far right.
	if(bFromRight){

		float fAWGData[NUM_SITES][1000];
		float fTemp;
		char buffer[20];

		for(int isite = 0; isite < NUM_SITES; isite++){			
			if(msSiteStat(isite)){
				
				//Copy data to array
				sprintf_s (buffer, "AWG%i", isite);
				namearray( buffer, NAMEARRAY_APU12ADC, (int)Meas_Pin[isite] , 1 );
				copy( buffer, fAWGData[isite], 1, Numb_Steps );

				//Read first measurement
				fTemp = fAWGData[isite][1];


				//Search for lowest point
				int iStep;
				int iLowestStep = 0;
				
				for(iStep = 2; iStep < Numb_Steps-10; iStep++){

					if(fAWGData[isite][iStep] < fTemp){
						fTemp = fAWGData[isite][iStep];
						iLowestStep = iStep;
						//break;
					}
					/*if(Threshold_Direction){
						if(fAWGData[isite][iStep] < Compare_Val){
							results[isite] = fAWGData[isite][iStep];
							break;
						}
					}
					else{

						if(fAWGData[isite][iStep] > Compare_Val){
							results[isite] = fAWGData[isite][iStep];
							break;
						}
					}*/
				}


				//Get actual value
				results[isite] = Start_Force + Step_Size * (iLowestStep);
				
				
			}
		}


		return results;

	}


	//Extrack actual source value
	for (int site = 0; site<NUM_SITES; site++)
	{
		if (msSiteStat(site))
		{
			if ( temp_results[site].value == FLT_MAX )
				results[site] = 99999;
			else if ( temp_results[site].value == 1 )
				results[site] = -99999;
			else
				results[site] = Start_Force + Step_Size * (temp_results[site].value - 1);
		}
	}


	return results;
}



PinListData Test_Ramp_Threshold_Char(double dStart_Force, double dStop_Force, double dStep_Size, SiteDouble sdCompare_Val, int iThreshold_Direction, void (*SetFunc)(double), PinListData (*MeasFunc)(void)){
		
	PinListData pldOut_val, pldResult;
	SiteBool sbfound;
	int isite;
	double dstep;


	SetFunc(dStart_Force);
	pldResult = pldOut_val = MeasFunc();  


	sbfound = TRUE;


	for (isite = 0; isite<NUM_SITES; isite++)
	{

		if (msSiteStat(isite))
		{
			if (iThreshold_Direction > 0) 
				sbfound[isite] = ( pldOut_val[isite] > sdCompare_Val[isite] );	// greater than
			else	
				sbfound[isite] = ( pldOut_val[isite] < sdCompare_Val[isite] );	// less than
		}

		pldResult[isite] = dStart_Force;

	}





	dstep = dStart_Force;

	while ((sbfound.Any(FALSE)) && 	(((dStart_Force < dStop_Force) && (dstep <= dStop_Force)) ||  ((dStart_Force > dStop_Force) && (dstep >= dStop_Force))))
	{


		dstep += dStep_Size;
		SetFunc(dstep);
		pldOut_val = MeasFunc();




		for (isite = 0; isite < NUM_SITES; isite++)
		{
			if (msSiteStat(isite))
			{

				if (!sbfound[isite])
					pldResult[isite] = dstep;

				if (iThreshold_Direction > 0) 
					sbfound[isite] = ( pldOut_val[isite] > sdCompare_Val[isite] );	// greater than
				else	
					sbfound[isite] = ( pldOut_val[isite] < sdCompare_Val[isite] );	// less than
			}
		}	




	}

	return pldResult;

}

PinListData Test_Ramp_Threshold_Char(double dStart_Force, double dStop_Force, double dStep_Size, double dCompare_Val, int iThreshold_Direction, void (*SetFunc)(double), PinListData (*MeasFunc)(void)){

	SiteDouble sdTemp;
	sdTemp = dCompare_Val;

	return Test_Ramp_Threshold_Char( dStart_Force, dStop_Force, dStep_Size, sdTemp, iThreshold_Direction, SetFunc, MeasFunc );

}



/// <summary>
/// Thresholds the test.
/// </summary>



int  pldapu12setcapload( int pin, int mode, double dValue, int vrange, int irange, double risetime, int connect)
{
	return apu12setcapload(pin, mode, dValue, vrange, irange, risetime, connect);
}


int  pldapu12setcapload( int pin, int mode, PinListData pldValue, int vrange, int irange, double risetime, int connect)
{
	ACCUMULATOR_STR apu_v[NUM_SITES];


	for(int isite = 0; isite<NUM_SITES; isite++)
		if (msSiteStat(isite) && pldValue[isite] > -30 && pldValue[isite] < 30)
			apu_v[isite].value1 = pldValue[isite];
		else
			apu_v[isite].value1 = 0.0; 

	groupsetaccumulator(apu_v, NUM_SITES);
	return apu12setcapload(pin, mode, USE_ACCUM, vrange, irange, risetime, connect);
}


// int pin, int mode, PinListData pldValue, int vrange, int irange, double risetime, int connect
template <typename T> void apu12_GoNoGo_Set(int pin, int mode, T tValue, int vrange, int irange, double risetime, int connect, double dOffset, double dStepSize, int iOrder)
{

	switch (iOrder){

	case 1 : // Lo Rising
		pldapu12setcapload(pin, mode, (tValue - dOffset*dStepSize*3), vrange, irange, risetime, connect); lwait (1 ms_f);
		pldapu12setcapload(pin, mode, (tValue - dOffset*dStepSize*2), vrange, irange, risetime, connect); lwait (1 ms_f);
		pldapu12setcapload(pin, mode, (tValue - dOffset*dStepSize*1.1), vrange, irange, risetime, connect); lwait (1 ms_f);
		pldapu12setcapload(pin, mode, (tValue - dOffset*dStepSize*1.0), vrange, irange, risetime, connect); lwait (1 ms_f);
		break;

	case 2 : // Hi Rising
		pldapu12setcapload(pin, mode, (tValue + dOffset*dStepSize*0.9), vrange, irange, risetime, connect); lwait (1 ms_f);
		pldapu12setcapload(pin, mode, (tValue + dOffset*dStepSize*0.95), vrange, irange, risetime, connect); lwait (1 ms_f);
		pldapu12setcapload(pin, mode, (tValue + dOffset*dStepSize*0.98), vrange, irange, risetime, connect); lwait (1 ms_f);
		pldapu12setcapload(pin, mode, (tValue + dOffset*dStepSize*1.0), vrange, irange, risetime, connect); lwait (1 ms_f);
		break;

	case 3 : // Hi Falling
		pldapu12setcapload(pin, mode, (tValue + dOffset*dStepSize*1.05), vrange, irange, risetime, connect); lwait (1 ms_f);
		pldapu12setcapload(pin, mode, (tValue + dOffset*dStepSize*1.02), vrange, irange, risetime, connect); lwait (1 ms_f);
		pldapu12setcapload(pin, mode, (tValue + dOffset*dStepSize*1.005), vrange, irange, risetime, connect); lwait (1 ms_f);
		pldapu12setcapload(pin, mode, (tValue + dOffset*dStepSize*1.0), vrange, irange, risetime, connect); lwait (1 ms_f);
		break;

	case 4 : // Lo Falling
		pldapu12setcapload(pin, mode, (tValue - dOffset*dStepSize*0.9), vrange, irange, risetime, connect); lwait (1 ms_f);
		pldapu12setcapload(pin, mode, (tValue - dOffset*dStepSize*0.95), vrange, irange, risetime, connect); lwait (1 ms_f);
		pldapu12setcapload(pin, mode, (tValue - dOffset*dStepSize*0.98), vrange, irange, risetime, connect); lwait (1 ms_f);
		pldapu12setcapload(pin, mode, (tValue - dOffset*dStepSize*1.0), vrange, irange, risetime, connect); lwait (1 ms_f);
		break;

	}
}


void ThresholdTest(int &DSIndex, TestTH *ts){



	PinListData pldTH_Rise1;
	PinListData pldTH_Fall1;
	PinListData pldTH_Rise2;
	PinListData pldTH_Fall2;
	PinListData pldTH_Hyst;
	PinListData pldTH_GNG1;
	PinListData pldTH_GNG2;
	PinListData pldTH_GNG3;
	PinListData pldTH_GNG4;


	double dLoLimit, dHiLimit;


	if(Maxim::isChar())
	{

		
		//Get Low and High limit to determine rising search windows
		if (ts->PercentDatalog){
			dLoLimit = 0.99 * Maxim::Limits::GetLoLimitbyIndex(DSIndex) * ts->RefValue / 100;
			dHiLimit = 1.01 * Maxim::Limits::GetHiLimitbyIndex(DSIndex) * ts->RefValue / 100;
		}
		else{
			dLoLimit = 0.99 * Maxim::Limits::GetLoLimitbyIndex(DSIndex);
			dHiLimit = 1.01 * Maxim::Limits::GetHiLimitbyIndex(DSIndex);
		}


		if(ts->bUseAWG)
			pldTH_Rise2 = APU_AWGThreshold( ts->SetPin.pinName, ts->AwgNameR, ts->SetPin.VRange, ts->SetPin.IRange, dLoLimit, dHiLimit, ts->step, ts->MCU_Divider, ts->MeasPin.pinName, ts->outTh1, ts->ThresholdDirection, 1 ms_f, ts->SetPin.ForceMode, ts->MeasPin.MeasMode, ts->SetPin.ConnectMode, ts->SampleToSkip, ts->FromRight);
		else
			//pldTH_Rise2 = Test_Ramp_Threshold_Char( dLoLimit, dHiLimit, ts->step, ts->outTh1, ts->ThresholdDirection, ts->TH_Set, ts->TH_Meas );
			pldTH_Rise2 = Binary_Search(dLoLimit, dHiLimit, ts->step, ts->ThresholdDirection, ts->outTh1, ts->TH_Set, ts->TH_Meas);




		if(ts->dFallingSearch){

			//Get Low and High limit to determine falling search windows
			if (ts->PercentDatalog){
				dLoLimit = 0.99 * Maxim::Limits::GetLoLimitbyIndex(DSIndex+1) * ts->RefValue / 100;
				dHiLimit = 1.01 * Maxim::Limits::GetHiLimitbyIndex(DSIndex+1) * ts->RefValue / 100;
			}
			else{
				dLoLimit = 0.99 * Maxim::Limits::GetLoLimitbyIndex(DSIndex+1);
				dHiLimit = 1.01 * Maxim::Limits::GetHiLimitbyIndex(DSIndex+1);
			}


			if(gbOUT_COMP_TH) {
				apu12set(ts->SetPin.pinName, ts->SetPin.ForceMode, 1.5, ts->SetPin.VRange, ts->SetPin.IRange, ts->SetPin.ConnectMode);
				lwait (1 ms_f);
				apu12setcapload(ts->SetPin.pinName, ts->SetPin.ForceMode, 1.2, ts->SetPin.VRange, ts->SetPin.IRange, 1, ts->SetPin.ConnectMode);
			}


			if(!gbOUT_COMP_TH){

				if(ts->bUseAWG)
					pldTH_Fall2 = APU_AWGThreshold( ts->SetPin.pinName, ts->AwgNameF, ts->SetPin.VRange, ts->SetPin.IRange, dHiLimit, dLoLimit, -ts->step, ts->MCU_Divider, ts->MeasPin.pinName, ts->outTh2, ts->ThresholdDirection > 0? 0:1, 1 ms_f, ts->SetPin.ForceMode, ts->MeasPin.MeasMode, ts->SetPin.ConnectMode, ts->SampleToSkip, ts->FromRight);
				else
					//pldTH_Fall2 = Test_Ramp_Threshold_Char( dHiLimit, dLoLimit, -ts->step, ts->outTh2, ts->ThresholdDirection > 0? 0:1, ts->TH_Set, ts->TH_Meas );
					pldTH_Fall2 = Binary_Search(dHiLimit, dLoLimit, ts->step, ts->ThresholdDirection > 0? 0:1, ts->outTh2, ts->TH_Set, ts->TH_Meas);

			}
			else{

				if(ts->bUseAWG)
					pldTH_Fall2 = APU_AWGThreshold( ts->SetPin.pinName, ts->AwgNameF, ts->SetPin.VRange, ts->SetPin.IRange, dHiLimit, dLoLimit, -ts->step, ts->MCU_Divider, ts->MeasPin.pinName, ts->outTh2, ts->ThresholdDirection, 1 ms_f, ts->SetPin.ForceMode, ts->MeasPin.MeasMode, ts->SetPin.ConnectMode, ts->SampleToSkip, ts->FromRight);
				else
					//pldTH_Fall2 = Test_Ramp_Threshold_Char( dHiLimit, dLoLimit, -ts->step, ts->outTh2, ts->ThresholdDirection, ts->TH_Set, ts->TH_Meas );
					pldTH_Fall2 = Binary_Search(dHiLimit, dLoLimit, ts->step, ts->ThresholdDirection, ts->outTh2, ts->TH_Set, ts->TH_Meas);

			}


		}




		if (ts->PercentDatalog){
				pldTH_Rise1 = pldTH_Rise2 * 100 / ts->pldRefValue;
				pldTH_Fall1 = pldTH_Fall2 * 100 / ts->pldRefValue;
		}
		else{

			pldTH_Rise1 = pldTH_Rise2;

			if(ts->dFallingSearch)   pldTH_Fall1 = pldTH_Fall2;
		}


		//Calculate hysteresis
		if(ts->dFallingSearch)  pldTH_Hyst = pldTH_Rise1 - pldTH_Fall1;


		//-------------- datalog ---------------
		Maxim::DatalogAll(DSIndex, pldTH_Rise1);

		if(ts->dFallingSearch) 
			Maxim::DatalogAll(DSIndex, pldTH_Fall1);
		else
			DSIndex++;



		//if (ts->PercentDatalog){
		//	Maxim::DatalogAll(DSIndex, pldTH_Rise2);
		//	Maxim::DatalogAll(DSIndex, pldTH_Fall2);
		//}
		//else
		//	DSIndex += 2;


		if(!gbOUT_COMP_TH && (ts->dFallingSearch))
			Maxim::DatalogAll(DSIndex, pldTH_Hyst);
		else
			DSIndex++;


		if(gbOUT_COMP_TH) {
			apu12set(ts->SetPin.pinName, ts->SetPin.ForceMode, 0.6, ts->SetPin.VRange, ts->SetPin.IRange, ts->SetPin.ConnectMode);
			lwait (2 ms_f);
		}


		//CHAR vs Go-no-Go correlation
		apu12_GoNoGo_Set(ts->SetPin.pinName, ts->SetPin.ForceMode, pldTH_Rise2, ts->SetPin.VRange, ts->SetPin.IRange, 1, ts->SetPin.ConnectMode, ts->Offset, ts->step, 1);
		pldTH_GNG1 = ts->TH_Meas();

		apu12_GoNoGo_Set(ts->SetPin.pinName, ts->SetPin.ForceMode, pldTH_Rise2, ts->SetPin.VRange, ts->SetPin.IRange, 1, ts->SetPin.ConnectMode, ts->Offset, ts->step, 2);
		pldTH_GNG2 = ts->TH_Meas();

		if(gbOUT_COMP_TH) {
			apu12set(ts->SetPin.pinName, ts->SetPin.ForceMode, 1.5, ts->SetPin.VRange, ts->SetPin.IRange, ts->SetPin.ConnectMode);
			lwait (1 ms_f);
			apu12setcapload(ts->SetPin.pinName, ts->SetPin.ForceMode, 1.2, ts->SetPin.VRange, ts->SetPin.IRange, 1, ts->SetPin.ConnectMode);
			lwait (1 ms_f);
		}

		if(ts->dFallingSearch){

			apu12_GoNoGo_Set(ts->SetPin.pinName, ts->SetPin.ForceMode, pldTH_Fall2, ts->SetPin.VRange, ts->SetPin.IRange, 1, ts->SetPin.ConnectMode, ts->Offset, ts->step, 3);
			pldTH_GNG3 = ts->TH_Meas();

			apu12_GoNoGo_Set(ts->SetPin.pinName, ts->SetPin.ForceMode, pldTH_Fall2, ts->SetPin.VRange, ts->SetPin.IRange, 1, ts->SetPin.ConnectMode, ts->Offset, ts->step, 4);
			pldTH_GNG4 = ts->TH_Meas();
		}

		Maxim::DatalogAll(DSIndex, pldTH_GNG1);
		Maxim::DatalogAll(DSIndex, pldTH_GNG2);

		if(ts->dFallingSearch){
			Maxim::DatalogAll(DSIndex, pldTH_GNG3);	
			Maxim::DatalogAll(DSIndex, pldTH_GNG4);
		}



	}
	else{


		//Get Low and High limit to determine rising search windows
		if (ts->PercentDatalog){
			dLoLimit = Maxim::Limits::GetLoLimitbyIndex(DSIndex) * ts->RefValue / 100;
			dHiLimit = Maxim::Limits::GetHiLimitbyIndex(DSIndex) * ts->RefValue / 100;
		}
		else{
			dLoLimit = Maxim::Limits::GetLoLimitbyIndex(DSIndex);
			dHiLimit = Maxim::Limits::GetHiLimitbyIndex(DSIndex);
		}


		DSIndex += 3;

		apu12_GoNoGo_Set(ts->SetPin.pinName, ts->SetPin.ForceMode, dLoLimit + (ts->Offset * ts->step), ts->SetPin.VRange, ts->SetPin.IRange, 1, ts->SetPin.ConnectMode, 1, ts->step, 1);
		pldTH_GNG1 = ts->TH_Meas();

		apu12_GoNoGo_Set(ts->SetPin.pinName, ts->SetPin.ForceMode, dHiLimit - (ts->Offset * ts->step), ts->SetPin.VRange, ts->SetPin.IRange, 1, ts->SetPin.ConnectMode, 1, ts->step, 2);
		pldTH_GNG2 = ts->TH_Meas();


		if(ts->dFallingSearch)  {

			//Get Low and High limit to determine falling search windows
			if (ts->PercentDatalog){
				dLoLimit = Maxim::Limits::GetLoLimitbyIndex(DSIndex-2) * ts->RefValue / 100;
				dHiLimit = Maxim::Limits::GetHiLimitbyIndex(DSIndex-2) * ts->RefValue / 100;
			}
			else{
				dLoLimit = Maxim::Limits::GetLoLimitbyIndex(DSIndex-2);
				dHiLimit = Maxim::Limits::GetHiLimitbyIndex(DSIndex-2);
			}


			if(gbOUT_COMP_TH) {
				apu12set(ts->SetPin.pinName, ts->SetPin.ForceMode, 1.5, ts->SetPin.VRange, ts->SetPin.IRange, ts->SetPin.ConnectMode);
				lwait (1 ms_f);
				apu12setcapload(ts->SetPin.pinName, ts->SetPin.ForceMode, 1.2, ts->SetPin.VRange, ts->SetPin.IRange, 1, ts->SetPin.ConnectMode);
			}


			apu12_GoNoGo_Set(ts->SetPin.pinName, ts->SetPin.ForceMode, dHiLimit - (ts->Offset * ts->step), ts->SetPin.VRange, ts->SetPin.IRange, 1, ts->SetPin.ConnectMode, 1, ts->step, 3);
			pldTH_GNG3 = ts->TH_Meas();

			apu12_GoNoGo_Set(ts->SetPin.pinName, ts->SetPin.ForceMode, dLoLimit + (ts->Offset * ts->step), ts->SetPin.VRange, ts->SetPin.IRange, 1, ts->SetPin.ConnectMode, 1, ts->step, 4);
			pldTH_GNG4 = ts->TH_Meas();

		}

		Maxim::DatalogAll(DSIndex, pldTH_GNG1);
		Maxim::DatalogAll(DSIndex, pldTH_GNG2);

		if(ts->dFallingSearch){
			Maxim::DatalogAll(DSIndex, pldTH_GNG3);	
			Maxim::DatalogAll(DSIndex, pldTH_GNG4);
		}


	}

}







void OUT_SG_ThresholdTest(int &DSIndex, TestTH *ts){



	PinListData pldTH_Fall1;
	PinListData pldTH_Fall2;
	PinListData pldTH_GNG3;
	PinListData pldTH_GNG4;


	double dLoLimit, dHiLimit;


	if(Maxim::isChar())
	{


		//Get Low and High limit to determine rising search windows
		dLoLimit = 0.99 * Maxim::Limits::GetLoLimitbyIndex(DSIndex) * 0.001;
		dHiLimit = 1.01 * Maxim::Limits::GetHiLimitbyIndex(DSIndex) * 0.001;




		if(ts->bUseAWG)
			pldTH_Fall2 = APU_AWGThreshold( ts->SetPin.pinName, ts->AwgNameF, ts->SetPin.VRange, ts->SetPin.IRange, dHiLimit, dLoLimit, -ts->step, ts->MCU_Divider, ts->MeasPin.pinName, ts->outTh2, ts->ThresholdDirection, 1 ms_f, ts->SetPin.ForceMode, ts->MeasPin.MeasMode, ts->SetPin.ConnectMode, ts->SampleToSkip, ts->FromRight);
		else
			//pldTH_Fall2 = Test_Ramp_Threshold_Char( dHiLimit, dLoLimit, -ts->step, ts->outTh2, ts->ThresholdDirection, ts->TH_Set, ts->TH_Meas );
			pldTH_Fall2 = Binary_Search(dHiLimit, dLoLimit, ts->step, ts->ThresholdDirection, ts->outTh2, ts->TH_Set, ts->TH_Meas);


		//-------------- datalog ---------------
		Maxim::DatalogAll(DSIndex, pldTH_Fall2 * mV);
		

		//CHAR vs Go-no-Go correlation
		apu12set(ts->SetPin.pinName, ts->SetPin.ForceMode, 0.7, ts->SetPin.VRange, ts->SetPin.IRange, ts->SetPin.ConnectMode);
		lwait(2 ms_f);

		apu12_GoNoGo_Set(ts->SetPin.pinName, ts->SetPin.ForceMode, pldTH_Fall2, ts->SetPin.VRange, ts->SetPin.IRange, 1, ts->SetPin.ConnectMode, ts->Offset, ts->step, 3);
		pldTH_GNG3 = ts->TH_Meas();

		apu12_GoNoGo_Set(ts->SetPin.pinName, ts->SetPin.ForceMode, pldTH_Fall2, ts->SetPin.VRange, ts->SetPin.IRange, 1, ts->SetPin.ConnectMode, ts->Offset, ts->step, 4);
		pldTH_GNG4 = ts->TH_Meas();
		
		Maxim::DatalogAll(DSIndex, pldTH_GNG3);	
		Maxim::DatalogAll(DSIndex, pldTH_GNG4);

		
	}
	else{


		//Get Low and High limit to determine rising search windows

			dLoLimit = Maxim::Limits::GetLoLimitbyIndex(DSIndex) * 0.001;
			dHiLimit = Maxim::Limits::GetHiLimitbyIndex(DSIndex) * 0.001;

			DSIndex += 1;


			apu12_GoNoGo_Set(ts->SetPin.pinName, ts->SetPin.ForceMode, dHiLimit - (ts->Offset * ts->step), ts->SetPin.VRange, ts->SetPin.IRange, 1, ts->SetPin.ConnectMode, 1, ts->step, 3);
			pldTH_GNG3 = ts->TH_Meas();

			apu12_GoNoGo_Set(ts->SetPin.pinName, ts->SetPin.ForceMode, dLoLimit + (ts->Offset * ts->step), ts->SetPin.VRange, ts->SetPin.IRange, 1, ts->SetPin.ConnectMode, 1, ts->step, 4);
			pldTH_GNG4 = ts->TH_Meas();
			

			Maxim::DatalogAll(DSIndex, pldTH_GNG3);	
			Maxim::DatalogAll(DSIndex, pldTH_GNG4);



	}

}






void OUT_Unused_ThresholdTest(int &DSIndex, TestTH *ts){



	PinListData pldTH_Rise1;
	PinListData pldTH_Rise2;
	PinListData pldTH_GNG1;
	PinListData pldTH_GNG2;


	double dLoLimit, dHiLimit;


	if(Maxim::isChar())
	{


		//Get Low and High limit to determine rising search windows
		dLoLimit = 0.99 * Maxim::Limits::GetLoLimitbyIndex(DSIndex);
		dHiLimit = 1.01 * Maxim::Limits::GetHiLimitbyIndex(DSIndex);




		if(ts->bUseAWG)
			pldTH_Rise2 = APU_AWGThreshold( ts->SetPin.pinName, ts->AwgNameR, ts->SetPin.VRange, ts->SetPin.IRange, dLoLimit, dHiLimit, ts->step, ts->MCU_Divider, ts->MeasPin.pinName, ts->outTh2, ts->ThresholdDirection, 1 ms_f, ts->SetPin.ForceMode, ts->MeasPin.MeasMode, ts->SetPin.ConnectMode, ts->SampleToSkip, ts->FromRight);
		else
			//pldTH_Rise2 = Test_Ramp_Threshold_Char( dLoLimit, dHiLimit, ts->step, ts->outTh2, ts->ThresholdDirection, ts->TH_Set, ts->TH_Meas );
			pldTH_Rise2 = Binary_Search(dLoLimit, dHiLimit, ts->step, ts->ThresholdDirection, ts->outTh2, ts->TH_Set, ts->TH_Meas);


		//-------------- datalog ---------------
		Maxim::DatalogAll(DSIndex, pldTH_Rise2 );
		

		//CHAR vs Go-no-Go correlation
		apu12set(ts->SetPin.pinName, ts->SetPin.ForceMode, 1, ts->SetPin.VRange, ts->SetPin.IRange, ts->SetPin.ConnectMode);
		lwait(2 ms_f);


		apu12_GoNoGo_Set(ts->SetPin.pinName, ts->SetPin.ForceMode, pldTH_Rise2, ts->SetPin.VRange, ts->SetPin.IRange, 1, ts->SetPin.ConnectMode, ts->Offset, ts->step, 1);
		pldTH_GNG1 = ts->TH_Meas();

		apu12_GoNoGo_Set(ts->SetPin.pinName, ts->SetPin.ForceMode, pldTH_Rise2, ts->SetPin.VRange, ts->SetPin.IRange, 1, ts->SetPin.ConnectMode, ts->Offset, ts->step, 2);
		pldTH_GNG2 = ts->TH_Meas();
		
		Maxim::DatalogAll(DSIndex, pldTH_GNG1);	
		Maxim::DatalogAll(DSIndex, pldTH_GNG2);

		
	}
	else{


		//Get Low and High limit to determine rising search windows

			dLoLimit = Maxim::Limits::GetLoLimitbyIndex(DSIndex);
			dHiLimit = Maxim::Limits::GetHiLimitbyIndex(DSIndex);

			DSIndex += 1;


			apu12set(ts->SetPin.pinName, ts->SetPin.ForceMode, 1, ts->SetPin.VRange, ts->SetPin.IRange, ts->SetPin.ConnectMode);
			lwait(2 ms_f);
			
			apu12_GoNoGo_Set(ts->SetPin.pinName, ts->SetPin.ForceMode, dLoLimit - (ts->Offset * ts->step), ts->SetPin.VRange, ts->SetPin.IRange, 1, ts->SetPin.ConnectMode, 1, ts->step, 1);
			pldTH_GNG1 = ts->TH_Meas();

			apu12_GoNoGo_Set(ts->SetPin.pinName, ts->SetPin.ForceMode, dHiLimit + (ts->Offset * ts->step), ts->SetPin.VRange, ts->SetPin.IRange, 1, ts->SetPin.ConnectMode, 1, ts->step, 2);
			pldTH_GNG2 = ts->TH_Meas();
			

			Maxim::DatalogAll(DSIndex, pldTH_GNG1);	
			Maxim::DatalogAll(DSIndex, pldTH_GNG2);



	}

}



void OUT_SHRT_ThresholdTest(int &DSIndex, TestTH *ts){



	PinListData pldTH_Rise1;
	PinListData pldTH_Rise2;
	PinListData pldTH_GNG1;
	PinListData pldTH_GNG2;


	double dLoLimit, dHiLimit;


	if(Maxim::isChar())
	{


		//Get Low and High limit to determine rising search windows
		dLoLimit = 0.99 * Maxim::Limits::GetLoLimitbyIndex(DSIndex);
		dHiLimit = 1.01 * Maxim::Limits::GetHiLimitbyIndex(DSIndex);




		if(ts->bUseAWG)
			pldTH_Rise2 = APU_AWGThreshold( ts->SetPin.pinName, ts->AwgNameR, ts->SetPin.VRange, ts->SetPin.IRange, dLoLimit, dHiLimit, ts->step, ts->MCU_Divider, ts->MeasPin.pinName, ts->outTh2, ts->ThresholdDirection, 1 ms_f, ts->SetPin.ForceMode, ts->MeasPin.MeasMode, ts->SetPin.ConnectMode, ts->SampleToSkip, ts->FromRight);
		else
			//pldTH_Rise2 = Test_Ramp_Threshold_Char( dLoLimit, dHiLimit, ts->step, ts->outTh2, ts->ThresholdDirection, ts->TH_Set, ts->TH_Meas );
			pldTH_Rise2 = Binary_Search(dLoLimit, dHiLimit, ts->step, ts->ThresholdDirection, ts->outTh2, ts->TH_Set, ts->TH_Meas);


		//-------------- datalog ---------------
		Maxim::DatalogAll(DSIndex, pldTH_Rise2 );
		

		//CHAR vs Go-no-Go correlation
		apu12set(ts->SetPin.pinName, ts->SetPin.ForceMode, dLoLimit - 1, ts->SetPin.VRange, ts->SetPin.IRange, ts->SetPin.ConnectMode);
		lwait(2 ms_f);


		apu12_GoNoGo_Set(ts->SetPin.pinName, ts->SetPin.ForceMode, pldTH_Rise2, ts->SetPin.VRange, ts->SetPin.IRange, 1, ts->SetPin.ConnectMode, ts->Offset, ts->step, 1);
		pldTH_GNG1 = ts->TH_Meas();

		apu12_GoNoGo_Set(ts->SetPin.pinName, ts->SetPin.ForceMode, pldTH_Rise2, ts->SetPin.VRange, ts->SetPin.IRange, 1, ts->SetPin.ConnectMode, ts->Offset, ts->step, 2);
		pldTH_GNG2 = ts->TH_Meas();
		
		Maxim::DatalogAll(DSIndex, pldTH_GNG1);	
		Maxim::DatalogAll(DSIndex, pldTH_GNG2);

		
	}
	else{


		//Get Low and High limit to determine rising search windows

			dLoLimit = Maxim::Limits::GetLoLimitbyIndex(DSIndex);
			dHiLimit = Maxim::Limits::GetHiLimitbyIndex(DSIndex);

			DSIndex += 1;


			apu12set(ts->SetPin.pinName, ts->SetPin.ForceMode, dLoLimit - 1, ts->SetPin.VRange, ts->SetPin.IRange, ts->SetPin.ConnectMode);
			lwait(2 ms_f);
			
			apu12_GoNoGo_Set(ts->SetPin.pinName, ts->SetPin.ForceMode, dLoLimit - (ts->Offset * ts->step), ts->SetPin.VRange, ts->SetPin.IRange, 1, ts->SetPin.ConnectMode, 1, ts->step, 1);
			pldTH_GNG1 = ts->TH_Meas();

			apu12_GoNoGo_Set(ts->SetPin.pinName, ts->SetPin.ForceMode, dHiLimit + (ts->Offset * ts->step), ts->SetPin.VRange, ts->SetPin.IRange, 1, ts->SetPin.ConnectMode, 1, ts->step, 2);
			pldTH_GNG2 = ts->TH_Meas();
			

			Maxim::DatalogAll(DSIndex, pldTH_GNG1);	
			Maxim::DatalogAll(DSIndex, pldTH_GNG2);



	}

}




PinListData VCC_MeasV(){

	lwait (2 ms_f);
	return apu12mv(VCC, 10);

}

PinListData NDRV_MeasV(){

	lwait (2 ms_f);
	return apu12mv(OUT3, 10);

}

PinListData CS_MeasV(){

	lwait (2 ms_f);
	return apu12mv(CS, 10);

}


PinListData pldFreqLow;
PinListData pldFreqHigh;


SiteDouble compute_ss(double center_freq)
{

#define SS_SAMPLES 1000


	int i;
	double data[SS_SAMPLES]; //array for pos/neg/pos/neg/poss time stamps (i.e. half periods)
	double data2[SS_SAMPLES/2]; //array for pos/pos/pos time stamps (i.e full periods)
	double ss_freq[SS_SAMPLES/2-1];
	double min_ss_freq, max_ss_freq;
	double ss_spread;
	SiteDouble result;

	for(int iSite = 0; iSite < giNumSites; iSite++ )
	{	
		if (msSiteStat(iSite))
		{

			qtmuarm( QTMU_GP, "AUTO", SS_SAMPLES, QTMU_EDGE, QTMU_POS );
			lwait( 100 us_f );
			qtmumt( QTMU_GP, QTMU_READ_ELAPSEDTIME, 25000 , -1, -1, 1 );

			namearray( "FREQ_SS1", NAMEARRAY_QTMU, QTMU_GP, 1);
			namearray( "FREQ_SS1", NAMEARRAY_QTMU, QTMU_GP[iSite], 1); //get data by site

			copy( "FREQ_SS1", data, 1, SS_SAMPLES );

			//parse out period to period time stamps
			for (i=0; i<SS_SAMPLES/2; i++)
			{
				data2[i] = data[2*i];
			}

			//find the min and max periods
			min_ss_freq = 1e9;
			max_ss_freq = 0;
			for (i=1; i<SS_SAMPLES/2-1; i++) //start from 1 to throw out first sample
			{
				ss_freq[i] = 1e6/(data2[i+1] - data2[i]);
				if (ss_freq[i] < min_ss_freq)  min_ss_freq=ss_freq[i];
				if (ss_freq[i] > max_ss_freq)  max_ss_freq=ss_freq[i];		
			}

			//compute spread between min and max periods as a percentage of center freq.
			ss_spread = 100*(max_ss_freq - min_ss_freq)/(center_freq/1.0e3);

			result[iSite] = ss_spread;

			pldFreqLow[iSite] = min_ss_freq;
			pldFreqHigh[iSite] = max_ss_freq;

		}
	}

	return result;
}




PinListData Binary_Search(double dBegin, double dEnd, double dAccu, int Direction, SiteDouble sdThres, void (*SetFunc)(SiteDouble), PinListData (*MeasFunc)(void)){

	// Initialise lower and upper bounnds
	SiteDouble lb, ub, mid;
	SiteBool sbFinish;
	PinListData pldResult;

	lb = dBegin;
	ub = dEnd;

	ForEachActive_iSite{
		if(dBegin < dEnd)
			sbFinish[iSite] = (lb[iSite] < ub[iSite])?false:true;
		else
			sbFinish[iSite] = (lb[iSite] > ub[iSite])?false:true;
	}

	// Perform Binary search
	while (sbFinish.Any(false))
	{

		sbFinish = true;

		SetFunc(dBegin);

		// Find mid
		if(dBegin < dEnd)
			mid = (lb+ub)/2;
		else
			mid = ((lb-ub)/2) + ub;


		SetFunc(mid);

		pldResult = MeasFunc();

		if(Direction < 1){
			ForEachActive_iSite{
				if(pldResult[iSite] < sdThres[iSite])
					pldResult[iSite] = pldResult[iSite] + sdThres[iSite]*10;
				else
					pldResult[iSite] = pldResult[iSite] - sdThres[iSite]*10;
			}
		}


		//update lower_bound or upper_bound
		ForEachActive_iSite{
			if(pldResult[iSite] < sdThres[iSite])
				lb[iSite] = mid[iSite];
			else
				ub[iSite] = mid[iSite];
		}


		ForEachActive_iSite{

			if(dBegin < dEnd)
				sbFinish[iSite] = (lb[iSite] < ub[iSite] - dAccu)?false:true;
			else
				sbFinish[iSite] = (lb[iSite] > ub[iSite] + dAccu)?false:true;

			if(sbFinish[iSite]) pldResult[iSite] = mid[iSite];

		}

	}

	return pldResult;

}

