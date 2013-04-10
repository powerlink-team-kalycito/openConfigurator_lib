/**
 ************************************************************************************************
 \file			XdcOperations.cpp

 \brief			Handles Parameter,Variable declaration, datatype for Index attributes and Sub-Index attributes
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

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <libxml/xmlreader.h>
#include <errno.h>
#include <string.h>
#include "../Include/openCONFIGURATOR.h"
#include "../Include/Declarations.h"
#include "../Include/Exception.h"

/****************************************************************************************************/
/* Defines */

#define MY_ENCODING "UTF-8"

/****************************************************************************************************/
/* Global Variables */

INT32 LastIndexParsed = 0;
static const char *gpa2bSimple[][2] =
{
{ "BOOL", "1" },
{ "BITSTRING", "1" },
{ "BYTE", "8" },
{ "CHAR", "1" },
{ "WORD", "16" },
{ "DWORD", "32" },
{ "LWORD", "64" },
{ "SINT", "8" },
{ "INT", "16" },
{ "DINT", "32" },
{ "LINT", "64" },
{ "USINT", "8" },
{ "UINT", "16" },
{ "UDINT", "32" },
{ "ULINT", "64" },
{ "REAL", "16" },
{ "LREAL", "64" },
{ "STRING", "1" },
{ "WSTRING", "1" } }; //array size in g_simple_arr_size

//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

/*****************************************************************************/
/**
 \brief			SetIndexAttributes
 
 This function shall set Index Attributes
 
 \param			xtrReader      xml Pointer Variable of type xmlTextReaderPtr
 \param			objIndex       Class Pointer Variable of  Index 
 \param			bhasPDO         Boolean Variable to check PDO 
 \return		void
 */
/*****************************************************************************/

void SetIndexAttributes(xmlTextReaderPtr xtrReader, Index *objIndex,
		bool& bhasPDO)
{
	const xmlChar *xcName = NULL;
	const xmlChar *xcValue = NULL;
	if (NULL == objIndex)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	//Retrieve the pxcName and Value of an attribute
	xcValue = xmlTextReaderConstValue(xtrReader);
	xcName = xmlTextReaderConstName(xtrReader);

	if (!strcmp(ConvertToUpper((char*) xcName), "INDEX"))
	{

		// Setting the Index Value
		objIndex->SetIndexValue((char*) xcValue);

		/*$S Commented out the TPDO and RPDO grouping $S*/
		//TODO: Check point 1 
		char* tempValue =
				new char[strlen((char*) xcValue) + STR_ALLOC_BUFFER];
		strcpy((char*) tempValue, (char*) xcValue);
		if ((!strncmp(tempValue, "14", 2))
				|| (!strncmp(tempValue, "16", 2)))
		{
			objIndex->SetPDOType(PDO_RPDO);
			bhasPDO = true;
		}
		else if ((!strncmp(tempValue, "18", 2))
				|| (!strncmp(tempValue, "1A", 2)))
		{
			objIndex->SetPDOType(PDO_TPDO);
			bhasPDO = true;
		}
		else
		{
			//No operations for Non-PDO's
		}
		delete[] tempValue;

	}
	else if (!(strcmp(ConvertToUpper((char*) xcName), "NAME")))
	{
		CheckAndCorrectName((char*) xcValue);
		objIndex->SetName((char*) xcValue);
	}
	else if (!strcmp(ConvertToUpper((char*) xcName), "OBJECTTYPE"))
	{
		objIndex->SetObjectType((char*) xcValue);
	}
	else if (!strcmp(ConvertToUpper((char*) xcName), "LOWLIMIT"))
	{
		objIndex->SetLowLimit((char*) xcValue);
	}
	else if (!strcmp(ConvertToUpper((char*) xcName), "HIGHLIMIT"))
	{
		objIndex->SetHighLimit((char*) xcValue);
	}
	else if (!strcmp(ConvertToUpper((char*) xcName), "ACCESSTYPE"))
	{
		objIndex->SetAccessType((char*) xcValue);
	}
	else if (!strcmp(ConvertToUpper((char*) xcName), "PDOMAPPING"))
	{
		objIndex->SetPDOMapping((char*) xcValue);
	}
	else if (!strcmp(ConvertToUpper((char*) xcName), "DEFAULTVALUE"))
	{
		objIndex->SetDefaultValue((char*) xcValue);
	}
	else if (!strcmp(ConvertToUpper((char*) xcName), "ACTUALVALUE"))
	{
		objIndex->SetActualValue((char*) xcValue);
	}
	else if (!strcmp(ConvertToUpper((char*) xcName), "DATATYPE"))
	{
		if (CheckIfDataTypeExists((char*) xcValue, objIndex->GetNodeID()))
		{
			objIndex->SetDataType((char*) xcValue);
		}
		else
		{
			ocfmException ex;
			ex.OCFMException(OCFM_ERR_DATATYPE_NOT_FOUND);
		}
	}
	else if (!strcmp(ConvertToUpper((char*) xcName), "UNIQUEIDREF"))
	{
		objIndex->SetUniqueIDRef((char*) xcValue);
	}
	else if (!strcmp(ConvertToUpper((char*) xcName), "CDCFLAG"))
	{
		if (!strcmp(ConvertToUpper((char*) xcValue), "FALSE"))
		{
			objIndex->SetFlagIfIncludedCdc(FALSE);
		}
		else if (!strcmp(ConvertToUpper((char*) xcValue), "TRUE"))
		{
			objIndex->SetFlagIfIncludedCdc(TRUE);
		}
		else
		{
			//Nothing to be added.
		}
	}
	else
	{
		//TODO: Else added. Operation to be specified.
	}
}
/*****************************************************************************/
/**
 \brief			SetSubIndexAttributes
 
 This Function shall set subindex Attributes
 
 \param			xtrReader         xml Pointer of xmlTextReaderPtr
 \param			objSubIndex       Class Pointer of SubIndex 
 \return		void
 */
/*****************************************************************************/
void SetSubIndexAttributes(xmlTextReaderPtr xtrReader, SubIndex *objSubIndex)
{
	const xmlChar *xcName = NULL;
	const xmlChar *xcValue = NULL;
	if (NULL == objSubIndex)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	//Retrieve the pxcName and Value of an attribute
	xcValue = xmlTextReaderConstValue(xtrReader);
	xcName = xmlTextReaderConstName(xtrReader);

	if (!strcmp(ConvertToUpper((char*) xcName), "SUBINDEX"))
	{
		objSubIndex->SetIndexValue((char*) xcValue);
	}
	else if (!strcmp(ConvertToUpper((char*) xcName), "NAME"))
	{
		CheckAndCorrectName((char*) xcValue);
		objSubIndex->SetName((char*) xcValue);
	}
	else if (!strcmp(ConvertToUpper((char*) xcName), "OBJECTTYPE"))
	{
		objSubIndex->SetObjectType((char*) xcValue);
	}
	else if (!strcmp(ConvertToUpper((char*) xcName), "LOWLIMIT"))
	{
		objSubIndex->SetLowLimit((char*) xcValue);
	}
	else if (!strcmp(ConvertToUpper((char*) xcName), "HIGHLIMIT"))
	{
		objSubIndex->SetHighLimit((char*) xcValue);
	}
	else if (!strcmp(ConvertToUpper((char*) xcName), "ACCESSTYPE"))
	{
		objSubIndex->SetAccessType((char*) xcValue);
	}
	else if (!strcmp(ConvertToUpper((char*) xcName), "PDOMAPPING"))
	{
		objSubIndex->SetPDOMapping((char*) xcValue);
	}
	else if (!strcmp(ConvertToUpper((char*) xcName), "DEFAULTVALUE"))
	{
		objSubIndex->SetDefaultValue((char*) xcValue);
	}
	else if (!strcmp(ConvertToUpper((char*) xcName), "ACTUALVALUE"))
	{
		objSubIndex->SetActualValue((char*) xcValue);
	}
	else if (!strcmp(ConvertToUpper((char*) xcName), "DATATYPE"))
	{
		objSubIndex->SetDataType((char*) xcValue);
	}
	else if (!strcmp(ConvertToUpper((char*) xcName), "UNIQUEIDREF"))
	{
		objSubIndex->SetUniqueIDRef((char*) xcValue);
	}
	else if (!strcmp(ConvertToUpper((char*) xcName), "CDCFLAG"))
	{
		if (!strcmp(ConvertToUpper((char*) xcValue), "FALSE"))
		{
			objSubIndex->SetFlagIfIncludedCdc(FALSE);
		}
		else if (!strcmp(ConvertToUpper((char*) xcValue), "TRUE"))
		{
			objSubIndex->SetFlagIfIncludedCdc(TRUE);
		}
		else
		{
			//Nothing will be executed.
		}
	}
	else
	{
#ifdef DEBUG
		cout << "Error! setSubIndexAttributes fails\n" << endl;
#endif
	}
}

/*****************************************************************************/
/**
 \brief			SetDataTypeAttributes
 
 This Fuction shall set datatype for the attributes
 
 \param			pxtrReader      xml Pointer variable of xmlTextReaderPtr
 \param			objDataType     Class Pointer Variable of DataType
 \return		void
 */
/*****************************************************************************/

void SetDataTypeAttributes(xmlTextReaderPtr pxtrReader, DataType *objDataType)
{
	const xmlChar *xcName = NULL;
	const xmlChar *xcValue = NULL;
	if (NULL == objDataType)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	objDataType->Name = NULL;
	objDataType->dataTypeValue = NULL;
	objDataType->dataSize = 0;

	//Retrieve the pxcName and Value of an attribute
	xcValue = xmlTextReaderConstValue(pxtrReader);
	xcName = xmlTextReaderConstName(pxtrReader);

	if (!strcmp(ConvertToUpper((char*) xcName), "DATATYPE"))
	{
		INT32 iRetVal;
		objDataType->dataTypeValue = new char[strlen((char*) xcValue) + 1];
		strcpy(objDataType->dataTypeValue, (char*) xcValue);
		//Read the Equivalent pxcName of a datatype
		iRetVal = xmlTextReaderRead(pxtrReader);
		if (1 != iRetVal)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_XML_FILE_CORRUPTED);
			throw objException;
		}
		while (XML_READER_TYPE_ELEMENT != xmlTextReaderNodeType(pxtrReader))
		{
			iRetVal = xmlTextReaderRead(pxtrReader);
			if (iRetVal != 1)
			{
				ocfmException objException;
				objException.OCFMException(OCFM_ERR_XML_FILE_CORRUPTED);
				throw objException;
			}
		}
		//pxcValue 	= xmlTextReaderConstValue(pxtrReader);
		xcName = xmlTextReaderConstName(pxtrReader);

		objDataType->Name = new char[strlen((char*) xcName) + ALLOC_BUFFER];
		strcpy(objDataType->Name, (char*) xcName);

		objDataType->dataSize = GetDataSize(objDataType->Name);
		if ((0 == strcmp(objDataType->Name, "Unsigned8"))
				|| (0 == strcmp(objDataType->Name, "UNSIGNED8")))
		{
			objDataType->IEC_dt = USINT;
		}
		else if ((0 == strcmp(objDataType->Name, "Boolean"))
				|| (0 == strcmp(objDataType->Name, "BOOLEAN")))
		{
			objDataType->IEC_dt = BOOL;
		}
		else if ((0 == strcmp(objDataType->Name, "Integer8"))
				|| (0 == strcmp(objDataType->Name, "INTEGER8")))
		{
			objDataType->IEC_dt = SINT;
		}
		else if ((0 == strcmp(objDataType->Name, "Unsigned16"))
				|| (0 == strcmp(objDataType->Name, "UNSIGNED16")))
		{
			objDataType->IEC_dt = UINT;
		}
		else if ((0 == strcmp(objDataType->Name, "Integer16"))
				|| (0 == strcmp(objDataType->Name, "INTEGER16")))
		{
			objDataType->IEC_dt = INT;
		}
		else if ((0 == strcmp(objDataType->Name, "Unsigned32"))
				|| (0 == strcmp(objDataType->Name, "UNSIGNED32")))
		{
			objDataType->IEC_dt = UDINT;
		}
		else if ((0 == strcmp(objDataType->Name, "Integer32"))
				|| (0 == strcmp(objDataType->Name, "INTEGER32")))
		{
			objDataType->IEC_dt = DINT;
		}
		else if ((0 == strcmp(objDataType->Name, "Unsigned64"))
				|| (0 == strcmp(objDataType->Name, "UNSIGNED64")))
		{
			objDataType->IEC_dt = ULINT;
		}
		else if ((0 == strcmp(objDataType->Name, "Integer64"))
				|| (0 == strcmp(objDataType->Name, "INTEGER64")))
		{
			objDataType->IEC_dt = LINT;
		}
		else if ((0 == strcmp(objDataType->Name, "Real32"))
				|| (0 == strcmp(objDataType->Name, "REAL32")))
		{
			objDataType->IEC_dt = REAL;
		}
		else if ((0 == strcmp(objDataType->Name, "Real64"))
				|| (0 == strcmp(objDataType->Name, "REAL64")))
		{
			objDataType->IEC_dt = LREAL;
		}
		else
		{
			cout << __FUNCTION__ << " unhandled datatype: " << objDataType->Name
					<< endl;
		}
	}
}

/*****************************************************************************/
/**
 \brief			SetParameterAttributes
 
 This Function shall set parameter attributes
 
 \param			xtrReader      xml Pointer of xmlTextReaderPtr
 \param			stParameter    Class Pointer of Parameter
 \return		void
 */
/*****************************************************************************/

void SetParameterAttributes(xmlTextReaderPtr xtrReader,
		Parameter *stParameter)
{
	const xmlChar *xcName = NULL;
	const xmlChar *xcValue = NULL;
	if (NULL == stParameter)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	//Retrieve the pxcName and Value of an attribute
	xcValue = xmlTextReaderConstValue(xtrReader);
	xcName = xmlTextReaderConstName(xtrReader);

	if (!strcmp(ConvertToUpper((char*) xcName), "UNIQUEID"))
	{
		stParameter->nameIdDtAttr.SetUniqueID((char*) xcValue);
	}
	else if (!strcmp(ConvertToUpper((char*) xcName), "NAME"))
	{
		CheckAndCorrectName((char*) xcValue);
		stParameter->nameIdDtAttr.SetName((char*) xcValue);
	}
	else if (!strcmp(ConvertToUpper((char*) xcName), "ACCESS"))
	{
		stParameter->Access = new char[strlen((const char*) xcValue) + 1];
		strcpy(stParameter->Access, (const char*) xcValue);
	}
	else
	{
		//TODO: Else added. operation to be added
	}

	/* TO DO: DATATYPE..There is no tag for it..need to check after how many reads datatype is define </label>
	 <USINT/>*/
}

/*****************************************************************************/
/**
 \brief			GetParaDT
 
 This function shall provide parameter datatype
 
 \param			xtrReader        xml Pointer of xmlTextReaderPtr
 \param			stParameter      Class Pointer of Parameter 
 \return		void
 */
/*****************************************************************************/

void GetParaDT(xmlTextReaderPtr xtrReader, Parameter *stParameter)
{
	const xmlChar *xcName = NULL;
	const xmlChar *xcValue = NULL;
	INT32 iRetVal;
	if (NULL == stParameter)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	iRetVal = xmlTextReaderRead(xtrReader);

	if (1 != iRetVal)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_XML_FILE_CORRUPTED);
		throw objException;
	}

	xcName = xmlTextReaderConstName(xtrReader);
	xcValue = xmlTextReaderConstValue(xtrReader);

	while (!(CheckEndElement(xmlTextReaderNodeType(xtrReader), (char*) xcName,
			(char*) "parameter")))
	{
		iRetVal = xmlTextReaderRead(xtrReader);

		if (1 != iRetVal)
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_XML_FILE_CORRUPTED);
			throw objException;
		}

		xcValue = xmlTextReaderConstValue(xtrReader);
		xcName = xmlTextReaderConstName(xtrReader);

		char abSize[3];

		if (CheckifSimpleDT((char*) xcName, abSize))
		{
			stParameter->nameIdDtAttr.SetDataType((char*) xcName);
		}
		if (CheckStartElement(xmlTextReaderNodeType(xtrReader),
				(char*) xcName, (char*) "dataTypeIDRef"))
		{
			if (TRUE == xmlTextReaderHasAttributes(xtrReader))
			{

				xmlTextReaderMoveToNextAttribute(xtrReader);
				xcValue = xmlTextReaderConstValue(xtrReader);
				xcName = xmlTextReaderConstName(xtrReader);

				if (FALSE
						== strcmp(ConvertToUpper((char*) xcName),
								"UNIQUEIDREF"))
				{
					stParameter->nameIdDtAttr.SetDtUniqueRefId(
							(char*) xcValue);
				}
			}
		}
	}
}

/*****************************************************************************/
/**
 \brief			SetCDTAttributes
 
 This function shall set CDT attributes
 
 \param			xtrReader    xml Pointer Variable of xmlTextReaderPtr
 \param			objCDT       Class Pointer Variable of ComplexDataType  
 \return		void
 */
/*****************************************************************************/

static void SetCDTAttributes(xmlTextReaderPtr xtrReader,
		ComplexDataType *objCDT)
{
	const xmlChar *xcName = NULL;
	const xmlChar *xcValue = NULL;
	if (NULL == objCDT)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	//Retrieve the pxcName and Value of an attribute
	xcValue = xmlTextReaderConstValue(xtrReader);
	xcName = xmlTextReaderConstName(xtrReader);

	if (0 == strcmp(ConvertToUpper((char*) xcName), "UNIQUEID"))
	{
		objCDT->nameIdAttr->SetUniqueID((char*) xcValue);
	}

	else if (0 == strcmp(ConvertToUpper((char*) xcName), "NAME"))
	{
		CheckAndCorrectName((char*) xcValue);
		objCDT->nameIdAttr->SetName((char*) xcValue);
	}
	else
	{
		//TODO: else addded. Operation to be added.
	}
}

/*****************************************************************************/
/**
 \brief			CheckifSimpleDT
 
 This function shall check the data type
 
 \param			iName         Character Pointer Variable to hold datatype name
 \param			iSize         Character Pointer Variable to hold datatype size

 \return		BOOL
 \retval			TRUE			if successful
 \retval			FALSE			if there is already a message pending
 */
/*****************************************************************************/

bool CheckifSimpleDT(char *iName, char *iSize)
{
	INT32 iLoopCount = 0;
	char *pbSimpleElement = NULL;

	while (iLoopCount < gSimpleArrSize)
	{
		pbSimpleElement = (char*) gpa2bSimple[iLoopCount][0];

		if (!strcmp(pbSimpleElement, iName))
		{
			strcpy(iSize, (char*) gpa2bSimple[iLoopCount][1]);
			return true;
		}

		iLoopCount++;
	}
	return false;
}

/*****************************************************************************/
/**
 \brief			SetVarDecAttributes
 
 This function shall set variable declaration for Attributes
 
 \param			xtrReader	xml Pointer of xmlTextReaderPtr
 \param			vdecl		Class Pointer of varDeclaration
 
 \return		void
 */
/*****************************************************************************/

void SetVarDecAttributes(xmlTextReaderPtr xtrReader, varDeclaration& vdecl)
{
	const xmlChar *xcName = NULL;
	const xmlChar *xcValue = NULL;

	//Retrieve the pxcName and Value of an attribute
	xcValue = xmlTextReaderConstValue(xtrReader);
	xcName = xmlTextReaderConstName(xtrReader);

	if (!strcmp(ConvertToUpper((char*) xcName), "UNIQUEID"))
	{
		vdecl.namIdDtAttr->SetUniqueID((char*) xcValue);
	}
	else if (!strcmp(ConvertToUpper((char*) xcName), "NAME"))
	{
		CheckAndCorrectName((char*) xcValue);
		vdecl.namIdDtAttr->SetName((char*) xcValue);
	}
	else
	{
		//TODO: Else added. Operation to be added.
	}

	if (!strcmp(ConvertToUpper((char*) xcName), "SIZE"))
	{
		strcpy((char*) vdecl.Size, (const char*) xcValue);
	}
}

/*****************************************************************************/
/**
 \brief			CheckEndElement
 
 This Function shall check end element
 
 \param			iNodeType         Integer Variable to hold the value of node type
 \param			iElement         Character Pointer Variable to hold element value 
 \param			compareWith     Character Pointer Variable to hold value of element to compare
 
 \return		BOOL
 \retval			TRUE			if successful
 \retval			FALSE			if there is already a message pending
 */
/*****************************************************************************/

bool CheckEndElement(INT32 iNodeType, char *iElement, char *compareWith)
{
	if ((XML_READER_TYPE_END_ELEMENT == iNodeType)
			&& (!strcmp(iElement, compareWith)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*****************************************************************************/
/**
 \brief			CheckStartElement
 
 This Function shall check start element
 
 \param			iNodeType         Integer Variable to hold the value of node type
 \param			iElement         Character Pointer Variable to hold element value 
 \param			compareWith     Character Pointer Variable to hold value of element to compare
 
 \return		BOOL
 \retval			TRUE			if successful
 \retval			FALSE			if there is already a message pending
 */
/*****************************************************************************/

bool CheckStartElement(INT32 iNodeType, char *iElement, char *compareWith)
{
	if ((XML_READER_TYPE_ELEMENT == iNodeType)
			&& (!strcmp(iElement, compareWith)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*****************************************************************************/
/**
 \brief			GetVarDeclaration
 
 This Function shall assign Variable declaration
 
 \param			xtrReader  xml Pointer variable of xmlTextReaderPtr
 \param			objCDT     Class Pointer Variable of ComplexDataType
 \return		void
 */
/*****************************************************************************/

static void GetVarDeclaration(xmlTextReaderPtr xtrReader,
		ComplexDataType *objCDT)
{
	const xmlChar *xcName = NULL;
	const xmlChar *xcValue = NULL;

	if (NULL == objCDT)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}

	INT32 iRetVal;
	varDeclaration ObjVarDecl;
	ObjVarDecl.Initialize();
	iRetVal = xmlTextReaderRead(xtrReader);

	if (1 != iRetVal)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_XML_FILE_CORRUPTED);
		throw objException;
	}

	xcName = xmlTextReaderConstName(xtrReader);
	xcValue = xmlTextReaderConstValue(xtrReader);

	while (!(CheckEndElement(xmlTextReaderNodeType(xtrReader), (char*) xcName,
			(char*) "struct")))
	{
		varDeclaration objTempVarDecl;
		objTempVarDecl.Initialize();

		try
		{
			iRetVal = xmlTextReaderRead(xtrReader);

			if (1 != iRetVal)
			{
				ocfmException objException; // = new ocfmException;
				objException.OCFMException(OCFM_ERR_XML_FILE_CORRUPTED);
				throw objException;
			}
		} catch (ocfmException *ex)
		{
			throw ex->_ocfmRetCode;
		}
		xcName = xmlTextReaderConstName(xtrReader);
		xcValue = xmlTextReaderConstValue(xtrReader);

		if (CheckEndElement(xmlTextReaderNodeType(xtrReader), (char*) xcName,
				(char*) "varDeclaration"))
		{
			ObjVarDecl.structUniqueId = new char[strlen(
					objCDT->nameIdAttr->GetUniqueID()) + 1];
			strcpy(ObjVarDecl.structUniqueId,
					objCDT->nameIdAttr->GetUniqueID());
			objCDT->AddVarDeclaration(ObjVarDecl);
			ObjVarDecl = objTempVarDecl;
		}
		if (CheckStartElement(xmlTextReaderNodeType(xtrReader),
				(char*) xcName, (char*) "varDeclaration"))
		{
			if (1 == xmlTextReaderHasAttributes(xtrReader))
			{
				while (xmlTextReaderMoveToNextAttribute(xtrReader))
				{
					SetVarDecAttributes(xtrReader, ObjVarDecl);
					xcValue = xmlTextReaderConstValue(xtrReader);
					xcName = xmlTextReaderConstName(xtrReader);
				}
			}
		}
		char abSize[3];

		if (CheckifSimpleDT((char*) xcName, abSize))
		{
			ObjVarDecl.namIdDtAttr->SetDataType((char*) xcName);

			if (!strcmp(ObjVarDecl.Size, ""))
			{
				strcpy(ObjVarDecl.Size, abSize);
			}
		}
		if (CheckStartElement(xmlTextReaderNodeType(xtrReader),
				(char*) xcName, (char*) "dataTypeIDRef"))
		{
			if (1 == xmlTextReaderHasAttributes(xtrReader))
			{

				xmlTextReaderMoveToNextAttribute(xtrReader);
				xcValue = xmlTextReaderConstValue(xtrReader);
				xcName = xmlTextReaderConstName(xtrReader);

				if (!strcmp(ConvertToUpper((char*) xcName), "UNIQUEIDREF"))
				{
					ObjVarDecl.namIdDtAttr->SetDtUniqueRefId(
							(char*) xcValue);
				}
			}
		}
	}
}

/*****************************************************************************/
/**
 \brief		ImportXML
 
 This API shall be used for parsing the XDC/XDD file and load the objects for 
 the Node ID of Node Type specified as arguments

 \param		fileName		Char pointer to hold the path of the configuration file[XDD or XDC] to be imported.
 \param		iNodeID			Integer variable to hold the Node Id of a node
 \param		varNodeType		Enum to hold the Node type of the node

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode ImportXML(char *fileName, INT32 iNodeID, NodeType varNodeType)
{
	ocfmRetCode stErrStruct;
	try
	{
		stErrStruct = ParseFile(fileName, iNodeID, varNodeType);

		if (0 != stErrStruct.code)
		{
			return stErrStruct;
		}
		xmlCleanupParser();
		/*
		 * this is to debug memory for regression tests
		 */
		xmlMemoryDump();

		/* Add other required index*/
		if (CN == varNodeType)
		{
			SetFlagForRequiredCNIndexes(iNodeID);
			/* Not required only in case of autogenartion 1020 of CN should be updated else it shud be as it is imported*/
			//AddOtherRequiredCNIndexes(iNodeID);
		}
		if (MN == varNodeType)
		{
			SetFlagForRequiredMNIndexes(iNodeID);

		}

		/* Copy default value of pdos to act value*/
		CopyPDODefToAct(iNodeID, varNodeType);
		CopyMNPropDefToAct(iNodeID, varNodeType);
		CalculateCNPollResponse(iNodeID, varNodeType);
		SetCNLossObjects(iNodeID, varNodeType);
		stErrStruct.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return stErrStruct;
}

/*****************************************************************************/
/**
 \brief			ProcessNode
 
 This Function shall Process the Node pxcValue,Name and its attributes
 
 \param			xtrReader         xml Pointer of xmlTextReaderPtr
 \param			varNodeType       Enum Variable of ENodeType 
 \param			iNodeIndex         Integer Variable to hold the value of node index
 \return		void
 */
/*****************************************************************************/

void ProcessNode(xmlTextReaderPtr xtrReader, NodeType varNodeType,
		INT32 iNodeIndex)
{
	const xmlChar *xcName = NULL;
	NodeCollection *objNodeCollection = NULL;
	Node *objNode = NULL;

	xcName = xmlTextReaderConstName(xtrReader);

	try
	{
		//If the NodeTYPE is ELEMENT
		if (TRUE == xmlTextReaderNodeType(xtrReader))
		{
			if (!strcmp(((char*) xcName), "defType"))
			{
				objNodeCollection = NodeCollection::GetNodeColObjectPointer();
				DataTypeCollection* pobjDataTypeCollection;
				DataType objDataType;
				objDataType.Name = NULL;
				objDataType.dataTypeValue = NULL;

				if (1 == xmlTextReaderHasAttributes(xtrReader))
				{
					while (xmlTextReaderMoveToNextAttribute(xtrReader))
					{
						SetDataTypeAttributes(xtrReader, &objDataType);
					}
				}
				objNode = objNodeCollection->GetNodePtr(varNodeType, iNodeIndex);
				pobjDataTypeCollection = objNode->GetDataTypeCollection();
				pobjDataTypeCollection->AddDataType(objDataType);
			}
			else if (!strcmp(((char*) xcName), "parameter"))
			{
				objNodeCollection = NodeCollection::GetNodeColObjectPointer();
				ApplicationProcess* pobjApplicationProcess;
				Parameter stParameter;

				objNode = objNodeCollection->GetNodePtr(varNodeType,
						iNodeIndex);
				if (1 == xmlTextReaderHasAttributes(xtrReader))
				{
					while (xmlTextReaderMoveToNextAttribute(xtrReader))
					{
						SetParameterAttributes(xtrReader, &stParameter);
					}
				}
				GetParaDT(xtrReader, &stParameter);
				// Add parameter to the parameter collection of a node
				pobjApplicationProcess = objNode->GetApplicationProcess();
				pobjApplicationProcess->AddParameter(stParameter);
			}
			else if (!strcmp(((char*) xcName), "struct"))
			{
				objNodeCollection = NodeCollection::GetNodeColObjectPointer();
				ApplicationProcess* pobjApplicationProcess;
				ComplexDataType objCDT;

				objNode = objNodeCollection->GetNodePtr(varNodeType,
						iNodeIndex);

				if (1 == xmlTextReaderHasAttributes(xtrReader))
				{
					while (xmlTextReaderMoveToNextAttribute(xtrReader))
					{
						SetCDTAttributes(xtrReader, &objCDT);
					}
				}

				GetVarDeclaration(xtrReader, &objCDT);
				// Add parameter to the parameter collection of a node
				pobjApplicationProcess = objNode->GetApplicationProcess();
				pobjApplicationProcess->AddComplexDataType(objCDT);
			}
			else if (!strcmp(((char*) xcName), "Object"))
			{
				objNodeCollection = NodeCollection::GetNodeColObjectPointer();
				IndexCollection* pobjIndexCollection;
				Index objIndex;

				objNode = objNodeCollection->GetNodePtr(varNodeType,
						iNodeIndex);
				//Set the NodeID
				objIndex.SetNodeID(objNode->GetNodeId());
				bool bhasPDO = false;

				if (1 == xmlTextReaderHasAttributes(xtrReader))
				{
					while (xmlTextReaderMoveToNextAttribute(xtrReader))
					{
						SetIndexAttributes(xtrReader, &objIndex, bhasPDO);
					}
				}
				if (bhasPDO)
				{
					objNode->SetFlagForPdoObjects(bhasPDO);
				}
				pobjIndexCollection = objNode->GetIndexCollection();

				//Add Index object to the IndexCollection
				pobjIndexCollection->AddIndex(objIndex);
				LastIndexParsed = pobjIndexCollection->GetNumberofIndexes() - 1;
			}
			else if (!strcmp(((char*) xcName), "SubObject"))
			{
				IndexCollection* objIndexCollection = NULL;
				SubIndex objSubIndex;
				Index* objIndexPtr = NULL;
				objNodeCollection = NodeCollection::GetNodeColObjectPointer();
				objNode = objNodeCollection->GetNodePtr(varNodeType,
						iNodeIndex);
				//Set the NodeID
				objSubIndex.SetNodeID(objNode->GetNodeId());

				if (1 == xmlTextReaderHasAttributes(xtrReader))
				{
					while (xmlTextReaderMoveToNextAttribute(xtrReader))
					{
						SetSubIndexAttributes(xtrReader, &objSubIndex);
					}
				}

				objIndexCollection = objNode->GetIndexCollection();
				objIndexPtr = objIndexCollection->GetIndex(LastIndexParsed);
				objIndexPtr->AddSubIndex(objSubIndex);
			}
			else if ((!strcmp(((char*) xcName), "GeneralFeatures"))
					|| (!strcmp(((char*) xcName), "MNFeatures"))
					|| (!strcmp(((char*) xcName), "CNFeatures")))
			{
				objNodeCollection = NodeCollection::GetNodeColObjectPointer();
				Feature stFeature;
				FeatureType varFeatureType;

				objNode = objNodeCollection->GetNodePtr(varNodeType,
						iNodeIndex);

				//Todo: Check case of feature types read from xdd/xdc
				if (!strcmp(((char*) xcName), "GeneralFeatures"))
				{
					varFeatureType = GENERAL_FEATURES;
				}
				else if (!strcmp(((char*) xcName), "MNFeatures"))
				{
					varFeatureType = MN_FEATURES;
				}
				else if (!strcmp(((char*) xcName), "CNFeatures"))
				{
					varFeatureType = CN_FEATURES;
				}
				else
				{
					//Todo: if no features are here "eFeatureType" is uninitialized. Also this loop will not run unless another feature is added
					varFeatureType = GENERAL_FEATURES;
					//Added other as a general features. To avoid errors
				}

				if (1 == xmlTextReaderHasAttributes(xtrReader))
				{
					NetworkManagement* objNwManagement = NULL;
					while (xmlTextReaderMoveToNextAttribute(xtrReader))
					{
						SetFeatures(xtrReader, &stFeature);
						stFeature.featureType = varFeatureType;
						objNwManagement = objNode->GetNetworkManagement();
						objNwManagement->AddFeature(stFeature);
					}
				}

			}
		}
	} catch (ocfmException* ex)
	{
		throw ex->_ocfmRetCode;
	}
}

/*****************************************************************************/
/**
 \brief		ParseFile
 
 Recursiveley parses the Nodes, elements and values from the XML[xdd/xdc] file

 \param		fileName		Char pointer  to hold the path of the file [XDD or XDC].
 \param		iNodeIndex		Integer variable to hold the Node Id of a node
 \param		varNodeType		Enum to hold the Node type of the node

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode ParseFile(char* fileName, INT32 iNodeIndex,
		NodeType varNodeType)
{
	xmlTextReaderPtr xtrReader;
	INT32 iRetVal;

	xtrReader = xmlReaderForFile(fileName, NULL, 0);
	try
	{
		if (NULL != xtrReader)
		{
			iRetVal = xmlTextReaderRead(xtrReader);
			while (1 == iRetVal)
			{
				ProcessNode(xtrReader, varNodeType, iNodeIndex);
				iRetVal = xmlTextReaderRead(xtrReader);
			}
			xmlFreeTextReader(xtrReader);
			if (0 != iRetVal)
			{
				ocfmException objException;
				objException.OCFMException(OCFM_ERR_PARSE_XML);
				throw objException;
			}
		}
		else
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_CANNOT_OPEN_FILE);
			throw objException;
		}
	} catch (ocfmException& ex)
	{
		DeleteNodeObjDict(iNodeIndex, varNodeType);
		return ex._ocfmRetCode;
	}
	try
	{
		if (MN == varNodeType)
		{
			ocfmException objException;
			Node objNode;
			NodeCollection *objNodeCollection = NULL;
			NetworkManagement *objNwManagement = NULL;
			objNodeCollection = NodeCollection::GetNodeColObjectPointer();
			if (NULL == objNodeCollection)
			{
#if defined DEBUG
				cout << "Memory allocation error" << __FUNCTION__ << endl;
#endif

				ocfmException ex;
				ex.OCFMException(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
				throw ex;
			}
			objNode = objNodeCollection->GetNode(varNodeType, iNodeIndex);
			objNwManagement = objNode.GetNetworkManagement();
			if (NULL == objNwManagement)
			{
#if defined DEBUG
				cout << "Memory allocation error" << __FUNCTION__ << endl;
#endif

				ocfmException ex;
				ex.OCFMException(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
				throw ex;
			}
			objNwManagement->CalculateMaxPDOCount();
			return objException._ocfmRetCode;
		}
	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	ocfmRetCode ErrStruct;
	ErrStruct.code = OCFM_ERR_SUCCESS;
	return ErrStruct;
}

/*****************************************************************************/
/**
 \brief			ReImportXML
 
 This API shall be used for parsing the XDC/XDD file and re-load the objects for
 the Node ID of Node Type specified as arguments for this API. This API will not
 delete and create the node. Only the objects for the node are recreated.

 \param			fileName		Char pointer to hold the path of the configuration file [XDD or XDC] to be reimported for a Node
 \param			iNodeID			Integer variable to hold the Node Id of a node
 \param			varNodeType		Enum to hold the Node type of the node

 \return		ocfmRetCode		
 */
/*****************************************************************************/

ocfmRetCode ReImportXML(char* fileName, INT32 iNodeID, NodeType varNodeType)
{
	INT32 iNodePos;
	ocfmRetCode errStruct;
	try
	{
		bool bFlag = false;
		errStruct = IfNodeExists(iNodeID, varNodeType, &iNodePos, bFlag);
		if ((0 == errStruct.code) && (true == bFlag))
		{

			errStruct = DeleteNodeObjDict(iNodeID, varNodeType);
			if (OCFM_ERR_SUCCESS != errStruct.code)
			{
#if defined DEBUG
				cout << "\nDeleteNodeObjDict in ReImport failed\n" << endl;
#endif
			}
			errStruct = ParseFile(fileName, iNodeID, varNodeType);
			if (OCFM_ERR_SUCCESS != errStruct.code)
			{
#if defined DEBUG
				cout << "\nparseFile in ReImport failed\n" << endl;
#endif
				errStruct = DeleteNodeObjDict(iNodeID, varNodeType);
				if (OCFM_ERR_SUCCESS != errStruct.code)
				{
#if defined DEBUG
					cout << "\nDeleteNodeObjDict in ReImport failed\n" << endl;
#endif
				}
			}
			xmlCleanupParser();
			/*
			 * this is to debug memory for regression tests
			 */
			xmlMemoryDump();

			/* Add other required index*/
			if (varNodeType == CN)
			{
				SetFlagForRequiredCNIndexes(iNodeID);
				/* Not required only in case of autogenartion 1020 of CN should be updated else it shud be as it is imported*/
			}
			if (varNodeType == MN)
			{
				SetFlagForRequiredMNIndexes(iNodeID);
			}
			/* Copy default value of pdos to act value*/
			CopyPDODefToAct(iNodeID, varNodeType);
			CopyMNPropDefToAct(iNodeID, varNodeType);
			CalculateCNPollResponse(iNodeID, varNodeType);
			SetCNLossObjects(iNodeID, varNodeType);
			if (varNodeType == MN)
				RecalculateMultiplex();

			errStruct.code = OCFM_ERR_SUCCESS;
		}
		else
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INVALID_NODEID);
			throw objException;
		}
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return errStruct;
}

/*****************************************************************************/
/**
 \brief		SaveNode

 This API shall be used to save a node in xml format. This API saves the all the objects [Index/SubIndex] under the node into a xml file along with all the attributes of the objects.

 \param		fileName		Char pointer  to hold the path of the file [XDD or XDC]
 \param		iNodeID			Integer variable to hold the Node Id of a node
 \param		varNodeType		Enum to hold the Node type of the node

 \return	ocfmRetCode
 */
/*****************************************************************************/
ocfmRetCode SaveNode(const char* fileName, INT32 iNodeID,
		NodeType varNodeType)
{
	INT32 iBytesWritten;
	xmlTextWriterPtr xtwWriter;
	xmlDocPtr xdDoc;
	ocfmRetCode stErrInfo;

	try
	{
		Node objNode;
		NodeCollection *objNodeCollection = NULL;
		IndexCollection *objIndexCollection = NULL;
		Index objIndex;
		Index* objIndexPtr = NULL;
		ApplicationProcess* objAppProc = NULL;
		INT32 iIndexPos = 0;

		objIndex.SetNodeID(objNode.GetNodeId());
		objNodeCollection = NodeCollection::GetNodeColObjectPointer();
		objNode = objNodeCollection->GetNode(varNodeType, iNodeID);

		objIndexCollection = objNode.GetIndexCollection();

		if (0 == objIndexCollection->GetNumberofIndexes())
		{
			stErrInfo.code = OCFM_ERR_SUCCESS;
			return stErrInfo;
		}

		/* Create a new XmlWriter for DOM, with no compression. */
		xtwWriter = xmlNewTextWriterDoc(&xdDoc, 0);
		if (NULL == xtwWriter)
		{
			printf("testXmlpxtwWriterDoc: Error creating the xml pxtwWriter\n");
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_CREATE_XML_WRITER_FAILED);
			throw objException;
		}
		/* Start the document with the xml default for the version,
		 * encoding UTF-8 and the default for the standalone
		 * declaration. */
		iBytesWritten = xmlTextWriterStartDocument(xtwWriter, NULL,
				MY_ENCODING, NULL);
		if (0 > iBytesWritten)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterStartDocument\n");
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_XML_START_DOC_FAILED);
			throw objException;
		}
		iBytesWritten = xmlTextWriterWriteComment(xtwWriter,
				BAD_CAST "This file was autogenerated by openCONFIGURATOR");

		// Start ISO15745ProfileContainer Tag		
		iBytesWritten = xmlTextWriterStartElement(xtwWriter,
				BAD_CAST "ISO15745ProfileContainer");
		if (0 > iBytesWritten)
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}

		// Start ProfileBody Tag
		iBytesWritten = xmlTextWriterStartElement(xtwWriter,
				BAD_CAST "ProfileBody");
		if (0 > iBytesWritten)
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}

		// Start ApplicationProcess Tag	
		iBytesWritten = xmlTextWriterStartElement(xtwWriter,
				BAD_CAST "ApplicationProcess");
		if (0 > iBytesWritten)
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}
		xmlTextWriterSetIndent(xtwWriter, 1);

		// Start dataTypeList Tag
		iBytesWritten = xmlTextWriterStartElement(xtwWriter,
				BAD_CAST "dataTypeList");
		if (0 > iBytesWritten)
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}

		ComplexDataType* objCDT = NULL;
		objAppProc = objNode.GetApplicationProcess();

		if (0 < objAppProc->GetCDTCount())
		{
			for (INT32 iLoopCount = 0; iLoopCount < objAppProc->GetCDTCount();
					iLoopCount++)
			{
				// Start struct Tag
				iBytesWritten = xmlTextWriterStartElement(xtwWriter,
						BAD_CAST "struct");
				if (0 > iBytesWritten)
				{
					printf(
							"testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
					ocfmException objException;
					objException.OCFMException(
							OCFM_ERR_XML_WRITER_START_ELT_FAILED);
					throw objException;
				}
				objCDT = objAppProc->GetCDTbyCount(iLoopCount);
				iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
						BAD_CAST "name",
						BAD_CAST objCDT->nameIdAttr->GetName());
				iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
						BAD_CAST "uniqueID",
						BAD_CAST objCDT->nameIdAttr->uniqueId);

				for (INT32 tempVarCount = 0;
						tempVarCount < objCDT->varCollection.Count();
						tempVarCount++)
				{
					varDeclaration vd;
					vd.Initialize();
					vd = objCDT->varCollection[tempVarCount];
					// Start varDeclaration Tag
					iBytesWritten = xmlTextWriterStartElement(xtwWriter,
							BAD_CAST "varDeclaration");
					if (0 > iBytesWritten)
					{
						printf(
								"testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
						ocfmException objException;
						objException.OCFMException(
								OCFM_ERR_XML_WRITER_START_ELT_FAILED);
						throw objException;
					}
					iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
							BAD_CAST "name",
							BAD_CAST vd.namIdDtAttr->GetName());
					iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
							BAD_CAST "uniqueID",
							BAD_CAST vd.namIdDtAttr->uniqueId);
					iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
							BAD_CAST "size", BAD_CAST vd.Size);

					if (NULL != vd.namIdDtAttr->GetDataType())
					{
						iBytesWritten = xmlTextWriterStartElement(xtwWriter,
								BAD_CAST vd.namIdDtAttr->GetDataType());

						if (0 > iBytesWritten)
						{
							printf(
									"testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
							ocfmException objException;
							objException.OCFMException(
									OCFM_ERR_XML_WRITER_START_ELT_FAILED);
							throw objException;
						}
						// End varDeclaration Tag
						iBytesWritten = xmlTextWriterEndElement(xtwWriter);
						if (0 > iBytesWritten)
						{
							printf(
									"testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
							ocfmException objException;
							objException.OCFMException(
									OCFM_ERR_XML_WRITER_END_ELT_FAILED);
							throw objException;
						}
					}
					else if (NULL != vd.namIdDtAttr->GetDtUniqueRefId())
					{
						iBytesWritten = xmlTextWriterStartElement(xtwWriter,
								BAD_CAST "dataTypeIDRef");
						if (0 > iBytesWritten)
						{
							printf(
									"testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
							ocfmException objException;
							objException.OCFMException(
									OCFM_ERR_XML_WRITER_START_ELT_FAILED);
							throw objException;
						}

						iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
								BAD_CAST "uniqueIDRef",
								BAD_CAST vd.namIdDtAttr->GetDtUniqueRefId());

						// End dataTypeIDRef Tag
						iBytesWritten = xmlTextWriterEndElement(xtwWriter);
						if (0 > iBytesWritten)
						{
							printf(
									"testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
							ocfmException objException;
							objException.OCFMException(
									OCFM_ERR_XML_WRITER_END_ELT_FAILED);
							throw objException;
						}
					}
					else
					{
						//TODO: Else added. Operation to be added.
					}

					// End varDeclaration Tag
					iBytesWritten = xmlTextWriterEndElement(xtwWriter);
					if (0 > iBytesWritten)
					{
						printf(
								"testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
						ocfmException objException;
						objException.OCFMException(
								OCFM_ERR_XML_WRITER_END_ELT_FAILED);
						throw objException;
					}
				}

				// End struct Tag
				iBytesWritten = xmlTextWriterEndElement(xtwWriter);
				if (0 > iBytesWritten)
				{
					printf(
							"testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
					ocfmException objException;
					objException.OCFMException(
							OCFM_ERR_XML_WRITER_END_ELT_FAILED);
					throw objException;
				}
			}
		}

		// End dataTypeList Tag
		iBytesWritten = xmlTextWriterEndElement(xtwWriter);
		if (0 > iBytesWritten)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}

		// Start parameterList Tag
		iBytesWritten = xmlTextWriterStartElement(xtwWriter,
				BAD_CAST "parameterList");
		if (0 > iBytesWritten)
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}

		if (NULL != objNode.GetApplicationProcess())
		{
			objAppProc = objNode.GetApplicationProcess();

			for (INT32 iLoopCount = 0;
					iLoopCount < objAppProc->ParameterCollection.Count();
					iLoopCount++)
			{
				Parameter Param;
				Param = objAppProc->GetUniqueIDRefbyParameterIndex(
						iLoopCount);
				// Start parameter Tag
				iBytesWritten = xmlTextWriterStartElement(xtwWriter,
						BAD_CAST "parameter");
				if (0 > iBytesWritten)
				{
					printf(
							"testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
					ocfmException objException;
					objException.OCFMException(
							OCFM_ERR_XML_WRITER_START_ELT_FAILED);
					throw objException;
				}
				iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
						BAD_CAST "uniqueID",
						BAD_CAST Param.nameIdDtAttr.GetUniqueID());
				iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
						BAD_CAST "access", BAD_CAST Param.Access);

				// Start dataTypeIDRef Tag
				iBytesWritten = xmlTextWriterStartElement(xtwWriter,
						BAD_CAST "dataTypeIDRef");
				if (0 > iBytesWritten)
				{
					printf(
							"testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
					ocfmException objException;
					objException.OCFMException(
							OCFM_ERR_XML_WRITER_START_ELT_FAILED);
					throw objException;
				}
				iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
						BAD_CAST "uniqueIDRef",
						BAD_CAST Param.nameIdDtAttr.GetDtUniqueRefId());
				// End dataTypeIDRef Tag
				iBytesWritten = xmlTextWriterEndElement(xtwWriter);
				if (0 > iBytesWritten)
				{
					printf(
							"testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
					ocfmException objException;
					objException.OCFMException(
							OCFM_ERR_XML_WRITER_END_ELT_FAILED);
					throw objException;
				}

				// End parameter Tag
				iBytesWritten = xmlTextWriterEndElement(xtwWriter);
				if (0 > iBytesWritten)
				{
					printf(
							"testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
					ocfmException objException;
					objException.OCFMException(
							OCFM_ERR_XML_WRITER_END_ELT_FAILED);
					throw objException;
				}
			}
		}
		// End parameterList Tag
		iBytesWritten = xmlTextWriterEndElement(xtwWriter);
		if (0 > iBytesWritten)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}

		// End ApplicationProcess Tag
		iBytesWritten = xmlTextWriterEndElement(xtwWriter);
		if (0 > iBytesWritten)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}

		// End ProfileBody Tag
		iBytesWritten = xmlTextWriterEndElement(xtwWriter);
		if (0 > iBytesWritten)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}

		// Start ProfileBody Tag
		iBytesWritten = xmlTextWriterStartElement(xtwWriter,
				BAD_CAST "ProfileBody");
		if (0 > iBytesWritten)
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}

		// Start ApplicationLayers Tag
		iBytesWritten = xmlTextWriterStartElement(xtwWriter,
				BAD_CAST "ApplicationLayers");
		if (0 > iBytesWritten)
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}

		// Start DataTypeList Tag
		iBytesWritten = xmlTextWriterStartElement(xtwWriter,
				BAD_CAST "DataTypeList");
		if (0 > iBytesWritten)
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}

		DataTypeCollection *pobjDataTypeCollection = NULL;

		pobjDataTypeCollection = objNode.GetDataTypeCollection();

		for (INT32 iLoopCount = 0;
				iLoopCount < pobjDataTypeCollection->GetNumberOfDataTypes();
				iLoopCount++)
		{
			DataType* dt = NULL;
			dt = pobjDataTypeCollection->GetDataTypeElement(iLoopCount);

			// Start defType Tag
			iBytesWritten = xmlTextWriterStartElement(xtwWriter,
					BAD_CAST "defType");
			if (0 > iBytesWritten)
			{
				printf(
						"testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
				ocfmException objException;
				objException.OCFMException(
						OCFM_ERR_XML_WRITER_START_ELT_FAILED);
				throw objException;
			}

			iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
					BAD_CAST "dataType", BAD_CAST dt->dataTypeValue);
			// Start DataType Tag
			iBytesWritten = xmlTextWriterStartElement(xtwWriter,
					BAD_CAST dt->GetName());
			if (0 > iBytesWritten)
			{
				printf(
						"testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
				ocfmException objException;
				objException.OCFMException(
						OCFM_ERR_XML_WRITER_START_ELT_FAILED);
				throw objException;
			}
			xmlTextWriterSetIndent(xtwWriter, 1);
			// End DataType Tag
			iBytesWritten = xmlTextWriterEndElement(xtwWriter);
			if (0 > iBytesWritten)
			{
				printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
				ocfmException objException;
				objException.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
				throw objException;
			}
			xmlTextWriterSetIndent(xtwWriter, 1);
			// End defType Tag
			iBytesWritten = xmlTextWriterEndElement(xtwWriter);
			if (0 > iBytesWritten)
			{
				printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
				ocfmException objException;
				objException.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
				throw objException;
			}
		}
		xmlTextWriterSetIndent(xtwWriter, 1);
		// End DataTypeList Tag
		iBytesWritten = xmlTextWriterEndElement(xtwWriter);
		if (0 > iBytesWritten)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}

		if (0 == objIndexCollection->GetNumberofIndexes())
		{
			printf("SaveNode: No Indexes Found\n");
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}

		// Start ObjectList Tag
		iBytesWritten = xmlTextWriterStartElement(xtwWriter,
				BAD_CAST "ObjectList");
		if (0 > iBytesWritten)
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}

		for (iIndexPos = 0;
				iIndexPos < objIndexCollection->GetNumberofIndexes();
				iIndexPos++)
		{
			// Start Object Tag
			iBytesWritten = xmlTextWriterStartElement(xtwWriter,
					BAD_CAST "Object");
			if (0 > iBytesWritten)
			{
				printf(
						"testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
				ocfmException objException;
				objException.OCFMException(
						OCFM_ERR_XML_WRITER_START_ELT_FAILED);
				throw objException;
			}

			objIndexPtr = objIndexCollection->GetIndex(iIndexPos);

			if (NULL != objIndexPtr->GetIndexValue())
			{
				iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
						BAD_CAST "index",
						BAD_CAST objIndexPtr->GetIndexValue());
			}
			if (NULL != objIndexPtr->GetName())
			{
				iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
						BAD_CAST "name", BAD_CAST objIndexPtr->GetName());
			}
			if (NULL != objIndexPtr->GetObjectType())
			{
				char *strObjectType = new char[10];
				strObjectType = IntToAscii(objIndexPtr->GetEObjectType(),
						strObjectType, 10);
				iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
						BAD_CAST "objectType", BAD_CAST strObjectType);
				delete[] strObjectType;
			}
			DataType objTmpDataType;
			objTmpDataType = objIndexPtr->GetDataType();
			if (NULL != objTmpDataType.dataTypeValue)
				if (strlen(objTmpDataType.dataTypeValue) != 0)
				{
					iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
							BAD_CAST "dataType",
							BAD_CAST objTmpDataType.dataTypeValue);
				}
			if ((objIndexPtr->GetAccessType() != NULL))
				if (strlen(objIndexPtr->GetAccessType()) != 0)
					iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
							BAD_CAST "accessType",
							BAD_CAST objIndexPtr->GetAccessType());
			if ((objIndexPtr->GetDefaultValue() != NULL))
				if (strlen(objIndexPtr->GetDefaultValue()) != 0)
					iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
							BAD_CAST "defaultValue",
							BAD_CAST objIndexPtr->GetDefaultValue());
			if ((objIndexPtr->GetActualValue() != NULL))
				if (strlen(objIndexPtr->GetActualValue()) != 0)
					iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
							BAD_CAST "actualValue",
							BAD_CAST objIndexPtr->GetActualValue());
			if (objIndexPtr->GetLowLimit() != NULL)
				if (strlen(objIndexPtr->GetLowLimit()) != 0)
				{
					iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
							BAD_CAST "lowLimit",
							BAD_CAST objIndexPtr->GetLowLimit());
				}
			if (objIndexPtr->GetHighLimit() != NULL)
				if (strlen(objIndexPtr->GetHighLimit()) != 0)
				{
					iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
							BAD_CAST "highLimit",
							BAD_CAST objIndexPtr->GetHighLimit());
				}
			if ((objIndexPtr->GetPDOMapping() != NULL))
				if (strlen(objIndexPtr->GetPDOMapping()) != 0)
					iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
							BAD_CAST "PDOmapping",
							BAD_CAST objIndexPtr->GetPDOMapping());
			if (objIndexPtr->GetUniqueIDRef() != NULL)
				if (strlen(objIndexPtr->GetUniqueIDRef()) != 0)
					iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
							BAD_CAST "uniqueIDRef",
							BAD_CAST objIndexPtr->GetUniqueIDRef());
			if (objIndexPtr->GetFlagIfIncludedCdc() == 0)
			{
				iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
						BAD_CAST "CDCFlag", BAD_CAST "FALSE");
			}
			else if (objIndexPtr->GetFlagIfIncludedCdc() == 1)
			{
				iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
						BAD_CAST "CDCFlag", BAD_CAST "TRUE");
			}
			else
			{
				//Nothing to do.
			}

			xmlTextWriterSetIndent(xtwWriter, 1);

			if (!(objIndexPtr->GetNumberofSubIndexes() <= 0))
			{
				INT32 SubIndexPos = 0;
				for (SubIndexPos = 0;
						SubIndexPos < objIndexPtr->GetNumberofSubIndexes();
						SubIndexPos++)
				{
					// Start SubObject Tag
					iBytesWritten = xmlTextWriterStartElement(xtwWriter,
							BAD_CAST "SubObject");
					if (iBytesWritten < 0)
					{
						printf(
								"testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
						ocfmException objException;
						objException.OCFMException(
								OCFM_ERR_XML_WRITER_START_ELT_FAILED);
						throw objException;
					}
					SubIndex* objSubIndexPtr = NULL;
					objSubIndexPtr = objIndexPtr->GetSubIndex(SubIndexPos);
					if (objSubIndexPtr->GetIndexValue() != NULL)
						iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
								BAD_CAST "subIndex",
								BAD_CAST objSubIndexPtr->GetIndexValue());
					if (objSubIndexPtr->GetName() != NULL)
						iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
								BAD_CAST "name",
								BAD_CAST objSubIndexPtr->GetName());
					if (objSubIndexPtr->GetObjectType() != NULL)
					{
						char *str_EObjectType = new char[10];
						str_EObjectType = IntToAscii(
								objSubIndexPtr->GetEObjectType(),
								str_EObjectType, 10);
						iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
								BAD_CAST "objectType",
								BAD_CAST str_EObjectType);
						delete[] str_EObjectType;
					}
					DataType objTempDataType;
					objTempDataType = objSubIndexPtr->GetDataType();
					if (objTempDataType.dataTypeValue != NULL)
						if (strlen(objTempDataType.dataTypeValue) != 0)
							iBytesWritten = xmlTextWriterWriteAttribute(
									xtwWriter, BAD_CAST "dataType",
									BAD_CAST objTempDataType.dataTypeValue);
					if (objSubIndexPtr->GetAccessType() != NULL)
						iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
								BAD_CAST "accessType",
								BAD_CAST objSubIndexPtr->GetAccessType());
					if (objSubIndexPtr->GetDefaultValue() != NULL)
						iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
								BAD_CAST "defaultValue",
								BAD_CAST objSubIndexPtr->GetDefaultValue());
					if (objSubIndexPtr->GetActualValue() != NULL)
						iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
								BAD_CAST "actualValue",
								BAD_CAST objSubIndexPtr->GetActualValue());
					if (objSubIndexPtr->GetLowLimit() != NULL)
					{
						if (strlen(objSubIndexPtr->GetLowLimit()) != 0)
						{
							iBytesWritten = xmlTextWriterWriteAttribute(
									xtwWriter, BAD_CAST "lowLimit",
									BAD_CAST objSubIndexPtr->GetLowLimit());
						}
					}

					if (objSubIndexPtr->GetHighLimit() != NULL)
					{
						if (strlen(objSubIndexPtr->GetHighLimit()) != 0)
						{
							iBytesWritten = xmlTextWriterWriteAttribute(
									xtwWriter, BAD_CAST "highLimit",
									BAD_CAST objSubIndexPtr->GetHighLimit());
						}
					}

					if ((objIndexPtr->GetPDOMapping() != NULL))
					{
						if (objSubIndexPtr->GetPDOMapping() != NULL)
						{
							iBytesWritten = xmlTextWriterWriteAttribute(
									xtwWriter, BAD_CAST "PDOmapping",
									BAD_CAST objSubIndexPtr->GetPDOMapping());
						}
					}

					if (objSubIndexPtr->GetUniqueIDRef() != NULL)
					{
						iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
								BAD_CAST "uniqueIDRef",
								BAD_CAST objSubIndexPtr->GetUniqueIDRef());
					}

					if (objSubIndexPtr->GetFlagIfIncludedCdc() == 0)
					{
						iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
								BAD_CAST "CDCFlag", BAD_CAST "FALSE");
					}
					else if (objSubIndexPtr->GetFlagIfIncludedCdc() == 1)
					{
						iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
								BAD_CAST "CDCFlag", BAD_CAST "TRUE");
					}
					else
					{
						//Nothing to do. 
					}
					// End SubObject Tag
					iBytesWritten = xmlTextWriterEndElement(xtwWriter);
					if (iBytesWritten < 0)
					{
						printf(
								"testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
						ocfmException objException;
						objException.OCFMException(
								OCFM_ERR_XML_WRITER_END_ELT_FAILED);
						throw objException;
					}

				}
			}

			// End Object Tag
			iBytesWritten = xmlTextWriterEndElement(xtwWriter);
			if (iBytesWritten < 0)
			{
				printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
				ocfmException objException;
				objException.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
				throw objException;
			}

		}

		xmlTextWriterSetIndent(xtwWriter, 1);
		// End ObjectList Tag
		iBytesWritten = xmlTextWriterEndElement(xtwWriter);
		if (iBytesWritten < 0)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}

		xmlTextWriterSetIndent(xtwWriter, 1);
		// End ApplicationLayers Tag
		iBytesWritten = xmlTextWriterEndElement(xtwWriter);
		if (iBytesWritten < 0)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}
		// Start Network Management Tag
		iBytesWritten = xmlTextWriterStartElement(xtwWriter,
				BAD_CAST "NetworkManagement");
		if (iBytesWritten < 0)
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}

		xmlTextWriterSetIndent(xtwWriter, 1);

		// Start General Features Tag
		iBytesWritten = xmlTextWriterStartElement(xtwWriter,
				BAD_CAST "GeneralFeatures");
		if (iBytesWritten < 0)
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}
		NetworkManagement *objNwManagement = NULL;
		objNwManagement = objNode.GetNetworkManagement();

		for (UINT32 uiLoopCount = 0;
				uiLoopCount < objNwManagement->GetNumberOfFeatures();
				uiLoopCount++)
		{
			Feature* feature = NULL;
			feature = objNwManagement->GetFeature(uiLoopCount);

			if (feature->featureType == GENERAL_FEATURES)
			{
				iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
						BAD_CAST feature->Name, BAD_CAST feature->Value);
				if (iBytesWritten < 0)
				{
					printf(
							"testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
					ocfmException objException;
					objException.OCFMException(
							OCFM_ERR_XML_WRITER_START_ELT_FAILED);
					throw objException;
				}
			}
		}
		xmlTextWriterSetIndent(xtwWriter, 1);
		// End General Features Tag
		iBytesWritten = xmlTextWriterEndElement(xtwWriter);

		//Write MN Features for MN Node
		if (objNode.GetNodeType() == MN)
		{
			//Start MN Features Tag			
			iBytesWritten = xmlTextWriterStartElement(xtwWriter,
					BAD_CAST "MNFeatures");
			if (iBytesWritten < 0)
			{
				printf(
						"testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
				ocfmException objException;
				objException.OCFMException(
						OCFM_ERR_XML_WRITER_START_ELT_FAILED);
				throw objException;
			}
			NetworkManagement *varObjNwManagement = NULL;
			varObjNwManagement = objNode.GetNetworkManagement();

			for (UINT32 uiLoopCount = 0;
					uiLoopCount < varObjNwManagement->GetNumberOfFeatures();
					uiLoopCount++)
			{
				Feature* feature = NULL;
				feature = varObjNwManagement->GetFeature(uiLoopCount);

				if (feature->featureType == MN_FEATURES)
				{
					iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
							BAD_CAST feature->Name,
							BAD_CAST feature->Value);
					if (iBytesWritten < 0)
					{
						printf(
								"testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
						ocfmException objException; // = new ocfmException;
						objException.OCFMException(
								OCFM_ERR_XML_WRITER_START_ELT_FAILED);
						throw objException;
					}
				}
			}
			xmlTextWriterSetIndent(xtwWriter, 1);
			// End MN Features Tag
			iBytesWritten = xmlTextWriterEndElement(xtwWriter);

		}

		//Write CN Features for CN Node
		else if (objNode.GetNodeType() == CN)
		{
			//Start CN Features Tag
			//Start MN Features Tag			
			iBytesWritten = xmlTextWriterStartElement(xtwWriter,
					BAD_CAST "CNFeatures");
			if (iBytesWritten < 0)
			{
				printf(
						"testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
				ocfmException objException;
				objException.OCFMException(
						OCFM_ERR_XML_WRITER_START_ELT_FAILED);
				throw objException;
			}
			NetworkManagement *varObjNwManagement = NULL;
			varObjNwManagement = objNode.GetNetworkManagement();

			for (UINT32 uiLoopCount = 0;
					uiLoopCount < varObjNwManagement->GetNumberOfFeatures();
					uiLoopCount++)
			{
				Feature* feature = NULL;
				feature = varObjNwManagement->GetFeature(uiLoopCount);

				if (feature->featureType == CN_FEATURES)
				{
					iBytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
							BAD_CAST feature->Name,
							BAD_CAST feature->Value);
					if (iBytesWritten < 0)
					{
						printf(
								"testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
						ocfmException objException;
						objException.OCFMException(
								OCFM_ERR_XML_WRITER_START_ELT_FAILED);
						throw objException;
					}
				}
			}
			xmlTextWriterSetIndent(xtwWriter, 1);
			//End CN Features Tag
			iBytesWritten = xmlTextWriterEndElement(xtwWriter);
		}
		else
		{
			//Nothing other than MN & CN
		}

		xmlTextWriterSetIndent(xtwWriter, 1);
		//End Network Management Tag
		iBytesWritten = xmlTextWriterEndElement(xtwWriter);

		xmlTextWriterSetIndent(xtwWriter, 1);
		// End ProfileBody Tag
		iBytesWritten = xmlTextWriterEndElement(xtwWriter);
		if (iBytesWritten < 0)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}

		xmlTextWriterSetIndent(xtwWriter, 1);
		// End ISO15745ProfileContainer Tag
		iBytesWritten = xmlTextWriterEndElement(xtwWriter);
		if (iBytesWritten < 0)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}

		iBytesWritten = xmlTextWriterEndDocument(xtwWriter);
		if (iBytesWritten < 0)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndDocument\n");
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_XML_END_DOC_FAILED);
			throw objException;
		}

		xmlFreeTextWriter(xtwWriter);

		xmlSaveFileEnc(fileName, xdDoc, MY_ENCODING);

		xmlFreeDoc(xdDoc);
	} catch (ocfmException *ex)
	{
		throw ex->_ocfmRetCode;
	}

	stErrInfo.code = OCFM_ERR_SUCCESS;
	return stErrInfo;
}

/*****************************************************************************/
/**
 \brief			SetFlagForRequiredCNIndexes
 
 This function shall set the flag for CN indexs
 
 \param			iNodeId     Integer Variable to hold the value of nodeid
 
 \return		void
 */
/*****************************************************************************/

void SetFlagForRequiredCNIndexes(INT32 iNodeId)
{
	Index* objIndex = NULL;
	IndexCollection* objIdxCol = NULL;
	NodeCollection* objNodeCol = NULL;
	Node* objNode = NULL;
	SubIndex* objSIdx = NULL;
	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	objNode = objNodeCol->GetNodePtr(CN, iNodeId);
	objIdxCol = objNode->GetIndexCollection();

	INT32 iIndexCount = objIdxCol->GetNumberofIndexes();

	for (INT32 iLoopCount = 0; iLoopCount < iIndexCount; iLoopCount++)
	{
		objIndex = objIdxCol->GetIndex(iLoopCount);

		if ((CheckIfNotPDO((char*) objIndex->GetIndexValue()) == false)
				|| (strcmp((char*) objIndex->GetIndexValue(), "1F98") == 0)
				|| (strcmp((char*) objIndex->GetIndexValue(), "1C14") == 0)
				|| (strcmp((char*) objIndex->GetIndexValue(), "1020") == 0)
				|| (strcmp((char*) objIndex->GetIndexValue(), "1006") == 0)
				|| CheckIfManufactureSpecificObject(
						(char*) objIndex->GetIndexValue()))
		{
			objIndex->SetFlagIfIncludedCdc(TRUE);
			for (INT32 iSIDxCount = 0;
					iSIDxCount < objIndex->GetNumberofSubIndexes();
					iSIDxCount++)
			{
				objSIdx = objIndex->GetSubIndex(iSIDxCount);
				if (objSIdx != NULL)
				{
					objSIdx->SetFlagIfIncludedCdc(TRUE);
				}

			}
		}
	}
}

/*****************************************************************************/
/**
 \brief		SetFlagForRequiredMNIndexes
 
 This Function shall set flag for MN indexs
 
 \param		iNodeId   Integer Variable to hold the value of node id
 
 \return	void
 */
/*****************************************************************************/

void SetFlagForRequiredMNIndexes(INT32 iNodeId)
{
	Index* objIndex = NULL;
	IndexCollection* objIdxCol = NULL;
	NodeCollection* objNodeCol = NULL;
	Node* objNode = NULL;

	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	objNode = objNodeCol->GetNodePtr(MN, iNodeId);
	objIdxCol = objNode->GetIndexCollection();

	INT32 iIndexCount = objIdxCol->GetNumberofIndexes();

	for (INT32 iLoopCount = 0; iLoopCount < iIndexCount; iLoopCount++)
	{
		objIndex = objIdxCol->GetIndex(iLoopCount);

		if (CheckIfNotPDO((char*) objIndex->GetIndexValue()) == false
				|| strcmp((char*) objIndex->GetIndexValue(), "1006") == 0
				|| strcmp((char*) objIndex->GetIndexValue(), "1020") == 0
				|| strcmp((char*) objIndex->GetIndexValue(), "1300") == 0
				|| strcmp((char*) objIndex->GetIndexValue(), "1C02") == 0
				|| strcmp((char*) objIndex->GetIndexValue(), "1C09") == 0
				|| strcmp((char*) objIndex->GetIndexValue(), "1C14") == 0
				|| strcmp((char*) objIndex->GetIndexValue(), "1F26") == 0
				|| strcmp((char*) objIndex->GetIndexValue(), "1F27") == 0
				|| strcmp((char*) objIndex->GetIndexValue(), "1F84") == 0
				|| strcmp((char*) objIndex->GetIndexValue(), "1F8B") == 0
				|| strcmp((char*) objIndex->GetIndexValue(), "1F8D") == 0
				|| strcmp((char*) objIndex->GetIndexValue(), "1F92") == 0
				|| strcmp((char*) objIndex->GetIndexValue(), "1F98") == 0)
		{
			objIndex->SetFlagIfIncludedCdc(TRUE);
			for (INT32 iSIDxCount = 0;
					iSIDxCount < objIndex->GetNumberofSubIndexes();
					iSIDxCount++)
			{
				SubIndex* pobjSIdx = NULL;
				pobjSIdx = objIndex->GetSubIndex(iSIDxCount);
				pobjSIdx->SetFlagIfIncludedCdc(TRUE);
			}
		}
		else if (strcmp((char*) objIndex->GetIndexValue(), "1F8A") == 0)
		{
			objIndex->SetFlagIfIncludedCdc(TRUE);
			for (INT32 iSIDxCount = 0;
					iSIDxCount < objIndex->GetNumberofSubIndexes();
					iSIDxCount++)
			{
				SubIndex* pobjSIdx = NULL;
				pobjSIdx = objIndex->GetSubIndex(iSIDxCount);
				if (strcmp((char*) pobjSIdx->GetIndexValue(), "02") == 0)
				{
					pobjSIdx->SetFlagIfIncludedCdc(TRUE);
					break;
				}
			}
		}
		else
		{
			//Nothing to be added.
		}
	}
}

/*****************************************************************************/
/**
 \brief		AddOtherRequiredCNIndexes
 
 This function shall add required CN indexes
 
 \param		iNodeId		Integer Variable to hold the value of node id
 
 \return	ocfmRetCode
 */
/*****************************************************************************/
//TODO: unused function
ocfmRetCode AddOtherRequiredCNIndexes(INT32 iNodeId)
{
	ocfmRetCode stRetCode;
	char* mnIndex = new char[INDEX_LEN];
	char* sIdx = new char[SUBINDEX_LEN];
	IndexCollection* objIdxCol = NULL;
	NodeCollection* objNodeCol = NULL;
	Node* objNode = NULL;

	try
	{
		objNodeCol = NodeCollection::GetNodeColObjectPointer();
		objNode = objNodeCol->GetNodePtr(CN, iNodeId);
		objIdxCol = objNode->GetIndexCollection();

		/* Add 1006*/
		strcpy(mnIndex, "1020");
#if defined DEBUG	
		cout << "string copied" << endl;
#endif
		stRetCode = AddIndex(objNode->GetNodeId(), CN, mnIndex);
#if defined DEBUG	
		cout << "stRetCode" << stRetCode.code << endl;
		cout << "1020 added" << endl;
#endif
		if ((stRetCode.code != 0)
				&& (stRetCode.code != OCFM_ERR_INDEX_ALREADY_EXISTS))
			return stRetCode;

		char* Val = new char[16];
		Val = IntToAscii(iConfigDate, Val, 10);

		/* Set 5ms pxcValue*/
		/* Set subindex pxcValue 40 or 0000028 */
		strcpy(sIdx, "01");
		SetSIdxValue(mnIndex, sIdx, Val, objIdxCol, objNode->GetNodeId(), CN,
				false);

		Val = IntToAscii(iConfigTime, Val, 10);

		strcpy(sIdx, "02");
		SetSIdxValue(mnIndex, sIdx, Val, objIdxCol, objNode->GetNodeId(), CN,
				false);
	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	stRetCode.code = OCFM_ERR_SUCCESS;
	return stRetCode;
}

/*****************************************************************************/
/**
 \brief			GetDataSize
 
 This Function shall provide datasize of the variable
 
 \param			dataTypeVal     Character Pointer Variable to hold datatype value
 \return		INT32
 */
/*****************************************************************************/

INT32 GetDataSize(char* dataTypeVal)
{
	if (strcmp(StringToUpper(dataTypeVal), "UNSIGNED8") == 0
			|| strcmp(StringToUpper(dataTypeVal), "BOOLEAN") == 0
			|| strcmp(StringToUpper(dataTypeVal), "INTEGER8") == 0)
	{
		return 1;
	}
	else if (strcmp(StringToUpper(dataTypeVal), "UNSIGNED16") == 0
			|| strcmp(StringToUpper(dataTypeVal), "INTEGER16") == 0)
	{
		return 2;
	}

	else if (strcmp(StringToUpper(dataTypeVal), "INTEGER24") == 0
			|| strcmp(StringToUpper(dataTypeVal), "UNSIGNED24") == 0)
	{
		return 3;
	}
	else if (strcmp(StringToUpper(dataTypeVal), "UNSIGNED32") == 0
			|| strcmp(StringToUpper(dataTypeVal), "INTEGER32") == 0
			|| strcmp(StringToUpper(dataTypeVal), "REAL32") == 0)
	{
		return 4;
	}
	else if (strcmp(StringToUpper(dataTypeVal), "INTEGER40") == 0
			|| strcmp(StringToUpper(dataTypeVal), "UNSIGNED40") == 0)
	{
		return 5;
	}
	else if (strcmp(StringToUpper(dataTypeVal), "INTEGER48") == 0
			|| strcmp(StringToUpper(dataTypeVal), "UNSIGNED48") == 0)
	{
		return 6;
	}
	else if (strcmp(StringToUpper(dataTypeVal), "INTEGER56") == 0
			|| strcmp(StringToUpper(dataTypeVal), "UNSIGNED56") == 0)
	{
		return 7;
	}
	else if (strcmp(StringToUpper(dataTypeVal), "UNSIGNED64") == 0
			|| strcmp(StringToUpper(dataTypeVal), "INTEGER64") == 0
			|| strcmp(StringToUpper(dataTypeVal), "REAL64") == 0)
	{
		return 8;
	}
	else if (strcmp(StringToUpper(dataTypeVal), "MAC_ADDRESS") == 0)
	{
		return 6;
	}
	else if (strcmp(StringToUpper(dataTypeVal), "IP_ADDRESS") == 0)
	{
		return 4;
	}
	/* NETTIME is composed as follows:
	 STRUCT OF
	 UNSIGNED32 seconds
	 UNSIGNED32 nanoseconds */
	else if (strcmp(StringToUpper(dataTypeVal), "NETTIME") == 0)
	{
		return 8;
	}
	/*STRUCT OF
	 UNSIGNED28 ms,
	 VOID4 reserved,
	 UNSIGNED16 days
	 TIME_DIFFERENCE*/
	else if (strcmp(StringToUpper(dataTypeVal), "TIME_DIFF") == 0)
	{
		return 6;
	}
	/*STRUCT OF
	 UNSIGNED28 ms,
	 VOID4 reserved,
	 UNSIGNED16 days
	 TIME_DIFFERENCE*/
	else if (strcmp(StringToUpper(dataTypeVal), "TIME_OF_DAY") == 0)
	{
		return 6;
	}
	else
	{
		//TODO: Else added. 
		cout << __FUNCTION__ << " Unhandled datatype:" << dataTypeVal << endl;
	}
	return 1; //by default
}

/*****************************************************************************/
/**
 \brief			CheckIfStringDatatypes
 
 This function shall check for string datatypes
 
 \param			dataTypeValue     Character Pointer Variable to hold datatype value
 \return		BOOL
 \retval			TRUE			if successful
 \retval			FALSE			if there is already a message pending
 */
/*****************************************************************************/

bool CheckIfStringDatatypes(char* dataTypeValue)
{
	if (strcmp(StringToUpper(dataTypeValue), "VISIBLE_STRING") == 0
			|| strcmp(StringToUpper(dataTypeValue), "OCTET_STRING") == 0
			|| strcmp(StringToUpper(dataTypeValue), "UNICODE_STRING") == 0)
		return true;
	else
		return false;
}

/*****************************************************************************/
/**
 \brief			SetFeatures
 
 This Function shall set the features
 
 \param			xtrReader  xml Pointer Variable of xmlTextReaderPtr
 \param			stFeature  struct Pointer Variable of type Feature
 
 \return		void			
 */
/*****************************************************************************/

void SetFeatures(xmlTextReaderPtr xtrReader, Feature *stFeature)
{
	const xmlChar *xcName = NULL;
	const xmlChar *xcValue = NULL;
	if (NULL == stFeature)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	//Retrieve the pxcName and Value of an attribute
	xcValue = xmlTextReaderConstValue(xtrReader);
	xcName = xmlTextReaderConstName(xtrReader);

	stFeature->Name = new char[strlen((char*) xcName) + STR_ALLOC_BUFFER];
	strcpy(stFeature->Name, (const char*) xcName);

	stFeature->Value = new char[strlen((char*) xcValue) + STR_ALLOC_BUFFER];
	strcpy(stFeature->Value, (const char*) xcValue);

}

