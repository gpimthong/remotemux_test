/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:
*		Supply_Current.cpp
*
*	Written and maintained by:
*		Govit Pimthong (govit.pimthong@MaximIntegrated.com)
*
*	URL:                   $URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/OTP_Post.cpp $
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

void BSTMON_UV_Blanking(LPCTSTR some_name, int some_int){
	SiteInt siAck;

	siAck = DUT::BSTUV::Write(AP89_AD, 0x02, 0x20);
	lwait( 1 ms_f );

}

// Test Function: OTP_Post
ETS_PRGFLOW_FUNC OTP_Post( int DSIndex, LPCTSTR TestLabel )
{

	//declare variables
	PinListData pldResults;	//declare a PLD variable for the measurement results.
	gsiBSTUV = SiteInt(0);


	cbitclose(K40, K41, K34, K37);
	
	dpinforce("DIM", "0", 3.0, 0.0, MS_ALL);
	dpinforce("SCL_IO", "0", 3.0, 0.0, MS_ALL);
	dpinforce("SDA_IO", "0", 3.0, 0.0, MS_ALL);
	lwait (1 ms_f);


	//Step#2
	apu12set( ALL_OUT, APU12_FV, 0.7, APU12_10V, APU12_100MA, APU12_PIN_TO_VI );
	apu12set( BSTMON, APU12_FV, 0.0, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
	lwait (1 ms_f);


	//Step#3
	apu12setcapload( IN1, APU12_FV, 12, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );
	dpinviset("SDA", DPIN_LOAD, 4, DPIN_8V, DPIN_512UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL);
	//*************************************************************************************************




	//*************************************************************************************************
	//BSTMON UV Blanking
	//*************************************************************************************************
	
	//Pull up FLT and PGATE
	cbitclose(K32, K43, K45, K49);
	lwait(3 ms_f);
	apu12set( EN, APU12_FV, 5, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );

	gpldBSTMON_UV_BL = MeasureTime(QTMU_GP, direct_v, direct_v, "AUTO", 1, BSTMON_UV_Blanking, "", 100 ms_f, QTMU_DIRECTA, QTMU_DIRECTB, QTMU_POS, QTMU_NEG, 0, 0, 2.5 , 2.5, 5000, 5000);
	
	cbitopen(K32, K43, K45, K49);
	lwait(1 ms_f);


	//*************************************************************************************************

	//Read BSTUV fault bit here.
	Maxim::I2C::Read(AP89_AD, 0x1F, gsiBSTUV);

	apu12set( BSTMON, APU12_FV, 0.7, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
	apu12set( EN, APU12_FV, 5, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
	lwait (2 ms_f);

	//Write '1' to ENA bit REG02<5>

	Maxim::I2C::Write(AP89_AD, 0x02, 0x20);

	lwait (2 ms_f);

	DUT::Initiate_TestMode();


	Maxim::I2C::SetSlaveAddress(OTP_3DV_AD);

	Maxim::I2C::Write(UNLOCK1_REG, 0x17); //Unlock passwd0 = 0x17
	Maxim::I2C::Write(UNLOCK2_REG, 0x65); //Unlock passwd1 = 0x65

	Maxim::I2C::Write(OTPMODE_REG, 0x00); //Turn off Look and bypass



	/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
	* Test : Lockbit
	**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
	SiteInt siOTPLockbit, siSPLockbit;

	Maxim::I2C::Read(LOCKSTATUS_REG, gsiReadBack);

	ForEachActive_iSite{

		if ((gsiReadBack[iSite] & 0x01) == 1){              
			siOTPLockbit[iSite] = true;	//1 = trimmed unit 
		}
		else{
			siOTPLockbit[iSite] = false;	//0 = fresh unit 
		}

		if ((gsiReadBack[iSite] & 0x02) == 2){              
			siSPLockbit[iSite] = true;	//1 = scratch pad is programmed
		}
		else{
			siSPLockbit[iSite] = false;	//0 = fresh unit 
		}

	}

	Maxim::DatalogAll(DSIndex, siOTPLockbit);
	Maxim::DatalogAll(DSIndex, siSPLockbit);




	/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
	* Test : BG
	* Test No : #14000
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


	pldResults = DUT::Trim::gtsBG.Measure();

	Maxim::DatalogAll(DSIndex, pldResults);






	/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
	* Test : REF
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

	lwait (5 ms_f);

	if(0){//Debug only

		PinListData refpv;

		FILE *ref_p_file;	
		fopen_s(&ref_p_file, "ref_post.txt", "a+");
		for(int i = 0; i < 5000; i++){
			lwait (1 ms_f);
			refpv = apu12mv(COMP, 1);
			fprintf(ref_p_file, "%f \n", refpv[0]);

		}

		fclose(ref_p_file);
	}

	pldResults = DUT::Trim::gtsREF.Measure();

	Maxim::DatalogAll(DSIndex, pldResults);

	//Switch APU ground sense to GND pin
	cbitopen(K18);





	/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
	* Test : LDO (AP79 Only)
	**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
	if(!AP89){


		//Setup
		apu12set( COMP, APU12_FI, 0, APU12_10V, APU12_10UA, APU12_PIN_TO_VI );
		Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
		Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>=0000
		Maxim::I2C::Write(0x01, 0x34); //write data to DHR2<11:8>=0100
		Maxim::I2C::Write(0x01, 0x20); //write data to DHR1<7:4>=0000
		Maxim::I2C::Write(0x01, 0x14); //write data to DHR0<3:0>=0100
		Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>

		Maxim::I2C::Write(0x01, 0x71); //AI<0>=V_VCC_K)

		lwait (5 ms_f);

		pldResults = DUT::Trim::gtsLDO.Measure();

		Maxim::DatalogAll(DSIndex, pldResults);

	}
	else
		DSIndex+=1;










	/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
	* Test : BIAS
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


	pldResults = DUT::Trim::gtsIBIAS.Measure();

	Maxim::DatalogAll(DSIndex, pldResults * uA);


	apu12set( COMP, APU12_OFF, 0, APU12_10V, APU12_100UA, APU12_PIN_TO_VI );







	/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
	* Test : SLOPE
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


	pldResults = DUT::Trim::gtsSLOPE.Measure();

	Maxim::DatalogAll(DSIndex, pldResults * uA);

	apu12setcapload( CS, APU12_FV, 0, APU12_10V, APU12_1MA, 0.5, APU12_PIN_TO_VI );
	apu12set( CS, APU12_OFF, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );








	/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
	* Test : OSC
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

	dpinrcvset("SCAN_OUT", DPIN_RCV_SPLIT, 2.5, 1.4, MS_ALL );
	qtmumode(QTMU_GP, QTMU_TIMER, QTMU_ARMOUTOFF );
	qtmustart(QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_POS, 0, 500);
	qtmustop(QTMU_GP, "SCAN_OUT", QTMU_RCVH, QTMU_POS, 1, 500);

	lwait (3 ms_f);
	
	pldResults = DUT::Trim::gtsOSC.Measure();

	Maxim::DatalogAll(DSIndex, pldResults);








	/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
	* Test : OSC20M
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


	pldResults = DUT::Trim::gtsOSC20M.Measure();

	Maxim::DatalogAll(DSIndex, pldResults * MHz);


	//Restore
	cbitopen(K43, K44);
	dpinrcvset("SCAN_OUT", DPIN_OFF, 2.5, 1.4, MS_ALL );
	qtmustart(QTMU_GP, "OFF", QTMU_RCVH, QTMU_POS, 0, 50, 0.0 );
	qtmustop(QTMU_GP, "OFF", QTMU_RCVH, QTMU_POS, 1, 50);

	if(0){

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

		apu12setcapload( ALL_OUT, APU12_FV, 0.3, APU12_10V, APU12_200MA, 1, APU12_PIN_TO_VI );

		//Write '0x21' to iset REG (50mA Sink Current)
		Maxim::I2C::Write(AP89_AD, 0x02, 0x21);

		//Write '0x02' to TEST REG (tm_iset test mode)
		Maxim::I2C::Write(AP89_AD, 0x20, 0x02);

		//Write '0xFF' to TEST_ISETDAC REG (ISET value to be programmed in tm_iset test mode)
		Maxim::I2C::Write(AP89_AD, 0x21, 0xFF);



		for (int i=0; i< 3; i++){


			if(i==1)	//Write '0x2B' to iset REG (100mA Sink Current)
				Maxim::I2C::Write(AP89_AD, 0x02, 0x2B);

			if(i==2)	//Write '0x2F' to iset REG (120mA Sink Current)
				Maxim::I2C::Write(AP89_AD, 0x02, 0x2F);


			/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
			* Test : OUT1
			**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/


			//Setup
			apu12setcapload( OUT1, APU12_FV, 2.0, APU12_10V, APU12_200MA, 1, APU12_PIN_TO_VI );
			lwait (5 ms_f);

			pldResults = DUT::Trim::gtsOUT1.Measure();
			Maxim::DatalogAll(DSIndex, pldResults);

			//Restore
			apu12set( OUT1, APU12_FV, 0.3, APU12_10V, APU12_200MA, APU12_PIN_TO_VI );



			/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
			* Test : OUT2
			**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/


			//Setup
			apu12setcapload( OUT2, APU12_FV, 2.0, APU12_10V, APU12_200MA, 1, APU12_PIN_TO_VI );
			lwait (5 ms_f);

			pldResults = DUT::Trim::gtsOUT2.Measure();
			Maxim::DatalogAll(DSIndex, pldResults);

			//Restore
			apu12set( OUT2, APU12_FV, 0.3, APU12_10V, APU12_200MA, APU12_PIN_TO_VI );



			/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
			* Test : OUT3
			**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/


			//Setup
			apu12setcapload( OUT3, APU12_FV, 2.0, APU12_10V, APU12_200MA, 1, APU12_PIN_TO_VI );
			lwait (5 ms_f);

			pldResults = DUT::Trim::gtsOUT3.Measure();
			Maxim::DatalogAll(DSIndex, pldResults);

			//Restore
			apu12set( OUT3, APU12_FV, 0.3, APU12_10V, APU12_200MA, APU12_PIN_TO_VI );




			/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
			* Test : OUT4
			**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/


			//Setup
			apu12setcapload( OUT4, APU12_FV, 2.0, APU12_10V, APU12_200MA, 1, APU12_PIN_TO_VI );
			lwait (5 ms_f);

			pldResults = DUT::Trim::gtsOUT4.Measure();
			Maxim::DatalogAll(DSIndex, pldResults);

			//Restore
			apu12set( OUT4, APU12_FV, 0.3, APU12_10V, APU12_200MA, APU12_PIN_TO_VI );



			if(AP89){

				/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
				* Test : OUT5
				**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/


				//Setup
				apu12setcapload( OUT5, APU12_FV, 2.0, APU12_10V, APU12_200MA, 1, APU12_PIN_TO_VI );
				lwait (5 ms_f);

				pldResults = DUT::Trim::gtsOUT5.Measure();
				Maxim::DatalogAll(DSIndex, pldResults);

				//Restore
				apu12set( OUT5, APU12_FV, 0.3, APU12_10V, APU12_200MA, APU12_PIN_TO_VI );





				/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
				* Test : OUT6
				**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/


				//Setup
				apu12setcapload( OUT6, APU12_FV, 2.0, APU12_10V, APU12_200MA, 1, APU12_PIN_TO_VI );
				lwait (5 ms_f);

				pldResults = DUT::Trim::gtsOUT6.Measure();
				Maxim::DatalogAll(DSIndex, pldResults);

				//Restore
				apu12set( OUT6, APU12_FV, 0.3, APU12_10V, APU12_200MA, APU12_PIN_TO_VI );


			}
			else
				DSIndex+=2;

			Maxim::I2C::Write(AP89_AD, 0x02, 0x20);


		}



	}
	cbitopen(K33, K40, K41, K34, K37);


    //End of function
    Maxim::DatalogFlush();	//Output the datalog queue.
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC
