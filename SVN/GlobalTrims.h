/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*                                                                            
*	Source title:		GlobalTrims.h                                    
*		Created from Maxim Template                         
*       Template Written and maintained by:								 
*			Patrick Hayes (Patrick.Hayes@MaximIntegrated.com)	                 
*			Mark Bruening (Mark.Bruening@MaximIntegrated.com)                      
*		  
*	URL:				$URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/GlobalTrims.h $
*   Last Modified by:	$LastChangedBy: govit.pimthong $                                       
*	Date:				$Date: 2017-05-30 22:00:32 +0700 (Tue, 30 May 2017) $                                                
*   Current Rev:		$Revision: 65091 $
*                                                                            
*	Description:  User defined global variables, pin groups, and #defines used for trimming this specific product.  
*                                                                            
*   Template Revision History                                                    
*
*   User Revision History                                                    
*                                                                            
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/

/**********************************************************************************************************************************************For*Doxygen**//**
@file	GlobalTrims.h
@brief	Trim related user defined global variables, pin groups, functions, etc.  
**************************************************************************************************************************************************************/

namespace DUT
{

/************************************************************************************************************************************************************For*Doxygen**//**
@brief Make declarations for trim related items.
@details Use the code below as a template to make declarations for your trims in GlobalTrims.h.  
Declarations should start inside the "namespace Trim { }" area

@code
	//Declare an object and measurement function for each trim that will use Maximfunclib_OTP routines
	extern Maxim::OTP::CTrimStructure gtsIBias;  // Declare a trim object for bias current.  Recommend a seperate object for each trim.
	PinListData MeasureIBias (Maxim::OTP::CTrimStructure * tsTrim);			 // Declare a measurement function for bias current. May get reused for multiple trims.
@endcode

**************************************************************************************************************************************************************/
	namespace Trim
	{

//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// {		declare extern (global) variables  (Edit this section if required)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	//Declare the external trim variables here.  Define and initializes them in GlobalTrims.cpp


	// { Sample Code 

	extern SiteBool gsbLockBit;		//LockBit Status
	extern SiteBool gsbSPLockBit;		//Scratch Pad LockBit Status

	#ifdef MAXIM_SAMPLE_CODE_ACTIVATE
		extern Maxim::OTP::CTrimStructure gtsIBias;  // Declare a trim object for bias current.  Recommend a seperate object for each trim.
		PinListData MeasureIBias (Maxim::OTP::CTrimStructure * tsTrim);			 // Declare a measurement function for bias current. May get reused for multiple trims.
	#endif


	extern Maxim::OTP::CTrimStructure gtsBG;
	extern Maxim::OTP::CTrimStructure gtsREF;
	extern Maxim::OTP::CTrimStructure gtsLDO;
	extern Maxim::OTP::CTrimStructure gtsIBIAS;
	extern Maxim::OTP::CTrimStructure gtsOSC;
	extern Maxim::OTP::CTrimStructure gtsOSC20M;
	extern Maxim::OTP::CTrimStructure gtsSLOPE;
	extern Maxim::OTP::CTrimStructure gtsOUTGAIN;
	extern Maxim::OTP::CTrimStructure gtsOUT1;
	extern Maxim::OTP::CTrimStructure gtsOUT2;
	extern Maxim::OTP::CTrimStructure gtsOUT3;
	extern Maxim::OTP::CTrimStructure gtsOUT4;
	extern Maxim::OTP::CTrimStructure gtsOUT5;
	extern Maxim::OTP::CTrimStructure gtsOUT6;

	extern PinListData MeasureBG (Maxim::OTP::CTrimStructure * tsTrim);
	extern PinListData MeasureLDO (Maxim::OTP::CTrimStructure * tsTrim);
	extern PinListData MeasureIBIAS (Maxim::OTP::CTrimStructure * tsTrim);
	extern PinListData MeasureOSC (Maxim::OTP::CTrimStructure * tsTrim);
	extern PinListData MeasureOSC20M (Maxim::OTP::CTrimStructure * tsTrim);
	extern PinListData MeasureSLOPE (Maxim::OTP::CTrimStructure * tsTrim);
	extern PinListData MeasureOUTGAIN (Maxim::OTP::CTrimStructure * tsTrim);
	extern PinListData MeasureOUT1 (Maxim::OTP::CTrimStructure * tsTrim);
	extern PinListData MeasureOUT2 (Maxim::OTP::CTrimStructure * tsTrim);
	extern PinListData MeasureOUT3 (Maxim::OTP::CTrimStructure * tsTrim);
	extern PinListData MeasureOUT4 (Maxim::OTP::CTrimStructure * tsTrim);
	extern PinListData MeasureOUT5 (Maxim::OTP::CTrimStructure * tsTrim);
	extern PinListData MeasureOUT6 (Maxim::OTP::CTrimStructure * tsTrim);

	extern SiteInt OUTx_Send_Data(int iDeviceAddress, int iReg, SiteInt siData);

	// } Sample Code

//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// }		declare extern (global) variables
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+

	
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// {		Typecast trim functions here
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	bool InitializeTrims(); // Do not delete or comment out this function


//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// }		Typecast trim functions here
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+

	}
}

