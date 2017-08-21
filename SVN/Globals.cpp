/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*                                                                            
*	Source title:		Globals.h                                    
*		Created from Maxim Template                         
*       Template Written and maintained by:								 
*			Patrick Hayes (Patrick.Hayes@MaximIntegrated.com)	                 
*			Mark Bruening (Mark.Bruening@MaximIntegrated.com)                      
*		  
*	URL:				$URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/Globals.cpp $
*   Last Modified by:	$LastChangedBy: govit.pimthong $                                       
*	Date:				$Date: 2017-08-03 22:36:37 +0700 (Thu, 03 Aug 2017) $                                                
*   Current Rev:		$Revision: 67633 $
*                                                                            
*	Description:  User defined global variables, pin groups, and #defines used for this specific product.  
*                                                                            
*   Template Revision History                                                    
*		20110919 MWB: Initial Development and Release		
*		20121218 MWB: Added sample global SiteVars.  Added additional comments.
*
*   User Revision History                                                    
*                                                                            
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/

/**********************************************************************************************************************************************For*Doxygen**//**
@file	Globals.cpp
@brief	User defined global variables, pin groups, functions, etc.  
**************************************************************************************************************************************************************/

#include "AP89_FT.h"


//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// {		define global variables that are declared as extern in Globals.h (Edit this section if required)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	///Declare the external variables here.  Define and initializes them in Globals.cpp

int OTP_3DV_AD = 0x78; //0xF0 OTP register address for I2C communication
int TM_3DV_AD =  0x50;//0xA0; //biptm address for I2C communication


//Device I2C ADDR
int AP89_AD = 96;

//OTP Reserved Control Registers
int OTPMODE_REG = 0xF0;
int PGMREG_REG = 0xF1;
int SOFTPOR_REG = 0xF2;
int PGMSTATUS_REG = 0xF3;
int LOCKSTATUS_REG = 0xF4;
int OTPNOCHK_REG = 0xF5;
int UNLOCK1_REG = 0xF6;
int UNLOCK2_REG = 0xF7;
int REVISION_REG = 0xF8;

//OTP Trim data Registers
int OTP_BG_REG= 0x00;
int OTP_REF_REG= 0x01;
int OTP_LDO_REG = 0x02;
int OTP_OUT5_REG = 0x02;
int OTP_BIAS_REG = 0x03;
int OTP_OSC_REG = 0x04;
int OTP_SLOPE_REG = 0x05;
int OTP_OUT_GAIN_REG = 0x06;
int OTP_OSC20M_REG = 0x07;
int OTP_OUT3_REG = 0x08;
int OTP_FUNC0_REG = 0x09;
int OTP_FUNC1_REG = 0x0A;
int OTP_BSTOSC_Bit = 0;


//OTP Trim Codes
SiteInt g_BG_code;
SiteInt g_REF_code;
SiteInt g_LDO_code;
SiteInt g_BIAS_code;
SiteInt g_OSC_code;
SiteInt g_OSC20M_code;
SiteInt g_SLOPE_code;
SiteInt g_OUT_GAIN_code;
SiteInt g_SPARE_code;
SiteInt g_OUT1_code;
SiteInt g_OUT2_code;
SiteInt g_OUT3_code;
SiteInt g_OUT4_code;
SiteInt g_OUT5_code;
SiteInt g_OUT6_code;
SiteInt g_FUNC0_code;
SiteInt g_FUNC1_code;
SiteInt ecc_word1, ecc_word2;
int g_DIM_code;
bool gbOUTGainPre;
SiteInt gsiBSTUV;

PinListData gpldOUTx_Pre[6];
PinListData gpldOUTx_Post[6];
PinListData gpldBSTMON_UV_BL;

SiteInt gsiReadBack;

int K32 = 1000;
int K33 = 1001;
int K34 = 1002;
int K35 = 1003;
int K36 = 1004;
int K37 = 1005;
int K38 = 1006;
int K39 = 1007;
int K40 = 1008;
int K41 = 1009;
int K42 = 1010;
int K43 = 1011;
int K44 = 1012;
int K45 = 1013;
int K46 = 1014;
int K47 = 1015;
int K48 = 1016;
int K49 = 1017;
int K50 = 1018;
int K51 = 1019;
int K52 = 1020;
int K53 = 1021;
int K54 = 1022;
int K55 = 1023;
int K56 = 1024;
int K57 = 1025;
int K58 = 1026;
int K59 = 1027;
int K60 = 1028;
int K61 = 1029;
int K62 = 1030;
int K63 = 1031;


bool AP89 = true;
bool gbLastContact;
bool gbLastSupply;
bool gbContactFail;
bool gbSkipPowerDown;

int giLED_CH;


int	giRandomBatchCode;
bool gbSample;


double	gdAWGStart;
double	gdAWGStop;
double	gdAWGStepSize;
int giAWGVectorSize;

bool gbOUT_COMP_TH;


bool bREF_Check = false;

//init Hamming(15,11) ECC generator matrix; Code From AP66
int Gp_11_15[11][15] =
{
	1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,
	0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,
	1,1,0,0,0,0,0,1,0,0,1,0,0,0,0,
	1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
	0,1,0,0,0,0,0,1,0,1,0,0,0,0,0,
	1,1,0,1,0,0,1,0,0,0,0,0,0,0,0,
	0,1,0,1,0,0,0,1,0,0,0,0,0,1,0,
	1,1,0,1,0,0,0,1,0,0,0,0,0,0,1,
	1,0,0,1,0,0,0,1,0,0,0,0,1,0,0,
	1,0,0,0,0,0,0,1,1,0,0,0,0,0,0
};
//Hamming parity matrix 
int H_11_15[4][15] = 
{
	1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
	0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,
	0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,
	0,0,0,0,0,0,0,1,1,1,1,1,1,1,1
};


void Code11Bits(SiteInt& word1, SiteInt& word2) //Code from AP66
{

	//This function performs Hamming(15,11) ecc code conversion for ecc otp registers
	//it takes one 8 bit result and one 3 bit result which are in two different registers 
	//and combines them and computes the required 4 parity bits to form the new 15 bit encoded result.
	//15 bit result ->>> p1,p2,d1,p4,d5,d2,d7,p8,d11,d6,d4,d3,d10,d8,d9
	//original 8 bit result ->>> d8,d7,d6,d5,d4,d3,d2,d1 = 8 bit register
	//original 3 bit result ->>> d11,d1,d9 = 3 bit register
	//p1,p2,p4,p8= parity bits

	int i,j;
	int uncoded11bits[11];
	int coded15bits[15];

	for( int isite=0; isite<NUM_SITES; isite++)
	{
		uncoded11bits[0] = word1[isite] & 0x01;
		uncoded11bits[1] = (word1[isite] & 0x02)>>1;
		uncoded11bits[2] = (word1[isite] & 0x04)>>2;
		uncoded11bits[3] = (word1[isite] & 0x08)>>3;
		uncoded11bits[4] = (word1[isite] & 0x10)>>4;
		uncoded11bits[5] = (word1[isite] & 0x20)>>5;
		uncoded11bits[6] = (word1[isite] & 0x40)>>6;
		uncoded11bits[7] = (word1[isite] & 0x80)>>7;
		uncoded11bits[8] = (word2[isite] & 0x01);
		uncoded11bits[9] = (word2[isite] & 0x02)>>1;
		uncoded11bits[10] = (word2[isite] & 0x04)>>2;

		for(i=0; i<15; i++)
		{
			coded15bits[i] = 0;
			for(j=0; j<11; j++)
			{
				coded15bits[i] += uncoded11bits[j] * Gp_11_15[j][i];
			}
			coded15bits[i] = coded15bits[i] % 2;
		}

		word1[isite] = coded15bits[0] + 2*coded15bits[1] + 4*coded15bits[2] + 8*coded15bits[3] + 16*coded15bits[4] + 32*coded15bits[5] + 64*coded15bits[6] + 128*coded15bits[7];
		word2[isite] = coded15bits[8] + 2*coded15bits[9] + 4*coded15bits[10] + 8*coded15bits[11] + 16*coded15bits[12] + 32*coded15bits[13] + 64*coded15bits[14];

	}

}


	namespace DUT
{
	//{{CG_DUT { Sample Code 

	//PinListData gpldResult;	//Global PinListData 
	Maxim::System::CSharedPinGroups SPUShareGroup;

	//}}CG_DUT  } Sample Code
}


//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// }		define global variables
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+

/************************************************************************************************************************************************************For*Doxygen**//**
Initialize any global variables in the program.

@return		true if successful.  Returning false will abort Entry into the Test/Quit prompt, and return to Main Menu 
@remarks
		This function is called from the OnTestInit function in the main CPP file.  See ETS Help file "TestMain Functions" for more information.
		Initialize any global variables that need to be setup before testing starts.  This includes global PinListData, SiteBool, and SiteVar
		variables which need to be initialized after UserInit is called and NUM_SITES is known.
		In general, it is best to avoid global variables whenever possible in favor of local variables to make the program easier to follow and to 
		avoid the complications of initialization.
		Instead of using a global PinListData to pass around measurement results that have been datalogged, use the Maxim::Results::GetResults function.
**************************************************************************************************************************************************************/
bool Maxim_OnTestInit()
{//{{CG_OnTestInit

	// { Sample Code 
	//gsbLockBit = SiteBool(TRUE);	//Initialize gsbLockBit to true
	//gsbLockBit = SiteBool();		//Initialize gsbLockBit to default value (FALSE)

	//gsiBGRegCode = SiteInt();		//Initialize gsiBGRegCode to default value (0)
	//gsiIBiasRegCode = SiteInt(5);	//Initialize gsiIBiasRegCode to 5
	//gsiBGRegCode = gsiIBiasRegCode + 3;	//Once globals are initialized, the standard methods can be used.

	//gpldResult = PinListData();	//Initialize a PLD.  Often, global PLDs are not required since the Maxim::Results::GetResults function can be used instead.


	DUT::Trim::InitializeTrims();


	apu12awgloader( EN, "BSTMON_UV_BL", APU12_FV, APU12_10V, APU12_1MA, BSTMON_UV_BL_Data, (sizeof(BSTMON_UV_BL_Data)/sizeof(double)), &BSTMON_UV_BL_Regions );
	gbLastContact = false;
	gbLastSupply = false;

	return true;
}//}}CG_OnTestInit

/************************************************************************************************************************************************************For*Doxygen**//**
Reinitialize any global variables in the program.

@returns	Return true if successful.  Returning false will abort Program flow execution and return to Test/Quit prompt.

@remarks:
		This function is called from the OnTestStartup function in the main CPP file.  See ETS Help file "TestMain Functions" for more information.
		Reset any global variables that need to be setup before each test insertion.  
		In general, it is best to avoid global variables whenever possible in favor of local variables to make the program easier to follow and to 
		avoid the complications of reinitialization.
**************************************************************************************************************************************************************/
bool Maxim_OnTestStartup()
{//{{CG_OnTestStartup

	return true;
}//}}CG_OnTestStartup

/************************************************************************************************************************************************************For*Doxygen**//**
Run user supplied code in the OnTestExit function.

@returns	Return true if successful.	Return false to exit out to eShell.

@remarks	This function is called from the OnTestExit function in the main CPP file.  See ETS Help file "TestMain Functions" for more information.
**************************************************************************************************************************************************************/
bool Maxim_OnTestExit()
{//{{CG_OnTestExit

	return true;
}//}}CG_OnTestExit


/************************************************************************************************************************************************************For*Doxygen**//**
Add code to declare Pin Groups here.  This function is called in UserInit.

@returns	Return true if successful.	Returning false will cause the program to abort loading.

@remarks	Add any required global pins groups that need to be created in code to this function.
			As of MST Rev 2011A and possibly 2011B, bugs in the Pin Map Editor require some pin groups to be created in code.
			For all other pin groups, best practice is to add the groups in the Pin Map Editor.
**************************************************************************************************************************************************************/
bool Create_Pin_Groups()
{
	//{{CG_Create_Pin_Groups
	// { Sample Code
#ifdef MAXIM_SAMPLE_CODE_ACTIVATE
	//Create group for SPU pins
	groupset( SPU_GROUP, "SPU_GROUP", "SP112_6, SP112_7" );	//Example code to declare a pin group for SPU pins.  
															//SPU Pins should be declared as "No Site" on the Pin Map.
															//Due to bugs in the MST Pin Map Editor, SPU groups cannot be defined in the Pin Map 
															//and be used with the Connect_SPU_Pin_To_Site function.  See sample code in Hardware_Checker.cpp for 
															//recommended best practice for sharing the SPU pins between sites.  
															//This pin definition is recommended for all Maxim products which need to share the SPU channels 
															//between sites.  (At least until the MST bug is fixed.)
	//Create groups for TMU pins
	//groupset( TMU_S0_S2_GS, "TMU_S0_S1", "QTMU%d, QTMU%d", POK_TMU[0], POK_TMU[2]);	//Example code to declare a pin group for QTMU pins
	//groupset( TMU_S1_S3_GS, "TMU_S2_S3", "QTMU%d, QTMU%d", POK_TMU[1], POK_TMU[3]);
#endif
	// } Sample Code


	return true;
	//}}CG_Create_Pin_Groups
}