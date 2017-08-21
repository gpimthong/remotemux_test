/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:
*		Power_Input.cpp
*
*	Written and maintained by:
*		Govit Pimthong (govit.pimthong@MaximIntegrated.com)
*
*	URL:                   $URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/Power_Input.cpp $
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

// Test Function: Power_Input
ETS_PRGFLOW_FUNC Power_Input( int DSIndex, LPCTSTR TestLabel )
{
     
    //declare variables
    PinListData pldResults;	//declare a PLD variable for the measurement results.

	//IN_UVLO_Rising
	//IN_UVLO_Falling
	//IN_UVLO_hysteresis
	//IN_UVLO_Rising-L
	//IN_UVLO_Rising-H
	//IN_UVLO_Falling-H
	//IN_UVLO_Falling-L


	dpinviset("SDA", DPIN_LOAD, 0, DPIN_8V, DPIN_512UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL);

	//Step#1
	dpinforce("DIM", "0", 3.0, 0.1, MS_ALL);
	dpinforce("SCL_IO", "0", 3.0, 0.0, MS_ALL);
	dpinforce("SDA_IO", "0", 3.0, 0.0, MS_ALL);
	lwait (1 ms_f);

	apu12set( EN, APU12_FV, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	apu12set( BSTMON, APU12_FV, 0, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
	apu12set( ALL_OUT, APU12_FV, 0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );

	apu12setcapload( IN1, APU12_FV, 0, APU12_30V, APU12_100MA, 2, APU12_PIN_TO_VI );
	apu12set( EN, APU12_OFF, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	cbitclose(K31, K27);
	lwait (4 ms_f);

	//Step#2
	apu12set( ALL_OUT, APU12_FV, 0.7, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	apu12set( BSTMON, APU12_FV, 0.7, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);


	//Step#3
	apu12setcapload( IN1, APU12_FV, 2.5, APU12_10V, APU12_100MA, 1, APU12_PIN_TO_VI );


	//*************************************************************************************************



	THForcePin SetPin = {
		IN1,
		APU12_10V,
		APU12_100MA,
		APU12_FV
	};

	THMeasPin MeasPin = {
		VCC,
		APU12_10V,
		APU12_1MA,
		APU12_MV
	};

	TestTH tsTH =
	{
		SetPin,                 // Force pin detail
		MeasPin,                // Meas pin detail
		NULL,            // Threshold set function
		VCC_MeasV,           // Threshold measure function
		"IN_UVLO_R",               // Rising AWG Name
		"IN_UVLO_F",               // Falling AWG Name
		true,                   // Use AWG
		10000,                  // MCU divider
		APU12_ABOVE_THRESH,     // Threshold direction for rising, falling will be the opposite.
		1 mV_f,                 // Step size
		1.0,        // Output transition point when rising threshold is reached.
		1.0,        // Output transition point when falling threshold is reached.
		1,                     // Sample to skip before using awgwhereat to calculate threshold (in case using AWG)
		false,                  // Find threhsold from right (not usual case)
		false,                  // True if datalong is in percentage of some REF value
		0,                      // Ref value associate with percentage.
		25,                      // Offset to use with Go-no-Go , this number will be multiply with step size.
		1.0,					//Ref value in pld format
		true					//Falling search
	};


	apu12set( VCC, APU12_FV, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );

	ThresholdTest (DSIndex, &tsTH);


    cbitopen(K31);


    //End of function
    Maxim::DatalogFlush();	//Output the datalog queue.
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC
