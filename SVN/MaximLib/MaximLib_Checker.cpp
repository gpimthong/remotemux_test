//DO NOT EDIT THIS FILE
/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*                                                                            
*	Source title:		Maximlib_Datalog.CPP                                                                            		  
*	URL:				$URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/MaximLib/MaximLib_Checker.cpp $
*   Last Modified by:	$LastChangedBy: govit.pimthong $                                       
*	Date:				$Date: 2017-03-09 12:55:55 +0700 (Thu, 09 Mar 2017) $                                                
*   Current Rev:		$Revision: 61735 $
*                                                                            
*	Description:  
*		Maxim library file for Hardware Checker functions
*		Maxim library written and maintained by:								 
*			Patrick Hayes (Patrick.Hayes@MaximIntegrated.com)	                 
*			Mark Bruening (Mark.Bruening@MaximIntegrated.com)                                                                            
*
*   Revision History
*		See Maximlib.h file.
*                                                                            
**************************************************************************************************************************************************************/
//DO NOT EDIT THIS FILE

/**********************************************************************************************************************************************For*Doxygen**//**
@file	Maximlib_Checker.CPP
@brief	Maxim library file for Hardware Checker functions
**************************************************************************************************************************************************************/

#include <assert.h>
#include "MaximLib_Checker.h"



//Functions and variables for the Template's Hardware Checker functions.  Not meant to be used by anything other than the Template.
BOOL pbDisplayPrompt;	//internal storage for bDisplayPrompt parameters passed in during bSkipChecker
BOOL pbStopOnFail;		//internal storage for status of the StopOnFail flag before running the checker
bool gbCheckerPassed = false;	//internal storage for the pass/fail status of the checker
std::string gsHardwareID;	//internal storage for the ID of the hardware that ran the checker.

// These functions will be defined later in the users hardware checker function.  If they are not defined, there will be a compile error
//    This is not necessarily the best structure, but is needed for backward compatibility.
void Check_For_DUT(int & DSIndex);
void Hardware_Checker(int & DSIndex);
std::string sReadHardwareID(void);

namespace Maxim
{
	namespace HWC
	{
		/************************************************************************************************************************************************************For*Doxygen**//**
		Called from the Template function which is called from the DataSheet.                                                                 
		
		@param [DSIndex]    DataSheet Index location passed in from the ETS Program Flow
		
		@return	ETS Program Flow (int) status indicating if the program should continue.   

		@remarks	Runs the Hardware Checker functions.
		**************************************************************************************************************************************************************/
		int Lib_Hardware_Checker(int DSIndex)
		{
			static std::string sHardwareID;	//Declare as static so test time is not wasted re-reading the EEPROM.  
			int iStartingDSIndex = DSIndex;	

			//Read the contactor board ID from the EEPROM  
			if (sHardwareID.length() == 0)
			{
				sHardwareID = sReadHardwareID();	//Get the Hardware ID from the user function above.
			}

			//Determine if the checker needs to be run.  
			if (Maxim::HWC::bSkip_Checker(GetUserSwitch(GPSW_HW_CHECKER_MSG), sHardwareID))	//If the checker needs to run.
			{
				return( msSiteStat( MS_ALL ) );		// Checker Skipped.  Continue with the rest of the program.
			}
			else	//Run the Checker
			{
				//Check for contact to verify that a DUT is not in the socket before starting the HW checker. 
				Check_For_DUT(DSIndex); //Call the user supplied function to check for a DUT in the test socket.

				if (iStartingDSIndex == DSIndex)	//If nothing was datalogged in Check_For_DUT
				{
					PopupStringDialog("Checker FAILED", 0, 0, 0, "No tests were datalogged in the HW Checker Check_For_DUT function.",106);
					return(-1); // Skip the remaining test functions (Hardware Checker is still connected).  Checker will not be recorded as passed (Log_HWChecker_Results is not called)
				}
				

				//Datalog and determine if any sites failed.
				//Inform the operator if a DUT is in the Socket
				if  (!Maxim::HWC::Found_DUT_In_Socket()) 	
				{
					iStartingDSIndex = DSIndex;

					//DUT not in socket, continue with HW Checker.
					Hardware_Checker(DSIndex);	//Call the user supplied function to run the checker
					
					if (iStartingDSIndex == DSIndex)	//If nothing was datalogged in Hardware_Checker
					{
						PopupStringDialog("Checker FAILED", 0, 0, 0, "No tests were datalogged in the Hardware_Checker function.",107);
						return(-1); // Skip the remaining test functions (Hardware Checker is still connected).  Checker will not be recorded as passed (Log_HWChecker_Results is not called)
					}

					// Datalog the results.	Display Result to the operator.
					Maxim::HWC::Datalog_Checker_Results();	
				}

				return(-1); // Skip the remaining test functions.  (Hardware Checker is still connected)
			}
		}


		/************************************************************************************************************************************************************For*Doxygen**//**
		Determines if the Hardware Checker should be skipped based on the current Test Step.  See Remarks. 
			Sets up the environment to either run or bypass the checker.                                                                      
		
		@param	[bDisplayPrompt] True to display operator prompts.  False to skip the prompts.  Skipping prompts is useful when running a Gage Study on the Checker.
								 Typically, pass in GetUserSwitch(GPSW_HW_CHECKER_MSG) from the Template.
		@param	[sHardwareID] Reserved for future use.  Currently ignored.
		
		@return True if the checker was bypassed.  false if the checker needs to be run.  
		
		@remarks:	In Production mode, the Maxim Loader reads the Checker Log file and forces the Test Step to "HW_CHECKER" when required.
					In Engineering mode, the Engineer must manually set the Test Step to "HW_CHECKER" to run the checker.  The Checker Log file is not read in Engineering mode.
		
		   Revision History
				20110914 MWB created.
				20111028 MWB Modified HW checker scheme so HW Checker will only run if Test Step is "HW_CHECKER".  Removed the iCheckerBypass User Integer.
								Skipping HW Checker prompts is now done with the bDisplayPrompt USer Switch.  
								This modification broke backwards compatibility, but was necessary so the Checker could work in production mode with the Maxim Loader.
		                                                                            
		**************************************************************************************************************************************************************/
		bool bSkip_Checker(BOOL bDisplayPrompt, std::string sHardwareID)
		{
			//Save globals
			pbStopOnFail = GetTestSwitch(STOP_ON_FAIL);	//Save the value of the Stop On Fail switch 
			pbDisplayPrompt = bDisplayPrompt;		//Save the value of bDisplayPrompt for the bGetCheckerResults function

			//Determine if the checker needs to be run.
			if (Maxim::bTest_Step_Is("HW_CHECKER"))		//Always run the checker if Test Step is HW_CHECKER.  																												
			{
				gbCheckerPassed = false;
				SetTestSwitches(0, STOP_ON_FAIL);	//Set the Stop on fail test switch to true so the program will halt if the checker fails.
				if ((bDisplayPrompt) || (GetETSVariable( SECURITY_LVL )==0))	//if display prompt, or in operator mode.
#ifndef MAXIM_SKIP_HWCMESSAGE
					etsMessageBox("Insert the Hardware Checker and hit OK when ready.", MB_OK, 100 );
#endif

				return false;	//Need to run the checker
			}
			else	//Any other test step will bypass the HW Checker.
			{
				gbCheckerPassed = true;
				return true;	//Checker will be skipped.
			}
		}

		/************************************************************************************************************************************************************For*Doxygen**//**
		Datalogs the hardware checker results.    Also displays the prompts to alert the operator if the checker passed or failed.                                                         

		@remarks	If the checker passed, the operator is instructed to get the setup ready to begin testing.
		
		   Revision History
				20110914 MWB created.
				20111028 MWB Modified HW checker scheme.  See function bSkip_Checker.
		**************************************************************************************************************************************************************/
		void Datalog_Checker_Results()
		{				
			gbCheckerPassed = (bool) !Maxim::DatalogFlush();	//Datalog and determine if the HW Checker passed.

			//Also check to see if anything failed that was not stored inside the datalog queue
			for (int iSite =0; iSite < giNumSites; iSite++)
			{
				if (msTestStat(iSite) == FAILURE )	//If the site failed outside of the stored Datalog
					gbCheckerPassed = false;		//Fail the checker
			}

			//log pass result to text file for future program load.
			if (gbCheckerPassed)
				Log_HWChecker_Results(gsHardwareID);

			if ((pbDisplayPrompt) || (GetETSVariable( SECURITY_LVL )!=0))	//if display prompt, or in operator mode.
			{
				if (gbCheckerPassed) {
					PopupStringDialog("Checker Passed", 0, 0, 0, "Hardware Checker passed.", 101);
					if (GetETSVariable( SECURITY_LVL )==0) { //If running in operator mode
						PopupStringDialog("Ready to start testing", 0, 0, 0, "Exit the program and return to the Loader.  Select the proper Test Step.  Reload the program and begin testing when ready.", 102);
					}
				
				}
				else {
					PopupStringDialog("Checker FAILED", 0, 0, 0, "Hardware Checker Failed.",103);	//keep STOP_ON_FAIL switch set to true (set in function bSkipChecker)
				}
			}
			
			//Restore the value of the STOP_ON_FAIL switch.  
			//Program execution is stopped if the checker failed by the "if (Maxim::HWC::bGet_Checker_Results())" statement at the top of each test function.
			//Program execution could be stopped by leaving the STOP_ON_FAIL switch on and returning -1 for the function, but this risks 
			//losing the STOP_ON_FAIL setting desired by the user (setting would be lost if the debugging session was stopped).
			if (pbStopOnFail) 
				SetTestSwitches(0, STOP_ON_FAIL);
			else
				SetTestSwitches(STOP_ON_FAIL, 0);
		}

		/************************************************************************************************************************************************************For*Doxygen**//**
		* bGet_Checker_Results
		*   Purpose:	Returns the pass/fail result of the hardware checker.                                                                      
		*	Arguments:	
		*		none
		*	Returns:
		*		Function returns true if the checker passed.  false if the checker failed.  
		*   Remarks:		
		*
		*   Revision History
		*		See Maximlib_Checker.h file.
		*                                                                            
		**************************************************************************************************************************************************************/
		bool bGet_Checker_Results()
		{
			return gbCheckerPassed;
		}

		/************************************************************************************************************************************************************For*Doxygen**//**
		Called by the standard Hardware Checker Template Function when a DUT is found in the test Socket instead of the HW Checker.                                                      
		
		@return		true if the test failed (and so a DUT was found in socket).  Returns false if the checker can start.

		@remarks	Informs the Operator to remove the DUT and re-run the Checker.
		**************************************************************************************************************************************************************/
		bool Found_DUT_In_Socket()
		{				

			gbCheckerPassed = (bool) !Maxim::DatalogFlush();	//Datalog and determine if a diode was detected
	
			//Also check to see if anything failed that was not stored inside the datalog queue
			for (int iSite =0; iSite < giNumSites; iSite++)
			{
				if (msTestStat(iSite) == FAILURE )	//If the site failed outside of the stored Datalog
					gbCheckerPassed = false;		//Fail the checker
			}

			//log pass result to text file for future program load.
			if (!gbCheckerPassed)
			{
				PopupStringDialog("Checker FAILED", 0, 0, 0, "Device found in socket.  Please remove the DUT and re-run the Checker.", 105);	
			}

			return !gbCheckerPassed;	//Invert the return (if checker failed, then Found_DUT_In_Socket is true)
		}

		/************************************************************************************************************************************************************For*Doxygen**//**
		Called by the sReadHardwareID Function in the Hardware Checker Template.  sReadHardwareID is user editable, so call is not guaranteed.

		@return	true.  Returning false will abort running the HW Checker code and exit the program.
		@remarks:		
		*		TODO.  This function is not implemented yet.  Eventually, the idea is to read the Hardware ID from the EEPROM on the contactor board (and Load Board)
		*					then translate that into the barcode number or hardware name.  The operator would scan the hardware barcode(s) into the ETS Loader before the 
		*					program is loaded.  The Loader would then look for the HW Checker log file to see if the product it is loading had already passed the
		*					checker on the hardware that was scanned in.  This would allow for a smarter Hardware Checker routine that would automatically request the
		*					checker to be run again if the hardware changed.  The program would need to provide a matching Hardware ID to be stored in the log file
		*					if the checker passed.  For this to be implemented, the MES (back end production control) systems need to be updated to be able to verify 
		*					that the proper hardware was scanned in by the operator (translate barcode ID into Hardware Name and verify proper hardware is used 
		*					using the setup sheet).
		**************************************************************************************************************************************************************/
		std::string sRead_EEPROM_Hardware_ID()
		{				

			//Possible ETS functions to investigate:
			//read_board_id
			//board_id_checksum

			return " ";	//Placeholder for now.  
		}

	}
}


bool Hardware_Recently_Passed(std::string sHardwareID)
{
	//TODO 
	//Read HWChecker log file to determine if this board has recently passed the HW checker 

	//Open file
	//Each active site must be listed in the HW Checker log file with this boardID, and have passed within the last 24 hours.



	gsHardwareID = sHardwareID;		//Save the Hardware ID
	return false;
}

bool Log_HWChecker_Results(std::string sHardwareID)
{
	//Write to HW Checker log file
	//Store "Checker_Log.txt" to C:\Program Files\Eagle Test Systems\Versions\2011B-MST [2011.2.1001.2]\BIN\ directory 
	// (file should not be placed in the program's directory as this is too easy to find.)


	//Log file format: csv
	//Log the following for each active site.
	//Program base name (die type), Load Board ID, Contactor Board ID, site, date_time, pass/fail



	Maxim::HWC::HWCheckerPassed(Maxim::GetPartNumber(), Maxim::System::GetEshellSector());


	return true;
}


