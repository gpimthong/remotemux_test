/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:
*		MOSFET_Driver.cpp
*
*	Written and maintained by:
*		Govit Pimthong (govit.pimthong@MaximIntegrated.com)
*
*	URL:                   $URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/MOSFET_Driver.cpp $
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


// Test Function: MOSFET_Driver
ETS_PRGFLOW_FUNC MOSFET_Driver( int DSIndex, LPCTSTR TestLabel )
{
     
  
    //declare variables
    PinListData pldResults, pldFET_v, pldFET_i;	//declare a PLD variable for the measurement results.
	ACCUMULATOR_STR FET_i[NUM_SITES];
	//ACCUMULATOR_STR VCC_v[NUM_SITES];

	SiteDouble sdFET_i;

	sdFET_i = 0;

	//Setup

	Maxim::I2C::Write(0x02, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x02, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x02, 0x30); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x02, 0x20); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x02, 0x10); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x02, 0xA0); //write DHR<15:0> to DCTLR<15:0>

	Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x01, 0x2E); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x01, 0x1C); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>

	Maxim::I2C::Write(0x01, 0x84); //AI<6>(V_NDRVKV)

    cbitclose(K11);
	apu12set( COMP, APU12_FI, 0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );
	apu12set( CS, APU12_FI, 0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );
	lwait (3 ms_f);


	apu12set( NDRV, APU12_FV, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	pldResults = apu12mv(COMP, 1);

	ForEachActive_iSite{

		if(pldResults[iSite] > 4)		
			FET_i[iSite].value1 = -30;
		else
			FET_i[iSite].value1 = 0;
	}


	groupsetaccumulator(FET_i, NUM_SITES);
	apu12set( NDRV, APU12_FI, USE_ACCUM, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	lwait (5 ms_f);

	pldResults = apu12mv(COMP, 1); //NDRV
	pldFET_v = apu12mv(CS, 10); //VCC
	pldFET_i = apu12mi(NDRV, APU12_MI_1X, 1);

	pldFET_v = pldResults - pldFET_v;

	pldResults = (pldFET_v) / (pldFET_i + 1e-12);

	
	apu12set( NDRV, APU12_FI, 0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	apu12set( NDRV, APU12_FI, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );

	Maxim::DatalogAll(DSIndex, -1 * pldFET_v * mV);
	Maxim::DatalogAll(DSIndex, pldFET_i);
	Maxim::DatalogAll(DSIndex, pldResults * mV);



	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x01, 0x2F); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x01, 0x1C); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>

	apu12set( NDRV, APU12_FV, 1, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	pldResults = apu12mv(COMP, 1);

	ForEachActive_iSite{

		if(pldResults[iSite] < 0.8)		
			FET_i[iSite].value1 = 30;
		else
			FET_i[iSite].value1 = 0;
	}


	groupsetaccumulator(FET_i, NUM_SITES);
	apu12set( NDRV, APU12_FI, USE_ACCUM, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	lwait (5 ms_f);

	pldResults = apu12mv(COMP, 1); //NDRV
	pldFET_v = apu12mv(CS, 10); //PGND
	pldFET_i = apu12mi(NDRV, APU12_MI_1X, 1);

	pldFET_v = pldResults - pldFET_v;

	pldResults = (pldFET_v) / (pldFET_i + 1e-12);

	
	apu12set( NDRV, APU12_FI, 0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	apu12set( NDRV, APU12_FI, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );

	Maxim::DatalogAll(DSIndex, pldFET_v * mV);
	Maxim::DatalogAll(DSIndex, pldFET_i);
	Maxim::DatalogAll(DSIndex, pldResults * mV);
	



	apu12set( CS, APU12_OFF, 0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );
	lwait (3 ms_f);

	apu12set( NDRV, APU12_FV, 0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );
	cbitopen(K11);

	Maxim::I2C::Write(0x01, 0x80);
     
    //End of function
    Maxim::DatalogFlush();	//Output the datalog queue.
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC
