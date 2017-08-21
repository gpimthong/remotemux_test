//DO NOT EDIT THIS FILE
/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*                                                                            
*	Source title:		MaximFuncLib_System.h                                                                           		  
*	URL:				$URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/MaximLib/MaximLib_System.h $
*   Last Modified by:	$LastChangedBy: govit.pimthong $                                       
*	Date:				$Date: 2017-03-09 12:55:55 +0700 (Thu, 09 Mar 2017) $                                                
*   Current Rev:		$Revision: 61735 $
*                                                                            
*	Description:  
*		Maxim library written and maintained by:								 
*			@author		Brad Sitton  (Brad.Sitton@maximintegrated.com)
*		                                                           
*
*   Revision History 
*                                                                            
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
//DO NOT EDIT THIS FILE

///	@file	MaximFuncLib_System.h
///	@brief	Contains functions and classes that involve the ETS system, but are not typically specific to a single instrument/
///			This means they may relate to no instrument or multiple instruments.
/// @author	Brad.Sitton@MaximIntegrated.com




#ifndef MAXIMLIB_SYSTEM_H
#define MAXIMLIB_SYSTEM_H

#pragma once

#ifdef _M_X64
#pragma comment ( lib, "sqlite3maxim" )
#else
#pragma comment ( lib, "sqlite3maximW32" )
#endif

#include <sqlite3maxim.h>    //Maximdll header file


/// The majority of functions developed within Maxim should fall under the Maxim namespace.  
namespace Maxim
{
	/// Contains functions that belong with ETS system, but are not specific to a particular instrument.
	namespace System
	{
		/************************************************************************************************************************************************************For*Doxygen**//**

		Calculates the power line frequency of the meter and returns the value in Hz.

		**************************************************************************************************************************************************************/
		double GetPowerLineFrequency(void);
		/************************************************************************************************************************************************************For*Doxygen**//**
		Used for reporting messages in a standard way (from .h file)
																		  
		**************************************************************************************************************************************************************/
		class CMessage{
			bool	mbHideRemainingMessages; // Keeps track of whether the operator has chosen to disable pop up boxes for the remainder of the current device
		public:
			bool	mbDisplayPopUp; ///< If false, then pop ups can not occur.  A true  does not guarantee pop ups will be enabled, since multiple conditions
									//   must be met before a pop up is allowed to occur.
			bool	mbLogMessagesToFile; ///< Enables/disables the output of messages to a file.
			CMessage(){ mbDisplayPopUp= TRUE; mbLogMessagesToFile=FALSE;  mbHideRemainingMessages = FALSE;} // Creates an error tracking object.
			void Deliver(std::string sMessageToDisplay, int iSite = -2) ; // Used -2 as a default for iSite because -1 is msAll Site

			/*************************************************************************************************************************************For*Doxygen**//**
			Converts stringstream to string, then calls Deliver()  
			@param	ssMessageToDisplay	The message that you want to display
			@param	iSite				Specify a site number if the message is site specific, otherwise do not enter anything
			@code
				std::stringstream ssMessage;
				ssMessage << "CTrimStructure::EvaluateMeasurementUsingTable was passed measurement data for more than one pin.\n";
				ssMessage << "That functionality is not yet supported\n" << this->msTrimName << " was being evaluated";
				Maxim::System::Message.Deliver(ssMessage);
			@endcode
			*******************************************************************************************************************************************************/
			void Deliver(std::stringstream &ssMessageToDisplay, int iSite = -2) {Deliver(ssMessageToDisplay.str(), iSite);} 

			// Clears the flag that allows user to hide pop-ups for the remainder of device, after seeing at least 1.
			void ResetForNewDevice() {mbHideRemainingMessages=false;}

		} ;


		
		extern CMessage Message; // Global object used to report errors

/************************************************************************************************************************************************************For*Doxygen**//**
Used to figure out where time is going in your test program.  Calls to on object of this type will create a list of time stamps, which you can use
to time different portions of your test program.

The example below shows how Maxim::System::TimeTool can be used to collect information.  Then you can inspect the contents either by trapping and watching or by
outputting the contents.

@code

	Maxim::System::TimeTool.MarkTime("Beginning");
		//Code to be timed
	Maxim::System::TimeTool.MarkTime("Step 2");
		//More code to be timed
	Maxim::System::TimeTool.MarkTime("Finished");
@endcode

@attention There seems to be a 15-45uS overhead each time you call MarkTime().

**************************************************************************************************************************************************************/
		class CTimeTool
		{
			class CTimeStamp
			{
			public:
				std::string msMessage;
				long		mlTimeUS;
				long		mlDeltaUS;
				CTimeStamp( std::string sMessage, long lTime, long lDeltaTime)
				{
					msMessage		= sMessage;
					mlTimeUS		= lTime;
					mlDeltaUS	= lDeltaTime;
				}
			};

			int		miEventCount;
			long	mlAccumulatedTimeUS;
			long	mlStartTimeUS, mlPauseTimeUS;

			std::vector<CTimeStamp> mvTimeStamp;
		public:
			/// Stores a message and time elapsed since SOT.  See CTimeTool for more information.
			/// @param sMessage	The message/label you provide for a given time stamp.
			void MarkTime(std::string sMessage)
			{
				long lTime = ltime(7);
				if((mvTimeStamp.size()==0)||(mvTimeStamp.back().mlTimeUS > lTime))
				{
					mvTimeStamp.clear();
					mvTimeStamp.push_back(CTimeStamp("Start of Program", 0, 0));
				}
				long lDeltaTime = lTime - mvTimeStamp.back().mlTimeUS;
				mvTimeStamp.push_back(CTimeStamp(sMessage, lTime, lDeltaTime)); 
			}
			void ClearTimer()
			{
				mlAccumulatedTimeUS = 0;
				miEventCount = 0;
				mlStartTimeUS=0;
				mlPauseTimeUS=0;

			}
			void StartTimer()
			{
				mlStartTimeUS = ltime(7);
				if(mlPauseTimeUS > mlStartTimeUS) ClearTimer();
			}
			void PauseTimer()
			{
				mlPauseTimeUS = ltime(7);
				mlAccumulatedTimeUS += mlPauseTimeUS - mlStartTimeUS;
				miEventCount++;
			}
			CTimeTool(){ ClearTimer();}
		};

		/// Object which can store a list of time stamps wherever you would like in your program.  CTimeTool has more information
		extern 	CTimeTool TimeTool;

		
/************************************************************************************************************************************************************For*Doxygen**//**
	Part of a schema that allows easy and flexible site loops.  
**************************************************************************************************************************************************************/
		class CActiveSite{
			int		miSite;
		public:
			CActiveSite(){ Begin();}				// Possible that this constructor and Begin() could have been a single function.
			/// Used in a for loop to begin a "group site" loop
			void Begin(){ miSite = -1;  Next();}	// Start with iSite = -1, so that Next() will find the first active site.
			/// Used in a for loop to step to the next group of instruments in a "group site" loop
			void Next(){ 
				for(miSite++; (NotFinished() && !msSiteStat(miSite)); miSite++); // Find the next active site that should be used on this iteration of the loop
			}
			// Checks to see if the end of a "group site" loop has been reached
			bool NotFinished(){ return (miSite < Maxim::GetNumSites());} //Evaluates whether the last site has been reached.
			operator int() const {return miSite;}
		};


		std::string GetCompleteFileName(std::string sFileName); // Adds project path to a file name




	} // end of System namespace
} // end of Maxim namespace

		/// Global object that is part of a schema to make site loops easier.  It is more flexible, but less intuitive than ForEachActive_iSite
		extern Maxim::System::CActiveSite ActiveSite;

		/************************************************************************************************************************************************************For*Doxygen**//**
		Macro that is part of a schema to simplify site loops. It is more flexible, but less intuitive than ForEachActive_iSite.
		The benefit over ForEachActive_iSite is you can use any variable name for site that you want.
		@code
			double dMax = 0;
			for(EachActiveSite(iYouChooseSiteVariableName))
			{
				dMax = max(dMax, fabs(sdValue[iSite]));
			}
		@endcode
		**************************************************************************************************************************************************************/
		#define EachActiveSite(a)  Maxim::System::CActiveSite a; a.NotFinished(); a.Next()






#endif