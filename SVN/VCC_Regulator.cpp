/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:
*		VCC_Regulator.cpp
*
*	Written and maintained by:
*		Govit Pimthong (govit.pimthong@MaximIntegrated.com)
*
*	URL:                   $URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/VCC_Regulator.cpp $
*	Last Modified by:      $LastChangedBy: govit.pimthong $
*	Date:                  $Date: 2017-08-03 22:36:37 +0700 (Thu, 03 Aug 2017) $
*	Current Rev:           $Revision: 67633 $
*
*	Description:
*		File Description
*
*	Revision History
*
*
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
#include "AP89_FT.h"

// Test Function: VCC_Regulator
ETS_PRGFLOW_FUNC VCC_Regulator( int DSIndex, LPCTSTR TestLabel )
{
    //declare variables
    PinListData pldResults, pldVCC4p2, pldVCCMax, pldVCCMin;	//declare a PLD variable for the measurement results.
    alarmset( ALARM_OVERRANGE, OFF ); 

	Maxim::I2C::Write(0x02, 0xB0); //Reset BIPTM Machine

	Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x01, 0x34); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x01, 0x20); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x01, 0x14); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>

	Maxim::I2C::Write(0x01, 0x71); //AI<0>(V_VCC_K)



	//5.75V 1MA
	apu12setcapload( IN1, APU12_FV, 5.75, APU12_10V, APU12_100MA, 1, APU12_PIN_TO_VI );
	apu12set( VCC, APU12_FV, 0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );
	apu12set( VCC, APU12_FV, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	lwait(10 ms_f);

	pldResults = apu12mv(COMP, 10);
	Maxim::DatalogAll(DSIndex, pldResults);

	pldResults = apu12mi(VCC, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResults);


	//5.75V 10MA
	apu12set( VCC, APU12_FV, 0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	lwait(10 ms_f);

	pldResults = apu12mv(COMP, 10);
	Maxim::DatalogAll(DSIndex, pldResults);

	pldResults = apu12mi(VCC, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResults);




	//4.2V 10MA
	apu12setcapload( IN1, APU12_FV, 4.2, APU12_10V, APU12_100MA, 1, APU12_PIN_TO_VI );
	lwait(5 ms_f);

	pldResults = apu12mv(COMP, 10);
	Maxim::DatalogAll(DSIndex, pldResults);







	//VCC Dropout
	PinListData pldVCC_DropOut, pldVCCv, pldINv;
	apu12setcapload( IN1, APU12_FV, 4.5, APU12_10V, APU12_100MA, 1, APU12_PIN_TO_VI );
	apu12set( BSTMON, APU12_FI, 0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );
	lwait(1 ms_f);

	apu12set( VCC, APU12_FI, -5.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	lwait (5 ms_f);

	pldINv = apu12mv(BSTMON, 100, 13.0 );  //kelvin sense of IN for LDO power
	pldVCCv = apu12mv(COMP, 100, 13.0 ); //kelvin sense of VCC for LDO power

	//computation
	pldVCC_DropOut = pldINv - pldVCCv;


	Maxim::DatalogAll(DSIndex, pldINv);
	Maxim::DatalogAll(DSIndex, pldVCCv);	
	Maxim::DatalogAll(DSIndex, pldVCC_DropOut);


	apu12set( VCC, APU12_FI, 0.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	lwait (2 ms_f);


	pldVCCv = apu12mv(COMP, 100, 13.0 ); //kelvin sense of VCC for LDO power






	//SHORT ILIM
	apu12setcapload( IN1, APU12_FV, 5.5, APU12_10V, APU12_100MA, 1, APU12_PIN_TO_VI );
	lwait(1 ms_f);
	apu12set( VCC, APU12_FI, 0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	lwait(1 ms_f);
	apu12set( VCC, APU12_FV, 4.90, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	apu12set( VCC, APU12_FV, 4.90, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	
	lwait (5 ms_f);

	pldResults = apu12mi(VCC, APU12_MI_1X, 100);

	Maxim::DatalogAll(DSIndex, -1 * pldResults);



	apu12set( VCC, APU12_FV, 4.90, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	lwait(1 ms_f);






	//ACCUMULATOR_STR VCC_v[NUM_SITES];
	//
	//for(int isite = 0; isite<NUM_SITES; isite++){

	//	if (msSiteStat(isite)){
	//		VCC_v[isite].value1 = pldVCCv[isite] + 0.005;
	//	}
	//	else{
	//		VCC_v[isite].value1 = 0;
	//	}

	//}

	//apu12set( VCC, APU12_FV, 0.0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );
	//lwait (1 ms_f);

	//cbitclose(K6, K9);
	//lwait(3 ms_f);

	//groupsetaccumulator(VCC_v, NUM_SITES);
	//apu12set( VCC_COMP, APU12_FV, USE_ACCUM, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	//lwait(1 ms_f);
	//apu12set( VCC_COMP, APU12_FV, USE_ACCUM, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );





	////VCC UVLO
	//Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
	//Maxim::I2C::Write(0x01, 0x41); //write data to DHR3<15:12>
	//Maxim::I2C::Write(0x01, 0x36); //write data to DHR2<11:8>
	//Maxim::I2C::Write(0x01, 0x20); //write data to DHR1<7:4>
	//Maxim::I2C::Write(0x01, 0x14); //write data to DHR0<3:0>
	//Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>

	//Maxim::I2C::Write(0x01, 0x97); 
	//Maxim::I2C::Write(0x01, 0x61);
	//lwait(1 ms_f);


	//apu12setcapload( IN1, APU12_FV, 10, APU12_10V, APU12_100MA, 1, APU12_PIN_TO_VI );
	//lwait(1 ms_f);
	//cbitclose(K32);
	//apu12set( VCC_COMP, APU12_FV, 3.8, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );




	//apu12set( VCC_COMP, APU12_FV, 4.5, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	//apu12set( VCC_COMP, APU12_FV, 0.2, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	//lwait(500 us_f);
	//apu12set( VCC_COMP, APU12_FV, 0.2, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	//apu12set( VCC_COMP, APU12_FV, 0.0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );

	//cbitopen(K6, K9, K32, K46, K45, K52);


	apu12set( VCC, APU12_FI, 0.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	lwait(1 ms_f);
	apu12set( VCC, APU12_OFF, 0, APU12_10V, APU12_100UA, APU12_PIN_TO_BUSA );
	apu12set( COMP, APU12_OFF, 0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );

	apu12set( PEDESTAL1, APU12_FV, 0, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
	apu12set( EN, APU12_FV, 0.2, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	apu12set( BSTMON, APU12_OFF, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	lwait(2 ms_f);
	apu12set( EN, APU12_OFF, 0.2, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	apu12setcapload( IN1, APU12_FV, 0.5, APU12_10V, APU12_100MA, 1, APU12_PIN_TO_VI_AND_BUSA );
	lwait(1 ms_f);


	//tie IN, EN and VCC, measure RT
	cbitclose(K4, K31);
	lwait(3 ms_f);
	apu12set( CS, APU12_FV, 0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );




	THForcePin SetPin = {
		IN1,
		APU12_10V,
		APU12_100MA,
		APU12_FV,
		APU12_PIN_TO_VI_AND_BUSA
	};

	THMeasPin MeasPin = {
		CS,
		APU12_10V,
		APU12_10UA,
		APU12_MV
	};

	TestTH tsTH =
	{
		SetPin,                 // Force pin detail
		MeasPin,                // Meas pin detail
		NULL,            // Threshold set function
		CS_MeasV,           // Threshold measure function
		"VCC_UVLO_R",               // Rising AWG Name
		"VCC_UVLO_F",               // Falling AWG Name
		true,                   // Use AWG
		5000,                  // MCU divider
		APU12_ABOVE_THRESH,     // Threshold direction for rising, falling will be the opposite.
		1 mV_f,                 // Step size
		1.0,        // Output transition point when rising threshold is reached.
		1.0,        // Output transition point when falling threshold is reached.
		1,                     // Sample to skip before using awgwhereat to calculate threshold (in case using AWG)
		false,                  // Find threhsold from right (not usual case)
		false,                  // True if datalong is in percentage of some REF value
		0,                      // Ref value associate with percentage.
		25,                      // Offset to use with Go-no-Go , this number will be multiply with step size.
		1.0,					//Ref value in pld format
		true					//Falling search
	};


	ThresholdTest (DSIndex, &tsTH);




	cbitopen(K4, K31);

	cbitclose(K16);
	lwait(3 ms_f);
	apu12set( CS, APU12_OFF, 0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );
	apu12setcapload( IN1, APU12_FV, 26, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );
	apu12set( EN, APU12_FV, 5, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	lwait (2 ms_f);


	DUT::Initiate_TestMode();


	Maxim::I2C::SetSlaveAddress(OTP_3DV_AD);

	Maxim::I2C::Write(UNLOCK1_REG, 0x17); //Unlock passwd0 = 0x17
	Maxim::I2C::Write(UNLOCK2_REG, 0x65); //Unlock passwd1 = 0x65

	Maxim::I2C::SetSlaveAddress(TM_3DV_AD);

	Maxim::I2C::Write(0x02, 0xB0); //Reset BIPTM Machine

	Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x01, 0x34); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x01, 0x20); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x01, 0x14); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>

	Maxim::I2C::Write(0x01, 0x71); //AI<0>(V_VCC_K)

	int iStackV = 10;

	for(int i = 0; i <= iStackV; i++){
			apu12set( PEDESTAL1, APU12_FV, i, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
			lwait (200 us_f);
	}
	
	
	apu12set( VCC, APU12_FV, -iStackV, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );
	apu12set( COMP, APU12_FI, 0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );

	apu12set( VCC, APU12_FV, -iStackV, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	//36V 10MA
	pldResults = apu12mv(COMP, 10) + iStackV;
	Maxim::DatalogAll(DSIndex, pldResults);

	pldResults = apu12mi(VCC, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResults);

	int iVCC_Sample = 200;

	//VCC for 200 sample
	pldVCCMax = pldVCCMin = apu12mv(COMP, iVCC_Sample, 20);
	
	namearray( "VCCdata", NAMEARRAY_APU12ADC, COMP, 1 );//put site array into work area for analysis
	pldVCCMax = peak("VCCdata", 1, iVCC_Sample, PEAK_HI) + iStackV;
	pldVCCMin = peak("VCCdata", 1, iVCC_Sample, PEAK_LO) + iStackV;

	//36V 1MA
	apu12set( VCC, APU12_FV, -iStackV, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	lwait(10 ms_f);

	pldResults = apu12mv(COMP, 10) + iStackV;
	Maxim::DatalogAll(DSIndex, pldResults);

	pldResults = apu12mi(VCC, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResults);

	apu12set( VCC, APU12_FV, iStackV, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );
	apu12set( VCC, APU12_OFF, 0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );
	apu12set( COMP, APU12_OFF, 0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );

	apu12setcapload( IN1, APU12_FV, 12, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );

	for(int i = iStackV; i >= 0; i--){

			apu12set( PEDESTAL1, APU12_FV, i, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
			lwait (200 us_f);

	}
	
	cbitopen(K16);
	apu12set( PEDESTAL1, APU12_OFF, 0, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
	


	//To insert VCC swing test here
	Maxim::DatalogAll(DSIndex, pldVCCMin);
	Maxim::DatalogAll(DSIndex, pldVCCMax);





	alarmset( ALARM_OVERRANGE, ON ); 
    //End of function
    Maxim::DatalogFlush();	//Output the datalog queue.
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC
