//DO NOT EDIT THIS FILE
/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*                                                                            
*	Source title:		Maximlib_SPU.CPP                                                                            		  
*	URL:				$URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/MaximLib/MaximLib_SPU.cpp $
*   Last Modified by:	$LastChangedBy: govit.pimthong $                                       
*	Date:				$Date: 2017-03-09 12:55:55 +0700 (Thu, 09 Mar 2017) $                                                
*   Current Rev:		$Revision: 61735 $
*                                                                            
*	Description:  
*		Maxim library file for SPU
*		Maxim library written and maintained by:								 
*			Patrick Hayes (Patrick.Hayes@MaximIntegrated.com)	                 
*			Mark Bruening (Mark.Bruening@MaximIntegrated.com)                                                                            
*
*   Revision History
*		
*                                                                            
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
//DO NOT EDIT THIS FILE

/**********************************************************************************************************************************************For*Doxygen**//**
@file	Maximlib_SPU.cpp
@brief	Maxim library header file for SPU
**************************************************************************************************************************************************************/


/*	PinListData Maximsp112mi (PME_INT pin, int vmeasgain, int imeasgain, int numsamples, double sampledelay)
	{
#ifdef MAXIM_DEGLITCH
		Maxim::Deglitch::glitch_handling();
#endif
		sp112mi(pin, vmeasgain, imeasgain, numsamples, sampledelay);
		return pld_groupgetresults(giNumSites);	//PME_INT only has one pin
	}
*/
	PinListData Maximsp112mi (int iPinGroup, int vmeasgain, int imeasgain, int numsamples, double sampledelay)
	{
#ifdef MAXIM_DEGLITCH
		Maxim::Deglitch::glitch_handling();
#endif

		//Need to handle a single instrument different from a PMEInt or GMEInt which are groups of pins.
		if (iPinGroup < 256)
		{
			PinListData pldMeasured;
			pldMeasured[Maxim::PinMap::mapResourceChannelSite[NAME_SP112][iPinGroup]] = sp112mi(iPinGroup, vmeasgain, imeasgain, numsamples, sampledelay);

			return pldMeasured;
		}
		else
		{
			sp112mi(iPinGroup, vmeasgain, imeasgain, numsamples, sampledelay);
			return pld_groupgetresults(Maxim::PinMap::CountResources(iPinGroup));
		}

//		return pld_groupgetresults(Maxim::PinMap::CountResources(group));	//Was GME_INT, changed to int to avoid an MST bug.  
													//Another MST bug prevents getting the group size from an int
													//Another MST bug prevent using the standard SPU groups defined on a Pin Map
													//Settled on defining a group in code and assuming their are only two SPU pins in the config.
	}
	
/*
	PinListData Maximsp112mv (PME_INT pin, int vmeasgain, int imeasgain, int numsamples, double sampledelay)
	{
#ifdef MAXIM_DEGLITCH
		Maxim::Deglitch::glitch_handling();
#endif
		sp112mv(pin, vmeasgain, imeasgain, numsamples, sampledelay);
		return pld_groupgetresults(giNumSites);	//PME_INT only has one pin
	}
*/
	PinListData Maximsp112mv (int iPinGroup, int vmeasgain, int imeasgain, int numsamples, double sampledelay)
	{
#ifdef MAXIM_DEGLITCH
		Maxim::Deglitch::glitch_handling();
#endif

		//Need to handle a single instrument different from a PMEInt or GMEInt which are groups of pins.
		if (iPinGroup < 256)
		{
			PinListData pldMeasured;
			pldMeasured[Maxim::PinMap::mapResourceChannelSite[NAME_SP112][iPinGroup]] = sp112mv(iPinGroup, vmeasgain, imeasgain, numsamples, sampledelay);

			return pldMeasured;
		}
		else
		{
			sp112mv(iPinGroup, vmeasgain, imeasgain, numsamples, sampledelay);
			return pld_groupgetresults(Maxim::PinMap::CountResources(iPinGroup));
		}

		//		return pld_groupgetresults(Maxim::PinMap::CountResources(group));	//Was GME_INT, changed to int to avoid an MST bug.  
													//Another MST bug prevents getting the group size from an int
													//Another MST bug prevent using the standard SPU groups defined on a Pin Map
													//Settled on defining a group in code and assuming their are only two SPU pins in the config.
	}



	int Maximsp112clamp (int pos, double percent_PFSV, double percent_NFSV, double percent_PFSI, double percent_NFSI)
	{	
#ifdef MAXIM_DEGLITCH
		Maxim::Deglitch::glitch_handling();
#endif
		return sp112clamp ( pos,  percent_PFSV,  percent_NFSV,  percent_PFSI,  percent_NFSI); 
	}
	int Maximsp112comp (int pos, int Loop_Resp, int FA_Resp )
	{
#ifdef MAXIM_DEGLITCH
		Maxim::Deglitch::glitch_handling();
#endif
		return sp112comp ( pos,  Loop_Resp,  FA_Resp ); 
	}

	int Maximsp112filter (int pos, int vfilter, int ifilter, int acfilter)
	{	
#ifdef MAXIM_DEGLITCH
		Maxim::Deglitch::glitch_handling();
#endif
		return sp112filter ( pos,  vfilter,  ifilter,  acfilter); 
	}

	int Maximsp112init (int pos)
	{
#ifdef MAXIM_DEGLITCH
		Maxim::Deglitch::glitch_handling();
#endif
		return sp112init ( pos); 
	}

	int Maximsp112kelvinmode (int pos, int mode)
	{	
#ifdef MAXIM_DEGLITCH
		Maxim::Deglitch::glitch_handling();
#endif
		return sp112kelvinmode ( pos,  mode); 
	}

	double Maximsp112miaccum (int pos, int timeslice)
	{
#ifdef MAXIM_DEGLITCH
		Maxim::Deglitch::glitch_handling();
#endif
		return sp112miaccum ( pos,  timeslice); 
	}

	double Maximsp112mvaccum (int pos, int timeslice)
	{
#ifdef MAXIM_DEGLITCH
		Maxim::Deglitch::glitch_handling();
#endif
		return sp112mvaccum ( pos,  timeslice); 
	}

	int Maximsp112pedestal ( int pos, int mode, double value, int gain )
	{
#ifdef MAXIM_DEGLITCH
		Maxim::Deglitch::glitch_handling();
#endif
		return sp112pedestal (  pos,  mode,  value,  gain ); 
	}

/* Check if we can get away with a single function that accepts a SiteDouble
	int Maximsp112set ( int pos, int mode, double force, int vrange, int irange)
	{
#ifdef MAXIM_DEGLITCH
		Maxim::Deglitch::glitch_handling();
#endif
		return sp112set (  pos,  mode,  force,  vrange,  irange); 
	}

*/


	int Maximsp112set( int pos, int mode, SiteDouble sdValue, int vrange, int irange)
	{	
		ACCUMULATOR_STR accValue[MAX_SITES];           //Use accumulator to set SPUs in different state

#ifdef MAXIM_DEGLITCH
		Maxim::Deglitch::glitch_handling();
#endif

		// If all sites are equal, call the standard version which has the potential to be faster
		if(sdValue.AreAllSitesEqual()) 		return sp112set (  pos,  mode,  sdValue[0],  vrange,  irange); 

		// If different sites have different values, then use the accumulator to set them
		for (int iSite = 0; iSite<Maxim::GetNumSites(); iSite++)
		{
			accValue[iSite].value1=sdValue[iSite];   //load site values into accumulator
		}
		groupsetaccumulator(accValue, giNumSites);
				
		return sp112set( pos, mode,  USE_ACCUM,  vrange,  irange);   //USE_ACCUM tells apu to check magic accumulator mailbox
	}

	int Maximsp112setcapload (int pos, int mode, double force, int vrange, int irange, double transition_time)
	{
#ifdef MAXIM_DEGLITCH
		Maxim::Deglitch::glitch_handling();
#endif
		return sp112setcapload ( pos,  mode,  force,  vrange,  irange,  transition_time); 
	}

namespace Maxim
{
		/************************************************************************************************************************************************************For*Doxygen**//**
		Tell the "No Site" pin that it is physically connected to this site.  Required for the Measurement and Datalog functions.                                              
		
		@param	[pmeSPUPin]  A single SPU112 channel
		@param	[iSite] The site to connect to.  Measurements will be associated with this site.  Pass NO_SITE to disassociate the pin with a site.
		
		@return		Zero on success.  Nonzero on failure (see msGroupSiteModify)
		
		@remarks	The SPU112 pin should be setup on the Pin Map Editor as a No Site to use this.
		**************************************************************************************************************************************************************/
		int Connect_SPU_Pin_To_Site(PME_INT pmeSPUPin, int iSite)
		{
			int iStatus = -1, i;
			int iPinNumber = pmeSPUPin[1];	//In a pin of type no site, the channel at index 1 is the pin number
			//TODO: verify input parameters
			//if (pmeSPUPin.Device != 
			if (((iSite < 0) || (iSite >= giNumSites)) && (iSite != NO_SITE))
			{
				etsprintf("Error!  Invalid site provided to Connect_SPU_Pin_To_Site function.\n");
				return iStatus;		//msGroupSiteModify returns nonzero to indicate failure.  
			}

			//First, remove the pin from all sites.
			for (i = 0; i < giNumSites; i++)
			{
				iStatus = msGroupSiteModify(i, FALSE, "SP112_%d", iPinNumber);	
				//TODO?  report if msGroupSiteModify returns a failure here?
			}
			//Now add it to the site
			if ((iSite >= 0) && (iSite <giNumSites))
			{	
				iStatus = msGroupSiteModify(iSite, TRUE, "SP112_%d", iPinNumber);	//Add the pin to the site
			}
			return iStatus;
		}
	namespace SPU{


	} // End of SPU namespace

}// End of Maxim namespace