/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:
*		LED_Fault_Detection.cpp
*
*	Written and maintained by:
*		Govit Pimthong (govit.pimthong@MaximIntegrated.com)
*
*	URL:                   $URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/LED_Fault_Detection.cpp $
*	Last Modified by:      $LastChangedBy: govit.pimthong $
*	Date:                  $Date: 2017-08-01 01:01:53 +0700 (Tue, 01 Aug 2017) $
*	Current Rev:           $Revision: 67487 $
*
*	Description:
*		File Description
*
*	Revision History
*
*
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
#include "AP89_FT.h"

void DIM_toggle(LPCTSTR some_name, int some_int){

	dpinforce("DIM", "1", 3.0, 0.1, MS_ALL);
	lwait(100 us_f);
	dpinforce("DIM", "0", 3.0, 0.1, MS_ALL); //DIM at 0V

}


// Test Function: LED_Fault_Detection
ETS_PRGFLOW_FUNC LED_Fault_Detection( int DSIndex, LPCTSTR TestLabel )
{
    //declare variables
    PinListData pldResults;	//declare a PLD variable for the measurement results.
	PME_INT OUT_PIN[6] = {OUT1, OUT2, OUT3, OUT4, OUT5, OUT6};
	std::string AWG_Label;

	if(!AP89){

		//Short detection threshold for AP79

	}
	else
		DSIndex+=12;



	//LED Source current
	Maxim::I2C::SetSlaveAddress(TM_3DV_AD);

	Maxim::I2C::Write(0x02, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x02, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x02, 0x38); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x02, 0x20); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x02, 0x10); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x02, 0xA0); //write DHR<15:0> to DCTLR<15:0>

	Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x01, 0x20); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x01, 0x10); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>



	dpinforce("DIM", "0", 3.0, 0.1, MS_ALL); //DIM at 0V
	lwait(1 ms_f);

	apu12set( ALL_OUT, APU12_FV, 0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );
	lwait(50 ms_f);


	pldResults = apu12mi(OUT1, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResults * -1 * uA);

	pldResults = apu12mi(OUT2, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResults * -1 * uA);

	pldResults = apu12mi(OUT3, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResults * -1 * uA);


	pldResults = apu12mi(OUT4, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResults * -1 * uA);

	if(AP89){
		pldResults = apu12mi(OUT5, APU12_MI_1X, 10);
		Maxim::DatalogAll(DSIndex, pldResults * -1 * uA);

		pldResults = apu12mi(OUT6, APU12_MI_1X, 10);
		Maxim::DatalogAll(DSIndex, pldResults * -1 * uA);
	}
	else
		DSIndex+=2;





	//Min delay
	cbitclose(K32, K45, K49);

	Maxim::I2C::Write(0x02, 0x48); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x02, 0x30); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x02, 0x20); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x02, 0x10); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x02, 0xA0); //write DHR<15:0> to DCTLR<15:0>

	Maxim::I2C::Write(0x01, 0x9A); //write DI<10>=1 to connect DI<10>(F_6U_OR_LEDDELAY) 
	Maxim::I2C::Write(0x01, 0x62); //enable D1 DMUX
	lwait (2 ms_f);

	pldResults = MeasureTime(QTMU_GP, direct_v, direct_v, "AUTO", 1, DIM_toggle, "", 1 ms_f, QTMU_DIRECTA, QTMU_DIRECTB, QTMU_POS, QTMU_POS, 0, 0, 2.0 , 2.0, 2000, 2000);
	Maxim::DatalogAll(DSIndex, pldResults * us);



	Maxim::I2C::Write(0x01, 0x9B); //write DI<10>=1 to connect DI<10>(F_6U_OR_LEDDELAY) 
	lwait (2 ms_f);

	pldResults = MeasureTime(QTMU_GP, direct_v, direct_v, "AUTO", 1, DIM_toggle, "", 1 ms_f, QTMU_DIRECTA, QTMU_DIRECTB, QTMU_POS, QTMU_POS, 0, 0, 2.0 , 2.0, 2000, 2000);
	Maxim::DatalogAll(DSIndex, pldResults * us);






	apu12set(VCC, APU12_FV, 0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI);
	Maxim::I2C::Write(0x02, 0xB0); //Reset
	Maxim::I2C::Write(0x01, 0x9C);
	cbitopen(K45, K49);
	cbitclose(K6);


	apu12set( ALL_OUT, APU12_FV, 0.7, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );


	THForcePin SetPin = {
		OUT1,
		APU12_3p6V,
		APU12_1MA,
		APU12_FV
	};

	THMeasPin MeasPin = {
		VCC,
		APU12_10V,
		APU12_10UA,
		APU12_MV
	};

	TestTH tsTH =
	{
		SetPin,                 // Force pin detail
		MeasPin,                // Meas pin detail
		NULL,            // Threshold set function
		VCC_MeasV,           // Threshold measure function
		"OUT1_SG_R",               // Rising AWG Name
		"OUT1_SG_F",               // Falling AWG Name
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
		10,                      // Offset to use with Go-no-Go , this number will be multiply with step size.
		1.0,					//Ref value in pld format
		true					//Falling search
	};



	OUT_SG_ThresholdTest (DSIndex, &tsTH);

	apu12set( OUT1, APU12_FV, 0.7, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );



	//Setup Force Pin
	tsTH.SetPin.pinName = OUT2;

	//Setup Threshold Test Detail
	tsTH.AwgNameF = "OUT2_SG_F";
	
	OUT_SG_ThresholdTest (DSIndex, &tsTH);

	apu12set(OUT2, APU12_FV, 0.7, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI);


	//Setup Force Pin
	tsTH.SetPin.pinName = OUT3;

	//Setup Threshold Test Detail
	tsTH.AwgNameF = "OUT3_SG_F";
	
	OUT_SG_ThresholdTest (DSIndex, &tsTH);

	apu12set(OUT3, APU12_FV, 0.7, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI);


	//Setup Force Pin
	tsTH.SetPin.pinName = OUT4;

	//Setup Threshold Test Detail
	tsTH.AwgNameF = "OUT4_SG_F";
	
	OUT_SG_ThresholdTest (DSIndex, &tsTH);

	apu12set(OUT4, APU12_FV, 0.7, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI);





	if(AP89){



		//Setup Force Pin
		tsTH.SetPin.pinName = OUT5;

		//Setup Threshold Test Detail
		tsTH.AwgNameF = "OUT5_SG_F";

		OUT_SG_ThresholdTest (DSIndex, &tsTH);

		apu12set(OUT5, APU12_FV, 0.7, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI);



		//Setup Force Pin
		tsTH.SetPin.pinName = OUT6;

		//Setup Threshold Test Detail
		tsTH.AwgNameF = "OUT6_SG_F";

		OUT_SG_ThresholdTest (DSIndex, &tsTH);

		apu12set(OUT6, APU12_FV, 0.7, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI);



	}
	else
		DSIndex+=6;






	apu12set(ALL_OUT, APU12_FV, 1.5, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI);

	Maxim::I2C::Write(0x02, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x02, 0x38); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x02, 0x20); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x02, 0x10); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x02, 0xA0); //write DHR<15:0> to DCTLR<15:0>

	Maxim::I2C::Write(0x01, 0x9D); //Unused detection






	//Setup Force Pin
	tsTH.SetPin.pinName = OUT1;

	//Setup Threshold Test Detail
	tsTH.AwgNameR = "OUT1_UnUsed_R";
	tsTH.ThresholdDirection = APU12_BELOW_THRESH;

	OUT_Unused_ThresholdTest (DSIndex, &tsTH);

	apu12set(OUT1, APU12_FV, 1.5, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI);



	//Setup Force Pin
	tsTH.SetPin.pinName = OUT2;

	//Setup Threshold Test Detail
	tsTH.AwgNameR = "OUT2_UnUsed_R";

	OUT_Unused_ThresholdTest (DSIndex, &tsTH);

	apu12set(OUT2, APU12_FV, 1.5, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI);





	//Setup Force Pin
	tsTH.SetPin.pinName = OUT3;

	//Setup Threshold Test Detail
	tsTH.AwgNameR = "OUT3_UnUsed_R";

	OUT_Unused_ThresholdTest (DSIndex, &tsTH);

	apu12set(OUT3, APU12_FV, 1.5, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI);






	//Setup Force Pin
	tsTH.SetPin.pinName = OUT4;

	//Setup Threshold Test Detail
	tsTH.AwgNameR = "OUT4_UnUsed_R";

	OUT_Unused_ThresholdTest (DSIndex, &tsTH);

	apu12set(OUT4, APU12_FV, 1.5, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI);



	if(AP89){


		//Setup Force Pin
		tsTH.SetPin.pinName = OUT5;

		//Setup Threshold Test Detail
		tsTH.AwgNameR = "OUT5_UnUsed_R";

		OUT_Unused_ThresholdTest (DSIndex, &tsTH);

		apu12set(OUT5, APU12_FV, 1.5, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI);






		//Setup Force Pin
		tsTH.SetPin.pinName = OUT6;

		//Setup Threshold Test Detail
		tsTH.AwgNameR = "OUT6_UnUsed_R";

		OUT_Unused_ThresholdTest (DSIndex, &tsTH);

		apu12set(OUT6, APU12_FV, 1.5, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI);



	}
	else
		DSIndex+=6;




	//Open detection is the same as short to ground detection comparator
	DSIndex+=18;




	//Short detection threshold
	//SLDET[1:0] = 01h


	Maxim::I2C::SetSlaveAddress(TM_3DV_AD);
	Maxim::I2C::Write(0x02, 0x41); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x02, 0x30); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x02, 0x20); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x02, 0x10); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x02, 0xA0); //write DHR<15:0> to DCTLR<15:0>

	Maxim::I2C::Write(0x01, 0x9F); //Unused detection

	tsTH.SetPin.VRange = APU12_10V;
	tsTH.ThresholdDirection = APU12_ABOVE_THRESH;
	tsTH.Offset = 20; //Need this for COLD

	double dThr = 2.5;

	for(int iSLDET_Code = 1; iSLDET_Code < 4; iSLDET_Code++){

		if(iSLDET_Code == 2)
			dThr = 5;
		if(iSLDET_Code == 3)
			dThr = 7;

		Maxim::I2C::Write(AP89_AD, 0x12, 0x10 + iSLDET_Code );
		apu12set(ALL_OUT, APU12_FV, dThr, APU12_10V, APU12_1MA, APU12_PIN_TO_VI);
		lwait(5 ms_f);


		for(int ich=0; ich<giLED_CH; ich++){


			//Setup Force Pin
			tsTH.SetPin.pinName = OUT_PIN[ich];

			AWG_Label = "OUT_SHRT" + std::to_string( static_cast<long long> (ich)) + std::to_string( static_cast<long long> (iSLDET_Code));

			//Setup Threshold Test Detail
			tsTH.AwgNameR = AWG_Label.c_str();

			OUT_SHRT_ThresholdTest (DSIndex, &tsTH);

			apu12set(OUT_PIN[ich], APU12_FV, dThr, APU12_10V, APU12_1MA, APU12_PIN_TO_VI);


		}

		if(!AP89)
			DSIndex+=6;

	}



	apu12set(ALL_OUT, APU12_FV, 0.7, APU12_10V, APU12_1MA, APU12_PIN_TO_VI);

	cbitopen(K6, K32, K45, K49);
	apu12set(VCC, APU12_OFF, 0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI);

    //End of function
    Maxim::DatalogFlush();	//Output the datalog queue.
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC
