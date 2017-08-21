/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:		MaximFuncLib_I2C.cpp
*   Written by:         John Paeng (john.paeng@MaximIntegrated.com)
*                       Jeffrey Jonathan (jeffrey.jonathan@MaximIntegrated.com)
*
*	URL:				$URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/MaximFuncLib/MaximFuncLib_I2C.cpp $
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
@file	MaximFuncLib_I2C.cpp
@brief	I2C routines for DPU pins.  Not for built-in I2C bus.
**************************************************************************************************************************************************************/

namespace Maxim
{
	namespace I2C
	{

		/**********************************************************************************************************************************************For*Doxygen**//**
		Setup DPin timing for I2C vectors defined with SCL in RO (Return to One) format and SDA in NR (Non Return) format.
		 
		@param	bGangedSCL:     Is SCL ganged?  TRUE=ganged, FALSE=dedicated
		@param	dFreq:          I2C frequency
		@param	dSCLFallEdge:   Placement of SCL falling(first) edge in RO format expressed as a fraction of the period.
		@param	dSCLRiseEdge:   Placement of SCL rising(return) edge in RO format expressed as a fraction of the period.
		@param	dSDADriveEdge:  Placement of SDA drive edge expressed as a fraction of the period.
		@param	dSDACompareEdge: Placement of SDA compare edge expressed as a fraction of the period.
		
		@pre
		      1. The following vectors defined in Vector Editor
		          - i2c_slave_id_w
		          - i2c_slave_id_r
		          - i2c_check_sladdr
		          - i2c_send_command
		          - i2c_write_1byte
		          - i2c_read_1byte
		      2. The following time sets defined in Time Set Editor
		          - ts_i2c_scl_start
		          - ts_i2c_scl_stop
		          - ts_i2c_scl_send
		          - ts_i2c_scl_receive
		          - ts_i2c_sda_start
		          - ts_i2c_sda_stop
		          - ts_i2c_sda_send
		          - ts_i2c_sda_receive
		
		 @post
		      The time sets listed in preconditions will be setup according to the frequency and edge parameters
		
		 Usage Examples:
		 @code
		      Maxim::I2C::SetupTimesets(FALSE, 400e3);    // Setup time sets for dedicated SCL, I2C frequency of 400KHz, and defaults per declaration in MaximFuncLib_I2C.h
		      Maxim::I2C::SetupTimesets(TRUE, 200e3, 0.1, 0.6, 0.4, 0.8); // Setup time sets for ganged SCL, frequency of 200KHz, SCL falling edge at 500ns (0.1*5us period), ...
		 @endcode
		**************************************************************************************************************************************************************/
		void SetupTimesets(bool bGangedSCL, double dFreq,  double dSCLFallEdge, double dSCLRiseEdge, double dSDADriveEdge, double dSDACompareEdge)
		{
			double dPeriodns = 1e9/dFreq;	// in ns

			// i2c_slave_id_w
			if(bGangedSCL)
			{
				dpintimeset( "SCL", "i2c_slave_id_w", "ts_i2c_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_NO_SITE );
				dpintimeset( "SCL", "i2c_slave_id_w", "ts_i2c_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_NO_SITE );
				dpintimeset( "SCL", "i2c_slave_id_w", "ts_i2c_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_NO_SITE );
				dpintimeset( "SCL", "i2c_slave_id_w", "ts_i2c_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_NO_SITE );
			}
			else
			{
				dpintimeset( "SCL", "i2c_slave_id_w", "ts_i2c_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
				dpintimeset( "SCL", "i2c_slave_id_w", "ts_i2c_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
				dpintimeset( "SCL", "i2c_slave_id_w", "ts_i2c_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
				dpintimeset( "SCL", "i2c_slave_id_w", "ts_i2c_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
			}
			dpintimeset( "SDA", "i2c_slave_id_w", "ts_i2c_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
			dpintimeset( "SDA", "i2c_slave_id_w", "ts_i2c_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
			dpintimeset( "SDA", "i2c_slave_id_w", "ts_i2c_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
			dpintimeset( "SDA", "i2c_slave_id_w", "ts_i2c_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

			// i2c_slave_id_r
			if(bGangedSCL)
			{
				dpintimeset( "SCL", "i2c_slave_id_r", "ts_i2c_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_NO_SITE );
				dpintimeset( "SCL", "i2c_slave_id_r", "ts_i2c_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_NO_SITE );
				dpintimeset( "SCL", "i2c_slave_id_r", "ts_i2c_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_NO_SITE );
				dpintimeset( "SCL", "i2c_slave_id_r", "ts_i2c_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_NO_SITE );
			}
			else
			{
				dpintimeset( "SCL", "i2c_slave_id_r", "ts_i2c_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
				dpintimeset( "SCL", "i2c_slave_id_r", "ts_i2c_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
				dpintimeset( "SCL", "i2c_slave_id_r", "ts_i2c_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
				dpintimeset( "SCL", "i2c_slave_id_r", "ts_i2c_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
			}
			dpintimeset( "SDA", "i2c_slave_id_r", "ts_i2c_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
			dpintimeset( "SDA", "i2c_slave_id_r", "ts_i2c_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
			dpintimeset( "SDA", "i2c_slave_id_r", "ts_i2c_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
			dpintimeset( "SDA", "i2c_slave_id_r", "ts_i2c_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

			// i2c_check_sladdr
			if(bGangedSCL)
			{
				dpintimeset( "SCL", "i2c_check_sladdr", "ts_i2c_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_NO_SITE );
				dpintimeset( "SCL", "i2c_check_sladdr", "ts_i2c_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_NO_SITE );
				dpintimeset( "SCL", "i2c_check_sladdr", "ts_i2c_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_NO_SITE );
				dpintimeset( "SCL", "i2c_check_sladdr", "ts_i2c_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_NO_SITE );
			}
			else
			{
				dpintimeset( "SCL", "i2c_check_sladdr", "ts_i2c_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
				dpintimeset( "SCL", "i2c_check_sladdr", "ts_i2c_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
				dpintimeset( "SCL", "i2c_check_sladdr", "ts_i2c_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
				dpintimeset( "SCL", "i2c_check_sladdr", "ts_i2c_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
			}
			dpintimeset( "SDA", "i2c_check_sladdr", "ts_i2c_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
			dpintimeset( "SDA", "i2c_check_sladdr", "ts_i2c_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
			dpintimeset( "SDA", "i2c_check_sladdr", "ts_i2c_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
			dpintimeset( "SDA", "i2c_check_sladdr", "ts_i2c_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

			// i2c_send_command
			if(bGangedSCL)
			{
				dpintimeset( "SCL", "i2c_send_command", "ts_i2c_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_NO_SITE );
				dpintimeset( "SCL", "i2c_send_command", "ts_i2c_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_NO_SITE );
				dpintimeset( "SCL", "i2c_send_command", "ts_i2c_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_NO_SITE );
				dpintimeset( "SCL", "i2c_send_command", "ts_i2c_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_NO_SITE );
			}
			else
			{
				dpintimeset( "SCL", "i2c_send_command", "ts_i2c_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
				dpintimeset( "SCL", "i2c_send_command", "ts_i2c_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
				dpintimeset( "SCL", "i2c_send_command", "ts_i2c_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
				dpintimeset( "SCL", "i2c_send_command", "ts_i2c_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
			}
			dpintimeset( "SDA", "i2c_send_command", "ts_i2c_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
			dpintimeset( "SDA", "i2c_send_command", "ts_i2c_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
			dpintimeset( "SDA", "i2c_send_command", "ts_i2c_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
			dpintimeset( "SDA", "i2c_send_command", "ts_i2c_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

			// i2c_write_1byte
			if(bGangedSCL)
			{
				dpintimeset( "SCL", "i2c_write_1byte", "ts_i2c_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_NO_SITE );
				dpintimeset( "SCL", "i2c_write_1byte", "ts_i2c_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_NO_SITE );
				dpintimeset( "SCL", "i2c_write_1byte", "ts_i2c_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_NO_SITE );
				dpintimeset( "SCL", "i2c_write_1byte", "ts_i2c_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_NO_SITE );
			}
			else
			{
				dpintimeset( "SCL", "i2c_write_1byte", "ts_i2c_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
				dpintimeset( "SCL", "i2c_write_1byte", "ts_i2c_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
				dpintimeset( "SCL", "i2c_write_1byte", "ts_i2c_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
				dpintimeset( "SCL", "i2c_write_1byte", "ts_i2c_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
			}
			dpintimeset( "SDA", "i2c_write_1byte", "ts_i2c_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
			dpintimeset( "SDA", "i2c_write_1byte", "ts_i2c_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
			dpintimeset( "SDA", "i2c_write_1byte", "ts_i2c_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
			dpintimeset( "SDA", "i2c_write_1byte", "ts_i2c_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

			// i2c_read_1byte
			if(bGangedSCL)
			{
				dpintimeset( "SCL", "i2c_read_1byte", "ts_i2c_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_NO_SITE );
				dpintimeset( "SCL", "i2c_read_1byte", "ts_i2c_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_NO_SITE );
				dpintimeset( "SCL", "i2c_read_1byte", "ts_i2c_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_NO_SITE );
				dpintimeset( "SCL", "i2c_read_1byte", "ts_i2c_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_NO_SITE );
			}
			else
			{
				dpintimeset( "SCL", "i2c_read_1byte", "ts_i2c_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
				dpintimeset( "SCL", "i2c_read_1byte", "ts_i2c_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
				dpintimeset( "SCL", "i2c_read_1byte", "ts_i2c_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
				dpintimeset( "SCL", "i2c_read_1byte", "ts_i2c_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
			}
			dpintimeset( "SDA", "i2c_read_1byte", "ts_i2c_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
			dpintimeset( "SDA", "i2c_read_1byte", "ts_i2c_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
			dpintimeset( "SDA", "i2c_read_1byte", "ts_i2c_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
			dpintimeset( "SDA", "i2c_read_1byte", "ts_i2c_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

		}


		/*********************************************************************************************************************************************For*Doxygen**//**
		Set slave address in vector memory for all subsequent I2C commands, writes, and reads to the DUT.
		
		@param	iSlaveAddress  The slave address to be set in vector memory
		
		@pre
		      The following vectors defined in Vector Editor
		          - i2c_slave_id_w
		          - i2c_slave_id_r
		
		@post
		      1. The input slave address written in vector memory.
		      2. Return value:  0=setup successful per site
		
		 Usage Examples:
			@code
		      siStatus = Maxim::I2C::SetSlaveAddress(0x03);  // Setup slave address for subsequent I2C communication
		      siStatus = Maxim::I2C::SendCommand(0xA4);
		      siStatus = Maxim::I2C::Write(0x06, 0x1C);
		      siStatus = Maxim::I2C::Read(0x10, siData);
			@endcode
		**************************************************************************************************************************************************************/

		SiteInt SetSlaveAddress (int iSlaveAddress)
		{
			static int iPrevAddress = -1000;
			//Check to see if different than last address
			if (iPrevAddress == iSlaveAddress)
				return SiteInt(0);

			SiteInt siStatus;
			int iStatus;
			__int64 i64SlaveAddress[7];

			for (int i=0, j=6; i < 7; i++, j--)
				if( 1<<i & iSlaveAddress )
					i64SlaveAddress[j]=0xFFFF;
				else
					i64SlaveAddress[j]=0x0000;

			iStatus = dpinvecblockwrite ( "SDA", "i2c_slave_id_w", DPIN_DRVDATA, 0, 7, i64SlaveAddress, MS_ALL );
			for (int iSite = 0; iSite<giNumSites; iSite++)
				siStatus[iSite] = iStatus;
			iStatus = dpinvecblockwrite ( "SDA", "i2c_slave_id_r", DPIN_DRVDATA, 0, 7, i64SlaveAddress, MS_ALL );
			for (int iSite = 0; iSite<giNumSites; iSite++)
				siStatus[iSite] = int(siStatus[iSite]) | iStatus;

			return siStatus;
		}


		/************************************************************************************************************************************************************For*Doxygen**//**
		Send just the slave address + write bit and check for ACK from DUT
		
		@note			If parameter is omitted, then the command will be run multi-site.   
		
		@pre
		      1. i2c_slave_id_w defined in Vector Editor
		      2. I2C time sets defined and set up
		      3. Slave address setup up using SetSlaveAddress
		
		@return  0=ACK received from DUT

		 Usage Examples:
			@code
		      siStatus = Maxim::I2C::SetSlaveAddress(0x03);  // Setup slave address for subsequent I2C communication
		      for (iSiteIndex=0; iSiteIndex<giNumSites; iSiteIndex++)
		      {
		          if (msSiteStat(iSiteIndex))
		          {
		              // Connect one site at a time and check serially
		              siStatus = Maxim::I2C::CheckSlaveAddress();	// Check slave address (multi-site)
		          }
		      }
		      Maxim::DatalogAll(DSIndex, siStatus);
			@endcode
		**************************************************************************************************************************************************************/
		SiteInt CheckSlaveAddress ()	// multi-site
		{
			SiteInt siStatus;
			int iStatus;
			__int64 i64FailPins;

			iStatus = mcurun( NULL, "i2c_check_sladdr");

			for (int iSite=0; iSite<giNumSites; iSite++)
				if (msSiteStat(iSite))
					siStatus[iSite] = iStatus | dpingetfailpins( "SDA", DPIN_ALL_PINS, &i64FailPins, iSite );

			return siStatus;
		}

		int CheckSlaveAddress (int site)	// single-site
		{
			int iStatus;
			__int64 i64FailPins;

			iStatus = mcurun( NULL, "i2c_check_sladdr");
			iStatus |= dpingetfailpins( "SDA", DPIN_ALL_PINS, &i64FailPins, site );

			return iStatus;
		}


		/************************************************************************************************************************************************************For*Doxygen**//**
		Send slave address[7 bits] + write bit[1 bit] + command[8 bits] and check for ACKs from DUT
		
		@param	iCommand	Command code to send to DUT
		@note				If parameter is omitted, then the command will be run multi-site.   
		
		@pre
		      1. I2C vectors defined in Vector Editor
		      2. I2C time sets defined and set up
		      3. Slave address setup up using SetSlaveAddress
		
		@return  0=ACKs received from DUT
		
		Usage Examples:
			@code
		      siStatus = Maxim::I2C::SendCommand(0xA3);  // Send 0xA3 to all sites in parallel
		      for (iSiteIndex=0; iSiteIndex<giNumSites; iSiteIndex++)
		      {
		          if (msSiteStat(iSiteIndex))
		          {
		              // Connect one site at a time and send serially
		              siStatus[iSiteIndex] = Maxim::I2C::SendCommand(iSiteIndex, 0xA3);	// Send 0xA3 to all sites in series
		          }
		      }
			@endcode
		**************************************************************************************************************************************************************/
		SiteInt SendCommand (int iCommand)	// multi-site
		{
			SiteInt siStatus;
			int iStatus;
			int	i, j;
			__int64 i64FailPins;
			__int64 i64Comm[8];

			for (i=0, j=7; i < 8; i++, j--)
				if( 1<<i & iCommand )
					i64Comm[j]=0xFFFF;
				else
					i64Comm[j]=0x0000;
			iStatus = dpinvecblockwrite ( "SDA", "i2c_send_command", DPIN_DRVDATA, 3, 8, i64Comm, MS_ALL );

			iStatus |= mcurun( NULL, "i2c_send_command");

			for (int iSite=0; iSite<giNumSites; iSite++)
				if (msSiteStat(iSite))
					siStatus[iSite] = iStatus | dpingetfailpins( "SDA", DPIN_ALL_PINS, &i64FailPins, iSite );

			return siStatus;
		}

		SiteInt SendCommand (int iAddr, int iCommand)	// multi-site
		{

			Maxim::I2C::SetSlaveAddress(iAddr);
			return Maxim::I2C::SendCommand(iCommand);
		}


		/*********************************************************************************************************************************************For*Doxygen**//**
		Write an I2C command
		Send slave address[7 bits] + write bit[1 bit] + register address[8 bits] + data[8 bits] and check for ACKs from DUT
		
		@param iReg		Register address to write to
		@param siData	Data to write

		@note			If parameter is omitted, then the command will be run multi-site.   
		
		@pre
		      1. I2C vectors defined in Vector Editor
		      2. I2C time sets defined and set up
		      3. Slave address setup up using SetSlaveAddress
		
		 @return  0=ACKs received from DUT
		
		 Usage Examples:
			@code
		      siStatus = Maxim::I2C::Write(0x03, 0x14);  // Write R03h=0x14 to all sites in parallel
		      for (iSiteIndex=0; iSiteIndex<giNumSites; iSiteIndex++)
		      {
		          if (msSiteStat(iSiteIndex))
		          {
		              // Connect one site at a time and write serially
		              siStatus[iSiteIndex] = Maxim::I2C::Write(iSiteIndex, 0x03, 0x14);  // Write R03h=0x14 to all sites in series
		          }
		      }
			@endcode
		**************************************************************************************************************************************************************/
		SiteInt Write (int iReg, SiteInt siData)	// multi-site
		{
			SiteInt siStatus;
			int iStatus;
			int	i, j;
			__int64 i64FailPins;
			__int64 i64Addr[8];
			__int64 i64Data[8];

			for (i=0, j=7; i < 8; i++, j--)
				if( 1<<i & iReg )
					i64Addr[j]=0xFFFF;
				else
					i64Addr[j]=0x0000;
			iStatus = dpinvecblockwrite ( "SDA", "i2c_write_1byte", DPIN_DRVDATA, 3, 8, i64Addr, MS_ALL );

			for(int iSite = 0; iSite < giNumSites; iSite++ )
				if (msSiteStat(iSite))
				{
					for (i=0, j=7; i < 8; i++, j--)
						if( 1<<i & siData[iSite] )
							i64Data[j]=0x1;
						else
							i64Data[j]=0x0;

					siStatus[iSite] = iStatus | dpinvecblockwrite ( "SDA", "i2c_write_1byte", DPIN_DRVDATA, 12, 8, i64Data, iSite );
				}

			iStatus = mcurun( NULL, "i2c_write_1byte");

			for (int iSite=0; iSite<giNumSites; iSite++)
				if (msSiteStat(iSite))
					siStatus[iSite] = int(siStatus[iSite]) | iStatus | dpingetfailpins( "SDA", DPIN_ALL_PINS, &i64FailPins, iSite );

			return siStatus;
		}

		SiteInt Write (int iReg, int iData)	// multi-site
		{
			SiteInt siStatus;
			int iStatus;
			int	i, j;
			__int64 i64FailPins;
			__int64 i64Addr[8];
			__int64 i64Data[8];

			for (i=0, j=7; i < 8; i++, j--)
				if( 1<<i & iReg )
					i64Addr[j]=0xFFFF;
				else
					i64Addr[j]=0x0000;
			iStatus = dpinvecblockwrite ( "SDA", "i2c_write_1byte", DPIN_DRVDATA, 3, 8, i64Addr, MS_ALL );

			for (i=0, j=7; i < 8; i++, j--)
				if( 1<<i & iData )
					i64Data[j]=0xFFFF;
				else
					i64Data[j]=0x0000;
			iStatus |= dpinvecblockwrite ( "SDA", "i2c_write_1byte", DPIN_DRVDATA, 12, 8, i64Data, MS_ALL );

			iStatus |= mcurun( NULL, "i2c_write_1byte");

			for (int iSite=0; iSite<giNumSites; iSite++)
				if (msSiteStat(iSite))
					siStatus[iSite] = int(siStatus[iSite]) | iStatus | dpingetfailpins( "SDA", DPIN_ALL_PINS, &i64FailPins, iSite );

			return siStatus;
		}

		SiteInt Write (int iAddr, int iReg, SiteInt siData)	// multi-site
		{
			Maxim::I2C::SetSlaveAddress(iAddr);
			return Maxim::I2C::Write(iReg, siData);
		}

		SiteInt Write (int iAddr, int iReg, int iData)	// multi-site
		{
			Maxim::I2C::SetSlaveAddress(iAddr);
			return Maxim::I2C::Write(iReg,iData);
		}



		/**********************************************************************************************************************************************For*Doxygen**//**
		Read an I2C register
		Send		slave address[7 bits] + write bit[1 bit] + register address[8 bits] + repeat START @n
		          + slave address[7 bits] + read bit[1 bit]  + 8 clocks for read[8 bits] + NACK to DUT to stop data.
		
		@param	iReg	Register address to read from
		@param	siData	Read data from DUT
		
		@pre
		      1. I2C vectors defined in Vector Editor
		      2. I2C time sets defined and set up
		      3. "psq_i2c_capture" defined in PSQ Map Editor and assigned in Vector Editor
		      4. Slave address setup up using SetSlaveAddress
		
		@return  0=ACKs received from DUT
		
		Usage Examples:
			@code
		      SiteInt siData;
		      int     iData;
		      siStatus = Maxim::I2C::Read(0x03, siData);  // Read from R03h on all sites in parallel
		      for (iSiteIndex=0; iSiteIndex<giNumSites; iSiteIndex++)
		      {
		          if (msSiteStat(iSiteIndex))
		          {
		              // Connect one site at a time and write serially
		              siStatus[iSiteIndex] = Maxim::I2C::Read(iSiteIndex, 0x03, iData);  // Read from R03h on all sites in series
		              siData[iSiteIndex] = iData;
		          }
		      }
			@endcode
		**************************************************************************************************************************************************************/
		SiteInt Read (int iReg, SiteInt& siData)	// multi-site
		{
			SiteInt siStatus;
			int iStatus;
			int i, j;
			int capture_steps;
			__int64 i64Data[8] = {0,0,0,0,0,0,0,0};
			__int64 i64Addr[8];
			__int64 i64FailPins;

			for (i=0, j=7; i < 8; i++, j--)
				if( 1<<i & iReg )
					i64Addr[j]=0xFFFF;
				else
					i64Addr[j]=0x0000;

			iStatus  = dpinvecblockwrite ( "SDA", "i2c_read_1byte", DPIN_DRVDATA, 3, 8, i64Addr, MS_ALL );

			iStatus |= mcurun( NULL, "i2c_read_1byte");

			for (int iSite=0; iSite<giNumSites; iSite++)
				if (msSiteStat(iSite))
				{
					siStatus[iSite] = iStatus | dpingetfailpins( "SDA", DPIN_ALL_PINS, &i64FailPins, iSite );
					capture_steps = dpingetcapturedata ("SDA", DPIN_CAPTDATA, 8, i64Data, iSite);
					siData[iSite] = 0;
					for ( i=0, j=7; i < 8; i++, j--  )
						if( i64Data[j] )
							siData[iSite] = siData[iSite] + (1<<i);
				}

			return siStatus;
		}

		int Read (int iSite, int iReg, int &iData)	// single-site
		{
			int iStatus;
			int i, j;
			int capture_steps;
			__int64 i64Addr[8];
			__int64 i64Data[8] = {0,0,0,0,0,0,0,0};
			__int64 i64FailPins;

			for (i=0, j=7; i < 8; i++, j--)
				if( 1<<i & iReg )
					i64Addr[j]=0x1;
				else
					i64Addr[j]=0x0;

			iStatus = dpinvecblockwrite ( "SDA", "i2c_read_1byte", DPIN_DRVDATA, 3, 8, i64Addr, iSite );

			iStatus |= mcurun( NULL, "i2c_read_1byte");

			iStatus |= dpingetfailpins( "SDA", DPIN_ALL_PINS, &i64FailPins, iSite );

			capture_steps = dpingetcapturedata ("SDA", DPIN_CAPTDATA, 8, i64Data, iSite);

			for ( i=0, j=7; i < 8; i++, j--  )
				if( i64Data[j] )
					iData = iData + (1<<i);

			return iStatus;
		}

		SiteInt Read (int iAddr, int iReg, SiteInt& siData)	// multi-site
		{
			Maxim::I2C::SetSlaveAddress(iAddr);
			return Maxim::I2C::Read(iReg,siData);
		}

		int Read (int iSite, int iAddr, int iReg, int &iData)	// single-site
		{
			Maxim::I2C::SetSlaveAddress(iAddr);
			return Maxim::I2C::Read(iSite,iReg,iData);
		}

		/*******************************************************************************************************************************************For*Doxygen**//**
		Check all 128 possible slave addresses and find which ones pass and which fail.  Print out results to the Output tab of the ETS shell.
		
		@param site The test site that you want to find the slave address for

		@pre
		      1. I2C vectors defined in Vector Editor
		      2. I2C time sets defined and set up
		      3. Set up pin levels.
		
		@return		0=ACKs received from DUT
		
		Usage Examples:
			@code
		      dpinperiod( 125 );			// 50MHz/125 = 400KHz
		      dpinlevel( "SCL", NULL, 5.0, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_NO_SITE );
		      dpinlevel( "SDA", NULL, 5.0, 0.0, 1.5, 0.5, MS_ALL );
		      dpinviset( "SDA", DPIN_LOAD, 5.0, DPIN_8V, DPIN_2MA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL );
		      Maxim::I2C::FindSlaveAddress(0);   // Find slave addresses on site 0
			@endcode
		**************************************************************************************************************************************************************/
		int FindSlaveAddress (int site)	// single-site
		{
			int iStatus;
			int iReturnStatus;
			SiteInt siStatus;
			int iSlaveAddress;
			std::string stPF;

			etsprintf("\t\tDPin::FindSlaveAddress=================\n");
			etsprintf("\t\tSite\tSlaveAddr\tP/F\n");

			iReturnStatus = 0;
			for ( iSlaveAddress=0; iSlaveAddress < 0x80; iSlaveAddress++)
			{
				siStatus = SetSlaveAddress (iSlaveAddress);
				iStatus = CheckSlaveAddress (site);
				if(iStatus == 0)
					stPF = "<PASS>";
				else
					stPF = "FAIL";
				etsprintf("\t\t %1d \t %2X \t %5s \n",site,iSlaveAddress,stPF.c_str());
				iReturnStatus |= iStatus | siStatus[site];;
			}


			return iReturnStatus;
		}
	}
}
