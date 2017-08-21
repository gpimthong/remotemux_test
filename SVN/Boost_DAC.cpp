/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:
*		Boost_DAC.cpp
*
*	Written and maintained by:
*		Govit Pimthong (govit.pimthong@MaximIntegrated.com)
*
*	URL:                   $URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/Boost_DAC.cpp $
*	Last Modified by:      $LastChangedBy: kanokwan.jitkoolsamp $
*	Date:                  $Date: 2017-06-06 18:02:08 +0700 (Tue, 06 Jun 2017) $
*	Current Rev:           $Revision: 65370 $
*
*	Description:
*		File Description
*
*	Revision History
*
*
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
#include "AP89_FT.h"


// Test Function: Boost_DAC
ETS_PRGFLOW_FUNC Boost_DAC( int DSIndex, LPCTSTR TestLabel )
{
     
    //declare variables
    PinListData pldResults;	//declare a PLD variable for the measurement results.
	PinListData pldDAC_EAREF_600, pldDAC_EAREF_125;
     
	apu12set(BSTMON, APU12_FV, 1.3 V_f, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI); //
	apu12set( COMP, APU12_FI, 0, APU12_3p6V, APU12_10UA, APU12_PIN_TO_VI );
	lwait (1 ms_f);



	//DAC_EAREF_600mV

	Maxim::I2C::SetSlaveAddress(TM_3DV_AD);

	Maxim::I2C::Write(0x02, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x02, 0x42); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x02, 0x30); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x02, 0x20); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x02, 0x10); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x02, 0xA0); //write DHR<15:0> to DCTLR<15:0>

	Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x01, 0x31); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x01, 0x21); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x01, 0x1C); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	
	Maxim::I2C::Write(0x01, 0x82); //AI<5>(V_DACKV)

	cbitclose(K18); //CS Gnd sense
	lwait (10 ms_f);


	pldDAC_EAREF_600 = pldResults = apu12mv(COMP, 100);

	Maxim::DatalogAll(DSIndex, pldResults);





	//DAC_EAREF_1p25V
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x01, 0x31); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x01, 0x22); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x01, 0x1C); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	lwait (10 ms_f);


	pldDAC_EAREF_125 = pldResults = apu12mv(COMP, 100);

	Maxim::DatalogAll(DSIndex, pldResults);




	//DAC_EAREF_925mV
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x01, 0x31); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x01, 0x23); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x01, 0x1C); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>

	Maxim::I2C::Write(0x01, 0x93); //DI<3>(S_COMPOVTH)
	Maxim::I2C::Write(0x01, 0x61); //Enable D0
	lwait (10 ms_f);


	pldResults = apu12mv(COMP, 100);

	Maxim::DatalogAll(DSIndex, pldResults);



	//DAC_EAREF_Threshold

	cbitclose(K6, K32);
	apu12set(VCC, APU12_FV, 0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI);
	apu12set(BSTMON, APU12_FV, 0.7 V_f, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI);
	lwait (2 ms_f);


	THForcePin SetPin = {
		BSTMON,
		APU12_3p6V,
		APU12_1MA,
		APU12_FV
	};

	THMeasPin MeasPin = {
		VCC,
		APU12_10V,
		APU12_100UA,
		APU12_MV
	};

	TestTH tsTH =
	{
		SetPin,                 // Force pin detail
		MeasPin,                // Meas pin detail
		NULL,            // Threshold set function
		VCC_MeasV,           // Threshold measure function
		"DAC_EAREF_R",               // Rising AWG Name
		"DAC_EAREF_F",               // Falling AWG Name
		true,                   // Use AWG
		5000,                  // MCU divider
		APU12_ABOVE_THRESH,     // Threshold direction for rising, falling will be the opposite.
		1 mV_f,                 // Step size
		1,        // Output transition point when rising threshold is reached.
		1,        // Output transition point when falling threshold is reached.
		1,                     // Sample to skip before using awgwhereat to calculate threshold (in case using AWG)
		false,                  // Find threhsold from right (not usual case)
		false,                  // True if datalong is in percentage of some REF value
		0,                      // Ref value associate with percentage.
		10,                      // Offset to use with Go-no-Go , this number will be multiply with step size.
		1.0,					//Ref value in pld format
		false					//Falling search
	};


	ThresholdTest (DSIndex, &tsTH);

	apu12set(VCC, APU12_OFF, 0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI);
	cbitopen(K6, K32);



	PinListData pldDACx[8];
	int ipnt;
	ipnt = DSIndex;


	//DAC_1
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x01, 0x31); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x01, 0x25); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x01, 0x1C); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>

	Maxim::I2C::Write(0x01, 0x90); //DI<3>(S_COMPOVTH)
	Maxim::I2C::Write(0x01, 0x60); //Enable D0
	lwait (10 ms_f);

	pldDACx[DSIndex - ipnt] = pldResults = apu12mv(COMP, 100);
	Maxim::DatalogAll(DSIndex, pldResults);





	//DAC_2
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x01, 0x31); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x01, 0x26); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x01, 0x1C); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	lwait (10 ms_f);

	pldDACx[DSIndex - ipnt] = pldResults = apu12mv(COMP, 100);
	Maxim::DatalogAll(DSIndex, pldResults);





	//DAC_4
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x01, 0x31); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x01, 0x27); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x01, 0x1C); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	lwait (10 ms_f);

	pldDACx[DSIndex - ipnt] = pldResults = apu12mv(COMP, 100);
	Maxim::DatalogAll(DSIndex, pldResults);





	//DAC_8
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x01, 0x31); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x01, 0x28); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x01, 0x1C); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	lwait (10 ms_f);

	pldDACx[DSIndex - ipnt] = pldResults = apu12mv(COMP, 100);
	Maxim::DatalogAll(DSIndex, pldResults);





	//DAC_16
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x01, 0x31); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x01, 0x29); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x01, 0x1C); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	lwait (10 ms_f);

	pldDACx[DSIndex - ipnt] = pldResults = apu12mv(COMP, 100);
	Maxim::DatalogAll(DSIndex, pldResults);





	//DAC_32
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x01, 0x31); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x01, 0x2A); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x01, 0x1C); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	lwait (10 ms_f);

	pldDACx[DSIndex - ipnt] = pldResults = apu12mv(COMP, 100);
	Maxim::DatalogAll(DSIndex, pldResults);





	//DAC_64
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x01, 0x31); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x01, 0x2B); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x01, 0x1C); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	lwait (10 ms_f);

	pldDACx[DSIndex - ipnt] = pldResults = apu12mv(COMP, 100);
	Maxim::DatalogAll(DSIndex, pldResults);





	//DAC_128
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x01, 0x31); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x01, 0x2C); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x01, 0x1C); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	lwait (10 ms_f);

	pldDACx[DSIndex - ipnt] = pldResults = apu12mv(COMP, 100);
	Maxim::DatalogAll(DSIndex, pldResults);





    //*********************************************************
    // Test: INL
    // DAC_x - [DAC_EAREF_600mV + y*(DAC_EAREF_1p25V - DAC_EAREF_600mV)/255]
    //*********************************************************

	pldResults = pldResults * 9999;

	for(int i=0; i< 8; i++){
		pldResults = (pldDACx[i] - (pldDAC_EAREF_600 + (1<<i) * (pldDAC_EAREF_125 - pldDAC_EAREF_600)/255));
		Maxim::DatalogAll(DSIndex, pldResults * mV);
	}


	//DAC H_Range
	pldResults = pldDACx[0] - pldDAC_EAREF_600;
	Maxim::DatalogAll(DSIndex, pldResults * mV);




	//DAC_0 L_Range
	Maxim::I2C::Write(0x02, 0x46); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x02, 0x30); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x02, 0x20); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x02, 0x10); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x02, 0xA0); //write DHR<15:0> to DCTLR<15:0>

	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x01, 0x31); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x01, 0x21); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x01, 0x1C); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	lwait (10 ms_f);

	pldDACx[0] = pldResults = apu12mv(COMP, 100);
	Maxim::DatalogAll(DSIndex, pldResults);




	//DAC_1 L_Range
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x01, 0x31); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x01, 0x25); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x01, 0x1C); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	lwait (10 ms_f);

	pldDACx[1] = pldResults = apu12mv(COMP, 100);
	Maxim::DatalogAll(DSIndex, pldResults);



	//DAC_LSB_L_Range


	pldResults = pldDACx[1] - pldDACx[0];
	Maxim::DatalogAll(DSIndex, pldResults * mV);





	cbitopen(K18); //CS Gnd sense
	apu12set( COMP, APU12_OFF, 0, APU12_3p6V, APU12_10UA, APU12_PIN_TO_VI );

     






    //End of function
    Maxim::DatalogFlush();	//Output the datalog queue.
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC
