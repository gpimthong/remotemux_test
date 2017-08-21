/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*                                                                            
*	Source title:		AP89_FT.h                                    
*		Created from Maxim Template                         
*		Template Written and maintained by:								 
*			Patrick Hayes (Patrick.Hayes@MaximIntegrated.com)	                 
*			Mark Bruening (Mark.Bruening@MaximIntegrated.com)                      
*		  
*						URL:  $URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/AP89_FT.h $
*	Last Modified by:	$LastChangedBy: govit.pimthong $                                       
*	Date:				$Date: 2017-07-31 06:41:45 +0700 (Mon, 31 Jul 2017) $                                                
*	Current Rev:		$Revision: 67435 $
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
@file	AP89_FT.h
@brief Test control functions and settings that relate to your specific product.

The ETS Wizard that creates a new project will rename this file based on the die type to be [DieType].*, for example PX01.h and PX01.cpp.

In the help files, this may also be called the main.cpp and/or main.h files.
**************************************************************************************************************************************************************/	

/************************************************************************************************************************************************************For*Doxygen**//**
@page guideTemplate Guide to Using ETS-88 Template
@details 

Confluence for [ETS-88_Template](https://confluence.maxim-ic.com/display/ETS88/ETS-88+Template)

**************************************************************************************************************************************************************/

#ifndef  AP89_FT_H
#define AP89_FT_H
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// {		#defines for library compile options  (Edit this section if required)
//                      Note: User #defines that only affect your program should generally be put in globals.h
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+

	//#define	MAXIM_VERBOSE	//Uncomment for extra debug messages.


	//Pre-release library features which can be enabled by uncommenting the appropriate lines
	//Since these are pre-release there is a chance that the feature will change before final release.  Not to worry, as you could always rename and keep a local copy
	//		of the pre-release function if a conflict arises.  
	//It is recommended you contact the developer of any prerelease library before using it.

	//ALPHA indicates that code is still in concept phase and there may be significant changes
	//BETA indicates that code has undergone some level of testing and is stable enough that others may want to start testing it.
	//PRERELEASE indicates that the code is pretty much ready to be released, but perhaps is missing documentation, error handling, or some other smaller issues.
	//#define ALPHA_PINLIST
	//#define ALPHA_CALIBRATE
	//#define ALPHA_MEASURE

//#define DATALOG_CHECK // If defined, then a warning will be used if Datalog() (no parameters) is used instead of DatalogFlush()

//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// }		#defines 
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+


//Standard include files
#include <string>


#include "./MaximFuncLib/MaximFuncLib.h"	//Maxim Function Library
#include "Globals.h"						//Globals.h should come after the library #includes in case it makes use of classes defined in the libraries.
#include "GlobalTrims.h"					//GlobalTrims.h should come after Globals.h
// Include any .h files from the test program here, especially if functions inside the file need to be used in other files
#include "Power_Down.h"
#include "Digital_Levels_And_Timing.h"
#include "AP89_FT_Utils.h"

//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// {		General Purpose Switches, Integers, Strings   (Edit this section if required)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+

//General Purpose Switches
//Switches 1 - 9 are reserved for the Maxim Library. 

#define     GPSW_BURN		  				10	//Available switch for product specific use.  
#define     GPSW_ForceSim	  				11	//Available switch for product specific use.  
#define     GPSW_BitWeightChar 				12	//Available switch for product specific use.  
#define     GPSW_USER4		  				13	//Available switch for product specific use.  
#define     GPSW_USER5		  				14	//Available switch for product specific use. 

//Site Switches are using switch 15 - 31
//#define     GPSW_NEW_SWITCH				MAX_SITE_SWITCH + 1	//Example define of new switch. 


//General Purpose Integers:   1 - 5 are reserved for the Maxim Library. 

//Define more General Purpose Integers here


//General Purpose Strings:   1 - 6 are reserved for the Maxim Library. 

//Define more General Purpose Strings here



//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// }		General Purpose Switches, Integers, Strings   (Edit this section if required)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+

//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// {		Eagle MST external Datasheet variable declarations   (Edit this section if required)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+

//External PDS variables should rarely be used.  
//The Maxim Template and MaximLib provide access to the Limits, Datasheet Gating, and other items.  
//Please investigate the available library functions before adding External PDS variables.  
//Please contact the librarians if you need access to a PDS variable that is not available from the library.  
//This way, commonly accessed items can be added to the library.

//////////////////////////////////////////////////////////////////////////////
// CAP89_FT Datasheet Variable Declarations
//{{AFX_ETS_PRODUCT_DATASHEET_VARIABLES

//}}AFX_ETS_PRODUCT_DATASHEET_VARIABLES

//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// }		Eagle MST external Datasheet variable declarations   (Edit this section if required)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+


//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// {		Eagle MST Wizard declarations   (Do not edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
//////////////////////////////////////////////////////////////////////////////
// CAP89_FT:
// See AP89_FT.cpp for the implementation of this class
//
//{{AFX_DEFINE_ACE_GROUP( Datasheet Array Size )
#define DSSIZE 1574
//}}AFX_DEFINE_ACE_GROUP
//{{AFX_ETS_EXTERN_PIN_MAP_VARIABLES
// Caution::  The following pin definitions are filled in by the PinMap Editor.
EXTERN_PINMAP_VARIABLE(PME_INT, IN1, "");
EXTERN_PINMAP_VARIABLE(PME_INT, CS, "RT");
EXTERN_PINMAP_VARIABLE(PME_INT, EN, "OUTB");
EXTERN_PINMAP_VARIABLE(PME_INT, VCC, "FLT");
EXTERN_PINMAP_VARIABLE(PME_INT, COMP, "DIM");
EXTERN_PINMAP_VARIABLE(PME_INT, BSTMON, "EP");
EXTERN_PINMAP_VARIABLE(PME_INT, OUT1, "VCC_COMP");
EXTERN_PINMAP_VARIABLE(PME_INT, OUT2, "CS_COMP");
EXTERN_PINMAP_VARIABLE(PME_INT, OUT3, "NDRV");
EXTERN_PINMAP_VARIABLE(PME_INT, OUT4, "SDA");
EXTERN_PINMAP_VARIABLE(PME_INT, OUT5, "SCL");
EXTERN_PINMAP_VARIABLE(PME_INT, OUT6, "PGATE");
EXTERN_PINMAP_VARIABLE(PME_INT, FSEN, "IREF");
EXTERN_PINMAP_VARIABLE(PME_INT, PEDESTAL1, "LEDGND");
EXTERN_PINMAP_VARIABLE(PME_INT, PEDESTAL2, "PGND");
EXTERN_PINMAP_VARIABLE(PME_INT, QTMU_GP, "");
EXTERN_PINMAP_VARIABLE(PME_INT, K_POWER, "");
EXTERN_PINMAP_VARIABLE(PME_INT, K4, "");
EXTERN_PINMAP_VARIABLE(PME_INT, K5, "");
EXTERN_PINMAP_VARIABLE(PME_INT, K6, "");
EXTERN_PINMAP_VARIABLE(PME_INT, K7, "");
EXTERN_PINMAP_VARIABLE(PME_INT, K8, "");
EXTERN_PINMAP_VARIABLE(PME_INT, K9, "");
EXTERN_PINMAP_VARIABLE(PME_INT, K10, "");
EXTERN_PINMAP_VARIABLE(PME_INT, K11, "");
EXTERN_PINMAP_VARIABLE(PME_INT, K12, "");
EXTERN_PINMAP_VARIABLE(PME_INT, K13, "");
EXTERN_PINMAP_VARIABLE(PME_INT, K14, "");
EXTERN_PINMAP_VARIABLE(PME_INT, K15, "");
EXTERN_PINMAP_VARIABLE(PME_INT, K16, "");
EXTERN_PINMAP_VARIABLE(PME_INT, K17, "");
EXTERN_PINMAP_VARIABLE(PME_INT, K18, "");
EXTERN_PINMAP_VARIABLE(PME_INT, K19, "");
EXTERN_PINMAP_VARIABLE(PME_INT, K20, "");
EXTERN_PINMAP_VARIABLE(PME_INT, K21, "");
EXTERN_PINMAP_VARIABLE(PME_INT, K22, "");
EXTERN_PINMAP_VARIABLE(PME_INT, K23, "");
EXTERN_PINMAP_VARIABLE(PME_INT, K24, "");
EXTERN_PINMAP_VARIABLE(PME_INT, K25, "");
EXTERN_PINMAP_VARIABLE(PME_INT, K26, "");
EXTERN_PINMAP_VARIABLE(PME_INT, K27, "");
EXTERN_PINMAP_VARIABLE(PME_INT, K28, "");
EXTERN_PINMAP_VARIABLE(PME_INT, K29, "");
EXTERN_PINMAP_VARIABLE(PME_INT, K30, "");
EXTERN_PINMAP_VARIABLE(PME_INT, K31, "");
//}}AFX_ETS_EXTERN_PIN_MAP_VARIABLES

//{{AFX_ETS_EXTERN_GROUP_MAP_VARIABLES
// Caution:: The following group definitions are filled in by the PinMap Editor
EXTERN_GROUPMAP_VARIABLE(GME_INT, RT, "");
EXTERN_GROUPMAP_VARIABLE(GME_INT, ALL_OUT, "");
EXTERN_GROUPMAP_VARIABLE(GME_INT, OUTB, "");
EXTERN_GROUPMAP_VARIABLE(GME_INT, FLT, "");
EXTERN_GROUPMAP_VARIABLE(GME_INT, DIM, "");
EXTERN_GROUPMAP_VARIABLE(GME_INT, EP, "");
EXTERN_GROUPMAP_VARIABLE(GME_INT, VCC_COMP, "");
EXTERN_GROUPMAP_VARIABLE(GME_INT, CS_COMP, "");
EXTERN_GROUPMAP_VARIABLE(GME_INT, NDRV, "");
EXTERN_GROUPMAP_VARIABLE(GME_INT, SDA, "");
EXTERN_GROUPMAP_VARIABLE(GME_INT, SCL, "");
EXTERN_GROUPMAP_VARIABLE(GME_INT, PGATE, "");
EXTERN_GROUPMAP_VARIABLE(GME_INT, IREF, "");
EXTERN_GROUPMAP_VARIABLE(GME_INT, LEDGND, "");
EXTERN_GROUPMAP_VARIABLE(GME_INT, PGND, "");
EXTERN_GROUPMAP_VARIABLE(GME_INT, BUSB, "");
EXTERN_GROUPMAP_VARIABLE(GME_INT, ALL_APU, "");
EXTERN_GROUPMAP_VARIABLE(GME_INT, SET1, "");
EXTERN_GROUPMAP_VARIABLE(GME_INT, SET2, "");
EXTERN_GROUPMAP_VARIABLE(GME_INT, SET3, "");
EXTERN_GROUPMAP_VARIABLE(GME_INT, SET5, "");
//}}AFX_ETS_EXTERN_GROUP_MAP_VARIABLES



//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// }		Eagle MST Wizard declarations   (Do not edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+

//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// {		Eagle MST standard declarations   (Do not edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+


































//{{AFX_ETS_FUNCTION_PROTOTYPES
ETS_PRGFLOW_FUNC Template_Hardware_Checker( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC Template_Contact( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC Preliminary_Check( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC OTP_Trim( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC OTP_Burn( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC SP_Trim( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC Device_ID( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC OTP_ReadBack( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC Leakage( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC Supply_Current( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC OTP_Post( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC Power_Input( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC Logic_Input( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC SCAN( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC Error_Amp( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC Boost_DAC( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC Servo( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC OverVoltage_UnderVoltage( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC CurrentSense_Comp( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC MOSFET_Driver( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC VCC_Regulator( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC RT_Oscillator( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC Freq_Dither( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC Ext_Sync( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC LED_Current_Sink( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC ISET_DAC( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC LED_Fault_Detection( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC LED_Current_Measurement( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC Thermal_Shutdown( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC DIAG( int DSIndex, LPCTSTR TestLabel );
ETS_PRGFLOW_FUNC I2C_Interface( int DSIndex, LPCTSTR TestLabel );
//}}AFX_ETS_FUNCTION_PROTOTYPES

class CAP89_FT : public CETSEvents
{
public:
	CAP89_FT() : CETSEvents() {};
	~CAP89_FT(){};
//////////////////////////////////////////////////////////////////////////////
// Overridable Functions
//
//  Function body in the AP89_FT.cpp file
//
	virtual BOOL  UserInit();                   // Called when user starts Application Program
	virtual BOOL  UserExit();                   // Called when user quits Application Program
	virtual BOOL  OnTestStartup();              // Called Before Program Flow Structure Execution
	virtual BOOL  OnTestCompletion();           // Called After Program Flow Structure Execution
	virtual BOOL  OnTestInit();                 // Called before 'GO / QUIT' buttons Displayed
	virtual BOOL  OnTestExit();                 // Called after 'QUIT' button pressed
	virtual BOOL  OnLogData( int site, datalog_info_t& info); // Called during Data logging
	virtual BOOL  OnFailSite(int site);         // Called during the msFailSite() utility
	virtual int   OnSot(int SotValue);          // Called while waiting for Start Of Test signal
	virtual BOOL  OnUtilError(int errnum);      // Called by the Utilities when an ETS Util Error occurs
	virtual BOOL  OnInterlock(ULONG intlocks);  // Called during the interlock check...
};

//////////////////////////////////////////////////////////////////////////////
//
// ACE Define Block
//
// ETS AceWizard generated define blocks
//
//{{AFX_DEFINE_ACE_GROUP( Miscellaneous Defines )
	//The FOR_EACH_SITE macro was defined here by Eagle.  However, it causes problems with the drop down function list in Visual Studio.  
	//Best Practice: do not use the FOR_EACH_SITE macro.  Use ForEachActive_iSite instead


	//ForEachActive_iSite
	//{
		//Code goes here
	//}


//}}AFX_DEFINE_ACE_GROUP

//{{AFX_DEFINE_ACE_GROUP( Group Block 1 )
	// NOTE: Add define members here for the group "Group Block 1"
    #define NUM_SITES 4
//}}AFX_DEFINE_ACE_GROUP

//{{AFX_DEFINE_ACE_GROUP( Group Block 2 )
	// NOTE: Add define members here for the group "Group Block 2"
//}}AFX_DEFINE_ACE_GROUP

//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// }		Eagle MST standard declarations   (Do not edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+

extern int din1[NUM_SITES];		
extern int din2[NUM_SITES];		
extern int direct_v[NUM_SITES];	



//*****************************************
// test structure for threshold test
//*****************************************

struct THForcePin{

	PME_INT pinName;
	int VRange;
	int IRange;
	int ForceMode;
	int ConnectMode;

};

struct THMeasPin{

	PME_INT pinName;
	int VRange;
	int IRange;
	int MeasMode;


};

struct TestTH	
{
	THForcePin SetPin;
	THMeasPin MeasPin;
	void (*TH_Set)(SiteDouble);
	PinListData (*TH_Meas)(void);
	LPCTSTR AwgNameR;
	LPCTSTR AwgNameF;
	bool bUseAWG;
	int MCU_Divider;
	int ThresholdDirection; //for rising only, falling will be the opposite of this
	double step;		// resolution step value
	double outTh1;		// output thresold for rising
	double outTh2;		// output thresold for falling
	int SampleToSkip;
	bool FromRight;
	bool PercentDatalog;
	double RefValue;
	double Offset;
	PinListData pldRefValue;
	bool dFallingSearch;
};

extern void ThresholdTest(int &DSIndex, TestTH *ts);
extern void OUT_SG_ThresholdTest(int &DSIndex, TestTH *ts);
extern void OUT_Unused_ThresholdTest(int &DSIndex, TestTH *ts);
extern void OUT_SHRT_ThresholdTest(int &DSIndex, TestTH *ts);


#endif	//end of #ifndef AP89_FT_H


#include "AP89_FT_Awg.h"

