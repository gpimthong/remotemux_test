/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:
*		LED_Current_Measurement.cpp
*
*	Written and maintained by:
*		Govit Pimthong (govit.pimthong@MaximIntegrated.com)
*
*	URL:                   $URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/LED_Current_Measurement.cpp $
*	Last Modified by:      $LastChangedBy: govit.pimthong $
*	Date:                  $Date: 2017-07-12 14:02:24 +0700 (Wed, 12 Jul 2017) $
*	Current Rev:           $Revision: 66719 $
*
*	Description:
*		File Description
*
*	Revision History
*
*
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
#include "AP89_FT.h"

// Test Function: LED_Current_Measurement
ETS_PRGFLOW_FUNC LED_Current_Measurement( int DSIndex, LPCTSTR TestLabel )
{     
    //declare variables
    PinListData pldResults;	//declare a PLD variable for the measurement results.
     


	DSIndex+=6;


	apu12set(CS, APU12_OFF, 0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI);

	//SAROUT measurement
	Maxim::I2C::SetSlaveAddress(TM_3DV_AD);

	Maxim::I2C::Write(0x02, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x02, 0x41); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x02, 0x34); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x02, 0x20); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x02, 0x10); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x02, 0xA0); //write DHR<15:0> to DCTLR<15:0>

	Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x01, 0x20); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x01, 0x1D); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>


	Maxim::I2C::Write(0x01, 0x90);
	Maxim::I2C::Write(0x01, 0x60);

	Maxim::I2C::Write(0x02, 0x74);
	
	cbitclose(K18);
	
	apu12set(FSEN, APU12_FV, 0.9, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI);
	apu12set(COMP, APU12_FI, 0, APU12_3p6V, APU12_10UA, APU12_PIN_TO_VI);
    lwait(2 ms_f);

	Maxim::I2C::SetSlaveAddress(AP89_AD);

	Maxim::I2C::Write(0x20, 0x01); //tm_adc
	Maxim::I2C::Write(0x15, 0x01); //adc_en, adc_sel<0>

	
	int iSAR_Code;


	//SAR 0
	iSAR_Code = 0;
	Maxim::I2C::Write(0x14, iSAR_Code);
	lwait(1 ms_f);
	pldResults = apu12mv(COMP, 10);
	Maxim::DatalogAll(DSIndex, pldResults * mV);

	//SAR1
	Maxim::I2C::Write(0x14, iSAR_Code+1);
	lwait(1 ms_f);
	pldResults = apu12mv(COMP, 10);
	Maxim::DatalogAll(DSIndex, pldResults * mV);

	//SAR2
	Maxim::I2C::Write(0x14, iSAR_Code+2);
	lwait(1 ms_f);
	pldResults = apu12mv(COMP, 10);
	Maxim::DatalogAll(DSIndex, pldResults * mV);
	
	
	iSAR_Code = 1;


	//Major code
	for(int i =0; i<6; i++){
		
		iSAR_Code = (iSAR_Code * 2) + 1;

		Maxim::I2C::Write(0x14, iSAR_Code);
		lwait(1 ms_f);
		pldResults = apu12mv(COMP, 10);
		Maxim::DatalogAll(DSIndex, pldResults * mV);


		Maxim::I2C::Write(0x14, iSAR_Code+1);
		lwait(1 ms_f);
		pldResults = apu12mv(COMP, 10);
		Maxim::DatalogAll(DSIndex, pldResults * mV);

	}

	
	//	<000> FSEN/ISET
	//	<001> IOUT1
	//	<002> IOUT2
	//	<003> IOUT3
	//	<004> IOUT4
	//	<005> IOUT5
	//	<006> IOUT6
	//	<007> BSTMON

	//Maxim::I2C::Write(TM_3DV_AD, 0x02, 0x72);
	//cbitopen(18);
	
	dpinforce("DIM", "1", 3.0, 0.1, MS_ALL); //DIM at 5V

	Maxim::I2C::SetSlaveAddress(AP89_AD);
	Maxim::I2C::Write(0x02, 0x24);
	Maxim::I2C::Write(0x20, 0x03);
	Maxim::I2C::Write(0x15, 0x13); //adc_en, adc_sel<1>
	lwait(1 ms_f);

	apu12set(OUT1, APU12_FV, 2, APU12_10V, APU12_100MA, APU12_PIN_TO_VI);
	lwait(2 ms_f);

	Maxim::I2C::Write(0x21, 0xFF);


	//SAR 0
	iSAR_Code = 0;
	Maxim::I2C::Write(0x14, iSAR_Code);
	lwait(1 ms_f);
	pldResults = apu12mv(COMP, 10);
	Maxim::DatalogAll(DSIndex, pldResults * mV);

	//SAR1
	Maxim::I2C::Write(0x14, iSAR_Code+1);
	lwait(1 ms_f);
	pldResults = apu12mv(COMP, 10);
	Maxim::DatalogAll(DSIndex, pldResults * mV);

	//SAR2
	Maxim::I2C::Write(0x14, iSAR_Code+2);
	lwait(1 ms_f);
	pldResults = apu12mv(COMP, 10);
	Maxim::DatalogAll(DSIndex, pldResults * mV);
	
	
	iSAR_Code = 1;


	//Major code
	for(int i =0; i<6; i++){
		
		iSAR_Code = (iSAR_Code * 2) + 1;

		Maxim::I2C::Write(0x14, iSAR_Code);
		lwait(1 ms_f);
		pldResults = apu12mv(COMP, 10);
		Maxim::DatalogAll(DSIndex, pldResults * mV);


		Maxim::I2C::Write(0x14, iSAR_Code+1);
		lwait(1 ms_f);
		pldResults = apu12mv(COMP, 10);
		Maxim::DatalogAll(DSIndex, pldResults * mV);

	}


	apu12set(OUT1, APU12_FV, 0.7, APU12_10V, APU12_1MA, APU12_PIN_TO_VI);
	apu12set(FSEN, APU12_FV, 0.0, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI);
	apu12set(COMP, APU12_OFF, 0, APU12_3p6V, APU12_10UA, APU12_PIN_TO_VI);

	cbitopen(K18);

    //End of function
    Maxim::DatalogFlush();	//Output the datalog queue.
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC
