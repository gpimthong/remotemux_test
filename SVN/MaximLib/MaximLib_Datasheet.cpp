//DO NOT EDIT THIS FILE
/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:		Maximlib_Datasheet.CPP
*	URL:				$URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/MaximLib/MaximLib_Datasheet.cpp $
*	Last Modified by:	$LastChangedBy: govit.pimthong $
*	Date:				$Date: 2017-03-09 12:55:55 +0700 (Thu, 09 Mar 2017) $
*	Current Rev:		$Revision: 61735 $
*
*	Description:  
*		Maxim library file for Eagle Datasheet functions
*		Maxim library written and maintained by:
*			Patrick Hayes (Patrick.Hayes@MaximIntegrated.com)
*			Mark Bruening (Mark.Bruening@MaximIntegrated.com)
*
*	Revision History
*		See Maximlib.h file.
*
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
//DO NOT EDIT THIS FILE

/**********************************************************************************************************************************************For*Doxygen**//**
@file	Maximlib_Datasheet.cpp
@brief	Maxim library file for Eagle Datasheet functions
**************************************************************************************************************************************************************/

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <algorithm>

PDS_CHAR gpdsPartGatePtr, gpdsTestStepGatePtr;

namespace Maxim
{
	///@brief	*Initializes Datasheet gating columns.  The setup is already in the template and does not need to be modified.	
	void init_DataSheet_Gating(PDS_CHAR PartGatePtr, PDS_CHAR TestStepGatePtr)
	{
		gpdsPartGatePtr = PartGatePtr;
		gpdsTestStepGatePtr = TestStepGatePtr;
	}
/************************************************************************************************************************************************************For*Doxygen**//**
Determine if the test (at the DSIndex provided) is disabled by Part-Type Gating

@param	[iDSIndex] The Datasheet index to check.

@return	True if test is disabled by the Gate.  False if not.  See Remarks.

@remarks

		This function retrieves the active Part Number from the MaximDLL and the Part Gate from the Datasheet.
		Function init_DataSheet_Gating must be run before calling this function to store the Part Gate Datasheet column.
		Blank Part Gates indicate that the test should be run for all test functions.  bTest_Disabled_By_Part_Gate returns false.
		If the Part Gate contains a string, it is searched to try to find a match with the active Part Number.
			A non-empty Part Gate string is expected to contain one or more part numbers in a delimited list.  Each part must end with the semicolon (;) delimiter.
			Examples:
				MAX8557;			//Run the test only when the MAX8557 part type is active.
				MAX8557;MAX8556;	//Run the test only when the MAX8557 or MAX8556 part types are active.
				!MAX8557			//Run the test only when the MAX8557 part type is NOT active.
				!MAX8557;!MAX8556;	//Run the test only when the MAX8557 or MAX8556 part types are NOT active.
				MAX8557;!MAX8556;	//Not allowed. Results not guaranteed.  If an ! is used, all part types must have one.  

		@code
			//Row 100 on the Datasheet contains : MAX8557;MAX8556;
			//Assumptions
			//Active part gate is MAX8557
			//DS index = 100
			if (!bTest_Disabled_By_Part_Gate(DSIndex))	//Use this line
			if (bTest_Enabled_By_Part_Gate(DSIndex))	//Or this line
			{
				//Code runs for MAX8557 or MAX8556
			}
			else
			{
				//Code runs for MAX8559
			}
		@endcode
**************************************************************************************************************************************************************/
	bool bTest_Disabled_By_Part_Gate(int iDSIndex)
	{
		char * ptrChar;	
		ptrChar = gpdsPartGatePtr[iDSIndex];
		if (ptrChar == 0)
			return false;	//No Gates on this line, so test is not disabled.
		else
		{
			return bTest_Disabled_By_Gate(gpdsPartGatePtr[iDSIndex], Maxim::GetPartNumber());
		}
	}

/************************************************************************************************************************************************************For*Doxygen**//**
Determine if the test (at the DSIndex provided) is enabled by Part-Type Gating

@param	[iDSIndex] The Datasheet index to check.

@return	True if test is disabled by the Gate.  False if not.

@remarks	See bTest_Enabled_By_Part_Gate() for more information and examples.  This function is just the logical inverse
**************************************************************************************************************************************************************/
	bool bTest_Enabled_By_Part_Gate(int iDSIndex)
	{
		return !bTest_Disabled_By_Part_Gate(iDSIndex);
	}

/************************************************************************************************************************************************************For*Doxygen**//**
Determine if the test (at the DSIndex provided) is disabled by TestStep Gating

@param	[iDSIndex] The Datasheet index to check.

@returns	True if test is disabled by the Gate.  False if not.  See Remarks.

@remarks
		This function retrieves the active TestStep from the MaximDLL and the TestStep Gate from the Datasheet.
		Function init_DataSheet_Gating must be run before calling this function to store the TestStep Gate Datasheet column.
		Blank TestStep Gates indicate that the test should be run for all test functions.  bTest_Disabled_By_TestStep_Gate returns false.
		If the TestStep Gate contains a string, it is searched to try to find a match with the active TestStep.
			A non-empty TestStep Gate string is expected to contain one or more TestSteps in a delimited list.  Each TestStep must end with the semicolon (;) delimiter.
			Examples:
				FT_ROOM;			//Run the test only when the FT_ROOM TestStep is active.
				FT_ROOM;CHAR;		//Run the test only when the FT_ROOM or CHAR TestSteps are active.
				!FT_ROOM			//Run the test only when the FT_ROOM TestStep is NOT active.
				!FT_ROOM;!CHAR;		//Run the test only when the FT_ROOM or CHAR TestSteps are NOT active.
				FT_ROOM;!CHAR;		//Not allowed. Results not guaranteed.  If an ! is used, all TestSteps must have one.  
**************************************************************************************************************************************************************/
	bool bTest_Disabled_By_TestStep_Gate(int iDSIndex)
	{
		char * ptrChar;	
		ptrChar = gpdsTestStepGatePtr[iDSIndex];
		if (ptrChar == 0)
			return false;	//No Gates on this line, so test is not disabled.
		else
		{
			return bTest_Disabled_By_Gate(gpdsTestStepGatePtr[iDSIndex], Maxim::GetTestStep());
		}
	}

/************************************************************************************************************************************************************For*Doxygen**//**
Determine if the test (at the DSIndex provided) is enabled by TestStep Gating

@param	[iDSIndex] The Datasheet index to check.

@returns	True if test is disabled by the Gate.  False if not.  See Remarks.

@remarks
		This function retrieves the active TestStep from the MaximDLL and the TestStep Gate from the Datasheet.
		Function init_DataSheet_Gating must be run before calling this function to store the TestStep Gate Datasheet column.
		Blank TestStep Gates indicate that the test should be run for all test functions.  bTest_Disabled_By_TestStep_Gate returns false.
		If the TestStep Gate contains a string, it is searched to try to find a match with the active TestStep.
			A non-empty TestStep Gate string is expected to contain one or more TestSteps in a delimited list.  Each TestStep must end with the semicolon (;) delimiter.
			Examples:
				FT_ROOM;			//Run the test only when the FT_ROOM TestStep is active.
				FT_ROOM;CHAR;		//Run the test only when the FT_ROOM or CHAR TestSteps are active.
				!FT_ROOM			//Run the test only when the FT_ROOM TestStep is NOT active.
				!FT_ROOM;!CHAR;		//Run the test only when the FT_ROOM or CHAR TestSteps are NOT active.
				FT_ROOM;!CHAR;		//Not allowed. Results not guaranteed.  If an ! is used, all TestSteps must have one.  
**************************************************************************************************************************************************************/
	bool bTest_Enabled_By_TestStep_Gate(int iDSIndex)
	{
			return !bTest_Disabled_By_TestStep_Gate(iDSIndex);
	}


/************************************************************************************************************************************************************For*Doxygen**//**
Returns True if the current active test step matches the string provided.

@param	[sTestStep] A character array with the name of the Test Step to check.

@returns	True if sTestStep is the current active test step.

@remarks
		This function retrieves the active TestStep from the MaximDLL (passed in during the UserInit function.
		The Test Step is case sensitive.

		@code
			if (bTest_Step_Is("FT_ROOM") {
				//Do something
			}
		@endcode
**************************************************************************************************************************************************************/
	bool bTest_Step_Is(const char * sTestStep)
	{
		return strcmp( Maxim::GetTestStep(), sTestStep ) == 0;
	}

/************************************************************************************************************************************************************For*Doxygen**//**
Returns True if the current active part number matches the string provided.

@param	[sPartNumber] A character array with the name of the part number to check.

@return		True if sPartNumber is the current active part number.

@remarks
		This function retrieves the active part number from the MaximDLL (passed in during the UserInit function.
		The part number is case sensitive.

		@code
		if (bPart_Number_Is("MAX8556") {
				//Do something
			}
		@endcode
**************************************************************************************************************************************************************/
	bool bPart_Number_Is(const char * sPartNumber)
	{
		return strcmp( Maxim::GetPartNumber(), sPartNumber ) == 0;
	}

	void Get_Limits_From_Datasheet(int iDSIndex)
	{

	}

}	//namespace Maxim


	/************************************************************************************************************************************************************For*Doxygen**//**
	Supporting function for bTest_Disabled_By_Part_Gate and bTest_Disabled_By_TestStep_Gate.  For internal Lib use only.
	
	@param	[sDatasheetGate]  The string from the Datasheet gate column
	@param	[sGate]	The active gate to check
	
	@return		True if test is disabled by the Gate.  False if not.  See Remarks.
	@remarks    See bTest_Disabled_By_Part_Gate and bTest_Disabled_By_TestStep_Gate. 
	**************************************************************************************************************************************************************/
	bool bTest_Disabled_By_Gate(std::string sDatasheetGate, std::string sGate) 
	{
		size_t found;

		sGate = sGate + ";";	//Appended Part Number with the delimiter ";", to make sure MAX855 does not match MAX8555.
		//Make the comparisons not case sensitive
		std::transform(sDatasheetGate.begin(), sDatasheetGate.end(), sDatasheetGate.begin(), ::toupper);	//Convert sDatasheetGate to upper case
		std::transform(sGate.begin(), sGate.end(), sGate.begin(), ::toupper);	//Convert sGate to upper case
		found = sDatasheetGate.find("!");	//Find "!" in the Gate String
		if (found != std::string::npos)	//If found "!"
		{
			sGate = "!" + sGate;	//Looking for (not) Part Number
			found = sDatasheetGate.find(sGate);	//Find PartNum in the Gate String
			if (found != std::string::npos)	//If found PartNum
				return true;	//Found ! Part Number, so disable test.
			else
				return false;
		}
		else	//No "!" used
		{
			found = sDatasheetGate.find(sGate);	//Find PartNum in the Gate String
			if (found != std::string::npos)	//If found PartNum
				return false;	//Found Part Number, so test should be enabled.
			else
				return true;	//Did not find the part number, disable test
		}

	}

