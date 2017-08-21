/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:
*		Thermal_Shutdown.cpp
*
*	Written and maintained by:
*		Govit Pimthong (govit.pimthong@MaximIntegrated.com)
*
*	URL:                   $URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/Thermal_Shutdown.cpp $
*	Last Modified by:      $LastChangedBy: govit.pimthong $
*	Date:                  $Date: 2017-06-12 18:47:59 +0700 (Mon, 12 Jun 2017) $
*	Current Rev:           $Revision: 65607 $
*
*	Description:
*		File Description
*
*	Revision History
*
*
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
#include "AP89_FT.h"

// Test Function: Thermal_Shutdown
ETS_PRGFLOW_FUNC Thermal_Shutdown( int DSIndex, LPCTSTR TestLabel )
{
    //declare variables
    PinListData pldResults;	//declare a PLD variable for the measurement results.
     


	Maxim::I2C::SetSlaveAddress(TM_3DV_AD);

	Maxim::I2C::Write(0x02, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x02, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x02, 0x31); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x02, 0x20); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x02, 0x10); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x02, 0xA0); //write DHR<15:0> to DCTLR<15:0>

	Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x01, 0x20); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x01, 0x1C); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>


	Maxim::I2C::Write(0x02, 0x71); //AI<0>=V_PTAT2_S)

	Maxim::I2C::Write(0x02, 0x61); //D0 MUX
	Maxim::I2C::Write(0x02, 0x91); //DI<1>(S_TWRN)

	apu12set(CS, APU12_OFF, 0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI);	
	cbitclose(K6, K10, K32);
	lwait(2 ms_f);
	apu12set(VCC, APU12_FV, 0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI);	
	apu12set(OUT2, APU12_FV, 0, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI);



	THForcePin SetPin = {
		OUT2,
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
		"THERMAL_W_R",               // Rising AWG Name
		"THERMAL_W_F",               // Falling AWG Name
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

	


	Maxim::I2C::Write(0x02, 0x70); //AI<0>=V_PTAT2_S)

	Maxim::I2C::Write(0x02, 0x60); //D0 MUX
	Maxim::I2C::Write(0x02, 0x90); //DI<1>(S_TWRN)


	Maxim::I2C::Write(0x02, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x02, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x02, 0x31); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x02, 0x20); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x02, 0x10); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x02, 0xA0); //write DHR<15:0> to DCTLR<15:0>

	Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x01, 0x44); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x01, 0x20); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x01, 0x1C); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	
	Maxim::I2C::Write(0x01, 0x74); //AI<2>(V_PTAT_S)
	Maxim::I2C::Write(0x01, 0x62); //D1 MUX
	Maxim::I2C::Write(0x01, 0x99); //DI<9>(S_TSDN)
	lwait(1 ms_f);

	tsTH.AwgNameR = "Thermal165_R";
	tsTH.AwgNameF = "Thermal165_F";


	ThresholdTest (DSIndex, &tsTH);



	apu12set(OUT2, APU12_FV, 0, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI);
	apu12set(OUT2, APU12_FV, 0.7, APU12_10V, APU12_1MA, APU12_PIN_TO_VI);
	apu12set(CS, APU12_FV, 0, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI);
	cbitopen(K10);
	lwait(2 ms_f);

	Maxim::I2C::Write(0x01, 0x70); //AI<2>(V_PTAT_S)
	Maxim::I2C::Write(0x01, 0x90); //DI<9>(S_TSDN)


	Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x01, 0x48); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x01, 0x20); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x01, 0x1C); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	
	Maxim::I2C::Write(0x01, 0x78); //AI<3>(V_VPTAT_R_S)
	Maxim::I2C::Write(0x01, 0x62);
	Maxim::I2C::Write(0x01, 0x98); //DI<8>(S_THSD_U)
	lwait(1 ms_f);


	tsTH.SetPin.pinName = CS;
	tsTH.AwgNameR = "Thermal175_R";
	tsTH.AwgNameF = "Thermal175_F";


	ThresholdTest (DSIndex, &tsTH);




	apu12set(CS, APU12_FV, 0, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI);
	apu12set(OUT2, APU12_FV, 0.7, APU12_10V, APU12_1MA, APU12_PIN_TO_VI);
	cbitopen(K6, K10, K32);
   // Maxim::DatalogAll(DSIndex, pldResults * V);	//Datalog the measurements in Volts
     apu12set(VCC, APU12_OFF, 0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI);	
     apu12set(CS, APU12_OFF, 0, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI);	
    //End of function
    Maxim::DatalogFlush();	//Output the datalog queue.
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC
