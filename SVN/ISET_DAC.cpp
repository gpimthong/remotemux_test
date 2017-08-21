/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:
*		ISET_DAC.cpp
*
*	Written and maintained by:
*		Govit Pimthong (govit.pimthong@MaximIntegrated.com)
*
*	URL:                   $URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/ISET_DAC.cpp $
*	Last Modified by:      $LastChangedBy: govit.pimthong $
*	Date:                  $Date: 2017-06-08 22:48:04 +0700 (Thu, 08 Jun 2017) $
*	Current Rev:           $Revision: 65489 $
*
*	Description:
*		File Description
*
*	Revision History
*
*
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
#include "AP89_FT.h"


// Test Function: ISET_DAC
ETS_PRGFLOW_FUNC ISET_DAC( int DSIndex, LPCTSTR TestLabel )
{
     
    //declare variables
    PinListData pldResults;	//declare a PLD variable for the measurement results.
     
	
	int iCH;

	for (int i=0; i<= 0xF; i++){

		iCH = 0;

		Maxim::I2C::Write(0x02, 0x20 | i);




		/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
		* Test : OUT1
		**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/


		//Setup
		apu12setcapload( OUT1, APU12_FV, 2.0, APU12_10V, APU12_200MA, 1, APU12_PIN_TO_VI );
		lwait (5 ms_f);

		pldResults = DUT::Trim::gtsOUT1.Measure();
		Maxim::DatalogAll(DSIndex, pldResults);

		//Restore
		apu12set( OUT1, APU12_FV, 0.3, APU12_10V, APU12_200MA, APU12_PIN_TO_VI );



		if(Maxim::isChar()){

			/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
			* Test : OUT2
			**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/


			//Setup
			apu12setcapload( OUT2, APU12_FV, 2.0, APU12_10V, APU12_200MA, 1, APU12_PIN_TO_VI );
			lwait (5 ms_f);

			pldResults = DUT::Trim::gtsOUT2.Measure();
			Maxim::DatalogAll(DSIndex, pldResults);

			//Restore
			apu12set( OUT2, APU12_FV, 0.3, APU12_10V, APU12_200MA, APU12_PIN_TO_VI );



			/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
			* Test : OUT3
			**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/


			//Setup
			apu12setcapload( OUT3, APU12_FV, 2.0, APU12_10V, APU12_200MA, 1, APU12_PIN_TO_VI );
			lwait (5 ms_f);

			pldResults = DUT::Trim::gtsOUT3.Measure();
			Maxim::DatalogAll(DSIndex, pldResults);

			//Restore
			apu12set( OUT3, APU12_FV, 0.3, APU12_10V, APU12_200MA, APU12_PIN_TO_VI );




			/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
			* Test : OUT4
			**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/


			//Setup
			apu12setcapload( OUT4, APU12_FV, 2.0, APU12_10V, APU12_200MA, 1, APU12_PIN_TO_VI );
			lwait (5 ms_f);

			pldResults = DUT::Trim::gtsOUT4.Measure();
			Maxim::DatalogAll(DSIndex, pldResults);

			//Restore
			apu12set( OUT4, APU12_FV, 0.3, APU12_10V, APU12_200MA, APU12_PIN_TO_VI );



			if(AP89){

				/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
				* Test : OUT5
				**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/


				//Setup
				apu12setcapload( OUT5, APU12_FV, 2.0, APU12_10V, APU12_200MA, 1, APU12_PIN_TO_VI );
				lwait (5 ms_f);

				pldResults = DUT::Trim::gtsOUT5.Measure();
				Maxim::DatalogAll(DSIndex, pldResults);

				//Restore
				apu12set( OUT5, APU12_FV, 0.3, APU12_10V, APU12_200MA, APU12_PIN_TO_VI );





				/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
				* Test : OUT6
				**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/


				//Setup
				apu12setcapload( OUT6, APU12_FV, 2.0, APU12_10V, APU12_200MA, 1, APU12_PIN_TO_VI );
				lwait (5 ms_f);

				pldResults = DUT::Trim::gtsOUT6.Measure();
				Maxim::DatalogAll(DSIndex, pldResults);

				//Restore
				apu12set( OUT6, APU12_FV, 0.3, APU12_10V, APU12_200MA, APU12_PIN_TO_VI );


			}
			else
				DSIndex+=2;

		}
		else
			DSIndex+=5;

	}



	//TST_ISETDAC
	apu12set( OUT1, APU12_FV, 2.0, APU12_10V, APU12_200MA, APU12_PIN_TO_VI );
	Maxim::I2C::Write(0x21, 0x07);
	lwait(2 ms_f);

	pldResults = DUT::Trim::gtsOUT1.Measure();
	Maxim::DatalogAll(DSIndex, pldResults);


	Maxim::I2C::Write(0x21, 0x08);
	lwait(2 ms_f);

	pldResults = DUT::Trim::gtsOUT1.Measure();
	Maxim::DatalogAll(DSIndex, pldResults);


	Maxim::I2C::Write(0x21, 0x0F);
	lwait(2 ms_f);

	pldResults = DUT::Trim::gtsOUT1.Measure();
	Maxim::DatalogAll(DSIndex, pldResults);


	Maxim::I2C::Write(0x21, 0x10);
	lwait(2 ms_f);

	pldResults = DUT::Trim::gtsOUT1.Measure();
	Maxim::DatalogAll(DSIndex, pldResults);



	Maxim::I2C::Write(0x21, 0x1F);
	lwait(2 ms_f);

	pldResults = DUT::Trim::gtsOUT1.Measure();
	Maxim::DatalogAll(DSIndex, pldResults);


	Maxim::I2C::Write(0x21, 0x20);
	lwait(2 ms_f);

	pldResults = DUT::Trim::gtsOUT1.Measure();
	Maxim::DatalogAll(DSIndex, pldResults);


	Maxim::I2C::Write(0x21, 0x3F);
	lwait(2 ms_f);

	pldResults = DUT::Trim::gtsOUT1.Measure();
	Maxim::DatalogAll(DSIndex, pldResults);



	Maxim::I2C::Write(0x21, 0x40);
	lwait(2 ms_f);

	pldResults = DUT::Trim::gtsOUT1.Measure();
	Maxim::DatalogAll(DSIndex, pldResults);




	Maxim::I2C::Write(0x21, 0x7F);
	lwait(2 ms_f);

	pldResults = DUT::Trim::gtsOUT1.Measure();
	Maxim::DatalogAll(DSIndex, pldResults);


	Maxim::I2C::Write(0x21, 0x80);
	lwait(2 ms_f);

	pldResults = DUT::Trim::gtsOUT1.Measure();
	Maxim::DatalogAll(DSIndex, pldResults);

	apu12set( OUT1, APU12_FV, 0.3, APU12_10V, APU12_200MA, APU12_PIN_TO_VI );

	Maxim::I2C::Write(0x02, 0x20);
	Maxim::I2C::Write(0x21, 0xFF);


	apu12setcapload( ALL_OUT, APU12_FV, 0.1, APU12_10V, APU12_1MA, 1, APU12_PIN_TO_VI );


    //End of function
    Maxim::DatalogFlush();	//Output the datalog queue.
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC
