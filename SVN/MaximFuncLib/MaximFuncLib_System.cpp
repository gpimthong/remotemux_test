//DO NOT EDIT THIS FILE
/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*                                                                            
*	Source title:		MaximFuncLib_System.cpp                                                                            		  
*	URL:				$URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/MaximFuncLib/MaximFuncLib_System.cpp $
*   Last Modified by:	$LastChangedBy: govit.pimthong $                                       
*	Date:				$Date: 2017-03-09 12:55:55 +0700 (Thu, 09 Mar 2017) $                                                
*   Current Rev:		$Revision: 61735 $
*                                                                            
*	Description:  
*		Maxim Function library file for System functions.  
*		Contains generic System functions.
*		Maxim library written and maintained by:								 
*			Brad Sitton  (Brad.Sitton@maximintegrated.com)
*
*   Revision History
*		
*                                                                            
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
//DO NOT EDIT THIS FILE

/**********************************************************************************************************************************************For*Doxygen**//**
@file	MaximFuncLib_System.cpp
@brief	Maxim Function library file for System functions.  Contains generic System functions.
**************************************************************************************************************************************************************/

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers


// disable warnings about long names
#ifdef WIN32
  #pragma warning( disable : 4786)
#endif

namespace Maxim
{
	
		/************************************************************************************************************************************************************For*Doxygen**//**
		Check to see if Char switch is on                               
		
		@returns	true if the char switch is enabled. False if not enabled
		**************************************************************************************************************************************************************/	
		bool isChar()
		{
			if (GetUserSwitch(GPSW_Characterization_Mode)==0)
				return false;
			else
				return true;
		}



	
	namespace System{



#if 0
/************************************************************************************************************************************************************For*Doxygen**//**
		Accepts a std::vector@<SiteDouble@> and returns the largest double found
			One use of this function is when autoranging a group of pins.  All pins must be on the same range, regardless of site.

@param	vsdValue	A std::vector@<SiteDouble@>

@return	The largest double associated with vsdValue.
**************************************************************************************************************************************************************/
		double GetMaxAbsoluteValueFromVSD(vector<SiteDouble> vsdValue )
		{
			double dValue = -HUGE_VAL;
			for(unsigned int i=0; i<vsdValue.size(); i++)
			{
				GetMaxAbsoluteValueFromSD(vsdValue[i]);
			}
			return dValue;
		}

/************************************************************************************************************************************************************For*Doxygen**//**
		Accepts a SiteDouble and returns the largest double found
			One use of this function is when autoranging a group of pins.  All pins must be on the same range, regardless of site.

@param	sdValue	A SiteDouble

@return	The largest double associated with vsdValue.
**************************************************************************************************************************************************************/
		double GetMaxAbsoluteValueFromSD(SiteDouble sdValue)
		{
			double dMax = 0;
			ForEachActive_iSite
			{
				dMax = max(dMax, fabs(sdValue[iSite]));
			}
			return dMax;
		}

		ALARM_STR aALMSTRalarms[8];        //8 comes from the help file
         int iAlarmStatus;
         std::string sKey, sKeyChannel;
         std::stringstream ssConvert;       
         
         void PrintAlarms(void)
		 {
			 iAlarmStatus = alarmstat( 0, aALMSTRalarms, 8 );         //Returns the number of alarms and ALARM_STR with alarms

			 //Build a map of alarms
			 std::map<std::string, int> mapAlarms;       //Move to global

			 for (int iAlarm = 0; iAlarm < iAlarmStatus; iAlarm++) 
			 {
				 //build the key as Board_channel_Alarmbit
				 aALMSTRalarms[iAlarm].boardID;
				 sKeyChannel = aALMSTRalarms[iAlarm].boardID + "_" + aALMSTRalarms[iAlarm].num;
				 sKeyChannel += "_";
				 for (int iAlarmBit = 0; iAlarmBit < 32; iAlarmBit++)
				 {
					 if (aALMSTRalarms[iAlarm].alarm  && iAlarmBit)      //if alarm bit is set
					 {
						 sKey = sKeyChannel;

						 ssConvert << iAlarmBit;   //convert int to string
						 sKey.append(ssConvert.str());         //Add alarm bit to finish building key

						 
						 if ( mapAlarms.find(sKey) == mapAlarms.end() ) {
							 // not found
							 //Add new alarm to map    ... mapAlarms.insert(
							 mapAlarms.insert ( std::pair<std::string,int>(sKey, 0); //Not sure if this is correct
							 //Need switch and case for each bit for string printing
							 //Declare a string and store alarm there instead of printing it. We can figure out printing later.
							 switch (iAlarmBit)
							 {
							 case 0: etsprintf ( "ALARM_OSCDETECT \n"); break;
							 case 1: etsprintf ( "ALARM_KELVINLO \n"); break;
							 case 2: etsprintf ( "ALARM_KELVINHI \n"); break;
							 case 3: etsprintf ( "NOT USED \n"); break;
							 case 4: etsprintf ( "NOT USED \n"); break;
							 case 5: etsprintf ( "ALARM_CLAMPLO \n"); break;
							 case 6: etsprintf ( "ALARM_CLAMPHI \n"); break;
							 case 7: etsprintf ( "NOT USED \n"); break;
							 case 8: etsprintf ( "NOT USED \n"); break;
							 case 9: etsprintf ( "ALARM_FATALERRBIT \n"); break;
							 case 10: etsprintf ( "ALARM_MCUTIMEOUT \n"); break;
							 case 11: etsprintf ( "ALARM_PIN_MISSING \n"); break;
							 case 12: etsprintf ( "NOT USED \n"); break;
							 case 13: etsprintf ( "ALARM_HEATSINKTEMP \n"); break;
							 case 14: etsprintf ( "NOT USED \n"); break;
							 case 15: etsprintf ( "ALARM_CAPDROOP \n"); break;
							 case 16: etsprintf ( "ALARM_OVERRANGELO \n"); break;
							 case 17: etsprintf ( "ALARM_OVERRANGEHI \n"); break;
							 case 18: etsprintf ( "NOT USED \n"); break;
							 case 19: etsprintf ( "ALARM_RCVMIDBAND \n"); break;
							 case 20: etsprintf ( "ALARM_RCVBADSTATE \n"); break;
							 case 21: etsprintf ( "ALARM_OVERVOLTAGE \n"); break;
							 case 22: etsprintf ( "ALARM_OVIPLUS \n"); break;
							 case 23: etsprintf ( "ALARM_OVINEG \n"); break;
							 case 24: etsprintf ( "NOT USED \n"); break;
							 case 25: etsprintf ( "NOT USED \n"); break;
							 case 26: etsprintf ( "NOT USED \n"); break;
							 case 27: etsprintf ( "NOT USED \n"); break;
							 case 28: etsprintf ( "NOT USED \n"); break;
							 case 29: etsprintf ( "NOT USED \n"); break;
							 case 30: etsprintf ( "NOT USED \n"); break;
							 case 31: etsprintf ( "ALARM_INTERLOCKBIT \n"); break;
							 }
							 //Print alarm to screen  etsprintf  (check to see if Brad has error handling/printing code)

						 } else {
							 // found
							 //Do nothing
						 }
					 }
				 }

				 mapAlarms.clear(); //Clear the map once you have printed
			 }
		 }


#endif

/************************************************************************************************************************************************************For*Doxygen**//**
		Accepts a std::vector@<PinListData@> and returns the largest double found
			One use of this function is when autoranging a group of pins.  All pins must be on the same range, regardless of site.

@param	vpldValue	A std::vector@<PinListData@>

@return	The largest double associated with vpldValue.
**************************************************************************************************************************************************************/
		double GetMaxAbsoluteValueFromVPLD(vector<PinListData> vpldValue )
		{
			double dValue = -HUGE_VAL;
			for(unsigned int i=0; i<vpldValue.size(); i++)
			{
				GetMaxAbsoluteValueFromPLD(vpldValue[i]);
			}
			return dValue;
		}

/************************************************************************************************************************************************************For*Doxygen**//**
		Accepts a PinListData and returns the largest double found

@param	pldValue	A set of PinListData to be evaluated.

@return	The largest double associated with vsdValue.
**************************************************************************************************************************************************************/
		double GetMaxAbsoluteValueFromPLD(PinListData pldValue)
		{
			double dMax = 0;
			for(int i=0; i< pldValue.iGetNumPins(); i++)
			{
				dMax = max(dMax, fabs(pldValue[i]));
			}
			return dMax;
		}










//########## Following are the comments and default definitions for several virtual functions from the CPinUnit base class ###################################

/************************************************************************************************************************************************************For*Doxygen**//**
		Instrument independent interface that applies dForceValue voltage to the instrument.

@param	dForceValue		Voltage that will be forced on the instrument
@param	iVRange			Voltage range that will be used
@param	iIRange			Current range that will be used
@param	bGetInstrumentInformation	Set to true if you want to retrieve the settings from ETS hardware instead of relying on memory
**************************************************************************************************************************************************************/
		int CPinUnit::ForceV(SiteDouble dForceValue, int iVRange, int iIRange, bool bGetInstrumentInformation){return MAXIM_INVALID;}
/************************************************************************************************************************************************************For*Doxygen**//**
		Instrument independent interface that applies dForceValue current to the instrument.

@param	dForceValue		Current that will be forced on the instrument
@param	iVRange			Voltage range that will be used
@param	iIRange			Current range that will be used
@param	bGetInstrumentInformation	Set to true if you want to retrieve the settings from ETS hardware instead of relying on memory
**************************************************************************************************************************************************************/
		int CPinUnit::ForceI(SiteDouble dForceValue, int iVRange, int iIRange, bool bGetInstrumentInformation){return MAXIM_INVALID;}
/************************************************************************************************************************************************************For*Doxygen**//**
		Instrument independent interface that measures voltage.

@param	iNumberOfSamples		Total number of ADC samples to take when making the measurement
@param	dDelayBetweenSamplesUS	Delay between consecutive ADC samples in microseconds
@param	iMeasureGain			Gain setting to use curing measurement
@param	bGetInstrumentInformation	Set to true if you want to retrieve the settings from ETS hardware instead of relying on memory

@return	Voltages measured in the form of pld (PinListData)
**************************************************************************************************************************************************************/
		PinListData CPinUnit::MeasureV(int iNumberOfSamples, double dDelayBetweenSamplesUS, int iMeasureGain, bool bGetInstrumentInformation){ return PinListData();} 
/************************************************************************************************************************************************************For*Doxygen**//**
		Instrument independent interface that measures current.

@param	iNumberOfSamples		Total number of ADC samples to take when making the measurement
@param	dDelayBetweenSamplesUS	Delay between consecutive ADC samples in microseconds
@param	iMeasureGain			Gain setting to use curing measurement
@param	bGetInstrumentInformation	Set to true if you want to retrieve the settings from ETS hardware instead of relying on memory

@return	Voltages measured in the form of pld (PinListData)
**************************************************************************************************************************************************************/
		PinListData CPinUnit::MeasureI(int iNumberOfSamples, double dDelayBetweenSamplesUS, int iMeasureGain, bool bGetInstrumentInformation){ return PinListData();} 
/************************************************************************************************************************************************************For*Doxygen**//**
		Instrument independent interface that will indicate if the device is in Force Voltage mode.
**************************************************************************************************************************************************************/
		bool CPinUnit::IsForceVoltageMode(bool bGetInstrumentInformation){return FALSE;}
/************************************************************************************************************************************************************For*Doxygen**//**
Instrument independent interface that will indicate if the device is in Force Current mode.
@param	bGetInstrumentInformation	Set to true if you want to retrieve the settings from ETS hardware instead of relying on memory
**************************************************************************************************************************************************************/
		bool CPinUnit::IsForceCurrentMode(bool bGetInstrumentInformation){return FALSE;}
/// Obtains the status from hardware for the given instrument ******************************************************************************************
		int CPinUnit::GetInstrumentInformation(){ return MAXIM_INVALID;}
/************************************************************************************************************************************************************For*Doxygen**//**
		Converts the voltage range into a double
			Basic problem is that ranges show up as an integer such as 0, 1, 2 ... which is not useful when performing calculations.
			For example, if you want the program to auto-range, you may need to perform some calculations to determine if the range is sufficient.

@param	bGetInstrumentInformation	Set to true if you want to retrieve the settings from ETS hardware instead of relying on memory

@return	The largest double associated with vsdValue.
**************************************************************************************************************************************************************/
		double CPinUnit::GetVRangeAsDouble(bool bGetInstrumentInformation){ return HUGE_VAL;}
/************************************************************************************************************************************************************For*Doxygen**//**
		Converts the current range into a double
			Basic problem is that ranges show up as an integer such as 0, 1, 2 ... which is not useful when performing calculations.
			For example, if you want the program to auto-range, you may need to perform some calculations to determine if the range is sufficient.

@param	bGetInstrumentInformation	Set to true if you want to retrieve the settings from ETS hardware instead of relying on memory

@return	The largest double associated with vsdValue.
**************************************************************************************************************************************************************/
		double CPinUnit::GetIRangeAsDouble(bool bGetInstrumentInformation){ return HUGE_VAL;}
/************************************************************************************************************************************************************For*Doxygen**//**
		Sets the measurement ADC to the the smallest range setting that is appropriate for the parameters provided.
			If no parameters are provided then largest range will be selected. 

@param	dValue1		Value that must be accommodated by the gain setting.
@param	dValue2		Optional second value that must be accommodated by the gain setting.
@param	bGetInstrumentInformation	Set to true if you want to retrieve the settings from ETS hardware instead of relying on memory
**************************************************************************************************************************************************************/
		int CPinUnit::AutoIRange(double dValue1, double dValue2, bool bGetInstrumentInformation){ return MAXIM_INVALID;}
/************************************************************************************************************************************************************For*Doxygen**//**
		Sets the measurement ADC to the the smallest range setting that is appropriate for the parameters provided.
			If no parameters are provided then largest range will be selected. 

@param	dValue1		Value that must be accommodated by the gain setting.
@param	dValue2		Optional second value that must be accommodated by the gain setting.
@param	bGetInstrumentInformation	Set to true if you want to retrieve the settings from ETS hardware instead of relying on memory
**************************************************************************************************************************************************************/
		int CPinUnit::AutoVRange(double dValue1, double dValue2, bool bGetInstrumentInformation){return MAXIM_INVALID;}
/************************************************************************************************************************************************************For*Doxygen**//**
		Sets the measurement ADC to the the highest gain setting that is appropriate for the values that are passed in.

@param	dValue1		Value that must be accommodated by the gain setting.
@param	dValue2		Optional second value that must be accommodated by the gain setting.
@param	bGetInstrumentInformation	Set to true if you want to retrieve the settings from ETS hardware instead of relying on memory
**************************************************************************************************************************************************************/
		int CPinUnit::AutoIGain(double dValue1, double dValue2, bool bGetInstrumentInformation){return MAXIM_INVALID;}
/************************************************************************************************************************************************************For*Doxygen**//**
		Sets the measurement ADC to the the highest gain setting that is appropriate for the values that are passed in.

@param	dValue1		Value that must be accommodated by the gain setting.
@param	dValue2		Optional second value that must be accommodated by the gain setting.
@param	bGetInstrumentInformation	Set to true if you want to retrieve the settings from ETS hardware instead of relying on memory
**************************************************************************************************************************************************************/
		int CPinUnit::AutoVGain(double dValue1, double dValue2, bool bGetInstrumentInformation){return MAXIM_INVALID;}

		PinListData CPinUnit::GetForceValue(bool bGetInstrumentInformation){return MAXIM_INVALID;}	///< Value that the instrument was forced to when GetInstrumentInformation() was last run
		int	CPinUnit::GetForceMode(bool bGetInstrumentInformation){return MAXIM_INVALID;}			///< Forcing mode when GetInstrumentInformation() was last run
		int	CPinUnit::GetVRange(bool bGetInstrumentInformation){return MAXIM_INVALID;}				///< Voltage range of instrument when GetInstrumentInformation() was last run
		int	CPinUnit::GetIRange(bool bGetInstrumentInformation){return MAXIM_INVALID;}				///< Current range of instrument when GetInstrumentInformation() was last run
		int CPinUnit::GetMeasureMode(bool bGetInstrumentInformation){return MAXIM_INVALID;}			///< Measurement mode of instrument when GetInstrumentInformation() was last run
		PinListData CPinUnit::GetMeasuredValue(bool bGetInstrumentInformation){return MAXIM_INVALID;}			///< Value measured by the instrument when GetInstrumentInformation() was last run

	void CSharedPinGroups::Share(int iGroup, PME_INT pmePin, int iSite)
	{
		int iPin = pmePin.PinNumber();

		if (mmapSharedPinGroup[iGroup].find(iPin) != mmapSharedPinGroup[iGroup].end())
		{
			std::stringstream ssMessage;
			ssMessage << "Warning:  Shared PinGroup " << iPin << " assignment is unexpectedly being changed from site "<< mmapSharedPinGroup[iPin].find(iPin)->second.miSite << "to " << iSite << std::endl;
			ssMessage << " in CSiteGroups::SiteGroupData::AddPin was passed measurement data for more than one pin.\n";
			Maxim::System::Message.Deliver(ssMessage);
		}
		mmapSharedPinGroup[iGroup].operator[](iPin) = CPinSite(pmePin, iSite);
	}

	// Will pause all sites that are not used on this iteration of site groups and allocate the instrument to the appropriate sites.
	//   If all site groups have been run, it will unpause all sites.
	void CSharedPinGroups::SiteGroupAllocation()
	{

		std::vector<int> viSitesStopped(GetNumSites(), FALSE); // No sites should be paused
		msSetSiteUserStopped( (int) viSitesStopped.size(), &viSitesStopped[0] );	

			if(mbRunLoopForActiveSitesOnly)
		{
			// Starting with the current pointer location of mitSharedPinGroup, see if any sites are active.  If not, increment
			//   the iterator until a match is found or the end of the container is reached.
			bool bFoundActiveSite = false;
			while( (bFoundActiveSite == false) && (mitSharedPinGroup != mmapSharedPinGroup.end()))
			{
				// Need to check each pin/site in the current group to see if it is active
				for(std::map<int, CPinSite>::iterator it = mitSharedPinGroup->second.begin(); it != mitSharedPinGroup->second.end(); it++)
				{
					if (msSiteStat(it->second.miSite)) bFoundActiveSite = true;
				}

				if(!bFoundActiveSite) mitSharedPinGroup++;
			}
		}

		if(mitSharedPinGroup == mmapSharedPinGroup.end()) // If end of list is reached, unpause all sites
		{
//			for(std::map<int, int>::iterator it = mitSiteGroup->second.mmapPinSite.begin(); it != mitSiteGroup->second.mmapPinSite.end(); it++)
			std::map<int, std::map<int, CPinSite>>::iterator itSiteGroup = this->mmapSharedPinGroup.begin();
			if(itSiteGroup != mmapSharedPinGroup.end())
			{
				for(std::map<int, CPinSite>::iterator it = itSiteGroup->second.begin(); it != itSiteGroup->second.end(); it++)
				{
					switch (Maxim::PinMap::GetInstrumentType(it->first))
					{
						case NAME_APU12:	Maxim::Connect_APU_Pin_To_Site(it->second.mpmePin, NO_SITE); break;
						case NAME_SP112:	Maxim::Connect_SPU_Pin_To_Site(it->second.mpmePin, NO_SITE); break;
						case NAME_QTMU:		Maxim::Connect_TMU_Pin_To_Site(it->second.mpmePin, NO_SITE); break;
						default: Maxim::System::Message.Deliver("SiteGroupAllocation received iPinGroup with no valid instrument associated");
					}			
				}
			}
			
		}
		else  		// Pause sites that aren't being operated on in this iteration
		{
			std::vector<int> viSitesStopped(GetNumSites(), TRUE); // Default sites to be paused

			// Allocate instruments with the appropriate sites
			for(std::map<int, CPinSite>::iterator it = mitSharedPinGroup->second.begin(); it != mitSharedPinGroup->second.end(); it++)
			{
				viSitesStopped.at(it->second.miSite) = FALSE; // Identify sites that should not be paused for this iteration

				switch (Maxim::PinMap::GetInstrumentType(it->first))
				{
					case NAME_APU12:	Maxim::Connect_APU_Pin_To_Site(it->second.mpmePin, it->second.miSite); break;
					case NAME_SP112:	Maxim::Connect_SPU_Pin_To_Site(it->second.mpmePin, it->second.miSite); break;
					case NAME_QTMU:		Maxim::Connect_TMU_Pin_To_Site(it->second.mpmePin, it->second.miSite); break;
					default: Maxim::System::Message.Deliver("SiteGroupAllocation received iPinGroup with no valid instrument associated");
				}			

				msSetSiteUserStopped((int) viSitesStopped.size(), &viSitesStopped[0] );	//Pause sites that aren't being used in this iteration
			}
		}
	}

	} // end of System name space
} // end of Maxim name space
