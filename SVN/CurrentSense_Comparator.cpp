/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:
*		CurrentSense_Comp.cpp
*
*	Written and maintained by:
*		Govit Pimthong (govit.pimthong@MaximIntegrated.com)
*
*	URL:                   $URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/CurrentSense_Comparator.cpp $
*	Last Modified by:      $LastChangedBy: govit.pimthong $
*	Date:                  $Date: 2017-05-31 18:12:23 +0700 (Wed, 31 May 2017) $
*	Current Rev:           $Revision: 65136 $
*
*	Description:
*		File Description
*
*	Revision History
*
*
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
#include "AP89_FT.h"

// Test Function: CurrentSense_Comp
ETS_PRGFLOW_FUNC CurrentSense_Comp( int DSIndex, LPCTSTR TestLabel )
{
     
    //declare variables
    PinListData pldResults;	//declare a PLD variable for the measurement results.
	int iMCU_Div = 10000;

	Maxim::I2C::Write(0x02, 0xB0); //Reset BIPTM Machine


	Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>=0000
	Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>=1000
	Maxim::I2C::Write(0x01, 0x2C); //write data to DHR1<7:4>=0000
	Maxim::I2C::Write(0x01, 0x10); //write data to DHR0<3:0>=1110
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>

	cbitclose(K11);
	apu12set( CS, APU12_FV, 0.2, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );
	apu12set( OUT3, APU12_FV, 0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );
	lwait( 3 ms_f );

	pldResults = APU_AWGThreshold( CS, "CS_R", APU12_3p6V, APU12_1MA, 0.35, 0.45, 0.001, iMCU_Div, OUT3, 1.5, APU12_ABOVE_THRESH, 1 ms_f, APU12_FV, APU12_MV);

	Maxim::DatalogAll(DSIndex, pldResults * mV);

	apu12set( CS, APU12_FV, 0, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );
	cbitopen(K11);
	apu12set( CS, APU12_OFF, 0, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );



    //End of function
    Maxim::DatalogFlush();	//Output the datalog queue.
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC
