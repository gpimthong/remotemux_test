/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:
*		Supply_Current.cpp
*
*	Written and maintained by:
*		Govit Pimthong (govit.pimthong@MaximIntegrated.com)
*
*	URL:                   $URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/Supply_Current.cpp $
*	Last Modified by:      $LastChangedBy: govit.pimthong $
*	Date:                  $Date: 2017-08-17 13:07:41 +0700 (Thu, 17 Aug 2017) $
*	Current Rev:           $Revision: 68112 $
*
*	Description:
*		File Description
*
*	Revision History
*
*
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
#include "AP89_FT.h"

// Test Function: Supply_Current
ETS_PRGFLOW_FUNC Supply_Current( int DSIndex, LPCTSTR TestLabel )
{
	
	PinListData pldResults;

	PinListData I_Standby_4p5;
	PinListData	I_Standby_12;
	PinListData	I_Standby_36;
	PinListData	I_Standby_52;
	PinListData	IQ_IN_4p5V;
	PinListData	IQ_IN_12V;
	PinListData	IQ_IN_36V;
	PinListData	Soft_Start_Check;


	//Prepare for stack-up
	cbitopen(K11, K12, K13, K14, K15, K28);
	dpinviset( "SHARED_DPU", DPIN_FV, 0.0, DPIN_8V, DPIN_512UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL );

	apu12set( CS, APU12_OFF, 0.0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );
	apu12set( VCC, APU12_OFF, 0., APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);

	apu12set( PEDESTAL1, APU12_FV, 0, APU12_30V, APU12_10MA, APU12_PIN_TO_VI );
	apu12set( PEDESTAL2, APU12_FV, 0, APU12_30V, APU12_10MA, APU12_PIN_TO_VI );

	//DIM at 0V
	dpinforce("DIM", "0", 3.0, 0.1, MS_ALL);
	cbitclose(K33, K34, K37, K48);


	apu12set(IN1, APU12_FV, 12, APU12_30V, APU12_1MA, APU12_PIN_TO_VI);
	lwait (10 ms_f);
	apu12set(IN1, APU12_FV, 12, APU12_30V, APU12_100UA, APU12_PIN_TO_VI);
	lwait(20 ms_f);

	I_Standby_12 = apu12mi( IN1, APU12_MI_10X, 100 );


	for(int i = 0; i <= 18; i++){
		apu12set( PEDESTAL1, APU12_FV, i, APU12_30V, APU12_10MA, APU12_PIN_TO_VI );		
		if (i > 12) apu12set(IN1, APU12_FV, i, APU12_30V, APU12_100UA, APU12_PIN_TO_VI);
		lwait (200 us_f);
	}

	lwait (20 ms_f);

	I_Standby_36 = apu12mi( IN1, APU12_MI_10X, 100 );


	for(int i = 18; i <= 26; i++){
		apu12set( PEDESTAL1, APU12_FV, i, APU12_30V, APU12_10MA, APU12_PIN_TO_VI );		
		apu12set(IN1, APU12_FV, i, APU12_30V, APU12_100UA, APU12_PIN_TO_VI);
		lwait (200 us_f);
	}

	lwait (20 ms_f);


	I_Standby_52 = apu12mi( IN1, APU12_MI_10X, 100 );

		
	for(int i = 25; i >= 0; i--){
		apu12set( PEDESTAL1, APU12_FV, i, APU12_30V, APU12_10MA, APU12_PIN_TO_VI );		
		if (i > 4 ) apu12set(IN1, APU12_FV, i, APU12_30V, APU12_100UA, APU12_PIN_TO_VI);
		lwait (300 us_f);
	}

	apu12set(IN1, APU12_FV, 4.5, APU12_30V, APU12_100UA, APU12_PIN_TO_VI);
	lwait (50 ms_f);

	I_Standby_4p5 = apu12mi( IN1, APU12_MI_10X, 100 );


	Maxim::DatalogAll(DSIndex, I_Standby_4p5 * uA);
	Maxim::DatalogAll(DSIndex, I_Standby_12 * uA);
	Maxim::DatalogAll(DSIndex, I_Standby_36 * uA);
	Maxim::DatalogAll(DSIndex, I_Standby_52 * uA);




	cbitopen(K48);


	apu12set( PEDESTAL1, APU12_FV, 0, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
	apu12set(IN1, APU12_FV, 4.5, APU12_30V, APU12_100MA, APU12_PIN_TO_VI);
	cbitclose(K28);
	lwait (500 us_f);
	cbitclose(K31);

	lwait(60 ms_f);

	//DIM high
	dpinforce("DIM", "1", 3.0, 0.1, MS_ALL);

	//BSTMON at 1.3V
	dpinviset( "SHARED_DPU", DPIN_FV, 1.3, DPIN_8V, DPIN_512UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL );

	lwait (10 ms_f);

	

	IQ_IN_4p5V = apu12mi( IN1, APU12_MI_1X, 100 );
	Maxim::DatalogAll(DSIndex, IQ_IN_4p5V);
	

	for(int i = 5; i <= 12; i++){
		apu12set(IN1, APU12_FV, i, APU12_30V, APU12_100MA, APU12_PIN_TO_VI);
		lwait (200 us_f);
	}

	lwait (10 ms_f);

	IQ_IN_12V = apu12mi( IN1, APU12_MI_1X, 100 );
	Maxim::DatalogAll(DSIndex, IQ_IN_12V);

	for(int i = 0; i <= 18; i++){
		apu12set( PEDESTAL1, APU12_FV, i, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );		
		if (i > 12) apu12set(IN1, APU12_FV, i, APU12_30V, APU12_100MA, APU12_PIN_TO_VI);
		lwait (200 us_f);
	}

	lwait (10 ms_f);

	IQ_IN_36V = apu12mi( IN1, APU12_MI_1X, 100 );
	Maxim::DatalogAll(DSIndex, IQ_IN_36V);


	for(int i = 17; i >= 0; i--){
		apu12set( PEDESTAL1, APU12_FV, i, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );		
		if (i > 12) apu12set(IN1, APU12_FV, i, APU12_30V, APU12_100MA, APU12_PIN_TO_VI);
		lwait (200 us_f);
	}

	//BSTMON
	dpinviset( "SHARED_DPU", DPIN_FV, 0.0, DPIN_8V, DPIN_512UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL );
	dpinviset( "SHARED_DPU", DPIN_OFF, 0.0, DPIN_8V, DPIN_512UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL );

	apu12set( PEDESTAL1, APU12_OFF, 0, APU12_30V, APU12_10MA, APU12_PIN_TO_VI );
	apu12set( PEDESTAL2, APU12_OFF, 0, APU12_30V, APU12_10MA, APU12_PIN_TO_VI );

	//DIM at 0V
	dpinforce("DIM", "0", 3.0, 0.1, MS_ALL);
	cbitopen(K31, K33, K23, K50, K40, K16, K17, K34, K37, K48);


	if(gbLastSupply){ //Prepare for Contact Post
	
		apu12setcapload(IN1, APU12_FV, 0.2, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI);
		lwait(1 ms_f);
		apu12set(IN1, APU12_FV, 0, APU12_30V, APU12_100MA, APU12_PIN_TO_VI);
		apu12set(IN1, APU12_OFF, 0, APU12_30V, APU12_1MA, APU12_PIN_TO_VI);
		apu12set(FSEN, APU12_OFF, 0, APU12_30V, APU12_1MA, APU12_PIN_TO_VI);

		//Discharge VCC cap
		apu12set(VCC, APU12_FV, 0, APU12_30V, APU12_10MA, APU12_PIN_TO_VI);
		lwait(5 ms_f);
		apu12set(VCC, APU12_OFF, 0, APU12_30V, APU12_1MA, APU12_PIN_TO_VI);

		cbitopen(K28);

	}

	gbLastSupply = true;

     
    //End of function
    Maxim::DatalogFlush();	//Output the datalog queue.
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC
