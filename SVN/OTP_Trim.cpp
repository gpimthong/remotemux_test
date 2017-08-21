/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:
*		OTP_Trim.cpp
*
*	Written and maintained by:
*		Govit Pimthong (govit.pimthong@MaximIntegrated.com)
*
*	URL:                   $URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/OTP_Trim.cpp $
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


template <typename T> void SiteDatalog(int &DSIndex, SiteBool sbSwitch, T stResult){

	for( int isite=0; isite<NUM_SITES; isite++)
		if (!sbSwitch[isite])
			Maxim::Datalog(isite, DSIndex, stResult[isite]);

	DSIndex++;
}


// Test Function: OTP_Trim
ETS_PRGFLOW_FUNC OTP_Trim( int DSIndex, LPCTSTR TestLabel )
{


	gsiReadBack = SiteInt();
	DUT::Trim::gsbLockBit = SiteBool(1);
	DUT::Trim::gsbSPLockBit = SiteBool(1);


	PinListData pldResult;

	int iReg = 0;

	//Design experiment feature, 0 for now
	OTP_BSTOSC_Bit = 0;



	bool bBitWeightsChar;

	if (GetUserSwitch(GPSW_BitWeightChar))
		bBitWeightsChar = true;
	else
		bBitWeightsChar = false;




	////BSTMON UV Blanking time -------------------------------------------------------------
	//RESULTS_STR FLT_results[NUM_SITES];
	//RESULTS_STR PGATE_results[NUM_SITES];
	//PinListData pldPGATE_pt, pldFLT_pt;

	//apu12setcapload( OUT6, APU12_FV, 0.0, APU12_10V, APU12_200MA, 0.5, APU12_PIN_TO_VI );
	//apu12setcapload( BSTMON, APU12_FV, 0.1, APU12_30V, APU12_1MA, 0.5, APU12_PIN_TO_VI );
	//apu12set( VCC, APU12_FV, 2.0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );

	////Pull up PGATE and FLT
	//cbitclose(K6, K14);
	//apu12set( EN, APU12_FVAWG, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	//apu12set( OUT6, APU12_FV, 2.0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );

	//apu12adcmode( VCC, APU12_MV, 1 );
	//apu12adcmode( OUT6, APU12_MV, 1 );
	//apu12setwhereat( OUT6, 1, 150, 0.5, APU12_BELOW_THRESH , -1, -1, 0.0, APU12_ABOVE_THRESH );
	//apu12setwhereat( VCC, 1, 150, 0.5, APU12_BELOW_THRESH , -1, -1, 0.0, APU12_ABOVE_THRESH );
	//mcusequence( MCU_CH0, "BSTMON_UV_BL", "%d ON", 150);
	//mcuconnect( MCU_CH0, "Group%d, Group%d, Group%d", (int)EN, (int)VCC, (int)OUT6 );
	//mcumode(MCU_CH0,MCU_CLK,25000,MCU_CLKA,MCU_TRIGNORMAL,0); //500us vector rate

	//lwait(2 ms_f);



	//mcurun( "BSTMON_UV_BL", NULL );




	////---- Disconnect the MCU
	//mcuconnect( MCU_CH0, "OFF" );



	////--- Find the whereat location and datalog the results
	//apu12getwhereat( VCC, APU12_THRESHOLD1 );
	//apu12setwhereat( VCC, -1, -1, 0.0, APU12_BELOW_THRESH, -1, -1, 0.0, APU12_BELOW_THRESH );  //--- Turn off the whereat
	//groupgetresults( FLT_results, NUM_SITES );

	//apu12getwhereat( OUT6, APU12_THRESHOLD1 );
	//apu12setwhereat( OUT6, -1, -1, 0.0, APU12_BELOW_THRESH, -1, -1, 0.0, APU12_BELOW_THRESH );  //--- Turn off the whereat
	//groupgetresults( PGATE_results, NUM_SITES );



	////Extrack trasition point
	//for (int site = 0; site<NUM_SITES; site++)
	//{
	//	if (msSiteStat(site))
	//	{
	//		pldFLT_pt[site] = FLT_results[site].value;
	//		pldPGATE_pt[site] = PGATE_results[site].value;

	//		pldResult[site] = 0.5 * (FLT_results[site].value - PGATE_results[site].value + 1);
	//	}
	//}

	//gpldBSTMON_UV_BL = pldResult;

	//cbitopen(K6, K14);
	//lwait (3 ms_f);
	//apu12set( BSTMON, APU12_FV, 0.7, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
	//apu12set( OUT6, APU12_FV, 0.7, APU12_10V, APU12_200MA, APU12_PIN_TO_VI );
	////--------------------------------------------------------------------------------






	apu12set( EN, APU12_FV, 5, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	Maxim::I2C::SetSlaveAddress(AP89_AD);

	//Write '1' to ENA bit REG02<5>
	Maxim::I2C::Write(0x02, 0x20);


	DUT::Initiate_TestMode();


	Maxim::I2C::SetSlaveAddress(OTP_3DV_AD);

	Maxim::I2C::Write(UNLOCK1_REG, 0x17); //Unlock passwd0 = 0x17
	Maxim::I2C::Write(UNLOCK2_REG, 0x65); //Unlock passwd1 = 0x65

	Maxim::I2C::Write(OTPMODE_REG, 0x03);


	//Release CS
	apu12set( CS, APU12_OFF, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );


	/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
	* Test : OTP_Lockbit_Pre
	* Test No : #13000
	* Description : Read back lockbit from Reg F4h<0>
	* Contour Link : https://contour.maxim-ic.com/contour//perspective.req?projectId=1841&docId=6727381
	**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/

	Maxim::I2C::Read(LOCKSTATUS_REG, gsiReadBack);

	ForEachActive_iSite{

		if ((gsiReadBack[iSite] & 0x01) == 1){              
			DUT::Trim::gsbLockBit[iSite] = true;	//1 = trimmed unit 
		}
		else{
			DUT::Trim::gsbLockBit[iSite] = false;	//0 = fresh unit 
		}

		if ((gsiReadBack[iSite] & 0x02) == 2){              
			DUT::Trim::gsbSPLockBit[iSite] = true;	//1 = scratch pad is programmed
		}
		else{
			DUT::Trim::gsbSPLockBit[iSite] = false;	//0 = fresh unit 
		}

	}

	Maxim::DatalogAll(DSIndex, DUT::Trim::gsbLockBit);



	//Load all '0' first
	Maxim::I2C::Write(OTP_BG_REG, 0);
	Maxim::I2C::Write(OTP_REF_REG, 0);
	Maxim::I2C::Write(OTP_LDO_REG, 0);
	Maxim::I2C::Write(OTP_BIAS_REG, 0);
	Maxim::I2C::Write(OTP_OSC_REG, 0);
	Maxim::I2C::Write(OTP_SLOPE_REG, 0);
	Maxim::I2C::Write(OTP_OUT_GAIN_REG, 0);
	Maxim::I2C::Write(OTP_OSC20M_REG, 0);
	Maxim::I2C::Write(OTP_OUT5_REG, 0);

	DUT::Trim::OUTx_Send_Data(OTP_3DV_AD, 1, 0);
	DUT::Trim::OUTx_Send_Data(OTP_3DV_AD, 2, 0);
	DUT::Trim::OUTx_Send_Data(OTP_3DV_AD, 3, 0);
	DUT::Trim::OUTx_Send_Data(OTP_3DV_AD, 4, 0);
	DUT::Trim::OUTx_Send_Data(OTP_3DV_AD, 6, 0);


	/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
	* Test : SP_Lockbit_Pre
	* Test No : #13010
	* Description : Read back Scratch Pad lockbit from Reg F4h<1>
	* Contour Link : https://contour.maxim-ic.com/contour//perspective.req?projectId=1841&docId=6727382
	**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/

	Maxim::DatalogAll(DSIndex, DUT::Trim::gsbSPLockBit);









	//begin trimming process if part is unprogrammed
	if ((GetUserSwitch(GPSW_ForceSim) && !Maxim::Operator()) || DUT::Trim::gsbLockBit.Any(false) && ( Maxim::bTest_Step_Is("FT_ROOM") || Maxim::bTest_Step_Is("CHAR")))
	{





		/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
		* Test : BG TRIM
		* Test No : #14000
		* Description : Write data to Reg 00h<5:0>, Measure voltage at COMP (CS as ground sense)
		* Contour Link : https://contour.maxim-ic.com/contour//perspective.req?projectId=1841&docId=6727383
		* Test Mode : 
						ADDR01 : DCTRL<11>(S_TMBG), DCTRL<2>(S_TMCOMPHZ), DCTRL<1>(S_TMCLKOFF), DCTRL<3>(S_TMCSHZ), AI<4>(V_TM_OUT1)
						ADDR02 : DCTRL<13>(S_TM_SGND_K)

		**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
		
		//Setup
		Maxim::I2C::SetSlaveAddress(TM_3DV_AD);

		Maxim::I2C::Write(0x02, 0xB0); //Reset BIPTM Machine
		Maxim::I2C::Write(0x02, 0x42); //write data to DHR3<15:12>=0010
		Maxim::I2C::Write(0x02, 0x30); //write data to DHR2<11:8>=0000
		Maxim::I2C::Write(0x02, 0x20); //write data to DHR1<7:4>=0000
		Maxim::I2C::Write(0x02, 0x10); //write data to DHR0<3:0>=0000
		Maxim::I2C::Write(0x02, 0xA0); //write DHR<15:0> to DCTLR<15:0>

		Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
		Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>=0000
		Maxim::I2C::Write(0x01, 0x38); //write data to DHR2<11:8>=1000
		Maxim::I2C::Write(0x01, 0x20); //write data to DHR1<7:4>=0000
		Maxim::I2C::Write(0x01, 0x1E); //write data to DHR0<3:0>=1110
		Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>

		Maxim::I2C::Write(0x01, 0x81); //AI<4>=V_TM_BG)


		//Switch APU ground sense to CS pin
		cbitclose(K18);

		apu12set( COMP, APU12_FI, 0, APU12_3p6V, APU12_10UA, APU12_PIN_TO_VI );

		lwait (5 ms_f);



		if(0){//Debug only
			PinListData bgv;

			FILE *bg_file;	
			fopen_s(&bg_file, "bgv.txt", "a+");
			for(int i = 0; i < 5000; i++){
				lwait (1 ms_f);
				bgv = apu12mv(COMP, 1);
				fprintf(bg_file, "%f \n", bgv[0]);

			}

			fclose(bg_file);

			SiteInt siReadback;
			int iData = 49;
			int iReg = OTP_BG_REG;

			Maxim::I2C::Write(OTP_3DV_AD, iReg, iData);
			pldResult = apu12mv(COMP, 100);

			Maxim::I2C::Write(OTP_3DV_AD, OTPMODE_REG, 0x0);
			Maxim::I2C::Write(OTP_3DV_AD, OTPMODE_REG, 0x03);

			Maxim::I2C::Read(iReg, siReadback);


		}


		//Trim
		if(bBitWeightsChar)
			DUT::Trim::gtsBG.CharacterizeBitWeights();

		DUT::Trim::gtsBG.FindBestTrimCode(DSIndex);
		g_BG_code = DUT::Trim::gtsBG.PostTrimCode();










		/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
		* Test : REF TRIM
		* Test No : #14200
		* Description : "Write data to Reg 01h<4:0>, Measure voltage at COMP (CS as ground sense)"
		* Contour Link : https://contour.maxim-ic.com/contour//perspective.req?projectId=1841&docId=6727388
		* Test Mode : 

					"ADDR01:
					AI<7>(V_REF1V25),
					DCTRL<2>(S_TMCOMPHZ), 
					DCTRL<3>(S_TMCSHZ)

					ADDR02:
					DCTRL<13>(S_TM_SGND_K)"
		**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/


		//Setup

		Maxim::I2C::SetSlaveAddress(TM_3DV_AD);

		Maxim::I2C::Write(0x02, 0xB0); //Reset BIPTM Machine
		Maxim::I2C::Write(0x02, 0x42); //write data to DHR3<15:12>=0010
		Maxim::I2C::Write(0x02, 0x30); //write data to DHR2<11:8>=0000
		Maxim::I2C::Write(0x02, 0x20); //write data to DHR1<7:4>=0000
		Maxim::I2C::Write(0x02, 0x10); //write data to DHR0<3:0>=0000
		Maxim::I2C::Write(0x02, 0xA0); //write DHR<15:0> to DCTLR<15:0>

		Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
		Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>=0000
		Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>=0000
		Maxim::I2C::Write(0x01, 0x20); //write data to DHR1<7:4>=0000
		Maxim::I2C::Write(0x01, 0x1E); //write data to DHR0<3:0>=1100
		Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>

		Maxim::I2C::Write(0x01, 0x88); //AI<7>=V_REF1V25)
		lwait (1 ms_f);


		if(0){//Debug only

			PinListData ref;
			Maxim::I2C::Write(OTP_3DV_AD, OTP_REF_REG, 0);
			FILE *ref_file;	
			fopen_s(&ref_file, "refv.txt", "a+");
			for(int i = 0; i < 5000; i++){
				lwait (1 ms_f);
				ref = apu12mv(COMP, 1);
				fprintf(ref_file, "%f \n", ref[0]);

			}

			fclose(ref_file);



			PinListData pldResult;
			SiteInt siReadback;
			int iData = 0;
			int iReg = OTP_REF_REG;

			Maxim::I2C::Write(OTP_3DV_AD, OTP_REF_REG, iData);
			pldResult = apu12mv(COMP, 100);

			Maxim::I2C::Write(OTP_3DV_AD, OTPMODE_REG, 0x03);

			Maxim::I2C::Read(iReg, siReadback);


		}



		//Trim
		if(bBitWeightsChar)
			DUT::Trim::gtsREF.CharacterizeBitWeights();

		DUT::Trim::gtsREF.FindBestTrimCode(DSIndex);
		g_REF_code = DUT::Trim::gtsREF.PostTrimCode();


		if(bREF_Check){
			Maxim::I2C::Read(OTP_REF_REG, gsiReadBack);
			for(int isite=0;isite<NUM_SITES;isite++){			
				if(msSiteStat(isite) && (gsiReadBack[isite] != g_REF_code[isite])){
					gsiReadBack = gsiReadBack;
				}
			}
		}
		//Switch APU ground sense to GND pin
		cbitopen(K18);




		/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
		* Test : LDO TRIM (AP79 Only)
		* Test No : #14400
		* Description : "Write data to Reg 02h<2:0>, Measure voltage at COMP"
		* Contour Link : https://contour.maxim-ic.com/contour//perspective.req?projectId=1841&docId=6727388
		* Test Mode : 
					"ADDR01:
					AI<0>(V_VCC_K),
					DCTRL<10>(S_TMLDO_K),
					DCTRL<2>(S_TMCOMPHZ)"
		**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
		if(0){


			//Setup
			Maxim::I2C::SetSlaveAddress(TM_3DV_AD);

			apu12set( COMP, APU12_FI, 0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );
			Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
			Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>=0000
			Maxim::I2C::Write(0x01, 0x34); //write data to DHR2<11:8>=0100
			Maxim::I2C::Write(0x01, 0x20); //write data to DHR1<7:4>=0000
			Maxim::I2C::Write(0x01, 0x14); //write data to DHR0<3:0>=0100
			Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>

			Maxim::I2C::Write(0x01, 0x71); //AI<0>=V_VCC_K)

			lwait (5 ms_f);


			//Trim
			if(bBitWeightsChar)
				DUT::Trim::gtsLDO.CharacterizeBitWeights();

			DUT::Trim::gtsLDO.FindBestTrimCode(DSIndex);
			g_LDO_code = DUT::Trim::gtsLDO.PostTrimCode();

		}
		else
			DSIndex+=5;










		/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
		* Test : BIAS TRIM
		* Test No : #14600
		* Description : Trim and measure the Current Reference on COMP
		* Contour Link : https://contour.maxim-ic.com/contour//perspective.req?projectId=1841&docId=6727388
		* Test Mode : 
					"ADDR01:
					AI<1>(I_ITRIM_10U),
					DCTRL<13>(S_TMIC),
					DCTRL<2>(S_TMCOMPHZ)"

		**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/


		//Setup
		Maxim::I2C::SetSlaveAddress(TM_3DV_AD);

		Maxim::I2C::Write(0x02, 0xB0); //Reset BIPTM Machine
		Maxim::I2C::Write(0x02, 0x40); //write data to DHR3<15:12>=0010
		Maxim::I2C::Write(0x02, 0x30); //write data to DHR2<11:8>=0000
		Maxim::I2C::Write(0x02, 0x20); //write data to DHR1<7:4>=0000
		Maxim::I2C::Write(0x02, 0x10); //write data to DHR0<3:0>=0000
		Maxim::I2C::Write(0x02, 0xA0); //write DHR<15:0> to DCTLR<15:0>

		Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
		Maxim::I2C::Write(0x01, 0x42); //write data to DHR3<15:12>=0010
		Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>=0000
		Maxim::I2C::Write(0x01, 0x20); //write data to DHR1<7:4>=0000
		Maxim::I2C::Write(0x01, 0x14); //write data to DHR0<3:0>=0100
		Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>

		Maxim::I2C::Write(0x01, 0x80);
		Maxim::I2C::Write(0x01, 0x72); //AI<1>(I_ITRIM_10U)

		apu12set( COMP, APU12_FV, 1, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );

		lwait (5 ms_f);


		//Trim
		if(bBitWeightsChar)
			DUT::Trim::gtsIBIAS.CharacterizeBitWeights();

		DUT::Trim::gtsIBIAS.FindBestTrimCode(DSIndex);
		g_BIAS_code = DUT::Trim::gtsIBIAS.PostTrimCode();


		apu12set( COMP, APU12_OFF, 0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );


		if(bREF_Check){
			Maxim::I2C::Read(OTP_REF_REG, gsiReadBack);
			for(int isite=0;isite<NUM_SITES;isite++){			
				if(msSiteStat(isite) && (gsiReadBack[isite] != g_REF_code[isite])){
					gsiReadBack = gsiReadBack;
				}
			}
		}




		/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
		* Test : SLOPE TRIM
		* Test No : #14800
		* Description : Trim and measure ISLOPE on CS by forcing 300mV and reading current.
		* Contour Link : https://contour.maxim-ic.com/contour//perspective.req?projectId=1841&docId=6727388
		* Test Mode : 
				"ADDR01:
				DCTRL<5>(S_TM<1>)"

		**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/


		//Setup
		Maxim::I2C::SetSlaveAddress(TM_3DV_AD);
		Maxim::I2C::Write(0x02, 0xB0); //Reset BIPTM Machine

		Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
		Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>=0000
		Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>=0000
		Maxim::I2C::Write(0x01, 0x22); //write data to DHR1<7:4>=0010
		Maxim::I2C::Write(0x01, 0x10); //write data to DHR0<3:0>=0000
		Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>

		Maxim::I2C::Write(0x01, 0x70);

		apu12set( CS, APU12_FV, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
		apu12set( CS, APU12_FV, 0.3, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );

		lwait (5 ms_f);


		//Trim
		if(bBitWeightsChar)
			DUT::Trim::gtsSLOPE.CharacterizeBitWeights();

		DUT::Trim::gtsSLOPE.FindBestTrimCode(DSIndex);
		g_SLOPE_code = DUT::Trim::gtsSLOPE.PostTrimCode();

		apu12setcapload( CS, APU12_FV, 0, APU12_10V, APU12_1MA, 0.5, APU12_PIN_TO_VI );
		apu12set( CS, APU12_OFF, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );




		if(bREF_Check){
			Maxim::I2C::Read(OTP_REF_REG, gsiReadBack);
			for(int isite=0;isite<NUM_SITES;isite++){			
				if(msSiteStat(isite) && (gsiReadBack[isite] != g_REF_code[isite])){
					gsiReadBack = gsiReadBack;
				}
			}
		}



		/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
		* Test : OSC TRIM
		* Test No : #15000
		* Description : Trim and measure boost clock divided by 8 on NDRV. Trim at 400kHz/8
		* Contour Link : 
		* Test Mode : 
				"ADDR01:
					DCTRL<6>(S_TM<2>)
				 ADDR02:
					DCTRL<0>(S_TMDTHOFF)

		**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/


		//Setup
		Maxim::I2C::SetSlaveAddress(TM_3DV_AD);
		Maxim::I2C::Write(0x02, 0xB0); //Reset BIPTM Machine
		Maxim::I2C::Write(0x02, 0x40); //write data to DHR3<15:12>=0000
		Maxim::I2C::Write(0x02, 0x30); //write data to DHR2<11:8>=0000
		Maxim::I2C::Write(0x02, 0x20); //write data to DHR1<7:4>=0100
		Maxim::I2C::Write(0x02, 0x11); //write data to DHR0<3:0>=0000
		Maxim::I2C::Write(0x02, 0xA0); //write DHR<15:0> to DCTLR<15:0>

		Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
		Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>=0000
		Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>=0000
		Maxim::I2C::Write(0x01, 0x24); //write data to DHR1<7:4>=0000
		Maxim::I2C::Write(0x01, 0x10); //write data to DHR0<3:0>=0001
		Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>


		cbitclose(K43, K44);

		qtmuinit(QTMU_GP);

		dpinrcvset("SCAN_OUT", DPIN_RCV_SPLIT, 2.5, 1.4, MS_ALL );
		qtmumode(QTMU_GP, QTMU_TIMER, QTMU_ARMOUTOFF );
		qtmustart(QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_POS, 0, 500);
		qtmustop(QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_POS, 1, 500);
		
		lwait (5 ms_f);


		//Trim
		if(bBitWeightsChar)
			DUT::Trim::gtsOSC.CharacterizeBitWeights();

		DUT::Trim::gtsOSC.FindBestTrimCode(DSIndex);
		g_OSC_code = DUT::Trim::gtsOSC.PostTrimCode();




		if(bREF_Check){
			Maxim::I2C::Read(OTP_REF_REG, gsiReadBack);
			for(int isite=0;isite<NUM_SITES;isite++){			
				if(msSiteStat(isite) && (gsiReadBack[isite] != g_REF_code[isite])){
					gsiReadBack = gsiReadBack;
				}
			}
		}





		/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
		* Test : OSC20M TRIM
		* Test No : #15200
		* Description : TTrim and measure 20MHz clock divided by 256 on NDRV.
		* Contour Link : 
		* Test Mode : 
				"ADDR01:
					DCTRL<4>(S_TM<0>)

		**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/


		//Setup
		Maxim::I2C::SetSlaveAddress(TM_3DV_AD);
		Maxim::I2C::Write(0x02, 0xB0); //Reset BIPTM Machine

		Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
		Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>=0000
		Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>=0000
		Maxim::I2C::Write(0x01, 0x21); //write data to DHR1<7:4>=0001
		Maxim::I2C::Write(0x01, 0x10); //write data to DHR0<3:0>=0000
		Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>


		lwait (5 ms_f);


		//Trim
		if(bBitWeightsChar)
			DUT::Trim::gtsOSC20M.CharacterizeBitWeights();

		DUT::Trim::gtsOSC20M.FindBestTrimCode(DSIndex);
		g_OSC20M_code = DUT::Trim::gtsOSC20M.PostTrimCode();

		//Restore
		cbitopen(K43, K44);
		dpinrcvset("SCAN_OUT", DPIN_OFF, 2.5, 1.4, MS_ALL );
		qtmustart(QTMU_GP, "OFF", QTMU_RCVH, QTMU_POS, 0, 50, 0.0 );
		qtmustop(QTMU_GP, "OFF", QTMU_RCVH, QTMU_POS, 1, 50);



		if(bREF_Check){
			Maxim::I2C::Read(OTP_REF_REG, gsiReadBack);
			for(int isite=0;isite<NUM_SITES;isite++){			
				if(msSiteStat(isite) && (gsiReadBack[isite] != g_REF_code[isite])){
					gsiReadBack = gsiReadBack;
				}
			}
		}



		/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
		* Test : DIM FREQ BIT
		* Test No : #15400
		* Description : 0 = DIM freq=100Hz (Default)          1 = DIM freq=1.6kHz
		**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
		g_DIM_code = 0;
		
		//No need to write any thing if 0
		if(g_DIM_code)
			Maxim::I2C::Write(OTP_3DV_AD, OTP_BG_REG, g_BG_code + 64);

		Maxim::DatalogAll(DSIndex, g_DIM_code);


		if(bREF_Check){
			Maxim::I2C::Read(OTP_REF_REG, gsiReadBack);
			for(int isite=0;isite<NUM_SITES;isite++){			
				if(msSiteStat(isite) && (gsiReadBack[isite] != g_REF_code[isite])){
					gsiReadBack = gsiReadBack;
				}
			}
		}


		/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
		* Test : OUT GAIN TRIM
		* Test No : #16000
		* Description : Trim and measure the Current on OUTx (config as 120mA)
		* Contour Link :
		* Test Mode : 
					"ADDR02:
					DCTRL<10>(S_TM_EN_SINK_LED),
					DCTRL<12>(S_TM_EN_COMP_LED)

		**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/

		//Setup
		Maxim::I2C::SetSlaveAddress(TM_3DV_AD);
		Maxim::I2C::Write(0x02, 0xB0); //Reset BIPTM Machine
		Maxim::I2C::Write(0x02, 0x41); //write data to DHR3<15:12>=0001
		Maxim::I2C::Write(0x02, 0x34); //write data to DHR2<11:8>=0100
		Maxim::I2C::Write(0x02, 0x20); //write data to DHR1<7:4>=0000
		Maxim::I2C::Write(0x02, 0x10); //write data to DHR0<3:0>=0000
		Maxim::I2C::Write(0x02, 0xA0); //write DHR<15:0> to DCTLR<15:0>

		Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
		Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>=0010
		Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>=0000
		Maxim::I2C::Write(0x01, 0x20); //write data to DHR1<7:4>=0000
		Maxim::I2C::Write(0x01, 0x10); //write data to DHR0<3:0>=0100
		Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>

		Maxim::I2C::Write(0x01, 0x70);
		Maxim::I2C::Write(0x01, 0x80);

		apu12setcapload( ALL_OUT, APU12_FV, 0.3, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );

		//Write '0x0F' to iset REG (120mA Sink Current)
		Maxim::I2C::Write(AP89_AD, 0x02, 0x2F);

		//Write '0x02' to TEST REG (tm_iset test mode)
		Maxim::I2C::Write(AP89_AD, 0x20, 0x02);

		//Write '0xFF' to TEST_ISETDAC REG (ISET value to be programmed in tm_iset test mode)
		Maxim::I2C::Write(AP89_AD, 0x21, 0xFF);

		iReg = 0x02;
		//Maxim::I2C::Read(AP89_AD,iReg, gsiReadBack);


		gbOUTGainPre = true;

		//Trim
		if(bBitWeightsChar)
			DUT::Trim::gtsOUTGAIN.CharacterizeBitWeights();
		
		DUT::Trim::gtsOUTGAIN.EnableDatalogging(false);
		DUT::Trim::gtsOUTGAIN.FindBestTrimCode();
		g_OUT_GAIN_code = DUT::Trim::gtsOUTGAIN.PostTrimCode();

		SiteBool sbLockBit_Temp;

		sbLockBit_Temp = DUT::Trim::gsbLockBit;


		if(bREF_Check){
			Maxim::I2C::Read(OTP_REF_REG, gsiReadBack);
			for(int isite=0;isite<NUM_SITES;isite++){			
				if(msSiteStat(isite) && (gsiReadBack[isite] != g_REF_code[isite])){
					gsiReadBack = gsiReadBack;
				}
			}
		}


		if (GetUserSwitch(GPSW_ForceSim)){
			ForEachActive_iSite{
				sbLockBit_Temp[iSite] = 0;
			}
		}


		//manually dataloging (PRE-TRIM)
		SiteDatalog(DSIndex, sbLockBit_Temp, gpldOUTx_Pre[0]);
		SiteDatalog(DSIndex, sbLockBit_Temp, gpldOUTx_Pre[1]);
		SiteDatalog(DSIndex, sbLockBit_Temp, gpldOUTx_Pre[2]);
		SiteDatalog(DSIndex, sbLockBit_Temp, gpldOUTx_Pre[3]);
		SiteDatalog(DSIndex, sbLockBit_Temp, gpldOUTx_Pre[4]);
		SiteDatalog(DSIndex, sbLockBit_Temp, gpldOUTx_Pre[5]);

		SiteDatalog(DSIndex, sbLockBit_Temp, DUT::Trim::gtsOUTGAIN.PreTrimMeasurement()); //Pre average
		SiteDatalog(DSIndex, sbLockBit_Temp, DUT::Trim::gtsOUTGAIN.PreTrimCode()); //Code Pre

		SiteDatalog(DSIndex, sbLockBit_Temp, gpldOUTx_Post[0]);
		SiteDatalog(DSIndex, sbLockBit_Temp, gpldOUTx_Post[1]);
		SiteDatalog(DSIndex, sbLockBit_Temp, gpldOUTx_Post[2]);
		SiteDatalog(DSIndex, sbLockBit_Temp, gpldOUTx_Post[3]);
		SiteDatalog(DSIndex, sbLockBit_Temp, gpldOUTx_Post[4]);
		SiteDatalog(DSIndex, sbLockBit_Temp, gpldOUTx_Post[5]);

		SiteDatalog(DSIndex, sbLockBit_Temp, DUT::Trim::gtsOUTGAIN.PostTrimMeasurement()); //Post average
		SiteDatalog(DSIndex, sbLockBit_Temp, DUT::Trim::gtsOUTGAIN.PostTrimCode()); //Code Post
		SiteDatalog(DSIndex, sbLockBit_Temp, DUT::Trim::gtsOUTGAIN.NumberOfAttempts());




		/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
		* Test : OUT1 TRIM
		* Test No : #17000
		* Description : Trim and measure the Current on OUTx (config as 50mA)
		* Contour Link :
		* Test Mode : 
					"ADDR02:
					DCTRL<10>(S_TM_EN_SINK_LED),
					DCTRL<12>(S_TM_EN_COMP_LED)

		**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/


		//Setup

		//Write '0x01' to iset REG (50mA Sink Current)
		Maxim::I2C::Write(AP89_AD, 0x02, 0x21);


		apu12setcapload( OUT1, APU12_FV, 2.0, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );
		lwait (5 ms_f);


		//Trim
		if(bBitWeightsChar)
			DUT::Trim::gtsOUT1.CharacterizeBitWeights();

		DUT::Trim::gtsOUT1.FindBestTrimCode(DSIndex);
		g_OUT1_code = DUT::Trim::gtsOUT1.PostTrimCode();


		//Restore
		apu12set( OUT1, APU12_FV, 0.3, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );




		if(bREF_Check){
			Maxim::I2C::Read(OTP_REF_REG, gsiReadBack);
			for(int isite=0;isite<NUM_SITES;isite++){			
				if(msSiteStat(isite) && (gsiReadBack[isite] != g_REF_code[isite])){
					gsiReadBack = gsiReadBack;
				}
			}
		}




		/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
		* Test : OUT2 TRIM
		* Test No : #17000
		* Description : Trim and measure the Current on OUTx (config as 50mA)
		* Contour Link :
		* Test Mode : 
					"ADDR02:
					DCTRL<10>(S_TM_EN_SINK_LED),
					DCTRL<12>(S_TM_EN_COMP_LED)

		**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/

		//Setup
		apu12setcapload( OUT2, APU12_FV, 2.0, APU12_10V, APU12_100MA, 1, APU12_PIN_TO_VI );
		lwait (5 ms_f);

		//Trim
		if(bBitWeightsChar)
			DUT::Trim::gtsOUT2.CharacterizeBitWeights();

		DUT::Trim::gtsOUT2.FindBestTrimCode(DSIndex);
		g_OUT2_code = DUT::Trim::gtsOUT2.PostTrimCode();

		//Restore
		apu12set( OUT2, APU12_FV, 0.3, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );


		if(bREF_Check){
			Maxim::I2C::Read(OTP_REF_REG, gsiReadBack);
			for(int isite=0;isite<NUM_SITES;isite++){			
				if(msSiteStat(isite) && (gsiReadBack[isite] != g_REF_code[isite])){
					gsiReadBack = gsiReadBack;
				}
			}
		}


		/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
		* Test : OUT3 TRIM
		* Test No : #
		* Description : Trim and measure the Current on OUTx (config as 50mA)
		* Contour Link :
		* Test Mode : 
					"ADDR02:
					DCTRL<10>(S_TM_EN_SINK_LED),
					DCTRL<12>(S_TM_EN_COMP_LED)

		**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/

		//Setup
		apu12setcapload( OUT3, APU12_FV, 2.0, APU12_10V, APU12_100MA, 1, APU12_PIN_TO_VI );
		lwait (5 ms_f);

		//Trim
		if(bBitWeightsChar)
			DUT::Trim::gtsOUT3.CharacterizeBitWeights();

		DUT::Trim::gtsOUT3.FindBestTrimCode(DSIndex);
		g_OUT3_code = DUT::Trim::gtsOUT3.PostTrimCode();

		//Restore
		apu12set( OUT3, APU12_FV, 0.3, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );



		if(bREF_Check){
			Maxim::I2C::Read(OTP_REF_REG, gsiReadBack);
			for(int isite=0;isite<NUM_SITES;isite++){			
				if(msSiteStat(isite) && (gsiReadBack[isite] != g_REF_code[isite])){
					gsiReadBack = gsiReadBack;
				}
			}
		}


		/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
		* Test : OUT4 TRIM
		* Test No : #
		* Description : Trim and measure the Current on OUTx (config as 50mA)
		* Contour Link :
		* Test Mode : 
					"ADDR02:
					DCTRL<10>(S_TM_EN_SINK_LED),
					DCTRL<12>(S_TM_EN_COMP_LED)

		**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/

		//Setup
		apu12setcapload( OUT4, APU12_FV, 2.0, APU12_10V, APU12_100MA, 1, APU12_PIN_TO_VI );
		lwait (5 ms_f);

		//Trim
		if(bBitWeightsChar)
			DUT::Trim::gtsOUT4.CharacterizeBitWeights();

		DUT::Trim::gtsOUT4.FindBestTrimCode(DSIndex);
		g_OUT4_code = DUT::Trim::gtsOUT4.PostTrimCode();

		//Restore
		apu12set( OUT4, APU12_FV, 0.3, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );


		if(bREF_Check){
			Maxim::I2C::Read(OTP_REF_REG, gsiReadBack);
			for(int isite=0;isite<NUM_SITES;isite++){			
				if(msSiteStat(isite) && (gsiReadBack[isite] != g_REF_code[isite])){
					gsiReadBack = gsiReadBack;
				}
			}
		}






		/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
		* Test : OUT5 TRIM
		* Test No : #
		* Description : Trim and measure the Current on OUTx (config as 50mA)
		* Contour Link :
		* Test Mode : 
					"ADDR02:
					DCTRL<10>(S_TM_EN_SINK_LED),
					DCTRL<12>(S_TM_EN_COMP_LED)

		**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/

		//Setup
		apu12setcapload( OUT5, APU12_FV, 2.0, APU12_10V, APU12_100MA, 1, APU12_PIN_TO_VI );
		lwait (5 ms_f);

		//Trim
		if(bBitWeightsChar)
			DUT::Trim::gtsOUT5.CharacterizeBitWeights();

		DUT::Trim::gtsOUT5.FindBestTrimCode(DSIndex);
		g_OUT5_code = DUT::Trim::gtsOUT5.PostTrimCode();

		//Restore
		apu12set( OUT5, APU12_FV, 0.3, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );


		if(bREF_Check){
			Maxim::I2C::Read(OTP_REF_REG, gsiReadBack);
			for(int isite=0;isite<NUM_SITES;isite++){			
				if(msSiteStat(isite) && (gsiReadBack[isite] != g_REF_code[isite])){
					gsiReadBack = gsiReadBack;
				}
			}
		}

		/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
		* Test : OUT6 TRIM
		* Test No : #
		* Description : Trim and measure the Current on OUTx (config as 50mA)
		* Contour Link :
		* Test Mode : 
					"ADDR02:
					DCTRL<10>(S_TM_EN_SINK_LED),
					DCTRL<12>(S_TM_EN_COMP_LED)

		**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/

		//Setup
		apu12setcapload( OUT6, APU12_FV, 2.0, APU12_10V, APU12_100MA, 1, APU12_PIN_TO_VI );
		lwait (5 ms_f);

		//Trim
		if(bBitWeightsChar)
			DUT::Trim::gtsOUT6.CharacterizeBitWeights();

		DUT::Trim::gtsOUT6.FindBestTrimCode(DSIndex);
		g_OUT6_code = DUT::Trim::gtsOUT6.PostTrimCode();

		//Restore
		apu12set( OUT6, APU12_FV, 0.3, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );



		if(bREF_Check){
			Maxim::I2C::Read(OTP_REF_REG, gsiReadBack);
			for(int isite=0;isite<NUM_SITES;isite++){			
				if(msSiteStat(isite) && (gsiReadBack[isite] != g_REF_code[isite])){
					gsiReadBack = gsiReadBack;
				}
			}
		}

		Maxim::DatalogAll(DSIndex, OTP_BSTOSC_Bit);
	

	}




	//End of function
	Maxim::DatalogFlush();	//Output the datalog queue.
	return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC
