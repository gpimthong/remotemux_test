/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:
*		DIAG.cpp
*
*	Written and maintained by:
*		Govit Pimthong (govit.pimthong@MaximIntegrated.com)
*
*	URL:                   $URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/DIAG.cpp $
*	Last Modified by:      $LastChangedBy: govit.pimthong $
*	Date:                  $Date: 2017-08-03 22:36:37 +0700 (Thu, 03 Aug 2017) $
*	Current Rev:           $Revision: 67633 $
*
*	Description:
*		File Description
*
*	Revision History
*
*
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
#include "AP89_FT.h"



// Test Function: DIAG
ETS_PRGFLOW_FUNC DIAG( int DSIndex, LPCTSTR TestLabel )
{
     
    //declare variables
    PinListData pldResults;	//declare a PLD variable for the measurement results.
	SiteInt siIREFOOR, siHWRST, siOV;


	cbitclose(K40, K41);	
	apu12setcapload( IN1, APU12_FV, 12, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );
	apu12set( BSTMON, APU12_FV, 0.7, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );
	apu12set( ALL_OUT, APU12_FV, 0.7, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	apu12set( EN, APU12_FV, 3, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );
	
	////*************************************************************************************************

	lwait (2 ms_f);

	//Write '1' to ENA bit REG02<5>
	dpinviset("SDA", DPIN_LOAD, 4, DPIN_8V, DPIN_512UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL);
	dpinforce("DIM", "1", 3.0, 0.1, MS_ALL); //DIM at 5V
	Maxim::I2C::Write(AP89_AD, 0x02, 0x20);
	lwait (2 ms_f);

	int iADD, iReg;

	iADD = AP89_AD;
	iReg = 0;


	DSIndex+=18;


	//DIAG 0x1F
	Maxim::I2C::Read(AP89_AD,0x1F,gsiReadBack);

	siHWRST = gsiReadBack;

	//Expect 0 for IREFOOR	and 1 for HWRST
	ForEachActive_iSite{
		siIREFOOR[iSite] = gsiReadBack[iSite] & 0x20;
		siHWRST[iSite] = gsiReadBack[iSite] & 0x04;
		siOV[iSite] = gsiReadBack[iSite] & 0x08;
	}

	//Create over voltage at IREF
	cbitclose(K15);
	lwait(3 ms_f);
	apu12set( BSTMON, APU12_FV, 3, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );
	apu12set( FSEN, APU12_FV, 1.5, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );
	lwait(1 ms_f);
	

	Maxim::I2C::Read(AP89_AD,0x1F,gsiReadBack);


	//Expect 1 for IREFOOR	and 0 for HWRST
	ForEachActive_iSite{
		siIREFOOR[iSite] = ((gsiReadBack[iSite] & 0x20) - siIREFOOR[iSite]) >> 5;
		siHWRST[iSite] = ((siHWRST[iSite] - gsiReadBack[iSite] & 0x04)) >> 2;
		siOV[iSite] = ((siOV[iSite] - gsiReadBack[iSite] & 0x08)) >> 3;
		gsiBSTUV[iSite] = (gsiBSTUV[iSite] - (gsiReadBack[iSite] & 0x10)) >> 4;
	}


	Maxim::DatalogAll(DSIndex, siIREFOOR);
	Maxim::DatalogAll(DSIndex, gsiBSTUV);
	Maxim::DatalogAll(DSIndex, siOV);

	Maxim::DatalogAll(DSIndex, siHWRST);
	
	
	//Restore
	apu12set( FSEN, APU12_FV, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	apu12set( BSTMON, APU12_FV, 0.7, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );
	lwait(1 ms_f);
	cbitopen(K15);





	DUT::Initiate_TestMode();

	Maxim::I2C::SetSlaveAddress(OTP_3DV_AD);

	Maxim::I2C::Write(UNLOCK1_REG, 0x17); //Unlock passwd0 = 0x17
	Maxim::I2C::Write(UNLOCK2_REG, 0x65); //Unlock passwd1 = 0x65

	Maxim::I2C::Write(OTPMODE_REG, 0x00); //Turn off Look and bypass
	
	

     
    //End of function
    Maxim::DatalogFlush();	//Output the datalog queue.
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC
