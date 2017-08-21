/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*                                                                            
*	Source title:		Digital_Levels_And_Timing.cpp                                
*		Created from Maxim Template                         
*		Template Written and maintained by:								 
*			Patrick Hayes (Patrick.Hayes@MaximIntegrated.com)	                 
*			Mark Bruening (Mark.Bruening@MaximIntegrated.com)                      
*		
*	URL:				$URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/Digital_Levels_And_Timing.cpp $
*		Last Modified by:	$LastChangedBy: sakrai.tongsansuk $                                       
*	Date:				$Date: 2017-07-13 16:47:29 +0700 (Thu, 13 Jul 2017) $                                                
*	Current Rev:		$Revision: 66789 $
*                                                                            
*	Description:  Digital Levels and Timing setup routine called from UserInit.                           
*                                                                            
*	Template Revision History                                                    
*		20110907 MWB: Initial Development and Release		       
*                                                                            
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/

#include "AP89_FT.h"


	/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
	* Maxim_Setup_Digital_Levels_And_Timing
	*   Purpose: Add the DUT's digital setup commands here.  This routine is called in UserInit.
	*	Arguments:	
	*		none
	*	Returns:
	*		Return true if successful.  Returning false will cause the program to abort loading.
	*   Remarks:
	*		
	*
	*   Revision History:
	*		
	*                                                                            
	**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
	bool Maxim_Setup_Digital_Levels_And_Timing()
	{


		//set up master clock
		mcuset(MCU_MAIN, 50.0);//50MHz.  (valid range=25-66MHz)
		mcuclksource(MCU_ALL, MCU_MAIN);
		dpinperiod(125);	//50Mhz/125=400KHz. Divide Master Clock by n (valid range=1-4096)

		//setup I2C pattern timing
		Maxim::I2C::SetupTimesets(FALSE, 400E3);	// nonganged SCL, 100KHz

		//Levels for I2C
		dpinlevelset("SCL", NULL, 4.0, 0.1, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
		dpinlevelset("SDA", NULL, 4.0, 0.1, 1.5, 0.5, DPIN_IGNORE_LEVEL, MS_ALL );	//SDA pullup
		dpinlevel( "SCAN_ENABLE", "EXT_SYNC", 4.0, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );

		double dScanPeriodRatio = 333; //in ns, formular setup.
		
		dpintimeset( "SCL_IO", "ScanChainInt", "DefaultTimesetIn&Out", DPIN_TS_NORMAL, DPIN_RZ , (0.5*dScanPeriodRatio), (0.8*dScanPeriodRatio), DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SDA_IO", "ScanChainInt", "DefaultTimesetIn&Out", DPIN_TS_NORMAL, DPIN_RZ , 0, (0.8*dScanPeriodRatio), DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SCAN_ENABLE", "ScanChainInt", "DefaultTimesetIn&Out", DPIN_TS_NORMAL, DPIN_NR , 0, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	    dpintimeset( "SCAN_RESET", "ScanChainInt", "DefaultTimesetIn&Out", DPIN_TS_NORMAL, DPIN_RO , (0.5*dScanPeriodRatio), (0.8*dScanPeriodRatio), DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SCAN_OUT", "ScanChainInt", "DefaultTimesetIn&Out", DPIN_TS_NORMAL, DPIN_NR , DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, (0.5*dScanPeriodRatio), ((0.5*dScanPeriodRatio) +15), MS_ALL );

		dpinlevel( "SCL_IO", "ScanChainInt", 4.0, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
		dpinlevel( "SDA_IO", "ScanChainInt", 4.0, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
		dpinlevel( "SCAN_ENABLE", "ScanChainInt", 4.0, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
		dpinlevel( "SCAN_RESET", "ScanChainInt", 4.0, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
		dpinlevel( "SCAN_OUT", "ScanChainInt", DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, 2.1, 0.5, MS_ALL );



		dpintimeset( "SCL_IO", "ScanStuckAtFault", "DefaultTimesetIn&Out", DPIN_TS_NORMAL, DPIN_RZ , (0.5*dScanPeriodRatio), (0.8*dScanPeriodRatio), DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SDA_IO", "ScanStuckAtFault", "DefaultTimesetIn&Out", DPIN_TS_NORMAL, DPIN_RZ , 0, (0.8*dScanPeriodRatio), DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SCAN_ENABLE", "ScanStuckAtFault", "DefaultTimesetIn&Out", DPIN_TS_NORMAL, DPIN_NR , 0, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	    dpintimeset( "SCAN_RESET", "ScanStuckAtFault", "DefaultTimesetIn&Out", DPIN_TS_NORMAL, DPIN_RO , (0.5*dScanPeriodRatio), (0.8*dScanPeriodRatio), DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SCAN_OUT", "ScanStuckAtFault", "DefaultTimesetIn&Out", DPIN_TS_NORMAL, DPIN_NR , DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, (0.5*dScanPeriodRatio), ((0.5*dScanPeriodRatio) +15), MS_ALL );

		dpinlevel( "SCL_IO", "ScanStuckAtFault", 4.0, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
		dpinlevel( "SDA_IO", "ScanStuckAtFault", 4.0, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
		dpinlevel( "SCAN_ENABLE", "ScanStuckAtFault", 4.0, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
		dpinlevel( "SCAN_RESET", "ScanStuckAtFault", 4.0, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
		dpinlevel( "SCAN_OUT", "ScanStuckAtFault", DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, 3.1, 0.5, MS_ALL );

		dpinlevelset("SCL_IO", "i2c_sda_low", 4.0, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
		dpinlevelset("SDA_IO", "i2c_sda_low", 4.0, 0.0, 2, 0.2, DPIN_IGNORE_LEVEL, MS_ALL );

		double dPeriodns = 1e9/400e3;
		double dSCLFallEdge = 0.0;
		double dSCLRiseEdge = 0.5;  //old std=0.67
		double dSDADriveEdge = 0.33;
		double dSDACompareEdge = 0.83;

		dpintimeset( "SCL_USER", "i2c_sda_low", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SCL_USER", "i2c_sda_low", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SCL_USER", "i2c_sda_low", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SCL_USER", "i2c_sda_low", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

		dpintimeset( "SDA_USER", "i2c_sda_low", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SDA_USER", "i2c_sda_low", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SDA_USER", "i2c_sda_low", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SDA_USER", "i2c_sda_low", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );



		//time set for BSTMON UV Blanking time
		dPeriodns = 1e9/400e3;	// in ns

		

		// i2c_slave_id_w
		dpintimeset( "SCL", "i2c_slave_id_w_bst_uv", "ts_i2c_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SCL", "i2c_slave_id_w_bst_uv", "ts_i2c_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SCL", "i2c_slave_id_w_bst_uv", "ts_i2c_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SCL", "i2c_slave_id_w_bst_uv", "ts_i2c_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

		dpintimeset( "SDA", "i2c_slave_id_w_bst_uv", "ts_i2c_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SDA", "i2c_slave_id_w_bst_uv", "ts_i2c_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SDA", "i2c_slave_id_w_bst_uv", "ts_i2c_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SDA", "i2c_slave_id_w_bst_uv", "ts_i2c_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SCAN_RESET", "i2c_slave_id_w_bst_uv", "DefaultTimesetIn&Out", DPIN_TS_NORMAL, DPIN_NR , 0, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

		// i2c_slave_id_r
		dpintimeset( "SCL", "i2c_slave_id_r_bst_uv", "ts_i2c_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SCL", "i2c_slave_id_r_bst_uv", "ts_i2c_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SCL", "i2c_slave_id_r_bst_uv", "ts_i2c_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SCL", "i2c_slave_id_r_bst_uv", "ts_i2c_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

		dpintimeset( "SDA", "i2c_slave_id_r_bst_uv", "ts_i2c_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SDA", "i2c_slave_id_r_bst_uv", "ts_i2c_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SDA", "i2c_slave_id_r_bst_uv", "ts_i2c_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SDA", "i2c_slave_id_r_bst_uv", "ts_i2c_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SCAN_RESET", "i2c_slave_id_r_bst_uv", "DefaultTimesetIn&Out", DPIN_TS_NORMAL, DPIN_NR , 0, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

		// i2c_check_sladdr
		dpintimeset( "SCL", "i2c_check_sladdr_bst_uv", "ts_i2c_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SCL", "i2c_check_sladdr_bst_uv", "ts_i2c_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SCL", "i2c_check_sladdr_bst_uv", "ts_i2c_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SCL", "i2c_check_sladdr_bst_uv", "ts_i2c_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

		dpintimeset( "SDA", "i2c_check_sladdr_bst_uv", "ts_i2c_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SDA", "i2c_check_sladdr_bst_uv", "ts_i2c_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SDA", "i2c_check_sladdr_bst_uv", "ts_i2c_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SDA", "i2c_check_sladdr_bst_uv", "ts_i2c_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SCAN_RESET", "i2c_check_sladdr_bst_uv", "DefaultTimesetIn&Out", DPIN_TS_NORMAL, DPIN_NR , 0, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

		// i2c_send_command
		dpintimeset( "SCL", "i2c_send_command_bst_uv", "ts_i2c_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SCL", "i2c_send_command_bst_uv", "ts_i2c_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SCL", "i2c_send_command_bst_uv", "ts_i2c_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SCL", "i2c_send_command_bst_uv", "ts_i2c_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

		dpintimeset( "SDA", "i2c_send_command_bst_uv", "ts_i2c_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SDA", "i2c_send_command_bst_uv", "ts_i2c_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SDA", "i2c_send_command_bst_uv", "ts_i2c_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SDA", "i2c_send_command_bst_uv", "ts_i2c_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SCAN_RESET", "i2c_send_command_bst_uv", "DefaultTimesetIn&Out", DPIN_TS_NORMAL, DPIN_NR , 0, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

		// i2c_write_1byte
		dpintimeset( "SCL", "i2c_write_1byte_bst_uv", "ts_i2c_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SCL", "i2c_write_1byte_bst_uv", "ts_i2c_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SCL", "i2c_write_1byte_bst_uv", "ts_i2c_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SCL", "i2c_write_1byte_bst_uv", "ts_i2c_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

		dpintimeset( "SDA", "i2c_write_1byte_bst_uv", "ts_i2c_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SDA", "i2c_write_1byte_bst_uv", "ts_i2c_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SDA", "i2c_write_1byte_bst_uv", "ts_i2c_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SDA", "i2c_write_1byte_bst_uv", "ts_i2c_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SCAN_RESET", "i2c_write_1byte_bst_uv", "DefaultTimesetIn&Out", DPIN_TS_NORMAL, DPIN_NR , 0, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

		// i2c_read_1byte
		dpintimeset( "SCL", "i2c_read_1byte_bst_uv", "ts_i2c_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SCL", "i2c_read_1byte_bst_uv", "ts_i2c_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SCL", "i2c_read_1byte_bst_uv", "ts_i2c_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SCL", "i2c_read_1byte_bst_uv", "ts_i2c_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

		dpintimeset( "SDA", "i2c_read_1byte_bst_uv", "ts_i2c_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SDA", "i2c_read_1byte_bst_uv", "ts_i2c_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SDA", "i2c_read_1byte_bst_uv", "ts_i2c_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SDA", "i2c_read_1byte_bst_uv", "ts_i2c_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
		dpintimeset( "SCAN_RESET", "i2c_read_1byte_bst_uv", "DefaultTimesetIn&Out", DPIN_TS_NORMAL, DPIN_NR , 0, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

		dpinlevel( "SCL_IO", NULL, 4.0, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
		dpinlevel( "SDA_IO", NULL, 4.0, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
		dpinlevel( "SCAN_RESET", NULL, 4.0, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );




		return true;

	}