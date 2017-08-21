
//DO NOT EDIT THIS FILE
/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*                                                                            
*	Source title:		MaximFuncLib_Wafer.h                                                                           		  
*	URL:				$URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/MaximFuncLib/MaximFuncLib_Wafer.h $
*   Last Modified by:	$LastChangedBy: govit.pimthong $                                       
*	Date:				$Date: 2017-03-09 12:55:55 +0700 (Thu, 09 Mar 2017) $                                                
*   Current Rev:		$Revision: 61735 $
*                                                                            
*	Description:  
*		Maxim library written and maintained by:								 
*			Patrick Hayes (Patrick.Hayes@MaximIntegrated.com)	                 
*			Mark Bruening (Mark.Bruening@MaximIntegrated.com)                                                                           
*

*                                                                            
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
//DO NOT EDIT THIS FILE

/**********************************************************************************************************************************************For*Doxygen**//**
@file	MaximFuncLib_Wafer.h
@brief	Maxim Function library file for Wafer Utilities.
**************************************************************************************************************************************************************/

namespace Maxim
{
	namespace Wafer
	{
		int GetWaferNumber();
		void GetXYCoordinates(SiteInt &siX, SiteInt &siY);
		int FTDatalogXYCoordinates(PinListData pldX,PinListData pldY );
		int FTDatalogWaferNumber(PinListData pldWaferNumber);
	}
}