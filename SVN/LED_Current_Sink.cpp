/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:
*		LED_Current_Sink.cpp
*
*	Written and maintained by:
*		Govit Pimthong (govit.pimthong@MaximIntegrated.com)
*
*	URL:                   $URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/LED_Current_Sink.cpp $
*	Last Modified by:      $LastChangedBy: govit.pimthong $
*	Date:                  $Date: 2017-08-09 18:29:28 +0700 (Wed, 09 Aug 2017) $
*	Current Rev:           $Revision: 67837 $
*
*	Description:
*		File Description
*
*	Revision History
*
*
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
#include "AP89_FT.h"


void LED_ON(LPCTSTR some_name, int some_int){
	
	Maxim::I2C::Write(0x02, 0x34); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x02, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	lwait(1 ms_f);

	Maxim::I2C::Write(0x02, 0x30); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x02, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	lwait(1 ms_f);

}

void LED_OFF(LPCTSTR some_name, int some_int){

	Maxim::I2C::Write(0x02, 0x30); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x02, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	lwait(1 ms_f);

	Maxim::I2C::Write(0x02, 0x34); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x02, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	lwait(1 ms_f);

}

void LED_Current_RiseFall(int &DSIndex, int relay){

	PinListData pldResults;	

	double dStart = 2.1; //90%
	double dStop = 0.5;  //10%


	//Setup
	cbitclose(relay);
	lwait (2 ms_f);
		
	//Turn On LED Sink
	Maxim::I2C::Write(0x02, 0x34); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x02, 0xA0); //write DHR<15:0> to DCTLR<15:0>
	lwait(1 ms_f);

	//Falling
	pldResults = MeasureTime(QTMU_GP, din1, din1, "AUTO", 1, LED_OFF, "", 1 ms_f, QTMU_RCVL, QTMU_RCVH, QTMU_POS, QTMU_POS, 0, 0, dStop, dStart, 10, 10);
	
	Maxim::DatalogAll(DSIndex, pldResults * us);	
	
	//Turn Off LED Sink
	Maxim::I2C::Write(0x02, 0x30); //write data to DHR2<11:8>
	Maxim::I2C::Write(0x02, 0xA0); //write DHR<15:0> to DCTLR<15:0>

	//Rising
	pldResults = MeasureTime(QTMU_GP, din1, din1, "AUTO", 1, LED_ON, "", 1 ms_f, QTMU_RCVH, QTMU_RCVL, QTMU_NEG, QTMU_NEG, 0, 0, dStart, dStop, 10, 10);
		
	Maxim::DatalogAll(DSIndex, pldResults * us);	
		
	cbitopen(relay);


}



// Test Function: LED_Current_Sink
ETS_PRGFLOW_FUNC LED_Current_Sink( int DSIndex, LPCTSTR TestLabel )
{
     
    //declare variables
	PinListData pldResults;	//declare a PLD variable for the measurement results.
	PinListData pldIOUTMatch[6];
	PinListData pldIOUT[4][6];
	PinListData pldIOUTMin, pldIOUTMax, pldIOUTAvg;
	int iDelay = 5;

	//if(Maxim::isChar())
	//	iDelay = 100;

	//Setup
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
	Maxim::I2C::Write(0x01, 0x10); //write data to DHR0<3:0>
	Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>




	apu12setcapload( ALL_OUT, APU12_FV, 0.3, APU12_10V, APU12_200MA, 1, APU12_PIN_TO_VI );

	//Write '0x02' to TEST REG (tm_iset test mode)
	Maxim::I2C::Write(AP89_AD, 0x20, 0x02);

	//Write '0xFF' to TEST_ISETDAC REG (ISET value to be programmed in tm_iset test mode)
	Maxim::I2C::Write(AP89_AD, 0x21, 0xFF);



	//IREF voltage
	apu12set( FSEN, APU12_FV, 0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );
	cbitclose(K15);
	apu12set( FSEN, APU12_FI, 0, APU12_3p6V, APU12_10UA, APU12_PIN_TO_VI );
	lwait(3 ms_f);


	pldResults = apu12mv(FSEN, 100);
	Maxim::DatalogAll(DSIndex, pldResults);


	apu12set( FSEN, APU12_FV, 0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );
	cbitopen(K15);
	lwait(2 ms_f);
	apu12set( FSEN, APU12_FV, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );

	int iCH;

	for (int i=0; i< 4; i++){

		iCH = 0;

		if(i==0){	//Write '0x20' to iset REG (45mA Sink Current)

			if(Maxim::isChar()){
				Maxim::I2C::Write(AP89_AD, 0x02, 0x20);
			}
			else{
				DSIndex+=6;
				i=1;
			}
		}


		if(i==1)	//Write '0x21' to iset REG (50mA Sink Current)
			Maxim::I2C::Write(AP89_AD, 0x02, 0x21);

		if(i==2)	//Write '0x2B' to iset REG (100mA Sink Current)
			Maxim::I2C::Write(AP89_AD, 0x02, 0x2B);

		if(i==3)	//Write '0x2F' to iset REG (120mA Sink Current)
			Maxim::I2C::Write(AP89_AD, 0x02, 0x2F);




		/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
		* Test : OUT1
		**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/


		//Setup
		apu12setcapload( OUT1, APU12_FV, 2.0, APU12_10V, APU12_200MA, 1, APU12_PIN_TO_VI );
		lwait (iDelay ms_f);

		pldIOUT[i][iCH++] = pldResults = DUT::Trim::gtsOUT1.Measure();
		Maxim::DatalogAll(DSIndex, pldResults);

		//Restore
		apu12set( OUT1, APU12_FV, 0.3, APU12_10V, APU12_200MA, APU12_PIN_TO_VI );



		/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
		* Test : OUT2
		**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/


		//Setup
		apu12setcapload( OUT2, APU12_FV, 2.0, APU12_10V, APU12_200MA, 1, APU12_PIN_TO_VI );
		lwait (iDelay ms_f);

		pldIOUT[i][iCH++] = pldResults = DUT::Trim::gtsOUT2.Measure();
		Maxim::DatalogAll(DSIndex, pldResults);

		//Restore
		apu12set( OUT2, APU12_FV, 0.3, APU12_10V, APU12_200MA, APU12_PIN_TO_VI );



		/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
		* Test : OUT3
		**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/


		//Setup
		apu12setcapload( OUT3, APU12_FV, 2.0, APU12_10V, APU12_200MA, 1, APU12_PIN_TO_VI );
		lwait (iDelay ms_f);

		pldIOUT[i][iCH++] = pldResults = DUT::Trim::gtsOUT3.Measure();
		Maxim::DatalogAll(DSIndex, pldResults);

		//Restore
		apu12set( OUT3, APU12_FV, 0.3, APU12_10V, APU12_200MA, APU12_PIN_TO_VI );




		/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
		* Test : OUT4
		**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/


		//Setup
		apu12setcapload( OUT4, APU12_FV, 2.0, APU12_10V, APU12_200MA, 1, APU12_PIN_TO_VI );
		lwait (iDelay ms_f);

		pldIOUT[i][iCH++] = pldResults = DUT::Trim::gtsOUT4.Measure();
		Maxim::DatalogAll(DSIndex, pldResults);

		//Restore
		apu12set( OUT4, APU12_FV, 0.3, APU12_10V, APU12_200MA, APU12_PIN_TO_VI );



		if(AP89){

			/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
			* Test : OUT5
			**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/


			//Setup
			apu12setcapload( OUT5, APU12_FV, 2.0, APU12_10V, APU12_200MA, 1, APU12_PIN_TO_VI );
			lwait (iDelay ms_f);

			pldIOUT[i][iCH++] = pldResults = DUT::Trim::gtsOUT5.Measure();
			Maxim::DatalogAll(DSIndex, pldResults);

			//Restore
			apu12set( OUT5, APU12_FV, 0.3, APU12_10V, APU12_200MA, APU12_PIN_TO_VI );





			/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
			* Test : OUT6
			**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/


			//Setup
			apu12setcapload( OUT6, APU12_FV, 2.0, APU12_10V, APU12_200MA, 1, APU12_PIN_TO_VI );
			lwait (iDelay ms_f);

			pldIOUT[i][iCH++] = pldResults = DUT::Trim::gtsOUT6.Measure();
			Maxim::DatalogAll(DSIndex, pldResults);

			//Restore
			apu12set( OUT6, APU12_FV, 0.3, APU12_10V, APU12_200MA, APU12_PIN_TO_VI );


		}
		else
			DSIndex+=2;

			Maxim::I2C::Write(AP89_AD, 0x02, 0x20);

	}



	for(int i =0 ; i<4; i++){

		if(i==0){		
			if(!Maxim::isChar()){
				i = 1;
				DSIndex+=9;
			}
		}

		pldIOUTMin = PinListData(1e6);
		pldIOUTMax = PinListData(0);
		pldIOUTAvg = PinListData(0);

		//Find Min&Max and Avg
		for(int j=0; j<giLED_CH; j++){

			for(int isite=0; isite<NUM_SITES; isite++){
				pldIOUTMin[isite] = (pldIOUT[i][j][isite] < pldIOUTMin[isite])?pldIOUT[i][j][isite]:pldIOUTMin[isite];
				pldIOUTMax[isite] = (pldIOUT[i][j][isite] > pldIOUTMax[isite])?pldIOUT[i][j][isite]:pldIOUTMax[isite];
			}

			pldIOUTAvg = pldIOUTAvg + pldIOUT[i][j];

		}
				
		pldIOUTAvg = pldIOUTAvg / double(giLED_CH);
		
		Maxim::DatalogAll(DSIndex, pldIOUTMin);
		Maxim::DatalogAll(DSIndex, pldIOUTMax);
		Maxim::DatalogAll(DSIndex, pldIOUTAvg);


		for(int j=0; j<giLED_CH; j++){

			pldIOUTMatch[j] = (pldIOUT[i][j] - pldIOUTAvg) / pldIOUTAvg;
			Maxim::DatalogAll(DSIndex, pldIOUTMatch[j] * 100);

		}

		if(!AP89) DSIndex+=2;

	}



	if(Maxim::isChar()){

		apu12set( ALL_OUT, APU12_FV, 0.3, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
		apu12set( ALL_OUT, APU12_OFF, 0.3, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
		cbitclose(K9, K10, K11, K12, K13, K14);

		//Power up 5V DC
		cbitclose(K24, K46, K43);



		int iSink = 0x0B;


		// Code : 0, Current = 45 mA
		// Code : 1, Current = 50 mA
		// Code : 2, Current = 55 mA
		// Code : 3, Current = 60 mA
		// Code : 4, Current = 65 mA
		// Code : 5, Current = 70 mA
		// Code : 6, Current = 75 mA
		// Code : 7, Current = 80 mA
		// Code : 8, Current = 85 mA
		// Code : 9, Current = 90 mA
		// Code : A, Current = 95 mA
		// Code : B, Current = 100 mA
		// Code : C, Current = 105 mA
		// Code : D, Current = 110 mA
		// Code : E, Current = 115 mA
		// Code : F, Current = 120 mA
		Maxim::I2C::Write(AP89_AD, 0x02, 0x2 | iSink);


		Maxim::I2C::SetSlaveAddress(TM_3DV_AD);

		Maxim::I2C::Write(0x02, 0x34); //write data to DHR2<11:8>
		Maxim::I2C::Write(0x02, 0xA0); //write DHR<15:0> to DCTLR<15:0>
		lwait(20 ms_f); //10ms is not enough for release mode.

		//OUT1
		LED_Current_RiseFall(DSIndex, K52);

		//OUT2
		LED_Current_RiseFall(DSIndex, K54);

		//OUT3
		LED_Current_RiseFall(DSIndex, K56);

		//OUT4
		LED_Current_RiseFall(DSIndex, K58);

		//OUT5
		LED_Current_RiseFall(DSIndex, K60);

		//OUT6
		LED_Current_RiseFall(DSIndex, K62);


		Maxim::I2C::Write(0x02, 0x34); //write data to DHR2<11:8>
		Maxim::I2C::Write(0x02, 0xA0); //write DHR<15:0> to DCTLR<15:0>

		//Restore
		Maxim::I2C::Write(AP89_AD, 0x02, 0x20);
		cbitopen(K24, K46, K43);
		cbitopen(K9, K10, K11, K12, K13, K14);
		lwait(4 ms_f);
		apu12set( ALL_OUT, APU12_FV, 0.3, APU12_10V, APU12_200MA, APU12_PIN_TO_VI );

	}

    //End of function
    Maxim::DatalogFlush();	//Output the datalog queue.
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC
