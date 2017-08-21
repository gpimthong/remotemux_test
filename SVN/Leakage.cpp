/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:
*		Leakage.cpp
*
*	Written and maintained by:
*		Govit Pimthong (govit.pimthong@MaximIntegrated.com)
*
*	URL:                   $URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/Leakage.cpp $
*	Last Modified by:      $LastChangedBy: govit.pimthong $
*	Date:                  $Date: 2017-08-04 03:15:03 +0700 (Fri, 04 Aug 2017) $
*	Current Rev:           $Revision: 67637 $
*
*	Description:
*		File Description
*
*	Revision History
*
*
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
#include "AP89_FT.h"

// Test Function: Leakage
ETS_PRGFLOW_FUNC Leakage( int DSIndex, LPCTSTR TestLabel )
{
     

	PinListData pldResults, pldI_PGATE_L, pldI_BSTMON_L;

	//Turn off current sink
	dpinforce("DIM", "0", 3.0, 0.1, MS_ALL);
	lwait(5 ms_f);

	//Move All OUT to OUT1 via BUSA
	apu12set( ALL_OUT, APU12_OFF, 0, APU12_10V, APU12_200MA, APU12_PIN_TO_VI );

	cbitclose(K52, K54, K56, K58, K60, K62);
	lwait (1 ms_f);

	//Now OUT1 APU connect to all of them.
	apu12set( OUT1, APU12_FV, 0, APU12_10V, APU12_200MA, APU12_PIN_TO_VI );
	apu12set( OUT1, APU12_FV, 0.7, APU12_10V, APU12_200MA, APU12_PIN_TO_VI );
	apu12set( PGATE, APU12_FV, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );


	apu12set( FSEN, APU12_FV, 0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );
	apu12set( CS, APU12_FV, 0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );



	//APU to PGATE
	cbitclose(K6, K14, K11);
	cbitopen(K27, K37, K34, K43, K44);






	//Setup
	Maxim::I2C::SetSlaveAddress(TM_3DV_AD);
	Maxim::I2C::Write(0x02, 0xB0); //Reset BIPTM Machine

	Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>=0000
	Maxim::I2C::Write(0x01, 0x31); //write data to DHR2<11:8>=0001
	Maxim::I2C::Write(0x01, 0x20); //write data to DHR1<7:4>=0000
	Maxim::I2C::Write(0x01, 0x10); //write data to DHR0<3:0>=0000
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	lwait (1 ms_f);

	apu12set( NDRV, APU12_FV, 0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );
	apu12set( NDRV, APU12_FV, 5, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	//I_NDRV_H
	pldResults = apu12mi(NDRV, APU12_MI_10X, 10);
 	Maxim::DatalogAll(DSIndex, pldResults * uA);


	apu12set( NDRV, APU12_FV, 0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	//I_NDRV_L
	pldResults = apu12mi(NDRV, APU12_MI_10X, 10);
 	Maxim::DatalogAll(DSIndex, pldResults * uA);



	Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>=0000
	Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>=0000
	Maxim::I2C::Write(0x01, 0x20); //write data to DHR1<7:4>=0000
	Maxim::I2C::Write(0x01, 0x14); //write data to DHR0<3:0>=0100
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>


	lwait (1 ms_f);
	
	apu12set( COMP, APU12_FV, 5, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	//I_COMP_H
	pldResults = apu12mi(COMP, APU12_MI_10X, 10);
 	Maxim::DatalogAll(DSIndex, pldResults * uA);


	apu12set( COMP, APU12_FV, 0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	//I_COMP_L
	pldResults = apu12mi(COMP, APU12_MI_10X, 10);
 	Maxim::DatalogAll(DSIndex, pldResults * uA);

	apu12set( NDRV, APU12_OFF, 0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );
	apu12set( COMP, APU12_OFF, 0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );

	cbitopen(K11);

	Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
	
	//DIM at 0V
	dpinforce("DIM", "0", 3.0, 0.1, MS_ALL);



	/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
	* Test : I_EN_L, I_EN_5V, I_EN_H
	**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
	PinListData pldI_EN_L, pldI_EN_5V, pldI_EN_H;

	apu12setcapload( EN, APU12_FV, 5, APU12_10V, APU12_1MA, 0.5, APU12_PIN_TO_VI );
	apu12setcapload( BSTMON, APU12_FV, 0, APU12_10V, APU12_1MA, 0.5, APU12_PIN_TO_VI );

	

	dpinforce("SCL_IO", "0", 3.0, 0.0, MS_ALL);	

	lwait (3 ms_f);
	apu12set( FLT, APU12_FV, 0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );


	//I_EN_5V
	pldResults = apu12mi(EN, APU12_MI_10X, 10);
 	Maxim::DatalogAll(DSIndex, pldResults * uA);



	//Disconnect Digital Driver from SDA
	dpindisconnect(MS_ALL, "SDA_IO");
	lwait (1 ms_f);

	//Connect PPMU
	dpinviset( "SDA_IO", DPIN_FV, 0.0, DPIN_10V, DPIN_8UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL );
	lwait (500 us_f);
	dpinviset( "SDA_IO", DPIN_FV, 5.0, DPIN_10V, DPIN_8UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL );
	lwait (2 ms_f);

	//I_SDA_H
	pldResults = dpinmi( "SDA_IO", 10, 13.0, MS_ALL );
	Maxim::DatalogAll(DSIndex, pldResults * uA);



	//I_SCL_L
	cbitopen(K41);
	lwait (1 ms_f);
	cbitclose(K13);

	apu12set( SCL, APU12_FV, 0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	pldResults = apu12mi(SCL, APU12_MI_1X, 10);	
	Maxim::DatalogAll(DSIndex, pldResults * uA);




	//I_SDA_L
	dpinviset( "SDA_IO", DPIN_FV, 0.0, DPIN_10V, DPIN_8UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL );
	apu12setcapload( SCL, APU12_FV, 5, APU12_10V, APU12_100UA, 0.5, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	pldResults = dpinmi( "SDA_IO", 10, 13.0, MS_ALL );
	Maxim::DatalogAll(DSIndex, pldResults * uA);



	//I_SCL_H
	pldResults = apu12mi(SCL, APU12_MI_1X, 10);	
	Maxim::DatalogAll(DSIndex, pldResults * uA);


	apu12setcapload( SCL, APU12_FV, 0, APU12_10V, APU12_100UA, 0.5, APU12_PIN_TO_VI );
	apu12set( SCL, APU12_OFF, 0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );


	//SCL at 0V by digital driver
	cbitopen(K13);
	cbitclose(K41);

	
	

	apu12setcapload( EN, APU12_FV, 0, APU12_10V, APU12_1MA, 0.5, APU12_PIN_TO_VI );
	apu12set( FLT, APU12_FV, 0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );
	lwait (2 ms_f);


	//I_EN_L
	pldResults = apu12mi(EN, APU12_MI_10X, 10);
 	Maxim::DatalogAll(DSIndex, pldResults * uA);


	//I_BSTMON_L
	pldResults = apu12mi(BSTMON, APU12_MI_10X, 10);
	Maxim::DatalogAll(DSIndex, pldResults * nA);



	apu12setcapload( BSTMON, APU12_FV, 1.3, APU12_10V, APU12_1MA, 0.5, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	//I_BSTMON_1p3
	pldResults = apu12mi(BSTMON, APU12_MI_10X, 10);
	Maxim::DatalogAll(DSIndex, pldResults * nA);



	//I_PGATE_L
 	pldResults = apu12mi(PGATE, APU12_MI_10X, 10);
	Maxim::DatalogAll(DSIndex, pldResults * uA);


	apu12setcapload( PGATE, APU12_FV, 12, APU12_30V, APU12_1MA, 0.5, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	//I_PGATE_12V
 	pldResults = apu12mi(PGATE, APU12_MI_10X, 10);
	Maxim::DatalogAll(DSIndex, pldResults * uA);


	//I_FLT_L
	pldResults = apu12mi(FLT, APU12_MI_1X, 10);	
	Maxim::DatalogAll(DSIndex, pldResults * uA);


	//I_FLT_H
	apu12setcapload( FLT, APU12_FV, 6, APU12_10V, APU12_100UA, 0.5, APU12_PIN_TO_VI );
	pldResults = apu12mi(FLT, APU12_MI_1X, 10);	
	Maxim::DatalogAll(DSIndex, pldResults * uA);

	apu12setcapload( FLT, APU12_FV, 0, APU12_10V, APU12_100UA, 0.5, APU12_PIN_TO_VI );



	cbitopen(K6);
	lwait(3 ms_f);

	apu12setcapload( VCC, APU12_FV, 0.4, APU12_10V, APU12_1MA, 0.5, APU12_PIN_TO_VI );
	lwait (1 ms_f);


	//I_CS_L
	pldResults = apu12mi(CS, APU12_MI_1X, 10);	
	Maxim::DatalogAll(DSIndex, pldResults * uA);



	//I_CS_0p3V
	apu12set( CS, APU12_FV, 0.3, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	pldResults = apu12mi(CS, APU12_MI_1X, 10);	
	Maxim::DatalogAll(DSIndex, pldResults * uA);

	apu12setcapload( VCC, APU12_FV, 5.1, APU12_10V, APU12_1MA, 0.5, APU12_PIN_TO_VI );
	lwait (1 ms_f);

	//I_CS_H
	//apu12set( CS, APU12_FV, 5, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );
	//lwait (2 ms_f);

	//pldResults = apu12mi(CS, APU12_MI_1X, 10);	
	//Maxim::DatalogAll(DSIndex, pldResults * uA);
	DSIndex++;

	apu12setcapload( CS, APU12_FV, 0, APU12_10V, APU12_100UA, 0.3, APU12_PIN_TO_VI );

	cbitclose(K4);



	//I_FSEN_L
	pldResults = apu12mi(FSEN, APU12_MI_1X, 10);	
	Maxim::DatalogAll(DSIndex, pldResults * uA);


	//I_FSEN_1V
	apu12set( FSEN, APU12_FV, 1.0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	pldResults = apu12mi(FSEN, APU12_MI_1X, 10);	
	Maxim::DatalogAll(DSIndex, pldResults * uA);


	//I_FSEN_5V
	apu12setcapload( FSEN, APU12_FV, 5.0, APU12_10V, APU12_100UA, 0.5, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	pldResults = apu12mi(FSEN, APU12_MI_1X, 10);	
	Maxim::DatalogAll(DSIndex, pldResults * uA);


	apu12setcapload( FSEN, APU12_FV, 0, APU12_10V, APU12_100UA, 0.5, APU12_PIN_TO_VI );
	lwait (1 ms_f);
	
	cbitclose(K15);
	lwait (2 ms_f);


	//I_RT_L
	pldResults = apu12mi(RT, APU12_MI_1X, 10);	
	Maxim::DatalogAll(DSIndex, pldResults * uA);

	//I_IREF_L
	pldResults = apu12mi(IREF, APU12_MI_1X, 10);	
	Maxim::DatalogAll(DSIndex, pldResults * uA);

	apu12setcapload( RT, APU12_FV, 5.0, APU12_10V, APU12_100UA, 0.5, APU12_PIN_TO_VI );
	apu12setcapload( IREF, APU12_FV, 5.0, APU12_10V, APU12_100UA, 0.5, APU12_PIN_TO_VI );
	lwait (10 ms_f);



	//I_RT_H
	pldResults = apu12mi(RT, APU12_MI_1X, 10);	
	Maxim::DatalogAll(DSIndex, pldResults * uA);


	//I_IREF_H
	pldResults = apu12mi(IREF, APU12_MI_1X, 10);	
	Maxim::DatalogAll(DSIndex, pldResults * uA);

	apu12setcapload( RT, APU12_FV, 0.0, APU12_10V, APU12_100UA, 0.5, APU12_PIN_TO_VI );
	apu12setcapload( IREF, APU12_FV, 0.0, APU12_10V, APU12_100UA, 0.5, APU12_PIN_TO_VI );



	//DIM_PU_I
	cbitopen(K40);
	cbitclose(K7);
	apu12set( EN, APU12_FV, 5.0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );

	lwait (3 ms_f);
	apu12setcapload( DIM, APU12_FV, 0.0, APU12_10V, APU12_100UA, 0.5, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	pldResults = -1 * apu12mi(DIM, APU12_MI_1X, 10);	
	Maxim::DatalogAll(DSIndex, pldResults * uA);

	//I_DIM_H
	apu12setcapload( DIM, APU12_FV, 5.0, APU12_10V, APU12_100UA, 0.5, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	pldResults = apu12mi(DIM, APU12_MI_10X, 10);	
	Maxim::DatalogAll(DSIndex, pldResults * uA);
	
	apu12setcapload( EN, APU12_FV, 0.0, APU12_10V, APU12_1MA, 0.5, APU12_PIN_TO_VI );
	apu12setcapload( DIM, APU12_FV, 0.0, APU12_10V, APU12_100UA, 0.5, APU12_PIN_TO_VI );
	lwait (1 ms_f);
	apu12set( DIM, APU12_OFF, 0.0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );

	cbitopen(K7);

	apu12setcapload( PGATE, APU12_FV, 0, APU12_30V, APU12_1MA, 0.5, APU12_PIN_TO_VI );
	apu12setcapload( BSTMON, APU12_FV, 0, APU12_30V, APU12_1MA, 0.5, APU12_PIN_TO_VI );

	dpinviset( "SDA_IO", DPIN_OFF, 0.0, DPIN_10V, DPIN_8UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL );
	cbitopen(K4, K14, K41, K52, K54, K56, K58, K60, K62);
	

    Maxim::DatalogFlush();	//Output the datalog queue.
	

	DSIndex += 4; //AP79 Leakage, to be added later. Govit, May-23-2017


	
	//Prepare for stack-up
	cbitopen(K11, K12, K13, K14, K15);

	apu12set( CS, APU12_OFF, 0.0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );
	apu12set( VCC, APU12_OFF, 0., APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);

	apu12set( PEDESTAL1, APU12_FV, 0, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
	apu12set( PEDESTAL2, APU12_FV, 0, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
	apu12setcapload( IN1, APU12_FV, 0, APU12_30V, APU12_100MA, 0.5, APU12_PIN_TO_VI );

	apu12set( OUT1, APU12_FV, 0, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
	apu12set( OUT2, APU12_FV, 0, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
	apu12set( OUT3, APU12_FV, 0, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
	apu12set( OUT4, APU12_FV, 0, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
	apu12set( OUT5, APU12_FV, 0, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
	apu12set( OUT6, APU12_FV, 0, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
	lwait (10 ms_f);


	apu12set( ALL_OUT, APU12_OFF, 0, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );

	cbitclose(K16, K17);
	lwait (3 ms_f);

	apu12setcapload( IN1, APU12_FV, 12, APU12_30V, APU12_100MA, 0.5, APU12_PIN_TO_VI );
	apu12set( EN, APU12_FV, 0, APU12_30V, APU12_100UA, APU12_PIN_TO_VI );

	apu12set( PEDESTAL1, APU12_FV, 0, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
	apu12set( PEDESTAL2, APU12_FV, 0, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );

	int iStackV = 26;

	//EN, BSTMON, PGATE at 52V
	if(Maxim::bTest_Step_Is("FT_ROOM") || Maxim::bTest_Step_Is("FT_HOT") || Maxim::bTest_Step_Is("FT_COLD") || Maxim::bTest_Step_Is("CHAR")){
		
		cbitclose(K14);
		lwait (3 ms_f);

		for(int i = 0; i < iStackV; i++){

			apu12set( IN1, APU12_FV, 12 - i, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
			apu12set( PEDESTAL1, APU12_FV, i, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
			apu12set( PEDESTAL2, APU12_FV, i, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );

			apu12set( EN, APU12_FV, i, APU12_30V, APU12_100UA, APU12_PIN_TO_VI );
			apu12set( BSTMON, APU12_FV, i, APU12_30V, APU12_100UA, APU12_PIN_TO_VI );
			apu12set( PGATE, APU12_FV, i, APU12_30V, APU12_100UA, APU12_PIN_TO_VI );

			lwait (200 us_f);

		}
					
		apu12set( EN, APU12_FV, 26, APU12_30V, APU12_100UA, APU12_PIN_TO_VI );
		apu12set( BSTMON, APU12_FV, 26, APU12_30V, APU12_100UA, APU12_PIN_TO_VI );
		apu12set( PGATE, APU12_FV, 26, APU12_30V, APU12_100UA, APU12_PIN_TO_VI );

		pldResults = apu12mi(EN, APU12_MI_1X, 10);	
		Maxim::DatalogAll(DSIndex, pldResults * uA);

		pldResults = apu12mi(PGATE, APU12_MI_10X, 10);	
		Maxim::DatalogAll(DSIndex, pldResults * uA);

		pldResults = apu12mi(BSTMON, APU12_MI_10X, 10);	
		Maxim::DatalogAll(DSIndex, pldResults * uA);

		for(int i = iStackV; i > 0; i--){

			apu12set( EN, APU12_FV, i, APU12_30V, APU12_100UA, APU12_PIN_TO_VI );
			apu12set( BSTMON, APU12_FV, i, APU12_30V, APU12_100UA, APU12_PIN_TO_VI );
			apu12set( PGATE, APU12_FV, i, APU12_30V, APU12_100UA, APU12_PIN_TO_VI );

			apu12set( PEDESTAL1, APU12_FV, i, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
			apu12set( PEDESTAL2, APU12_FV, i, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
			apu12set( IN1, APU12_FV, 12 - i, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
			lwait (200 us_f);

		}

		apu12set( EN, APU12_FV, 0, APU12_30V, APU12_100UA, APU12_PIN_TO_VI );
		apu12set( BSTMON, APU12_FV, 0, APU12_30V, APU12_100UA, APU12_PIN_TO_VI );
		apu12set( PGATE, APU12_OFF, 0, APU12_30V, APU12_100UA, APU12_PIN_TO_VI );

		apu12set( PEDESTAL1, APU12_FV, 0, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
		apu12set( PEDESTAL2, APU12_FV, 0, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
		apu12set( IN1, APU12_FV, 12, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
		lwait (2 ms_f);

		cbitopen(K14);
	}



	apu12set( BSTMON, APU12_OFF,0, APU12_30V, APU12_100UA, APU12_PIN_TO_VI );
	apu12set( EN, APU12_OFF, 0, APU12_30V, APU12_100UA, APU12_PIN_TO_VI );
	cbitclose(K31, K40, K23, K50);
	
	dpinviset( "SHARED_DPU", DPIN_FV, 0.7, DPIN_8V, DPIN_512UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL );
	lwait (2 ms_f);

	PinListData pldOUTi[6];
	PME_INT OUT_PIN[6] = {OUT1, OUT2, OUT3, OUT4, OUT5, OUT6};

	iStackV = 24;

	//OUT Leak
	for(int i = 0; i <= iStackV; i++){
				
		apu12set( IN1, APU12_FV, 12 - i, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
		apu12set( PEDESTAL1, APU12_FV, i, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
		apu12set( PEDESTAL2, APU12_FV, i, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
		apu12set( ALL_OUT, APU12_FV, i, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );

		lwait (200 us_f);

	}
	
	lwait (2 ms_f);


	//FT only (All OUT at 52V)
	if(Maxim::bTest_Step_Is("FT_ROOM") || Maxim::bTest_Step_Is("FT_HOT") || Maxim::bTest_Step_Is("FT_COLD") || Maxim::bTest_Step_Is("CHAR")){

		apu12setcapload( ALL_OUT, APU12_FV, 52 - iStackV, APU12_30V, APU12_1MA, 2, APU12_PIN_TO_VI );
		lwait (50 ms_f);

		for(int i = 0; i < 6; i++){
			pldOUTi[i] = apu12mi(OUT_PIN[i], APU12_MI_10X, 10);
		}

		//Total_OUT_Leakage52V
		pldResults = pldOUTi[0]+pldOUTi[1]+pldOUTi[2]+pldOUTi[3]+pldOUTi[4]+pldOUTi[5];

		Maxim::DatalogAll(DSIndex, pldResults * uA);

		//Back to 48V
		apu12setcapload( ALL_OUT, APU12_FV, 48 - iStackV, APU12_30V, APU12_1MA, 2, APU12_PIN_TO_VI );
		lwait (2 ms_f);
	}
	else
		DSIndex+=1;



	//Total_OUT_Leakage
	apu12set( ALL_OUT, APU12_FV, 48 - iStackV, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
	lwait (100 ms_f);

	for(int i = 0; i < 6; i++){
		pldOUTi[i] = apu12mi(OUT_PIN[i], APU12_MI_10X, 10);
	}

	pldResults = pldOUTi[0]+pldOUTi[1]+pldOUTi[2]+pldOUTi[3]+pldOUTi[4]+pldOUTi[5];

	Maxim::DatalogAll(DSIndex, pldResults * uA);





	//I_OUT1_L
	for(int i = iStackV; i >= -iStackV; i--){
		apu12set( OUT1, APU12_FV, i, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
		lwait (200 us_f);
	}
	apu12set( OUT1, APU12_FV, -iStackV, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
	lwait (10 ms_f);

	pldResults = apu12mi(OUT1, APU12_MI_10X, 10);	
	Maxim::DatalogAll(DSIndex, pldResults * uA);

	//Restore
	for(int i = -iStackV; i <= iStackV; i++){
		apu12set( OUT1, APU12_FV, i, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
		apu12set( OUT2, APU12_FV, -i, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
		lwait (200 us_f);
	}
	apu12set( OUT1, APU12_FV, iStackV, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );


	//I_OUT2_L
	apu12set( OUT2, APU12_FV, -iStackV, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
	lwait (10 ms_f);

	pldResults = apu12mi(OUT2, APU12_MI_10X, 10);	
	Maxim::DatalogAll(DSIndex, pldResults * uA);

	//Restore
	for(int i = -iStackV; i <= iStackV; i++){
		apu12set( OUT2, APU12_FV, i, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
		apu12set( OUT3, APU12_FV, -i, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
		lwait (200 us_f);
	}
	apu12set( OUT2, APU12_FV, iStackV, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );


	//I_OUT3_L
	apu12set( OUT3, APU12_FV, -iStackV, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
	lwait (10 ms_f);

	pldResults = apu12mi(OUT3, APU12_MI_10X, 10);	
	Maxim::DatalogAll(DSIndex, pldResults * uA);

	//Restore
	for(int i = -iStackV; i <= iStackV; i++){
		apu12set( OUT3, APU12_FV, i, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
		apu12set( OUT4, APU12_FV, -i, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
		lwait (200 us_f);
	}
	apu12set( OUT3, APU12_FV, iStackV, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );



	//I_OUT4_L
	apu12set( OUT4, APU12_FV, -iStackV, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
	lwait (10 ms_f);

	pldResults = apu12mi(OUT4, APU12_MI_10X, 10);	
	Maxim::DatalogAll(DSIndex, pldResults * uA);


	//Restore
	for(int i = -iStackV; i <= iStackV; i++){
		apu12set( OUT4, APU12_FV, i, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
		apu12set( OUT5, APU12_FV, -i, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
		lwait (200 us_f);
	}
	apu12set( OUT4, APU12_FV, iStackV, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );




	//I_OUT5_L
	apu12set( OUT5, APU12_FV, -iStackV, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
	lwait (10 ms_f);

	pldResults = apu12mi(OUT5, APU12_MI_10X, 10);	
	Maxim::DatalogAll(DSIndex, pldResults * uA);

	//Restore
	for(int i = -iStackV; i <= iStackV; i++){
		apu12set( OUT5, APU12_FV, i, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
		apu12set( OUT6, APU12_FV, -i, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
		lwait (200 us_f);
	}
	apu12set( OUT5, APU12_FV, iStackV, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );



	//I_OUT6_L
	apu12set( OUT6, APU12_FV, -iStackV, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
	lwait (10 ms_f);

	pldResults = apu12mi(OUT6, APU12_MI_10X, 10);	
	Maxim::DatalogAll(DSIndex, pldResults * uA);




	

	//I_OUT1_H
	for(int i = iStackV; i >= -iStackV; i--){
		apu12set( OUT2, APU12_FV, i, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
		apu12set( OUT3, APU12_FV, i, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
		apu12set( OUT4, APU12_FV, i, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
		apu12set( OUT5, APU12_FV, i, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
		lwait (200 us_f);
	}
	lwait (10 ms_f);

	pldResults = apu12mi(OUT1, APU12_MI_10X, 10);	
	Maxim::DatalogAll(DSIndex, pldResults * uA);

	//Restore
	for(int i = iStackV; i >= -iStackV; i--){
		apu12set( OUT1, APU12_FV, i, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
		apu12set( OUT2, APU12_FV, -i, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
		lwait (200 us_f);
	}
	apu12set( OUT1, APU12_FV, -iStackV, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );


	//I_OUT2_H
	apu12set( OUT2, APU12_FV, iStackV, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
	lwait (10 ms_f);

	pldResults = apu12mi(OUT2, APU12_MI_10X, 10);	
	Maxim::DatalogAll(DSIndex, pldResults * uA);

	//Restore
	for(int i = iStackV; i >= -iStackV; i--){
		apu12set( OUT2, APU12_FV, i, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
		apu12set( OUT3, APU12_FV, -i, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
		lwait (200 us_f);
	}
	apu12set( OUT2, APU12_FV, -iStackV, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );


	//I_OUT3_H
	apu12set( OUT3, APU12_FV, iStackV, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
	lwait (10 ms_f);

	pldResults = apu12mi(OUT3, APU12_MI_10X, 10);	
	Maxim::DatalogAll(DSIndex, pldResults * uA);

	//Restore
	for(int i = iStackV; i >= -iStackV; i--){
		apu12set( OUT3, APU12_FV, i, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
		apu12set( OUT4, APU12_FV, -i, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
		lwait (200 us_f);
	}
	apu12set( OUT3, APU12_FV, -iStackV, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );



	//I_OUT4_H
	apu12set( OUT4, APU12_FV, iStackV, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
	lwait (10 ms_f);

	pldResults = apu12mi(OUT4, APU12_MI_10X, 10);	
	Maxim::DatalogAll(DSIndex, pldResults * uA);


	//Restore
	for(int i = iStackV; i >= -iStackV; i--){
		apu12set( OUT4, APU12_FV, i, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
		apu12set( OUT5, APU12_FV, -i, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
		lwait (200 us_f);
	}
	apu12set( OUT4, APU12_FV, -iStackV, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );




	//I_OUT5_H
	apu12set( OUT5, APU12_FV, iStackV, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
	lwait (10 ms_f);

	pldResults = apu12mi(OUT5, APU12_MI_10X, 10);	
	Maxim::DatalogAll(DSIndex, pldResults * uA);

	//Restore
	for(int i = iStackV; i >= -iStackV; i--){
		apu12set( OUT5, APU12_FV, i, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
		apu12set( OUT6, APU12_FV, -i, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
		lwait (200 us_f);
	}
	apu12set( OUT5, APU12_FV, -iStackV, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );



	//I_OUT6_H
	apu12set( OUT6, APU12_FV, iStackV, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
	lwait (10 ms_f);

	pldResults = apu12mi(OUT6, APU12_MI_10X, 10);	
	Maxim::DatalogAll(DSIndex, pldResults * uA);


	//Restore
	for(int i = iStackV; i >= -iStackV; i--){
		apu12set( OUT6, APU12_FV, i, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
		lwait (200 us_f);
	}
	apu12set( OUT6, APU12_FV, -iStackV, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );





	for(int i = iStackV; i >= 0; i--){

		apu12set( ALL_OUT, APU12_FV, 0 - i, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
		apu12set( PEDESTAL1, APU12_FV, i, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
		apu12set( PEDESTAL2, APU12_FV, i, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
		apu12set( IN1, APU12_FV, 12 - i, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
		lwait (200 us_f);

	}


	apu12setcapload( PEDESTAL1, APU12_FV, 0, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );
	apu12setcapload( PEDESTAL2, APU12_FV, 0, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );
	apu12setcapload( IN1, APU12_FV, 0, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	apu12set( ALL_OUT, APU12_OFF, 0 , APU12_30V, APU12_1MA, APU12_PIN_TO_VI );


	cbitopen(K14, K31);
	
	dpinviset( "SHARED_DPU", DPIN_FV, 0.0, DPIN_8V, DPIN_512UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL );


    //End of function
    Maxim::DatalogFlush();	//Output the datalog queue.
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC
