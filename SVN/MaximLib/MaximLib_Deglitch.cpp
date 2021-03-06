//DO NOT EDIT THIS FILE
/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*                                                                            
*	Source title:		Maximlib_Deglitch.CPP                                                                            		  
*	URL:				$URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/MaximLib/MaximLib_Deglitch.cpp $
*   Last Modified by:	$LastChangedBy: govit.pimthong $                                       
*	Date:				$Date: 2017-03-09 12:55:55 +0700 (Thu, 09 Mar 2017) $                                                
*   Current Rev:		$Revision: 61735 $
*                                                                            
*	Description:  
*		Maxim library file for Deglitch
*		Maxim library written and maintained by:								 
*			Daniele Ghezzi(Daniele.Ghezzi@MaximIntegrated.com)
*			Patrick Hayes (Patrick.Hayes@MaximIntegrated.com)	                 
*			Mark Bruening (Mark.Bruening@MaximIntegrated.com)                                                                            
*
*   Revision History
*		
*                                                                            
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
//DO NOT EDIT THIS FILE

/**********************************************************************************************************************************************For*Doxygen**//**
@file	Maximlib_Deglitch.cpp
@brief	Maxim library file for Deglitch  
**************************************************************************************************************************************************************/


namespace Maxim
{
	namespace Deglitch
	{
		BOOL DA_enabled;

		void deglitch_Init(void)  // initialize Deglitch Assistant Driver
		{
			ULONG  io_mask;
			int ret;

			DA_enabled = GetUserSwitch(GPSW_Deglitch_Assist);
			if(DA_enabled)
			{
				// init handler port
				ret = mshinit();
				io_mask = 0xFFFE;
				ret = mshioset(io_mask);

				io_mask = 0x01;  // reset flip flop
				ret = mshiowrite(io_mask);
				io_mask = 0x0;
				ret = mshiowrite(io_mask);
			}
		}  


		inline void glitch_handling(void) // check if glitch has happened.  Function is suggested inline since it is called in so many places.
		{
			if(DA_enabled)
			{
				ULONG  io;
				io=mshioread();		// read flip flop, 19usec
				if ((io & 0x0004)) //  code to readback trigger info: true when trig is present
				{	
					DeglitchBreakpoint();  // call breakpoint user function 
					deglitch_Init(); // reset the glitch latch
				}
			}
		}
	}
}