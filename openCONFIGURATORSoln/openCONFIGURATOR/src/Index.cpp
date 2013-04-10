/**
 ************************************************************************************************
 \file			Index.cpp

 \brief			Handles adding index to all PDO types
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

#include <stdio.h>
#include "../Include/Index.h"
#include "../Include/Internal.h"

//==========================================================================//
// 				F U N C T I O N  D E C L A R A T I O N S 					//
//==========================================================================//

/*************************************************************************/
/* Constructor */

/**
 

 */

Index::Index(void)
{
	SubIndexCount = collectionObj.Count();
	//m_PDOType = 0; //TODO: Review Initialisation
}

/*************************************************************************/
/* Destructor */

/**
 

 */

Index::~Index(void)
{
	//Add destructor code here
}

/*****************************************************************************/
/**
 \brief			GetPDOType
 
 This function returns PDO type
 
 \return		PDOType
 */
/*****************************************************************************/

PDOType Index::GetPDOType()
{
	return pdoType;
}

/*****************************************************************************/
/**
 \brief		SetPDOType
 
 This is a member function of setPDOType to assign PDO type
 
 \param		varPDOType		Enum Variable of EPDOType to hold the value of PDO type			
 \return	void
 */
/*****************************************************************************/

void Index::SetPDOType(PDOType varPDOType)
{
	pdoType = varPDOType;
}

#ifndef __GNUC__
#pragma region MemberFunctions
#endif

/*****************************************************************************/
/**
 \brief		AddSubIndex
 
 This is a member function of CIndex to add SubIndex in Index Object
 
 \param		objSubIndex		Class variable of CSubIndex for object subindex		
 
 \return	void
 */
/*****************************************************************************/

void Index::AddSubIndex(SubIndex objSubIndex)
{
	INT32 iItemPosition = collectionObj.Add();
	collectionObj[iItemPosition] = objSubIndex;
	SubIndexCount = collectionObj.Count();
#if defined DEBUG
	cout << "addSubIndex function:";
	cout << "\t Index" << Index::GetIndexValue();
	cout << "\tSubindex:" << objSubIndex.GetIndexValue() << endl;
#endif
}

/*****************************************************************************/
/**
 \brief		DeleteSubIndex
 
 This is a member function of CIndex  to delete the SubIndex in Index Object
 
 \param		iSubIndexID		Integer Variable to hold the value of subindex id		
 
 \return	void
 */
/*****************************************************************************/

void Index::DeleteSubIndex(INT32 iSubIndexID)
{
	collectionObj.Remove(iSubIndexID);
	SubIndexCount = collectionObj.Count();
}

/*****************************************************************************/
/**
 \brief		DeleteSubIndexCollection
 
 This is a member function of CIndex to delete the SubIndex from Index Object
 
 \return	void
 */
/*****************************************************************************/

void Index::DeleteSubIndexCollection()
{
	collectionObj.Clear();
	SubIndexCount = collectionObj.Count();
}

/*****************************************************************************/
/**
 \brief		GetNumberofSubIndexes
 
 This is a member function of CIndex returns number of subindexes
 
 \return	INT32
 */
/*****************************************************************************/

INT32 Index::GetNumberofSubIndexes()
{
	return collectionObj.Count();
}

/*****************************************************************************/
/**
 \brief		GetSubIndex
 
 This is a member function of CIndex returns number of subindex ids
 
 \param		iSubIndexId Integer variable to hold the value of subindex id   
 
 \return	SubIndex*
 */
/*****************************************************************************/

SubIndex* Index::GetSubIndex(INT32 iSubIndexId)
{
	return &collectionObj[iSubIndexId];
}

/*****************************************************************************/
/**
 \brief			GetSubIndexbyIndexValue
 
 This is a member function of CIndex returns number of subindex value 
 
 \param			varIndex		Character pointer to hold index value
 
 \return		SubIndex*
 */
/*****************************************************************************/

SubIndex* Index::GetSubIndexbyIndexValue(char* varIndex)
{
	INT32 iLoopCount = 0;
	SubIndex* objSIndex = NULL;

	for (iLoopCount = 0; iLoopCount < collectionObj.Count(); iLoopCount++)
	{
		objSIndex = &collectionObj[iLoopCount];
		//Check for null , alignment changes
		if (0
				== strcmp(StringToUpper((char*) objSIndex->GetIndexValue()),
						StringToUpper(varIndex)))
		{
			return objSIndex;
		}
		else
		{
			objSIndex = NULL;
		}
	}

	objSIndex = NULL;
	return objSIndex;
}

/*****************************************************************************/
/**
 \brief		SwapSubObjects
 
 This is a member function of CIndex swaps the subobjects
 
 \param		iPos1	Integer Variable to hold the subobject position
 \param		iPos2	Integer Variable to hold the subobject position

 \return	void
 */
/*****************************************************************************/

void Index::SwapSubObjects(INT32 iPos1, INT32 iPos2)
{
	swap(collectionObj[iPos1], collectionObj[iPos2]);
}

/*****************************************************************************/
/**
 \brief			UpdateArraySubObjects
 
 This is a member function of CIndex updates subobjects in array  
 
 \return		void
 */
/*****************************************************************************/

void Index::UpdateArraySubObjects()
{
	INT32 iLoopCount = 0;
	SubIndex* objSIndex = NULL;

	for (iLoopCount = 0; iLoopCount < collectionObj.Count(); iLoopCount++)
	{
		objSIndex = &collectionObj[iLoopCount];
		//Check for null , alignment changes
		if ((NULL != objSIndex->GetIndexValue())
				&& 0 == (strcmp(objSIndex->GetIndexValue(), "00")))
			continue;
		objSIndex->SetDataTypeST(this->GetDataType());
	}
	objSIndex = NULL;
}
#ifndef __GNUC__
#pragma endregion MemberFunctions
#endif
