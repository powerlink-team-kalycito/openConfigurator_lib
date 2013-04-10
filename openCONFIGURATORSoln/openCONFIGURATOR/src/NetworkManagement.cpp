/**
 ************************************************************************************************
 \file			NetworkManagement.cpp

 \brief			Handles information of each object node present in the network
 ************************************************************************************************
 */

/*
 (c) Kalycito Infotech Private Limited

 License:

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 1. Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.

 3. Neither the name of Kalycito Infotech Private Limited nor the names of 
 its contributors may be used to endorse or promote products derived
 from this software without prior written permission. For written
 permission, please contact info@kalycito.com.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.

 Severability Clause:

 If a provision of this License is or becomes illegal, invalid or
 unenforceable in any jurisdiction, that shall not affect:
 1. the validity or enforceability in that jurisdiction of any other
 provision of this License; or
 2. the validity or enforceability in other jurisdictions of that or
 any other provision of this License.

 ****************************************************************************/

/****************************************************************************************************/
/* Includes */

#include "../Include/NetworkManagement.h"
#include "../Include/Internal.h"

//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

/*************************************************************************/
/* Constructor */

/**
 

 */

NetworkManagement::NetworkManagement(void)
{
	MaxPDOCount = 0;
	nmNodeID = 0;
}

/*************************************************************************/
/* Destructor */

/**
 

 */

NetworkManagement::~NetworkManagement(void)
{
	//Add destructor code here
}

/*****************************************************************************/
/**
 \brief		AddFeature
 
 This is a member function of NetworkManagement collects feature for each object

 \param		stFeature	Structure Variable of feature to add feature to collection list
 
 \return	void
 */
/*****************************************************************************/

void NetworkManagement::AddFeature(Feature stFeature)
{
	INT32 iItemPosition = FeatureCollection.Add();
	FeatureCollection[iItemPosition] = stFeature;
}

/*****************************************************************************/
/**
 \brief			GetNodeID
 
 This is a member function of NetworkManagement Returns the NodeID of NetworkManagement Object

 \return		INT32
 */
/*****************************************************************************/

//TODO: unused function
INT32 NetworkManagement::GetNodeID()
{
	return nmNodeID;
}

/*****************************************************************************/
/**
 \brief			SetNodeID
 
 This is a member function of NetworkManagement sets the NodeID of the NetworkManagement Object

 \return		void
 */
/*****************************************************************************/
//TODO: unused function
void NetworkManagement::SetNodeID(INT32 nodeID)
{
	nmNodeID = nodeID;
}

/*****************************************************************************/
/**
 \brief			getFeatureValue
 
 This is a member function of NetworkManagement gets the network Management feature value

 \param		featureType		Enum variable of FeatureType to hold the feature type
  \param	featureName		Character pointer variable of to hold feature name    

 \return		char*
 */
/*****************************************************************************/

char* NetworkManagement::getFeatureValue(FeatureType featureType,
		char* featureName)
{
	INT32 iLoopCount = 0;
	char* retString = NULL;
	Feature stFeature;

	for (iLoopCount = 0; iLoopCount < FeatureCollection.Count(); iLoopCount++)
	{
		stFeature = FeatureCollection[iLoopCount];
		if (stFeature.featureType == featureType
				&& (!strcmp(featureName, stFeature.Name)))
		{
			retString =
					new char[strlen(stFeature.Value) + STR_ALLOC_BUFFER];
			strcpy(retString, stFeature.Value);
			return retString;
		}
	}
	retString = new char[1 + STR_ALLOC_BUFFER];
	strcpy((char*) retString, "");
	return retString;
}

/*****************************************************************************/
/**
 \brief			GetNumberOfFeatures
 
 This is a member function of NetworkManagement returns the Number of Features

 \return		UINT32
 */
/*****************************************************************************/

UINT32 NetworkManagement::GetNumberOfFeatures()
{
	return FeatureCollection.Count();
}

/*****************************************************************************/
/**
 \brief			GetFeature
 
 This is a member function of NetworkManagement returns feature collection list 

 \param			uiCount  Unsigned integer variable to hold count value
 
 \return		Feature*
 */
/*****************************************************************************/

Feature* NetworkManagement::GetFeature(UINT32 uiCount)
{
	return &FeatureCollection[uiCount];
}

/*****************************************************************************/
/**
 \brief			DeleteFeatureCollections
 
 This is a member function of NetworkManagement Deletes NetworkManagement Collections

 \return		void
 */
/*****************************************************************************/

void NetworkManagement::DeleteFeatureCollections()
{
	if (0 != FeatureCollection.Count())
	{
		FeatureCollection.Clear();
	}
}

/*****************************************************************************/
/**
 \brief			GetMaxPDOCount
 
 This is a member function of NetworkManagement to get max PDO count

 \return		INT32
 */
/*****************************************************************************/

INT32 NetworkManagement::GetMaxPDOCount()
{
	return MaxPDOCount;
}

/*****************************************************************************/
/**
 \brief			CalculateMaxPDOCount
 
 This is a member function of NetworkManagement calculates the PDO count from the PDOTPDOChannels parameter in MN xdd and MaxPDOCount is updated with that specified value

 \return		void
 */
/*****************************************************************************/

void NetworkManagement::CalculateMaxPDOCount()
{
	char* maxPDOCount = new char[5];
	char* featureName = new char[20];
	MaxPDOCount = 0;
	if ((NULL == maxPDOCount) || (NULL == featureName))
	{
#if defined DEBUG
		cout << "Memory allocation error" << __FUNCTION__ << endl;
#endif

		ocfmException ex;
		ex.OCFMException(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
		throw ex;
	}
	else
	{
		strcpy(featureName, "PDOTPDOChannels");
		strcpy(maxPDOCount, getFeatureValue(MN_FEATURES, featureName));
		MaxPDOCount = atoi((char*) maxPDOCount);
		//check is made for the validating the value in MN xdd. 
		//Min value = 0; Maxvalue = 256 (EPSG specification)
		if (MaxPDOCount > 256)
		{
			ocfmException ex;
			ex.OCFMException(OCFM_ERR_EXCEEDS_MAX_TPDO_CHANNELS);
			throw ex;
		}
	}
	delete[] maxPDOCount;
	delete[] featureName;
}
