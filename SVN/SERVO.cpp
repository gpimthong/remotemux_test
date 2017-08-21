/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:
*		Servo.cpp
*
*	Written and maintained by:
*		Govit Pimthong (govit.pimthong@MaximIntegrated.com)
*
*	URL:                   $URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/SERVO.cpp $
*	Last Modified by:      $LastChangedBy: govit.pimthong $
*	Date:                  $Date: 2017-08-03 22:51:35 +0700 (Thu, 03 Aug 2017) $
*	Current Rev:           $Revision: 67634 $
*
*	Description:
*		File Description
*
*	Revision History
*
*
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
#include "AP89_FT.h"



void NDRV_Freq(PinListData &pldResultMax, PinListData &pldResultMin, PinListData &pldResultAvg){
#define	SS_SAMPLES 20

	RESULTS_STR* results;
	results = new RESULTS_STR[NUM_SITES];

	qtmustart( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_POS, 0, 100);
	qtmustop( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_POS, 1, 100);
	lwait( 1 ms_f );	

	qtmuarm( QTMU_GP, "AUTO", SS_SAMPLES, QTMU_EDGE, QTMU_POS );

	pldResultAvg = pldResultMin = pldResultMax = qtmumt( QTMU_GP, QTMU_READFREQ, 1000);


	//Grab all data points
	groupgetresults(results, NUM_SITES);
	namearray("Freq_data", NAMEARRAY_QTMU, QTMU_GP, 1 );


	//Find Min Freq
	peak("Freq_data", 1, SS_SAMPLES, PEAK_HI);
	groupgetresults(results, NUM_SITES);

	ForEachActive_iSite{
		pldResultMin[iSite] = 1e6 / results[iSite].value;
	}



	//Find Max Freq
	peak("Freq_data", 1, SS_SAMPLES, PEAK_LO);
	groupgetresults(results, NUM_SITES);

	ForEachActive_iSite{
		pldResultMax[iSite] = 1e6 / results[iSite].value;
	}


}


void DIM_Hi(LPCTSTR some_name, int some_int){
	
	//Maxim::I2C::Write(AP89_AD, 0x02, 0x2B);
	//lwait(1 ms_f);
	//Maxim::I2C::Write(AP89_AD, 0x02, 0x20);

	dpinforce("DIM", "1", 3.0, 0.1, MS_ALL); //DIM
	lwait(1 ms_f);
	dpinforce("DIM", "0", 3.0, 0.1, MS_ALL); //DIM
	lwait(1 ms_f);

}

void DIM_Lo(LPCTSTR some_name, int some_int){

	//Maxim::I2C::Write(AP89_AD, 0x02, 0x20);
	//lwait(1 ms_f);
	//Maxim::I2C::Write(AP89_AD, 0x02, 0x2B);


	dpinforce("DIM", "0", 3.0, 0.1, MS_ALL); //DIM
	lwait(1 ms_f);
	dpinforce("DIM", "1", 3.0, 0.1, MS_ALL); //DIM
	lwait(1 ms_f);


}


// Test Function: Servo
ETS_PRGFLOW_FUNC Servo( int DSIndex, LPCTSTR TestLabel )
{
     
    //declare variables
    PinListData pldResults;	//declare a PLD variable for the measurement results.
	PinListData pldFreqMax, pldFreqMin, pldFreqAvg, pldSUP_Servo_I;

	//Need Power cycle to exit test mode

	//Step#1
	dpinforce("DIM", "0", 3.0, 0.1, MS_ALL); //DIM
	dpinforce("SCL_IO", "0", 3.0, 0.0, MS_ALL);
	dpinforce("SDA_IO", "0", 3.0, 0.0, MS_ALL);
	lwait (1 ms_f);	

	cbitclose(K7);


	apu12setcapload( EN, APU12_FV, 0, APU12_10V, APU12_1MA, 1, APU12_PIN_TO_VI );
	apu12setcapload( BSTMON, APU12_FV, 0, APU12_30V, APU12_1MA, 1, APU12_PIN_TO_VI );
	apu12set( FSEN, APU12_FV, 0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	apu12set( CS, APU12_FV, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	apu12set( COMP, APU12_OFF, 0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	apu12set( ALL_OUT, APU12_FV, 0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );

	apu12setcapload( IN1, APU12_FV, 0, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );




	//Config SERVO
	cbitclose(K22);
	cbitclose(K52, K54, K56);
	cbitclose(K59, K61, K63);
	lwait (10 ms_f);



	//Step#2
	apu12set( ALL_OUT, APU12_FV, 0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );
	apu12set( BSTMON, APU12_FV, 1.5, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);


	//Step#3
	apu12setcapload( IN1, APU12_FV, 12, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );


	dpinforce("DIM", "1", 3.0, 0.1, MS_ALL); //DIM at 5V
	apu12set( EN, APU12_FV, 5, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	Maxim::I2C::Write(AP89_AD, 0x02, 0x20);

	lwait (200 ms_f);


	apu12set( BSTMON, APU12_OFF, 1.5, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	lwait (100 ms_f);



	PinListData OUTA_RA, OUTB_RB, OUTA_RB, OUTB_RA;

	pldFreqLow = pldFreqHigh = OUTA_RA = apu12mv(OUT1, 100);
	OUTB_RB = apu12mv(OUT4, 100);


	Maxim::DatalogAll(DSIndex, OUTA_RA);
	Maxim::DatalogAll(DSIndex, OUTB_RB);





	dpinforce("DIM", "0", 3.0, 0.1, MS_ALL);
	apu12set( EN, APU12_FV, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	cbitopen(K52, K54, K56, K59, K61, K63);
	cbitclose(K53, K55, K57, K58, K60, K62);
	lwait (2 ms_f);

	apu12set( BSTMON, APU12_FV, 1.5, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	dpinforce("DIM", "1", 3.0, 0.1, MS_ALL); //DIM at 5V
	apu12set( EN, APU12_FV, 5, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	Maxim::I2C::Write(AP89_AD, 0x02, 0x20);

	lwait (200 ms_f);


	apu12set( BSTMON, APU12_OFF, 1.5, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	lwait (100 ms_f);



	OUTA_RB = apu12mv(OUT1, 100);
	OUTB_RA = apu12mv(OUT4, 100);

	//pldSUP_Servo_I = apu12mi(IN1, APU12_MI_1X, 10);

	Maxim::DatalogAll(DSIndex, OUTA_RB);
	Maxim::DatalogAll(DSIndex, OUTB_RA);

	dpinrcvset("SCAN_OUT", DPIN_RCV_SPLIT, 2, 1, MS_ALL);
	cbitclose(K43, K44);
	cbitopen(K7);
	lwait(10 ms_f);


	apu12set( COMP, APU12_FV, 2.15, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	lwait(2 ms_f);
	apu12set( COMP, APU12_FV, 1.5, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	dpinforce("DIM", "0", 3.0, 0.1, MS_ALL);
	lwait(20 ms_f);

	pldSUP_Servo_I = apu12mi(IN1, APU12_MI_1X, 10);
	NDRV_Freq(pldFreqMax, pldFreqMin, pldFreqAvg);

	//ForEachActive_iSite{
	//	if((pldFreqAvg[iSite] < 350) || (pldFreqAvg[iSite] > 450)){
	//		lwait(1 ms_f);
	//	}
	//}
	
	Maxim::DatalogAll(DSIndex, pldFreqMin);
	Maxim::DatalogAll(DSIndex, pldFreqMax);
	Maxim::DatalogAll(DSIndex, pldFreqAvg);

	apu12set( COMP, APU12_OFF, 2, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );

	//Regulation check at 2.2MHz freq (CHAR only)
	if(Maxim::isChar()){


		cbitclose(K7, K36);
		cbitopen(K37);

		dpinforce("DIM", "0", 3.0, 0.1, MS_ALL);
		apu12set( EN, APU12_FV, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
		lwait (2 ms_f);

		apu12set( BSTMON, APU12_FV, 1.5, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
		dpinforce("DIM", "1", 3.0, 0.1, MS_ALL); //DIM at 5V
		apu12set( EN, APU12_FV, 5, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
		lwait (2 ms_f);

		Maxim::I2C::Write(AP89_AD, 0x02, 0x20);

		lwait (200 ms_f);

		apu12set( BSTMON, APU12_OFF, 1.5, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
		lwait (100 ms_f);


		OUTA_RB = apu12mv(OUT1, 100);
		OUTB_RA = apu12mv(OUT4, 100);


		Maxim::DatalogAll(DSIndex, OUTA_RB);
		Maxim::DatalogAll(DSIndex, OUTB_RA);




		dpinforce("DIM", "0", 3.0, 0.1, MS_ALL);
		apu12set( EN, APU12_FV, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
		lwait (2 ms_f);

		cbitopen(K53, K55, K57, K58, K60, K62);
		cbitclose(K52, K54, K56, K59, K61, K63);
		lwait (2 ms_f);

		apu12set( BSTMON, APU12_FV, 1.5, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
		dpinforce("DIM", "1", 3.0, 0.1, MS_ALL); //DIM at 5V
		apu12set( EN, APU12_FV, 5, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
		lwait (2 ms_f);

		Maxim::I2C::Write(AP89_AD, 0x02, 0x20);

		lwait (200 ms_f);



		apu12set( BSTMON, APU12_OFF, 1.5, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
		lwait (100 ms_f);



		OUTA_RA = apu12mv(OUT1, 100);
		OUTB_RB = apu12mv(OUT4, 100);


		Maxim::DatalogAll(DSIndex, OUTA_RA);
		Maxim::DatalogAll(DSIndex, OUTB_RB);


		cbitopen(K7);
		lwait(10 ms_f);

		apu12set( COMP, APU12_FV, 2.15, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
		lwait(2 ms_f);
		apu12set( COMP, APU12_FV, 1.5, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
		dpinforce("DIM", "0", 3.0, 0.1, MS_ALL);
		lwait(20 ms_f);


		NDRV_Freq(pldFreqMax, pldFreqMin, pldFreqAvg);

		//ForEachActive_iSite{
		//	if((pldFreqAvg[iSite] < 2000) || (pldFreqAvg[iSite] > 2300)){
		//		lwait(1 ms_f);
		//	}
		//}

		Maxim::DatalogAll(DSIndex, pldFreqMin * MHz);
		Maxim::DatalogAll(DSIndex, pldFreqMax * MHz);
		Maxim::DatalogAll(DSIndex, pldFreqAvg * MHz);

		cbitclose(K37);
		cbitopen(K36);	
	
	}
	else
		DSIndex+=7;


	Maxim::DatalogAll(DSIndex, pldSUP_Servo_I);

	apu12set( COMP, APU12_OFF, 0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	cbitopen(K43, K44);
	dpinrcvset("SCAN_OUT", DPIN_OFF, 2, 1, MS_ALL);

	dpinforce("DIM", "0", 3.0, 0.1, MS_ALL); //DIM at 5V
	apu12set( EN, APU12_FV, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	lwait (2 ms_f);



	//Config SERVO
	cbitopen(K7, K22, K43, K44);
	cbitopen(K53, K55, K57, K58, K60, K62, K52, K54, K56, K59, K61, K63);




	

	////OUT RISING and FALLING

	//double dStart, dStop;


	////Power up 5V DC
	//cbitclose(K24, K46, K43);
	//apu12set( ALL_OUT, APU12_FV, 0.7, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	//apu12set( BSTMON, APU12_FV, 0.7, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );

	//dpinforce("DIM", "0", 3.0, 0.1, MS_ALL); //DIM at 0V
	//apu12set( EN, APU12_FV, 5, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	//lwait(5 ms_f);


	//int iSink = 0x07;
	//// Code : 0, Current = 45 mA
	//// Code : 1, Current = 50 mA
	//// Code : 2, Current = 55 mA
	//// Code : 3, Current = 60 mA
	//// Code : 4, Current = 65 mA
	//// Code : 5, Current = 70 mA
	//// Code : 6, Current = 75 mA
	//// Code : 7, Current = 80 mA
	//// Code : 8, Current = 85 mA
	//// Code : 9, Current = 90 mA
	//// Code : A, Current = 95 mA
	//// Code : B, Current = 100 mA
	//// Code : C, Current = 105 mA
	//// Code : D, Current = 110 mA
	//// Code : E, Current = 115 mA
	//// Code : F, Current = 120 mA
	//Maxim::I2C::Write(AP89_AD, 0x02, 0x2 | iSink);





	///*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
	//* Test : OUT1
	//**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
	////Setup
	//apu12set( OUT1, APU12_OFF, 0.7, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	//cbitclose(K52);
	//lwait (5 ms_f);

	//dStart = 0.9;
	//dStop = 0.4;

	//pldResults = MeasureTime(QTMU_GP, din1, din1, "AUTO", 1, DIM_Hi, "", 1 ms_f, QTMU_RCVH, QTMU_RCVL, QTMU_NEG, QTMU_NEG, 0, 0, dStart, dStop, 10, 10);

	//pldResults = MeasureTime(QTMU_GP, din1, din1, "AUTO", 1, DIM_Lo, "", 1 ms_f, QTMU_RCVL, QTMU_RCVH, QTMU_POS, QTMU_POS, 0, 0, dStop, dStart, 10, 10);

	//cbitopen(K52);
	//apu12set( OUT1, APU12_FV, 0.7, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );

	//
	////Restore
	//Maxim::I2C::Write(AP89_AD, 0x02, 0x20);





	//cbitopen(K24, K46, K43);
	//apu12set( ALL_OUT, APU12_OFF, 0.3, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );

	//double dV = 0;
	//
	//if(0){
	//	apu12set( ALL_OUT, APU12_FV, dV, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );
	//	apu12set( ALL_OUT, APU12_FV, dV, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	//	apu12set( ALL_OUT, APU12_OFF, 0.3, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	//}

     
    //End of function
    Maxim::DatalogFlush();	//Output the datalog queue.
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC
