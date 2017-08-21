/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*                                                                            
*	Source title:		AP89_FT.cpp                                     
*		Created from Maxim Template                         
*		Template Written and maintained by:								 
*			Patrick Hayes (Patrick.Hayes@MaximIntegrated.com)	                 
*			Mark Bruening (Mark.Bruening@MaximIntegrated.com)                      
*
*	URL:				$URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/AP89_FT.cpp $
*	Last Modified by:	$LastChangedBy: govit.pimthong $                                       
*	Date:				$Date: 2017-07-12 14:02:24 +0700 (Wed, 12 Jul 2017) $                                                
*	Current Rev:		$Revision: 66719 $
*
*	Description:  Test Control Functions
*
*	Template Revision History
*		20110907 MWB: Initial Development and Release
*
*	User Revision History                                                    
*
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/

/**********************************************************************************************************************************************For*Doxygen**//**
@file	AP89_FT.cpp
@copydoc AP89_FT.h
**************************************************************************************************************************************************************/


#include "AP89_FT.h"


//Add any necessary globals to Globals.h and Globals.cpp


/// @cond DontDocument

//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// {		Eagle MST Wizard declarations   (Do not edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+

//////////////////////////////////////////////////////////////////////////////
// CAP89_FT Datasheet Variable Declarations
//
// The following declaration registers the test program with the test TestMain
// DLL. The value passed into the declaration specifies the Program Flow
// structure for the application. Passing in a value of ProductDatasheetFlow
// will direct the TestMain DLL to use the flow in the project's datasheet
// file.
//
// Keep the following from showing up in Help documents
CAP89_FT theApp;
//{{AFX_ETS_PRODUCT_DATASHEET_VARIABLES
DECLARE_PDS_VARIABLE( PDS_CHAR, Gate_Part, "Part Gating" );
DECLARE_PDS_VARIABLE( PDS_CHAR, Gate_Test_Step, "Test Step Gating" );
DECLARE_PDS_VARIABLE( PDS_TESTNUM, TestNmbr, "Test Number" );
DECLARE_PDS_VARIABLE( PDS_DLOGDESC, DLogDesc, "Test Name" );
DECLARE_PDS_VARIABLE( PDS_DFORMAT, DFormat, "Data Format" );
DECLARE_PDS_VARIABLE( PDS_UNITS, Units, "Units" );
DECLARE_PDS_VARIABLE( PDS_LOFBIN, LoFBin, "Low Fail Bin" );
DECLARE_PDS_VARIABLE( PDS_HIFBIN, HiFBin, "High Fail Bin" );
DECLARE_PDS_VARIABLE( PDS_DOUBLE, Offline_Answer, "Offline Answer" );
DECLARE_PDS_VARIABLE( PDS_LOLIM, L_CHAR, "CHAR" );
DECLARE_PDS_VARIABLE( PDS_HILIM, H_CHAR, "CHAR" );
DECLARE_PDS_VARIABLE( PDS_LOLIM, L_FT_ROOM, "FT_ROOM" );
DECLARE_PDS_VARIABLE( PDS_HILIM, H_FT_ROOM, "FT_ROOM" );
DECLARE_PDS_VARIABLE( PDS_LOLIM, L_QA_ROOM, "QA_ROOM" );
DECLARE_PDS_VARIABLE( PDS_HILIM, H_QA_ROOM, "QA_ROOM" );
DECLARE_PDS_VARIABLE( PDS_LOLIM, L_FT_HOT, "FT_HOT" );
DECLARE_PDS_VARIABLE( PDS_HILIM, H_FT_HOT, "FT_HOT" );
DECLARE_PDS_VARIABLE( PDS_LOLIM, L_QA_HOT, "QA_HOT" );
DECLARE_PDS_VARIABLE( PDS_HILIM, H_QA_HOT, "QA_HOT" );
DECLARE_PDS_VARIABLE( PDS_LOLIM, L_FT_COLD, "FT_COLD" );
DECLARE_PDS_VARIABLE( PDS_HILIM, H_FT_COLD, "FT_COLD" );
DECLARE_PDS_VARIABLE( PDS_LOLIM, L_QA_COLD, "QA_COLD" );
DECLARE_PDS_VARIABLE( PDS_HILIM, H_QA_COLD, "QA_COLD" );
DECLARE_PDS_VARIABLE( PDS_LOLIM, L_HW_CHECKER, "HW_CHECKER" );
DECLARE_PDS_VARIABLE( PDS_HILIM, H_HW_CHECKER, "HW_CHECKER" );
DECLARE_PDS_VARIABLE( PDS_REMARK, Notes, "Notes" );
//}}AFX_ETS_PRODUCT_DATASHEET_VARIABLES
//{{AFX_ETS_PIN_MAP_VARIABLES
// Caution::  The following pin definitions are filled in by the PinMap Editor.
//PMEVersion 1 1 ID 38 56 3;
DECLARE_PINMAP_VARIABLE(PME_INT, IN1, "");
DECLARE_PINMAP_VARIABLE(PME_INT, CS, "RT");
DECLARE_PINMAP_VARIABLE(PME_INT, EN, "OUTB");
DECLARE_PINMAP_VARIABLE(PME_INT, VCC, "FLT");
DECLARE_PINMAP_VARIABLE(PME_INT, COMP, "DIM");
DECLARE_PINMAP_VARIABLE(PME_INT, BSTMON, "EP");
DECLARE_PINMAP_VARIABLE(PME_INT, OUT1, "VCC_COMP");
DECLARE_PINMAP_VARIABLE(PME_INT, OUT2, "CS_COMP");
DECLARE_PINMAP_VARIABLE(PME_INT, OUT3, "NDRV");
DECLARE_PINMAP_VARIABLE(PME_INT, OUT4, "SDA");
DECLARE_PINMAP_VARIABLE(PME_INT, OUT5, "SCL");
DECLARE_PINMAP_VARIABLE(PME_INT, OUT6, "PGATE");
DECLARE_PINMAP_VARIABLE(PME_INT, FSEN, "IREF");
DECLARE_PINMAP_VARIABLE(PME_INT, PEDESTAL1, "LEDGND");
DECLARE_PINMAP_VARIABLE(PME_INT, PEDESTAL2, "PGND");
DECLARE_PINMAP_VARIABLE(PME_INT, QTMU_GP, "");
DECLARE_PINMAP_VARIABLE(PME_INT, K_POWER, "");
DECLARE_PINMAP_VARIABLE(PME_INT, K4, "");
DECLARE_PINMAP_VARIABLE(PME_INT, K5, "");
DECLARE_PINMAP_VARIABLE(PME_INT, K6, "");
DECLARE_PINMAP_VARIABLE(PME_INT, K7, "");
DECLARE_PINMAP_VARIABLE(PME_INT, K8, "");
DECLARE_PINMAP_VARIABLE(PME_INT, K9, "");
DECLARE_PINMAP_VARIABLE(PME_INT, K10, "");
DECLARE_PINMAP_VARIABLE(PME_INT, K11, "");
DECLARE_PINMAP_VARIABLE(PME_INT, K12, "");
DECLARE_PINMAP_VARIABLE(PME_INT, K13, "");
DECLARE_PINMAP_VARIABLE(PME_INT, K14, "");
DECLARE_PINMAP_VARIABLE(PME_INT, K15, "");
DECLARE_PINMAP_VARIABLE(PME_INT, K16, "");
DECLARE_PINMAP_VARIABLE(PME_INT, K17, "");
DECLARE_PINMAP_VARIABLE(PME_INT, K18, "");
DECLARE_PINMAP_VARIABLE(PME_INT, K19, "");
DECLARE_PINMAP_VARIABLE(PME_INT, K20, "");
DECLARE_PINMAP_VARIABLE(PME_INT, K21, "");
DECLARE_PINMAP_VARIABLE(PME_INT, K22, "");
DECLARE_PINMAP_VARIABLE(PME_INT, K23, "");
DECLARE_PINMAP_VARIABLE(PME_INT, K24, "");
DECLARE_PINMAP_VARIABLE(PME_INT, K25, "");
DECLARE_PINMAP_VARIABLE(PME_INT, K26, "");
DECLARE_PINMAP_VARIABLE(PME_INT, K27, "");
DECLARE_PINMAP_VARIABLE(PME_INT, K28, "");
DECLARE_PINMAP_VARIABLE(PME_INT, K29, "");
DECLARE_PINMAP_VARIABLE(PME_INT, K30, "");
DECLARE_PINMAP_VARIABLE(PME_INT, K31, "");
//}}AFX_ETS_PIN_MAP_VARIABLES

//{{AFX_ETS_GROUP_MAP_VARIABLES
// Caution:: The following group definitions are filled in by the PinMap Editor
DECLARE_GROUPMAP_VARIABLE(GME_INT, RT, "");
DECLARE_GROUPMAP_VARIABLE(GME_INT, ALL_OUT, "");
DECLARE_GROUPMAP_VARIABLE(GME_INT, OUTB, "");
DECLARE_GROUPMAP_VARIABLE(GME_INT, FLT, "");
DECLARE_GROUPMAP_VARIABLE(GME_INT, DIM, "");
DECLARE_GROUPMAP_VARIABLE(GME_INT, EP, "");
DECLARE_GROUPMAP_VARIABLE(GME_INT, VCC_COMP, "");
DECLARE_GROUPMAP_VARIABLE(GME_INT, CS_COMP, "");
DECLARE_GROUPMAP_VARIABLE(GME_INT, NDRV, "");
DECLARE_GROUPMAP_VARIABLE(GME_INT, SDA, "");
DECLARE_GROUPMAP_VARIABLE(GME_INT, SCL, "");
DECLARE_GROUPMAP_VARIABLE(GME_INT, PGATE, "");
DECLARE_GROUPMAP_VARIABLE(GME_INT, IREF, "");
DECLARE_GROUPMAP_VARIABLE(GME_INT, LEDGND, "");
DECLARE_GROUPMAP_VARIABLE(GME_INT, PGND, "");
DECLARE_GROUPMAP_VARIABLE(GME_INT, BUSB, "");
DECLARE_GROUPMAP_VARIABLE(GME_INT, ALL_APU, "");
DECLARE_GROUPMAP_VARIABLE(GME_INT, SET1, "");
DECLARE_GROUPMAP_VARIABLE(GME_INT, SET2, "");
DECLARE_GROUPMAP_VARIABLE(GME_INT, SET3, "");
DECLARE_GROUPMAP_VARIABLE(GME_INT, SET5, "");
//}}AFX_ETS_GROUP_MAP_VARIABLES
/// @endcond DontDocument


//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// }		Eagle MST Wizard declarations   (Do not edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+

//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// {		Map Test Step Names to Limit Column  (Edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
/************************************************************************************************************************************************************For*Doxygen**//**
Call Maxim::Limits::TestLimits using the above ETS Product DataSheet Variables.  
	This maps the test steps provided in the Load file to the proper limits columns on the DataSheet. 

@remarks
			Every Test Step listed on the product's Setup Sheet must appear in the Load File.  Every Test Step and part number combination in the Load File must
			be mapped to a set of limits columns using the TestLimits function.

An Example of lines you may put in this function
@code
	//FT examples where all part types use the same limit sets.
	Maxim::Limits::TestLimits("FT_ROOM",L_FT_ROOM, H_FT_ROOM, "MAX8556");
	Maxim::Limits::TestLimits("FT_ROOM",L_FT_ROOM, H_FT_ROOM, "MAX8557");
	Maxim::Limits::TestLimits("QA_ROOM",L_QA_ROOM, H_QA_ROOM, "MAX8556");
	Maxim::Limits::TestLimits("QA_ROOM",L_QA_ROOM, H_QA_ROOM, "MAX8557");
	Maxim::Limits::TestLimits("FT_HOT",L_FT_HOT, H_FT_HOT, "MAX8556");
	Maxim::Limits::TestLimits("FT_HOT",L_FT_HOT, H_FT_HOT, "MAX8557");
	Maxim::Limits::TestLimits("QA_HOT",L_QA_HOT, H_QA_HOT, "MAX8556");
	Maxim::Limits::TestLimits("QA_HOT",L_QA_HOT, H_QA_HOT, "MAX8557");
	Maxim::Limits::TestLimits("FT_COLD",L_FT_COLD, H_FT_COLD, "MAX8556");
	Maxim::Limits::TestLimits("FT_COLD",L_FT_COLD, H_FT_COLD, "MAX8557");
	Maxim::Limits::TestLimits("QA_COLD",L_QA_COLD, H_QA_COLD, "MAX8556");
	Maxim::Limits::TestLimits("QA_COLD",L_QA_COLD, H_QA_COLD, "MAX8557");
	
	//Example where different parts use different limit sets.
	Maxim::Limits::TestLimits("CHAR",L_CHAR_MAX8556, H_CHAR_MAX8556,"MAX8556");
	Maxim::Limits::TestLimits("CHAR",L_CHAR_MAX8557, H_CHAR_MAX8557, "MAX8557");	

	//WLP/CSP examples
	//Maxim::Limits::TestLimits("PST_BUMP",L_PST_BUMP, H_PST_BUMP, "Part Number");
	//Maxim::Limits::TestLimits("QC",L_QC, H_QC, "Part Number");	

	//Hardware Checker Limits
	Maxim::Limits::TestLimits("HW_CHECKER",L_HW_CHECKER, H_HW_CHECKER, "MAX8556");
	Maxim::Limits::TestLimits("HW_CHECKER",L_HW_CHECKER, H_HW_CHECKER, "MAX8557");
@endcode
**************************************************************************************************************************************************************/
void Maxim_Map_Test_Steps_To_Limits(void)
{
#ifdef MAXIM_VERBOSE
	etsprintf("Mapping Test Steps and Part Numbers to Datasheet columns\n");
#endif	
	//{{CG_Map_Test_Steps_To_Limits


	//FT examples where all part types use the same limit sets.
	Maxim::Limits::TestLimits("FT_ROOM",L_FT_ROOM, H_FT_ROOM, "MAX20446");
	Maxim::Limits::TestLimits("QA_ROOM",L_QA_ROOM, H_QA_ROOM, "MAX20446");
	Maxim::Limits::TestLimits("FT_HOT",L_FT_HOT, H_FT_HOT, "MAX20446");
	Maxim::Limits::TestLimits("QA_HOT",L_QA_HOT, H_QA_HOT, "MAX20446");
	Maxim::Limits::TestLimits("FT_COLD",L_FT_COLD, H_FT_COLD, "MAX20446");
	Maxim::Limits::TestLimits("QA_COLD",L_QA_COLD, H_QA_COLD, "MAX20446");
	Maxim::Limits::TestLimits("CHAR",L_CHAR, H_CHAR, "MAX20446");

	//Hardware Checker Limits
	Maxim::Limits::TestLimits("HW_CHECKER",L_HW_CHECKER, H_HW_CHECKER, "MAX20446");

	//}}CG_Map_Test_Steps_To_Limits
}
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// }		Map Test Step Names to Limit Column (Edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+

//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// {		Product Specific Initialization  (Edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
/************************************************************************************************************************************************************For*Doxygen**//**
This function is called by UserInit to allow the Template user to add product specific initialization items. 

@returns	Return TRUE if successful.  Returning FALSE will cause the program to abort loading.

@remarks
		This function is called at the end of the UserInit function, after all of the required Maxim items have been initialized.
		It is called after the call to the  Maxim_Setup_Digital_Levels_And_Timing function and after the call to the Create_Pin_Groups function
		See those functions for additional information.   
		Also see ETS Help file "TestMain Functions" for more information for items to initialize in Maxim_UserInit.

**************************************************************************************************************************************************************/
BOOL Maxim_UserInit(void)
{
	//Use function Maxim_Setup_Digital_Levels_And_Timing for any digital system setup.
	//Use function Create_Pin_Groups to create analog pin groups that are not created in the Pin Map.
	//Add further product specific User Initialization items here.  
	//If non-standard meter settings are required, call Maxim::Meter::DMM::Config() here

	//{{CG_UserInit


	//Create Random batch number, once for each program load
	//srand(time(NULL));
	srand ( (unsigned int)time(NULL) ); // Use this instead to get rid of warning C4244; Govit Jul-13-2017

	giRandomBatchCode = rand() % 127;

	//Electronically mark as sample if not using production operator interface.
	if (Maxim::Operator())
		gbSample = false;
	else
		gbSample = true;


	return true;	
	//}}CG_UserInit
}
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// }		Product Specific Initialization  (Edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+


















/*****************************************************************************
*                                                                            *
*                           Main-support functions                           *
*                                                                            *
*                       Nothing needs to be edited below                     *
*                       this line in the Maxim Template.                     *
*                                                                            *
*****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  The UserInit() function is called by the 'TestMain' module one time     //
//  near the beginning of test execution, after all Global variables and    //
//  Global Switches have been initialized.                                  //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// {		Maxim Template  (Do NOT edit this section)
//				 Use function Maxim_UserInit above to add any additional items to CAP89_FT::UserInit
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
BOOL CAP89_FT::UserInit( void )
{
	//Call user defined function to setup the available limits.
	Maxim_Map_Test_Steps_To_Limits();

	//Initialize other items:  Number of sites, Load vector files, Initialize all sites, choose Test Limits, initialize Agilent Meter.
	if (!Maxim::Exec::Lib_UserInit(NUM_SITES, true, true, GPSW_REAL_TIME_DATALOG)) return FALSE;	//Exit loading Test program if Lib_UserInit failed.

	//Initialize the datalogging functions
#ifdef MAXIM_USE_OFFLINE_ANSWERS	//See top of file
	if (Maxim::DatalogInit( TestNmbr, TestNmbr, DLogDesc, DFormat, Maxim::Limits::GetLoLimit(), Maxim::Limits::GetHiLimit(), Units, LoFBin, HiFBin,Maxim::Limits::GetLoLimit(),Maxim::Limits::GetHiLimit(), NUM_SITES, Offline_Answer, DSSIZE) == FAILURE)
		 return FALSE;	//Exit loading Test program if msLogResultInit failed.
#else
	//Used if the DataSheet does not have an Offline Answer column.
	if (Maxim::DatalogInit( TestNmbr, TestNmbr, DLogDesc, DFormat, Maxim::Limits::GetLoLimit(), Maxim::Limits::GetHiLimit(), Units, LoFBin, HiFBin,Maxim::Limits::GetLoLimit(),Maxim::Limits::GetHiLimit(), NUM_SITES, DSSIZE ) == FAILURE)
		return FALSE;	//Exit loading Test program if msLogResultInit failed.
#endif
	
	//Initialize datasheet Gating items
	Maxim::init_DataSheet_Gating(Gate_Part, Gate_Test_Step);


	//Call any product specific items that still need to be initialized.
	//Create product specific pin groups	
	if (!Create_Pin_Groups()) 
		return FALSE;	//Exit loading Test program if failed.

	//Product specific digital setup
	if (!Maxim_Setup_Digital_Levels_And_Timing())
		return FALSE;	//Exit loading Test program if failed.
		
	//Any additional product specific items
	if (!Maxim_UserInit()) 
		return FALSE;	//Exit loading Test program if failed.
		
	// Returning FALSE will abort Loading of Test program and exit Test Executive.
	return TRUE;	
}		
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// }		Maxim Template  (Do NOT edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+



//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  The UserExit() function is called by the 'TestMain' module one time     //
//  near the end of test execution, before the Global variables and         //
//  Global Switches are saved.                                              //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
BOOL CAP89_FT::UserExit( void )
{
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// {		Maxim Template  (Do NOT edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	if (!Maxim::Exec::Lib_UserExit())  
		return FALSE;
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// }		Maxim Template  (Do NOT edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  The OnTestInit() function is called before the display of the Test      //
//  Main Menu to reinitialize the DUT and/or reinitialize any program       //
//  variables that might need to be reset.  This function will be invoked   //
//  prior to calling the first function in the progflow structure.          //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
BOOL CAP89_FT::OnTestInit( void )
{
	//Add global variable initialization code to the Maxim_OnTestInit function in the Globals.cpp file.
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// {		Maxim Template  (Do NOT edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	BOOL bTestInitPass;
	bTestInitPass = Maxim::Exec::Lib_OnTestInitPre();

	//Call product specific Maxim_OnTestInit code
	bTestInitPass = Maxim_OnTestInit() && bTestInitPass;		

	if (!Maxim::Exec::Lib_OnTestInitPost() || (!bTestInitPass))	
		return FALSE;  // Returning FALSE will abort Program flow execution and return to Test/Quit prompt.
	else
		return TRUE;	
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// }		Maxim Template  (Do NOT edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  The OnTestExit() function is called after the the selection of the      //
//  Quit button from the "Test", "Quit" Buttons Screen to reinitialize any  //
//  program variables or system resources that need to be "cleaned-up".     //
//  This function will be invoked regardless of the pass/fail status of     //
//  the DUT.                                                                //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
BOOL CAP89_FT::OnTestExit( void )
{
	//Add power down code to the Maxim_OnTestExit function in the Globals.cpp file.
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// {		Maxim Template  (Do NOT edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	BOOL bTestExitPass;
	bTestExitPass = Maxim::Exec::Lib_OnTestExitPre();

	//Call product specific Maxim_OnTestExit code
	bTestExitPass = Maxim_OnTestExit() && bTestExitPass;		

	if (!Maxim::Exec::Lib_OnTestExitPost() || (!bTestExitPass))	
		return FALSE;  // Returning FALSE will abort Program flow execution and return to Test/Quit prompt.
	else
		return TRUE;	
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// }		Maxim Template  (Do NOT edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  The OnTestStartup() function is called before each test-run to          //
//  initialize any program variables or system resources that might need to //
//  be reset each time the test is run as opposed to only at the time of    //
//  program entry as in the OnTestInit() routine. This function will be     //
//  invoked prior to calling the first function in the progflow structure.  //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
BOOL CAP89_FT::OnTestStartup( void )
{
	//Add power up code to the Maxim_OnTestStartup function in the Globals.cpp file.
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// {		Maxim Template  (Do NOT edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+	
	BOOL bTestStartupPass;
	bTestStartupPass = Maxim::Exec::Lib_OnTestStartupPre(GetUserSwitch(GPSW_Time_Tests));  //Enable Test Time per Test if enabled by the Shell's user switch.

	//Call product specific Maxim_OnTestStartup code
	bTestStartupPass = Maxim_OnTestStartup() && bTestStartupPass;		

	if (!Maxim::Exec::Lib_OnTestStartupPost() || (!bTestStartupPass))	
		return FALSE;  // Returning FALSE will abort Program flow execution and return to Test/Quit prompt.
	else
		return TRUE;	
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// }		Maxim Template  (Do NOT edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  The OnTestCompletion() function is called after each test-run to        //
//  re-initialize any program variables or system resources that might need //
//  to be reset after each test suite is run This function will be invoked  //
//  after to calling the last function in the progflow structure.           //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
BOOL CAP89_FT::OnTestCompletion( void )
{
	//Add power down code to the Maxim_OnTestCompletion function in the Power_Down.cpp file.
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// {		Maxim Template  (Do NOT edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	BOOL bPrePowerDownPass;
	bPrePowerDownPass = Maxim::Exec::Lib_OnTestCompletionPrePowerDown();

	//Power down (all sites) to a safe state.
	bPrePowerDownPass = Maxim_OnTestCompletion() && bPrePowerDownPass;		

	if (!Maxim::Exec::Lib_OnTestCompletionPostPowerDown() || (!bPrePowerDownPass))	//Calculate elapsed test time if enabled in Maxim::Exec::Lib_OnTestStartup.
		return FALSE;
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// }		Maxim Template  (Do NOT edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  The OnLogData() function is called During a call to the LogData(),      //
//  msLogData() and msLogDataAll() functions.                               //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
BOOL CAP89_FT::OnLogData(int site, datalog_info_t& info  )
{

//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// {		Maxim Template  (Do NOT edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	if (!Maxim::Exec::Lib_OnLogData(site, info))	//Determine if an alarm occurred.  While debugging, place a breakpoint inside this function to determine the alarm.
		return FALSE;
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// }		Maxim Template  (Do NOT edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  The OnFailSite() function is called During a call to the msFailSite()   //
//  to enable the user to turn off and invalidate a particular site         //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
BOOL CAP89_FT::OnFailSite( int site )
{
	//Add power down code to the Maxim_OnFailSite function in the Power_Down.cpp file.
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// {		Maxim Template  (Do NOT edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	BOOL bPrePowerDownPass;
	bPrePowerDownPass = Maxim::Exec::Lib_OnFailSitePrePowerDown(site);

	//Power down (failing sites) to a safe state.
	bPrePowerDownPass = Maxim_OnFailSite(site) && bPrePowerDownPass;

	if (!Maxim::Exec::Lib_OnFailSitePostPowerDown(site) || (!bPrePowerDownPass))	
		return FALSE;
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// }		Maxim Template  (Do NOT edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  The OnSot() function is called while waiting for a valid SOT            //
//  signal either from the Operator Box, Handler, or user interface.        //
//  SotValue contains the SOT event.                                        //
//                                                                          //
//    -2  =   Handler signaled SOT                                          //
//    -1  =   Operator Box SOT switch Pressed                               //
//    'T' =   Key Board 'T'/ or Test Button Pressed                         //
//    'Q' =   Key Board 'Q'/ or Quit Button Pressed                         //
//    F1_KEY - F5_KEY  = special Test Key/Button Pressed                    //
//    F6_KEY - F10_KEY = Datalog Mode/Binning Summary State                 //
//                       Change requested                                   //
//                                                                          //
//  The SotValue is the virtual key code of the key                         //
//  hit on the keyboard (NOT ASCII CODE). When trapping specific            //
//  key hits, use the windows defines for the virtual key codes. These      //
//  defines are: (alpha codes A through Z)   VK_A, VK_B, ..., VK_Z             //
//               (numeric codes 0 through 9) VK_0, VK_1, ..., VK_9             //
//               (function key codes F1 - F12) F1_KEY, F2_KEY, ..., F12_KEY //
//                                                                          //
//  Upon return from this function, the TestMain Framework will take action //
//  on the SotValue.                                                        //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
int CAP89_FT::OnSot(int SotValue)
{
	int iRetVal;
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// {		Maxim Template  (Do not edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	iRetVal = Maxim::Exec::Lib_OnSot(SotValue, GPSW_SITE0, MAX_SITE_SWITCH, GPI_LoopCount, GPI_Loop_Delay,  Maxim::HWC::bGet_Checker_Results());	//Process the SOT value.  
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// }		Maxim Template  (Do not edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+

	return iRetVal;
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  The OnUtilError function is called in response to an ETS Utility        //
//  error. The ETS Utility Error Number is passed in to the user so         //
//  certain errors can be trapped.                                          //
//                                                                          //
//  Returning TRUE will pass the error into the ETS Message Handler         //
//  program, and returning FALSE will direct the Message Handler to ignore  //
//  the error.                                                              //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
BOOL CAP89_FT::OnUtilError(int errnum)
{
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// {		Maxim Template  (Do NOT edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	if (!Maxim::Exec::Lib_OnUtilError(errnum))	
		return FALSE;
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// }		Maxim Template  (Do NOT edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+


	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//  This function will automatically be called each time the interlock is   //
//  checked by the Test Executive. The interlock is checked before each DUT   //
//  tested, before the call to the user's OnTestStartup() routine. The      //
//  intlocks parameter passed in is a bit mapped value containing the state  //
//  of all interlocks in the ETS Test head.                                 //
//                                                                          //
//  Returning TRUE will pass the interlock check. Returning FALSE will fail  //
//  the interlock check. If the intlocks variable is non-zero the return    //
//  value has no effect. If the interlock check fails, the DUT will not be  //
//  tested.                                                                 //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
BOOL CAP89_FT::OnInterlock(ULONG intlocks)
{
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// {		Maxim Template  (Do NOT edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	if (!Maxim::Exec::Lib_OnInterlock(intlocks))	
		return FALSE;
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// }		Maxim Template  (Do NOT edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+

	return TRUE;
}

//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
//		For ease of Template reuse and update, nothing should appear below this line (Add test functions to new CPP files.)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+

