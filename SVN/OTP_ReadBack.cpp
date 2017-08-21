/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:
*		OTP_ReadBack.cpp
*
*	Written and maintained by:
*		Govit Pimthong (govit.pimthong@MaximIntegrated.com)
*
*	URL:                   $URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/OTP_ReadBack.cpp $
*	Last Modified by:      $LastChangedBy: govit.pimthong $
*	Date:                  $Date: 2017-06-08 19:47:42 +0700 (Thu, 08 Jun 2017) $
*	Current Rev:           $Revision: 65481 $
*
*	Description:
*		File Description
*
*	Revision History
*
*
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
#include "AP89_FT.h"


template <typename T> void SiteDatalog(int &DSIndex, SiteBool sbSwitch, T stResult){

	for( int isite=0; isite<NUM_SITES; isite++)
		if (msSiteStat(isite) && !sbSwitch[isite])
			Maxim::Datalog(isite, DSIndex, stResult[isite]);

	DSIndex++;
}


// Test Function: OTP_ReadBack
ETS_PRGFLOW_FUNC OTP_ReadBack( int DSIndex, LPCTSTR TestLabel )
{
     
	SiteInt siReadBack, siTemp, siFUNC0, siFUNC1;
	SiteInt siBG_Match;
	SiteInt siDIM_Match;
	SiteInt siREF_Match;
	SiteInt siLDO_Match;
	SiteInt siBIAS_Match;
	SiteInt siSLOPE_Match;
	SiteInt siOSC_Match;
	SiteInt siOSC20M_Match;
	SiteInt siOUT_Gain_Match;
	SiteInt siOUT1_Match;
	SiteInt siOUT2_Match;
	SiteInt siOUT3_Match;
	SiteInt siOUT4_Match;
	SiteInt siOUT5_Match;
	SiteInt siOUT6_Match;


	/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
	* Test : OTP content Read back
	* Test No : #23000 - 23140
	* Description : Read back from all OTP registers 
	* Contour Link : 
	**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
	/*
	//OTP Trim data Registers		//OTP Trim Codes
									
	OTP_BG_REG= 0x00;				g_BG_code;
	OTP_REF_REG= 0x01;				g_REF_code;
	OTP_LDO_REG = 0x02;				g_LDO_code;
	OTP_OUT5_REG = 0x02;			g_BIAS_code;
	OTP_BIAS_REG = 0x03;			g_OSC_code;
	OTP_OSC_REG = 0x04;				g_OSC20M_code;
	OTP_SLOPE_REG = 0x05;			g_SLOPE_code;
	OTP_OUT_GAIN_REG = 0x06;		g_OUT_GAIN_code;
	OTP_OSC20M_REG = 0x07;			g_SPARE_code;
	OTP_OUT3_REG = 0x08;			g_OUT1_code;
	OTP_FUNC0_REG = 0x09;			g_OUT2_code;
	OTP_FUNC1_REG = 0x0A;			g_OUT3_code;
	OTP_BSTOSC_Bit = 0;				g_OUT4_code;
									g_OUT5_code;
									g_OUT6_code;
									g_FUNC0_code;
									g_FUNC1_code;
	*/



	Maxim::I2C::SetSlaveAddress(OTP_3DV_AD);

	//BG
	Maxim::I2C::Read(OTP_BG_REG, siTemp);

	ForEachActive_iSite	siReadBack[iSite] = siTemp[iSite] & 0x3F;

	siBG_Match = siReadBack - g_BG_code;

	Maxim::DatalogAll(DSIndex, siReadBack);


	//DIM bit
	ForEachActive_iSite	siReadBack[iSite] = (siTemp[iSite] & 0x40)?1:0;
	siDIM_Match = siReadBack - g_DIM_code;
	Maxim::DatalogAll(DSIndex, siReadBack);


	//REF_Reg
	Maxim::I2C::Read(OTP_REF_REG, siReadBack);
	siREF_Match = siReadBack - g_REF_code;
	Maxim::DatalogAll(DSIndex, siReadBack);


	//LDO_Reg
	if(!AP89){
		Maxim::I2C::Read(OTP_LDO_REG, siReadBack);
		siLDO_Match = siReadBack - g_LDO_code;
		Maxim::DatalogAll(DSIndex, siReadBack);
	}
	else
		DSIndex++;



	//IBIAS_Reg
	Maxim::I2C::Read(OTP_BIAS_REG, siReadBack);
	siBIAS_Match = siReadBack - g_BIAS_code;
	Maxim::DatalogAll(DSIndex, siReadBack);



	//SlopeComp_Reg
	Maxim::I2C::Read(OTP_SLOPE_REG, siTemp);

	ForEachActive_iSite{
		siReadBack[iSite] = siTemp[iSite] & 0x1F;
		siOUT6_Match[iSite] = (siTemp[iSite] & 0x20) >> 3;
	}

	siSLOPE_Match = siReadBack - g_SLOPE_code;
	Maxim::DatalogAll(DSIndex, siReadBack);


	//OSC_Reg
	Maxim::I2C::Read(OTP_OSC_REG, siTemp);

	ForEachActive_iSite{
		siReadBack[iSite] = siTemp[iSite] & 0x1F;
		siOUT6_Match[iSite] |= (siTemp[iSite] & 0x20) >> 4;
	}

	siOSC_Match = siReadBack - g_OSC_code;
	Maxim::DatalogAll(DSIndex, siReadBack);


	//OSC20M_Reg
	Maxim::I2C::Read(OTP_OSC20M_REG, siReadBack);
	siOSC20M_Match = siReadBack - g_OSC20M_code;
	Maxim::DatalogAll(DSIndex, siReadBack);


	//OUT_Gain_Reg
	Maxim::I2C::Read(OTP_OUT_GAIN_REG, siReadBack);
	siOUT_Gain_Match = siReadBack - g_OUT_GAIN_code;
	Maxim::DatalogAll(DSIndex, siReadBack);


	Maxim::I2C::Read(OTP_FUNC0_REG, siFUNC0);
	Maxim::I2C::Read(OTP_FUNC1_REG, siFUNC1);


	//OUT1_Reg
	ForEachActive_iSite	siReadBack[iSite] = siFUNC0[iSite] & 0x07;
	siOUT1_Match = siReadBack - g_OUT1_code;
	Maxim::DatalogAll(DSIndex, siReadBack);


	//OUT2_Reg
	ForEachActive_iSite	siReadBack[iSite] = (siFUNC0[iSite] & 0x38) >> 3;
	siOUT2_Match = siReadBack - g_OUT2_code;
	Maxim::DatalogAll(DSIndex, siReadBack);



	//OUT3_Reg
	Maxim::I2C::Read(OTP_OUT3_REG, siTemp);

	ForEachActive_iSite{
		siReadBack[iSite] = siTemp[iSite] & 0x07;
		siOUT6_Match[iSite] |= (siTemp[iSite] & 0x08) >> 3;
	}

	siOUT3_Match = siReadBack - g_OUT3_code;
	Maxim::DatalogAll(DSIndex, siReadBack);


	//OUT4_Reg
	ForEachActive_iSite	siReadBack[iSite] = siFUNC1[iSite] & 0x07;
	siOUT4_Match = siReadBack - g_OUT4_code;
	Maxim::DatalogAll(DSIndex, siReadBack);


	if(AP89){

		//OUT5_Reg
		Maxim::I2C::Read(OTP_OUT5_REG, siReadBack);
		siOUT5_Match = siReadBack - g_OUT5_code;
		Maxim::DatalogAll(DSIndex, siReadBack);


		//OUT6_Reg
		Maxim::DatalogAll(DSIndex, siOUT6_Match);
		siOUT6_Match = 	siOUT6_Match - g_OUT6_code;

	}
	else
		DSIndex+=2;


	//Check match only at first insertion
	if ( (Maxim::bTest_Step_Is("FT_ROOM") |Maxim::bTest_Step_Is("CHAR")) && DUT::Trim::gsbLockBit.Any(false))
	{


		SiteDatalog(DSIndex, DUT::Trim::gsbLockBit, siBG_Match);
		SiteDatalog(DSIndex, DUT::Trim::gsbLockBit, siDIM_Match);
		SiteDatalog(DSIndex, DUT::Trim::gsbLockBit, siREF_Match);

		if(!AP89)
			SiteDatalog(DSIndex, DUT::Trim::gsbLockBit, siLDO_Match);
		else
			DSIndex++;

		SiteDatalog(DSIndex, DUT::Trim::gsbLockBit, siBIAS_Match);
		SiteDatalog(DSIndex, DUT::Trim::gsbLockBit, siSLOPE_Match);
		SiteDatalog(DSIndex, DUT::Trim::gsbLockBit, siOSC_Match);
		SiteDatalog(DSIndex, DUT::Trim::gsbLockBit, siOSC20M_Match);
		SiteDatalog(DSIndex, DUT::Trim::gsbLockBit, siOUT_Gain_Match);
		SiteDatalog(DSIndex, DUT::Trim::gsbLockBit, siOUT1_Match);
		SiteDatalog(DSIndex, DUT::Trim::gsbLockBit, siOUT2_Match);
		SiteDatalog(DSIndex, DUT::Trim::gsbLockBit, siOUT3_Match);
		SiteDatalog(DSIndex, DUT::Trim::gsbLockBit, siOUT4_Match);

		if(AP89){
			SiteDatalog(DSIndex, DUT::Trim::gsbLockBit, siOUT5_Match);
			SiteDatalog(DSIndex, DUT::Trim::gsbLockBit, siOUT6_Match);
		}

	}

     
    //End of function
    Maxim::DatalogFlush();	//Output the datalog queue.
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC
