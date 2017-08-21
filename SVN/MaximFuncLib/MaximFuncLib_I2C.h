/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:		MaximFuncLib_I2C.h
*   Written by:         John Paeng (john.paeng@MaximIntegrated.com)
*                       Jeffrey Jonathan (jeffrey.jonathan@MaximIntegrated.com)
*
*	URL:				$URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/MaximFuncLib/MaximFuncLib_I2C.h $
*   Last Modified by:	$LastChangedBy: govit.pimthong $
*	Date:				$Date: 2017-03-09 12:55:55 +0700 (Thu, 09 Mar 2017) $
*   Current Rev:		$Revision: 61735 $
*
*	Description:        I2C routines for DPU pins.  Not for built-in I2C bus.
*
*   Revision History
*		20120220  JP:   Initial Development and Release
*
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/

/**********************************************************************************************************************************************For*Doxygen**//**
@file	MaximFuncLib_I2C.h
@brief	I2C routines for DPU pins.  
@note	Not for built-in I2C bus.
**************************************************************************************************************************************************************/

// Following is documentation to be used by Doxygen to create help files.
/**********************************************************************************************************************************************For*Doxygen**//**
@page quickStartI2C I2C Quick Start

@details The following represents the recommended way of using the I2C Library.

-# Perform the following in Maxim_UserInit() which is in the @<dietype@>.cpp file
	-# Setup the MCU and dpin timing
		@code
			mcuset( MCU_MAIN, 50.0 );	// Set master clock frequency to 50MHz
			mcuclksource(MCU_ALL,MCU_MAIN);

			// I2C
			dpinperiod( 125 );  // dpinperiod function sets frequency of timing generator to a multiple of the master clock unit so 50MHz/125 = 400KHz
		@endcode
	-# Setup the dpin levels
		@code
			dpinlevelset( "SDA", NULL, 5.0, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
			dpinlevelset( "SCL", NULL, 5.0, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
		@endcode
	-# Specify the timing for your device using Maxim::I2C::SetupTimesets
		@note	Two examples are provided below.  Use the first if it works for you, otherwise use the second line.

		@code
			// First example
		      Maxim::I2C::SetupTimesets(FALSE, 400e3);    // Use default I2C timing edges which will be calculated as a percentage of the dpin period.  
			                                              //   The period provided should match the period resulting from what was set using mcuset and dpinperiod.
			// Second example
		      Maxim::I2C::SetupTimesets(TRUE, 400e3, 0.1, 0.6, 0.4, 0.8);	// Setup custom timing edges as a percentage of the dpin period.  The period provided  
																			//   should match the period resulting from what was set using mcuset and dpinperiod.
		@endcode
	-# Specify the address for your device using Maxim::I2C::SetSlaveAddress
		-#	Maxim::I2C::Check::SlaveAddress will tell you which addresses your device will respond to.  Very useful during debug if you weren't give the correct address.
			@code
		      siStatus = Maxim::I2C::SetSlaveAddress(0x03);  // Setup slave address for subsequent I2C communication
			@endcode
	@note If you use multiple timings or addresses then you will need to perform these steps somewhere besides Maxim_UserInit().
-# Copy the vectors into your main vector file
	- MaximFuncLib/MaximFuncLib_I2C.evd
-# Start communicating with your device
			@code
		      siStatus = Maxim::I2C::SendCommand(0xA4);
		      siStatus = Maxim::I2C::Write(0x06, 0x1C);
		      siStatus = Maxim::I2C::Read(0x10, siData);
			@endcode

@attention	If your I2C frequency is not 400kHz then you will need to modify the input to mcuset() and dpinperiod() functions in order to get your
				desired frequency.  This frequency will need to be supplied to the Maxim::I2C::SetupTimesets() function.
**************************************************************************************************************************************************************/

#ifndef MAXIMFUNCLIB_I2C
#define MAXIMFUNCLIB_I2C

namespace Maxim
{
	/************************************************************************************************************************************************************For*Doxygen**//**
	@brief Namespace used for I2C related items. See @ref quickStartI2C for usage.
	**************************************************************************************************************************************************************/
	namespace I2C
	{
		void SetupTimesets(bool bGangedSCL, double dFreq,  double dSCLFallEdge=0.0, double dSCLRiseEdge=0.67, double dSDADriveEdge=0.33, double dSDACompareEdge=0.83);

		SiteInt SetSlaveAddress (int iSlaveAddress);

		SiteInt CheckSlaveAddress ();		// multi-site
		int CheckSlaveAddress (int site);	// single-site

		SiteInt SendCommand (int iCommand);			// multi-site
		SiteInt SendCommand (int iAddr,int iCommand);			// multi-site
		//int SendCommandSS (int iSite, int iCommand);	// single-site
		//int SendCommandSS (int iSite, int iAddr, int iCommand);	// single-site

		SiteInt Write (int iReg, SiteInt siData);		// multi-site
		SiteInt Write (int iReg, int iData);			// multi-site
		SiteInt Write (int iAddr, int iReg, SiteInt siData);		// multi-site
		SiteInt Write (int iAddr, int iReg, int iData);			// multi-site
		//int WriteSS (int iSite, int iAddr, int iData);	// single-site

		SiteInt Read (int iAddr, int iReg, SiteInt& siData);		// multi-site
		int Read (int iSite, int iAddr, int iReg, int &iData);	// single-site
		SiteInt Read (int iReg, SiteInt& siData);		// multi-site
		int Read (int iSite, int iReg, int &iData);	// single-site

		int FindSlaveAddress (int site);	// single-site
	}
}

#endif
