/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*                                                                            
*	Source title:		GlobalTrims.h                                    
*		Created from Maxim Template                         
*       Template Written and maintained by:								 
*			Patrick Hayes (Patrick.Hayes@MaximIntegrated.com)	                 
*			Mark Bruening (Mark.Bruening@MaximIntegrated.com)                      
*		  
*	URL:				$URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/GlobalTrims.cpp $
*   Last Modified by:	$LastChangedBy: govit.pimthong $                                       
*	Date:				$Date: 2017-08-17 13:07:41 +0700 (Thu, 17 Aug 2017) $                                                
*   Current Rev:		$Revision: 68112 $
*                                                                            
*	Description:  User defined global variables, pin groups, and #defines used for trimming this specific product.  
*                                                                            
*   Template Revision History                                                    
*
*   User Revision History                                                    
*                                                                            
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/

/**********************************************************************************************************************************************For*Doxygen**//**
@file	GlobalTrims.cpp
@brief	Trim related user defined global variables, pin groups, functions, etc.  
**************************************************************************************************************************************************************/

#include "AP89_FT.h"

//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// {		define global trim variables that are declared as extern in GlobalTrims.h (Edit this section if required)
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
	//Declare the external trim variables here.  Define and initializes them in GlobalTrims.h
	namespace DUT
{

/************************************************************************************************************************************************************For*Doxygen**//**
@details Use the code below as a template to define items in GlobalTrims.cpp.  Note that these items should be declared in GlobalTrims.h.  
Definitions should start inside the "namespace Trim { }" area

@code
		//Declare an object for each trim that will use Maximfunclib_OTP routines
		Maxim::OTP::CTrimStructure gtsIBias; //Declare trim object

		//**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
		//	Create product and trim specific measurement routines.
		//		o	This function is called right after loading a trim code into the DUT.
		//		o	Frequently all that will be needed is a delay statement and a measurement statement.
		//		o	As needed, one can use a search routine or other more involved sequence.
		//*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****
		PinListData MeasureIBias (Maxim::OTP::CTrimStructure * tsTrim)
		{
			lwait ( 500 us_f);
			return apu12mi( OUT1, APU12_MI_1X, 500 , 13.0 );
		}

@endcode

**************************************************************************************************************************************************************/
	namespace Trim
	{
		// { Sample Code --- Note that these variables are being created in the DUT::Trim:: namespace
		SiteBool gsbLockBit;		//LockBit Status
		SiteBool gsbSPLockBit;		//Scratch Pad LockBit Status

		// } Sample Code

		//Declare an object for each trim that will use Maximfunclib_OTP routines

		//**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
		//	Create product and trim specific measurement routines.
		//		o	This function is called right after loading a trim code into the DUT.
		//		o	Frequently all that will be needed is a delay statement and a measurement statement.
		//		o	As needed, one can use a search routine or other more involved sequence.
		//*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****

		Maxim::OTP::CTrimStructure gtsBG;
		Maxim::OTP::CTrimStructure gtsREF;
		Maxim::OTP::CTrimStructure gtsLDO;
		Maxim::OTP::CTrimStructure gtsIBIAS;
		Maxim::OTP::CTrimStructure gtsOSC;
		Maxim::OTP::CTrimStructure gtsOSC20M;
		Maxim::OTP::CTrimStructure gtsSLOPE;
		Maxim::OTP::CTrimStructure gtsOUTGAIN;
		Maxim::OTP::CTrimStructure gtsOUT1;
		Maxim::OTP::CTrimStructure gtsOUT2;
		Maxim::OTP::CTrimStructure gtsOUT3;
		Maxim::OTP::CTrimStructure gtsOUT4;
		Maxim::OTP::CTrimStructure gtsOUT5;
		Maxim::OTP::CTrimStructure gtsOUT6;



		SiteInt OUTx_Send_Data(int iDeviceAddress, int iReg, SiteInt siData)
		{
			SiteInt siTempData;

			switch (iReg) {

			case 1 :

				ecc_word1 = siData;
				ecc_word2 = g_OUT4_code;

				ForEachActive_iSite{
					ecc_word1[iSite] |= (g_OUT2_code[iSite] << 3) | (OTP_BSTOSC_Bit << 7);
				}

				Code11Bits(ecc_word1, ecc_word2); //create 15 bit ecc coded results

				Maxim::I2C::Write(iDeviceAddress, OTP_FUNC0_REG, ecc_word1);
				Maxim::I2C::Write(iDeviceAddress, OTP_FUNC1_REG, ecc_word2);

				break;


			case 2 :

				ecc_word1 = g_OUT1_code;
				ecc_word2 = g_OUT4_code;

				ForEachActive_iSite{
					ecc_word1[iSite] |= (siData[iSite] << 3) | (OTP_BSTOSC_Bit << 7);
				}

				Code11Bits(ecc_word1, ecc_word2); //create 15 bit ecc coded results

				Maxim::I2C::Write(iDeviceAddress, OTP_FUNC0_REG, ecc_word1);
				Maxim::I2C::Write(iDeviceAddress, OTP_FUNC1_REG, ecc_word2);
				break;


			case 3 :

				siTempData = siData;

				ForEachActive_iSite{
					siTempData[iSite] |= (g_OUT6_code[iSite] << 3) & 0x08;
				}

				Maxim::I2C::Write(iDeviceAddress, 0x08, siTempData);
				break;



			case 4 :

				ecc_word2 = siData;

				ForEachActive_iSite{
					ecc_word1[iSite] = g_OUT1_code[iSite] | (g_OUT2_code[iSite] << 3) | (OTP_BSTOSC_Bit << 7);
				}

				Code11Bits(ecc_word1, ecc_word2); //create 15 bit ecc coded results

				Maxim::I2C::Write(iDeviceAddress, OTP_FUNC0_REG, ecc_word1);
				Maxim::I2C::Write(iDeviceAddress, OTP_FUNC1_REG, ecc_word2);
				break;


			case 6 :

				ForEachActive_iSite{
					siTempData[iSite] = g_OSC_code[iSite] | ((siData[iSite] << 4) & 0x20 );
				}

				Maxim::I2C::Write(iDeviceAddress, 0x04, siTempData);

				ForEachActive_iSite{
					siTempData[iSite] = g_SLOPE_code[iSite] | ((siData[iSite] << 3) & 0x20 );
				}

				Maxim::I2C::Write(iDeviceAddress, 0x05, siTempData);

				ForEachActive_iSite{
					siTempData[iSite] = g_OUT3_code[iSite] | ((siData[iSite] << 3) & 0x08 );
				}

				Maxim::I2C::Write(iDeviceAddress, 0x08, siTempData);

				break;


			}


			return 0;
		}




		PinListData MeasureBG (Maxim::OTP::CTrimStructure * tsTrim)
		{
			lwait (50 ms_f);
			//return apu12mv( COMP, 100) - apu12mv(CS, 100);
			return apu12mv( COMP, 100);
		}

		PinListData MeasureLDO (Maxim::OTP::CTrimStructure * tsTrim){
			lwait (10 ms_f);
			return apu12mv( COMP, 100);
		}


		PinListData MeasureIBIAS (Maxim::OTP::CTrimStructure * tsTrim){
			lwait (10 ms_f);
			return apu12mi(COMP, APU12_MI_1X, 10);
		}


		PinListData MeasureOSC (Maxim::OTP::CTrimStructure * tsTrim){

			PinListData pldResult;
			lwait (10 ms_f);

			//pldResult = MeasureTime(QTMU_GP, din1, din1, "AUTO", 50, NULL, "", 1 ms_f, QTMU_RCVH, QTMU_RCVH, QTMU_NEG, QTMU_NEG, 0, 1, 2.5 , 2.0, 500, 500);

			qtmuarm( QTMU_GP, "AUTO", 10, QTMU_EDGE, QTMU_POS );
			lwait( 1 ms_f );

			pldResult = qtmumt( QTMU_GP, QTMU_READFREQ, 5000);


			return pldResult * 8;

		}


		PinListData MeasureOSC20M (Maxim::OTP::CTrimStructure * tsTrim){

			PinListData pldResult;
			lwait (10 ms_f);

			//pldResult = MeasureTime(QTMU_GP, din1, din1, "AUTO", 50, NULL, "", 1 ms_f, QTMU_RCVH, QTMU_RCVH, QTMU_NEG, QTMU_NEG, 0, 1, 2.5 , 2.0, 500, 500);

			qtmuarm( QTMU_GP, "AUTO", 10, QTMU_EDGE, QTMU_POS );
			lwait( 1 ms_f );

			pldResult = qtmumt( QTMU_GP, QTMU_READFREQ, 5000);

			return 256 * pldResult;

		}

		PinListData MeasureSLOPE (Maxim::OTP::CTrimStructure * tsTrim){
			lwait (10 ms_f);
			return -1 * apu12mi(CS, APU12_MI_1X, 10);
		}

		PinListData MeasureOUTGAIN (Maxim::OTP::CTrimStructure * tsTrim){

			PinListData pldOUTxi[6], pldOUTavg;
			PME_INT OUT_PIN[6] = {OUT1, OUT2, OUT3, OUT4, OUT5, OUT6};



			for (int i = 0; i < 6; i++){

				apu12set(OUT_PIN[i], APU12_FV, 2.0, APU12_10V, APU12_200MA, APU12_PIN_TO_VI);
				lwait (5 ms_f);
				pldOUTxi[i] = apu12mi(OUT_PIN[i], APU12_MI_1X, 100);

				if(gbOUTGainPre)
					gpldOUTx_Post[i]  = gpldOUTx_Pre[i] = pldOUTxi[i];
				else
					gpldOUTx_Post[i] = pldOUTxi[i];

				apu12set(OUT_PIN[i], APU12_FV, 0.3, APU12_10V, APU12_200MA, APU12_PIN_TO_VI);

			}

			pldOUTavg = (pldOUTxi[0] + pldOUTxi[1] + pldOUTxi[2] + pldOUTxi[3] + pldOUTxi[4] + pldOUTxi[5]) / 6;

			gbOUTGainPre = false;

			return pldOUTavg;

		}

		PinListData MeasureOUT1 (Maxim::OTP::CTrimStructure * tsTrim){
			lwait (1 ms_f);
			return apu12mi(OUT1, APU12_MI_1X, 1000, 10);
		}
		PinListData MeasureOUT2 (Maxim::OTP::CTrimStructure * tsTrim){
			lwait (1 ms_f);
			return apu12mi(OUT2, APU12_MI_1X, 1000, 10);
		}
		PinListData MeasureOUT3 (Maxim::OTP::CTrimStructure * tsTrim){
			lwait (1 ms_f);
			return apu12mi(OUT3, APU12_MI_1X, 1000, 10);
		}
		PinListData MeasureOUT4 (Maxim::OTP::CTrimStructure * tsTrim){
			lwait (1 ms_f);
			return apu12mi(OUT4, APU12_MI_1X, 1000, 10);
		}
		PinListData MeasureOUT5 (Maxim::OTP::CTrimStructure * tsTrim){
			lwait (1 ms_f);
			return apu12mi(OUT5, APU12_MI_1X, 1000, 10);
		}
		PinListData MeasureOUT6 (Maxim::OTP::CTrimStructure * tsTrim){
			lwait (1 ms_f);
			return apu12mi(OUT6, APU12_MI_1X, 1000, 10);
		}


//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+
// }		define global variables
//-- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----+

/************************************************************************************************************************************************************For*Doxygen**//**

Initialize trim information for this program by editing this function in GlobalTrims.cpp.

@return	Return true if successful.

@details Use the sample code below as a template to load information that is specific to each of your trims

@code
		Maxim::OTP::sharedTrimInformation.AddTrimStepForAllTrims("FT_ROOM"); //Define Tests Steps where trimming can occur

		//Load Trim Information
		DUT::Trim::gtsIBias.LoadTrimInformation("IBias", 10 uA_f, 0.05 uA_f, uA, 0x31, 0x10);
	
		// Store trim table for the trim in an array.  
		//		Typically the trim table will be the deltas from code 0 of your trim.  Array element 5 corresponds to trim code 5.
		//		Code 0 is typically the first element of the array and would have value 0.
		//		The word typical is used, because there are options to do things differently if needed.
		double	dIBiasTrimTable[] = {0, 0.007185892, 0.01248583, 0.017746421, 0.022765875, 0.02756412, 0.032165118, 0.036675819};

		// Load the array into your trim object
		DUT::Trim::gtsIBias.LoadTrimTable(8, dIBiasTrimTable); 

		@note There are two scenarios for trims.  One where the trim table is in different units from what your measurement is.  For example, trim table
				may be in percentage of pretrim whereas measurement is in uA.  The first example below addresses this, the second example omits the 
				domain transfer function and assumes the trim table is provided in uA.

		//Specify communication function, trim function and if needed a domain function
		DUT::Trim::gtsIBias.LoadTrimFunctions(Maxim::I2C::Write, MeasureIBIAS, Maxim::OTP::DomainTransformOffsetPercentOfPretrim); 

		//If your trim table units are the same as measurement units then you can use the simplified call
		//DUT::Trim::gtsIBias.LoadTrimFunctions(Maxim::I2C::Write, MeasureIBIAS); 

		// Repeat these steps for each trim

@endcode

@attention Make sure DUT::Trim::InitializeTrims() is called in Maxim_OnTestInit() which can be found in Globals.cpp.  This is especially important
				if you started your program with an older template and then updated to a version supporting OTP.

@note Make sure to review both versions of Maxim::OTP::CTrimStructure::LoadTrimFunctions to see which is most appropriate for your situation.
		Typically you will use the 3 parameter version shown in the example above.
**************************************************************************************************************************************************************/
		bool InitializeTrims()
		{

			Maxim::OTP::sharedTrimInformation.AddTrimStepForAllTrims("FT_ROOM");
			Maxim::OTP::sharedTrimInformation.AddTrimStepForAllTrims("CHAR");


			g_BG_code = SiteInt(); 
			g_REF_code = SiteInt(); 
			g_LDO_code = SiteInt(); 
			g_BIAS_code	= SiteInt(); 
			g_OSC_code = SiteInt(); 
			g_OSC20M_code = SiteInt(); 
			g_SLOPE_code = SiteInt(); 
			g_OUT_GAIN_code = SiteInt(); 
			g_SPARE_code = SiteInt(); 
			g_OUT1_code	= SiteInt(); 
			g_OUT2_code	= SiteInt(); 
			g_OUT3_code	= SiteInt(); 
			g_OUT4_code	= SiteInt(); 
			g_OUT5_code	= SiteInt(); 
			g_OUT6_code	= SiteInt(); 
			g_FUNC0_code = SiteInt(); 
			g_FUNC1_code = SiteInt(); 
			ecc_word1  = SiteInt(); 
			ecc_word2  = SiteInt();

			double dOUT_Target = 50.7;

			//Load Trim Information
			DUT::Trim::gtsBG.LoadTrimInformation("BG", 1.210 V_f, 0.0008 V_f, V, OTP_3DV_AD, OTP_BG_REG);
			DUT::Trim::gtsREF.LoadTrimInformation("REF", 1.250 V_f, 0.0013 V_f, V, OTP_3DV_AD, OTP_REF_REG);
			DUT::Trim::gtsLDO.LoadTrimInformation("LDO", 5 V_f, 0.0125 V_f, V, OTP_3DV_AD, OTP_LDO_REG);
			DUT::Trim::gtsIBIAS.LoadTrimInformation("IBIAS", 10 uA_f, 0.07 uA_f, uA, OTP_3DV_AD, OTP_BIAS_REG);
			DUT::Trim::gtsOSC.LoadTrimInformation("OSC", 400, 5, KHz, OTP_3DV_AD, OTP_OSC_REG);
			DUT::Trim::gtsOSC20M.LoadTrimInformation("OSC20M", 20000, 400, MHz, OTP_3DV_AD, OTP_OSC20M_REG);
			DUT::Trim::gtsSLOPE.LoadTrimInformation("SLOPE", 50 uA_f, 1 uA_f, uA, OTP_3DV_AD, OTP_SLOPE_REG);
			DUT::Trim::gtsOUTGAIN.LoadTrimInformation("OUTGAIN", 121 mA_f, 2.0 mA_f, mA, OTP_3DV_AD, OTP_OUT_GAIN_REG);

			DUT::Trim::gtsOUT1.LoadTrimInformation("OUT1", dOUT_Target mA_f, 0.24 mA_f, mA, OTP_3DV_AD, 0x01);
			DUT::Trim::gtsOUT2.LoadTrimInformation("OUT2", dOUT_Target mA_f, 0.24 mA_f, mA, OTP_3DV_AD, 0x02);
			DUT::Trim::gtsOUT3.LoadTrimInformation("OUT3", dOUT_Target mA_f, 0.24 mA_f, mA, OTP_3DV_AD, 0x03);
			DUT::Trim::gtsOUT4.LoadTrimInformation("OUT4", dOUT_Target mA_f, 0.24 mA_f, mA, OTP_3DV_AD, 0x04);
			DUT::Trim::gtsOUT5.LoadTrimInformation("OUT5", dOUT_Target mA_f, 0.24 mA_f, mA, OTP_3DV_AD, OTP_OUT5_REG);
			DUT::Trim::gtsOUT6.LoadTrimInformation("OUT6", dOUT_Target mA_f, 0.24 mA_f, mA, OTP_3DV_AD, 0x06);


			DUT::Trim::gtsOUTGAIN.SetStartingTrimCode(2);


			if(!Maxim::Operator()){ //Disable Trim Optimization in engineering
				DUT::Trim::gtsBG.OptimizeStartingTrimCode(0);
				DUT::Trim::gtsREF.OptimizeStartingTrimCode(0);
				DUT::Trim::gtsLDO.OptimizeStartingTrimCode(0);
				DUT::Trim::gtsIBIAS.OptimizeStartingTrimCode(0);
				DUT::Trim::gtsOSC.OptimizeStartingTrimCode(0);
				DUT::Trim::gtsOSC20M.OptimizeStartingTrimCode(0);
				DUT::Trim::gtsSLOPE.OptimizeStartingTrimCode(0);
				DUT::Trim::gtsOUTGAIN.OptimizeStartingTrimCode(0);
				DUT::Trim::gtsOUT1.OptimizeStartingTrimCode(0);
				DUT::Trim::gtsOUT2.OptimizeStartingTrimCode(0);
				DUT::Trim::gtsOUT3.OptimizeStartingTrimCode(0);
				DUT::Trim::gtsOUT4.OptimizeStartingTrimCode(0);
				DUT::Trim::gtsOUT5.OptimizeStartingTrimCode(0);
				DUT::Trim::gtsOUT6.OptimizeStartingTrimCode(0);
			}




			double dBGTrimTable[64] =  
			{
				0,     0.00185144,     0.00367166,      0.0056118,     0.00745325,     0.00933343,      0.0111361,      0.0130288,
				0.0149527,      0.0167767,      0.0186306,      0.0205008,      0.0223498,      0.0242349,      0.0260851,      0.0279241,
				0.0297693,       0.031592,      0.0334772,      0.0353124,      0.0371713,      0.0390427,      0.0409204,      0.0427694,
				0.044682,       0.046506,      0.0483587,      0.0502151,      0.0520965,       0.053958,      0.0558194,      0.0576634,
				-0.0510596,     -0.0492044,      -0.047373,     -0.0454978,     -0.0436314,     -0.0417774,     -0.0398935,     -0.0380333,
				-0.0361069,     -0.0343229,     -0.0324427,     -0.0305838,     -0.0286999,     -0.0268347,      -0.024947,     -0.0231505,
				-0.0213091,     -0.0194464,     -0.0176087,      -0.015756,     -0.0138409,     -0.0120356,      -0.010158,    -0.00830027,
				-0.00635762,    -0.00455115,    -0.00266473,   -0.000810788,     0.00105815,     0.00295207,     0.00476353,     0.00665246

			};

			double dREFTrimTable[32] =  
			{
				0,     0.00276467,     0.00523701,     0.00793922,      0.0100105,       0.012714,      0.0151239,      0.0178598,
				0.0192902,      0.0220862,      0.0245298,      0.0272682,      0.0293707,       0.032078,      0.0344753,      0.0371788,
				-0.038999,     -0.0361019,     -0.0339269,     -0.0308487,      -0.029001,     -0.0260002,     -0.0238102,     -0.0208993,
				-0.0195838,     -0.0167892,     -0.0144293,     -0.0115584,    -0.00966574,    -0.00673741,    -0.00444247,    -0.00167405

			};

			double dLDOTrimTable[8] =  
			{
				0,       0.458614,       0.913124,        1.37104,       -1.80482,       -1.34621,        -0.8901,      -0.432181

			};

			double dIBIASTrimTable[64] =
			{
				0,         1.4805,        3.07162,        4.55496,         6.1092,        7.64076,        9.23471,        10.7436,
				12.2978,         13.767,        15.3723,        16.8783,        18.4581,         19.913,          21.49,        23.0159,
				24.7006,        26.2548,        27.7722,        29.3123,        30.8693,         32.364,        33.9495,        35.4555,
				36.9927,        38.5611,        40.0416,        41.5675,        43.1218,         44.659,        46.1735,        47.7335,
				-49.3019,       -47.7987,       -46.2104,       -44.7072,       -43.1445,       -41.6526,       -40.0785,       -38.5782,
				-37.0494,       -35.4952,       -33.9353,       -32.4264,       -30.8665,       -29.3718,       -27.7864,       -26.2491,
				-24.5956,       -23.0839,         -21.49,       -19.9783,       -18.4411,       -16.9095,       -15.3666,       -13.7925,
				-12.3063,         -10.84,       -9.20918,       -7.75137,       -6.15458,       -4.67124,       -3.08013,       -1.56843
			};


			double dOSCTrimTable[32] =
			{
				0,        2.22394,         3.9902,         6.2081,        7.82957,        9.95613,        11.7017,        13.9294,
				16.3064,        18.3859,        20.1451,        22.2946,        24.1133,        26.1523,         27.845,        30.0118,
				-30.9932,       -28.8352,       -27.0358,       -24.9226,        -23.102,       -21.0186,       -19.2918,       -17.1709,
				-14.7269,       -12.4176,       -10.8278,       -8.63547,       -6.85555,        -4.6755,       -3.07014,       -1.06344

			};

			double dOSC20MTrimTable[64] =
			{

				0,        2.02674,        4.22528,        6.36641,        8.81709,        11.1746,        13.8099,        16.5379,
				19.4115,        22.3928,         25.589,        28.8739,        32.5224,        36.1632,        40.1354,        44.1824,
				48.9685,        53.6329,        58.7923,        64.1502,        70.1815,        76.3024,         83.227,        90.4912,
				98.7837,        107.245,         116.77,        126.979,        139.007,        151.875,        166.855,        183.835,
				-38.9133,        -37.966,       -37.1165,       -36.2393,       -35.3991,       -34.5286,       -33.5695,       -32.6455,
				-31.6841,        -30.682,       -29.6774,       -28.6256,        -27.462,       -26.3861,       -25.1993,        -23.993,
				-22.7437,        -21.423,       -20.0263,       -18.7332,       -17.1986,        -15.746,       -14.1379,       -12.5975,
				-10.8411,       -9.23665,        -7.4253,       -5.66982,       -3.70772,       -1.83607,       0.234611,        2.27399,

			};



			double dSLOPETrimTable[32] =
			{

				0,        3.86465,        7.95802,        11.7347,        15.9395,        19.7455,        23.8154,        27.6742,
				31.5095,        35.3625,        39.4206,        43.2442,        47.4197,        51.3078,        55.3543,        59.2013,
				-59.606,       -55.7882,       -51.7242,       -47.8713,       -43.6547,       -39.9367,       -35.8609,       -32.0608,
				-28.1023,       -24.3022,       -20.2205,       -16.3676,       -12.1804,       -8.34507,       -4.25757,      -0.433967

			};

			double dOUTGAINTrimTable[4] =  
			{
				0,      -0.853112,       0.847065,     -0.0126977

			};


			double dOUT1TrimTable[8] =  
			{
				//0,       0.481116,       0.941679,        1.41221,       -1.39851,       -0.92175,      -0.461809,      0.0137017
				0,        0.47769,       0.940122,        1.40691,        -1.4611,      -0.982784,      -0.525958,     -0.0320744

			};
			double dOUT2TrimTable[8] =  
			{
				//0,       0.488386,       0.966804,        1.46235,       -1.46485,      -0.974279,      -0.498664,    -0.00467206
				0,       0.490566,       0.964001,        1.45581,       -1.45768,      -0.961198,      -0.489631,     -0.0143276
			};
			double dOUT3TrimTable[8] =  
			{
				0,       0.495521,       0.974224,        1.46196,        -1.4713,      -0.966438,      -0.496144,    -0.00436034

	
			};
			double dOUT4TrimTable[8] =  
			{
				//0,       0.481116,       0.941679,        1.41221,       -1.39851,       -0.92175,      -0.461809,      0.0137017
				 0,       0.508052,       0.993691,        1.49396,       -1.51824,       -1.01175,      -0.518014,    -0.00871659
			};
			double dOUT5TrimTable[8] =  
			{
				//0,       0.481116,       0.941679,        1.41221,       -1.39851,       -0.92175,      -0.461809,      0.0137017
				0,       0.495464,       0.968816,        1.45182,       -1.43625,      -0.958228,       -0.48363,     0.00716258
			};
			double dOUT6TrimTable[8] =  
			{
				0,       0.457504,       0.925908,        1.38871,       -1.45006,      -0.985704,      -0.533184,     -0.0719424

				//0,       0.458438,        -1.4429,      -0.984459,       0.919368,        1.38684,      -0.535987,     -0.0641564
			};




			// Load the array into your trim object
			DUT::Trim::gtsBG.LoadTrimTable(64, dBGTrimTable);
			DUT::Trim::gtsREF.LoadTrimTable(32, dREFTrimTable);
			DUT::Trim::gtsLDO.LoadTrimTable(8, dLDOTrimTable);
			DUT::Trim::gtsIBIAS.LoadTrimTable(64, dIBIASTrimTable);
			DUT::Trim::gtsOSC.LoadTrimTable(32, dOSCTrimTable);
			DUT::Trim::gtsOSC20M.LoadTrimTable(64, dOSC20MTrimTable);
			DUT::Trim::gtsSLOPE.LoadTrimTable(32, dSLOPETrimTable);
			DUT::Trim::gtsOUTGAIN.LoadTrimTable(4, dOUTGAINTrimTable);
			DUT::Trim::gtsOUT1.LoadTrimTable(8, dOUT1TrimTable);
			DUT::Trim::gtsOUT2.LoadTrimTable(8, dOUT2TrimTable);
			DUT::Trim::gtsOUT3.LoadTrimTable(8, dOUT3TrimTable);
			DUT::Trim::gtsOUT4.LoadTrimTable(8, dOUT4TrimTable);
			DUT::Trim::gtsOUT5.LoadTrimTable(8, dOUT5TrimTable);
			DUT::Trim::gtsOUT6.LoadTrimTable(8, dOUT6TrimTable);


			//Specify communication function, measure function and if needed a domain function
			DUT::Trim::gtsBG.LoadTrimFunctions(Maxim::I2C::Write, DUT::Trim::MeasureBG, Maxim::OTP::DomainTransformDefault);
			DUT::Trim::gtsREF.LoadTrimFunctions(Maxim::I2C::Write, DUT::Trim::MeasureBG, Maxim::OTP::DomainTransformDefault);
			DUT::Trim::gtsLDO.LoadTrimFunctions(Maxim::I2C::Write, DUT::Trim::MeasureLDO, Maxim::OTP::DomainTransformOffsetPercentOfPretrim);
			DUT::Trim::gtsIBIAS.LoadTrimFunctions(Maxim::I2C::Write, DUT::Trim::MeasureIBIAS, Maxim::OTP::DomainTransformOffsetPercentOfPretrim);
			DUT::Trim::gtsOSC.LoadTrimFunctions(Maxim::I2C::Write, DUT::Trim::MeasureOSC, Maxim::OTP::DomainTransformOffsetPercentOfPretrim);
			DUT::Trim::gtsOSC20M.LoadTrimFunctions(Maxim::I2C::Write, DUT::Trim::MeasureOSC20M, Maxim::OTP::DomainTransformOffsetPercentOfPretrim);
			DUT::Trim::gtsSLOPE.LoadTrimFunctions(Maxim::I2C::Write, DUT::Trim::MeasureSLOPE, Maxim::OTP::DomainTransformOffsetPercentOfPretrim);
			DUT::Trim::gtsOUTGAIN.LoadTrimFunctions(Maxim::I2C::Write, DUT::Trim::MeasureOUTGAIN, Maxim::OTP::DomainTransformOffsetPercentOfPretrim);

			DUT::Trim::gtsOUT1.LoadTrimFunctions(DUT::Trim::OUTx_Send_Data, DUT::Trim::MeasureOUT1, Maxim::OTP::DomainTransformDefault);
			DUT::Trim::gtsOUT2.LoadTrimFunctions(DUT::Trim::OUTx_Send_Data, DUT::Trim::MeasureOUT2, Maxim::OTP::DomainTransformDefault);
			DUT::Trim::gtsOUT3.LoadTrimFunctions(DUT::Trim::OUTx_Send_Data, DUT::Trim::MeasureOUT3, Maxim::OTP::DomainTransformDefault);
			DUT::Trim::gtsOUT4.LoadTrimFunctions(DUT::Trim::OUTx_Send_Data, DUT::Trim::MeasureOUT4, Maxim::OTP::DomainTransformDefault);
			DUT::Trim::gtsOUT5.LoadTrimFunctions(Maxim::I2C::Write, DUT::Trim::MeasureOUT5, Maxim::OTP::DomainTransformDefault);
			DUT::Trim::gtsOUT6.LoadTrimFunctions(DUT::Trim::OUTx_Send_Data, DUT::Trim::MeasureOUT6, Maxim::OTP::DomainTransformDefault);

			return true;
		}


	} // Trim namespace
} // DUT namespace
