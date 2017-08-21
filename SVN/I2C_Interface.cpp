/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:
*		I2C_Interface.cpp
*
*	Written and maintained by:
*		Govit Pimthong (govit.pimthong@MaximIntegrated.com)
*
*	URL:                   $URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/I2C_Interface.cpp $
*	Last Modified by:      $LastChangedBy: govit.pimthong $
*	Date:                  $Date: 2017-08-17 13:07:41 +0700 (Thu, 17 Aug 2017) $
*	Current Rev:           $Revision: 68112 $
*
*	Description:
*		File Description
*
*	Revision History
*
*
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
#include "AP89_FT.h"

PinListData I2C_Timing_Char(double Start_Force, double Stop_Force, double Step_Size, double Compare_Val, 
	int Threshold_Direction, void (*SetFunc)(double), PinListData (*MeasFunc)(void))
{
	PinListData out_val, Result;
	SiteBool found;
	int isite;
	double step;

	SetFunc(Start_Force);
	Result = out_val = MeasFunc();  

	found = TRUE;
	for (isite = 0; isite<NUM_SITES; isite++)
	{
		if (msSiteStat(isite))
			if (Threshold_Direction>0) 
				found[isite]=(out_val[isite]>Compare_Val);	// greater than
			else	
				found[isite]=(out_val[isite]<Compare_Val);	// less than
		Result[isite]=Start_Force;
	}

	step = Start_Force;
	while ((found.Any(FALSE)) && 
		(((Start_Force < Stop_Force) && (step <= Stop_Force)) || 
		((Start_Force > Stop_Force) && (step >= Stop_Force))))
	{
		if (Start_Force>Stop_Force) step-=fabs(Step_Size); //ramping down
		else step += fabs(Step_Size); //ramping up				
		SetFunc(step);
		out_val = MeasFunc();

		for (isite = 0; isite<NUM_SITES; isite++)
			if (msSiteStat(isite))
			{
				if (!found[isite])
					Result[isite]=step;
				if (Threshold_Direction>0) 
					found[isite]=(out_val[isite]>Compare_Val);	// greater than
				else	
					found[isite]=(out_val[isite]<Compare_Val);	// less than
			}
	}



	//fail if hit start_force or stop_force
	for (isite = 0; isite<NUM_SITES; isite++)
		if (msSiteStat(isite)){
			if(Start_Force < Stop_Force){
				if ((Result[isite] >= Stop_Force) || (Result[isite] <= Start_Force))
					Result[isite]=1e9;
			}
			else{
				if ((Result[isite] <= Stop_Force) || (Result[isite] >= Start_Force))
					Result[isite]=1e9;
			}
		}

	return Result;
}

void SDA_SET(SiteDouble value){

	ForEachActive_iSite{
		apu12setcapload( OUT4[iSite], APU12_FV, value[iSite], APU12_3p6V, APU12_10MA, 1, APU12_PIN_TO_VI );
	}

	lwait( 1 ms_f );

}

void SCL_SET(SiteDouble value){

	ForEachActive_iSite{
		apu12setcapload( OUT5[iSite], APU12_FV, value[iSite], APU12_3p6V, APU12_10MA, 1, APU12_PIN_TO_VI );
	}

	lwait( 1 ms_f );
}

void Set_tSCL_2p5(double stop);


PinListData Meas_i2cTime();
void SetDefaultI2Clevel(double dLevel);
PinListData SetDefaultI2Ctime();

void Set_i2cTime_slave_w(double stop);
void Set_i2cTime_SCLpulse_w(double stop);
void Set_i2cTime_SCLtSP_w(double stop);
void Set_i2cTime_SDAtSPlo_w(double stop);
void Set_i2cTime_SDAtSPhi_w(double stop);
void Set_i2cTime_SDAtSUdatRxHigh_w(double stop);
void Set_i2cTime_SDAtSUdatRxLo_w(double stop);
void Set_i2cTime_SDAtHDdatRxLo_w(double stop);
void Set_i2cTime_SDAtHDdatRxHigh_w(double stop);
void Set_i2cTime_tBUF_w(double stop);
void Set_i2cTime_tHD_STA_w(double stop);
void Set_i2cTime_tF_30_r(double stop);
void Set_i2cTime_tF_70_r(double stop);


// Test Function: I2C_Interface
ETS_PRGFLOW_FUNC I2C_Interface( int DSIndex, LPCTSTR TestLabel )
{

	//declare variables
	PinListData pldResults;	//declare a PLD variable for the measurement results.

	Maxim::I2C::SetSlaveAddress(TM_3DV_AD);

	Maxim::I2C::Write(0x02, 0xB0); //Reset BIPTM Machine
	Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine

	Maxim::I2C::Write(0x02, 0x61);
	Maxim::I2C::Write(0x02, 0x92); //DI<2>
	lwait(1 ms_f);

	cbitclose(K6, K32);
	apu12set( VCC, APU12_FV, 0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );
	apu12setcapload( OUT4, APU12_FV, 0, APU12_10V, APU12_100MA, 1, APU12_PIN_TO_VI );
	apu12set( OUT4, APU12_FV, 0, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI );

	dpinforce("SCL_IO", "0", 3.0, 0.0, MS_ALL);
	lwait(1 ms_f);
	dpinforce("SDA_IO", "0", 3.0, 0.0, MS_ALL);
	cbitclose(K12);
	dpindisconnect(MS_ALL, "SDA_IO");
	lwait(3 ms_f);



	THForcePin SetPin = {
		OUT4,
		APU12_3p6V,
		APU12_10MA,
		APU12_FV
	};

	THMeasPin MeasPin = {
		VCC,
		APU12_10V,
		APU12_10UA,
		APU12_MV
	};

	TestTH tsTH =
	{
		SetPin,                 // Force pin detail
		MeasPin,                // Meas pin detail
		SDA_SET,            // Threshold set function
		VCC_MeasV,           // Threshold measure function
		"SDA_R",               // Rising AWG Name
		"SDA_F",               // Falling AWG Name
		true,                   // Use AWG
		5000,                  // MCU divider
		APU12_ABOVE_THRESH,     // Threshold direction for rising, falling will be the opposite.
		1 mV_f,                 // Step size
		1.0,        // Output transition point when rising threshold is reached.
		1.0,        // Output transition point when falling threshold is reached.
		1,                     // Sample to skip before using awgwhereat to calculate threshold (in case using AWG)
		false,                  // Find threhsold from right (not usual case)
		false,                  // True if datalong is in percentage of some REF value
		0,                      // Ref value associate with percentage.
		25,                      // Offset to use with Go-no-Go , this number will be multiply with step size.
		1.0,					//Ref value in pld format
		true					//Falling search
	};


	ThresholdTest (DSIndex, &tsTH);


	apu12setcapload( OUT4, APU12_FV, 0, APU12_3p6V, APU12_10MA, 1, APU12_PIN_TO_VI );
	lwait(1 ms_f);

	dpinforce("SDA_IO", "0", 3.0, 0.0, MS_ALL);

	cbitopen(K12);
	lwait(3 ms_f);

	apu12setcapload( OUT4, APU12_FV, 0.7, APU12_10V, APU12_100MA, 1, APU12_PIN_TO_VI );








	Maxim::I2C::Write(0x02, 0x61);
	Maxim::I2C::Write(0x02, 0x93); //DI<3>
	lwait(1 ms_f);




	apu12setcapload( OUT5, APU12_FV, 0, APU12_10V, APU12_100MA, 1, APU12_PIN_TO_VI );
	apu12set( OUT5, APU12_FV, 0, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI );

	dpinforce("SCL_IO", "0", 3.0, 0.0, MS_ALL);
	lwait(100 us_f);
	dpinforce("SDA_IO", "0", 3.0, 0.0, MS_ALL);
	cbitopen(K41);
	lwait(1 ms_f);

	cbitclose(K13);
	lwait(3 ms_f);


	tsTH.SetPin.pinName = OUT5;
	tsTH.TH_Set = SCL_SET;
	tsTH.AwgNameR = "SCL_R";
	tsTH.AwgNameF = "SCL_F";



	ThresholdTest (DSIndex, &tsTH);



	cbitopen(K6, K32);
	apu12set( VCC, APU12_OFF, 0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );
	apu12setcapload( OUT5, APU12_FV, 0, APU12_10V, APU12_10MA, 1, APU12_PIN_TO_VI );
	apu12set( OUT5, APU12_FV, 0, APU12_3p6V, APU12_10MA, APU12_PIN_TO_VI );

	dpinforce("SDA_IO", "0", 3.0, 0.0, MS_ALL);
	cbitopen(K13);
	lwait(3 ms_f);
	cbitclose(K41);


	apu12setcapload( OUT5, APU12_FV, 0.7, APU12_10V, APU12_100MA, 1, APU12_PIN_TO_VI );





	/************************************************************************************************************
	*  TEST SDA_VOL
	*       
	*  SHORT: SDA Output Low Voltage
	*
	**************************************************************************************************************/
	PinListData pldSDA_VOL;

	



	//setup
	mcurun( NULL, "i2c_sda_low");
	lwait(300 us_f);

	dpinviset("SDA_IO", DPIN_FI, 1 mA_f, DPIN_8V, DPIN_2MA );
	lwait(2500 us_f);

	//measure
	pldSDA_VOL = dpinmv("SDA_IO", 100, 13.0, MS_ALL );

	dpinviset("SDA_IO", DPIN_LOAD, 4.3 V_f, DPIN_8V, DPIN_128UA );
	lwait(1000 us_f);

	//restore
	mcurun( NULL, "i2c_sda_low_reset");
	lwait(200 us_f);

	//datalog
	Maxim::DatalogAll(DSIndex, pldSDA_VOL *V); //V


	if(GetUserSwitch(GPSW_Characterization_Mode))
	{


		//dpinviset("SDA", DPIN_LOAD, 4, DPIN_8V, DPIN_8MA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL);

		/************************************************************************************************************
		*  I2C Timing Characterization 
		*       
		*
		**************************************************************************************************************/
		PinListData pldStatus;

		//set default condition
		pldStatus = SetDefaultI2Ctime();

		double dLevel = 5.0;

		dpinlevelset("SCL_USER", "i2cTime_tF_slave_w", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
		dpinlevelset("SDA_USER", "i2cTime_tF_slave_w", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );
		dpinlevelset("SCL_USER", "i2cTime_tF_30_slave_r", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
		dpinlevelset("SDA_USER", "i2cTime_tF_30_slave_r", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );
		dpinlevelset("SCL_USER", "i2cTime_tF_30_read4byte", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
		dpinlevelset("SDA_USER", "i2cTime_tF_30_read4byte", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );

		dpinlevelset("SCL_USER", "i2cTime_tF_70_slave_r", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
		dpinlevelset("SDA_USER", "i2cTime_tF_70_slave_r", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );
		dpinlevelset("SCL_USER", "i2cTime_tF_70_read4byte", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
		dpinlevelset("SDA_USER", "i2cTime_tF_70_read4byte", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );

		/************************************************************************************************************
		*  I2C Timing Setup
		*       
		*
		**************************************************************************************************************/
		PinListData pldtSCL_2p5;
		PinListData pldSCL_tHIGH_2p5;
		PinListData pldSCL_tLOW_2p5;
		PinListData pldSCL_tSP_H_2p5;
		PinListData pldSCL_tSP_L_2p5;
		PinListData pldSDA_tSP_H_2p5;
		PinListData pldSDA_tSP_L_2p5;
		PinListData pldSDA_tSU_DAT_RX_HIGH_2p5;
		PinListData pldSDA_tSU_DAT_RX_LOW_2p5;
		PinListData pldSDA_tHD_DAT_RX_HIGH_2p5;
		PinListData pldSDA_tHD_DAT_RX_LOW_2p5;
		PinListData pldSDA_tBUF_2p5;
		PinListData pldSDA_tSU_STA_2p5;
		PinListData pldSDA_tHD_STA_2p5;
		PinListData pldSDA_tSU_STO_2p5;
		PinListData pldSDA_tF_30_2p5;
		PinListData pldSDA_tF_70_2p5;
		PinListData pldSDA_tF_2p5; //computation

		double dStep = 0.001;

		//Maximum SCL frequency (No test needed, garanteed by tLOW and tHIGH)
		//pldtSCL_2p5 = I2C_Timing_Char(50, 10, 1, 0.5, +1, Set_i2cTime_slave_w, Meas_i2cTime);
		//pldtSCL_2p5 = (1e6)*(50/(pldtSCL_2p5 + 1)); //in Hz increased by 1 step size.

		//restore condition
		//pldStatus = SetDefaultI2Ctime();

		//Minumum SCL High Time
		pldSCL_tHIGH_2p5 = I2C_Timing_Char(0.93, 0.99, dStep, 0.5, +1, Set_i2cTime_SCLpulse_w, Meas_i2cTime);
		pldSCL_tHIGH_2p5 = (2.5e3)*(1 - (pldSCL_tHIGH_2p5 - dStep)); //in ns deduced by 1 step size.

		//restore condition
		pldStatus = SetDefaultI2Ctime();

		//Minimum SCL Low Time
		pldSCL_tLOW_2p5 = I2C_Timing_Char(0.07, 0.01, dStep, 0.5, +1, Set_i2cTime_SCLpulse_w, Meas_i2cTime);
		pldSCL_tLOW_2p5 = (2.5e3)*(pldSCL_tLOW_2p5 + dStep); //in ns increased by 1 step size.

		//restore condition
		pldStatus = SetDefaultI2Ctime();

		//Positive spike on SCL
		pldSCL_tSP_H_2p5 = I2C_Timing_Char(0.98, 0.5, dStep, 0.5, +1, Set_i2cTime_SCLtSP_w, Meas_i2cTime);
		pldSCL_tSP_H_2p5 = (2.5e3)*(1 - (pldSCL_tSP_H_2p5 - dStep)); //in ns deduced by 1 step size

		//restore condition
		pldStatus = SetDefaultI2Ctime();

		//Negative spike on SCL
		pldSCL_tSP_L_2p5 = I2C_Timing_Char(0.03, 0.5, dStep, 0.5, +1, Set_i2cTime_SCLtSP_w, Meas_i2cTime);
		pldSCL_tSP_L_2p5 = (2.5e3)*(pldSCL_tSP_L_2p5 + dStep); //in ns increased by 1 step size

		//restore condition
		pldStatus = SetDefaultI2Ctime();

		//Positive spike on SDA
		pldSDA_tSP_H_2p5 = I2C_Timing_Char(0.985, 0.5, dStep, 0.5, +1, Set_i2cTime_SDAtSPlo_w, Meas_i2cTime);
		pldSDA_tSP_H_2p5 = (2.5e3)*(1.01 - (pldSDA_tSP_H_2p5 + dStep)); //in ns increased by 1 step size, plus 0.01*offset

		//restore condition
		pldStatus = SetDefaultI2Ctime();

		//Negative spike on SDA
		pldSDA_tSP_L_2p5 = I2C_Timing_Char(0.985, 0.5, dStep, 0.5, +1, Set_i2cTime_SDAtSPhi_w, Meas_i2cTime);
		pldSDA_tSP_L_2p5 = (2.5e3)*(1.01 - (pldSDA_tSP_L_2p5 + dStep)); //in ns increased by 1 step size, plus 0.01*offset

		//restore condition
		pldStatus = SetDefaultI2Ctime();

		//Data Setup Time, RX test in rising case
		pldSDA_tSU_DAT_RX_HIGH_2p5 = I2C_Timing_Char(0.46, 0.6, dStep, 0.5, +1, Set_i2cTime_SDAtSUdatRxHigh_w, Meas_i2cTime);
		pldSDA_tSU_STO_2p5 = (2.5e3)*(pldSDA_tSU_DAT_RX_HIGH_2p5 - 0.5);  // setup time for a stop condition, from SCL to SDA
		pldSDA_tSU_DAT_RX_HIGH_2p5 = (2.5e3)*(0.5 - (pldSDA_tSU_DAT_RX_HIGH_2p5 - dStep)); //in ns decreased by 1 step size, from SDA to SCL

		//restore condition
		pldStatus = SetDefaultI2Ctime();

		//Data Setup Time, RX test in falling case
		pldSDA_tSU_DAT_RX_LOW_2p5 = I2C_Timing_Char(0.45, 0.6, dStep, 0.5, +1, Set_i2cTime_SDAtSUdatRxLo_w, Meas_i2cTime);
		pldSDA_tSU_STA_2p5 = (2.5e3)*(pldSDA_tSU_DAT_RX_LOW_2p5 - 0.5); // setup time for a repeated start condition, from SCL to SDA
		pldSDA_tSU_DAT_RX_LOW_2p5 = (2.5e3)*(0.5 - (pldSDA_tSU_DAT_RX_LOW_2p5 - dStep)); //in ns decreased by 1 step size.

		//restore condition
		pldStatus = SetDefaultI2Ctime();

		//Data Hold Time, RX test in falling data transition (negative value)
		pldSDA_tHD_DAT_RX_LOW_2p5 = I2C_Timing_Char(0.96, 0.998, dStep, 0.5, -1, Set_i2cTime_SDAtHDdatRxLo_w, Meas_i2cTime);
		pldSDA_tHD_DAT_RX_LOW_2p5 = (2.5e3)*(pldSDA_tHD_DAT_RX_LOW_2p5 - 1); //in ns. (SDA transition can happen before the falling SCL and still get an ACK) 

		//restore condition
		pldStatus = SetDefaultI2Ctime();

		//Data Hold Time, RX test in rising data transition (negative value)
		pldSDA_tHD_DAT_RX_HIGH_2p5 = I2C_Timing_Char(0.95, 0.998, dStep, 0.5, -1, Set_i2cTime_SDAtHDdatRxHigh_w, Meas_i2cTime);
		pldSDA_tHD_DAT_RX_HIGH_2p5 = (2.5e3)*(pldSDA_tHD_DAT_RX_HIGH_2p5 - 1); //in ns. (SDA transition can happen before the falling SCL and still get an ACK) 

		//restore condition
		pldStatus = SetDefaultI2Ctime();

		//Bus Free Time between Start and Stop testing (tBUF)
		pldSDA_tBUF_2p5 = I2C_Timing_Char(0.93, 0.98, dStep, 0.5, +1, Set_i2cTime_tBUF_w, Meas_i2cTime);
		pldSDA_tBUF_2p5 = (2.5e3)*(1.01 - (pldSDA_tBUF_2p5 - dStep)); //in ns. increased by 1 step size. 

		//restore condition
		pldStatus = SetDefaultI2Ctime();

		//Hold Time for a START Condition (tHD_STA))
		pldSDA_tHD_STA_2p5 = I2C_Timing_Char(0.95, 0.998, dStep, 0.5, +1, Set_i2cTime_tHD_STA_w, Meas_i2cTime);
		pldSDA_tHD_STA_2p5 = (2.5e3)*(1 - (pldSDA_tHD_STA_2p5 - dStep)); //in ns. decreased by 1 step size. 

		//restore condition
		pldStatus = SetDefaultI2Ctime();

		//SDA Transmitting Fall Time (tF)
		pldSDA_tF_30_2p5 = I2C_Timing_Char(0.08, 0.25, dStep, 0.5, -1, Set_i2cTime_tF_30_r, Meas_i2cTime);
		pldSDA_tF_30_2p5 = (2.5e3)*(pldSDA_tF_30_2p5); //in ns. 

		//restore condition
		pldStatus = SetDefaultI2Ctime();

		//SDA Transmitting Fall Time (tF)
		//pldSDA_tF_70_2p5 = I2C_Timing_Char(0.05, 0.25, 0.0005, 0.5, 1, Set_i2cTime_tF_70_r, Meas_i2cTime);
		pldSDA_tF_70_2p5 = I2C_Timing_Char(0.1, 0.05, dStep, 0.5, -1, Set_i2cTime_tF_70_r, Meas_i2cTime);
		pldSDA_tF_70_2p5 = (2.5e3)*(pldSDA_tF_70_2p5); //in ns. 

		//computation
		pldSDA_tF_2p5 = pldSDA_tF_30_2p5 - pldSDA_tF_70_2p5; //in ns

		//datalog
		Maxim::DatalogAll(DSIndex, pldSCL_tHIGH_2p5 *ns); //ns
		Maxim::DatalogAll(DSIndex, pldSCL_tLOW_2p5 *ns); //ns
		Maxim::DatalogAll(DSIndex, pldSCL_tSP_H_2p5 *ns); //ns
		Maxim::DatalogAll(DSIndex, pldSCL_tSP_L_2p5 *ns); //ns
		Maxim::DatalogAll(DSIndex, pldSDA_tSP_H_2p5 *ns); //ns
		Maxim::DatalogAll(DSIndex, pldSDA_tSP_L_2p5 *ns); //ns
		Maxim::DatalogAll(DSIndex, pldSDA_tSU_DAT_RX_HIGH_2p5 *ns); //ns
		Maxim::DatalogAll(DSIndex, pldSDA_tSU_STO_2p5 *ns); //ns
		Maxim::DatalogAll(DSIndex, pldSDA_tSU_DAT_RX_LOW_2p5 *ns); //ns
		Maxim::DatalogAll(DSIndex, pldSDA_tSU_STA_2p5 *ns); //ns
		Maxim::DatalogAll(DSIndex, pldSDA_tHD_DAT_RX_LOW_2p5 *ns); //ns
		Maxim::DatalogAll(DSIndex, pldSDA_tHD_DAT_RX_HIGH_2p5 *ns); //ns
		Maxim::DatalogAll(DSIndex, pldSDA_tBUF_2p5 *ns); //ns
		Maxim::DatalogAll(DSIndex, pldSDA_tHD_STA_2p5 *ns); //ns
		Maxim::DatalogAll(DSIndex, pldSDA_tF_2p5 *ns); //ns


	}


	//End of function
	Maxim::DatalogFlush();	//Output the datalog queue.
	return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC






/************************************************************************************
*
*  I2C Timing
*
*************************************************************************************/
void SetDefaultI2Clevel(double dLevel)
{
	//DPIN Levels setting***********************************************************************************************************************************************

	dpinlevelset("SCL_USER", "i2cTime_slave_w", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SDA_USER", "i2cTime_slave_w", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SCL_USER", "i2cTime_slave_w1byte", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SDA_USER", "i2cTime_slave_w1byte", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );

	dpinlevelset("SCL_USER", "i2cTime_SCLpulse_w", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SDA_USER", "i2cTime_SCLpulse_w", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SCL_USER", "i2cTime_SCLpulse_w1byte", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SDA_USER", "i2cTime_SCLpulse_w1byte", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );

	dpinlevelset("SCL_USER", "i2cTime_SCLtSP_w", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SDA_USER", "i2cTime_SCLtSP_w", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SCL_USER", "i2cTime_SCLtSP_w1byte", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SDA_USER", "i2cTime_SCLtSP_w1byte", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );

	dpinlevelset("SCL_USER", "i2cTime_SDAtSPlo_w", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SDA_USER", "i2cTime_SDAtSPlo_w", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SCL_USER", "i2cTime_SDAtSPlo_w1byte", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SDA_USER", "i2cTime_SDAtSPlo_w1byte", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );

	dpinlevelset("SCL_USER", "i2cTime_SDAtSPhi_w", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SDA_USER", "i2cTime_SDAtSPhi_w", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SCL_USER", "i2cTime_SDAtSPhi_w1byte", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SDA_USER", "i2cTime_SDAtSPhi_w1byte", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );

	dpinlevelset("SCL_USER", "i2cTime_SDAtSUdatRxHigh_w", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SDA_USER", "i2cTime_SDAtSUdatRxHigh_w", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SCL_USER", "i2cTime_SDAtSUdatRxHigh_w1byte", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SDA_USER", "i2cTime_SDAtSUdatRxHigh_w1byte", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );

	dpinlevelset("SCL_USER", "i2cTime_SDAtSUdatRxLo_w", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SDA_USER", "i2cTime_SDAtSUdatRxLo_w", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SCL_USER", "i2cTime_SDAtSUdatRxLo_w1byte", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SDA_USER", "i2cTime_SDAtSUdatRxLo_w1byte", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );

	dpinlevelset("SCL_USER", "i2cTime_SDAtHDdatRxLo_w", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SDA_USER", "i2cTime_SDAtHDdatRxLo_w", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SCL_USER", "i2cTime_SDAtHDdatRxLo_w1byte", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SDA_USER", "i2cTime_SDAtHDdatRxLo_w1byte", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );

	dpinlevelset("SCL_USER", "i2cTime_SDAtHDdatRxHigh_w", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SDA_USER", "i2cTime_SDAtHDdatRxHigh_w", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SCL_USER", "i2cTime_SDAtHDdatRxHigh_w1byte", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SDA_USER", "i2cTime_SDAtHDdatRxHigh_w1byte", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );

	dpinlevelset("SCL_USER", "i2cTime_tBUF_10001b_w", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SCL_USER", "i2cTime_tBUF_10101b_w", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SCL_USER", "i2cTime_tBUF_w2byte", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );

	dpinlevelset("SDA_USER", "i2cTime_tBUF_10001b_w", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SDA_USER", "i2cTime_tBUF_10101b_w", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SDA_USER", "i2cTime_tBUF_w2byte", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );

	dpinlevelset("SCL_USER", "i2cTime_tHD_STA_w", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SDA_USER", "i2cTime_tHD_STA_w", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SCL_USER", "i2cTime_tHD_STA_w1byte", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SDA_USER", "i2cTime_tHD_STA_w1byte", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );

	dpinlevelset("SCL_USER", "i2cTime_tF_slave_w", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SDA_USER", "i2cTime_tF_slave_w", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SCL_USER", "i2cTime_tF_30_slave_r", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SDA_USER", "i2cTime_tF_30_slave_r", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SCL_USER", "i2cTime_tF_30_read4byte", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SDA_USER", "i2cTime_tF_30_read4byte", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );

	dpinlevelset("SCL_USER", "i2cTime_tF_70_slave_r", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SDA_USER", "i2cTime_tF_70_slave_r", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SCL_USER", "i2cTime_tF_70_read4byte", dLevel, 0.0, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, DPIN_IGNORE_LEVEL, MS_ALL );
	dpinlevelset("SDA_USER", "i2cTime_tF_70_read4byte", dLevel, 0.0, (0.7 * dLevel), (0.3 * dLevel), DPIN_IGNORE_LEVEL, MS_ALL );


}
PinListData SetDefaultI2Ctime()
{
	PinListData pldStatus;
	SiteInt  siStatus;
	siStatus = SiteInt(1);

	//setup
	double dPeriodns = 2.5e3;	// in ns
	double dSCLFallEdge = 0.0;
	double dSCLRiseEdge = 0.67;
	double dSDADriveEdge = 0.33;
	double dSDACompareEdge = 0.83;

	dpinperiod(125); //125 = 400KHz for I2C timing

	//tSCL - i2cTime_slave_w***************************************************************************************************************************************
	dpintimeset( "SCL_USER", "i2cTime_slave_w", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_slave_w", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_slave_w", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_slave_w", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_slave_w", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_slave_w", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_slave_w", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_slave_w", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_slave_w1byte", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_slave_w1byte", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_slave_w1byte", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_slave_w1byte", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_slave_w1byte", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_slave_w1byte", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_slave_w1byte", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_slave_w1byte", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	//tHIGH - i2cTime_SCLpulse_w*****************************************************************************************************************************************
	dpintimeset( "SCL_USER", "i2cTime_SCLpulse_w", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SCLpulse_w", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SCLpulse_w", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SCLpulse_w", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SCLpulse_w", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SCLpulse_w", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SCLpulse_w", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SCLpulse_w", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_SCLpulse_w1byte", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SCLpulse_w1byte", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SCLpulse_w1byte", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SCLpulse_w1byte", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SCLpulse_w1byte", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SCLpulse_w1byte", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SCLpulse_w1byte", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SCLpulse_w1byte", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	//tSP - i2cTime_SCLtSP_w*****************************************************************************************************************************************
	dpintimeset( "SCL_USER", "i2cTime_SCLtSP_w", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SCLtSP_w", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SCLtSP_w", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SCLtSP_w", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SCLtSP_w", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SCLtSP_w", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SCLtSP_w", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SCLtSP_w", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_SCLtSP_w1byte", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SCLtSP_w1byte", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SCLtSP_w1byte", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SCLtSP_w1byte", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SCLtSP_w1byte", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SCLtSP_w1byte", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SCLtSP_w1byte", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SCLtSP_w1byte", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	//tSP H- SDA lo*****************************************************************************************************************************************
	dpintimeset( "SCL_USER", "i2cTime_SDAtSPlo_w", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSPlo_w", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSPlo_w", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSPlo_w", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SDAtSPlo_w", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSPlo_w", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSPlo_w", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSPlo_w", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_SDAtSPlo_w1byte", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSPlo_w1byte", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSPlo_w1byte", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSPlo_w1byte", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SDAtSPlo_w1byte", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSPlo_w1byte", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSPlo_w1byte", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSPlo_w1byte", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	//tSP L- SDA Hi*****************************************************************************************************************************************
	dpintimeset( "SCL_USER", "i2cTime_SDAtSPhi_w", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSPhi_w", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSPhi_w", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSPhi_w", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SDAtSPhi_w", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSPhi_w", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSPhi_w", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSPhi_w", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_SDAtSPhi_w1byte", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSPhi_w1byte", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSPhi_w1byte", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSPhi_w1byte", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SDAtSPhi_w1byte", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSPhi_w1byte", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSPhi_w1byte", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSPhi_w1byte", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	//tSU_DAT_RX_HIGH - SDA*****************************************************************************************************************************************
	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxHigh_w", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxHigh_w", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxHigh_w", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxHigh_w", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxHigh_w", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxHigh_w", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxHigh_w", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxHigh_w", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxHigh_w1byte", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxHigh_w1byte", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxHigh_w1byte", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxHigh_w1byte", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxHigh_w1byte", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxHigh_w1byte", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxHigh_w1byte", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxHigh_w1byte", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	//tSU_DAT_RX_LO - SDA*****************************************************************************************************************************************
	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxLo_w", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxLo_w", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxLo_w", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxLo_w", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxLo_w", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxLo_w", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxLo_w", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxLo_w", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxLo_w1byte", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxLo_w1byte", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxLo_w1byte", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxLo_w1byte", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxLo_w1byte", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxLo_w1byte", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxLo_w1byte", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxLo_w1byte", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	//tHD_DAT_RX_LO - SDA*****************************************************************************************************************************************
	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxLo_w", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxLo_w", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxLo_w", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxLo_w", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxLo_w", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxLo_w", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxLo_w", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxLo_w", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxLo_w1byte", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxLo_w1byte", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxLo_w1byte", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxLo_w1byte", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxLo_w1byte", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxLo_w1byte", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxLo_w1byte", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxLo_w1byte", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );


	//tHD_DAT_RX_HI - SDA*****************************************************************************************************************************************
	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxHigh_w", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxHigh_w", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxHigh_w", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxHigh_w", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxHigh_w", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxHigh_w", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxHigh_w", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxHigh_w", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxHigh_w1byte", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxHigh_w1byte", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxHigh_w1byte", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxHigh_w1byte", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxHigh_w1byte", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxHigh_w1byte", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxHigh_w1byte", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxHigh_w1byte", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	//tBUF - SDA*****************************************************************************************************************************************
	dpintimeset( "SCL_USER", "i2cTime_tBUF_10001b_w", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tBUF_10001b_w", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tBUF_10001b_w", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tBUF_10001b_w", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_tBUF_10001b_w", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tBUF_10001b_w", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tBUF_10001b_w", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tBUF_10001b_w", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_tBUF_10101b_w", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tBUF_10101b_w", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tBUF_10101b_w", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tBUF_10101b_w", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_tBUF_10101b_w", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tBUF_10101b_w", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tBUF_10101b_w", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tBUF_10101b_w", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_tBUF_w2byte", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tBUF_w2byte", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tBUF_w2byte", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tBUF_w2byte", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_tBUF_w2byte", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tBUF_w2byte", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tBUF_w2byte", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tBUF_w2byte", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );


	//tHD_STA  *****************************************************************************************************************************************
	dpintimeset( "SCL_USER", "i2cTime_tHD_STA_w", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tHD_STA_w", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tHD_STA_w", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tHD_STA_w", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_tHD_STA_w", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tHD_STA_w", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tHD_STA_w", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tHD_STA_w", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_tHD_STA_w1byte", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tHD_STA_w1byte", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tHD_STA_w1byte", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tHD_STA_w1byte", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_tHD_STA_w1byte", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tHD_STA_w1byte", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tHD_STA_w1byte", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tHD_STA_w1byte", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	//tF_SDA  *****************************************************************************************************************************************
	dpintimeset( "SCL_USER", "i2cTime_tF_slave_w", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_slave_w", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_slave_w", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_slave_w", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_tF_slave_w", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_slave_w", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_slave_w", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_slave_w", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_tF_30_slave_r", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_30_slave_r", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_30_slave_r", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_30_slave_r", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_tF_30_slave_r", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_30_slave_r", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_30_slave_r", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_30_slave_r", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_tF_30_read4byte", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_30_read4byte", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_30_read4byte", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_30_read4byte", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_tF_30_read4byte", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_30_read4byte", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_30_read4byte", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_30_read4byte", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_tF_70_slave_r", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_70_slave_r", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_70_slave_r", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_70_slave_r", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_tF_70_slave_r", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_70_slave_r", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_70_slave_r", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_70_slave_r", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_tF_70_read4byte", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_70_read4byte", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_70_read4byte", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_70_read4byte", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_tF_70_read4byte", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_70_read4byte", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_70_read4byte", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_70_read4byte", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );




	lwait(2000 us_f);
	//digital pattern run
	siStatus = mcurun( NULL, "i2cTime_slave_w1byte");
	mcustop();

	_int64 i64FailPins;

	for(int isite_no=0; isite_no < NUM_SITES; isite_no++)
	{
		if(msSiteStat(isite_no))
		{
			siStatus[isite_no] = dpingetfailpins("SDA_USER", DPIN_ALL_PINS, &i64FailPins, isite_no );
		}
	}	

	pldStatus = siStatus;

	return pldStatus;
}

//I2C Meas Function
PinListData Meas_i2cTime()
{
	PinListData pldStatus;
	SiteInt siStatus;

	pldStatus = PinListData(0);
	siStatus = SiteInt(1);

	_int64 i64FailPins;

	for(int isite_no=0; isite_no < NUM_SITES; isite_no++)
	{
		if(msSiteStat(isite_no))
		{
			siStatus[isite_no] = dpingetfailpins("SDA_USER", DPIN_ALL_PINS, &i64FailPins, isite_no );
		}
	}	

	pldStatus = siStatus;

	return pldStatus;
}

//i2cTime_slave_w
void Set_i2cTime_slave_w(double stop)
{
	SiteInt  siStatus;
	siStatus = SiteInt(1);

	//setup
	//double dPeriodns = 2.5e3;	// in ns
	double dSCLFallEdge = 0.0;
	double dSCLRiseEdge = 0.67;
	double dSDADriveEdge = 0.33;
	double dSDACompareEdge = 0.83;

	//Custom timeset for "ts_i2cUser_scl_send, ts_i2cUser_sda_send" setup
	double dPeriodns;
	double dSCLRiseEdgeUser = 0.5;  //50%
	double dSDADriveEdgeUser = 0.1; //10%

	dPeriodns = (1000)*(stop/50); //period in ns, starting from 125 or 400KHz
	dpinperiod((int)stop); //125 = 400KHz for I2C timing

	dpintimeset( "SCL_USER", "i2cTime_slave_w", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_slave_w", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_slave_w", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdgeUser*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_slave_w", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_slave_w", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_slave_w", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_slave_w", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdgeUser*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_slave_w", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_slave_w1byte", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_slave_w1byte", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_slave_w1byte", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdgeUser*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_slave_w1byte", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_slave_w1byte", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_slave_w1byte", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_slave_w1byte", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdgeUser*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_slave_w1byte", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	lwait(2000 us_f);
	siStatus = mcurun( NULL, "i2cTime_slave_w1byte");
	mcustop();
}


//i2cTime_SCLpulse_w
void Set_i2cTime_SCLpulse_w(double stop)
{
	SiteInt  siStatus;
	siStatus = SiteInt(1);

	//setup
	double dPeriodns = 2.5e3;	// in ns
	double dSCLFallEdge = 0.0;
	double dSCLRiseEdge = 0.67;
	double dSDADriveEdge = 0.0;
	double dSDACompareEdge = 0.83;

	//Custom timeset for "ts_i2cUser_scl_send, ts_i2cUser_sda_send" setup
	//double dPeriodns;
	//double dSCLRiseEdgeUser = 0.5;  //50%
	//double dSDADriveEdgeUser = 0.1; //10%
	double dSCLRiseEdgeUser;

	dSCLRiseEdgeUser = stop; // starting from 0.5

	dpinperiod(125); //125 = 400KHz for I2C timing

	dpintimeset( "SCL_USER", "i2cTime_SCLpulse_w", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SCLpulse_w", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SCLpulse_w", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdgeUser*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SCLpulse_w", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SCLpulse_w", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SCLpulse_w", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SCLpulse_w", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SCLpulse_w", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_SCLpulse_w1byte", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SCLpulse_w1byte", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SCLpulse_w1byte", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdgeUser*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SCLpulse_w1byte", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SCLpulse_w1byte", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SCLpulse_w1byte", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SCLpulse_w1byte", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SCLpulse_w1byte", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	lwait(2000 us_f);
	siStatus = mcurun( NULL, "i2cTime_SCLpulse_w1byte");
	mcustop();
}



//i2cTime_SCLtSP_w
void Set_i2cTime_SCLtSP_w(double stop)
{
	SiteInt  siStatus;
	siStatus = SiteInt(1);

	//setup
	double dPeriodns = 2.5e3;	// in ns
	double dSCLFallEdge = 0.0;
	double dSCLRiseEdge = 0.67;
	double dSDADriveEdge = 0.33;
	double dSDACompareEdge = 0.83;

	//Custom timeset for "ts_i2cUser_scl_send, ts_i2cUser_sda_send" setup
	//double dPeriodns;
	//double dSCLRiseEdgeUser = 0.5;  //50%
	//double dSDADriveEdgeUser = 0.1; //10%
	double dSCLRiseEdgeUser;

	dSCLRiseEdgeUser = stop; // starting from 0.5

	dpinperiod(125); //125 = 400KHz for I2C timing

	dpintimeset( "SCL_USER", "i2cTime_SCLtSP_w", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SCLtSP_w", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SCLtSP_w", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdgeUser*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SCLtSP_w", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SCLtSP_w", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SCLtSP_w", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SCLtSP_w", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SCLtSP_w", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_SCLtSP_w1byte", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SCLtSP_w1byte", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SCLtSP_w1byte", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdgeUser*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SCLtSP_w1byte", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SCLtSP_w1byte", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SCLtSP_w1byte", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SCLtSP_w1byte", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SCLtSP_w1byte", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	lwait(2000 us_f);
	siStatus = mcurun( NULL, "i2cTime_SCLtSP_w1byte");
	mcustop();
}

//i2cTime_SDAtSPlo_w
void Set_i2cTime_SDAtSPlo_w(double stop)
{
	SiteInt  siStatus;
	siStatus = SiteInt(1);

	//setup
	double dPeriodns = 2.5e3;	// in ns
	double dSCLFallEdge = 0.0;
	double dSCLRiseEdge = 0.67;  //old std=0.67
	double dSDADriveEdge = 0.01; //old std=0.33
	double dSDACompareEdge = 0.83;

	//Custom timeset for "ts_i2cUser_scl_send, ts_i2cUser_sda_send" setup
	//double dPeriodns;
	//double dSCLRiseEdgeUser = 0.5;  //50%
	//double dSDADriveEdgeUser = 0.1; //10%
	double dSDADriveEdgeUser;

	dSDADriveEdgeUser = stop; // starting from 0.1

	dpinperiod(125); //125 = 400KHz for I2C timing

	dpintimeset( "SCL_USER", "i2cTime_SDAtSPlo_w", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSPlo_w", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSPlo_w", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSPlo_w", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SDAtSPlo_w", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSPlo_w", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSPlo_w", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdgeUser*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSPlo_w", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_SDAtSPlo_w1byte", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSPlo_w1byte", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSPlo_w1byte", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSPlo_w1byte", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SDAtSPlo_w1byte", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSPlo_w1byte", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSPlo_w1byte", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdgeUser*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSPlo_w1byte", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	lwait(2000 us_f);
	siStatus = mcurun( NULL, "i2cTime_SDAtSPlo_w1byte");
	mcustop();
}


//i2cTime_SDAtSPhi_w
void Set_i2cTime_SDAtSPhi_w(double stop)
{
	SiteInt  siStatus;
	siStatus = SiteInt(1);

	//setup
	double dPeriodns = 2.5e3;	// in ns
	double dSCLFallEdge = 0.0;
	double dSCLRiseEdge = 0.67;  //old std=0.67
	double dSDADriveEdge = 0.01; //old std=0.33
	double dSDACompareEdge = 0.83;

	//Custom timeset for "ts_i2cUser_scl_send, ts_i2cUser_sda_send" setup
	//double dPeriodns;
	//double dSCLRiseEdgeUser = 0.5;  //50%
	//double dSDADriveEdgeUser = 0.1; //10%
	double dSDADriveEdgeUser;

	dSDADriveEdgeUser = stop; // starting from 0.1

	dpinperiod(125); //125 = 400KHz for I2C timing

	dpintimeset( "SCL_USER", "i2cTime_SDAtSPhi_w", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSPhi_w", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSPhi_w", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSPhi_w", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SDAtSPhi_w", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSPhi_w", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSPhi_w", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdgeUser*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSPhi_w", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_SDAtSPhi_w1byte", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSPhi_w1byte", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSPhi_w1byte", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSPhi_w1byte", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SDAtSPhi_w1byte", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSPhi_w1byte", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSPhi_w1byte", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdgeUser*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSPhi_w1byte", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	lwait(2000 us_f);
	siStatus = mcurun( NULL, "i2cTime_SDAtSPhi_w1byte");
	mcustop();
}


//i2cTime_SDAtSUdatRxHigh_w
void Set_i2cTime_SDAtSUdatRxHigh_w(double stop)
{
	SiteInt  siStatus;
	siStatus = SiteInt(1);

	//setup
	double dPeriodns = 2.5e3;	// in ns
	double dSCLFallEdge = 0.0;
	double dSCLRiseEdge = 0.5;  //old std=0.67
	double dSDADriveEdge = 0.33;
	double dSDACompareEdge = 0.83;

	//Custom timeset for "ts_i2cUser_scl_send, ts_i2cUser_sda_send" setup
	//double dPeriodns;
	//double dSCLRiseEdgeUser = 0.5;  //50%
	//double dSDADriveEdgeUser = 0.1; //10%
	double dSDADriveEdgeUser;

	dSDADriveEdgeUser = stop; // starting from 0.1

	dpinperiod(125); //125 = 400KHz for I2C timing

	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxHigh_w", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxHigh_w", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxHigh_w", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxHigh_w", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxHigh_w", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxHigh_w", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxHigh_w", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdgeUser*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxHigh_w", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxHigh_w1byte", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxHigh_w1byte", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxHigh_w1byte", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxHigh_w1byte", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxHigh_w1byte", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxHigh_w1byte", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxHigh_w1byte", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdgeUser*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxHigh_w1byte", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	lwait(2000 us_f);
	siStatus = mcurun( NULL, "i2cTime_SDAtSUdatRxHigh_w1byte");
	mcustop();
}


//i2cTime_SDAtSUdatRxLo_w
void Set_i2cTime_SDAtSUdatRxLo_w(double stop)
{
	SiteInt  siStatus;
	siStatus = SiteInt(1);

	//setup
	double dPeriodns = 2.5e3;	// in ns
	double dSCLFallEdge = 0.0;
	double dSCLRiseEdge = 0.5;  //old std=0.67
	double dSDADriveEdge = 0.33;
	double dSDACompareEdge = 0.83;

	//Custom timeset for "ts_i2cUser_scl_send, ts_i2cUser_sda_send" setup
	//double dPeriodns;
	//double dSCLRiseEdgeUser = 0.5;  //50%
	//double dSDADriveEdgeUser = 0.1; //10%
	double dSDADriveEdgeUser;

	dSDADriveEdgeUser = stop; // starting from 0.1

	dpinperiod(125); //125 = 400KHz for I2C timing

	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxLo_w", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxLo_w", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxLo_w", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxLo_w", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_NR, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxLo_w", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxLo_w", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxLo_w", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdgeUser*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxLo_w", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxLo_w1byte", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxLo_w1byte", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxLo_w1byte", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtSUdatRxLo_w1byte", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxLo_w1byte", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxLo_w1byte", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxLo_w1byte", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdgeUser*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtSUdatRxLo_w1byte", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	lwait(2000 us_f);
	siStatus = mcurun( NULL, "i2cTime_SDAtSUdatRxLo_w1byte");
	mcustop();
}


//i2cTime_SDAtHDdatRxLo_w
void Set_i2cTime_SDAtHDdatRxLo_w(double stop)
{
	SiteInt  siStatus;
	siStatus = SiteInt(1);

	//setup
	double dPeriodns = 2.5e3;	// in ns
	double dSCLFallEdge = 0.0;
	double dSCLRiseEdge = 0.5;  //old std=0.67
	double dSDADriveEdge = 0.33;
	double dSDACompareEdge = 0.83;

	//Custom timeset for "ts_i2cUser_scl_send, ts_i2cUser_sda_send" setup
	//double dPeriodns;
	//double dSCLRiseEdgeUser = 0.5;  //50%
	//double dSDADriveEdgeUser = 0.1; //10%
	double dSDADriveEdgeUser;

	dSDADriveEdgeUser = stop; // starting from 0.1

	dpinperiod(125); //125 = 400KHz for I2C timing

	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxLo_w", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxLo_w", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxLo_w", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxLo_w", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxLo_w", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxLo_w", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxLo_w", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdgeUser*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxLo_w", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxLo_w1byte", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxLo_w1byte", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxLo_w1byte", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxLo_w1byte", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxLo_w1byte", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxLo_w1byte", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxLo_w1byte", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdgeUser*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxLo_w1byte", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	lwait(2000 us_f);
	siStatus = mcurun( NULL, "i2cTime_SDAtHDdatRxLo_w1byte");
	mcustop();
}


//i2cTime_SDAtHDdatRxHigh_w
void Set_i2cTime_SDAtHDdatRxHigh_w(double stop)
{
	SiteInt  siStatus;
	siStatus = SiteInt(1);

	//setup
	double dPeriodns = 2.5e3;	// in ns
	double dSCLFallEdge = 0.0;
	double dSCLRiseEdge = 0.5;  //old std=0.67
	double dSDADriveEdge = 0.33;
	double dSDACompareEdge = 0.83;

	//Custom timeset for "ts_i2cUser_scl_send, ts_i2cUser_sda_send" setup
	//double dPeriodns;
	//double dSCLRiseEdgeUser = 0.5;  //50%
	//double dSDADriveEdgeUser = 0.1; //10%
	double dSDADriveEdgeUser;

	dSDADriveEdgeUser = stop; // starting from 0.1

	dpinperiod(125); //125 = 400KHz for I2C timing

	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxHigh_w", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxHigh_w", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxHigh_w", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxHigh_w", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxHigh_w", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxHigh_w", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxHigh_w", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdgeUser*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxHigh_w", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxHigh_w1byte", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxHigh_w1byte", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxHigh_w1byte", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_SDAtHDdatRxHigh_w1byte", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxHigh_w1byte", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxHigh_w1byte", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxHigh_w1byte", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdgeUser*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_SDAtHDdatRxHigh_w1byte", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	lwait(2000 us_f);
	siStatus = mcurun( NULL, "i2cTime_SDAtHDdatRxHigh_w1byte");
	mcustop();
}

//i2cTime_SDAtHDdatRxHigh_w
void Set_i2cTime_tBUF_w(double stop)
{
	SiteInt  siStatus;
	siStatus = SiteInt(1);

	//setup
	double dPeriodns = 2.5e3;	// in ns
	double dSCLFallEdge = 0.3;
	double dSCLRiseEdge = 0.99;  //old std=0.67
	double dSDADriveEdge = 0.01; //old std=0.33
	double dSDACompareEdge = 0.83;

	//Custom timeset for "ts_i2cUser_scl_send, ts_i2cUser_sda_send" setup
	//double dPeriodns;
	//double dSCLRiseEdgeUser = 0.5;  //50%
	//double dSDADriveEdgeUser = 0.1; //10%
	double dSDADriveEdgeUser;

	dSDADriveEdgeUser = stop; // starting from 0.9

	dpinperiod(125); //125 = 400KHz for I2C timing

	dpintimeset( "SCL_USER", "i2cTime_tBUF_10001b_w", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tBUF_10001b_w", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tBUF_10001b_w", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tBUF_10001b_w", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_tBUF_10001b_w", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tBUF_10001b_w", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tBUF_10001b_w", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tBUF_10001b_w", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_tBUF_10101b_w", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tBUF_10101b_w", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tBUF_10101b_w", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tBUF_10101b_w", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_tBUF_10101b_w", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tBUF_10101b_w", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tBUF_10101b_w", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tBUF_10101b_w", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_tBUF_w2byte", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tBUF_w2byte", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tBUF_w2byte", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tBUF_w2byte", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_tBUF_w2byte", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tBUF_w2byte", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tBUF_w2byte", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdgeUser*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tBUF_w2byte", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	lwait(2000 us_f);
	siStatus = mcurun( NULL, "i2cTime_tBUF_w2byte");
	mcustop();
}


//i2cTime_tHD_STA_w
void Set_i2cTime_tHD_STA_w(double stop)
{
	SiteInt  siStatus;
	siStatus = SiteInt(1);

	//setup
	double dPeriodns = 2.5e3;	// in ns
	double dSCLFallEdge = 0.0;
	double dSCLRiseEdge = 0.5;  //old std=0.67
	double dSDADriveEdge = 0.33;
	double dSDACompareEdge = 0.83;

	//Custom timeset for "ts_i2cUser_scl_send, ts_i2cUser_sda_send" setup
	//double dPeriodns;
	//double dSCLRiseEdgeUser = 0.5;  //50%
	//double dSDADriveEdgeUser = 0.1; //10%
	double dSDADriveEdgeUser;

	dSDADriveEdgeUser = stop; // starting from 0.1

	dpinperiod(125); //125 = 400KHz for I2C timing

	dpintimeset( "SCL_USER", "i2cTime_tHD_STA_w", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tHD_STA_w", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tHD_STA_w", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tHD_STA_w", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_tHD_STA_w", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tHD_STA_w", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tHD_STA_w", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdgeUser*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tHD_STA_w", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_tHD_STA_w1byte", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tHD_STA_w1byte", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tHD_STA_w1byte", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tHD_STA_w1byte", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_tHD_STA_w1byte", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tHD_STA_w1byte", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tHD_STA_w1byte", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdgeUser*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tHD_STA_w1byte", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	lwait(2000 us_f);
	siStatus = mcurun( NULL, "i2cTime_tHD_STA_w1byte");
	mcustop();
}


//i2cTime_tF_30_r
void Set_i2cTime_tF_30_r(double stop)
{
	SiteInt  siStatus;
	siStatus = SiteInt(1);

	//setup
	double dPeriodns = 2.5e3;	// in ns
	double dSCLFallEdge = 0.0;
	double dSCLRiseEdge = 0.5;  //old std=0.67
	double dSDADriveEdge = 0.33;
	double dSDACompareEdge = 0.83;

	//Custom timeset for "ts_i2cUser_scl_send, ts_i2cUser_sda_send" setup
	//double dPeriodns;
	//double dSCLRiseEdgeUser = 0.5;  //50%
	//double dSDADriveEdgeUser = 0.1; //10%
	double dSDACompareEdgeUser;

	dSDACompareEdgeUser = stop; // starting from 0.1

	dpinperiod(125); //125 = 400KHz for I2C timing

	dpintimeset( "SCL_USER", "i2cTime_tF_slave_w", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_slave_w", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_slave_w", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_slave_w", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_tF_slave_w", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_slave_w", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_slave_w", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_slave_w", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdgeUser*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_tF_30_slave_r", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_30_slave_r", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_30_slave_r", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_30_slave_r", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_tF_30_slave_r", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_30_slave_r", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_30_slave_r", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_30_slave_r", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdgeUser*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_tF_30_read4byte", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_30_read4byte", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_30_read4byte", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_30_read4byte", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_tF_30_read4byte", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_30_read4byte", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_30_read4byte", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_30_read4byte", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdgeUser*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	lwait(2000 us_f);
	siStatus = mcurun( NULL, "i2cTime_tF_30_read4byte");
	mcustop();
}


//i2cTime_tF_70_r
void Set_i2cTime_tF_70_r(double stop)
{
	SiteInt  siStatus;
	siStatus = SiteInt(1);

	//setup
	double dPeriodns = 2.5e3;	// in ns
	double dSCLFallEdge = 0.0;
	double dSCLRiseEdge = 0.5;  //old std=0.67
	double dSDADriveEdge = 0.33;
	double dSDACompareEdge = 0.83;

	//Custom timeset for "ts_i2cUser_scl_send, ts_i2cUser_sda_send" setup
	//double dPeriodns;
	//double dSCLRiseEdgeUser = 0.5;  //50%
	//double dSDADriveEdgeUser = 0.1; //10%
	double dSDACompareEdgeUser;

	dSDACompareEdgeUser = stop; // starting from 0.1

	dpinperiod(125); //125 = 400KHz for I2C timing

	dpintimeset( "SCL_USER", "i2cTime_tF_slave_w", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_slave_w", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_slave_w", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_slave_w", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_tF_slave_w", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_slave_w", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_slave_w", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_slave_w", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdgeUser*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_tF_70_slave_r", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_70_slave_r", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_70_slave_r", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_70_slave_r", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_tF_70_slave_r", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_70_slave_r", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_70_slave_r", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_70_slave_r", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdgeUser*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SCL_USER", "i2cTime_tF_70_read4byte", "ts_i2cUser_scl_start", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_70_read4byte", "ts_i2cUser_scl_stop", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_70_read4byte", "ts_i2cUser_scl_send", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SCL_USER", "i2cTime_tF_70_read4byte", "ts_i2cUser_scl_receive", DPIN_TS_NORMAL, DPIN_RO, dSCLFallEdge*dPeriodns, dSCLRiseEdge*dPeriodns, DPIN_IGNORE_TIMING, DPIN_IGNORE_TIMING, MS_ALL );

	dpintimeset( "SDA_USER", "i2cTime_tF_70_read4byte", "ts_i2cUser_sda_start", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_70_read4byte", "ts_i2cUser_sda_stop", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_70_read4byte", "ts_i2cUser_sda_send", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdge*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	dpintimeset( "SDA_USER", "i2cTime_tF_70_read4byte", "ts_i2cUser_sda_receive", DPIN_TS_NORMAL, DPIN_NR, dSDADriveEdge*dPeriodns, DPIN_IGNORE_TIMING, dSDACompareEdgeUser*dPeriodns, DPIN_IGNORE_TIMING, MS_ALL );
	lwait(2000 us_f);
	siStatus = mcurun( NULL, "i2cTime_tF_70_read4byte");
	mcustop();
}