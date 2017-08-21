/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:
*		SCAN.cpp
*
*	Written and maintained by:
*		Govit Pimthong (govit.pimthong@MaximIntegrated.com)
*
*	URL:                   $URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/SCAN.cpp $
*	Last Modified by:      $LastChangedBy: kiratiya.kaewpaluek $
*	Date:                  $Date: 2017-07-07 11:07:18 +0700 (Fri, 07 Jul 2017) $
*	Current Rev:           $Revision: 66609 $
*
*	Description:
*		File Description
*
*	Revision History
*
*
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
#include "AP89_FT.h"



// Test Function: SCAN
ETS_PRGFLOW_FUNC SCAN( int DSIndex, LPCTSTR TestLabel )
{
     
    //declare variables
    PinListData pldResults;	//declare a PLD variable for the measurement results.
	int iStatus;
	SiteInt siStatus;

	//Maxim::DatalogAll(DSIndex, pldResults * V);	//Datalog the measurements in Volts



	Maxim::I2C::SetSlaveAddress(TM_3DV_AD);

	Maxim::I2C::Write(0x02, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x02, 0x40); //write data to DHR3<15:12>=0000
	Maxim::I2C::Write(0x02, 0x30); //write data to DHR2<11:8>=1000
	Maxim::I2C::Write(0x02, 0x20); //write data to DHR1<7:4>=0000
	Maxim::I2C::Write(0x02, 0x18); //write data to DHR0<3:0>=1110
	Maxim::I2C::Write(0x02, 0xA0); //write DHR<15:0> to DCTLR<15:0>

	Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>=0000
	Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>=1000
	Maxim::I2C::Write(0x01, 0x20); //write data to DHR1<7:4>=0000
	Maxim::I2C::Write(0x01, 0x1A); //write data to DHR0<3:0>=1110
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>


	apu12set(FSEN, APU12_FV, 0, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI);
	apu12set(CS, APU12_OFF, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI);
	apu12set(BSTMON, APU12_FV, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI);
	apu12set(ALL_OUT, APU12_OFF, 0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI);
	dpinviset("SDA", DPIN_OFF, 4, DPIN_8V, DPIN_512UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL);
	dpinforce("SCAN_OUT", "-", 5.0, 0.0, MS_ALL);

	cbitclose(K4, K40, K41, K42, K43, K44);
	mcutimeout(5000000);



	mcuset(MCU_MAIN, 60.0);//(valid range=25-66MHz)
	dpinperiod(20);	//3MHz

	iStatus = 0;

	iStatus |= mcurun( NULL, "ScanChainInt");

	mcustop();



	__int64 i64FailPins;

	siStatus = SiteInt(1); //initialization

	for(int isite_no=0; isite_no < NUM_SITES; isite_no++)
	{
		if(msSiteStat(isite_no))
		{
			siStatus[isite_no] = dpingetfailpins("SCAN_OUT", DPIN_ALL_PINS, &i64FailPins, isite_no );
		}
	}


	//datalog
	Maxim::DatalogAll(DSIndex, siStatus);  //PF: 0 passed







	iStatus |= mcurun( NULL, "ScanStuckAtFault");

	mcustop();

	siStatus = SiteInt(1); //initialization

	for(int isite_no=0; isite_no < NUM_SITES; isite_no++)
	{
		if(msSiteStat(isite_no))
		{
			siStatus[isite_no] = dpingetfailpins("SCAN_OUT", DPIN_ALL_PINS, &i64FailPins, isite_no );
		}
	}








	//datalog
	Maxim::DatalogAll(DSIndex, siStatus);  //PF: 0 passed


	dpinforce("DIM", "0", 3.0, 0.1, MS_ALL); //DIM at 5V
	dpinforce("SCL_IO", "0", 3.0, 0.0, MS_ALL);
	dpinforce("SDA_IO", "0", 3.0, 0.0, MS_ALL);
	dpinforce("SCAN_ENABLE", "0", 3.0, 0.0, MS_ALL);



	mcuset(MCU_MAIN, 50.0);//50MHz.  (valid range=25-66MHz)
	dpinperiod(125);	//50Mhz/125=400KHz. Divide Maste

	cbitopen(K4, K20, K40, K41, K42, K43, K44);

	apu12setcapload( EN, APU12_FV, 0, APU12_10V, APU12_1MA, 0.5, APU12_PIN_TO_VI );
	lwait (2 ms_f);


	apu12setcapload( IN1, APU12_FV, 0, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );
	lwait (5 ms_f);

	//*************************************************************************************************




    //End of function
    Maxim::DatalogFlush();	//Output the datalog queue.
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC
