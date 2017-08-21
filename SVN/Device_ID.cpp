/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*
*	Source title:
*		Device_ID.cpp
*
*	Written and maintained by:
*		Govit Pimthong (govit.pimthong@MaximIntegrated.com)
*
*	URL:                   $URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/Device_ID.cpp $
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



// Test Function: Device_ID
ETS_PRGFLOW_FUNC Device_ID( int DSIndex, LPCTSTR TestLabel )
{

	SiteInt siReadBack;

	if(gbSkipPowerDown){ //Device has been power down at previous function. Need power up here

		//Powering up (https://contour.maxim-ic.com/contour//perspective.req?projectId=1841&docId=7679744)

		//Step#1
		apu12set( IN1, APU12_FV, 0, APU12_30V, APU12_100MA, APU12_PIN_TO_VI );
		apu12set( EN, APU12_FV, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
		apu12set( BSTMON, APU12_FV, 0, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
		apu12set( FSEN, APU12_FV, 0, APU12_10V, APU12_10MA, APU12_PIN_TO_VI );
		apu12set( CS, APU12_FV, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
		apu12set( ALL_OUT, APU12_FV, 0, APU12_10V, APU12_200MA, APU12_PIN_TO_VI );

		cbitclose(K40, K41, K34, K37);
		lwait (1 ms_f);

		dpinforce("DIM", "1", 3.0, 0.1, MS_ALL); //DIM at 5V
		dpinforce("SCL_IO", "0", 3.0, 0.0, MS_ALL);
		dpinforce("SDA_IO", "0", 3.0, 0.0, MS_ALL);
		lwait (1 ms_f);


		//Step#2
		apu12set( ALL_OUT, APU12_FV, 0.7, APU12_10V, APU12_200MA, APU12_PIN_TO_VI );
		apu12set( BSTMON, APU12_FV, 0.7, APU12_30V, APU12_1MA, APU12_PIN_TO_VI );
		lwait (1 ms_f);


		//Step#3
		apu12setcapload( IN1, APU12_FV, 12, APU12_30V, APU12_100MA, 1, APU12_PIN_TO_VI );

		//*************************************************************************************************

		apu12set( EN, APU12_FV, 5, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );
		lwait (2 ms_f);

		//Write '1' to ENA bit REG02<5>
		dpinviset("SDA", DPIN_LOAD, 4, DPIN_8V, DPIN_512UA, DPIN_CLAMP_OFF, DPIN_CLAMP_OFF, MS_ALL);
		Maxim::I2C::Write(AP89_AD, 0x02, 0x20);

		lwait (2 ms_f);

		DUT::Initiate_TestMode();


		Maxim::I2C::SetSlaveAddress(OTP_3DV_AD);

		Maxim::I2C::Write(UNLOCK1_REG, 0x17); //Unlock passwd0 = 0x17
		Maxim::I2C::Write(UNLOCK2_REG, 0x65); //Unlock passwd1 = 0x65

		Maxim::I2C::Write(OTPMODE_REG, 0x00); //Turn off Look and bypass


		//Setup
		Maxim::I2C::SetSlaveAddress(TM_3DV_AD);

		Maxim::I2C::Write(0x02, 0xB0); //Reset BIPTM Machine

		Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
		Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>=0000
		Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>=1000
		Maxim::I2C::Write(0x01, 0x20); //write data to DHR1<7:4>=0000
		Maxim::I2C::Write(0x01, 0x12); //write data to DHR0<3:0>=1110
		Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>



		//Release CS
		apu12set( CS, APU12_OFF, 0, APU12_10V, APU12_1MA, APU12_PIN_TO_VI );

	}

	else{

		Maxim::I2C::SetSlaveAddress(OTP_3DV_AD);

		Maxim::I2C::Write(OTPMODE_REG, 0x00); //Turn off Look and bypass


		//Setup
		Maxim::I2C::SetSlaveAddress(TM_3DV_AD);

		Maxim::I2C::Write(0x02, 0xB0); //Reset BIPTM Machine

		Maxim::I2C::Write(0x01, 0xB0); //Reset BIPTM Machine
		Maxim::I2C::Write(0x01, 0x40); //write data to DHR3<15:12>=0000
		Maxim::I2C::Write(0x01, 0x30); //write data to DHR2<11:8>=1000
		Maxim::I2C::Write(0x01, 0x20); //write data to DHR1<7:4>=0000
		Maxim::I2C::Write(0x01, 0x10); //write data to DHR0<3:0>=1110
		Maxim::I2C::Write(0x01, 0xA0); //write DHR<15:0> to DCTLR<15:0>

	}


	/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
	* Test : EngSample, WaferNumber, X_Coordinate, Y_Coordinate and SerialNumber
	* Test No : #22000 - 22040
	* Description : Read back die information from scratch pad register
	* Contour Link : 
	**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/

	Maxim::I2C::SetSlaveAddress(OTP_3DV_AD);

	ScratchPad_XY_ReadBack(DSIndex);


	DSIndex+=2; //Skip RandomID and SerialNumber2 for now


	
	/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
	* Test : DEV_ID
	* Test No : #22070
	* Description : Read back from device register 0x00. Expect '0x44' for AP79 and '0x46' for AP89
	* Contour Link : 
	**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
	Maxim::I2C::SetSlaveAddress(AP89_AD);

	Maxim::I2C::Read(0x00,siReadBack);

	Maxim::DatalogAll(DSIndex, siReadBack);



	/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
	* Test : REV_ID
	* Test No : #22080
	* Description : Read back from device register 0x01 (4-bit). Expect '0x00' for PASS1 , '0x01' for PASS2.. and so on
	* Contour Link : 
	**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/

	Maxim::I2C::Read(0x01,siReadBack);

	Maxim::DatalogAll(DSIndex, siReadBack);






  	gbSkipPowerDown = false;   

    //End of function
    Maxim::DatalogFlush();	//Output the datalog queue.
    return( msSiteStat( MS_ALL ) ); // Return w/status
} // END_ETS_PRGFLOW_FUNC
