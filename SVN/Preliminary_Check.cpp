/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:
*		Preliminary_Check.cpp
*
*	Written and maintained by:
*		Govit Pimthong (govit.pimthong@MaximIntegrated.com)
*
*	URL:                   $URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/Preliminary_Check.cpp $
*	Last Modified by:      $LastChangedBy: govit.pimthong $
*	Date:                  $Date: 2017-08-02 02:25:56 +0700 (Wed, 02 Aug 2017) $
*	Current Rev:           $Revision: 67532 $
*
*	Description:
*		File Description
*
*	Revision History
*
*
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
#include "AP89_FT.h"



// Test Function: Preliminary_Check
ETS_PRGFLOW_FUNC Preliminary_Check( int DSIndex, LPCTSTR TestLabel )
{



	//declare variables
	PinListData pldResults;	//declare a PLD variable for the measurement results.
	gbContactFail = false;


	/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
	* Test : I_Standby_Check
	* Test No : #12000
	* Description : IN = 30V , EN = 0V, Measure supply current
	* Contour Link : https://contour.maxim-ic.com/contour//perspective.req?projectId=1841&docId=6729673
	**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/




	//Powering up (https://contour.maxim-ic.com/contour//perspective.req?projectId=1841&docId=7679744)

	//Step#1
	apu12set( IN1, APU12_FV, 0, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
	apu12set( EN, APU12_FV, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	apu12set( BSTMON, APU12_FV, 0, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
	apu12set( FSEN, APU12_FV, 0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	apu12set( CS, APU12_FV, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	apu12set( ALL_OUT, APU12_FV, 0, APU12_10V, APU12_200MA, APU12_PIN_TO_VI );

	cbitclose(K40, K41, K34, K37);
	lwait (1 ms_f);

	dpinforce("DIM", "1", 3.0, 0.1, MS_ALL); //DIM at 5V
	dpinforce("SCL_IO", "0", 3.0, 0.0, MS_ALL);
	dpinforce("SDA_IO", "0", 3.0, 0.0, MS_ALL);
	lwait (1 ms_f);


	//Step#2
	apu12set( ALL_OUT, APU12_FV, 0.7, APU12_10V, APU12_200MA, APU12_PIN_TO_VI );
	apu12set( BSTMON, APU12_FV, 0.7, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);


	//Step#3
	apu12setcapload( IN1, APU12_FV, 30, APU12_30V, APU12_100UA, 1, APU12_PIN_TO_VI );

	//*************************************************************************************************


	////Check for 49.9K resistor at IREF to make sure it's within 0.1% range.
	//PinListData pldIREF_v, pldIREF_i, pldIREF_R;


	//cbitclose(K15);
	//lwait(3 ms_f);

	//apu12set( FSEN, APU12_FV, 1, APU12_3p6V, APU12_100UA, APU12_PIN_TO_VI );
	//lwait(100 ms_f);

	//pldIREF_v = apu12mv(FSEN, 100);
	//pldIREF_i = apu12mi(FSEN, APU12_MI_1X, 200);

	//cbitopen(K34);
	//lwait (100 ms_f);

	//pldIREF_i = pldIREF_i - apu12mi(FSEN, APU12_MI_1X, 200);


	//pldIREF_R = (pldIREF_v / pldIREF_i);

	//

	//apu12set( FSEN, APU12_FV, 0, APU12_3p6V, APU12_100UA, APU12_PIN_TO_VI );
	//apu12set( FSEN, APU12_FV, 0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	//cbitopen(K15);
	//cbitclose(K34);

	//******************************************************************************


	lwait (30 ms_f);



	pldResults = apu12mi(IN1, APU12_MI_10X, 10);
	Maxim::DatalogAll(DSIndex, pldResults * uA);




	/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
	* Test : VCC_Check
	* Test No : #12010
	* Description : IN = 30V, EN =5V, Measure VCC output voltage with 10mA Load
	* Contour Link : https://contour.maxim-ic.com/contour//perspective.req?projectId=1841&docId=6729674
	**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
	SiteInt siAck;
	PinListData pldStartUpDelay;
	double dTime1,dTime2;
	int iReg = 2;
	SiteBool sbStartUpDone;
	

	dTime1 = -1.0;	
	sbStartUpDone = true;


	apu12setcapload( IN1, APU12_FV, 30, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );

	dpinviset("SDA", DPIN_LOAD, 4, DPIN_8V, DPIN_512UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL);
	lwait (3 ms_f);	

	Maxim::I2C::SetSlaveAddress(AP89_AD);

	ForEachActive_iSite{
		sbStartUpDone[iSite] = false;
	}
	

	apu12set( EN, APU12_FV, 2, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	lwait(100 us_f);
	mark(2);
	lwait(1500 us_f);


	while(sbStartUpDone.Any(false)){

		dTime1 = ltime(2);
		siAck = Maxim::I2C::CheckSlaveAddress();
		dTime2 = ltime(2);

		ForEachActive_iSite{

			if(!siAck[iSite] && !sbStartUpDone[iSite]){
				pldResults[iSite] = dTime1;
				sbStartUpDone[iSite] = true;
			}
			else{
				sbStartUpDone[iSite] = false;
			}

		}

		//Fail safe
		if(dTime1 > 5000)
			sbStartUpDone = true;

	}

	pldStartUpDelay = pldResults;

	//Write '1' to ENA bit REG02<5>, expect '0'
	siAck = Maxim::I2C::Write(iReg, 0x20);


	apu12set( VCC, APU12_FV, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	cbitclose(K28);	
	lwait (2 ms_f);

	pldResults = apu12mv(VCC, 10);
	Maxim::DatalogAll(DSIndex, pldResults);


	
	Maxim::DatalogAll(DSIndex, pldStartUpDelay * 0.001);



	//Restore
	apu12setcapload( EN, APU12_FV, 0, APU12_10V, APU12_1MA, 0.5, APU12_PIN_TO_VI );
	lwait (1 ms_f);
	apu12set( VCC, APU12_OFF, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	apu12setcapload( IN1, APU12_FV, 12, APU12_30V, APU12_100MA, 0.5, APU12_PIN_TO_VI );


	//Maxim::DatalogAll(DSIndex, pldIREF_R);

	//End of function
	Maxim::DatalogFlush();	//Output the datalog queue.
	return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC
