/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:
*		Freq_Dither.cpp
*
*	Written and maintained by:
*		Govit Pimthong (govit.pimthong@MaximIntegrated.com)
*
*	URL:                   $URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/Freq_Dither.cpp $
*	Last Modified by:      $LastChangedBy: govit.pimthong $
*	Date:                  $Date: 2017-06-07 19:40:55 +0700 (Wed, 07 Jun 2017) $
*	Current Rev:           $Revision: 65418 $
*
*	Description:
*		File Description
*
*	Revision History
*
*
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
#include "AP89_FT.h"


// Test Function: Freq_Dither
ETS_PRGFLOW_FUNC Freq_Dither( int DSIndex, LPCTSTR TestLabel )
{
     
    //declare variables
    PinListData pldResults, pldRT_i;	//declare a PLD variable for the measurement results.
    int num_cycles = 100;
	SiteDouble sdFreq400, sdFreq2200, sdRTv;

	PinListData Dlo_SS0_400, Dlo_SS0_400p, DHi_SS0_400, DHi_SS0_400p, Dlo_SS0_2200, Dlo_SS0_2200p, DHi_SS0_2200, DHi_SS0_2200p;
	PinListData Dlo_SS1_400, Dlo_SS1_400p, DHi_SS1_400, DHi_SS1_400p, Dlo_SS1_2200, Dlo_SS1_2200p, DHi_SS1_2200, DHi_SS1_2200p;

	sdFreq400 = Maxim::Results::GetResults(36000);
	sdFreq2200 = Maxim::Results::GetResults(36060);
	sdRTv = Maxim::Results::GetResults(36140);

	//Dither-400K-SSL0-Lo
	cbitclose(K43, K44);
	dpinrcvset( "SCAN_OUT", DPIN_RCV_SPLIT, 3, 2 , MS_ALL );
	Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x01, 0x25); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x01, 0x10); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	lwait(1 ms_f);

	qtmustart( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_NEG, 0, 200);
	qtmustop( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_NEG, 1, 200);
	lwait( 1 ms_f );

	qtmuarm( QTMU_GP, "AUTO", num_cycles, QTMU_EDGE, QTMU_POS );
	lwait( 2 ms_f );

	Dlo_SS0_400 = 8 * qtmumt( QTMU_GP, QTMU_READFREQ, 5000);
	Dlo_SS0_400p = 100 * (Dlo_SS0_400 - sdFreq400) / sdFreq400;
	
	
	
	//Dither-400K-SSL0-Hi
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x01, 0x26); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x01, 0x10); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	lwait(1 ms_f);

	qtmuarm( QTMU_GP, "AUTO", num_cycles, QTMU_EDGE, QTMU_POS );
	lwait( 2 ms_f );

	DHi_SS0_400 = 8 * qtmumt( QTMU_GP, QTMU_READFREQ, 5000);
	DHi_SS0_400p = 100 * (DHi_SS0_400 - sdFreq400) / sdFreq400;



	//SSL to 1
	Maxim::I2C::Write(AP89_AD, 0x12, 0x14);


	//Dither-400K-Hi
	qtmuarm( QTMU_GP, "AUTO", num_cycles, QTMU_EDGE, QTMU_POS );
	lwait( 2 ms_f );

	DHi_SS1_400 = 8 * qtmumt( QTMU_GP, QTMU_READFREQ, 5000);
	DHi_SS1_400p = 100 * (DHi_SS1_400 - sdFreq400) / sdFreq400;



	//Dither-400K-Lo
	Maxim::I2C::SetSlaveAddress(TM_3DV_AD);
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>=0000
	Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>=0000
	Maxim::I2C::Write(0x01, 0x25); //write data to DHR1<7:4>=0000
	Maxim::I2C::Write(0x01, 0x10); //write data to DHR0<3:0>=0001
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	lwait( 1 ms_f );

	qtmuarm( QTMU_GP, "AUTO", num_cycles, QTMU_EDGE, QTMU_POS );
	lwait( 2 ms_f );

	Dlo_SS1_400 = 8 * qtmumt( QTMU_GP, QTMU_READFREQ, 5000);
	Dlo_SS1_400p = 100 * (Dlo_SS1_400 - sdFreq400) / sdFreq400;





	//Switch to 2.2MHz

	cbitclose(K4);

	ForEachActive_iSite{	
		pldRT_i[iSite] = (sdRTv[iSite] / -13.3);
		apu12set( CS[iSite], APU12_FI, pldRT_i[iSite], APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );
	}

	cbitopen(K37);
	lwait(3 ms_f);


	//Dither-2p2M-Lo
	qtmuarm( QTMU_GP, "AUTO", num_cycles, QTMU_EDGE, QTMU_POS );
	lwait( 2 ms_f );

	Dlo_SS1_2200 = 8 * qtmumt( QTMU_GP, QTMU_READFREQ, 5000);
	Dlo_SS1_2200p = 100 * (Dlo_SS1_2200 - sdFreq2200) / sdFreq2200;




	//Dither-2p2M-Hi
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x01, 0x26); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x01, 0x10); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	lwait(1 ms_f);

	qtmuarm( QTMU_GP, "AUTO", num_cycles, QTMU_EDGE, QTMU_POS );
	lwait( 2 ms_f );

	DHi_SS1_2200 = 8 * qtmumt( QTMU_GP, QTMU_READFREQ, 5000);
	DHi_SS1_2200p = 100 * (DHi_SS1_2200 - sdFreq2200) / sdFreq2200;



	//SSL to 0
	Maxim::I2C::Write(AP89_AD, 0x12, 0x10);
	lwait(1 ms_f);


	//Dither-2p2M-SSL0-Hi
	qtmuarm( QTMU_GP, "AUTO", num_cycles, QTMU_EDGE, QTMU_POS );
	lwait( 2 ms_f );

	DHi_SS0_2200 = 8 * qtmumt( QTMU_GP, QTMU_READFREQ, 5000);
	DHi_SS0_2200p = 100 * (DHi_SS0_2200 - sdFreq2200) / sdFreq2200;



	//Dither-2p2M-SSL0-Lo
	Maxim::I2C::SetSlaveAddress(TM_3DV_AD);
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>=0000
	Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>=0000
	Maxim::I2C::Write(0x01, 0x25); //write data to DHR1<7:4>=0000
	Maxim::I2C::Write(0x01, 0x10); //write data to DHR0<3:0>=0001
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	lwait( 1 ms_f );

	qtmuarm( QTMU_GP, "AUTO", num_cycles, QTMU_EDGE, QTMU_POS );
	lwait( 2 ms_f );

	Dlo_SS0_2200 = 8 * qtmumt( QTMU_GP, QTMU_READFREQ, 5000);
	Dlo_SS0_2200p = 100 * (Dlo_SS0_2200 - sdFreq2200) / sdFreq2200;



	cbitclose(K37);
	apu12set( CS, APU12_FI, 0, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );
	cbitopen(K4);


	Maxim::DatalogAll(DSIndex, Dlo_SS0_400);
	Maxim::DatalogAll(DSIndex, Dlo_SS0_400p);
	Maxim::DatalogAll(DSIndex, DHi_SS0_400);
	Maxim::DatalogAll(DSIndex, DHi_SS0_400p);
	Maxim::DatalogAll(DSIndex, Dlo_SS0_2200);
	Maxim::DatalogAll(DSIndex, Dlo_SS0_2200p);
	Maxim::DatalogAll(DSIndex, DHi_SS0_2200);
	Maxim::DatalogAll(DSIndex, DHi_SS0_2200p);

	Maxim::DatalogAll(DSIndex, Dlo_SS1_400);
	Maxim::DatalogAll(DSIndex, Dlo_SS1_400p);
	Maxim::DatalogAll(DSIndex, DHi_SS1_400);
	Maxim::DatalogAll(DSIndex, DHi_SS1_400p);
	Maxim::DatalogAll(DSIndex, Dlo_SS1_2200);
	Maxim::DatalogAll(DSIndex, Dlo_SS1_2200p);
	Maxim::DatalogAll(DSIndex, DHi_SS1_2200);
	Maxim::DatalogAll(DSIndex, DHi_SS1_2200p);
     
    //End of function
    Maxim::DatalogFlush();	//Output the datalog queue.
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC
