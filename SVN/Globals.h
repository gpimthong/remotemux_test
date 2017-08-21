/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*                                                                            
*	Source title:		Globals.h                                    
*		Created from Maxim Template                         
*       Template Written and maintained by:								 
*			Patrick Hayes (Patrick.Hayes@MaximIntegrated.com)	                 
*			Mark Bruening (Mark.Bruening@MaximIntegrated.com)                      
*		  
*	URL:				$URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/Globals.h $
*   Last Modified by:	$LastChangedBy: govit.pimthong $                                       
*	Date:				$Date: 2017-08-03 22:36:37 +0700 (Thu, 03 Aug 2017) $                                                
*   Current Rev:		$Revision: 67633 $
*                                                                            
*	Description:  User defined global variables, pin groups, and #defines used for this specific product.  
*                                                                            
*   Template Revision History                                                    
*		20110919 MWB: Initial Development and Release		
*
*   User Revision History                                                    
*                                                                            
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/

/***********************************************************************************************************************************************For*Doxygen**//**
@file Globals.h
@brief	User defined global variables, pin groups, and # defines used for this specific product.  
**************************************************************************************************************************************************************/

//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// {		#Defines  (Edit this section if required)
//				Note: The "DieType.h" (i.e. PX01.h) has #defines that in a limited number of circumstances  provide useful library compile options/features
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	
	#define	MAXIM_USE_OFFLINE_ANSWERS	//Uncomment for offline development.  For extra safety, comment this out for production.
										//When on a tester, this setting has no effect (tester measurements will be used) 
										//Offline answers are taken from the Datasheet "Offline_Answer" column

	//#define MAXIM_SAMPLE_CODE_ACTIVATE  // Uncomment this line if you want to run the sample template code.  Normally this line should be commented out.
	//#define GALAXY_PAT
	#define MAXIM_SKIP_HWCMESSAGE //Uncomment this if you wish to skip the HW Checker "Insert HW Checker" popup.

	//Sample Code
	#ifdef MAXIM_SAMPLE_CODE_ACTIVATE
		#define SPU_GROUP 20000		//Used to declare the SPU_GROUP of pins in Globals.cpp.
	#endif
	

//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// }		#Defines 
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// {		declare extern (global) variables  (Edit this section if required)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	//Declare the external variables here.  Define and initializes them in Globals.cpp

extern int OTP_3DV_AD;
extern int TM_3DV_AD;


//Device I2C ADDR
extern int AP89_AD;

//OTP Reserved Control Registers
extern int OTPMODE_REG;
extern int PGMREG_REG;
extern int SOFTPOR_REG;
extern int PGMSTATUS_REG;
extern int LOCKSTATUS_REG;
extern int OTPNOCHK_REG;
extern int UNLOCK1_REG;
extern int UNLOCK2_REG;
extern int REVISION_REG;

//OTP Trim data Registers
extern int OTP_BG_REG;
extern int OTP_REF_REG;
extern int OTP_LDO_REG;
extern int OTP_BIAS_REG;
extern int OTP_OSC_REG;
extern int OTP_SLOPE_REG;
extern int OTP_OUT_GAIN_REG;
extern int OTP_OSC20M_REG;
extern int OTP_OUT3_REG;
extern int OTP_OUT5_REG;
extern int OTP_FUNC0_REG;
extern int OTP_FUNC1_REG;
extern int OTP_BSTOSC_Bit;

//OTP Trim Codes
extern SiteInt g_BG_code;
extern SiteInt g_REF_code;
extern SiteInt g_LDO_code;
extern SiteInt g_BIAS_code;
extern SiteInt g_OSC_code;
extern SiteInt g_OSC20M_code;
extern SiteInt g_SLOPE_code;
extern SiteInt g_OUT_GAIN_code;
extern SiteInt g_SPARE_code;
extern SiteInt g_OUT1_code;
extern SiteInt g_OUT2_code;
extern SiteInt g_OUT3_code;
extern SiteInt g_OUT4_code;
extern SiteInt g_OUT5_code;
extern SiteInt g_OUT6_code;
extern SiteInt g_FUNC0_code;
extern SiteInt g_FUNC1_code;
extern SiteInt ecc_word1, ecc_word2;
extern SiteInt gsiReadBack;
extern int g_DIM_code;
extern bool gbOUTGainPre;
extern SiteInt gsiBSTUV;

extern PinListData gpldOUTx_Pre[6];
extern PinListData gpldOUTx_Post[6];
extern PinListData gpldBSTMON_UV_BL;

extern int K32;
extern int K33;
extern int K34;
extern int K35;
extern int K36;
extern int K37;
extern int K38;
extern int K39;
extern int K40;
extern int K41;
extern int K42;
extern int K43;
extern int K44;
extern int K45;
extern int K46;
extern int K47;
extern int K48;
extern int K49;
extern int K50;
extern int K51;
extern int K52;
extern int K53;
extern int K54;
extern int K55;
extern int K56;
extern int K57;
extern int K58;
extern int K59;
extern int K60;
extern int K61;
extern int K62;
extern int K63;

extern bool AP89;
extern bool gbLastContact;
extern bool gbLastSupply;
extern bool gbContactFail;
extern bool gbSkipPowerDown;

extern int giLED_CH;

extern int	giRandomBatchCode;
extern bool gbSample;

extern void Code11Bits(SiteInt& word1, SiteInt& word2);

extern bool gbOUT_COMP_TH;
extern bool bREF_Check;

extern double	gdAWGStart;
extern double	gdAWGStop;
extern double	gdAWGStepSize;
extern int giAWGVectorSize;

/************************************************************************************************************************************************************For*Doxygen**//**
@brief Namespace to be used by the test engineer to declare functions, variables, etc. that are specific to their product.
	//{{CG_DUT { Sample Code 

@remarks	
			User functions can be easily found using Intellisense if you put them in the DUT namespace.
			An example of something you may put in the namespace is

			@code
			// { Sample Code 
			extern PinListData gpldResult;	//Global PinListData 

			@endcode

*************************************************************************************************************************************************************/
namespace DUT
{

	extern Maxim::System::CSharedPinGroups SPUShareGroup;

}

//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// }		declare extern (global) variables
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+








//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// {		Maxim Template  (Do NOT edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
bool Maxim_OnTestInit();
bool Maxim_OnTestStartup();
bool Maxim_OnTestExit();
bool Create_Pin_Groups();
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// }		Maxim Template  (Do NOT edit this section)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+