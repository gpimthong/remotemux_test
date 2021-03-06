//DO NOT EDIT THIS FILE
/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*                                                                            
*	Source title:		Maximlib_DPIN.h                                                                           		  
*	URL:				$URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/MaximLib/MaximLib_DPIN.h $
*   Last Modified by:	$LastChangedBy: govit.pimthong $                                       
*	Date:				$Date: 2017-03-09 12:55:55 +0700 (Thu, 09 Mar 2017) $                                                
*   Current Rev:		$Revision: 61735 $
*                                                                            
*	Description:
*		Maxim library header file for DPIN
*		Maxim library written and maintained by:								 
*			Patrick Hayes (Patrick.Hayes@MaximIntegrated.com)	                 
*			Mark Bruening (Mark.Bruening@MaximIntegrated.com)                                                                            
*
*   Revision History                                                    
*                                                                            
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
//DO NOT EDIT THIS FILE

/**********************************************************************************************************************************************For*Doxygen**//**
@file	MaximLib_DPIN.h
@brief	Maxim library file for DPIN functions
**************************************************************************************************************************************************************/

#ifndef MAXIMLIB_DPIN_H
#define MAXIMLIB_DPIN_H

	PinListData Maximdpinmi		( LPCTSTR name, int samples, double delay, int site = -1 );
	PinListData Maximdpinmv		( LPCTSTR name, int samples, double delay, int site = -1 );
	int Maximdpinsysinit		( void );
	int Maximdpindisconnect     ( int site = -1, LPCTSTR name = NULL);
	int Maximdpinforce          ( LPCTSTR name, LPCTSTR force, double HiLevel = DPIN_IGNORE_LEVEL, double LoLevel = DPIN_IGNORE_LEVEL, int site = -1 );
	int Maximdpinforceint       ( LPCTSTR name, INT64 force, double HiLevel = DPIN_IGNORE_LEVEL, double LoLevel = DPIN_IGNORE_LEVEL, int site = -1 );
	int Maximdpingetcapturedata ( LPCTSTR name, int type, int count, PINT64 array, int site );
	int Maximdpingetfaildata    ( LPCTSTR name, DPIN_FAILSTR *failures, int steps, int site, int rev = DPIN_FAILSTR_REV );
	int Maximdpingetfaildataex  ( LPCTSTR name, DPIN_FAILSTR_EX *failures, int steps, int site, int rev = DPIN_FAILSTR_EX_REV );
	int Maximdpingetfailpins    ( LPCTSTR name, int mode, PINT64 failures, int site = -1 );
	int Maximdpinlevel          ( LPCTSTR name, LPCTSTR label, double DHi, double DLo, double RHi, double RLo, int site = -1 );
	int Maximdpinlevelset       ( LPCTSTR name, LPCTSTR label, double DHi, double DLo, double RHi, double RLo, double VTT = DPIN_IGNORE_LEVEL, int site = -1 );
	int Maximdpinppviset        ( LPCTSTR name, int mode, double value = 0.0, int irange = DPIN_2MA, int site = -1 );
	int Maximdpinrcvset         ( LPCTSTR name, int connect, double HiLevel = DPIN_IGNORE_LEVEL, double LoLevel = DPIN_IGNORE_LEVEL, int site = -1 );
	int Maximdpinshviset        ( LPCTSTR name, int mode, double value = 0.0, int vrange = DPIN_15V, int irange = DPIN_100MA, int site = -1 );
	int Maximdpinstrobe         ( LPCTSTR name, int type, PINT64 results, int site );
	int Maximdpinvicompare      ( LPCTSTR name, int type, PINT64 pResults, int vrange=DPIN_8V, double hilevel=DPIN_IGNORE_LEVEL, double lolevel=DPIN_IGNORE_LEVEL, int site=-1);
	int Maximdpinviset          ( LPCTSTR name, int mode, double value=0.0, int vrange=DPIN_8V, int irange=DPIN_32MA, double uvclamp=DPIN_CLAMP_OFF, double lvclamp=DPIN_CLAMP_OFF, int site=-1);

	int Maximdpinviset          ( LPCTSTR name, int mode, SiteDouble value=0.0, int vrange=DPIN_8V, int irange=DPIN_32MA, double uvclamp=DPIN_CLAMP_OFF, double lvclamp=DPIN_CLAMP_OFF, int site=-1);


#endif