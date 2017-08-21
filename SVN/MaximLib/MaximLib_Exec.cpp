//DO NOT EDIT THIS FILE
/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:		Maximlib_Exec.CPP                                                                            		  
*	URL:				$URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/MaximLib/MaximLib_Exec.cpp $
*	Last Modified by:	$LastChangedBy: govit.pimthong $                                       
*	Date:				$Date: 2017-03-09 12:55:55 +0700 (Thu, 09 Mar 2017) $                                                
*	Current Rev:		$Revision: 61735 $
*
*	Description:  
*		Maxim library file for Test Control functions
*		Maxim library written and maintained by:								 
*			Patrick Hayes (Patrick.Hayes@MaximIntegrated.com)	                 
*			Mark Bruening (Mark.Bruening@MaximIntegrated.com)                                                                            
*
*	Revision History:
*		See Maximlib_Exec.h file.
*
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
//DO NOT EDIT THIS FILE

/**********************************************************************************************************************************************For*Doxygen**//**
@file	Maximlib_Exec.cpp
@brief	Maxim library file for Test Control functions
**************************************************************************************************************************************************************/

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers


#include <string>
#include <algorithm>


#define MAX_SITES 64	//Same as defined in Maximdll_internal.h

// Allows calling of Contact in this library, even though it won't be defined until later.
extern void Contact(int &);

namespace Maxim
{
	///@brief Namespace used for a variety of Test Control Functions
	namespace Exec
	{
		//namespace Lib
		//{
	 

		/************************************************************************************************************************************************************For*Doxygen**//**
		Called by the UserInit Test Control Function.  Initialize common program items and requirements for MaximLib and MaximDLL                                                

		@param	[iNumSites]  The number of sites defined in the Pin Map.  Pass the pre-defined NUM_SITES constant.
		@param	[bLoadVector] Set to true to load a vector file.   
		@param	[bInitMeter] Optional: Set to true to initialize and configure the Agilent Meter.  Default is false to maintain backwards compatibility.   
		@param	[iRealTimeDatalogSwitch] Optional: Pass in the switch number to allow the Real Time Datalog functionality.  
														Default is -1 (invalid switch number) to maintain backwards compatibility. 
		@returns	TRUE if UserInit was successful.  FALSE if the vector file failed to load.

		@remarks
				The bInitMeter parameter is set to false if not provided, however, the Template's default (in function UserInit) is true.  
				bInitMeter is only provided as an option because some ETS-88 testers do not have the proper meter hardware (as of 20111128).
				Once the hardware is fixed, bInitMeter should always be set to true, even if the program is not using the meter. 
		**************************************************************************************************************************************************************/
			BOOL Lib_UserInit(int iNumSites, bool bLoadVector, bool bInitMeter, int iRealTimeDatalogSwitch)
			{
				int iaValid[MAX_SITES], iSite;

				//Check to see if in debug mode
	#if _DEBUG 
		if (Maxim::Operator())
		{
			etsMessageBox("Program was compiled in DEBUG mode.\n  Program should be compiled in Release mode.\n  Please Contact TE", MB_OK);
			return false;
		}
	#endif
				//Set the active Low Limits (gdsLoLim) and High Limits (gdsHiLim).  
				//In Engineering mode, limits are picked based on the User Strings or selection prompts. 
				//In Production mode, limits are passed in by the loader (which reads the load file).
				if (!Maxim::Limits::SetTestLimits()) 
					return FALSE;	//Could not get the limits, halt loading

				//Enable reporting of all alarms by the alarmstat() utility.  
				alarmset( ALARM_ALL, TRUE );	

				//Load the Vector file into tester hardware memory.
				if (bLoadVector)
				{
					char vecFile[MAX_PATH];
					GetETSVariable(VEC_FILE, vecFile, MAX_PATH);

					if ( dpinvecload(vecFile) ) 
					{
						etsMessageBox("Could Not Initialize Vector File !!", MB_OK | MB_ICONSTOP);
						return FALSE;   // Abort Run of Test program DLL and exit TestExecutive
					}
				}

				//Pass the MaximDLL the number of sites.
				Maxim::SetNumSites(iNumSites);	

				//For multi-site programs, initialize all sites to be enabled.  
				if (iNumSites > 1)
				{
					//For multi-site programs, initialize all sites to be enabled.  
					//Sites may be individually turned off with the SITE parameters in the General Purpose Switches (found in the MST eShell).
					//Sites are individually disabled in OnSot()
					msFirstSite(0);	//Set the first site to be site 0
					for( iSite = 0; iSite < iNumSites; iSite ++ )
					{
						iaValid[iSite] = TRUE;	//Initially set turn on all sites.
					}
					iSite = msSetSites( iNumSites, iaValid );
				}
			
				//Display a user message for the F3 key
				//SetETSVariable(FLYBY_F3, "Start Test Loop (Hold s key to stop)");		//Not implemented in MST 2011B RC2


				//Define bin 32 to be used for the alarm bin.  This also matches the bin on the Datasheet.
				alarmbin(32);

				if (bInitMeter)
				{
					if (!Maxim::Meter::DMM::Init_And_Config_Maxim_Standard()) 
						return FALSE;	//Could not initialize the meter, halt loading
				}

	#ifdef MAXIM_DEGLITCH	//very old programs do not have the deglitch_Init function.  This #ifdef is here to prevent compiler errors for those old programs.
				//Turn on the Deglitch Assistant (internally checks the Deglitch_Assistant user switch)
				Maxim::Deglitch::deglitch_Init();
	#endif

				// Loads PMEInt and GMEInt information in an accessible map -- BRS 
				Maxim::PinMap::LoadPGMEInt(); 
				Maxim::CBits::Extended::CBitSiteMapCleanUp(); 

				return TRUE;
			};


		/************************************************************************************************************************************************************For*Doxygen**//**
		Called by the UserExit Test Control Function.   Placeholder for now.                                        

		@returns	TRUE. 
		**************************************************************************************************************************************************************/
			BOOL Lib_UserExit()
			{
		#ifdef GALAXY_PAT
			Maxim::Galaxy::UserExit_();
		#endif
				return TRUE;
			}

		/************************************************************************************************************************************************************For*Doxygen**//**
		Called by the OnTestInit Test Control Function.   Placeholder for now.                                        

		@returns	TRUE. 

		@remarks	This function is called prior to calling any product specific code in OnTestInit.
		**************************************************************************************************************************************************************/
			BOOL Lib_OnTestInitPre()
			{
				return TRUE;
			}

		/************************************************************************************************************************************************************For*Doxygen**//**
		Called by the OnTestInit Test Control Function.   Placeholder for now.                                        

		@returns	TRUE. 
	
		@remarks:	
				This function is called prior to calling any product specific code in OnTestInit.
		**************************************************************************************************************************************************************/
			BOOL Lib_OnTestInitPost()
			{
				BOOL bRet=TRUE;
	#ifdef GALAXY_PAT
				bRet=Galaxy::OnTestInit_(Maxim::GetNumSites(),Maxim::GetPartNumber(), Maxim::GetTestStep());
	#endif
				return bRet;
			}


		/************************************************************************************************************************************************************For*Doxygen**//**
		Called by the OnTestExit Test Control Function.   Placeholder for now.                                        

		@returns	TRUE. 
		@remarks	This function is called prior to calling any product specific code in OnTestExit.
		**************************************************************************************************************************************************************/
			BOOL Lib_OnTestExitPre()
			{
				return TRUE;
			}
		/************************************************************************************************************************************************************For*Doxygen**//**
		Called by the OnTestExit Test Control Function.   Placeholder for now.                                        

		@returns	TRUE
	
		@remarks	This function is called after calling any product specific code in OnTestExit.
		**************************************************************************************************************************************************************/
			BOOL Lib_OnTestExitPost()
			{
				return TRUE;
			}

		/************************************************************************************************************************************************************For*Doxygen**//**
		Called by the OnTestStartup Test Control Function.  Enables Test Timer at the start of the test.                                        

		@param	[bTimeTests] Set to TRUE to enable the test timer.  FALSE to disable test timing.  From the Template, pass GetUserSwitch(GPSW_Time_Tests).

		@returns	TRUE

		@remarks:	
				Sets global Maxim::DatalogLib::gTestTimeEnable.  It is used in Lib_OnTestCompletionPostPowerDown and Lib_OnSot. 
				This function is called prior to calling any product specific code in OnTestStartup.
		**************************************************************************************************************************************************************/
			//virtual BOOL FuncLib_OnTestStartupPre(BOOL bTimeTests){return TRUE;}

			BOOL Lib_OnTestStartupPre(BOOL bTimeTests)
			{
				Maxim::System::Message.ResetForNewDevice();
		#ifdef MAXIM_VERBOSE
				etsprintf( "DUT Socket Power ON\n" );
		#endif
	
				Maxim::DatalogLib::gTestTimeEnable=bTimeTests;  //Enable Test Time per Test 
				//Start test timer
				if (Maxim::DatalogLib::gTestTimeEnable)	//Check Test Time flag
				{
					mark(6); //use marker 6 for timing tests.
					Maxim::DatalogLib::gTestTimeCumulative=0.0;
				}
				Maxim::DatalogLib::SiteDatalogged(); //reset SiteDatalogged flags.
				Maxim::Results::ClearResults(); //Clear Results map


				/*if(!FuncLib_OnTestStartupPre())
				{
					return FALSE;
				}*/
				return TRUE;
			}

		/************************************************************************************************************************************************************For*Doxygen**//**
		Called by the OnTestStartup Test Control Function.    Placeholder for now.                                       

		@return		TRUE

		@remarks:	
				This function is called after calling any product specific code in OnTestStartup.
		**************************************************************************************************************************************************************/
			BOOL Lib_OnTestStartupPost()
			{
		#ifdef GALAXY_PAT 
				Maxim::Galaxy::OnTestStartup_();
		#endif


				return TRUE;
			}


		/************************************************************************************************************************************************************For*Doxygen**//**
		Called by the OnTestCompletion Test Control Function before the power down sequence.   Placeholder for now.                                       

		@returns	TRUE
		**************************************************************************************************************************************************************/
			BOOL Lib_OnTestCompletionPrePowerDown()
			{
				Maxim::DatalogFlush(); // call this in case TE forgets to.
				return TRUE;
			}

		/************************************************************************************************************************************************************For*Doxygen**//**
		Called by the OnTestCompletion Test Control Function after the power down sequence..  Calculates the test time for the program if enabled.                                       
	
		@return		TRUE. 
		@remarks	Global Maxim::DatalogLib::gTestTimeEnable is set in Lib_OnTestStartup. 
		**************************************************************************************************************************************************************/
			BOOL Lib_OnTestCompletionPostPowerDown()
			{

				//Check to make sure tests have been run on each site.
				bool NoTests=false;
				std::string sites_no_tests;
				for (int iSite=0; iSite<giNumSites; iSite++)
				{
					if (msSiteStat(iSite)) //if site active
					{
						if (!Maxim::DatalogLib::SiteDatalogged(iSite))
						{
								msFailBin(iSite,"32");
								msFailSite(iSite);
								NoTests=true;	
						}
					}
				}
				if (NoTests) 
				{
						etsMessageBox("No Tests run. Exiting Program", MB_OK | MB_ICONSTOP);   
						return false;
				}

				if (Maxim::DatalogLib::gTestTimeEnable)	//Output Test Time. (Time from SOT to End of Program, excluding handler index time).
				{
					double TestTime=double(ltime(6)/1000.0);
					mark(6);
					Maxim::DatalogLib::FlushTestTimeQueue();
					std::string format = "Time : %7s  %"+ MaxLogFormat + "f %5s %22s %-30s\n"; 
					etsprintf(format.c_str(),"",TestTime,"ms","","End of Program");
					//etsprintf("Test Time:  %40s %9.1f ms\n","End of Program",TestTime);
					Maxim::DatalogLib::gTestTimeCumulative=Maxim::DatalogLib::gTestTimeCumulative + TestTime;
					//etsprintf("Test Time:  %40s %9.1f ms\n","Cumulative",Maxim::DatalogLib::gTestTimeCumulative);
					etsprintf(format.c_str(),"",Maxim::DatalogLib::gTestTimeCumulative,"ms","","Cumulative");
			
				}
		#ifdef MAXIM_VERBOSE
				etsprintf( "DUT Socket Power OFF\n" );
		#endif
		#ifdef GALAXY_PAT 
				Maxim::Galaxy::OnTestCompletion_();
		#endif

				return TRUE;
			}

		/************************************************************************************************************************************************************For*Doxygen**//**
		Called by the OnLogData Test Control Function.  Sets up alarm posting for the datalog and STDF files.                           

		@param	[site] Passed in from OnLogData (not currently used)
		@param	[info] Passed in from OnLogData (not currently used)

		@return		TRUE. 
		**************************************************************************************************************************************************************/
			BOOL Lib_OnLogData(int site, datalog_info_t& info  )
			{



				int iPassFail;
				if ( ( iPassFail = GetETSVariable(PASSFAIL) ) == 2 )
				{
					PostAlarm( "ALARM" );	//Alarm occurred.  To determine the alarm message while debugging, place a breakpoint here.
											//When the debugger stops at the breakpoint, open RAIDE and call alarmstat(0)
				}
				else if ( iPassFail )
					PostAlarm("");			//Fail.  This string shows up in the STDF file, so should be empty unless an alarm occurred.
				else
					PostAlarm("");			//Pass


				return TRUE;
			}

		/************************************************************************************************************************************************************For*Doxygen**//**
		Called by the OnFailSite Test Control Function before the power down sequence.   Placeholder for now.            
	
		@param	[site] Passed in from OnFailSite (not currently used)
		@return		TRUE. 
		**************************************************************************************************************************************************************/
			BOOL Lib_OnFailSitePrePowerDown(int site)
			{
				return TRUE;
			}

		/************************************************************************************************************************************************************For*Doxygen**//**
		Called by the OnFailSite Test Control Function after the power down sequence.   Placeholder for now.            
	
		@param	[site] Passed in from OnFailSite (not currently used)
	
		@return		TRUE
		**************************************************************************************************************************************************************/
			BOOL Lib_OnFailSitePostPowerDown(int site)
			{

		#ifdef GALAXY_PAT
			Maxim::Galaxy::BinFail(site);
		#endif
				return TRUE;
			}

		/************************************************************************************************************************************************************For*Doxygen**//**
		Called by the OnSot Test Control Function.    Captures the Start of Test signal and sets up the program.  See Remarks. 

		@param	[SotValue] Passed in from OnSot.
		@param	[UserSwitchSite0] The General Purpose User Switch Number for the SITE_0 switch.
		@param	[UserSwitchSiteMax] The General Purpose User Switch Number for the last SITE_N switch.
		@param	[UserIntLoopCount] The General Purpose User Integer Number for the Loop_Count Integer.
		@param	[UserIntLoopDelayms] The General Purpose User Integer Number for the Loop_Delay_ms Integer.
		@param	[bCheckerPassed] Pass the result of  Maxim::HWC::bGet_Checker_Results().

		@return		TRUE

		@remarks
				- Enable or disable sites.  Enable test looping.  Setup the handler for Hardware Checker.
				- Global Maxim::DatalogLib::gTestTimeEnable is set in Lib_OnTestStartup.
				- The User parameters (UserSwitchSite0, UserSwitchSiteMax, UserIntLoopCount, and UserIntLoopDelayms) are passed by location number rather than result
					to save some execution time.  OnSot is called a very large number of times.  
				- This way, the results, such as GetUserInteger(UserIntLoopCount)), are only evaluated when required, instead of every time OnSot is called.
	
			Revision History:
				- 20110907 MWB Created.
				- 20111027 PHH Modified site enable/disable code to work for multiple sectors.
				- 20111028 MWB Added parameters UserSwitchSiteMax and UserIntLoopDelayms.
		**************************************************************************************************************************************************************/
			int Lib_OnSot(int SotValue, int UserSwitchSite0, int UserSwitchSiteMax, int UserIntLoopCount, int UserIntLoopDelayms, bool bCheckerPassed )
			{
				static bool statbEOT=false;
				static int statiLoopCount=0;
				int iRetVal = SotValue;
				int iaValid[MAX_SITES], iSite;
				bool bSitesEnabled;
				int iMaxSiteNum = (UserSwitchSiteMax - UserSwitchSite0) + 1;	//Number of sites defined in the General Purpose User Switches.

				if (SotValue !=0 && SotValue!=-2 )	//Only need to check the SotValue if one was sent and not from handler
				{
					//Enable or disable sites based on the Site Switches
					//	All pins with active sites in the group must be in the same state.  Otherwise the tester will throw an error.
					//	Make sure your power down functions in OnTestCompletion and in OnFailSite are properly written.
					//GetStationNumber should return 1 for Home sector and 2 for MSD, etc if we had more sectors.
					//Code should use first Maxim::GetNumSites() flags for sector 1 (Home sector) and (Maxim::GetNumSites() + 1) to (Maxim::GetNumSites() * 2) flags for sector 2.
					if (GetETSVariable(SECURITY_LVL) != 0)	//if not in operator mode  (don't allow operator to disable sites)
					{  
						bSitesEnabled = false;
						for( iSite = 0; iSite < Maxim::GetNumSites() && iSite < iMaxSiteNum  ; iSite ++ )		//Maxim::GetNumSites is set in Lib_UserInit.  
						{
							int iSwitchNum = iSite + (GetStationNumber() - 1) * Maxim::GetNumSites();
							if (iSwitchNum < iMaxSiteNum)
							{
								iaValid[iSite] = GetUserSwitch(UserSwitchSite0 + iSwitchNum);
								//msSiteStat doesn't read back correctly, don't change active sites if handler is involved.
								//if (SotValue==-2)  //handler said go, use handler as starting point.
									//iaValid[iSite]=iaValid[iSite]&&msSiteStat(iSite);
								bSitesEnabled = bSitesEnabled || iaValid[iSite];			// bSitesEnabled is false if no sites are enabled.
							}
							else
							{
								etsMessageBox("Program has more sites defined than General Purpose Switches to enable/disable sites.  Please add more site switches.",MB_OK | MB_ICONSTOP);
							}
						}
						iSite = msSetSites( Maxim::GetNumSites(), iaValid );
						if (!bSitesEnabled)		//If no Sites
						{
							iRetVal = 0;	//Return no SOT
							etsMessageBox("No sites are enabled !!",MB_OK | MB_ICONSTOP);
						}
					}

					/*	//MWB - Should be taken care of in the Loader
					//Setup Handler for Hardware Checker or production
					if (!bCheckerPassed)	
					{
						handler(false);		//Tell sot() function to not look for a handler.
					}
					else	//TODO check that the checker just passed and the program is running in operator mode.
					{
						//general idea:  clear the datalog, reset the lot and the serial numbers so the summary will only contain devices (not the HW checker)
						//clear the datalog
						//LogNewSubLot("NewSubLot", "NewLogFile");
						//LogClearHeader
						//return VK_R;	//Return Retest 
					}		
					*/
	
					//Start looping if F3 pressed
					if(SotValue==F3_KEY)  
					{ 
						statiLoopCount=GetUserInteger(UserIntLoopCount);	//Loop the number of times indicated in the User Integer
						Maxim::etsprintfHOME("Loop Mode: Started : Loop Count = %d\n",statiLoopCount);
						iRetVal=0;
					}

					if(SotValue==F4_KEY)  
					{ 
						static char cPreviousFileName[128]={""};
						char cFileName[128];
						char *Description[]={"LogFileName"};
						char *Strings[]={cFileName};
						int iRet;
						strcpy_s(cFileName, sizeof(cFileName),"LogFileName");
						iRet=PopupStringDialog("New DataLog File",Description,Strings,1,"",902);
						if (iRet==IDCANCEL)
							return 0;
						SetETSVariable(DLOG_TO_DISK,true);
						SetETSVariable(DLOG_STDF,true);
						if (strcmp(cPreviousFileName,cFileName)==0) 
							strcat_s(cFileName,sizeof(cFileName),"_");
						LogNewSubLot(cFileName,cFileName);
						strcpy_s(cPreviousFileName,sizeof(cPreviousFileName),cFileName);
						iRetVal=0;
					}

					//Stop looping if 's' key is pressed (must be held down (or timed exactly right) to register)
					if( SotValue == VK_S )
					{
						statiLoopCount = 0;
					}
				}

				if( statiLoopCount > 0)	//Program is Looping
				{
					if (ltime(7)/1000.0 > GetUserInteger(UserIntLoopDelayms))
					{
						statiLoopCount = statiLoopCount - 1;	//Decrement loop count
						lwait(GetUserInteger(UserIntLoopDelayms) ms_f);			//Wait the user specified amount of time before the next loop.
				
						iRetVal = VK_T;							//Return Test Start
						Maxim::etsprintfHOME("Loop Mode: Loop Count = %d of %d\n",GetUserInteger(UserIntLoopCount) - statiLoopCount,GetUserInteger(UserIntLoopCount));
					}
					if (statbEOT)
						Maxim::etsprintfHOME("Loop Mode: Press s to stop.");
				}
				if (statbEOT) //End of Test (First OnSot poll after part is binned)
				{	
					statbEOT=false;
				}


				if (iRetVal == VK_T || iRetVal == -2)	//If returning Test Start or Handler SOT
				{
					statbEOT=true;	//Indicate that a test was started so test time can be logged on the next poll
				}

				return iRetVal;
			}

		/************************************************************************************************************************************************************For*Doxygen**//**
		Called by the OnUtilError Test Control Function.   Placeholder for now.            
	
		@param	[errnum] Passed in from OnUtilError (not currently used)

		@returns	TRUE
	
		@remarks:	
		**************************************************************************************************************************************************************/
			BOOL Lib_OnUtilError(int errnum)
			{
				return TRUE;
			}

		/************************************************************************************************************************************************************For*Doxygen**//**
		Called by the OnInterlock Test Control Function.   Placeholder for now.            
	
		@param	[intlocks] Passed in from OnInterlock (not currently used)
	
		@returns	TRUE
		**************************************************************************************************************************************************************/
			BOOL Lib_OnInterlock(ULONG intlocks)
			{
				return TRUE;
			}
		/************************************************************************************************************************************************************For*Doxygen**//**
		Called in OnUserInit.  used to interface with MaximDLL to get limits columns.             

		@returns
				False if setting the limits fails 
				True if successful
		**************************************************************************************************************************************************************/
			BOOL Lib_GetTestLimits(PDS_DOUBLE &lolim, PDS_DOUBLE &hilim)
			{
				return true;
			}

		/************************************************************************************************************************************************************For*Doxygen**//**
		Called from the Template function which is called from the DataSheet.  Calls the product specific contact function.  Checks the results.        

		@param	[DSIndex]    DataSheet Index location passed in from the ETS Program Flow
	
		@return		ETS Program Flow (int) status indicating if the program should continue.   
		**************************************************************************************************************************************************************/
			int Lib_Contact(int & DSIndex)
			{
				//If hardware checker code was properly run (checker passed, or checker was bypassed)
				if ((Maxim::HWC::bGet_Checker_Results()) &&	 (! Maxim::bTest_Step_Is("HW_CHECKER")))	
				{
					Contact(DSIndex);	//Call the user supplied function to run contact

					//Check the results
					//Remove StopOnFail code until Eagle resolves issues.
					//Sites are "reactivated" in next function because of bug in MST
					//Stop On Fail was not intended to be changed during testing
					//Production Loader forces STOP_ON_FAIL to on so not worried about a production problem at this point
					//BOOL bStopOnFail = GetTestSwitch(STOP_ON_FAIL);	//Save the value of the Stop On Fail switch 
					//SetTestSwitches(0, STOP_ON_FAIL);	//force stop on fail for any site that failed contact.	
					Maxim::DatalogFlush();	//Datalog the results
		
					//restore the previous STOP_ON_FAIL setting
					//if (bStopOnFail) 
					//	SetTestSwitches(0, STOP_ON_FAIL);
					//else
					//	SetTestSwitches(STOP_ON_FAIL, 0); 		
		
					return( msSiteStat( MS_ALL ) ); // Return w/status
				}
				else	
				{
					//Program should not get here under normal conditions.  
					//If the program got here, please verify that the Hardware Checker function has not been disabled on the DataSheet.
					//Please also verify that the Template's structure has not been substantially modified 
					//	(critical functions deleted, MaximLib files modified, MaximLib functions not called, etc.)
					etsMessageBox("Contact Test Engineer. \nHardware checker is not properly configured. \n" \
									"The contact function was run without first calling the proper MaximLib files. \n" \
									"Or the Hardware Checker is disabled on the DataSheet.", MB_OK | MB_ICONSTOP, 200 );
					return -1;	//Force the program to halt.
				}
			}
	
		//}
	
	}
	namespace System
		{			
			/************************************************************************************************************************************************************For*Doxygen**//**
			Returns the physical ETS Tester Sector that is running this program's DLL       

			@return		Returns the sector number as an ETSSector enumerated type.
						- Program running only on sector A returns ETSSector1A.
						- Program running only on physical sector B on the A side CPU returns ETSSector1B.
						- Program running only on physical sector B on the B side CPU returns ETSSector1B (development systems only).
						- Program running on physical sectors A and B, called from eShell sector A returns ETSSector1A.
						- Program running on physical sectors A and B, called from eShell sector B returns ETSSector1B.
			@remarks	
					If the program is running on both sectors, there are two independent DLLs running (one on each sector).  This call will return ETSSector1A or ETSSector1B
						properly, depending on the physical sector that is actually running the DLL. 
			@note 	This differs from the built in ETS GetStationNumber function call.  GetStationNumber returns 1 if running only on sector A, 
							and returns 1 if running only on sector B.  However, GetStationNumber returns 2 if called from sector B while running on both sectors.
			@note	The B side CPU only exists for development systems.  Production systems only have one CPU (A Side) that is dedicated to both physical sectors.
							The B Side eShell only list sector A even though it is physically connected to sector B.  This is due to the config file.
					
			@see function GetEshellSector and GetETSSectorString.

			
			**************************************************************************************************************************************************************/
			ETSSector GetETSSector()
			{
				char  acComputerName [MAX_COMPUTERNAME_LENGTH + 1];
				DWORD dwComputerName = sizeof ( acComputerName );
				std::string sComputerName;
				Maxim::System::ETSSector eETSSector;

				eETSSector = Maxim::System::GetEshellSector();
				if (eETSSector == Maxim::System::ETSSector1A)				
				{
					//If ETSSector1A, determine if the system is running on the A side CPU or B side CPU.
					//The B Side CPU (development systems only) reports that it is running on the 1A quadrant.
					//This is due to the way the config.ets file must be defined (as of MST 2011A-MST [2011.1.1005.22])
					if (! GetComputerName( acComputerName, &dwComputerName ))
					{ 
						etsMessageBox("Could not get the Computer Name.  Sector Unknown.", MB_OK|MB_ICONSTOP);
						return ETSSectorUnknown;
					}
					else //Determine the CPU name to figure out the sector.  The CPU name must follow the Maxim standard.
					{
						sComputerName.assign(acComputerName);	//Assign to a string to use the string functions.
						std::transform(sComputerName.begin(), sComputerName.end(), sComputerName.begin(), ::toupper);	//Convert sComputerName to upper case
						if ((sComputerName.compare(0, 5, "ETS88") == 0) && (sComputerName.compare(sComputerName.size() -1,  1, "B")) == 0) 
							return ETSSector1B;		//If computer name begins with "ETS88" and ends with "B", Currently running on CPU B (Sector 1B)
						else
							return ETSSector1A;		//All other system names map to sector 1A.
					}
				}
				else 
					return eETSSector;
			}	
			/************************************************************************************************************************************************************For*Doxygen**//**
			Returns the sector according to eShell (A or B) as opposed to the physical sector (GetETSSector)      

			@returns	Returns the sector number as an ETSSector enumerated type.
						- Program running only on sector A returns ETSSector1A.
						- Program running only on physical sector B on the A side CPU returns ETSSector1B.
						- Program running only on physical sector B on the B side CPU returns ETSSector1A (development systems only).
						- Program running on physical sectors A and B, called from eShell sector A returns ETSSector1A.
						- Program running on physical sectors A and B, called from eShell sector B returns ETSSector1B.
			
			@remarks	
					If the program is running on both sectors, there are two independent DLLs running (one on each sector).  This call will return ETSSector1A or ETSSector1B
						properly, depending on the sector that is actually running the DLL. 
			
			@note	This differs from the built in ETS GetStationNumber function call.  GetStationNumber returns 1 if running only on sector A, 
							and returns 1 if running only on sector B.  However, GetStationNumber returns 2 if called from sector B while running on both sectors.

			@see function GetETSSectorString.
			**************************************************************************************************************************************************************/
			ETSSector GetEshellSector()
			{
				UTILSTAT_STR testerInfo;
				utilstat(&testerInfo,sizeof(UTILSTAT_STR));
				if (testerInfo.Quadrants[0]==0)
					return ETSSector1A;		
				else if (testerInfo.Quadrants[0]==1)
					return ETSSector1B;
				else if (testerInfo.Quadrants[0]==2)
					return ETSSector2A;
				else if (testerInfo.Quadrants[0]==3)
					return ETSSector2B;
				else 
					return ETSSectorUnknown;
			}	
			/************************************************************************************************************************************************************For*Doxygen**//**
			Returns the ETS Tester Sector that is running this program's DLL       
	
			@returns	The sector number as a string.  Possible returns: "Unknown", "1A", "1B", "2A", "2B"

			@see function GetETSSector.
			**************************************************************************************************************************************************************/
			std::string GetETSSectorString()
			{
				Maxim::System::ETSSector eETSSector;
				eETSSector = Maxim::System::GetETSSector();
				if (eETSSector == Maxim::System::ETSSector1A)		//Sector 1A
					return "1A";
				else if (eETSSector == Maxim::System::ETSSector1B)	//Sector 1B			
					return "1B";
				else if (eETSSector == Maxim::System::ETSSector2A)	//Sector 2A			
					return "2A";
				else if (eETSSector == Maxim::System::ETSSector2B)	//Sector 2B			
					return "2B";
				else	//Unknown sector				
					return "Unknown";	
			}		

			/************************************************************************************************************************************************************For*Doxygen**//**
			Determine if this is an off line workstation or an on line tester. 
			
			@returns	True if this program is running on a tester.  False if it is running on an off line development system
			**************************************************************************************************************************************************************/
			bool bOnline()
			{
				//
				UTILSTAT_STR testerInfo;
				if (utilstat(&testerInfo,sizeof(UTILSTAT_STR)) == 1 ) 
					return true;
				else
					return false;
			}

		}

		bool Operator()
		{
			return (GetETSVariable( SECURITY_LVL )==0);
		}
}