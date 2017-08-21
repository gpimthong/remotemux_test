/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:
*		Logic_Input.cpp
*
*	Written and maintained by:
*		Govit Pimthong (govit.pimthong@MaximIntegrated.com)
*
*	URL:                   $URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/Logic_Input.cpp $
*	Last Modified by:      $LastChangedBy: govit.pimthong $
*	Date:                  $Date: 2017-07-31 06:41:45 +0700 (Mon, 31 Jul 2017) $
*	Current Rev:           $Revision: 67435 $
*
*	Description:
*		File Description
*
*	Revision History
*
*
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
#include "AP89_FT.h"

// Test Function: Logic_Input
ETS_PRGFLOW_FUNC Logic_Input( int DSIndex, LPCTSTR TestLabel )
{
     
     
    //declare variables
    PinListData pldResults, pldFLT_Low;	//declare a PLD variable for the measurement results.


	apu12set( EN, APU12_FV, 0, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );
	apu12setcapload( IN1, APU12_FV, 12, APU12_30V, APU12_100MA, 2, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	THForcePin SetPin = {
		EN,
		APU12_3p6V,
		APU12_1MA,
		APU12_FV
	};

	THMeasPin MeasPin = {
		VCC,
		APU12_10V,
		APU12_1MA,
		APU12_MV
	};

	TestTH tsTH =
	{
		SetPin,                 // Force pin detail
		MeasPin,                // Meas pin detail
		NULL,            // Threshold set function
		VCC_MeasV,           // Threshold measure function
		"EN_R",               // Rising AWG Name
		"EN_F",               // Falling AWG Name
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


	apu12set( VCC, APU12_OFF, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );



	cbitopen(K40);
	cbitclose(K34, K37, K41, K7);


	apu12set( EN, APU12_FV, 5, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	//Write '1' to ENA bit REG02<5>
	dpinviset("SDA", DPIN_LOAD, 4, DPIN_8V, DPIN_512UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL);
	Maxim::I2C::Write(AP89_AD, 0x02, 0x20);

	lwait (2 ms_f);

	DUT::Initiate_TestMode();


	Maxim::I2C::SetSlaveAddress(OTP_3DV_AD);

	Maxim::I2C::Write(UNLOCK1_REG, 0x17); //Unlock passwd0 = 0x17
	Maxim::I2C::Write(UNLOCK2_REG, 0x65); //Unlock passwd1 = 0x65

	Maxim::I2C::Write(OTPMODE_REG, 0x00); //Turn off Look and bypass


	//Setup
	Maxim::I2C::SetSlaveAddress(TM_3DV_AD);

	Maxim::I2C::Write(0x02, 0xB0); //Reset BIPTM Machine

	Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>=0000
	Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>=1000
	Maxim::I2C::Write(0x01, 0x2B); //write data to DHR1<7:4>=0000
	Maxim::I2C::Write(0x01, 0x10); //write data to DHR0<3:0>=1110
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>

	cbitopen(K40);

	//Setup Force Pin
	tsTH.SetPin.pinName = COMP; //DIM
	tsTH.SetPin.VRange  = APU12_3p6V;
	tsTH.SetPin.IRange = APU12_1MA;
	tsTH.SetPin.ConnectMode = APU12_PIN_TO_VI;

	//Setup Meas Pin
	tsTH.MeasPin.pinName = OUT3; //NDRV
	tsTH.MeasPin.MeasMode = APU12_MV;

	//Setup Threshold Test Detail
	tsTH.TH_Meas = NDRV_MeasV;
	tsTH.AwgNameR = "DIM_R";
	tsTH.AwgNameF = "DIM_F";
	tsTH.bUseAWG = true;
	tsTH.MCU_Divider = 5000;
	tsTH.ThresholdDirection = APU12_ABOVE_THRESH;
	tsTH.step = 0.001;
	tsTH.outTh1 = 2.0; //Output transition point when rising threshold is reached.
	tsTH.outTh2 = tsTH.outTh1; //Output transition point when falling threshold is reached.
	tsTH.Offset = 25;


	apu12set( COMP, APU12_FV, 0, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );
	apu12set( OUT3, APU12_FV, 0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	lwait (200 us_f);
	apu12set( OUT3, APU12_FV, 0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );
	lwait (200 us_f);
	cbitclose(K11);
	lwait (3 ms_f);


	ThresholdTest (DSIndex, &tsTH);
    

	apu12set( COMP, APU12_OFF, 0, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );
	apu12set( OUT3, APU12_FV, 0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );
	lwait (200 us_f);
	cbitopen(K7, K11);
	lwait (3 ms_f);
	apu12set( OUT3, APU12_FV, 0.7, APU12_10V, APU12_100MA, APU12_PIN_TO_VI ); 
	cbitclose(K40);









	//Setup
	Maxim::I2C::SetSlaveAddress(TM_3DV_AD);
	Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x02, 0x61); //D0 MUX
	Maxim::I2C::Write(0x02, 0x95); //DI<5>




	//Setup Force Pin
	tsTH.SetPin.pinName = FSEN;
	tsTH.SetPin.VRange  = APU12_3p6V;
	tsTH.SetPin.IRange = APU12_1MA;
	tsTH.SetPin.ConnectMode = APU12_PIN_TO_VI;

	//Setup Meas Pin
	tsTH.MeasPin.pinName = VCC; //FLT
	tsTH.MeasPin.MeasMode = APU12_MV;

	//Setup Threshold Test Detail
	tsTH.TH_Meas = VCC_MeasV;
	tsTH.AwgNameR = "FSEN_R";
	tsTH.AwgNameF = "FSEN_F";
	tsTH.bUseAWG = true;
	tsTH.MCU_Divider = 5000;
	tsTH.ThresholdDirection = APU12_ABOVE_THRESH;
	tsTH.step = 0.001;
	tsTH.outTh1 = 2.0; //Output transition point when rising threshold is reached.
	tsTH.outTh2 = tsTH.outTh1; //Output transition point when falling threshold is reached.
	tsTH.Offset = 25;


	cbitclose(K32, K6);
	apu12set( FSEN, APU12_FV, 0, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );
	apu12set( VCC, APU12_FV, 0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );


	ThresholdTest (DSIndex, &tsTH);
    

	//Measure also FLT Out low here
	pldFLT_Low = apu12mv(FLT, 10);
	

	cbitopen(K32, K6);
	apu12set( VCC, APU12_OFF, 0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );
	apu12set( FSEN, APU12_FV, 0, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );
	Maxim::I2C::Write(0x02, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x02, 0x60); //D0 MUX
	Maxim::I2C::Write(0x02, 0x90); //DI<5>




	DSIndex+=13; //AP79 thresold test. To implement later

	Maxim::DatalogAll(DSIndex, pldFLT_Low);


    //End of function
    Maxim::DatalogFlush();	//Output the datalog queue.
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC
