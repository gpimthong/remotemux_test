//DO NOT EDIT THIS FILE
/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*                                                                            
*	Source title:		Maximlib_PinListData.h                                                                           		  
*	URL:				$URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/MaximLib/MaximLib_PinListData.h $
*   Last Modified by:	$LastChangedBy: govit.pimthong $                                       
*	Date:				$Date: 2017-03-09 12:55:55 +0700 (Thu, 09 Mar 2017) $                                                
*   Current Rev:		$Revision: 61735 $
*                                                                            
*	Description:
*		Maxim library header file for Template Class PinListData
*		Maxim library written and maintained by:								 
*			Patrick Hayes (Patrick.Hayes@MaximIntegrated.com)	                 
*			Mark Bruening (Mark.Bruening@MaximIntegrated.com)                                                                            
*
*   Revision History                                                    
*                                                                            
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
//DO NOT EDIT THIS FILE

/**********************************************************************************************************************************************For*Doxygen**//**
@file	Maximlib_PinListData.h
@brief	Maxim library header file for Template Class PinListData
**************************************************************************************************************************************************************/

// Following is documentation to be used by Doxygen to create help files.
/************************************************************************************************************************************************************For*Doxygen**//**
@page quickStartPinListData Pin List Data Quick Start
@details 
\section overview Overview

- PinListData is a class defined in MaximLib to make reading, datalogging, and manipulating measurements easier.
- SiteInt, SiteDouble, and SiteBool classes are also supplied to do math or logic involving multiple sites.
- Currently supports +,-,/,* of integers, doubles, PinListData, or SiteVar classes. If other operations required, please contact us.
- PinListData has many advantages over using the standard ETS-88 RESULTS_STR.
	- Ability to see watch values in debug.
	- No Need to keep track of number of sites and number of pins for most measurements.
	- No Need to loop datalog staments.
	- No Need to loop math functions. 

\section examples Examples 
\subsection DataloggingPLD Datalogging a Typical Test
<table  style="boarder: none">
	<tr>
		<td style="background: honeydew; vertical-align: top">
			<b> <span style="color:green">Efficient </span>datalogging using PinListData:</b>
			<div class="line"><span class="comment">//Declare the variables</span></div>
			<div class="line" >PinListData pldResults, pldResultsI, pldResultsV;</div>
			<div class="line" ></div>
			<div class="line" ><span class="comment">//Force 5V</span></div>
			<div class="line" >apu12set( IN1_OUT1, APU12_FV, 5.0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI);</div>
			<div class="line" ></div>
			<div class="line" ><span class="comment">//Measure the current</span></div>
			<div class="line" >pldResultsI = apu12mi( IN1_OUT1, APU12_MI_1X, 100, 13.0 );</div>
			<div class="line" ></div>
			<div class="line" ><span class="comment">//Datalog the results</span></div>
			<div class="line" ><a class="code" href="namespace_maxim.html#a0223056babb0826b6cf173d34b3a403e" title="Replace the built in msLogResultAll function with added functionality.">Maxim::DatalogAll</a>(DSIndex, pldResultsI);</div>
			<div class="line" ></div>
			<div class="line" ><span class="comment">//Measure the voltage</span></div>
			<div class="line" >pldResultsV = apu12mv(IN1_OUT1, 100, 13);  </div>
			<div class="line" ></div>
			<div class="line" ><span class="comment">//Datalog the results</span></div>
			<div class="line" ><a class="code" href="namespace_maxim.html#a0223056babb0826b6cf173d34b3a403e" title="Replace the built in msLogResultAll function with added functionality.">Maxim::DatalogAll</a>(DSIndex, pldResultsV);</div>
			<div class="line" ></div>
			<div class="line" ><span class="comment">//Record the resistance        </span></div>
			<div class="line" >pldResults = pldResultsV / pldResultsI;</div>
			<div class="line" ></div>
			<div class="line" ><span class="comment">//Datalog the results</span></div>
			<div class="line" ><a class="code" href="namespace_maxim.html#a0223056babb0826b6cf173d34b3a403e" title="Replace the built in msLogResultAll function with added functionality.">Maxim::DatalogAll</a>(DSIndex, pldResults);</div>
		</td>
		<td style="background: mistyrose">
			<b> <span style="color:red">Inefficient </span>datalogging using site loops.<br>  This code does the same thing as the code to the left</b>
			<div class="line"><span class="comment">//Declare the variables</span></div>
			<div class="line"><span class="keyword">const</span> <span class="keywordtype">int</span> coni_APU_GROUP_PIN_COUNT = 2;  <span class="comment">//must be set to # of pins</span></div>
			<div class="line">RESULTS_STR APUGroupResults[coni_APU_GROUP_PIN_COUNT][ NUM_SITES ];</div>
			<div class="line">RESULTS_STR APUGroupResults2[coni_APU_GROUP_PIN_COUNT][ NUM_SITES ];</div>
			<div class="line">RESULTS_STR Results[coni_APU_GROUP_PIN_COUNT][ NUM_SITES ];</div>
			<div class="line" ></div>
			<div class="line"><span class="comment">//Force 5V</span></div>
			<div class="line">apu12set( IN1_OUT1, APU12_FV, 5.0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );</div>
			<div class="line">       </div>
			<div class="line"><span class="comment">//Measure the current</span></div>
			<div class="line">apu12mi( IN1_OUT1, APU12_MI_1X, 100, 13.0 );</div>
			<div class="line" ></div>
			<div class="line" ><span class="comment">//Datalog the results</span></div>
			<div class="line">groupgetresults( &amp;APUGroupResults[0][0], NUM_SITES * coni_APU_GROUP_PIN_COUNT);</div>
			<div class="line"><span class="keywordflow">for</span> (i = 0; i &lt; coni_APU_GROUP_PIN_COUNT; i++) <span class="comment">//for each pin</span></div>
			<div class="line">   {</div>
			<div class="line">   msLogResultAll( DSIndex++, APUGroupResults[i],  NUM_SITES, 0, 1 );</div>
			<div class="line">   }</div>
			<div class="line">  </div>
			<div class="line"><span class="comment">//Measure the voltage</span></div>
			<div class="line">apu12mv( IN1_OUT1, 100, 13);</div>
			<div class="line" ></div>
			<div class="line" ><span class="comment">//Datalog the results</span></div>
			<div class="line">groupgetresults( &amp;APUGroupResults2[0][0], NUM_SITES * coni_APU_GROUP_PIN_COUNT);</div>
			<div class="line"><span class="keywordflow">for</span> (i = 0; i &lt; coni_APU_GROUP_PIN_COUNT; i++)  <span class="comment">//for each pin</span></div></div>
			<div class="line">   {</div>
			<div class="line">   msLogResultAll( DSIndex++, APUGroupResults2[i],  NUM_SITES, 0, 1 );</div>
			<div class="line">   }</div>
			<div class="line">  </div>
			<div class="line"><span class="comment">//Record the resistance    </span></div>
			<div class="line"><span class="keywordflow">for</span> (i = 0; i &lt; coni_APU_GROUP_PIN_COUNT * NUM_SITES; i++)</div>
			<div class="line">   {</div>
			<div class="line">   Results[i] = APUGroupResults2[i];   </div>
			<div class="line">   Results[i].value = APUGroupResults2[i].value/APUGroupResults[i].value;     </div>
			<div class="line">   }</div>
			<div class="line">   </div>
			<div class="line" ><span class="comment">//Datalog the results</span></div>
			<div class="line"><span class="keywordflow">for</span> (i = 0; i &lt; coni_APU_GROUP_PIN_COUNT; i++) <span class="comment">//for each pin</span></div>
			<div class="line">   {</div>
			<div class="line">   msLogResultAll( DSIndex++, Results[i],  NUM_SITES, 0, 1 );</div>
			<div class="line">   }</div>		
		</td>
	</tr>
</table>



\subsection DataloggingSiteLoops Datalogging When Site Loops are Needed

	Site loops may be required when calculations need to be based on conditions at each site.
	The following sample code shows how to handle the site specific values and datalog the measurement.
	Note that a site loop is only required for the conditional ("if") statement.  Simple math functions can be handled without a loop. 
	@code
		//Declare variables
		SiteInt si1;
		SiteDouble sd1;
		SiteBool sb1;
		PinListData pldResults, pld1, pld2, pld3, pld4;

		//Measure OUT1 pin and store the results from all sites in a PinListData variable.
		pldResults=apu12mv( OUT1, 1, 13.0 );

		//Perform calculations using the measurements.
		pld1 = pldResults + 1.0;
		pld1 = 1 + pldResults;
		pld2 = pldResults - 2.0;
		pld2 = 2 - pldResults;
		pld3 = pldResults / 3;
		pld3 = 3.0  / pldResults;
		pld4 = pldResults * 4;
		pld4 = 4.0 * pldResults;

		//Initialize all sites in SiteInt variable to equal 1.
		si1 = 1;   

		//Initialize each site in SiteDouble variable to a unique value.
		sd1[0] = 0.0;   
		sd1[1] = 0.1;
		sd1[2] = 0.2;
		sd1[3] = 0.3;

		//Add the (site-unique) values stored in sd1 to pldResults. 
		pldResults = pldResults + sd1;	//The site loop occurs within the library. 

		//Set SiteBool variable for site 2 to false, and all other sites to true.
		sb1 = true;
		sb1[2] = false;

		//Perform calculations based on conditions at each site.
		for(int iSite=0; iSite<NUM_SITES;iSite++)
		{
			if (sb1[iSite])
			{
				pld1[iSite] = pld1[iSite] - sd1[iSite];
			}
			else
			{
				pld1[iSite] = si1[iSite];
			}

			//Datalog each site one at a time
			Maxim::Datalog(iSite, DSIndex, pld1[iSite]);
		}
		DSIndex++;    //When datalogging a single site at a time using Maxim::Datalog, Datasheet Index  must be manually incremented.

		//Datalog all sites at once (equivalent to above Maxim::Datalog call).  Note that DSIndex is automatically incremented when using Maxim::DatalogAll)
		Maxim::DatalogAll(DSIndex, pld1);

		//Datalog SiteVars
		Maxim::DatalogAll(DSIndex, si1);
		Maxim::DatalogAll(DSIndex, sd1);
		Maxim::DatalogAll(DSIndex, sb1);
	@endcode

	
**************************************************************************************************************************************************************/


#ifndef MAXIMLIB_PINLISTDATA_H
#define MAXIMLIB_PINLISTDATA_H
#define PLD_INVALID_RESOURCE 9999
#define PLD_UNKNOWN_RESOURCE 9998
#define NO_SITE -2	

#include "MaximLib\MaximLib_SiteVar.h"

	class PinListData 
	{
		public:
			PinListData();
			PinListData(RESULTS_STR* results,int num_pins, int iNumValid);	//construct a vector from an array of RESULTS_STR
			PinListData(GME_INT pinGroup);				//constructor for an empty PinListData variable sized for the pinGroup.
			PinListData(const PinListData &pldCopy);	//Copy constructor
			PinListData(const SiteDouble& sdVal); //cast SiteDouble to PinListData
			PinListData(const SiteInt& siVal);  //cast SiteInt to PinListData
			PinListData(const int& iVal); //cast int to PinListData
			PinListData(const double& dVal); //cast double to PinListData
			PinListData(const SiteBool& sbVal); //cast SiteBool to PinListData
			//~PinListData();

			//Conversion functions
			//operator double() const;		//cast to double
			RESULTS_STR* ResultsStructPtr() const; //cast to pointer to RESULTS_STR array.
			
			const int iGetNumPins() const;	//Number of pins in the group
			const size_t iGetVecSize() const;	//Vector size

			//copy another PinListData
			PinListData & operator=(const PinListData pld);	
			
			//Merge two PinListData objects (used to mesh results taken from different sites)
			PinListData operator&(const PinListData right);	
			PinListData operator&=(const PinListData right);

			//Math operators.
			PinListData operator+(const PinListData& right);
			PinListData operator-(const PinListData& right);
			PinListData operator*(const PinListData& right);
			PinListData operator/(const PinListData& right);
			PinListData operator+(const int& right);
			PinListData operator-(const int& right);
			PinListData operator*(const int& right);
			PinListData operator/(const int& right);
			PinListData operator+(const double& right);
			PinListData operator-(const double& right);
			PinListData operator*(const double& right);
			PinListData operator/(const double& right);
			PinListData operator+(const ULONG& right);
			PinListData operator-(const ULONG& right);
			PinListData operator*(const ULONG& right);
			PinListData operator/(const ULONG& right);			
			//return a single value from the PinListData.   This is only suited to looping through the entire PinListData
			double& operator[] (const unsigned int i); 
			const double& operator[] (const unsigned int i) const; 
			//return a single value from the PinListData.   
			double& operator() (const unsigned int Pin, const unsigned int Site); 

		private:
			vector<RESULTS_STR> vecPinData;
			int piNumPins;	//Number of pins in the group
			void pResult(RESULTS_STR& left, const RESULTS_STR& right, bool bCopyValue = false);
	
			PinListData Fix_RESULTS_STR_Site_Order(PinListData pldInput);
			void init_STR(RESULTS_STR& res);

	};


	
	PinListData operator + ( const SiteDouble & ULeft, const PinListData& pldRight);
	PinListData operator - ( const SiteDouble & ULeft, const PinListData& pldRight);
	PinListData operator / ( const SiteDouble & ULeft, const PinListData& pldRight);
	PinListData operator * ( const SiteDouble & ULeft, const PinListData& pldRight);
	PinListData operator + ( const SiteInt & ULeft, const PinListData& pldRight);
	PinListData operator - ( const SiteInt & ULeft, const PinListData& pldRight);
	PinListData operator / ( const SiteInt & ULeft, const PinListData& pldRight);
	PinListData operator * ( const SiteInt & ULeft, const PinListData& pldRight);
	PinListData operator + ( const int & ULeft, const PinListData& pldRight);
	PinListData operator - ( const int & ULeft, const PinListData& pldRight);
	PinListData operator / ( const int & ULeft, const PinListData& pldRight);
	PinListData operator * ( const int & ULeft, const PinListData& pldRight);
	PinListData operator + ( const double & ULeft, const PinListData& pldRight);
	PinListData operator - ( const double & ULeft, const PinListData& pldRight);
	PinListData operator / ( const double & ULeft, const PinListData& pldRight);
	PinListData operator * ( const double & ULeft, const PinListData& pldRight);
	
	//PinListData Math functions
	//Add PinListData Math functions here for now.
	PinListData abs(PinListData pldVal);
	PinListData log(PinListData pldVal);
	PinListData log10(PinListData pldVal);

	PinListData Maximaverage (char *cDataName, int iStart, int iStop);
	PinListData Maximdspaverage (char *cDataName, int iStart, int iStop);
	PinListData Maximpeak( char *cDataName, int iStart, int iStop, peaktype_t pHiOrLo);
	PinListData Maximdsppeak (char *cDataName, int iStart, int iStop, peaktype_t pHiOrLo);
	PinListData Maximwhereat (char *cDataName, int iStart, int iStop, double dValue);
	PinListData Maximdspwhereat (char *cDataName, int iStart, int iStop, double dValue);

	



#endif