//DO NOT EDIT THIS FILE
/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*                                                                            
*	Source title:		Maximlib_SiteVar.h                                                                           		  
*	URL:				$URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/MaximLib/MaximLib_SiteVar.h $
*   Last Modified by:	$LastChangedBy: govit.pimthong $                                       
*	Date:				$Date: 2017-03-09 12:55:55 +0700 (Thu, 09 Mar 2017) $                                                
*   Current Rev:		$Revision: 61735 $
*                                                                            
*	Description:
*		Maxim library header file for Template Class SiteDouble
*		Maxim library written and maintained by:								 
*			Patrick Hayes (Patrick.Hayes@MaximIntegrated.com)	                 
*			Mark Bruening (Mark.Bruening@MaximIntegrated.com)                                                                            
*
*   Revision History                                                    
*                                                                            
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
//DO NOT EDIT THIS FILE

/**********************************************************************************************************************************************For*Doxygen**//**
@file	Maximlib_SiteVar.h
@brief	Maxim library header file for Template Class SiteDouble
**************************************************************************************************************************************************************/

#ifndef MAXIMLIB_SiteVar_H
#define MAXIMLIB_SiteVar_H

	class SiteInt;
	class SiteDouble;

	class SiteBool
	{
		public:
			SiteBool();					//Default constructor
			SiteBool(const BOOL bValue);	//construct a SiteBool vector from a BOOL
			SiteBool(const SiteBool &sbCopy);	//Copy constructor
			~SiteBool();			

			SiteBool & operator=(const SiteBool Val);	//copy another SiteBool
			void operator=(const BOOL bValue);			//Set SiteBool to a constant value

			BOOL & SiteBool::operator[](const unsigned iSite);					//Set the value of a specific site to a BOOL.		sbVal[site] = FALSE;
			const bool SiteBool::operator[] (const unsigned iSite) const; 		//Return the value from a specific site.			bool bVal = sbVal[site]; 
																				//Returns a bool (not BOOL) to enable safe (bVal == TRUE) comparison.


			//operators.
			bool operator==(const SiteBool& right) const;		//Returns true if each site in the left SiteBool has the same value as each site in the right SiteBool.  Returns a bool (not BOOL) to enable safe (bVal == TRUE) comparison.
			bool operator!=(const SiteBool& right) const;		//Returns true if each site in the left SiteBool has the opposite value as each site in the right SiteBool.  Returns a bool (not BOOL) to enable safe (bVal == TRUE) comparison.
			SiteBool operator~();						//Logical Invert
			SiteBool operator&(const SiteBool& right);	//Logical AND
			SiteBool operator|(const SiteBool& right);	//Logical OR
			SiteBool operator^(const SiteBool& right);	//Logical XOR

			BOOL All(const BOOL bCheck);	//Returns true if all sites match bCheck
			BOOL Any(const BOOL bCheck);	//Returns true if any sites match bCheck

			
			unsigned int size() const;	//Size of SiteBool (Number of Sites)
			void resize(int size);	//ReSize of SiteBool (Number of Sites)
		private:
			vector<BOOL> vecBool;

	};

	
	class SiteDouble 
	{
		public:
			friend class SiteInt;
			SiteDouble();					//Default constructor
			SiteDouble(const double dValue);	//construct a SiteDouble vector from an T
			SiteDouble(const SiteDouble &sdCopy);	//Copy constructor
			SiteDouble(const SiteInt siVal); //cast SiteInt to SiteDouble
			
			~SiteDouble();			

			SiteDouble & operator=(const SiteDouble Val);		//copy another SiteDouble
			

			double & operator[] (const unsigned iSite);	//Set the value of a specific site to an T.  siVal[site] = 5;
			const double operator[] (const unsigned iSite) const;	//Return the value from a specific site.   T iVal = siVal[site];  
			//operator SiteDouble() const; //cast SiteDouble<int> to SiteDouble<double>
			
		

			
			SiteDouble operator+(const SiteDouble& right) const;
			SiteDouble operator-(const SiteDouble& right) const;
			SiteDouble operator*(const SiteDouble& right) const;
			SiteDouble operator/(const SiteDouble& right) const;
			SiteDouble operator+(const SiteInt& right) const;
			SiteDouble operator-(const SiteInt& right) const;
			SiteDouble operator*(const SiteInt& right) const;
			SiteDouble operator/(const SiteInt& right) const;
			SiteDouble operator+(const int& right) const;
			SiteDouble operator-(const int& right) const;
			SiteDouble operator*(const int& right) const;
			SiteDouble operator/(const int& right) const;
			SiteDouble operator+(const double& right) const;
			SiteDouble operator-(const double& right) const;
			SiteDouble operator*(const double& right) const;
			SiteDouble operator/(const double& right) const;
			SiteDouble operator+=(const SiteDouble& right);
			SiteDouble operator-=(const SiteDouble& right);
			SiteDouble operator*=(const SiteDouble& right);
			SiteDouble operator/=(const SiteDouble& right);
			//TODO, Other operators as required
				//  +=   -=   *=   /=   ++   --   %   ^  &  |   
				//  <    >    ==   !=   <=   >=   

			unsigned int size() const;	//Size of SiteDouble (Number of Sites)
			void resize(int size) ;	//Resize of SiteBool (Number of Sites)
			bool AreAllSitesEqual();
		private:
			vector<double> vecD;

	};
	
	//Math operators.
	SiteDouble operator+(const SiteInt& left,const SiteDouble& right);
	SiteDouble operator-(const SiteInt& left,const SiteDouble& right);
	SiteDouble operator*(const SiteInt& left, const SiteDouble& right);
	SiteDouble operator/(const SiteInt& left,const SiteDouble& right);
	SiteDouble operator+(const int& left,const SiteDouble& right);
	SiteDouble operator-(const int& left,const SiteDouble& right);
	SiteDouble operator*(const int& left, const SiteDouble& right);
	SiteDouble operator/(const int& left,const SiteDouble& right);
	SiteDouble operator+(const double& left,const SiteDouble& right);
	SiteDouble operator-(const double& left,const SiteDouble& right);
	SiteDouble operator*(const double& left, const SiteDouble& right);
	SiteDouble operator/(const double& left,const SiteDouble& right);

	//SiteDouble Math functions
	SiteDouble abs(const SiteDouble right);
	SiteDouble log(const SiteDouble right);
	SiteDouble log10(const SiteDouble right);
	

		class SiteInt 
	{
		public:
			SiteInt();					//Default constructor
			SiteInt(const int iValue);	//construct a SiteInt vector from an int
			SiteInt(const SiteInt &sdCopy);	//Copy constructor
			~SiteInt();			

			SiteInt & operator=(const SiteInt Val);		//copy another SiteInt
			//void operator=(const double TValue);			//Set SiteInt to a constant value
			

			int & operator[] (const unsigned iSite);	//Set the value of a specific site to an T.  siVal[site] = 5;
			const int operator[] (const unsigned iSite) const;	//Return the value from a specific site.   T iVal = siVal[site];  
			//operator SiteDouble() const; //cast SiteInt to SiteDouble
			
			//Math operators.
			SiteInt operator+(const SiteInt& right) const;
			SiteInt operator-(const SiteInt& right) const;
			SiteInt operator*(const SiteInt& right) const;
			SiteInt operator/(const SiteInt& right) const;
			
			SiteInt operator+(const int& right) const;
			SiteInt operator-(const int& right) const;
			SiteInt operator*(const int& right) const;
			SiteInt operator/(const int& right) const;

			//TODO, Other operators as required
				//  +=   -=   *=   /=   ++   --   %   ^  &  |   
				//  <    >    ==   !=   <=   >=   

			unsigned int size() const;	//Size of SiteInt (Number of Sites)
			void resize(int size) ;	//Resize of SiteBool (Number of Sites)
		private:
			vector<int> vecI;

	};

	SiteInt operator+(const int& left,const SiteInt& right);
	SiteInt operator-(const int& left,const SiteInt& right);
	SiteInt operator*(const int& left,const SiteInt& right);
	SiteInt operator/(const int& left,const SiteInt& right);

/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
//////////////////// SiteVar Helper functions
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/

	/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
	* Set_Error_Value
	*	Purpose:	Internal library helper function.  Returns an error value for cases where one is needed (divide by zero, site index out of range)                                    
	*	Arguments:	
	*		A variable (function is a template, so type can be as needed)
	*	Returns:
	*		Returns the largest number the type can represent.
	*	Remarks:	
	*		See notes in code
	*	Revision History:
	*		
	*
	**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
	template <typename T>
	void Set_Error_Value(T &VarType)
	{
		if (std::numeric_limits<T>::has_infinity) //The has_infinity check makes sure we are dealing with floating point numbers (single or double precision).
		{
			//infinity would make sense mathematically, but it is not datalogged correctly in MST 2012B.10 (datalogs in the STDF file as 0 and flags as a valid measurement)
			//VarType = std::numeric_limits<T>::infinity();	//+infinity		
			//Could use ::max instead, However, ::max causes an issue for floating types in Excel.  Floats are is stored as 3.40282366921e+038 in the STDF file, and read in as +1.#INF in GBD.  
			//But when it is time to output the result, Excel converts it to 65535 due to the bug explained at http://visualstudiomagazine.com/articles/2008/07/01/return-double-values-in-excel.aspx
			//VarType = (std::numeric_limits<T>::max)();		//largest possible number
			//float max divided by 10 could work. 
			//But it causes compiler warnings about converting from float to int since the compiler is checking both cases in the if statement.
			//VarType = (std::numeric_limits<float>::max)()/10;		//almost the largest possible floating point number.  
			//Solve the compiler warning by explicitly casting to the correct type.	
			VarType = (T) (std::numeric_limits<float>::max)()/10;
		}
		else
		{
			VarType = (std::numeric_limits<T>::max)();		//largest possible number
		}
	}




#endif
