/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*                                                                            
*	Source title:		Power_Down.cpp                                
*		Created from Maxim Template                         
*       Template Written and maintained by:								 
*			Patrick Hayes (Patrick.Hayes@MaximIntegrated.com)	                 
*			Mark Bruening (Mark.Bruening@MaximIntegrated.com)                      
*		  
*	URL:				$URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/Power_Down.cpp $
*   Last Modified by:	$LastChangedBy: govit.pimthong $                                       
*	Date:				$Date: 2017-08-17 13:07:41 +0700 (Thu, 17 Aug 2017) $                                                
*   Current Rev:		$Revision: 68112 $
*                                                                            
*	Description:  Power down routines called from OnFailSite and OnTestCompletion.                        
*                                                                            
*   Template Revision History                                                    
*		20110907 MWB: Initial Development and Release		       
*                                                                            
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/

#include "AP89_FT.h"


/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
* Maxim_OnFailSite
*   Purpose: Add the DUT's power down sequence here. This will be called from OnFailSite in the main CPP file for the site that fails.
*	Arguments:	
*		[site] Passed in from OnFailSite 
*	Returns:
*		Return true 
*   Remarks:
*		This function is only called when: "Stop On Fail" in the MST Shell's Test Controls form is checked, and a failing test has been datalogged.
*
*   Revision History:
*		
*                                                                            
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
bool Maxim_OnFailSite( const int site )	
{
	//{{CG_OnFailSite
	//Add the chip power down sequence here.  

	//Do power down if only passed contact test.
	if (!gbContactFail && !gbSkipPowerDown){

		dpinviset("SDA", DPIN_OFF, 4, DPIN_8V, DPIN_512UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, site);

		//Step#1
		apu12setcapload( EN[site], APU12_FV, 0, APU12_10V, APU12_1MA, 1, APU12_PIN_TO_VI );
		lwait (1 ms_f);
		apu12setcapload( BSTMON[site], APU12_FV, 0, APU12_30V, APU12_1MA, 1, APU12_PIN_TO_VI );
		apu12setcapload( FSEN[site], APU12_FV, 0, APU12_10V, APU12_10MA, 1, APU12_PIN_TO_VI );
		apu12setcapload( CS[site], APU12_FV, 0, APU12_10V, APU12_1MA, 1, APU12_PIN_TO_VI );
		apu12set( CS[site], APU12_OFF, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );

		//Step#2
		apu12setcapload( OUT1[site], APU12_FV, 0, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );
		apu12setcapload( OUT2[site], APU12_FV, 0, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );
		apu12setcapload( OUT3[site], APU12_FV, 0, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );
		apu12setcapload( OUT4[site], APU12_FV, 0, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );
		apu12setcapload( OUT5[site], APU12_FV, 0, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );
		apu12setcapload( OUT6[site], APU12_FV, 0, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );
		apu12setcapload( BSTMON[site], APU12_FV, 0, APU12_30V, APU12_1MA, 1, APU12_PIN_TO_VI );
		lwait (1 ms_f);

		dpinforce("SDA_IO", "0", 3.0, 0, site);

		//Step#3
		apu12setcapload( IN1[site], APU12_FV, 0, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );

		//*************************************************************************************************
		

		cbitopen(K_POWER[site]);
		lwait (5 ms_f);


	}

	apu12set( VCC[site], APU12_OFF, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	apu12set( EN[site], APU12_OFF, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	apu12set( BSTMON[site], APU12_OFF, 0, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
	apu12set( FSEN[site], APU12_OFF, 0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	apu12set( CS[site], APU12_OFF, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	apu12set( IN1[site], APU12_OFF, 0, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
	apu12set( OUT1[site], APU12_OFF, 0, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
	apu12set( OUT2[site], APU12_OFF, 0, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
	apu12set( OUT3[site], APU12_OFF, 0, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
	apu12set( OUT4[site], APU12_OFF, 0, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
	apu12set( OUT5[site], APU12_OFF, 0, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
	apu12set( OUT6[site], APU12_OFF, 0, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
	apu12set( PEDESTAL1[site], APU12_OFF, 0, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
	apu12set( PEDESTAL2[site], APU12_OFF, 0, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
	
	dpindisconnect(site, "SCAN_OUT");
	dpindisconnect(site, "SHARED_DPU");
	dpindisconnect(site, "SDA_IO");


	return true;
	//}}CG_OnFailSite
}

/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
* Maxim_OnTestCompletion
*   Purpose: Add the DUT's power down sequence here. This will be called from OnTestCompletion in the main CPP file.
*	Arguments:	
*	Returns:
*		Return true 
*   Remarks:
*		This function is only called at the end of the program on active sites
*
*   Revision History:
*		
*                                                                            
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
bool Maxim_OnTestCompletion()
{
	//{{CG_OnTestCompletion
	//Add the chip power down sequence here.

	//Do power down if only passed contact test.
	if (!gbContactFail && !gbSkipPowerDown){

		dpinviset("SDA", DPIN_OFF, 4, DPIN_8V, DPIN_512UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL);

		//Step#1
		apu12setcapload( EN, APU12_FV, 0, APU12_10V, APU12_1MA, 1, APU12_PIN_TO_VI );
		lwait (1 ms_f);
		apu12setcapload( BSTMON, APU12_FV, 0, APU12_30V, APU12_1MA, 1, APU12_PIN_TO_VI );
		apu12setcapload( FSEN, APU12_FV, 0, APU12_10V, APU12_10MA, 1, APU12_PIN_TO_VI );
		apu12setcapload( CS, APU12_FV, 0, APU12_10V, APU12_1MA, 1, APU12_PIN_TO_VI );

		//Step#2
		apu12setcapload( OUT1, APU12_FV, 0, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );
		apu12setcapload( OUT2, APU12_FV, 0, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );
		apu12setcapload( OUT3, APU12_FV, 0, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );
		apu12setcapload( OUT4, APU12_FV, 0, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );
		apu12setcapload( OUT5, APU12_FV, 0, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );
		apu12setcapload( OUT6, APU12_FV, 0, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );
		apu12setcapload( BSTMON, APU12_FV, 0, APU12_30V, APU12_1MA, 1, APU12_PIN_TO_VI );
		lwait (1 ms_f);


		//Step#3
		apu12setcapload( IN1, APU12_FV, 0, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );

		//*************************************************************************************************
		

		cbitopen(K_POWER);
		lwait (5 ms_f);

	}

	apu12set( COMP, APU12_OFF, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );

	apu12set( VCC, APU12_OFF, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	apu12set( EN, APU12_OFF, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	apu12set( BSTMON, APU12_OFF, 0, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
	apu12set( FSEN, APU12_OFF, 0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	apu12set( CS, APU12_OFF, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	apu12set( IN1, APU12_OFF, 0, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
	apu12set( OUT1, APU12_OFF, 0, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
	apu12set( OUT2, APU12_OFF, 0, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
	apu12set( OUT3, APU12_OFF, 0, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
	apu12set( OUT4, APU12_OFF, 0, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
	apu12set( OUT5, APU12_OFF, 0, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
	apu12set( OUT6, APU12_OFF, 0, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
	apu12set( PEDESTAL1, APU12_OFF, 0, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
	apu12set( PEDESTAL2, APU12_OFF, 0, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );

	gbLastContact = false;
	gbLastSupply = false;
	gbSkipPowerDown = false;

	cbitopen(K24, K27, K28, K29, K34, K37, K40, K41, K43, K44);

	dpindisconnect(MS_ALL, "SCAN_OUT");
	dpindisconnect(MS_ALL, "SHARED_DPU");
	dpindisconnect(MS_ALL, "SDA_IO");

	return true;
	//}}CG_OnTestCompletion
}


