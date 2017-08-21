//DO NOT EDIT THIS FILE
/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*                                                                            
*	Source title:		Maximlib_Results.H                                                                          		  
*	URL:				$URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/MaximLib/MaximLib_Results.h $
*   Last Modified by:	$LastChangedBy: govit.pimthong $                                       
*	Date:				$Date: 2017-03-09 12:55:55 +0700 (Thu, 09 Mar 2017) $                                                
*   Current Rev:		$Revision: 61735 $
*                                                                            
*	Description:  
*		Maxim library file for Results functions
*		Maxim library written and maintained by:								 
*			Patrick Hayes (Patrick.Hayes@MaximIntegrated.com)	                 
*			Mark Bruening (Mark.Bruening@MaximIntegrated.com)                                                                            
*
*   Revision History
*		See Maximlib.h file.
*                                                                            
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
//DO NOT EDIT THIS FILE

/**********************************************************************************************************************************************For*Doxygen**//**
@file	Maximlib_Results.h
@brief	Maxim library header file for Results
**************************************************************************************************************************************************************/

namespace Maxim
{
	namespace Results
	{
#ifdef MAXIMLIB_LIB 
		void SetResult(int iIndex, RESULTS_STR* rsResults); //used by library to store multi-site results for GetResults call.
		void SetResult(int iIndex, double dResult, int iSite); //used by library to store single site results for GetResults call.
		void ClearResults(); //used by library to clear Results map at end of test run 
#endif
		SiteDouble GetResults(int iTestNumber);  //Returns SiteDouble of datalogged results for iTestNumber

	}
}