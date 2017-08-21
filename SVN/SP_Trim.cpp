/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:
*		SP_Trim.cpp
*
*	Written and maintained by:
*		Govit Pimthong (govit.pimthong@MaximIntegrated.com)
*
*	URL:                   $URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/SP_Trim.cpp $
*	Last Modified by:      $LastChangedBy: govit.pimthong $
*	Date:                  $Date: 2017-05-12 14:49:22 +0700 (Fri, 12 May 2017) $
*	Current Rev:           $Revision: 64321 $
*
*	Description:
*		File Description
*
*	Revision History
*
*
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
#include "AP89_FT.h"

// Test Function: SP_Trim
ETS_PRGFLOW_FUNC SP_Trim( int DSIndex, LPCTSTR TestLabel )
{


	if(DUT::Trim::gsbSPLockBit.Any(false)){

		Maxim::I2C::SetSlaveAddress(OTP_3DV_AD);

		ScratchPad_XY_Write(DSIndex);

	}


    //End of function
    Maxim::DatalogFlush();	//Output the datalog queue.
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC