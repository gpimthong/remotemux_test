/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:
*		Error_Amp.cpp
*
*	Written and maintained by:
*		Govit Pimthong (govit.pimthong@MaximIntegrated.com)
*
*	URL:                   $URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/Error_Amp.cpp $
*	Last Modified by:      $LastChangedBy: govit.pimthong $
*	Date:                  $Date: 2017-08-01 01:01:53 +0700 (Tue, 01 Aug 2017) $
*	Current Rev:           $Revision: 67487 $
*
*	Description:
*		File Description
*
*	Revision History
*
*
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
#include "AP89_FT.h"
#define SS_SAMPLES 100

PinListData gpldNDRV_Freq;
PinListData gpldNDRV_Freq_Min;
PinListData gpldNDRV_Freq_Max;

PinListData gpldNDRV_Duty;
PinListData gpldNDRV_Duty_Min;
PinListData gpldNDRV_Duty_Max;

void COMP_SET(SiteDouble value){

	ForEachActive_iSite{
		apu12set(COMP[iSite], APU12_FV, value[iSite], APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI);
	}

}

void COMP_SET(double value){

	apu12set(COMP, APU12_FV, value, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI);

}


PinListData NDRV_Ton_measure(){

	PinListData pldResult;
	
	qtmuarm( QTMU_GP, "AUTO", 1, QTMU_EDGE, QTMU_POS );
	pldResult = qtmumt( QTMU_GP, QTMU_READTIME, 50);

	ForEachActive_iSite{

		if(pldResult[iSite] > 1e10)
			pldResult[iSite] = -1;

	}
	
	return pldResult;

}


PinListData NDRV_Freq_measure(){
	
	RESULTS_STR* results;
	results = new RESULTS_STR[NUM_SITES];

	qtmustart( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_POS, 0, 100);
	qtmustop( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_POS, 1, 100);
	lwait( 1 ms_f );	
	
	qtmuarm( QTMU_GP, "AUTO", SS_SAMPLES, QTMU_EDGE, QTMU_POS );

	gpldNDRV_Freq = qtmumt( QTMU_GP, QTMU_READFREQ, 1000);
	

	//Grab all data points
	groupgetresults(results, NUM_SITES);
	namearray("Freq_data", NAMEARRAY_QTMU, QTMU_GP, 1 );


	//Find Min Freq
	peak("Freq_data", 1, SS_SAMPLES, PEAK_HI);
	groupgetresults(results, NUM_SITES);

	ForEachActive_iSite{
		gpldNDRV_Freq_Min[iSite] = 1e6 / results[iSite].value;
	}



	//Find Max Freq
	peak("Freq_data", 1, SS_SAMPLES, PEAK_LO);
	groupgetresults(results, NUM_SITES);

	ForEachActive_iSite{
		gpldNDRV_Freq_Max[iSite] = 1e6 / results[iSite].value;
	}

	
	return gpldNDRV_Freq;


}


PinListData NDRV_Timing_measure()
{

	RESULTS_STR* results;
	results = new RESULTS_STR[NUM_SITES];

	qtmuarm( QTMU_GP, "AUTO", SS_SAMPLES, QTMU_EDGE, QTMU_POS );
	gpldNDRV_Duty = qtmumt( QTMU_GP, QTMU_READTIME, 1000 ) * gpldNDRV_Freq * 1e-6;


	//Grab all data points
	groupgetresults(results, NUM_SITES);
	namearray("Duty_data", NAMEARRAY_QTMU, QTMU_GP, 1 );


	//Find Max Duty
	peak("Duty_data", 1, SS_SAMPLES, PEAK_HI);
	groupgetresults(results, NUM_SITES);

	ForEachActive_iSite{

		if (results[iSite].value < 0)
			gpldNDRV_Duty_Max[iSite] = 0;
		else
			gpldNDRV_Duty_Max[iSite] = (results[iSite].value * gpldNDRV_Freq_Min[iSite]) * 1e-6;

	}



	//Find Min Duty
	peak("Duty_data", 1, SS_SAMPLES, PEAK_LO);
	groupgetresults(results, NUM_SITES);

	ForEachActive_iSite{

		if (results[iSite].value < 0)
			gpldNDRV_Duty_Min[iSite] = 0;
		else
			gpldNDRV_Duty_Min[iSite] = (results[iSite].value * gpldNDRV_Freq_Max[iSite]) * 1e-6;

	}


	return gpldNDRV_Duty_Min;

}

PinListData COMP_Search(double dBegin, double dEnd, double dAccu, SiteDouble sdThres, PinListData (*MeasFunc)(void)){

	// Initialise lower and upper bounnds
	SiteDouble lb, ub, mid;
	SiteBool sbFinish;
	PinListData pldResult;

	lb = dBegin;
	ub = dEnd;

	ForEachActive_iSite{
		sbFinish[iSite] = (lb[iSite] < ub[iSite])?false:true;
	}

	// Perform Binary search
	while (sbFinish.Any(false))
	{

		sbFinish = true;

		COMP_SET(dBegin);

		// Find mid
		mid = (lb+ub)/2;


		COMP_SET(mid);

		pldResult = MeasFunc();

		//update lower_bound or upper_bound
		ForEachActive_iSite{
			if(pldResult[iSite] < sdThres[iSite])
				lb[iSite] = mid[iSite];
			else
				ub[iSite] = mid[iSite];
		}


		ForEachActive_iSite{
			sbFinish[iSite] = (lb[iSite] < ub[iSite] - dAccu)?false:true;
			if(sbFinish[iSite]) pldResult[iSite] = mid[iSite];
		}

	}

	return pldResult;

}


void GM_Meas(int& DSIndex){

	PinListData pldV1, pldV2, pldI1, pldI2, pldGM;

	apu12set(BSTMON, APU12_FV, 0.975 V_f, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI);
	lwait(5 ms_f);

	pldV1 = apu12mv(BSTMON, 10);
	pldI1 = apu12mi(COMP, APU12_MI_1X, 10, 13.0 );

	apu12set(BSTMON, APU12_FV, 0.875 V_f, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI);
	lwait(5 ms_f);

	pldV2 = apu12mv(BSTMON, 10);
	pldI2 = apu12mi(COMP, APU12_MI_1X, 10, 13.0 );

	pldGM = 1e3 * (pldI1 - pldI2) / (pldV1 - pldV2); // reported in uA/V or uS

	Maxim::DatalogAll(DSIndex, pldGM);
	Maxim::DatalogAll(DSIndex, pldV1);
	Maxim::DatalogAll(DSIndex, pldV2);
	Maxim::DatalogAll(DSIndex, pldI1 * uA);
	Maxim::DatalogAll(DSIndex, pldI2 * uA);

}


// Test Function: Error_Amp
ETS_PRGFLOW_FUNC Error_Amp( int DSIndex, LPCTSTR TestLabel )
{
     
    //declare variables
    PinListData pldResults;	//declare a PLD variable for the measurement results.



	//apu12set( EN, APU12_OFF, 0, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );
	cbitclose(K40, K41);	
	//apu12setcapload( IN1, APU12_FV, 12, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );

	////*************************************************************************************************

	//lwait (2 ms_f);

	////Write '1' to ENA bit REG02<5>
	//dpinviset("SDA", DPIN_LOAD, 4, DPIN_8V, DPIN_512UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL);
	//Maxim::I2C::Write(AP89_AD, 0x02, 0x20);
	//lwait (2 ms_f);

	//DUT::Initiate_TestMode();

	//Maxim::I2C::SetSlaveAddress(OTP_3DV_AD);

	//Maxim::I2C::Write(UNLOCK1_REG, 0x17); //Unlock passwd0 = 0x17
	//Maxim::I2C::Write(UNLOCK2_REG, 0x65); //Unlock passwd1 = 0x65

	//Maxim::I2C::Write(OTPMODE_REG, 0x00); //Turn off Look and bypass


	dpinforce("DIM", "0", 3.0, 0.1, MS_ALL); //DIM LOW



	PinListData pldICompSRC, pldICompSNK;

	Maxim::I2C::SetSlaveAddress(TM_3DV_AD);

	Maxim::I2C::Write(0x02, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x02, 0x40); //write data to DHR3<15:12>=0000
	Maxim::I2C::Write(0x02, 0x31); //write data to DHR2<11:8>=1000
	Maxim::I2C::Write(0x02, 0x20); //write data to DHR1<7:4>=0000
	Maxim::I2C::Write(0x02, 0x10); //write data to DHR0<3:0>=1110
	Maxim::I2C::Write(0x02, 0xA0); //write DHR<15:0> to DCTLR<15:0>

	Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>=0000
	Maxim::I2C::Write(0x01, 0x31); //write data to DHR2<11:8>=1000
	Maxim::I2C::Write(0x01, 0x24); //write data to DHR1<7:4>=0000
	Maxim::I2C::Write(0x01, 0x10); //write data to DHR0<3:0>=1110
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>

	Maxim::I2C::Write(0x01, 0x61); //D0 MUX
	Maxim::I2C::Write(0x01, 0x92); //DI<2>


	apu12set(COMP, APU12_FV, 2, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI);
	apu12set(BSTMON, APU12_FV, 1.825 V_f, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI); // (925+900) mV
	lwait(2 ms_f);


	pldICompSNK = apu12mi(COMP, APU12_MI_1X, 20, 13.0 );
    Maxim::DatalogAll(DSIndex, pldICompSNK * uA);



	apu12set(COMP, APU12_FV, 1, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI);
	apu12set(BSTMON, APU12_FV, 0.025 V_f, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI); // (925-900) mV
	lwait(2 ms_f);

	pldICompSRC = apu12mi(COMP, APU12_MI_1X, 20, 13.0 );
	
    Maxim::DatalogAll(DSIndex, pldICompSRC * uA);



	
	//Transconductance-IN12V-Comp1V
	GM_Meas(DSIndex);


	//Transconductance-IN12V-Comp2V
	apu12set(COMP, APU12_FV, 2, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI);
	lwait (2 ms_f);
	GM_Meas(DSIndex);

	//Transconductance-IN4p5V-Comp2V
	apu12setcapload( IN1, APU12_FV, 4.5, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );
	lwait (2 ms_f);
	GM_Meas(DSIndex);

	//Transconductance-IN4p5V-Comp1V
	apu12set(COMP, APU12_FV, 2, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI);
	lwait (2 ms_f);
	GM_Meas(DSIndex);





	//PWM COMP

	apu12setcapload( IN1, APU12_FV, 12, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );
	cbitclose(K6);
	apu12set(VCC, APU12_FV, 5, APU12_10V, APU12_100UA, APU12_PIN_TO_VI);
	apu12set(CS, APU12_FV, 0.2, APU12_10V, APU12_1MA, APU12_PIN_TO_VI);
	apu12set(COMP, APU12_FV, 0.8, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI);

	lwait (2 ms_f);



	gbOUT_COMP_TH = true;

	THForcePin SetPin = {
		COMP,
		APU12_3p6V,
		APU12_10MA,
		APU12_FV
	};

	THMeasPin MeasPin = {
		VCC,
		APU12_10V,
		APU12_100UA,
		APU12_MV
	};

	TestTH tsTH =
	{
		SetPin,                 // Force pin detail
		MeasPin,                // Meas pin detail
		NULL,            // Threshold set function
		VCC_MeasV,           // Threshold measure function
		"PWMCOMP_R",               // Rising AWG Name
		"PWMCOMP_F",               // Falling AWG Name
		true,                   // Use AWG
		5000,                  // MCU divider
		APU12_ABOVE_THRESH,     // Threshold direction for rising, falling will be the opposite.
		1 mV_f,                 // Step size
		1,        // Output transition point when rising threshold is reached.
		1,        // Output transition point when falling threshold is reached.
		1,                     // Sample to skip before using awgwhereat to calculate threshold (in case using AWG)
		false,                  // Find threhsold from right (not usual case)
		false,                  // True if datalong is in percentage of some REF value
		0,                      // Ref value associate with percentage.
		35,                      // Offset to use with Go-no-Go , this number will be multiply with step size.
		1.0,					//Ref value in pld format
		false					//Falling search
	};


	ThresholdTest (DSIndex, &tsTH);


	cbitopen(K32);



	apu12set(CS, APU12_OFF, 0.2, APU12_10V, APU12_10MA, APU12_PIN_TO_VI);
	apu12set(COMP, APU12_FV, 0.8, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI);

	apu12setcapload( IN1, APU12_FV, 12, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );
	lwait (1 ms_f);


	apu12set(ALL_OUT, APU12_FV, 0.7, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI);

	

	DSIndex+=5;

	//	Search for COMP min
	//
	//	Connect the resistor from CS to GND; set RT to set 400Khz
	//	Set COMP =0.8V and Monitor SOURCE : it should be not switching;
	//	Sweep COMP until you see narrow pulses at NDRV: this is the COMP value the NDRV starts switching (Comp_thr_sw): datalog it. 
	//	Sweep COMP until the NDRV switches at the maximum duty cycle: datalog the  value of comp(Comp_max) at which the NDRV switches at max duty, and the NDRV duty (~95%);
	//	Now set COMP=comp_max -50mV , still monitoring the  switching NDRV : datalog the new value of NDRV duty cycle .
	//	The NDRV duty cycle should be different from max duty and around  60%.
	PinListData pldCOMP_Min, pldCOMP_Max;
	SiteDouble sdDuty;


	//Initialize 
	gpldNDRV_Freq = gpldNDRV_Duty = gpldNDRV_Duty_Min = gpldNDRV_Duty_Max = gpldNDRV_Freq_Min = gpldNDRV_Freq_Max = pldICompSNK;
	apu12set(BSTMON, APU12_FV, 0.7, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI);

	Maxim::I2C::Write(AP89_AD, 0x12, 0x18); //turn of spread spectrum

	
	dpinrcvset( "SCAN_OUT", DPIN_RCV_SPLIT, 2.0, 1.0, MS_ALL ); // set receiver pin comparator thresholds
	qtmumode( QTMU_GP, QTMU_TIMER, QTMU_ARMOUTOFF); // qtmu in timer mode, software armed

	cbitopen(K11, K45);
	cbitclose(K44, K43, K33);
	lwait(3 ms_f);

	double dStart = 0.8;
	double dStop = 1.5;
	double dStep = 0.002;
	double dThres = -0.09; //10uA

	//alarmset( ALARM_OVERRANGE, OFF );

	qtmustart( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_POS, 0, 50);
	qtmustop( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_NEG, 1, 50);
	lwait( 1 ms_f );	


	//pldCOMP_Min = APU_AWGThreshold( COMP, "COMP_MIN", APU12_3p6V, APU12_10MA, dStart, dStop, dStep, 10000, OUT3, dThres, APU12_BELOW_THRESH, 1 ms_f, APU12_FV, APU12_MI_1X);
	pldCOMP_Min = COMP_Search(dStart, dStop, dStep, 0, NDRV_Ton_measure);
	Maxim::DatalogAll(DSIndex, pldCOMP_Min);


	//alarmset( ALARM_OVERRANGE, ON ); 
	//ForEachActive_iSite{

	//	if((pldCOMP_Min[iSite] > dStart) && (pldCOMP_Min[iSite] < dStop))
	//		COMP_v[iSite].value1 = pldCOMP_Min[iSite];
	//	else
	//		COMP_v[iSite].value1 = 0.8;

	//}


	//groupsetaccumulator(COMP_v, NUM_SITES);
	//apu12set( COMP, APU12_FV, USE_ACCUM, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI );
	//lwait(1 ms_f);
	//	
	//NDRV_Freq_measure();


	//qtmustart( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_POS, 0, 50);
	//qtmustop( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_NEG, 0, 50);
	//lwait(1 ms_f);
	//		
	//pldResults = NDRV_Timing_measure();

	//Maxim::DatalogAll(DSIndex, gpldNDRV_Duty_Min * 100);
	//Maxim::DatalogAll(DSIndex, gpldNDRV_Duty_Max * 100);
	//Maxim::DatalogAll(DSIndex, gpldNDRV_Duty * 100);

	//Maxim::DatalogAll(DSIndex, gpldNDRV_Freq_Min);
	//Maxim::DatalogAll(DSIndex, gpldNDRV_Freq_Max);
	//Maxim::DatalogAll(DSIndex, gpldNDRV_Freq);

	DSIndex+=13;



	apu12set( COMP, APU12_FV, 1.5, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI );
	lwait(1 ms_f);

	NDRV_Freq_measure();


	qtmustart( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_POS, 0, 50);
	qtmustop( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_NEG, 0, 50);
	lwait(1 ms_f);

	pldResults = NDRV_Timing_measure();

	dStart = 1.0;
	dStop = 1.5;

	ForEachActive_iSite{
		sdDuty[iSite] = pldResults[iSite] - 0.01;
	}


	//pldCOMP_Max = Test_Ramp_Threshold_Char( dStart, dStop, dStep, sdDuty, 1, COMP_SET, NDRV_Timing_measure );

	pldCOMP_Max = COMP_Search(dStart, dStop, dStep, sdDuty, NDRV_Timing_measure);

	Maxim::DatalogAll(DSIndex, pldCOMP_Max);

	
	//ForEachActive_iSite{

	//	if((pldCOMP_Max[iSite] > dStart) && (pldCOMP_Max[iSite] < dStop))
	//		COMP_v[iSite].value1 = pldCOMP_Max[iSite];
	//	else
	//		COMP_v[iSite].value1 = 0.8;

	//}


	//groupsetaccumulator(COMP_v, NUM_SITES);
	//apu12set( COMP, APU12_FV, USE_ACCUM, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI );
	//lwait(1 ms_f);

	//NDRV_Freq_measure();


	//qtmustart( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_POS, 0, 50);
	//qtmustop( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_NEG, 0, 50);
	//lwait(1 ms_f);

	//pldResults = NDRV_Timing_measure();


	//Maxim::DatalogAll(DSIndex, gpldNDRV_Duty_Min * 100);
	//Maxim::DatalogAll(DSIndex, gpldNDRV_Duty_Max * 100);
	//Maxim::DatalogAll(DSIndex, gpldNDRV_Duty * 100);


	//
	////50mV below COMP Max
	//ForEachActive_iSite{

	//	if((pldCOMP_Max[iSite] > dStart) && (pldCOMP_Max[iSite] < dStop))
	//		COMP_v[iSite].value1 = pldCOMP_Max[iSite] - 0.05;
	//	else
	//		COMP_v[iSite].value1 = 0.8;

	//}

	//groupsetaccumulator(COMP_v, NUM_SITES);
	//apu12set( COMP, APU12_FV, USE_ACCUM, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI );
	//lwait(1 ms_f);
	//
	//NDRV_Freq_measure();


	//qtmustart( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_POS, 0, 50);
	//qtmustop( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_NEG, 0, 50);
	//lwait(1 ms_f);

	//pldResults = NDRV_Timing_measure();

	//Maxim::DatalogAll(DSIndex, pldCOMP_Max - 0.05);
	//Maxim::DatalogAll(DSIndex, gpldNDRV_Duty_Min * 100);
	//Maxim::DatalogAll(DSIndex, gpldNDRV_Duty_Max * 100);
	//Maxim::DatalogAll(DSIndex, gpldNDRV_Duty * 100);



	////100mV below COMP Max
	//ForEachActive_iSite{

	//	if((pldCOMP_Max[iSite] > dStart) && (pldCOMP_Max[iSite] < dStop))
	//		COMP_v[iSite].value1 = pldCOMP_Max[iSite] - 0.1;
	//	else
	//		COMP_v[iSite].value1 = 0.8;

	//}

	//groupsetaccumulator(COMP_v, NUM_SITES);
	//apu12set( COMP, APU12_FV, USE_ACCUM, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI );
	//lwait(10 ms_f);
	//
	//NDRV_Freq_measure();


	//qtmustart( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_POS, 0, 50);
	//qtmustop( QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_NEG, 0, 50);
	//lwait(1 ms_f);

	//pldResults = NDRV_Timing_measure();

	//Maxim::DatalogAll(DSIndex, pldCOMP_Max - 0.1);
	//Maxim::DatalogAll(DSIndex, gpldNDRV_Duty_Min * 100);
	//Maxim::DatalogAll(DSIndex, gpldNDRV_Duty_Max * 100);
	//Maxim::DatalogAll(DSIndex, gpldNDRV_Duty * 100);


	apu12set(COMP, APU12_OFF, 2, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI);

	cbitopen(K43, K33);

	DSIndex+=11;

	
	apu12set(ALL_OUT, APU12_FV, 0.9, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI);

	Maxim::I2C::SetSlaveAddress(TM_3DV_AD);
	Maxim::I2C::Write(0x02, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x02, 0x40); //write data to DHR3<15:12>=0000
	Maxim::I2C::Write(0x02, 0x30); //write data to DHR2<11:8>=1000
	Maxim::I2C::Write(0x02, 0x20); //write data to DHR1<7:4>=0000
	Maxim::I2C::Write(0x02, 0x10); //write data to DHR0<3:0>=1110
	Maxim::I2C::Write(0x02, 0xA0); //write DHR<15:0> to DCTLR<15:0>

	Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>=0000
	Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>=1000
	Maxim::I2C::Write(0x01, 0x2A); //write data to DHR1<7:4>=0000
	Maxim::I2C::Write(0x01, 0x10); //write data to DHR0<3:0>=1110
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>

	Maxim::I2C::Write(0x01, 0x60); //D0 MUX
	Maxim::I2C::Write(0x01, 0x90); //DI


	cbitclose(K6, K44, K45);
	apu12set(VCC, APU12_FV, 0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI);
	apu12set(COMP, APU12_OFF, 2, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI);

	gbOUT_COMP_TH = true;


	//Setup Force Pin
	tsTH.SetPin.pinName = OUT1;

	//Setup Threshold Test Detail
	tsTH.AwgNameR = "OUT1_R";
	tsTH.AwgNameF = "OUT1_F";
	tsTH.ThresholdDirection = APU12_BELOW_THRESH;
	tsTH.outTh1 = tsTH.outTh2 = 4;
	tsTH.Offset = 30;
	tsTH.dFallingSearch = true;

	apu12set(OUT1, APU12_FV, 0.7, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI);

	ThresholdTest (DSIndex, &tsTH);

	apu12set(OUT1, APU12_FV, 0.9, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI);
	lwait (1 ms_f);






	//Setup Force Pin
	tsTH.SetPin.pinName = OUT2;

	//Setup Threshold Test Detail
	tsTH.AwgNameR = "OUT2_R";
	tsTH.AwgNameF = "OUT2_F";
	
	apu12set(OUT2, APU12_FV, 0.7, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI);

	ThresholdTest (DSIndex, &tsTH);

	apu12set(OUT2, APU12_FV, 0.9, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI);
	lwait (1 ms_f);




	//Setup Force Pin
	tsTH.SetPin.pinName = OUT3;

	//Setup Threshold Test Detail
	tsTH.AwgNameR = "OUT3_R";
	tsTH.AwgNameF = "OUT3_F";
	
	apu12set(OUT3, APU12_FV, 0.7, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI);

	ThresholdTest (DSIndex, &tsTH);

	apu12set(OUT3, APU12_FV, 0.9, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI);
	lwait (1 ms_f);





	//Setup Force Pin
	tsTH.SetPin.pinName = OUT4;

	//Setup Threshold Test Detail
	tsTH.AwgNameR = "OUT4_R";
	tsTH.AwgNameF = "OUT4_F";
	
	apu12set(OUT4, APU12_FV, 0.7, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI);

	ThresholdTest (DSIndex, &tsTH);

	apu12set(OUT4, APU12_FV, 0.9, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI);
	lwait (1 ms_f);


	if(AP89){


		//Setup Force Pin
		tsTH.SetPin.pinName = OUT5;

		//Setup Threshold Test Detail
		tsTH.AwgNameR = "OUT5_R";
		tsTH.AwgNameF = "OUT5_F";

		apu12set(OUT5, APU12_FV, 0.7, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI);

		ThresholdTest (DSIndex, &tsTH);

		apu12set(OUT5, APU12_FV, 0.9, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI);
		lwait (1 ms_f);



		//Setup Force Pin
		tsTH.SetPin.pinName = OUT6;

		//Setup Threshold Test Detail
		tsTH.AwgNameR = "OUT6_R";
		tsTH.AwgNameF = "OUT6_F";

		apu12set(OUT6, APU12_FV, 0.7, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI);

		ThresholdTest (DSIndex, &tsTH);

		apu12set(OUT6, APU12_FV, 0.9, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI);
		lwait (1 ms_f);


	}
	else
		DSIndex+=14;


	gbOUT_COMP_TH = false;

	cbitopen(K11, K31, K33);
	cbitopen(K6, K44, K45);
	apu12set(VCC, APU12_OFF, 0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI);
	apu12set(ALL_OUT, APU12_OFF, 0.9, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI);
	apu12set(COMP, APU12_OFF, 0.8, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI);



    //End of function
    Maxim::DatalogFlush();	//Output the datalog queue.
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC
