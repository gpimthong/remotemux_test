/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:
*		RT_Oscillator.cpp
*
*	Written and maintained by:
*		Govit Pimthong (govit.pimthong@MaximIntegrated.com)
*
*	URL:                   $URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/RT_Oscillator.cpp $
*	Last Modified by:      $LastChangedBy: govit.pimthong $
*	Date:                  $Date: 2017-07-31 01:38:54 +0700 (Mon, 31 Jul 2017) $
*	Current Rev:           $Revision: 67433 $
*
*	Description:
*		File Description
*
*	Revision History
*
*
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
#include "AP89_FT.h"

// Test Function: RT_Oscillator
ETS_PRGFLOW_FUNC RT_Oscillator( int DSIndex, LPCTSTR TestLabel )
{
       
    //declare variables
    PinListData pldResults, pldRT_i, pldRT_v, pldRT400, pldRT1300, pldRT2200;	//declare a PLD variable for the measurement results.

	dpinrcvset("SCAN_OUT", DPIN_RCV_SPLIT, 2.5, 1.4, MS_ALL );
	qtmumode(QTMU_GP, QTMU_TIMER, QTMU_ARMOUTOFF );
	qtmustart(QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_POS, 0, 500);
	qtmustop(QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_POS, 1, 500);

	//Setup
	Maxim::I2C::SetSlaveAddress(TM_3DV_AD);
	Maxim::I2C::Write(0x02, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x02, 0x40); //write data to DHR3<15:12>=0000
	Maxim::I2C::Write(0x02, 0x30); //write data to DHR2<11:8>=0000
	Maxim::I2C::Write(0x02, 0x20); //write data to DHR1<7:4>=0100
	Maxim::I2C::Write(0x02, 0x11); //write data to DHR0<3:0>=0000
	Maxim::I2C::Write(0x02, 0xA0); //write DHR<15:0> to DCTLR<15:0>

	Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>=0000
	Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>=0000
	Maxim::I2C::Write(0x01, 0x24); //write data to DHR1<7:4>=0000
	Maxim::I2C::Write(0x01, 0x10); //write data to DHR0<3:0>=0001
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	Maxim::I2C::Write(0x01, 0x70); //AI<0>(V_VCC_K)



	apu12set( ALL_OUT, APU12_FV, 0.7, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	apu12set( BSTMON, APU12_FV, 0.7, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
	Maxim::I2C::Write(AP89_AD, 0x02, 0x20);




	cbitclose(K43, K44);
	lwait (5 ms_f);

	pldRT400 = DUT::Trim::gtsOSC.Measure();

	Maxim::DatalogAll(DSIndex, pldRT400);	


	cbitclose(K33, K4);
	apu12set( CS, APU12_FI, 0, APU12_3p6V, APU12_10UA, APU12_PIN_TO_VI );
	lwait(5 ms_f);

	//1.3MHz

	pldRT_i = pldRT_v = apu12mv(CS, 100);

	ForEachActive_iSite{	
		pldRT_i[iSite] = (pldRT_v[iSite] / -23.069);
		apu12set( CS[iSite], APU12_FI, pldRT_i[iSite], APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );
	}
	cbitopen(K37);
	lwait(2 ms_f);

	pldRT1300 = DUT::Trim::gtsOSC.Measure();

	Maxim::DatalogAll(DSIndex, pldRT1300);




	//2.2MHz
	ForEachActive_iSite{	
		pldRT_i[iSite] = (pldRT_v[iSite] / -13.3);
		apu12set( CS[iSite], APU12_FI, pldRT_i[iSite], APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );
	}
	
	pldRT2200 = DUT::Trim::gtsOSC.Measure();

	Maxim::DatalogAll(DSIndex, pldRT2200);



	//Accuracy calculation
	pldResults = 100*(pldRT400 - 400)/400;
	Maxim::DatalogAll(DSIndex, pldResults);

	pldResults = 100*(pldRT1300 - 1300)/1300;
	Maxim::DatalogAll(DSIndex, pldResults);
	
	pldResults = 100*(pldRT2200 - 2200)/2200;
	Maxim::DatalogAll(DSIndex, pldResults);
	
	
	
	//RT Output
	Maxim::DatalogAll(DSIndex, pldRT_v);
	
	PinListData pld400MaxDuty, pld2200MaxDuty, pld400tOff, pld2200tOff;
	PinListData pld400MaxDutyP, pld2200MaxDutyP, pld400tOffP, pld2200tOffP;	
	PinListData pld400MaxDutyN, pld2200MaxDutyN, pld400tOffN, pld2200tOffN;	
	
	Maxim::I2C::SetSlaveAddress(TM_3DV_AD);
	Maxim::I2C::Write(0x02, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x02, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x02, 0x30); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x02, 0x20); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x02, 0x10); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x02, 0xA0); //write DHR<15:0> to DCTLR<15:0>

	Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x01, 0x27); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x01, 0x10); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	

	int num_cycles = 100;
	PinListData pldPeriod;
	qtmumode( QTMU_GP, QTMU_TIMER, QTMU_ARMOUTOFF);
	dpinrcvset( "SCAN_OUT", DPIN_RCV_SPLIT, 3, 2 );

	qtmustart( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_NEG, 0, 30);
	qtmustop( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_NEG, 1, 30);
	lwait( 1 ms_f );

	qtmuarm( QTMU_GP, "AUTO", num_cycles, QTMU_EDGE, QTMU_POS );
	lwait( 2 ms_f );

	pldPeriod = qtmumt( QTMU_GP, QTMU_READTIME, 5000);
	
	qtmustop( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_POS, 0, 30);
	qtmuarm( QTMU_GP, "AUTO", num_cycles, QTMU_EDGE, QTMU_POS );
	lwait( 2 ms_f );

	pld2200tOff = qtmumt( QTMU_GP, QTMU_READTIME, 5000);

	pld2200MaxDuty = pldPeriod - pld2200tOff;
	pld2200MaxDuty = 100 * pld2200MaxDuty / pldPeriod;




	//DT -
	Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x01, 0x28); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x01, 0x10); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	lwait(1 ms_f);

	qtmustop( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_NEG, 1, 30);
	lwait( 1 ms_f );

	qtmuarm( QTMU_GP, "AUTO", num_cycles, QTMU_EDGE, QTMU_POS );
	lwait( 2 ms_f );

	pldPeriod = qtmumt( QTMU_GP, QTMU_READTIME, 5000);


	qtmustop( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_POS, 0, 30);
	lwait( 1 ms_f );

	qtmuarm( QTMU_GP, "AUTO", num_cycles, QTMU_EDGE, QTMU_POS );
	lwait( 2 ms_f );

	pld2200tOffN = qtmumt( QTMU_GP, QTMU_READTIME, 5000);


	pld2200MaxDutyN = pldPeriod - pld2200tOffN;
	pld2200MaxDutyN = 100 * pld2200MaxDutyN / (pldPeriod);





	//DT +
	Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x01, 0x29); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x01, 0x10); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	lwait(1 ms_f);


	qtmustop( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_NEG, 1, 30);
	lwait( 1 ms_f );

	qtmuarm( QTMU_GP, "AUTO", num_cycles, QTMU_EDGE, QTMU_POS );
	lwait( 2 ms_f );

	pldPeriod = qtmumt( QTMU_GP, QTMU_READTIME, 5000);


	qtmustop( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_POS, 0, 30);
	lwait( 1 ms_f );

	qtmuarm( QTMU_GP, "AUTO", num_cycles, QTMU_EDGE, QTMU_POS );
	lwait( 2 ms_f );

	pld2200tOffP = qtmumt( QTMU_GP, QTMU_READTIME, 5000);


	pld2200MaxDutyP = pldPeriod - pld2200tOffP;
	pld2200MaxDutyP = 100 * pld2200MaxDutyP / (pldPeriod);



	cbitclose(K37);
	apu12set( CS, APU12_FI, 0, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );
	lwait(1 ms_f);
	cbitopen(K4);
	apu12set( CS, APU12_OFF, 0, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );



	//400KHz
	Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x01, 0x27); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x01, 0x10); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	
	qtmustop( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_POS, 0, 100);
	lwait( 1 ms_f );

	qtmuarm( QTMU_GP, "AUTO", num_cycles, QTMU_EDGE, QTMU_POS );
	lwait( 2 ms_f );

	pld400tOff = qtmumt( QTMU_GP, QTMU_READTIME, 5000);

	pld400MaxDuty = (1e6 / pldRT400) - pld400tOff;
	pld400MaxDuty = 100 * pld400MaxDuty / (1e6 / pldRT400);




	//DT -
	Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x01, 0x28); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x01, 0x10); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	lwait(1 ms_f);

	qtmustop( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_NEG, 1, 100);
	lwait( 1 ms_f );

	qtmuarm( QTMU_GP, "AUTO", num_cycles, QTMU_EDGE, QTMU_POS );
	lwait( 2 ms_f );

	pldPeriod = qtmumt( QTMU_GP, QTMU_READTIME, 5000);


	qtmustop( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_POS, 0, 100);
	lwait( 1 ms_f );

	qtmuarm( QTMU_GP, "AUTO", num_cycles, QTMU_EDGE, QTMU_POS );
	lwait( 2 ms_f );

	pld400tOffN = qtmumt( QTMU_GP, QTMU_READTIME, 5000);


	pld400MaxDutyN = pldPeriod - pld400tOffN;
	pld400MaxDutyN = 100 * pld400MaxDutyN / (pldPeriod);





	//DT +
	Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>
	Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x01, 0x29); //write data to DHR1<7:4>
	Maxim::I2C::Write(0x01, 0x10); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	lwait(1 ms_f);


	qtmustop( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_NEG, 1, 100);
	lwait( 1 ms_f );

	qtmuarm( QTMU_GP, "AUTO", num_cycles, QTMU_EDGE, QTMU_POS );
	lwait( 2 ms_f );

	pldPeriod = qtmumt( QTMU_GP, QTMU_READTIME, 5000);


	qtmustop( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_POS, 0, 100);
	lwait( 1 ms_f );

	qtmuarm( QTMU_GP, "AUTO", num_cycles, QTMU_EDGE, QTMU_POS );
	lwait( 2 ms_f );

	pld400tOffP = qtmumt( QTMU_GP, QTMU_READTIME, 5000);


	pld400MaxDutyP = pldPeriod - pld400tOffP;
	pld400MaxDutyP = 100 * pld400MaxDutyP / (pldPeriod);



	Maxim::DatalogAll(DSIndex, pld400tOff);
	Maxim::DatalogAll(DSIndex, pld400MaxDuty);
	Maxim::DatalogAll(DSIndex, pld2200tOff);
	Maxim::DatalogAll(DSIndex, pld2200MaxDuty);

	Maxim::DatalogAll(DSIndex, pld400tOffP);
	Maxim::DatalogAll(DSIndex, pld400MaxDutyP);
	Maxim::DatalogAll(DSIndex, pld2200tOffP);
	Maxim::DatalogAll(DSIndex, pld2200MaxDutyP);

	Maxim::DatalogAll(DSIndex, pld400tOffN);
	Maxim::DatalogAll(DSIndex, pld400MaxDutyN);
	Maxim::DatalogAll(DSIndex, pld2200tOffN);
	Maxim::DatalogAll(DSIndex, pld2200MaxDutyN);

	dpinrcvset( "SCAN_OUT", DPIN_OFF, 3, 2 , MS_ALL );
	qtmustart( QTMU_GP, "OFF", QTMU_RCVH, QTMU_NEG, 0, 500);
	qtmustop( QTMU_GP, "OFF", QTMU_RCVH, QTMU_POS, 0, 500);


	cbitopen(K33, K4, K36, K43, K44);
	apu12set( CS, APU12_OFF, 0, APU12_3p6V, APU12_10UA, APU12_PIN_TO_VI );
     
    //End of function
    Maxim::DatalogFlush();	//Output the datalog queue.
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC
