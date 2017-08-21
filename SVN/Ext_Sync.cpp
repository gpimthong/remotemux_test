/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:
*		Ext_Sync.cpp
*
*	Written and maintained by:
*		Govit Pimthong (govit.pimthong@MaximIntegrated.com)
*
*	URL:                   $URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/Ext_Sync.cpp $
*	Last Modified by:      $LastChangedBy: govit.pimthong $
*	Date:                  $Date: 2017-08-03 03:57:09 +0700 (Thu, 03 Aug 2017) $
*	Current Rev:           $Revision: 67598 $
*
*	Description:
*		File Description
*
*	Revision History
*
*
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
#include "AP89_FT.h"

void EXT_SYNC_SET(SiteDouble value){

	apu12set( PEDESTAL2, APU12_FV, value, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI );
	lwait( 1 ms_f );

}

PinListData EXT_SYNC_Meas(){

	PinListData pldResult;

	mcustart( NULL, "EXT_SYNC");
	
	qtmuarm( QTMU_GP, "AUTO", 1, QTMU_EDGE, QTMU_POS );
	lwait(100 us_f);

	mcustop( NULL);

	pldResult = qtmumt( QTMU_GP, QTMU_READFREQ, 1000);

	return pldResult;

}

// Test Function: Ext_Sync
ETS_PRGFLOW_FUNC Ext_Sync( int DSIndex, LPCTSTR TestLabel )
{     
    //declare variables
    PinListData pldResults;	//declare a PLD variable for the measurement results.
     
	Maxim::I2C::SetSlaveAddress(TM_3DV_AD);
	Maxim::I2C::Write(0x02, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x02, 0x40); //write data to DHR3<15:12>=0000
	Maxim::I2C::Write(0x02, 0x30); //write data to DHR2<11:8>=0000
	Maxim::I2C::Write(0x02, 0x20); //write data to DHR1<7:4>=0100
	Maxim::I2C::Write(0x02, 0x10); //write data to DHR0<3:0>=0000
	Maxim::I2C::Write(0x02, 0xA0); //write DHR<15:0> to DCTLR<15:0>

	Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>=0000
	Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>=0000
	Maxim::I2C::Write(0x01, 0x27); //write data to DHR1<7:4>=0000
	Maxim::I2C::Write(0x01, 0x10); //write data to DHR0<3:0>=0001
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>


	qtmustart( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_NEG, 100, 100);
	qtmustop( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_NEG, 1, 100);
	dpinrcvset("SCAN_OUT", DPIN_RCV_SPLIT, 2, 1, MS_ALL);
	dpinforce("DIM", "0", 3.0, 0.1, MS_ALL);
	cbitclose(K21);

	//600KHz
	double dExtFreqSet = 600;
	mcuset( MCU_MAIN, dExtFreqSet / 10 );

	int iMainClkDivider = 100;
	dpinperiod( iMainClkDivider ); 

	double dEdgeC = (1000/dExtFreqSet)*(0.5); 

	double dAcc = 0.005;
	double dThres = 450;

	//Apply edge set
	dpintimeset( "CS", "EXT_SYNC", "DefaultTimesetInOut", DPIN_TS_NORMAL, DPIN_RZ, 0.0, dEdgeC * 1E+3 , DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	lwait (200 us_f);


	double dLoLimit = Maxim::Limits::GetLoLimitbyIndex(DSIndex);
	double dHiLimit = Maxim::Limits::GetHiLimitbyIndex(DSIndex);



	//Sync Rising Threshold
	if(Maxim::isChar()){

		//pldResults = Test_Ramp_Threshold_Char( 0.8, 2.0, 0.001, 590, 1, EXT_SYNC_SET, EXT_SYNC_Meas );
		pldResults = Binary_Search(0.5, 2.0, dAcc, 1, dThres, EXT_SYNC_SET, EXT_SYNC_Meas);

		Maxim::DatalogAll(DSIndex, pldResults * V);

	}
	else
		DSIndex++;

		
	apu12set( PEDESTAL2, APU12_FV, 0.6, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI );
	lwait(500 us_f);


	//Sync Rising Threshold-L
	apu12setcapload( PEDESTAL2, APU12_FV, dLoLimit, APU12_3p6V, APU12_10MA, 1, APU12_PIN_TO_VI );
	lwait(1 ms_f);

	pldResults = EXT_SYNC_Meas();
	Maxim::DatalogAll(DSIndex, pldResults);



	//Sync Rising Threshold-H
	apu12setcapload( PEDESTAL2, APU12_FV, dHiLimit, APU12_3p6V, APU12_10MA, 1, APU12_PIN_TO_VI );
	lwait(1 ms_f);

	pldResults = EXT_SYNC_Meas();
	Maxim::DatalogAll(DSIndex, pldResults);


	DSIndex++;




	//Min_Sync_Freq400K_LoDuty (25% duty, 120% Freq)
	dExtFreqSet = 400 * 1.2;
	mcuset( MCU_MAIN, dExtFreqSet / 10 );


	if(Maxim::isChar()){
		dEdgeC = (1000/dExtFreqSet)*(0.25); 
		dpintimeset( "CS", "EXT_SYNC", "DefaultTimesetInOut", DPIN_TS_NORMAL, DPIN_RZ, 0.0, dEdgeC * 1E+3 , DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		lwait (200 us_f);

		pldResults = EXT_SYNC_Meas();
		Maxim::DatalogAll(DSIndex, pldResults);
	}
	else
		DSIndex++;


	//Min_Sync_Freq400K_50Duty (50% duty, 120% Freq)
	dEdgeC = (1000/dExtFreqSet)*(0.5); 
	dpintimeset( "CS", "EXT_SYNC", "DefaultTimesetInOut", DPIN_TS_NORMAL, DPIN_RZ, 0.0, dEdgeC * 1E+3 , DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	lwait (200 us_f);

	pldResults = EXT_SYNC_Meas();
	Maxim::DatalogAll(DSIndex, pldResults);



	if(Maxim::isChar()){
		//Min_Sync_Freq400K_HiDuty (75% duty, 120% Freq)
		dEdgeC = (1000/dExtFreqSet)*(0.75); 
		dpintimeset( "CS", "EXT_SYNC", "DefaultTimesetInOut", DPIN_TS_NORMAL, DPIN_RZ, 0.0, dEdgeC * 1E+3 , DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		lwait (200 us_f);

		pldResults = EXT_SYNC_Meas();
		Maxim::DatalogAll(DSIndex, pldResults);
	}
	else
		DSIndex++;

	//Max_Sync_Freq400K_LoDuty (25% duty, 150% Freq)
	dExtFreqSet = 400 * 1.5;
	mcuset( MCU_MAIN, dExtFreqSet / 10 );

	if(Maxim::isChar()){
		dEdgeC = (1000/dExtFreqSet)*(0.25); 
		dpintimeset( "CS", "EXT_SYNC", "DefaultTimesetInOut", DPIN_TS_NORMAL, DPIN_RZ, 0.0, dEdgeC * 1E+3 , DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		lwait (200 us_f);

		pldResults = EXT_SYNC_Meas();
		Maxim::DatalogAll(DSIndex, pldResults);
	}
	else
		DSIndex++;

	//Max_Sync_Freq400K_50Duty
	dEdgeC = (1000/dExtFreqSet)*(0.5); 
	dpintimeset( "CS", "EXT_SYNC", "DefaultTimesetInOut", DPIN_TS_NORMAL, DPIN_RZ, 0.0, dEdgeC * 1E+3 , DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	lwait (200 us_f);

	pldResults = EXT_SYNC_Meas();
	Maxim::DatalogAll(DSIndex, pldResults);


	if(Maxim::isChar()){
		//Max_Sync_Freq400K_HiDuty
		dEdgeC = (1000/dExtFreqSet)*(0.75); 
		dpintimeset( "CS", "EXT_SYNC", "DefaultTimesetInOut", DPIN_TS_NORMAL, DPIN_RZ, 0.0, dEdgeC * 1E+3 , DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		lwait (200 us_f);

		pldResults = EXT_SYNC_Meas();
		Maxim::DatalogAll(DSIndex, pldResults);

	}
	else
		DSIndex++;

	cbitclose(K36);
	lwait(1 ms_f);
	cbitopen(K37);
	lwait(1 ms_f);




	//Min_Sync_Freq2p2M_LoDuty (25% duty, 120% Freq)
	dExtFreqSet = 22 * 1.2;
	mcuset( MCU_MAIN, dExtFreqSet );

	iMainClkDivider = 10;
	dpinperiod( iMainClkDivider ); 

	if(Maxim::isChar()){
		dEdgeC = (10/dExtFreqSet)*(0.25); 
		dpintimeset( "CS", "EXT_SYNC", "DefaultTimesetInOut", DPIN_TS_NORMAL, DPIN_RZ, 0.0, dEdgeC * 1E+3 , DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		lwait (200 us_f);

		pldResults = EXT_SYNC_Meas();
		Maxim::DatalogAll(DSIndex, pldResults);
	}
	else
		DSIndex++;

	//Min_Sync_Freq2p2M_50Duty (50% duty, 120% Freq)
	dEdgeC = (10/dExtFreqSet)*(0.5); 
	dpintimeset( "CS", "EXT_SYNC", "DefaultTimesetInOut", DPIN_TS_NORMAL, DPIN_RZ, 0.0, dEdgeC * 1E+3 , DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	lwait (200 us_f);

	pldResults = EXT_SYNC_Meas();
	Maxim::DatalogAll(DSIndex, pldResults);



	if(Maxim::isChar()){
		//Min_Sync_Freq2p2M_HiDuty (75% duty, 120% Freq)
		dEdgeC = (10/dExtFreqSet)*(0.75); 
		dpintimeset( "CS", "EXT_SYNC", "DefaultTimesetInOut", DPIN_TS_NORMAL, DPIN_RZ, 0.0, dEdgeC * 1E+3 , DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		lwait (200 us_f);

		pldResults = EXT_SYNC_Meas();
		Maxim::DatalogAll(DSIndex, pldResults);
	}
	else
		DSIndex++;




	//Max_Sync_Freq2p2M_LoDuty (25% duty, 150% Freq)
	dExtFreqSet = 22 * 1.5;
	mcuset( MCU_MAIN, dExtFreqSet );

	if(Maxim::isChar()){
		dEdgeC = (10/dExtFreqSet)*(0.25); 
		dpintimeset( "CS", "EXT_SYNC", "DefaultTimesetInOut", DPIN_TS_NORMAL, DPIN_RZ, 0.0, dEdgeC * 1E+3 , DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		lwait (200 us_f);

		pldResults = EXT_SYNC_Meas();
		Maxim::DatalogAll(DSIndex, pldResults);
	}
	else
		DSIndex++;

	//Max_Sync_Freq2p2M_50Duty
	dEdgeC = (10/dExtFreqSet)*(0.5); 
	dpintimeset( "CS", "EXT_SYNC", "DefaultTimesetInOut", DPIN_TS_NORMAL, DPIN_RZ, 0.0, dEdgeC * 1E+3 , DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	lwait (200 us_f);

	pldResults = EXT_SYNC_Meas();
	Maxim::DatalogAll(DSIndex, pldResults);


	if(Maxim::isChar()){
		//Max_Sync_Freq2p2M_HiDuty
		dEdgeC = (10/dExtFreqSet)*(0.75); 
		dpintimeset( "CS", "EXT_SYNC", "DefaultTimesetInOut", DPIN_TS_NORMAL, DPIN_RZ, 0.0, dEdgeC * 1E+3 , DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		lwait (200 us_f);

		pldResults = EXT_SYNC_Meas();
		Maxim::DatalogAll(DSIndex, pldResults);
	}
	else
		DSIndex++;




	cbitclose(K37);
	cbitopen(K36);


	apu12setcapload( PEDESTAL2, APU12_FV, 0, APU12_3p6V, APU12_10MA, 1, APU12_PIN_TO_VI );

	qtmustart( QTMU_GP, "OFF", QTMU_RCVH, QTMU_NEG, 50, 200);
	qtmustop( QTMU_GP, "OFF", QTMU_RCVH, QTMU_NEG, 1, 200);
	cbitopen(K21, K43, K44);
	mcuset(MCU_MAIN, 50.0);//50MHz.  (valid range=25-66MHz)
	dpinperiod(125);


    //End of function
    Maxim::DatalogFlush();	//Output the datalog queue.
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC
