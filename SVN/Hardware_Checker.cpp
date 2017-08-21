/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*                                                                            
*	Source title:		Hardware_Checker.cpp                                
*		Created from Maxim Template                         
*		Template Written and maintained by:								 
*			Patrick Hayes (Patrick.Hayes@MaximIntegrated.com)	                 
*			Mark Bruening (Mark.Bruening@MaximIntegrated.com)                      
*		  
*	URL:				$URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/Hardware_Checker.cpp $
*	Last Modified by:	$LastChangedBy: kiratiya.kaewpaluek $                                       
*	Date:				$Date: 2017-07-07 11:11:20 +0700 (Fri, 07 Jul 2017) $                                                
*	Current Rev:		$Revision: 66611 $
*                                                                            
*	Description:  Hardware Checker function template.                            
*                                                                            
*	Template Revision History                                                    
*		20110907 MWB: Initial Development and Release
*		20111102 MWB: Cleaned up HWChecker scheme by separating code that need to be edited by the user into new functions.
*
*	User Revision History                                                    
*                                                                            
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/

#include "AP89_FT.h"


void SmallCapTest(int &DSIndex, int pin, double dCapValue, int relay = 0, double dExpectedV = 5){

	PinListData pldResults;

	double dCurrent;	//mA
	ULONG uChargeTime;	//ms

	uChargeTime = 10;  //ms

	dCurrent = dCapValue * dExpectedV / uChargeTime;


	//Discharge
	apu12set( pin, APU12_FV, 0.0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	lwait (5 ms_f);

	//Charging Cap with constant current
	apu12set( pin, APU12_FI, dCurrent, APU12_10V, APU12_1MA, APU12_PIN_TO_VI);
	lwait(uChargeTime ms_f);

	pldResults = apu12mv(pin, 5, 13.0);

	//restore settings
	apu12setcapload(pin, APU12_FV, 0.0, APU12_10V, APU12_10MA, 1, APU12_PIN_TO_VI);


	Maxim::DatalogAll(DSIndex, pldResults);

}


void LargeCapTest(int &DSIndex, int pin, double dCapValue, int relay = 0, double dExpectedV = 10){

	PinListData pldResults;

	double dCurrent;	//mA
	ULONG uChargeTime;	//ms

	uChargeTime = 20;  //ms

	dCurrent = dCapValue * dExpectedV / uChargeTime;


	cbitclose(relay);

	//Discharge
	apu12set( pin, APU12_FV, 0.0, APU12_30V, APU12_10MA, APU12_PIN_TO_VI );
	lwait (5 ms_f);

	//Charging Cap with constant current
	apu12set( pin, APU12_FI, dCurrent, APU12_30V, APU12_10MA, APU12_PIN_TO_VI);
	lwait(uChargeTime ms_f);

	pldResults = apu12mv(pin, 5, 13.0);
	
	//restore settings
	apu12setcapload(pin, APU12_FV, 0.0, APU12_30V, APU12_10MA, 1, APU12_PIN_TO_VI);
	lwait (5 ms_f);
	cbitopen(relay);
	apu12set(pin, APU12_FV, 0.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI);

	Maxim::DatalogAll(DSIndex, pldResults);

}


void OUTxLeakTest(int &DSIndex, int pin){

	PinListData pldResults;

	apu12set( pin, APU12_FV, 0.0, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
	lwait(200 us_f);
	apu12setcapload( pin, APU12_FV, 30.0, APU12_30V, APU12_1MA, 5, APU12_PIN_TO_VI );
	lwait(100 ms_f);	
	apu12set( pin, APU12_FV, 30.0, APU12_30V, APU12_100UA, APU12_PIN_TO_VI );	
	lwait (100 ms_f);

	pldResults = apu12mi(pin, APU12_MI_1X, 100);
	Maxim::DatalogAll(DSIndex, pldResults * uA);

	apu12set( pin, APU12_FV, 0.0, APU12_30V, APU12_100UA, APU12_PIN_TO_VI );
	lwait (1 ms_f);
	apu12set( pin, APU12_FV, 0.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );

}


void NDRV_toggle(LPCTSTR some_name, int some_int){

	apu12set( NDRV, APU12_FV, 5.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	lwait (2 ms_f);
	apu12set( NDRV, APU12_FV, 0.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);
	apu12set( NDRV, APU12_FV, 5.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	lwait (2 ms_f);
	apu12set( NDRV, APU12_FV, 0.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	lwait (10 ms_f);

}

void OUT1_toggle(LPCTSTR some_name, int some_int){

	apu12set( OUT1, APU12_FV, 5.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	lwait (2 ms_f);
	apu12set( OUT1, APU12_FV, 0.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);
	apu12set( OUT1, APU12_FV, 5.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	lwait (2 ms_f);
	apu12set( OUT1, APU12_FV, 0.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	lwait (10 ms_f);

}


/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
* Check_For_DUT
*   Purpose: Check for DUT contact (diode) to verify that a DUT is not in the socket before starting the HW checker.
*	Arguments:	
*		[DSIndex]  DataSheet Index location passed in from the ETS Program Flow
*	Returns:
*		[DSIndex]  Updated DSIndex is returned.
*   Remarks:
*		This test is used to implement the Best Practice described at https://confluence.maxim-ic.com/display/TE/Hardware+checker. 
*		This function is called by the Maxim Library.
*		This function must datalog at least one result.  Use one of the Maxim::Datalog functions to store the result. 
*			The result will be checked in the calling function.  Do not flush the datalog in this function, it is done by the calling function.
*
*   Revision History:
*		
*       
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
void Check_For_DUT(int & DSIndex)
{	
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// { 	  Implement test(s) to verify that no DUT was accidentally left in the socket of any active site.  (Replace with code for your product)
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+

	// { Sample Code


	PinListData pldResults;

	alarmset( ALARM_CLAMP, OFF );  //Turn off current clamp alarm that would occur if a diode is present and the APU is forcing -2V.
	gbSkipPowerDown = true;
	cbitclose(K_POWER);

	//----- Ground all pins before doing continuity
	apu12set( ALL_APU, APU12_FV, 0.0, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );
	lwait( 1 ms_f );


	apu12set( OUT1, APU12_FI, 0.0 mA_f, APU12_3p6V, APU12_100UA, APU12_PIN_TO_VI );
	apu12set( OUT2, APU12_FI, 0.0 mA_f, APU12_3p6V, APU12_100UA, APU12_PIN_TO_VI );
	lwait( 50 us_f );
	apu12set( OUT1, APU12_FV, -2.0 V_f, APU12_3p6V, APU12_100UA, APU12_PIN_TO_VI );	//Force to -2V on 100uA current range to limit the voltage and current at the same time.
	apu12set( OUT2, APU12_FV, -2.0 V_f, APU12_3p6V, APU12_100UA, APU12_PIN_TO_VI );	//Force to -2V on 100uA current range to limit the voltage and current at the same time.
	lwait( 3 ms_f );

	pldResults = apu12mv( OUT1, 10, 13.0 );	//Measure for a diode drop.  Expect -2V as a part should not be in the socket at this time.
	Maxim::DatalogAll(DSIndex, pldResults * V);	//Datalog.  Make sure to use proper limits on the DataSheet.  Expect -2V.

	pldResults = apu12mv( OUT2, 10, 13.0 );	//Measure for a diode drop.  Expect -2V as a part should not be in the socket at this time.
	Maxim::DatalogAll(DSIndex, pldResults * V);	//Datalog.  Make sure to use proper limits on the DataSheet.  Expect -2V.

	//--- Turn off the resource
	apu12set( OUT1, APU12_FV, 0.0 V_f, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );
	apu12set( OUT2, APU12_FV, 0.0 V_f, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );
	lwait( 3 ms_f );	//Discharge any capacitance

	apu12set( ALL_APU, APU12_OFF, 0.0, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );

	alarmset( ALARM_CLAMP, ON );  //---- Turn alarms back on	
	

	cbitopen(K_POWER);

	// } Sample Code

	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// } 	  End of test.  (Replace with code for your product)
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+

}


/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
* Hardware_Checker
*   Purpose: User supplied function to run Hardware Checker code.
*	Arguments:	
*		[DSIndex]  DataSheet Index location passed in from the ETS Program Flow
*	Returns:
*		[DSIndex]  Updated DSIndex is returned.
*   Remarks:
*		This function is called by the Maxim Library.
*		This function must datalog at least one result.  Use one of the Maxim::Datalog functions to store the result. 
*			The result will be checked in the calling function.  Do not flush the datalog in this function, it is done by the calling function.
*
*		To run the hardware checker, set the "Test_Step" General Purpose String in the MST Shell to "HW_CHECKER".
*			Or leave the "Test_Step" string blank and select "HW_CHECKER" from the option list when the program runs.
*		The Maxim Loader will automatically enforce running the checker for production.
*
*		The Checker Operator prompts can be avoided by setting the "HW_Checker_Message" General Purpose Switch in the MST Shell to False.
*		This is useful when collecting Gage Study data for the Checker.
*
*		DO NOT DISABLE THE HARDWARE_CHECKER FUNCTION IN THE DATASHEET
*			The checker will only run when the test step is "HW_CHECKER".   
*			Do not add code to bypass the contents of this function for other test steps.  The library already takes care of that.
*
*   Revision History:
*		
*                                                                            
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
void Hardware_Checker(int & DSIndex)
{
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// {		Start of HW Checker.  (Edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	alarmset( ALARM_CLAMP, OFF );  //Turn off current clamp alarm that would occur if a diode is present and the APU is forcing -2V.


	PinListData pldResults, pldResultsI, pldResultsV, pldResults2;
	gbSkipPowerDown = true;


	cbitclose(K_POWER);



	apu12set( ALL_APU, APU12_FV, 0.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	lwait( 1 ms_f );





	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// {		BLOCK : Bypass CAP
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+

	//IN 0.1uF
	SmallCapTest(DSIndex, IN1, 0.1); //Test#120

	//IN 4.7uF (+0.1uF)
	LargeCapTest(DSIndex, IN1, 4.8, K27, 10); //Test#130



	//VCC 0.1uF
	SmallCapTest(DSIndex, VCC, 0.1); //Test#140

	//VCC 1.0uF (+0.1uF)
	LargeCapTest(DSIndex, VCC, 1.1, K28); //Test#150



	//OUT1 0.1uF
	SmallCapTest(DSIndex, OUT1, 0.1); //Test#160

	//OUT1 1.0uF (+0.1uF)
	LargeCapTest(DSIndex, OUT1, 1.1, K29); //Test#170



	//OUT2 0.1uF
	SmallCapTest(DSIndex, OUT2, 0.1); //Test#180

	//OUT2 1.0uF (+0.1uF)
	LargeCapTest(DSIndex, OUT2, 1.1, K29); //Test#190



	//OUT3 0.1uF
	SmallCapTest(DSIndex, OUT3, 0.1); //Test#200

	//OUT3 1.0uF (+0.1uF)
	LargeCapTest(DSIndex, OUT3, 1.1, K29); //Test#210



	//OUT4 0.1uF
	SmallCapTest(DSIndex, OUT4, 0.1); //Test#220

	//OUT4 1.0uF (+0.1uF)
	LargeCapTest(DSIndex, OUT4, 1.1, K29); //Test#230



	//OUT5 0.1uF
	SmallCapTest(DSIndex, OUT5, 0.1); //Test#240

	//OUT5 1.0uF (+0.1uF)
	LargeCapTest(DSIndex, OUT5, 1.1, K30); //Test#250



	//OUT6 0.1uF
	SmallCapTest(DSIndex, OUT6, 0.1); //Test#260

	//OUT6 1.0uF (+0.1uF)
	LargeCapTest(DSIndex, OUT6, 1.1, K30); //Test#270




	//PEDESTAL1 4.7uF
	LargeCapTest(DSIndex, PEDESTAL1, 4.7, K16); //Test#280


	//PEDESTAL2 4.7uF
	LargeCapTest(DSIndex, PEDESTAL2, 4.7, K17); //Test#290



	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// }		END 
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+


	



	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// {		BLOCK : EN (pin24) , K31, Connect IN and EN
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+

	apu12set( EN, APU12_FV, 0.0, APU12_30V, APU12_10UA, APU12_PIN_TO_VI );
	apu12set( IN1, APU12_FV, 5, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );	
	cbitclose(K31);	
	lwait (3 ms_f);

	//Relay close
	pldResults = apu12mv(EN, 10);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#300

	cbitopen(K31);
	lwait (5 ms_f);

	//Relay open
	pldResults = apu12mv(EN, 10);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#310

	apu12set( IN1, APU12_FV, 0.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);
	apu12setcapload( EN, APU12_FV, 30, APU12_30V, APU12_10UA, 1, APU12_PIN_TO_VI );
	lwait (10 ms_f);

	//Leakage check at EN pin
	pldResults = apu12mi(EN, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResults * uA); //Test#320

	apu12setcapload( EN, APU12_FV, 0, APU12_30V, APU12_10UA, 1, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	apu12set( EN, APU12_FV, 0.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );

	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// }		END 
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+







	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// {		BLOCK : PGATE (pin2) (K14)
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	
	cbitclose(K14); // APU12-to-PGATE
	cbitclose(K60,K62); // OUT5 and OUT6 to BUSA
	lwait (2 ms_f);

	apu12set( PGATE, APU12_FV, 0.0, APU12_30V, APU12_10UA, APU12_PIN_TO_VI );
	apu12setcapload( PGATE, APU12_FV, 30.0, APU12_30V, APU12_10UA, 1, APU12_PIN_TO_VI );
	lwait (10 ms_f);

	pldResults = apu12mi(PGATE, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResults * uA); //Test#330

	apu12setcapload( PGATE, APU12_FV, 0.0, APU12_30V, APU12_10UA, 1, APU12_PIN_TO_VI );
	lwait (1 ms_f);

	cbitopen(K14, K60, K62);
	lwait(1 ms_f);
	apu12set( PGATE, APU12_FV, 0.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );


	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// }		END 
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+









	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// {		BLOCK : FLT (Pin6)
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+

	apu12set( FLT, APU12_FI, 0.0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );
	cbitclose(K6); // APU4-to-FLT
	cbitclose(K32); // FLT-to-500Ohm pull-up
	lwait(5 ms_f);

	pldResultsV = apu12mv(FLT, 10);
	Maxim::DatalogAll(DSIndex, pldResultsV); //Test#340

	apu12set( FLT, APU12_FV, 0.0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	lwait (10 ms_f);

	pldResultsI = apu12mi(FLT, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResultsI); //Test#350

	apu12set( FLT, APU12_FI, 0.0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	lwait (3 ms_f);

	//Calculate R pull-up
	pldResults = 1000 * pldResultsV / (pldResultsI + 1e-12);
	Maxim::DatalogAll(DSIndex, abs(pldResults)); //Test#360


	//Check leakage
	cbitopen(K32);
	lwait (2 ms_f);
	apu12set( FLT, APU12_FV, 0.0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );
	apu12setcapload( FLT, APU12_FV, 6, APU12_10V, APU12_10UA, 1, APU12_PIN_TO_VI );
	lwait (10 ms_f);

	pldResultsI = apu12mi(FLT, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResultsI * uA); //Test#370

	apu12setcapload( FLT, APU12_FV, 0, APU12_10V, APU12_10UA, 1, APU12_PIN_TO_VI );
	lwait(1 ms_f);

	cbitopen(K6);
	apu12set( FLT, APU12_FV, 0.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );

	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// }		END 
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+



	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// {		BLOCK : SCL (pin7), SDA (pin8)
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	cbitclose(K13); // APU11-to-SCL
	cbitclose(K58, K60, K62); // OUT4, OUT5 and OUT6 to BUSA
	lwait (3 ms_f);

	apu12setcapload( SCL, APU12_FV, 6, APU12_10V, APU12_10UA, 1, APU12_PIN_TO_VI );
	lwait (10 ms_f);

	pldResultsI = apu12mi(SCL, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResultsI * uA); //Test#380

	apu12setcapload( SCL, APU12_FV, 0, APU12_10V, APU12_10UA, 1, APU12_PIN_TO_VI );
	lwait(1 ms_f);

	cbitopen(K13);
	apu12set( SCL, APU12_FV, 0.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// }		END 
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+



	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// {		BLOCK : SDA (pin8)
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	cbitclose(K12); // APU10-to-SDA
	lwait (3 ms_f);

	apu12setcapload( SDA, APU12_FV, 6, APU12_10V, APU12_10UA, 1, APU12_PIN_TO_VI );
	lwait (10 ms_f);

	pldResultsI = apu12mi(SDA, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResultsI * uA); //Test#390

	apu12setcapload( SDA, APU12_FV, 0, APU12_10V, APU12_10UA, 1, APU12_PIN_TO_VI );
	lwait(1 ms_f);

	cbitopen(K12, K58, K60, K62);
	apu12set( SDA, APU12_FV, 0.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// }		END 
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+







	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// {		BLOCK : CS (pin19)
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+

	cbitclose(K33); // CS-to-5KOhm pull-down
	lwait(2 ms_f);
	apu12setcapload( CS, APU12_FV, 5.0, APU12_10V, APU12_10MA, 1, APU12_PIN_TO_VI );
	lwait(2 ms_f);

	pldResultsI = apu12mi(CS, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResultsI); //Test#400


	//Calculate R pull-down
	pldResults = 5000 / (pldResultsI + 1e-12);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#410

	apu12set( CS, APU12_FV, 0.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	cbitopen(K33);
	lwait (1 ms_f);

	apu12set( CS, APU12_FV, 5.0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );
	lwait (10 ms_f);

	pldResultsI = apu12mi(CS, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResultsI * uA); //Test#420

	apu12set( CS, APU12_FV, 0.0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );
	lwait (500 us_f);
	apu12set( CS, APU12_FV, 0.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );

	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// }		END 
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+







	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// {		BLOCK : DIM (pin13)
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	cbitclose(K7); //APU5-to-DIM
	lwait (3 ms_f);
	apu12setcapload( DIM, APU12_FV, 6.0, APU12_10V, APU12_10UA, 1, APU12_PIN_TO_VI );
	lwait (10 ms_f);

	pldResultsI = apu12mi(DIM, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResultsI * uA); //Test#430

	apu12set( DIM, APU12_FV, 0.0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );

	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// }		END 
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+




	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// {		BLOCK : COMP (pin18)
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	cbitopen(K7); //APU5-to-COMP
	lwait (3 ms_f);
	apu12setcapload( COMP, APU12_FV, 6.0, APU12_10V, APU12_10UA, 1, APU12_PIN_TO_VI );
	lwait (10 ms_f);

	pldResultsI = apu12mi(COMP, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResultsI * uA); //Test#440

	apu12set( COMP, APU12_FV, 0.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// }		END 
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+





	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// {		BLOCK : NDRV (pin22)
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	cbitclose(K11); //APU9-to-NDRV
	cbitclose(K56, K58); //OUT3 and OUT4 to BUSA
	lwait (3 ms_f);

	apu12setcapload( NDRV, APU12_FV, 6.0, APU12_10V, APU12_10UA, 1, APU12_PIN_TO_VI );
	lwait (10 ms_f);

	pldResultsI = apu12mi(NDRV, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResultsI * uA); //Test#450

	apu12set( NDRV, APU12_FV, 0.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	cbitopen(K11, K56, K58);
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// }		END 
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+





	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// {		BLOCK : BSTMON (Pin20)
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	apu12set( BSTMON, APU12_FV, 0.0, APU12_30V, APU12_10UA, APU12_PIN_TO_VI );
	lwait(200 us_f);
	apu12setcapload( BSTMON, APU12_FV, 30.0, APU12_30V, APU12_10UA, 1, APU12_PIN_TO_VI );
	lwait (10 ms_f);

	pldResultsI = apu12mi(BSTMON, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResultsI * uA); //Test#460

	apu12set( BSTMON, APU12_FV, 0.0, APU12_30V, APU12_10UA, APU12_PIN_TO_VI );
	lwait (1 ms_f);
	apu12set( BSTMON, APU12_FV, 0.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// }		END 
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+






	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// {		BLOCK : IREF (pin12)
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	cbitclose(K15); //APU13-to-IREF
	cbitclose(K34); // IREF-to-49.9KOhm pull-down
	lwait(2 ms_f);
	apu12setcapload( IREF, APU12_FV, 5.0, APU12_10V, APU12_10MA, 1, APU12_PIN_TO_VI );
	lwait(500 ms_f);

	pldResultsI = apu12mi(IREF, APU12_MI_10X, 10);
	Maxim::DatalogAll(DSIndex, pldResultsI * uA); //Test#470


	//Calculate R pull-down
	pldResults = 5 / (pldResultsI + 1e-12);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#480

	apu12set( IREF, APU12_FV, 0.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	cbitopen(K34);
	lwait (1 ms_f);

	apu12set( IREF, APU12_FV, 5.0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );
	lwait (10 ms_f);

	pldResultsI = apu12mi(IREF, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResultsI * uA); //Test#490

	apu12set( IREF, APU12_FV, 0.0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );
	lwait (500 us_f);
	apu12set( IREF, APU12_FV, 0.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	cbitopen(K15);
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// }		END 
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+








	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// {		BLOCK : FSEN (pin17)
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+

	cbitclose(K35); // FSEN-to-3.3KOhm pull-down
	lwait(2 ms_f);
	apu12setcapload( FSEN, APU12_FV, 5.0, APU12_10V, APU12_10MA, 1, APU12_PIN_TO_VI );
	lwait(2 ms_f);

	pldResultsI = apu12mi(FSEN, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResultsI * mA); //Test#500


	//Calculate R pull-down
	pldResults = 5000 / (pldResultsI + 1e-12);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#510

	apu12set( FSEN, APU12_FV, 0.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	cbitopen(K35);
	lwait (1 ms_f);

	apu12set( FSEN, APU12_FV, 5.0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );
	lwait (10 ms_f);

	pldResultsI = apu12mi(FSEN, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResultsI * uA); //Test#520

	apu12set( FSEN, APU12_FV, 0.0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );
	lwait (500 us_f);
	apu12set( FSEN, APU12_FV, 0.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );

	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// }		END 
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+








	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// {		BLOCK : RT (pin10)
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	cbitclose(K4); //APU2-to-RT
	cbitclose(K36); // RT-to-12.4KOhm pull-down
	lwait(2 ms_f);
	apu12setcapload( RT, APU12_FV, 5.0, APU12_10V, APU12_10MA, 1, APU12_PIN_TO_VI );
	lwait(2 ms_f);

	pldResultsI = apu12mi(RT, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResultsI * uA); //Test#530


	//Calculate R pull-down
	pldResults = 5 / (pldResultsI + 1e-12);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#540

	apu12set( FSEN, APU12_FV, 0.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	cbitopen(K36);
	

	cbitclose(K37); // RT-to-76.8KOhm pull-down
	lwait (1 ms_f);
	apu12setcapload( RT, APU12_FV, 5.0, APU12_10V, APU12_10MA, 1, APU12_PIN_TO_VI );
	lwait(2 ms_f);

	pldResultsI = apu12mi(RT, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResultsI * uA); //Test#550


	//Calculate R pull-down
	pldResults = 5 / (pldResultsI + 1e-12);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#560

	apu12set( RT, APU12_FV, 0.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	cbitopen(K37);


	apu12set( RT, APU12_FV, 5.0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );
	lwait (10 ms_f);

	pldResultsI = apu12mi(RT, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResultsI * uA); //Test#570

	apu12set( RT, APU12_FV, 0.0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );
	lwait (500 us_f);
	apu12set( RT, APU12_FV, 0.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );

	cbitopen(K4);

	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// }		END 
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+






	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// {		BLOCK : OUT1 (pin3), OUT2 (pin16), OUT3 (pin4), OUT4 (pin15), OUT5 (pin5), OUT6 (pin14)
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+

	OUTxLeakTest(DSIndex, OUT1); //Test#580
	OUTxLeakTest(DSIndex, OUT2); //Test#590
	OUTxLeakTest(DSIndex, OUT3); //Test#600
	OUTxLeakTest(DSIndex, OUT4); //Test#610
	OUTxLeakTest(DSIndex, OUT5); //Test#620
	OUTxLeakTest(DSIndex, OUT6); //Test#630

	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// }		END 
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+




	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// {		BLOCK : HDSET (pin5) (for AP79)
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+

	cbitclose(K39); // HDSET-to-3.3KOhm pull-down
	lwait(2 ms_f);
	apu12setcapload( OUT5, APU12_FV, 5.0, APU12_10V, APU12_10MA, 1, APU12_PIN_TO_VI );
	lwait(2 ms_f);

	pldResultsI = apu12mi(OUT5, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResultsI * mA); //Test#640

	//Calculate R pull-down
	pldResults = 5000 / (pldResultsI + 1e-12);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#650

	apu12set( OUT5, APU12_FV, 0.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	cbitopen(K39);

	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// }		END 
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+










	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// {		BLOCK : LED BUSA
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	
	//BUSA
	apu12set( OUT1, APU12_FV, 0.0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );

	cbitclose(K52, K54); //OUT1 and OUT2
	lwait (1 ms_f);
	apu12set( OUT1, APU12_FV, 1.0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);

	pldResultsI = apu12mi(OUT1, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResultsI); //Test#660

	apu12set( OUT1, APU12_FV, 0.0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);


	cbitclose(K56); //OUT3
	lwait (1 ms_f);

	apu12set( OUT1, APU12_FV, 1.0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);
	pldResultsI = apu12mi(OUT1, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResultsI); //Test#670

	apu12set( OUT1, APU12_FV, 0.0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);

	cbitclose(K58); //OUT4
	lwait (1 ms_f);

	apu12set( OUT1, APU12_FV, 1.0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);
	pldResultsI = apu12mi(OUT1, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResultsI); //Test#680


	apu12set( OUT1, APU12_FV, 0.0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);

	cbitclose(K60); //OUT5
	lwait (1 ms_f);

	apu12set( OUT1, APU12_FV, 1.0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);
	pldResultsI = apu12mi(OUT1, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResultsI); //Test#690


	apu12set( OUT1, APU12_FV, 0.0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);

	cbitclose(K62); //OUT6
	lwait (1 ms_f);

	apu12set( OUT1, APU12_FV, 1.0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);
	pldResultsI = apu12mi(OUT1, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResultsI); //Test#700


	apu12set( OUT1, APU12_FV, 0.0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);

	cbitopen(K52, K54, K56, K58, K60, K62);
	apu12set( OUT1, APU12_FV, 0.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);


	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// }		END 
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+








	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// {		BLOCK : LED BUSB
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	
	//BUSB
	cbitclose(K5); //APU3-to-BUSB
	apu12set( BUSB, APU12_FV, 0.0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );


	cbitclose(K53); //OUT1
	lwait (3 ms_f);
	apu12set( BUSB, APU12_FV, 1.0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);

	pldResultsI = apu12mi(BUSB, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResultsI); //Test#710

	apu12set( BUSB, APU12_FV, 0.0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);


	cbitclose(K55); //OUT2
	lwait (1 ms_f);
	apu12set( BUSB, APU12_FV, 1.0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);

	pldResultsI = apu12mi(BUSB, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResultsI); //Test#720

	apu12set( BUSB, APU12_FV, 0.0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);

	cbitclose(K57); //OUT3
	lwait (3 ms_f);
	apu12set( BUSB, APU12_FV, 1.0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);

	pldResultsI = apu12mi(BUSB, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResultsI); //Test#730

	apu12set( BUSB, APU12_FV, 0.0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);

	cbitclose(K59); //OUT4
	lwait (3 ms_f);
	apu12set( BUSB, APU12_FV, 1.0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);

	pldResultsI = apu12mi(BUSB, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResultsI); //Test#740

	apu12set( BUSB, APU12_FV, 0.0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);

	cbitclose(K61); //OUT5
	lwait (3 ms_f);
	apu12set( BUSB, APU12_FV, 1.0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);

	pldResultsI = apu12mi(BUSB, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResultsI); //Test#750

	apu12set( BUSB, APU12_FV, 0.0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);


	cbitclose(K63); //OUT6
	lwait (3 ms_f);
	apu12set( BUSB, APU12_FV, 1.0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);

	pldResultsI = apu12mi(BUSB, APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResultsI); //Test#760

	apu12set( BUSB, APU12_FV, 0.0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);

	cbitopen(K5, K53, K55, K57, K59, K61, K63);
	apu12set( BUSB, APU12_FV, 0.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);



	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// }		END 
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+







	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// {		BLOCK : TIMING BUFFER U4
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+

	//NDRV
	cbitclose(K11, K43, K44);
	lwait (1 ms_f);

	pldResults = MeasureTime(QTMU_GP, din1, direct_v, "AUTO", 1, NDRV_toggle, "", 1 ms_f, QTMU_RCVH, QTMU_DIRECTB, QTMU_POS, QTMU_POS, 0, 1, 2.5 , 2.5, 5000, 5000);

	Maxim::DatalogAll(DSIndex, pldResults * ms); //Test#770

	cbitopen(K44);




	//FLT
	cbitclose(K6, K45);
	lwait (1 ms_f);
	dpinviset("SCAN_OUT", DPIN_FV, 0.0, DPIN_10V, DPIN_8MA, 7, -1, MS_ALL);
	apu12setcapload( FLT, APU12_FV, 6.0, APU12_10V, APU12_10MA, 1, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	pldResults = dpinmv("SCAN_OUT", 10, 13);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#780


	apu12setcapload( FLT, APU12_FV, 0.1, APU12_10V, APU12_10MA, 1, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	pldResults = dpinmv("SCAN_OUT", 10, 13);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#790

	apu12setcapload( FLT, APU12_FV, 0, APU12_10V, APU12_10MA, 1, APU12_PIN_TO_VI );
	lwait (2 ms_f);
	cbitopen(K6, K45);


	//OUTA
	cbitclose(K46, K52);
	lwait (1 ms_f);

	apu12setcapload( OUT1, APU12_FV, 2.50, APU12_10V, APU12_10MA, 1, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	pldResults = dpinmv("SCAN_OUT", 10, 13);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#800

	apu12setcapload( OUT1, APU12_FV, 0, APU12_10V, APU12_10MA, 1, APU12_PIN_TO_VI );
	lwait (2 ms_f);
	cbitopen(K43);
	lwait (1 ms_f);

	apu12setcapload( OUT1, APU12_FV, 5.0, APU12_10V, APU12_10MA, 1, APU12_PIN_TO_VI );
	lwait (2 ms_f);
	pldResults = dpinmv("SCAN_OUT", 10, 13);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#810

	apu12setcapload( OUT1, APU12_FV, 0.0, APU12_10V, APU12_10MA, 1, APU12_PIN_TO_VI );
	lwait (2 ms_f);
	dpinviset("SCAN_OUT", DPIN_OFF, 0.0, DPIN_10V, DPIN_8MA, 7, -1, MS_ALL);
	cbitopen(K11, K46, K52);
	
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// }		END 
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+








	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// {		BLOCK : TIMING BUFFER U5
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	

	//BUSB
	cbitclose(K23, K47, K53);
	lwait (1 ms_f);

	pldResults = MeasureTime(QTMU_GP, direct_v, din2, "AUTO", 1, OUT1_toggle, "", 1 ms_f, QTMU_DIRECTA, QTMU_RCVH, QTMU_POS, QTMU_POS, 0, 1, 2.5 , 2.5, 5000, 5000);

	Maxim::DatalogAll(DSIndex, pldResults * ms); //Test#820

	cbitopen(K23);

	

	//EN

	cbitclose(K48);
	apu12set( EN, APU12_FV, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);
	apu12setcapload( OUT1, APU12_FV, 5.0, APU12_10V, APU12_10MA, 1, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	pldResults = apu12mv(EN, 10);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#830

	apu12setcapload( OUT1, APU12_FV, 0.0, APU12_10V, APU12_10MA, 1, APU12_PIN_TO_VI );
	lwait (1 ms_f);	
	cbitopen(K48);
	apu12set( EN, APU12_FV, 0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );



	//DIM
	apu12set( DIM, APU12_FV, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	cbitclose(K7, K49);
	lwait (1 ms_f);

	apu12setcapload( OUT1, APU12_FV, 4.5, APU12_10V, APU12_10MA, 1, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	pldResults = apu12mv(DIM, 10);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#840

	apu12setcapload( OUT1, APU12_FV, 0.0, APU12_10V, APU12_10MA, 1, APU12_PIN_TO_VI );
	lwait (1 ms_f);	
	cbitopen(K7, K49);
	apu12set( DIM, APU12_FV, 0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );



	//BSTMON
	apu12set( BSTMON, APU12_FV, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	cbitclose(K50);
	lwait (1 ms_f);

	apu12setcapload( OUT1, APU12_FV, 4.0, APU12_10V, APU12_10MA, 1, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	pldResults = apu12mv(BSTMON, 10);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#850

	apu12setcapload( OUT1, APU12_FV, 0.0, APU12_10V, APU12_10MA, 1, APU12_PIN_TO_VI );
	lwait (1 ms_f);	
	cbitopen(K47, K50, K53);

	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// }		END 
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+







	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// {		BLOCK : OUTPUT REGULATION LOOP
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	double dOUTCurrent = -50;

	cbitclose(K22);
	lwait (3 ms_f);

	//Expect 0V here
	pldResults = apu12mv(BSTMON, 10);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#860



	//OUT1, OUT2, OUT3 to BUSA
	cbitclose(K52, K54, K56);

	//OUT4, OUT5, OUT6 to BUSB
	cbitclose(K59, K61, K63);


	apu12setcapload( COMP, APU12_FV, 4, APU12_10V, APU12_10MA, 1, APU12_PIN_TO_VI );
	lwait (10 ms_f);

	apu12setcapload( OUT1, APU12_FI, dOUTCurrent, APU12_10V, APU12_100MA, 1, APU12_PIN_TO_VI );
	apu12setcapload( OUT2, APU12_FI, dOUTCurrent, APU12_10V, APU12_100MA, 1, APU12_PIN_TO_VI );
	apu12setcapload( OUT3, APU12_FI, dOUTCurrent, APU12_10V, APU12_100MA, 1, APU12_PIN_TO_VI );
	apu12setcapload( OUT4, APU12_FI, dOUTCurrent, APU12_10V, APU12_100MA, 1, APU12_PIN_TO_VI );
	apu12setcapload( OUT5, APU12_FI, dOUTCurrent, APU12_10V, APU12_100MA, 1, APU12_PIN_TO_VI );
	apu12setcapload( OUT6, APU12_FI, dOUTCurrent, APU12_10V, APU12_100MA, 1, APU12_PIN_TO_VI );

	lwait (10 ms_f);

	pldResults = apu12mv(BSTMON, 10);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#870

	pldResults = apu12mv(OUT1, 10);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#880
	
	pldResults = apu12mv(OUT2, 10);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#890

	pldResults = apu12mv(OUT3, 10);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#900

	pldResults = apu12mv(OUT4, 10);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#910

	pldResults = apu12mv(OUT5, 10);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#920

	pldResults = apu12mv(OUT6, 10);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#930

	apu12setcapload( COMP, APU12_FV, 0, APU12_10V, APU12_10MA, 1, APU12_PIN_TO_VI );
	apu12setcapload( OUT1, APU12_FV, 0, APU12_10V, APU12_100MA, 1, APU12_PIN_TO_VI );
	apu12setcapload( OUT2, APU12_FV, 0, APU12_10V, APU12_100MA, 1, APU12_PIN_TO_VI );
	apu12setcapload( OUT3, APU12_FV, 0, APU12_10V, APU12_100MA, 1, APU12_PIN_TO_VI );
	apu12setcapload( OUT4, APU12_FV, 0, APU12_10V, APU12_100MA, 1, APU12_PIN_TO_VI );
	apu12setcapload( OUT5, APU12_FV, 0, APU12_10V, APU12_100MA, 1, APU12_PIN_TO_VI );
	apu12setcapload( OUT6, APU12_FV, 0, APU12_10V, APU12_100MA, 1, APU12_PIN_TO_VI );

	lwait (10 ms_f);

	apu12set( OUT1, APU12_FV, 0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	apu12set( OUT2, APU12_FV, 0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	apu12set( OUT3, APU12_FV, 0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	apu12set( OUT4, APU12_FV, 0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	apu12set( OUT5, APU12_FV, 0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	apu12set( OUT6, APU12_FV, 0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );

	apu12set( BSTMON, APU12_FV, 0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	lwait (10 ms_f);

	cbitopen(K22);
	cbitopen(K52, K54, K56);
	cbitopen(K59, K61, K63);

	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// }		END 
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+






	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// {		BLOCK : DIGITAL BUFFER for SCAN
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	apu12set( SCL, APU12_FV, 0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );
	apu12set( SDA, APU12_FV, 0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );
	apu12set( DIM, APU12_FV, 0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );
	apu12set( CS, APU12_FV, 0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );

	//SCAN_RESET, K40
	cbitclose(K7, K40);
	lwait (1 ms_f);

	dpinviset("DIM", DPIN_FV, 0.0, DPIN_10V, DPIN_32UA, 7, -1, MS_ALL);
	lwait (5 ms_f);

	//Expect 0V
	pldResults = apu12mv(DIM, 10);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#940
	

	dpinviset("DIM", DPIN_FV, 3, DPIN_10V, DPIN_32UA, 7, -1, MS_ALL);
	lwait (5 ms_f);

	//Expect 5V
	pldResults = apu12mv(DIM, 10);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#950


	dpinviset("DIM", DPIN_FV, 0.0, DPIN_10V, DPIN_32UA, 7, -1, MS_ALL);
	lwait (5 ms_f);
	cbitopen(K7, K40);
	dpinviset("DIM", DPIN_OFF, 0.0, DPIN_10V, DPIN_32UA, 7, -1, MS_ALL);




	//SCAN_CLOCK (SCL), K41
	cbitclose(K13, K41);
	lwait (1 ms_f);

	dpinviset("SCL_IO", DPIN_FV, 0.0, DPIN_10V, DPIN_32UA, 7, -1, MS_ALL);
	lwait (5 ms_f);

	//Expect 0V
	pldResults = apu12mv(SCL, 10);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#960
	

	dpinviset("SCL_IO", DPIN_FV, 3, DPIN_10V, DPIN_32UA, 7, -1, MS_ALL);
	lwait (5 ms_f);

	//Expect 5V
	pldResults = apu12mv(SCL, 10);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#970


	dpinviset("SCL_IO", DPIN_FV, 0.0, DPIN_10V, DPIN_32UA, 7, -1, MS_ALL);
	lwait (5 ms_f);
	cbitopen(K13, K41);
	dpinviset("SCL_IO", DPIN_OFF, 0.0, DPIN_10V, DPIN_32UA, 7, -1, MS_ALL);


	//SCAN_ENABLE (CS), K42
	cbitclose(K42);
	lwait (1 ms_f);

	dpinviset("CS", DPIN_FV, 0.0, DPIN_10V, DPIN_32UA, 7, -1, MS_ALL);
	lwait (5 ms_f);

	//Expect 0V
	pldResults = apu12mv(CS, 10);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#980
	

	dpinviset("CS", DPIN_FV, 3, DPIN_10V, DPIN_32UA, 7, -1, MS_ALL);
	lwait (5 ms_f);

	//Expect 5V
	pldResults = apu12mv(CS, 10);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#990


	dpinviset("CS", DPIN_FV, 0.0, DPIN_10V, DPIN_32UA, 7, -1, MS_ALL);
	lwait (5 ms_f);
	cbitopen(K42);
	dpinviset("CS", DPIN_OFF, 0.0, DPIN_10V, DPIN_32UA, 7, -1, MS_ALL);


	//SCAN_IN (SDA), K20 open
	apu12set( SDA, APU12_FV, 0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );
	lwait (1 ms_f);

	cbitclose(K12);
	lwait (3 ms_f);

	dpinviset("SDA_IO", DPIN_FV, 0.0, DPIN_10V, DPIN_32UA, 7, -1, MS_ALL);
	lwait (5 ms_f);

	//Expect 0V
	pldResults = apu12mv(SDA, 10);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#1000
	

	dpinviset("SDA_IO", DPIN_FV, 3, DPIN_10V, DPIN_32UA, 7, -1, MS_ALL);
	lwait (5 ms_f);

	//Expect 3V
	pldResults = apu12mv(SDA, 10);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#1010


	dpinviset("SDA_IO", DPIN_FV, 0.0, DPIN_10V, DPIN_32UA, 7, -1, MS_ALL);
	lwait (5 ms_f);




	//SCAN_IN (SDA), K20, with buffer
	cbitclose(K20);
	lwait (3 ms_f);

	//Expect 0V
	pldResults = apu12mv(SDA, 10);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#1020
	

	dpinviset("SDA_IO", DPIN_FV, 3, DPIN_10V, DPIN_32UA, 7, -1, MS_ALL);
	lwait (5 ms_f);

	//Expect 5V
	pldResults = apu12mv(SDA, 10);
	Maxim::DatalogAll(DSIndex, pldResults); //Test#1030


	dpinviset("SDA_IO", DPIN_FV, 0.0, DPIN_10V, DPIN_32UA, 7, -1, MS_ALL);
	lwait (5 ms_f);
	cbitopen(K12, K20);
	dpinviset("SDA_IO", DPIN_OFF, 0.0, DPIN_10V, DPIN_32UA, 7, -1, MS_ALL);

	apu12set( SCL, APU12_FV, 0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	apu12set( SDA, APU12_FV, 0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	apu12set( DIM, APU12_FV, 0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	apu12set( CS, APU12_FV, 0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );

	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// }		END 
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+






	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// {		BLOCK : Power Module (PI45)
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	

	//Connect OUT1 to BUSA
	cbitclose(K52);
	lwait (1 ms_f);

	pldResults = apu12mv(OUT1, 10);
	Maxim::DatalogAll(DSIndex, pldResults);

	pldResults = apu12mi(OUT1,APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResults);	


	//Turn on Power module
	cbitclose(K24);
	lwait (10 ms_f);

	pldResults = apu12mv(OUT1, 10);
	Maxim::DatalogAll(DSIndex, pldResults);

	pldResults = apu12mi(OUT1,APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResults);	


	apu12set(OUT1, APU12_FV, 0, APU12_10V, APU12_200MA, APU12_PIN_TO_VI);
	lwait (10 ms_f);

	pldResults = apu12mv(OUT1, 10);
	Maxim::DatalogAll(DSIndex, pldResults);

	pldResults = apu12mi(OUT1,APU12_MI_1X, 10);
	Maxim::DatalogAll(DSIndex, pldResults);



	//Turn off Power module
	cbitopen(K24);
	apu12set(OUT1, APU12_FV, 0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI);
	lwait (10 ms_f);
	//DisConnect OUT1 from BUSA
	cbitopen(K52);


	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// }		END 
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	











	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// {		BLOCK : 
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// }		END 
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// {		BLOCK : 
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// }		END 
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// {		BLOCK : 
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// }		END 
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+




























	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// {		BLOCK : 
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// }		END 
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+




	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// {		Power down to a safe state.  (Edit this section)
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+



	apu12set( ALL_APU, APU12_OFF, 0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
	gbSkipPowerDown = true;

	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	// }		Power down to a safe state.  (Edit this section)
	//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	alarmset( ALARM_CLAMP, ON );

	//}}CG_Hardware_Checker

//HW Checker code will be automatically datalogged by the calling function.  Do Not call Maxim::Datalog(); here to flush the datalog.

//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// }		End of HW Checker. 
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
}

/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
* sReadHardwareID
*   Purpose: User supplied function to return the Hardware ID code from the EEPROM.  See Remarks. 
*	Arguments:	
*		none 
*	Returns:
*		Return a user supplied string with the Contactor Board barcode name and serial number.  This is typically generated by reading the EEPROM on the Contactor board.
*   Remarks:
*		This function is called by the Maxim Library.
*		The Hardware ID is used by the Maxim::HWC::bSkip_Checker function to determine when to run the Hardware Checker.  
*		The Hardware ID is logged to the checker log file to indicate the hardware used when the checker passed/failed.  
*		If the Hardware ID changes, the operator will be prompted to re-run the checker when the program is reloaded.
*
*   Revision History:
*		
*                                                                            
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
std::string sReadHardwareID(void)
{
	std::string sHardwareID, sEEPROMCode;
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// {		Return the HardwareID string (Edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	//Read the contactor board ID from the EEPROM 
	//See note in library for this function.  
	sEEPROMCode = Maxim::HWC::sRead_EEPROM_Hardware_ID();	//Comment this line out if the Contactor Board does not have an EEPROM to read.
										
	//If required, translate the EEPROM code into the Hardware Name to match the database and Setup Sheet
	sHardwareID = sEEPROMCode;	//TODO

	return sHardwareID;
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// }		Return the HardwareID string (Edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
}





//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// {		DataSheet Checker Function.  (Do not edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
//DO NOT EDIT THIS FUNCTION
/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
* Template_Hardware_Checker
*   Purpose: Template's Hardware Checker function.  Called from the DataSheet.  Calls library and user functions to run the HW Checker when required.
*	Arguments:	
*		[DSIndex]    DataSheet Index location passed in from the ETS Program Flow
*		[TestLabel]  DataSheet TestLabel passed in from the ETS Program Flow
*	Returns:
*		ETS Program Flow (int) status indicating if the program should continue. 
*   Remarks:
*		DO NOT EDIT THIS FUNCTION.  It calls the functions above for the user to edit.
*			If you find a need to edit this function, please submit the changes to the ETS Template librarians so all future products can benefit from the changes.
*
*		To run the hardware checker, set the "Test_Step" General Purpose String in the MST Shell to "HW_CHECKER".
*			Or leave the "Test_Step" string blank and select "HW_CHECKER" from the option list when the program runs.
*		The Maxim Loader will automatically enforce running the checker for production.
*
*		The Checker Operator prompts can be avoided by setting the "HW_Checker_Message" General Purpose Switch in the MST Shell to False.
*		This is useful when collecting Gage Study data for the Checker.
*
*		DO NOT DISABLE THE HARDWARE_CHECKER FUNCTION IN THE DATASHEET
*			The checker will only run when the test step is "HW_CHECKER".   
*			Do not add code to bypass the contents of this function for other test steps.  The library already takes care of that.
*
*   Revision History:
*		
*                                                                            
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
// DO NOT EDIT THIS FUNCTION.
ETS_PRGFLOW_FUNC Template_Hardware_Checker( int DSIndex, LPCTSTR TestLabel )
{
	return Maxim::HWC::Lib_Hardware_Checker(DSIndex);	//Call the Maxim Library to call the above user functions.
} // END_ETS_PRGFLOW_FUNC
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// }		DataSheet Checker Function.  (Do not edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+

