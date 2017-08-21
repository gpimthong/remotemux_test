//DO NOT EDIT THIS FILE
/*** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-
*                                                                            
*	Source title:		Maximlib_DPAT.CPP                                                                            		  
*	URL:				$URL: https://svn.maxim-ic.com/svn/ets/test_programs/AP89/AP89_FT/Trunk/MaximLib/MaximLib_DPAT.cpp $
*   Last Modified by:	$LastChangedBy: govit.pimthong $                                       
*	Date:				$Date: 2017-03-09 12:55:55 +0700 (Thu, 09 Mar 2017) $                                                
*   Current Rev:		$Revision: 61735 $
*                                                                            
*	Description:  
*		Maxim library file for DPAT functions
*		Maxim library written and maintained by:								 
*			Patrick Hayes (Patrick.Hayes@MaximIntegrated.com)	                 
*			Mark Bruening (Mark.Bruening@MaximIntegrated.com)                                                                            
*
*   Revision History
*		See Maximlib.h file.
*                                                                            
**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****-**** **** **** **** **** **** **** **** **** **** **** **** **** **** **** ****/
//DO NOT EDIT THIS FILE

/**********************************************************************************************************************************************For*Doxygen**//**
@file	MaximLib_DPAT.cpp
@brief	Maxim library file for DPAT functions
**************************************************************************************************************************************************************/

#include <iostream>
#include <fstream>
#include <deque>
#include <map>
#include <algorithm>
#include <math.h>
#include <vector>
#include <string>
#include <direct.h>

enum DPAT_Algorithm{ DPAT_STD, DPAT_QUARTILE};

typedef struct {
	int dpat_test_number; //Store the testid of DPAT enabled tests, index will be the same for all info about that test
	char* dpat_test_name; //Store test name
	char* dpat_unit;
	SiteDouble dpat_low_limit; //Store low limit
	SiteDouble dpat_high_limit; // Store high limit
	double fixed_low_limit;
	double fixed_high_limit;
	int dpat_sample_size; //Store sample size for each dpat test
	double dpat_sigma_low; //Store sigma 
	double dpat_sigma_high;
	int dpat_window_size; //store window size for each DPAT test
	DPAT_Algorithm dpat_method; // Store DPAT Method type 
	SiteDouble dpat_value_queue; //Used to store data to be added if part passes
	SiteBool dpat_value_queue_valid; //keep track of validity of data
	deque<double> dpat_values[MAX_SITES]; //Used to store data for DPAT calculations   
	SiteDouble dpat_sum;
	SiteDouble dpat_sos;
	SiteDouble dpat_stdev;
	SiteDouble dpat_median;
	SiteDouble dpat_mean;
	bool activated;
} dpat_info;

void calc_IQR(dpat_info* dpat_temp, int iSite);
const int	DPAT_MIN_WINDOW_SIZE = 100;
const int   DPAT_MAX_WINDOW_SIZE = 250;
const int	DPAT_DEFAULT_SAMPLE_SIZE = 30;
const int	DPAT_MIN_SAMPLE_SIZE = 30;
const int	DPAT_MAX_SAMPLE_SIZE = 100;
const double	DPAT_DEFAULT_PARAMETER = 6.;
const int	DPAT_DEFAULT_WINDOW_SIZE = 100;
const bool	DPAT_DEBUG = FALSE;
const double DPAT_NO_LOW_LIMIT = DBL_MAX;
const double DPAT_NO_HIGH_LIMIT = DBL_MAX;
const int	DPAT_TESTNUM_OFFSET=	1000000000;
const int	DPAT_LOW_LIMIT_OFFSET = 100000000;
const int	DPAT_HIGH_LIMIT_OFFSET =200000000;

typedef vector<string> LINE;
static std::map<int,dpat_info> dpat_tests;
static bool DPAT_ON = FALSE;
static int DPAT_SAMPLE_BIN=1;
namespace Maxim
{
	namespace DPAT
	{

		void DPAT_enable(bool on_off)
		{
			//This will turn on DPAT testing and should be called in all programs that are going to use DPAT
			//FT (NOT QA)
			DPAT_ON = on_off;
			if (on_off)
				Maxim::etsprintfHOME("DPAT ENABLED\n");
		}

		void DPAT_set_sample_bin(int iBin)
		{
			DPAT_SAMPLE_BIN =iBin;
		}
		void DPAT_add_test(int testnum,char* method,int initial_samples,int window_size,double sigma_scalar_low,double sigma_scalar_high)
		{
			//This function adds DPAT testing for test proc_num.test_no.
			//Parameters are input and checked against min/max for parameters.
			map<int,dpat_info>::iterator dpat_it;
			dpat_info dpat_temp={};
			dpat_info* dpat_temp_ptr;

			//This adds DPAT test information and enables DPAT data collection
				if (initial_samples >= window_size || initial_samples < DPAT_MIN_SAMPLE_SIZE || initial_samples > DPAT_MAX_SAMPLE_SIZE)     
					{initial_samples = DPAT_DEFAULT_SAMPLE_SIZE;}
				if (window_size >  DPAT_MAX_WINDOW_SIZE || window_size < DPAT_MIN_WINDOW_SIZE)
					{window_size = DPAT_DEFAULT_WINDOW_SIZE;}
				if (sigma_scalar_low < 0 || sigma_scalar_low > 30.0) 
					{sigma_scalar_low = DPAT_DEFAULT_PARAMETER;}
				if (sigma_scalar_high < 0 || sigma_scalar_high > 30.0) 
					{sigma_scalar_high = DPAT_DEFAULT_PARAMETER;}
				else if (!(strcmp(method,"Stdev") || strcmp(method,"Quartile"))) 
				   {Maxim::etsprintfHOME("Warning:  DPAT_enable: Illegal Method.\n");}
				else
				{
					if (dpat_tests.count(testnum) ==0)
					{
						//doesn't exist so add
						dpat_tests[testnum]=dpat_temp;
					}
					else
					{
						Maxim::etsprintfHOME("Warning:  DPAT_enable: Test Number already added. Updating info.\n");
					}


						dpat_it=dpat_tests.find(testnum);
						dpat_temp_ptr=&dpat_it->second;

						(*dpat_temp_ptr).dpat_test_number=testnum;	
						(*dpat_temp_ptr).dpat_sample_size= initial_samples;
						(*dpat_temp_ptr).dpat_sigma_low = sigma_scalar_low;
						(*dpat_temp_ptr).dpat_sigma_high = sigma_scalar_high;
						(*dpat_temp_ptr).dpat_window_size=window_size;
						if (strstr("Stdev", method) != NULL )
							(*dpat_temp_ptr).dpat_method= DPAT_STD;
						else if (strstr("Quartile", method) != NULL)
							(*dpat_temp_ptr).dpat_method=DPAT_QUARTILE;
						else
							(*dpat_temp_ptr).dpat_method= DPAT_STD; //default
						(*dpat_temp_ptr).activated=false;
						//Resize SiteVars
						(*dpat_temp_ptr).dpat_low_limit.resize(Maxim::GetNumSites());
						(*dpat_temp_ptr).dpat_high_limit.resize(Maxim::GetNumSites());
						(*dpat_temp_ptr).dpat_stdev.resize(Maxim::GetNumSites());
						(*dpat_temp_ptr).dpat_sum.resize(Maxim::GetNumSites());
						(*dpat_temp_ptr).dpat_sos.resize(Maxim::GetNumSites());
						(*dpat_temp_ptr).dpat_median.resize(Maxim::GetNumSites());
						(*dpat_temp_ptr).dpat_mean.resize(Maxim::GetNumSites());
						(*dpat_temp_ptr).dpat_value_queue.resize(Maxim::GetNumSites());
						(*dpat_temp_ptr).dpat_value_queue_valid.resize(Maxim::GetNumSites());
						//Initialize variables
						(*dpat_temp_ptr).dpat_sum=0.0;
						(*dpat_temp_ptr).dpat_sos=0.0;
						Maxim::etsprintfHOME("\tTest Number: %d\n",testnum);
						Maxim::etsprintfHOME("\tMethod: %s\n", method);
						Maxim::etsprintfHOME("\tSample Size: %d\n", (*dpat_temp_ptr).dpat_sample_size);
						Maxim::etsprintfHOME("\tWindow Size: %d\n",(*dpat_temp_ptr).dpat_window_size);
						Maxim::etsprintfHOME("\tMethod Scalar Low: %f\n", (*dpat_temp_ptr).dpat_sigma_low);
						Maxim::etsprintfHOME("\tMethod Scalar High: %f\n", (*dpat_temp_ptr).dpat_sigma_high);
							
				}
		}

		void DPAT_add_test(int testnum,char* method,int initial_samples,int window_size,double sigma_scalar)
		{
			DPAT_add_test(testnum,method, initial_samples,window_size,sigma_scalar, sigma_scalar);
		}

		

		void DPAT_load_sheet(char* filename)
		{
			//This loads a DPAT sheet as defined in GBD.
			//filename is file in programs directory.
			//First entry is DPAT_X where X is the version number
			//DPAT_1: 2 empty rows, title row (,Test #,Test Name,Enable,Method,Initial Samples,Window Size,Sigma/IQR Scalar,Comments) 
			//then 1 test per row
			//DPAT_2: 2 empty rows, title row (,Test #,Test Name,Enable,Method,Initial Samples,Window Size,Sigma/IQR Scalar (low),Window Size,Sigma/IQR Scalar (high),Comments) 

			ifstream dpat_sheet;
			string line;
			int tests_added=0;
			int pos;
			vector<LINE> array;
			char filepath[256];
			bool two_sigmas = false;
	
			_getcwd(filepath,256);
			strcat_s(filepath,"\\");
			strcat_s(filepath,filename);

			//open file
			dpat_sheet.open(filepath,ifstream::in);

			//check to make sure no problems opening
			if (!dpat_sheet) 
			{
				cout << "Error opening " <<filepath <<endl;
				return; 
			}
			//load strings into array 
			cout << "Loading DPAT sheet....." <<endl;
			while (getline(dpat_sheet,line))
			{
				LINE ln;
				while((pos=(int)line.find(','))>=0)
				{
					string field=line.substr(0,pos);
					line=line.substr(pos+1);
					ln.push_back(field);
				}
				array.push_back(ln);
			}
	
			LINE& row=array[0];
			string& str=row[0];
	

			row = array[0];
			str = row[0];
			if (str.compare("DPAT_1")==0 || str.compare("DPAT_2")==0)
			{
				if (str.compare("DPAT_2")==0) 
					two_sigmas=true;
				for (size_t row_num=4; row_num < array.size(); row_num++)
				{
//					int proc_num=-1;
					int test_num=-1;
					bool enable_test = false;
					char method[20];
					char test_name[100];
					int samples =-1;
					int window_size = -1;
					double scalar = -1.0;
					double scalar_low = -1.0;
					double scalar_high = -1.0;
					bool complete=false;
			
					row=array[row_num];
					for (size_t col_num=1;col_num<row.size();col_num++)
					{
						str=row[col_num];
						switch (col_num)
						{
						case 1: 
							test_num=atoi(str.c_str());
							break;
						case 2: 
							strcpy_s(test_name,str.c_str());
							break;
						case 3:
							if (str.compare("On")==0)
								enable_test = true;
							else enable_test = false;
							break;
						case 4:
							strcpy_s(method,str.c_str());
							break;
						case 5:
							samples=atoi(str.c_str());
							break;
						case 6: 
							window_size=atoi(str.c_str());
							break;
						case 7:
							if (two_sigmas) 
								scalar_low = atof(str.c_str());	
							else
								{
								scalar=atof(str.c_str());
								complete=true;
								}
							break;
						case 8: 
							if (two_sigmas)
							{
								scalar_high = atof(str.c_str());
								complete=true;
							}
						}

					}
					if (complete && enable_test)
					{
						if (two_sigmas)
							DPAT_add_test(test_num,method,samples,window_size,scalar_low,scalar_high);
						else
							DPAT_add_test(test_num,method,samples,window_size,scalar);
					
						tests_added++;
					}
				}
			}
			if (tests_added == 0) 
				cout << "Error: No Tests Found" <<endl;
			else
				cout << "DPAT Sheet Loaded: " << tests_added << " Tests Found" << endl;

		
		}

		void DPAT_data(int iIndex, double value, int iSite)
		{
			//Passes data to DPAT information for processing when DPAT is called at end of program.
			map<int,dpat_info>::iterator dpat_it;
			int testnum;
			dpat_info* dpat_temp;
		
				if (! DPAT_ON) return;
				testnum=gdsiTestNumber[iIndex];

				if (dpat_tests.count(testnum) > 0) //DPAT test exists
				{
					//store data in value_queue for later.  Store high and low limit.  Store test name.
					dpat_it=dpat_tests.find(testnum);
					dpat_temp=&((*dpat_it).second);

					(*dpat_temp).dpat_value_queue[iSite]=value;
					(*dpat_temp).dpat_value_queue_valid[iSite]=TRUE;
					(*dpat_temp).dpat_test_name=Maxim::Limits::GetTestNamebyIndex(iIndex);
					(*dpat_temp).dpat_unit=Maxim::Limits::GetUnitsbyIndex(iIndex);
					(*dpat_temp).fixed_high_limit=Maxim::Limits::GetHiLimitbyIndex(iIndex);
					(*dpat_temp).fixed_low_limit=Maxim::Limits::GetLoLimitbyIndex(iIndex);
			
				}
		}

		void DPAT()
		{
			//This test function will datalog all DPAT results.
			//Datalog DPAT minimum sample met
			//Datalog each DPAT test enabled if sample is met - set fail bin if any test fails

			map<int,dpat_info>::iterator dpat_it;
//			int test_num=1;
			dpat_info* dpat_temp;
			int iSite=0;
			char test_name[128];
			bool calculate_now;
			SiteBool sbSiteFailed;
			int iDPAT_count=-1;
			bool bInitialSample=false;

			sbSiteFailed=FALSE;
	
			if (DPAT_ON && (!dpat_tests.empty()))	//run this only if DPAT is on
			{
				//loop through each dpat test and datalog if activated
				for (dpat_it=dpat_tests.begin() ; dpat_it != dpat_tests.end(); dpat_it++) 
				{
					iDPAT_count++;
					dpat_temp=&((*dpat_it).second);
					if ((*dpat_temp).activated) 
					{
						//loop through each site
						for (iSite=0; iSite<Maxim::GetNumSites();iSite++)
						{
							if (msSiteStat(iSite) && msBinValue(iSite)<=1 && (*dpat_temp).dpat_value_queue_valid[iSite])
							{
								//Datalog Value
								SetTestNumber(0);
								SetSubTestNumber(DPAT_TESTNUM_OFFSET+(*dpat_temp).dpat_test_number);
								//set test name, low limit, high limit, then test value_queue
								msLogData(iSite,(*dpat_temp).dpat_test_name,9.3,(*dpat_temp).fixed_low_limit,(*dpat_temp).fixed_high_limit,(*dpat_temp).dpat_value_queue[iSite],(*dpat_temp).dpat_unit);						

								//Datalog Low Limit
								strcpy_s(test_name,(*dpat_temp).dpat_test_name);
								strcat_s(test_name," Low Limit");
								SetTestNumber(0);
								SetSubTestNumber(DPAT_TESTNUM_OFFSET+(*dpat_temp).dpat_test_number+DPAT_LOW_LIMIT_OFFSET);
								msLogData(iSite,test_name,9.3,(*dpat_temp).fixed_low_limit,(*dpat_temp).fixed_high_limit,(*dpat_temp).dpat_low_limit[iSite],(*dpat_temp).dpat_unit);						
								if ((*dpat_temp).dpat_value_queue[iSite]<(*dpat_temp).dpat_low_limit[iSite]) //Fail based on low limit
								{
									sbSiteFailed[iSite]=true;
								}

								//Datalog High Limit
								strcpy_s(test_name,(*dpat_temp).dpat_test_name);
								strcat_s(test_name," High Limit");
								SetTestNumber(0);
								SetSubTestNumber(DPAT_TESTNUM_OFFSET+(*dpat_temp).dpat_test_number+DPAT_HIGH_LIMIT_OFFSET);
								msLogData(iSite,test_name,9.3,(*dpat_temp).fixed_low_limit,(*dpat_temp).fixed_high_limit,(*dpat_temp).dpat_high_limit[iSite],(*dpat_temp).dpat_unit);						

								if ((*dpat_temp).dpat_value_queue[iSite]>(*dpat_temp).dpat_high_limit[iSite])  //fail based on high limit
								{
									sbSiteFailed[iSite]=true;
								}

							}
						}
					}
					else
					{
						//Sample parts.
						bInitialSample=true;
					}
				}
				for (iSite=0; iSite<Maxim::GetNumSites();iSite++)
				{
					if (msSiteStat(iSite))
					{
						int iPass=1;
						if (sbSiteFailed[iSite])
						{
							iPass=0;
						}
						SetTestNumber(0);
						SetSubTestNumber(DPAT_TESTNUM_OFFSET);
						msLogData(iSite,"DPAT",9.3,1,1,iPass,"");						

						if (sbSiteFailed[iSite])
						{
							msFailSite(iSite);
							msFailBin(iSite,"10");
						}
					}
				}


		
				//move data and calculate, can we do this after bin?
				for (dpat_it=dpat_tests.begin() ; dpat_it != dpat_tests.end(); dpat_it++) //loop through each dpat test
				{
					dpat_temp=&((*dpat_it).second);
					//loop through each site
					for (iSite=0; iSite<Maxim::GetNumSites();iSite++)
					{
						if (msSiteStat(iSite) && msBinValue(iSite)<=1 && (*dpat_temp).dpat_value_queue_valid[iSite])
						{
							if (!(*dpat_temp).activated) //if not activated then apply data to all sites
								for (int iSite2 = 0 ; iSite2<Maxim::GetNumSites();iSite2++)
								{
									//move values to FIFO, if over window size recalc sum and sos
									(*dpat_temp).dpat_values[iSite2].push_back((*dpat_temp).dpat_value_queue[iSite]);
									(*dpat_temp).dpat_sum[iSite2]=(*dpat_temp).dpat_sum[iSite2]+(*dpat_temp).dpat_value_queue[iSite];
									(*dpat_temp).dpat_sos[iSite2]=(*dpat_temp).dpat_sos[iSite2]+(*dpat_temp).dpat_value_queue[iSite]*(*dpat_temp).dpat_value_queue[iSite];
									if ((int)(*dpat_temp).dpat_values[iSite2].size()>(*dpat_temp).dpat_window_size)
									{
										(*dpat_temp).dpat_sum[iSite2]=(*dpat_temp).dpat_sum[iSite2]-(*dpat_temp).dpat_values[iSite2].front();
										(*dpat_temp).dpat_sos[iSite2]=(*dpat_temp).dpat_sos[iSite2]-(*dpat_temp).dpat_values[iSite2].front()*(*dpat_temp).dpat_values[iSite2].front();
										(*dpat_temp).dpat_values[iSite2].pop_front();
									}
								}
							else  //apply data only to specific site calculations
							{
								//move values to FIFO, if over window size recalc sum and sos
								(*dpat_temp).dpat_values[iSite].push_back((*dpat_temp).dpat_value_queue[iSite]);
								(*dpat_temp).dpat_sum[iSite]=(*dpat_temp).dpat_sum[iSite]+(*dpat_temp).dpat_value_queue[iSite];
								(*dpat_temp).dpat_sos[iSite]=(*dpat_temp).dpat_sos[iSite]+(*dpat_temp).dpat_value_queue[iSite]*(*dpat_temp).dpat_value_queue[iSite];
								if ((int)(*dpat_temp).dpat_values[iSite].size()>(*dpat_temp).dpat_window_size)
								{
									(*dpat_temp).dpat_sum[iSite]=(*dpat_temp).dpat_sum[iSite]-(*dpat_temp).dpat_values[iSite].front();
									(*dpat_temp).dpat_sos[iSite]=(*dpat_temp).dpat_sos[iSite]-(*dpat_temp).dpat_values[iSite].front()*(*dpat_temp).dpat_values[iSite].front();
									(*dpat_temp).dpat_values[iSite].pop_front();
								}
							}
						}
					}
				} //end move data
				
				//calculate
				for (dpat_it=dpat_tests.begin() ; dpat_it != dpat_tests.end(); dpat_it++) //loop through each dpat test
				{
					dpat_temp=&((*dpat_it).second);
					//loop through each site
					
					for ( iSite=0; iSite<Maxim::GetNumSites();iSite++)
					{
						calculate_now=FALSE;
						//run calcs if 
						//1) activated, active,and queue valid
						//2) not activated and sample size met
						if ((msSiteStat(iSite) && msBinValue(iSite)<=1 && (*dpat_temp).dpat_value_queue_valid[iSite]&&(*dpat_temp).activated) || (!(*dpat_temp).activated&&(int)(*dpat_temp).dpat_values[iSite].size()>=(*dpat_temp).dpat_sample_size))
						{
							calculate_now=TRUE;
						}
						(*dpat_temp).dpat_value_queue_valid[iSite]=FALSE;
						if (calculate_now)
						{
							//check which method
							if ((*dpat_temp).dpat_method==DPAT_STD)
							{
								//mean = sum/count
								//stdev = squareroot(abs(sos -(mean*mean*count))/(count-1))
								(*dpat_temp).dpat_mean[iSite]=(*dpat_temp).dpat_sum[iSite]/(*dpat_temp).dpat_values[iSite].size();
								(*dpat_temp).dpat_stdev[iSite]=sqrt(fabs((*dpat_temp).dpat_sos[iSite] - (*dpat_temp).dpat_mean[iSite]*(*dpat_temp).dpat_mean[iSite]*(*dpat_temp).dpat_values[iSite].size())/((*dpat_temp).dpat_values[iSite].size() -1));
								(*dpat_temp).dpat_high_limit[iSite]=(*dpat_temp).dpat_mean[iSite] + (*dpat_temp).dpat_stdev[iSite]*(*dpat_temp).dpat_sigma_high;

								(*dpat_temp).dpat_low_limit[iSite]=(*dpat_temp).dpat_mean[iSite] - (*dpat_temp).dpat_stdev[iSite]*(*dpat_temp).dpat_sigma_low;

							}
							else if ((*dpat_temp).dpat_method==DPAT_QUARTILE)
							{
								calc_IQR(dpat_temp,iSite);
							}
							//add else for other methods
							
							if	((*dpat_temp).dpat_high_limit[iSite] > (*dpat_temp).fixed_high_limit) 
							{
								(*dpat_temp).dpat_high_limit[iSite]=(*dpat_temp).fixed_high_limit;
							}
							if ((*dpat_temp).fixed_low_limit != DPAT_NO_LOW_LIMIT)
							if ((*dpat_temp).dpat_low_limit[iSite] < (*dpat_temp).fixed_low_limit) 
							{
								(*dpat_temp).dpat_low_limit[iSite]=(*dpat_temp).fixed_low_limit;
							}
						}
					}
					if (calculate_now) (*dpat_temp).activated=TRUE;
				}
			}
			for (iSite=0; iSite<Maxim::GetNumSites();iSite++)
			{
				if (msSiteStat(iSite) && msBinValue(iSite)<=1 && bInitialSample&&DPAT_SAMPLE_BIN!=1)
				{
					char cBinNum[5];
					_itoa_s(DPAT_SAMPLE_BIN,cBinNum,sizeof(cBinNum));
					msFailSite(iSite);
					msFailBin(iSite,cBinNum);
				}
			}
		}

		void DPAT_reset_stats()
		{
			//This test function will reset all DPAT stats.

			map<int,dpat_info>::iterator dpat_it;
			dpat_info* dpat_temp;
	
			if (!dpat_tests.empty())
			{
				for (dpat_it=dpat_tests.begin() ; dpat_it != dpat_tests.end(); dpat_it++) 
				{
					for (int iSite=0; iSite<Maxim::GetNumSites(); iSite++)
					{
						dpat_temp=&((*dpat_it).second);
						(*dpat_temp).dpat_values[iSite].clear();
						(*dpat_temp).activated=FALSE;
						(*dpat_temp).dpat_sos=0;
						(*dpat_temp).dpat_sum=0;
					}
				}
			}
		}
	}
}
void calc_IQR(dpat_info* dpat_temp, int iSite)
{
	//Calculates robust sigma and median based on IQR.
	deque<double> sorted;
	int rem,size,size2;
	double q1,q2,q3,iqr;

	sorted=(*dpat_temp).dpat_values[iSite];
	sort(sorted.begin(),sorted.end()); 
	size=(int)sorted.size();
	//calculate quartiles
	rem = size % 2;
	if (rem == 1)
		q2 = sorted[int(size/2) +1];
	else
		q2 = (sorted[int(size/2)] + sorted[int(size/2)+1])/2;
   

	size2 = size/2;    //get size of lower and upper halfs
	rem = size2 % 2;
	if (rem == 1)
	{
		q1 = sorted[size2/2 +1];
		q3 = sorted[((size-size2)+size2/2 +1)];
	}
	else
	{
		q1 = (sorted[(size2/2)] + sorted[size2/2+1])/2;
		q3 = (sorted[((size-size2) +size2/2)] + sorted[(size-size2) + size2/2 +1])/2;
	}
	
	iqr = q3-q1;
	//calculate new limits
	(*dpat_temp).dpat_high_limit[iSite]=q2 + (iqr/1.35)*(*dpat_temp).dpat_sigma_high;
	(*dpat_temp).dpat_low_limit[iSite]=q2 - (iqr/1.35)*(*dpat_temp).dpat_sigma_low;
	//for (rem = 0; rem < size; rem ++)
		//printf("element %i = %f",rem,sorted[rem]);

}

