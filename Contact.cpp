/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*                                                                            
*	Source title:		Contact.cpp                                
*		Created from Maxim Template                         
*       Template Written and maintained by:								 
*			Patrick Hayes (Patrick.Hayes@MaximIntegrated.com)	                 
*			Mark Bruening (Mark.Bruening@MaximIntegrated.com)                      
*		  
*	URL:				$URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/Contact.cpp $
*   Last Modified by:	$LastChangedBy: govit.pimthong $                                       
*	Date:				$Date: 2017-08-04 03:15:03 +0700 (Fri, 04 Aug 2017) $                                                
*   Current Rev:		$Revision: 67637 $
*                                                                            
*	Description:  Contact function template.                            
*                                                                            
*   Template Revision History                                                    
*		20110707 MWB: Initial Development and Release		
*
*   User Revision History                                                    
*                                                                            
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
#include "AP89_FT.h"



/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
* Contact
*   Purpose: User supplied function to run the Contact test.
*	Arguments:	
*		[DSIndex]  DataSheet Index location passed in from the ETS Program Flow
*	Returns:
*		[DSIndex]  Updated DSIndex is returned.
*   Remarks:
*		This function is called by the Maxim Library.
*		This function must datalog at least one result.  Use one of the Maxim::Datalog functions to store the result. 
*			The result will be checked in the calling function.  Do not flush the datalog in this function, it is done by the calling function.
*
*   Revision History:
*		
*                                                                            
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/


void Contact(int & DSIndex)
{
	//{{CG_Contact 
	// { Sample Code

	//Turn alarms for current and voltage clamps off during continuity test.
	alarmset( ALARM_CLAMP, OFF ); 
	//APU12 pins cannot simultaneously clamp voltage and current.
	//If the APU12 pins are set to force current with a 3.6V or 10V voltage range, and a DUT is not in the socket, 
	//	the APU will drive to -12V (120% of 10V range).
	//SPU112 pins do not share this issue.

	//To get around this issue, force -2V on the APU12 with 100uA current range.  This will limit the current to no more than 130uA (130% of range)
	//With an open socket, expect -2V, 0uA, no alarm.  (Test fails, below -1V lower limit)
	//With a diode present, expect -0.6V, 130uA, current clamp hit before programmed -2V reached, 
	//	so alarm is triggered but suppressed by the alarmset statement above. (Test passes)
	//With an short circuit, expect 0V, 130uA, current clamp alarm suppressed. (Test fails, above -0.3V higher limit)
		
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// {		Start Contact Tests.  (Edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+


	//--Maxim::CBits::Extended::maximCBits.mapCBitStates

	if(!gbLastContact){

		mark(1);
		cbitclose(K_POWER);

	}

	if(AP89)
		giLED_CH = 6;
	else
		giLED_CH = 4;

	gbContactFail = true;
	gbSkipPowerDown = false;

	gbOUT_COMP_TH = false;

	//Example code:  This code will be device specific. 
	PinListData pldResults;	//declare a PLD variable for the results.

	//----- Ground all pins before doing continuity
	apu12set( ALL_APU, APU12_FV, 0.0, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );
	lwait( 1 ms_f );
	cbitopen(K24, K28, K29, K34, K37, K40, K50, K17, K23, K16);
	cbitclose(K6, K11, K14, K41, K4, K42, K15, K7); //FLT, PGATE, SCL, RT, CS, IREF, DIM

	//Ground SDA, SCL, CS using DPU
	dpinviset( "SCL_IO", DPIN_FV, 0.0, DPIN_8V, DPIN_2MA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL ); //SCL
	dpinviset( "SDA_IO", DPIN_FV, 0.0, DPIN_8V, DPIN_2MA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL ); //SDA
	dpinviset( "CS", DPIN_FV, 0.0, DPIN_8V, DPIN_2MA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL ); //CS


	//OUT6 to BUSA and 0V (Drive by OUT3)
	cbitclose(K62, K56);
	
	//---------------------------------------------------------------------------------
	//SET1,  Force -2V on limited current range.
	//IN	OUT1	OUT5	HDSET	RT	DIM	OUT4	BSTMON	NDRV
	//---------------------------------------------------------------------------------
	apu12set( SET1, APU12_FV, -2.0, APU12_3p6V, APU12_100UA, APU12_PIN_TO_VI );	
	lwait(3 ms_f );

	//IN
	pldResults = apu12mv( IN1, 1, 13.0 );
	Maxim::DatalogAll(DSIndex, pldResults);

	//OUT1
	pldResults = apu12mv( OUT1, 1, 13.0 );
	Maxim::DatalogAll(DSIndex, pldResults);

	//OUT5 (HDSET)
	pldResults = apu12mv( OUT5, 1, 13.0 );

	if(AP89){
		Maxim::DatalogAll(DSIndex, pldResults);
		DSIndex++;
	}
	else{
		DSIndex++;
		Maxim::DatalogAll(DSIndex, pldResults);
	}


	//RT
	pldResults = apu12mv( RT, 1, 13.0 );
	Maxim::DatalogAll(DSIndex, pldResults);

	//DIM
	pldResults = apu12mv( DIM, 1, 13.0 );
	Maxim::DatalogAll(DSIndex, pldResults);

	//OUT4
	pldResults = apu12mv( OUT4, 1, 13.0 );
	Maxim::DatalogAll(DSIndex, pldResults);

	//BSTMON
	pldResults = apu12mv( BSTMON, 1, 13.0 );
	Maxim::DatalogAll(DSIndex, pldResults);

	//NDRV
	pldResults = apu12mv( NDRV, 1, 13.0 );
	Maxim::DatalogAll(DSIndex, pldResults);

	apu12set( SET1, APU12_FV, 0, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );	
	
	//---------------------------------------------------------------------------------







	//---------------------------------------------------------------------------------
	//SET2,  Force -2V on limited current range.
	//OUT3	FLT	SDA_PSEN	IREF	OUT6	I2CDIS_RSDT	OUT2	COMP	EN
	//---------------------------------------------------------------------------------
	//Ground DIM by short it to BSTMON
	
	//OUT6-APU, OUT6-BUSA, CS-APU, CS-DPU, COMP-APU, OUT3
	cbitopen(K14, K62, K4, K42, K7, K56, K11);
	dpinviset( "SDA_IO", DPIN_OFF, 0.0, DPIN_8V, DPIN_2MA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL ); //SDA
	//SDA-APU, OUT4-BUSB, IREF-APU, DIM-BSTMON
	cbitclose(K12, K59, K15, K49, K50, K47);
	lwait(3 ms_f );

	apu12set( SET2, APU12_FV, -2.0, APU12_3p6V, APU12_100UA, APU12_PIN_TO_VI );	
	lwait (1 ms_f);

	//OUT3
	pldResults = apu12mv( OUT3, 1, 13.0 );
	Maxim::DatalogAll(DSIndex, pldResults);

	//FLT
	pldResults = apu12mv( FLT, 1, 13.0 );
	Maxim::DatalogAll(DSIndex, pldResults);

	//SDA_PSEN
	pldResults = apu12mv( SDA, 1, 13.0 );
	Maxim::DatalogAll(DSIndex, pldResults);

	//IREF
	pldResults = apu12mv( IREF, 1, 13.0 );
	Maxim::DatalogAll(DSIndex, pldResults);

	//OUT6 (I2CDIS/RSDT)
	pldResults = apu12mv( OUT6, 1, 13.0 );

	if(AP89){
		Maxim::DatalogAll(DSIndex, pldResults);
		DSIndex++;
	}
	else{
		DSIndex++;
		Maxim::DatalogAll(DSIndex, pldResults);
	}


	//OUT2
	pldResults = apu12mv( OUT2, 1, 13.0 );
	Maxim::DatalogAll(DSIndex, pldResults);

	//COMP
	pldResults = apu12mv( COMP, 1, 13.0 );
	Maxim::DatalogAll(DSIndex, pldResults);

	//EN
	pldResults = apu12mv( EN, 1, 13.0 );
	Maxim::DatalogAll(DSIndex, pldResults);


	apu12set( SET2, APU12_FV, 0, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );	
	dpinviset( "SDA_IO", DPIN_FV, 0.0, DPIN_8V, DPIN_2MA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL ); //SDA

	//---------------------------------------------------------------------------------






	//---------------------------------------------------------------------------------
	//SET3,  Force -2V on limited current range.
	//PGATE	SCL_IRANGE	LEDGND	FSEN_ISET	CS	PGND	VCC
	//---------------------------------------------------------------------------------
	
	//VCC-APU, SCL-DPU, FSEN-APU
	cbitopen(K6, K41, K15);

	//PGATE-APU, OUT6-BUSB, SCL-APU, OUT5-BUSB, LEDGND-PGND-APU, NDRV-APU, OUT3-BUSB
	cbitclose(K14, K63, K13, K61, K19, K11, K57);
	lwait(3 ms_f );

	apu12set( SET3, APU12_FV, -2.0, APU12_3p6V, APU12_100UA, APU12_PIN_TO_VI );

	//Special need for VCC pin
	apu12set( VCC, APU12_FV, -2.0, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );
	lwait(5 ms_f);


	lwait (2 ms_f);

	//PGATE
	pldResults = apu12mv( PGATE, 1, 13.0 );
	Maxim::DatalogAll(DSIndex, pldResults);

	//SCL
	pldResults = apu12mv( SCL, 1, 13.0 );
	Maxim::DatalogAll(DSIndex, pldResults);

	//LEDGND
	pldResults = apu12mv( LEDGND, 1, 13.0 );
	Maxim::DatalogAll(DSIndex, pldResults);

	//FSEN
	pldResults = apu12mv( FSEN, 1, 13.0 );
	Maxim::DatalogAll(DSIndex, pldResults);

	//CS
	pldResults = apu12mv( CS, 1, 13.0 );
	Maxim::DatalogAll(DSIndex, pldResults);

	//PGND
	pldResults = apu12mv( PGND, 1, 13.0 );
	Maxim::DatalogAll(DSIndex, pldResults);


	//Low yield at VCC, temporary fix for now.


	int iTry = 0;
try_vcc:

	//VCC
	pldResults = apu12mv( VCC, 1, 13.0 );

	ForEachActive_iSite{
		//Retry if fail
		if((pldResults[iSite] > -0.15) && (iTry < 3)){
			iTry++;
			lwait(10 ms_f);
			goto try_vcc;
		}
	}

	Maxim::DatalogAll(DSIndex, pldResults);

	apu12set( VCC, APU12_FV, -2.0, APU12_3p6V, APU12_100UA, APU12_PIN_TO_VI );
	apu12set( SET3, APU12_FV, 0, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );	


	//---------------------------------------------------------------------------------




	//Restore
	dpinviset( "SCL_IO", DPIN_OFF, 0.0, DPIN_8V, DPIN_2MA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL ); //SCL
	dpinviset( "SDA_IO", DPIN_OFF, 0.0, DPIN_8V, DPIN_2MA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL ); //SDA
	dpinviset( "CS", DPIN_OFF, 0.0, DPIN_8V, DPIN_2MA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL ); //CS
	cbitopen(K14, K63, K13, K61, K19, K11, K57);
	cbitopen(K4, K7, K12, K42, K47, K49, K50, K56, K59, K62);





	//---------------------------------------------------------------------------------
	//SET4,  Force 2V on limited current range.
	//VCC_IN
	//---------------------------------------------------------------------------------
	apu12set( VCC, APU12_FV, 2.0, APU12_3p6V, APU12_100UA, APU12_PIN_TO_VI );
	lwait (1 ms_f);

	//VCC
	pldResults = apu12mv( VCC, 1, 13.0 );
	Maxim::DatalogAll(DSIndex, pldResults);

	apu12set( VCC, APU12_FV, 0, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );

	//---------------------------------------------------------------------------------




	//---------------------------------------------------------------------------------
	//SET5,  Force 2V on limited current range.
	//HDSET_VCC	RT_VCC	IREF_VCC	FSEN_VCC	COMP_VCC	CS_VCC	NDRV_VCC	PGND_GND	LEDGND_GND	EP
	//---------------------------------------------------------------------------------
	cbitclose(K4, K11, K15, K19);

	if(!AP89){
		apu12set( OUT5, APU12_FV, 2.0, APU12_3p6V, APU12_100UA, APU12_PIN_TO_VI );
		lwait (1 ms_f);
		//HDSET
		pldResults = apu12mv( OUT5, 1, 13.0 );
		Maxim::DatalogAll(DSIndex, pldResults);

		apu12set( OUT5, APU12_FV, 0.0, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );

	}
	else
		DSIndex++;

	
	apu12set( SET5, APU12_FV, 2.0, APU12_3p6V, APU12_100UA, APU12_PIN_TO_VI );
	lwait (2 ms_f);
	
	//RT
	pldResults = apu12mv( RT, 1, 13.0 );
	Maxim::DatalogAll(DSIndex, pldResults);

	//IREF
	pldResults = apu12mv( IREF, 1, 13.0 );
	Maxim::DatalogAll(DSIndex, pldResults);


	apu12set( SET5, APU12_FV, 0.0, APU12_3p6V, APU12_100UA, APU12_PIN_TO_VI );
	lwait (500 us_f);
	cbitopen(K4, K15);
	lwait (3 ms_f);

	apu12set( SET5, APU12_FV, 2.0, APU12_3p6V, APU12_100UA, APU12_PIN_TO_VI );
	lwait (1 ms_f);

	//FSEN
	pldResults = apu12mv( FSEN, 1, 13.0 );
	Maxim::DatalogAll(DSIndex, pldResults);

	//COMP
	pldResults = apu12mv( COMP, 1, 13.0 );
	Maxim::DatalogAll(DSIndex, pldResults);

	//CS
	pldResults = apu12mv( CS, 1, 13.0 );
	Maxim::DatalogAll(DSIndex, pldResults);

	//NDRV
	pldResults = apu12mv( NDRV, 1, 13.0 );
	Maxim::DatalogAll(DSIndex, pldResults);

	//PGND
	pldResults = apu12mv( PGND, 1, 13.0 );
	Maxim::DatalogAll(DSIndex, pldResults);

	//LEDGND
	pldResults = apu12mv( LEDGND, 1, 13.0 );
	Maxim::DatalogAll(DSIndex, pldResults);


	apu12set( SET5, APU12_FV, 0, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );	
	lwait (500 us_f);
	cbitopen(K11, K19);
	//---------------------------------------------------------------------------------




	//---------------------------------------------------------------------------------
	//EP,  Force 2V on limited current range.
	//Exposed pad
	//---------------------------------------------------------------------------------
	cbitclose(K8);
	lwait (3 ms_f);
	apu12set( BSTMON, APU12_FV, 2.0, APU12_3p6V, APU12_100UA, APU12_PIN_TO_VI );
	lwait (500 us_f);

	//EP
	pldResults = apu12mv( BSTMON, 1, 13.0 );
	Maxim::DatalogAll(DSIndex, pldResults);

	apu12set( BSTMON, APU12_FV, 0, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );
	cbitopen(K8);

	//---------------------------------------------------------------------------------


	apu12set( ALL_APU, APU12_OFF, 0, APU12_3p6V, APU12_1MA, APU12_PIN_TO_VI );







	gbLastContact = true;
	//Contact code will be automatically datalogged by the calling function.  Do Not call Maxim::Datalog(); here to flush the datalog.

//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// }		End of Contact Tests.  (Edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	alarmset( ALARM_CLAMP, ON );  //---- Turn alarms back on	
	// } Sample Code
	//}}CG_Contact
} 



//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// {		DataSheet Contact Function.  (Do not edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
//DO NOT EDIT THIS FUNCTION
/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
* Template_Contact
*   Purpose: Template's Contact function.  Called from the DataSheet.  Calls library and user functions to run the product specific contact function.
*	Arguments:	
*		[DSIndex]    DataSheet Index location passed in from the ETS Program Flow
*		[TestLabel]  DataSheet TestLabel passed in from the ETS Program Flow
*	Returns:
*		ETS Program Flow (int) status indicating if the program should continue. 
*   Remarks:
*		DO NOT EDIT THIS FUNCTION.  It calls the function above for the user to edit.
*			If you find a need to edit this function, please submit the changes to the ETS Template librarians so all future products can benefit from the changes.
*
*   Revision History:
*		
*                                                                            
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
// DO NOT EDIT THIS FUNCTION.
ETS_PRGFLOW_FUNC Template_Contact( int DSIndex, LPCTSTR TestLabel )
{
	return Maxim::Exec::Lib_Contact(DSIndex);  //Call the Maxim Library to call the above user contact function.
}	 // END_ETS_PRGFLOW_FUNC
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// }		DataSheet Contact Function.  (Do not edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+


