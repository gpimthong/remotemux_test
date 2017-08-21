/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:
*		OTP_Burn.cpp
*
*	Written and maintained by:
*		Govit Pimthong (govit.pimthong@MaximIntegrated.com)
*
*	URL:                   $URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/OTP_Burn.cpp $
*	Last Modified by:      $LastChangedBy: kiratiya.kaewpaluek $
*	Date:                  $Date: 2017-07-07 11:07:18 +0700 (Fri, 07 Jul 2017) $
*	Current Rev:           $Revision: 66609 $
*
*	Description:
*		File Description
*
*	Revision History
*
*
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
#include "AP89_FT.h"


const double dBurnVoltage = 6.7;
const double dBurnTime = 15; //ms

SiteInt siBurnCode;

template <typename T> void SiteDatalog(int &DSIndex, SiteBool sbSwitch, T stResult){

	for( int isite=0; isite<NUM_SITES; isite++)
		if (!sbSwitch[isite])
			Maxim::Datalog(isite, DSIndex, stResult[isite]);

	DSIndex++;
}


/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
* SetBurnVoltage
*	Purpose: Set BIAS voltage at 6.7V to burn OTP, Only for untrimmed site.
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
void SetBurnVoltage(){

	ACCUMULATOR_STR Burn_v[NUM_SITES];

	siBurnCode = SiteInt();

	for(int isite = 0; isite<NUM_SITES; isite++){

		if (!DUT::Trim::gsbLockBit[isite] && msSiteStat(isite) && (msBinValue(isite) == 1)){ //Untrimmed && Site is Active && Still BIN 1
			Burn_v[isite].value1 = dBurnVoltage;
			siBurnCode[isite] = 0x01;
		}
		else{
			Burn_v[isite].value1 = 5.0; 
			siBurnCode[isite] = 0x00;
		}

		if (!DUT::Trim::gsbSPLockBit[isite])
			siBurnCode[isite] += 0x02;

	}


	//initiate burn comman
	Maxim::I2C::Write(OTP_3DV_AD, 0xF1, siBurnCode);	

	groupsetaccumulator(Burn_v, NUM_SITES);
	apu12set( VCC, APU12_FV, USE_ACCUM, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	mark(0);

}

// Test Function: OTP_Burn
ETS_PRGFLOW_FUNC OTP_Burn( int DSIndex, LPCTSTR TestLabel )
{


	PinListData pldResults;	//declare a PLD variable for the measurement results.
	PinListData pldOTP_I_10, pldOTP_I_90, pldOTP_I_Delta, pldOTP_I_perBit, pldBurn_v;



	//The following condition will make the program skip the burn sequence
	// 1. Test Step that IS NOT 'FT_ROOM' or 'CHAR'
	// 2. All devices in socket are trimmed.
	// 3. Run the program in engineering mode with 'GPSW_BURN' switch OFF.
	if((GetUserSwitch(GPSW_BURN) || Maxim::Operator()) && DUT::Trim::gsbLockBit.Any(false) && (Maxim::bTest_Step_Is("FT_ROOM") || Maxim::bTest_Step_Is("CHAR"))){




		//VCC-F, COMP-S
		//apu12setcapload( VCC, APU12_FV, 0, APU12_10V, APU12_100MA, 0.5, APU12_PIN_TO_VI );
		//lwait (1 ms_f);
		//apu12set( VCC, APU12_FV, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );

		//cbitclose(K9);
		//lwait (3 ms_f);

		if(bREF_Check){
			Maxim::I2C::Read(OTP_REF_REG, gsiReadBack);
			for(int isite=0;isite<NUM_SITES;isite++){			
				if(msSiteStat(isite) && (gsiReadBack[isite] != g_REF_code[isite])){
					gsiReadBack = gsiReadBack;
				}
			}
		}


		apu12set( VCC, APU12_FV, 5.1, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
		apu12set( VCC, APU12_FV, 5.1, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );

		lwait (1 ms_f);

		//Maxim::I2C::SetSlaveAddress(TM_3DV_AD);

		//Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
		//Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>=0000
		//Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>=0100
		//Maxim::I2C::Write(0x01, 0x20); //write data to DHR1<7:4>=0000
		//Maxim::I2C::Write(0x01, 0x12); //write data to DHR0<3:0>=0100
		//Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>
		//Maxim::I2C::Write(0x01, 0x71); //AI<0>=V_VCC_K)

		//Maxim::I2C::Write(0x02, 0xB0); //Reset BIPTM Machine






		///---------------------------------------------------------------
		//OTP data check before burn
		///---------------------------------------------------------------

		
		Maxim::I2C::SetSlaveAddress(OTP_3DV_AD);

		//BG
		if(g_DIM_code)
			Maxim::I2C::Write(OTP_BG_REG, g_BG_code+64);
		else
			Maxim::I2C::Write(OTP_BG_REG, g_BG_code);


		//REF_Reg
		Maxim::I2C::Write(OTP_REF_REG, g_REF_code);



		//LDO_Reg
		if(!AP89)
			Maxim::I2C::Write(OTP_LDO_REG, g_LDO_code);

		
		//IBIAS_Reg
		Maxim::I2C::Write(OTP_BIAS_REG, g_BIAS_code);


		//OSC20M_Reg
		Maxim::I2C::Write(OTP_OSC20M_REG, g_OSC20M_code);


		//OUT_Gain_Reg
		Maxim::I2C::Write(OTP_OUT_GAIN_REG, g_OUT_GAIN_code);
		
		//OUT1
		DUT::Trim::OUTx_Send_Data(OTP_3DV_AD, 1, g_OUT1_code);


		//OUT2_Reg
		DUT::Trim::OUTx_Send_Data(OTP_3DV_AD, 2, g_OUT2_code);


		//OUT3_Reg
		DUT::Trim::OUTx_Send_Data(OTP_3DV_AD, 3, g_OUT3_code);


		//OUT4_Reg
		DUT::Trim::OUTx_Send_Data(OTP_3DV_AD, 4, g_OUT4_code);


		if(AP89){

			//OUT5_Reg
			Maxim::I2C::Write(OTP_OUT5_REG, g_OUT5_code);

			//OUT6_Reg
			DUT::Trim::OUTx_Send_Data(OTP_3DV_AD, 6, g_OUT6_code);

		}





		///---------------------------------------------------------------








		SetBurnVoltage();

		lwait(unsigned long( 0.1 * dBurnTime) ms_f );
		pldOTP_I_10 = apu12mi(VCC, APU12_MI_1X, 1);

		pldBurn_v = apu12mv(VCC, 1);

		lwait(unsigned long( 0.8 * dBurnTime) ms_f );
		pldOTP_I_90 = apu12mi(VCC, APU12_MI_1X, 1);


		lwait(unsigned long( 0.1 * dBurnTime) ms_f );
		apu12set( VCC, APU12_FV, 5.0, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );

		//Record burn time
		double otp_time = ltime(0)/1000;


		pldOTP_I_perBit = pldOTP_I_Delta = pldOTP_I_90 - pldOTP_I_10;

		ForEachActive_iSite{

			if(DUT::Trim::gsbSPLockBit[iSite])
				pldOTP_I_perBit[iSite] = (pldOTP_I_90[iSite] + pldOTP_I_10[iSite])/(2 * 56); //56 OTP bits
			else
				pldOTP_I_perBit[iSite] = (pldOTP_I_90[iSite] + pldOTP_I_10[iSite])/(2 * 80); //+24 Scratch Pad bits

		}


		SiteDatalog(DSIndex, DUT::Trim::gsbLockBit, pldOTP_I_10);
		SiteDatalog(DSIndex, DUT::Trim::gsbLockBit, pldOTP_I_90);
		SiteDatalog(DSIndex, DUT::Trim::gsbLockBit, pldOTP_I_Delta);
		SiteDatalog(DSIndex, DUT::Trim::gsbLockBit, pldOTP_I_perBit);
		SiteDatalog(DSIndex, DUT::Trim::gsbLockBit, pldBurn_v);

		pldResults = otp_time;
		SiteDatalog(DSIndex, DUT::Trim::gsbLockBit, pldResults); // Burn time



		//VCC-F, COMP-S
		apu12set( VCC, APU12_OFF, 5, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
		lwait (1 ms_f);

		//cbitopen(K9);
		//lwait (3 ms_f);

		//apu12set( VCC_COMP, APU12_FV, 0.7, APU12_10V, APU12_100MA, APU12_PIN_TO_VI ); //OUT1




		//Complete Power down after burn.
		apu12setcapload( EN, APU12_FV, 0, APU12_10V, APU12_1MA, 0.5, APU12_PIN_TO_VI );
		apu12setcapload( BSTMON, APU12_FV, 0, APU12_30V, APU12_1MA, 0.5, APU12_PIN_TO_VI );
		apu12setcapload( ALL_OUT, APU12_FV, 0, APU12_10V, APU12_100MA, 0.5, APU12_PIN_TO_VI );

		dpinforce("DIM", "0", 3.0, 0.0, MS_ALL); //DIM at 0V
		dpinforce("SCL_IO", "0", 3.0, 0.0, MS_ALL);
		dpinforce("SDA_IO", "0", 3.0, 0.0, MS_ALL);

		lwait (1 ms_f);

		apu12setcapload( IN1, APU12_FV, 0, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );
		lwait (2 ms_f);

		gbSkipPowerDown = true;

	}//Skip OTP Burn







	//End of function
	Maxim::DatalogFlush();	//Output the datalog queue.
	return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC
