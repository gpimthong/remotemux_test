/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:
*		OverVoltage_UnderVoltage.cpp
*
*	Written and maintained by:
*		Govit Pimthong (govit.pimthong@MaximIntegrated.com)
*
*	URL:                   $URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/OverVoltage_UnderVoltage.cpp $
*	Last Modified by:      $LastChangedBy: govit.pimthong $
*	Date:                  $Date: 2017-07-26 17:31:15 +0700 (Wed, 26 Jul 2017) $
*	Current Rev:           $Revision: 67256 $
*
*	Description:
*		File Description
*
*	Revision History
*
*
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
#include "AP89_FT.h"

void BSTMON_Toggle(LPCTSTR some_name, int some_int){

	apu12set( BSTMON, APU12_FV, 0.1, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);
	apu12set( BSTMON, APU12_FV, 0.7, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI );

}

// Test Function: OverVoltage_UnderVoltage
ETS_PRGFLOW_FUNC OverVoltage_UnderVoltage( int DSIndex, LPCTSTR TestLabel )
{

    //declare variables
    PinListData pldResults, pldPGATE_PD;	//declare a PLD variable for the measurement results.
     

	apu12set( ALL_OUT, APU12_FV, 0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );
	cbitclose(K11);
	apu12setcapload( BSTMON, APU12_FV, 0, APU12_10V, APU12_10MA, 1, APU12_PIN_TO_VI );
	apu12set( FSEN, APU12_FV, 0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	apu12set( CS, APU12_OFF, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	apu12set( BSTMON, APU12_FV, 0.7, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI );
	
	dpinforce("DIM", "1", 3.0, 0.1, MS_ALL);

	apu12set( EN, APU12_FV, 5, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	//Write '1' to ENA bit REG02<5>
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
	Maxim::I2C::Write(0x02, 0x40); //write data to DHR3<15:12>=0010
	Maxim::I2C::Write(0x02, 0x32); //write data to DHR2<11:8>=0000
	Maxim::I2C::Write(0x02, 0x20); //write data to DHR1<7:4>=0000
	Maxim::I2C::Write(0x02, 0x10); //write data to DHR0<3:0>=0000
	Maxim::I2C::Write(0x02, 0xA0); //write DHR<15:0> to DCTLR<15:0>

	Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>=0000
	Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>=1000
	Maxim::I2C::Write(0x01, 0x2D); //write data to DHR1<7:4>=0000
	Maxim::I2C::Write(0x01, 0x10); //write data to DHR0<3:0>=1110
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>


	cbitclose(K14);
	lwait (3 ms_f);
	apu12setcapload( OUT6, APU12_FV, 2, APU12_10V, APU12_1MA, 1, APU12_PIN_TO_VI );
	lwait (1 ms_f);

	pldPGATE_PD = apu12mi(OUT6, APU12_MI_1X, 10);


	apu12setcapload( OUT6, APU12_FV, 0, APU12_10V, APU12_1MA, 1, APU12_PIN_TO_VI );
	lwait (1 ms_f);
	cbitopen(K14);
	lwait (3 ms_f);
	apu12setcapload( OUT6, APU12_FV, 0, APU12_10V, APU12_10UA, 1, APU12_PIN_TO_VI );
	
	//BSTMON OV	
	
	THForcePin SetPin = {
		BSTMON,
		APU12_3p6V,
		APU12_10MA,
		APU12_FV
	};

	THMeasPin MeasPin = {
		OUT3,//NDRV
		APU12_10V,
		APU12_10UA,
		APU12_MV
	};

	TestTH tsTH =
	{
		SetPin,                 // Force pin detail
		MeasPin,                // Meas pin detail
		NULL,            // Threshold set function
		NDRV_MeasV,           // Threshold measure function
		"BSTMON_OV_R",               // Rising AWG Name
		"BSTMON_OV_F",               // Falling AWG Name
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


	ThresholdTest (DSIndex, &tsTH);




	//BSTMON UV

	tsTH.AwgNameR = "BSTMON_UV_R";
	tsTH.AwgNameF = "BSTMON_UV_F";
	tsTH.ThresholdDirection = APU12_BELOW_THRESH;

	apu12set( BSTMON, APU12_FV, 0.2, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI );

	ThresholdTest (DSIndex, &tsTH);

	apu12set( BSTMON, APU12_FV, 0.7, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI );




	//Insert UV Blaning time here
	Maxim::DatalogAll(DSIndex, gpldBSTMON_UV_BL * ms);
	



	//Enable 10us delay
	Maxim::I2C::Write(0x02, 0x30); //write data to DHR2<11:8>=0000
	Maxim::I2C::Write(0x02, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	cbitclose(K44, K50);

	pldResults = MeasureTime(QTMU_GP, direct_v, direct_v, "AUTO", 1, BSTMON_Toggle, "", 1 ms_f, QTMU_DIRECTA, QTMU_DIRECTB, QTMU_NEG, QTMU_POS, 0, 0, 0.4 , 2.5, 1000, 1000);

	Maxim::DatalogAll(DSIndex, pldResults * us);



	
	Maxim::DatalogAll(DSIndex, pldPGATE_PD * uA);


	cbitopen(K11, K44, K50);

    //End of function
    Maxim::DatalogFlush();	//Output the datalog queue.
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC
