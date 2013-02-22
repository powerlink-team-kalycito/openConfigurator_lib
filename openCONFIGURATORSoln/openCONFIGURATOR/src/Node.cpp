/**
************************************************************************************************
\file			Node.cpp

\brief			
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
#include "../Include/Node.h"
#include "../Include/Internal.h"


				//==========================================================================//
				// 				F U N C T I O N  D E F I N I T I O N S  					//
				//==========================================================================//


/*************************************************************************/
/* Constructor */

/**
        
    @param void
*/

CNode::CNode(void)
{
	m_NodeName = NULL;
	m_IndexCollection = NULL;
	m_DataTypeCollection = NULL;
	m_IndexCollection = NULL;
	m_ApplicationProcess = NULL;
	m_NetworkManagement = NULL;
	m_HasPdoObjects = false;
	m_StationType = NORMAL;
	m_ForcedCycle = NULL;
	m_PollResponseTimeout = NULL;
	m_ForcedCycleFlag = false;
	m_NodeId = 0;
	m_NodeIndex = 0;
	//m_NodeType = 0; //TODO: Review initialization
	m_PResActPayload = 0;
	m_PReqActPayload = 0;
	//cout<<"CNode: Constructor Executed"<<endl;
}

/*************************************************************************/
/* Destructor */

/**
        
    @param void
*/

CNode::~CNode(void)
{
	//Add destrucor code here
}
#ifndef __GNUC__
#pragma region Properties
#endif


/*****************************************************************************/
/**
\brief			getNodeType
 
				Returns the NodeType of the Node

\retval			ENodeType
*/
/*****************************************************************************/

ENodeType CNode::getNodeType()
{
	return m_NodeType;
}


/*****************************************************************************/
/**
\brief			setNodeType
 
				sets the Node Type of the Node.Value can be either CN or MN

\param			objNodeType
\retval			void
*/
/*****************************************************************************/

void CNode::setNodeType(ENodeType objNodeType)
{
	m_NodeType = objNodeType;
}


/*****************************************************************************/
/**
\brief			getNodeId
 
				Returns the Index of the Index Object

\retval			INT32
*/
/*****************************************************************************/

INT32 CNode::getNodeId()
{
	return m_NodeId;
}


/*****************************************************************************/
/**
\brief			setNodeId
 
				sets the Index of the Index Object

\retval			void
*/
/*****************************************************************************/

void CNode::setNodeId(INT32 iNodeId)
{
	m_NodeId = iNodeId;

}


/*****************************************************************************/
/**
\brief			getNodeIndex
 
				Returns the Index of the Index Object

\retval			INT32
*/
/*****************************************************************************/

INT32 CNode::getNodeIndex()
{
	return m_NodeIndex;
}


/*****************************************************************************/
/**
\brief			setNodeIndex
 
				sets the Index of the Index Object

\retval			void
*/
/*****************************************************************************/


void CNode::setNodeIndex(INT32 iNodeIndex)
{
	m_NodeIndex = iNodeIndex;
}


/*****************************************************************************/
/**
\brief			getNodeName
 
				Returns the Name of the Node

\retval			char*			Node name as string
*/
/*****************************************************************************/

char* CNode::getNodeName()
{
	return m_NodeName;
}


/*****************************************************************************/
/**
\brief			setNodeName
 
				sets the Name of the Node

\retval			void
*/
/*****************************************************************************/


void CNode::setNodeName(char* pbNodeName)
{
	if (NULL != m_NodeName)
	{
		delete[] m_NodeName;
	}
	m_NodeName = new char[strlen(pbNodeName) + STR_ALLOC_BUFFER];
	strcpy((char*) m_NodeName, pbNodeName);
}

 
/*****************************************************************************/
/**
\brief			HasPdoObjects
 
				Returns true if PDO Objects are present else false

\retval			BOOL
\retval			TRUE			if successful
\retval			FALSE			if there is already a message pending	
*/
/*****************************************************************************/

bool CNode::HasPdoObjects()
{
	return m_HasPdoObjects;
}


/*****************************************************************************/
/**
\brief			setFlagForPdoObjects
 
				sets the Name of the Node

\param			bFlag
\retval			void
*/
/*****************************************************************************/

void CNode::setFlagForPdoObjects(bool bFlag)
{
	m_HasPdoObjects = bFlag;
}



/*****************************************************************************/
/**
\brief			getIndexCollection
 
				Returns the IndexCollection of the Node

\retval			CIndexCollection*
*/
/*****************************************************************************/


CIndexCollection* CNode::getIndexCollection()
{
	return m_IndexCollection;
}


/*****************************************************************************/
/**
\brief			getApplicationProcess
 
				Returns the Application Process of the Node

\retval			 CApplicationProcess*
*/
/*****************************************************************************/

CApplicationProcess* CNode::getApplicationProcess()
{
	return m_ApplicationProcess;
}


/*****************************************************************************/
/**
\brief			getNetworkManagement
 
				Returns the NetworkManagement of the Node

\retval			CNetworkManagement*
*/
/*****************************************************************************/

CNetworkManagement* CNode::getNetworkManagement()
{
	return m_NetworkManagement;
}

 
/*****************************************************************************/
/**
\brief			getDataTypeCollection
 
				returns the DataType Collection

\retval			CDataTypeCollection*
*/
/*****************************************************************************/

CDataTypeCollection* CNode::getDataTypeCollection()
{
	return m_DataTypeCollection;
}


/*****************************************************************************/
/**
\brief			CreateIndexCollection
 
				This function creates index collection

\retval			void
*/
/*****************************************************************************/

void CNode::CreateIndexCollection()
{
	CIndexCollection* pobjIndexCollection = NULL;
	pobjIndexCollection = new CIndexCollection();
	m_IndexCollection = pobjIndexCollection;
}


/*****************************************************************************/
/**
\brief			CreateDataTypeCollection
 
				This function creates data type collection

\retval			void
*/
/*****************************************************************************/

void CNode::CreateDataTypeCollection()
{
	CDataTypeCollection* pobjDataTypeCollection = NULL;
	pobjDataTypeCollection = new CDataTypeCollection();
	m_DataTypeCollection = pobjDataTypeCollection;
}


/*****************************************************************************/
/**
\brief			addProcessImage
 
				This function collects the process images
				
\param			objProcessImage
\retval			void
*/
/*****************************************************************************/

void CNode::addProcessImage(ProcessImage objProcessImage)
{
	INT32 iItemPosition = ProcessImageCollection.Add();
	ProcessImageCollection[iItemPosition] = objProcessImage;
}


/*****************************************************************************/
/**
\brief			addNETProcessImage
 
				This function collects the NETprocess images 
				
\param			objNETProcessImage
\retval			void
*/
/*****************************************************************************/

void CNode::addNETProcessImage(NETProcessImage objNETProcessImage)
{
	INT32 iItemPosition = NETProcessImageCollection.Add();
	NETProcessImageCollection[iItemPosition] = objNETProcessImage;
}


/*****************************************************************************/
/**
\brief			addMNPDOvar
 
				This function adds PDO variables for MN
				
\param			stPdoVar
\param			enumEpdoType
\retval			void
*/
/*****************************************************************************/

void CNode::addMNPDOvar(MNPdoVariable stPdoVar, EPDOType enumEpdoType)
{
	if (PDO_RPDO == enumEpdoType)
	{
		INT32 iItemPosition = MNPDOOUTVarCollection.Add();
		MNPDOOUTVarCollection[iItemPosition] = stPdoVar;
	}
	else if (PDO_TPDO == enumEpdoType)
	{
		INT32 iItemPosition = MNPDOINVarCollection.Add();
		MNPDOINVarCollection[iItemPosition] = stPdoVar;
	}
	else
	{
		//Nothing to be added.
	}
}

/*****************************************************************************/
/**
\brief			CreateApplicationProcess
 
				This Function creates application process 
				
\retval			void
*/
/*****************************************************************************/

void CNode::CreateApplicationProcess()
{
	CApplicationProcess* pobjAppliProcess = NULL;

	pobjAppliProcess = new CApplicationProcess();
	m_ApplicationProcess = pobjAppliProcess;
}


/*****************************************************************************/
/**
\brief			CreateNetworkManagament
 
				This function creates  network management for the objects 
				
\retval			void
*/
/*****************************************************************************/

void CNode::CreateNetworkManagament()
{
	CNetworkManagement* pobjNwManagement = NULL;

	pobjNwManagement = new CNetworkManagement();
	m_NetworkManagement = pobjNwManagement;
}

/*****************************************************************************/
/**
\brief			getPDOIndexCollection
 
				This function adds the indexs under EPDO index collection
				
\param			enumEpdoType				
\retval			CIndexCollection*
*/
/*****************************************************************************/

CIndexCollection* CNode::getPDOIndexCollection(EPDOType enumEpdoType)
{
	CIndexCollection* pobjPdoIndexCollection = new CIndexCollection();
	CIndex* pobjIndex = NULL;
	for (INT32 iLoopCount = 0;
			iLoopCount < m_IndexCollection->getNumberofIndexes(); iLoopCount++)
	{
		pobjIndex = m_IndexCollection->getIndex(iLoopCount);

		if (pobjIndex->getPDOType() == enumEpdoType)
		{
			pobjPdoIndexCollection->addIndex(*pobjIndex);
		}
	}
	return pobjPdoIndexCollection;
}


/*****************************************************************************/
/**
\brief			getPDOIndexCollection
 
				This function adds the indexs under PDO index collection
				
\retval			CIndexCollection*
*/
/*****************************************************************************/

CIndexCollection* CNode::getPDOIndexCollection()
{
	CIndexCollection* pobjPdoIndexCollection = new CIndexCollection();
	CIndex* pobjIndex = NULL;
	cout << "getPDOIndexCollection Execution:"
			<< m_IndexCollection->getNumberofIndexes() << endl;
	for (INT32 iLoopCount = 0;
			iLoopCount < m_IndexCollection->getNumberofIndexes(); iLoopCount++)
	{
		pobjIndex = m_IndexCollection->getIndex(iLoopCount);
		if ((pobjIndex->getPDOType() == PDO_TPDO)
				|| (pobjIndex->getPDOType() == PDO_RPDO))
		{
			cout << "PDO INdex: " << pobjIndex->getIndexValue() << endl;
			pobjPdoIndexCollection->addIndex(*pobjIndex);
		}
		else
		{
			cout << "I: " << pobjIndex->getIndexValue() << endl;
		}
	}
	return pobjPdoIndexCollection;
}


/*****************************************************************************/
/**
\brief			getIndexCollectionWithoutPDO
 
				This function adds the indexes for non PDO type
				
\retval			CIndexCollection*
*/
/*****************************************************************************/

CIndexCollection* CNode::getIndexCollectionWithoutPDO()
{
	CIndexCollection* pobjIndexCol = new CIndexCollection();
	CIndex* pobjIndex = NULL;
	for (INT32 iLoopCount = 0;
			iLoopCount < m_IndexCollection->getNumberofIndexes(); iLoopCount++)
	{
		pobjIndex = m_IndexCollection->getIndex(iLoopCount);
		if (CheckIfNotPDO((char*) pobjIndex->getIndexValue()))
		{
			pobjIndexCol->addIndex(*pobjIndex);
		}
	}
	return pobjIndexCol;
}


/*****************************************************************************/
/**
\brief			isNull
 
				This function returns the status of the index collection
				
\retval			BOOL
\retval			TRUE			if successful
\retval			FALSE			if there is already a message pending	
*/
/*****************************************************************************/

bool CNode::isNull()
{
	if (NULL == m_IndexCollection)
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
\brief			getPIbyParaIndex
 
				This function returns process image for each parameter index
				
\param			iParaIndex
\retval			ProcessImage*	
*/
/*****************************************************************************/

ProcessImage* CNode::getPIbyParaIndex(INT32 iParaIndex)
{
	ProcessImage* pobjProcessImage = NULL;

	for (INT32 iLoopCount = 0; iLoopCount < ProcessImageCollection.Count();
			iLoopCount++)
	{
		pobjProcessImage = ProcessImageCollection.GetAddress(iLoopCount);

		if (pobjProcessImage->ParametrIndex == iParaIndex)
		{
			return pobjProcessImage;
		}
	}
	return pobjProcessImage;
}


/*****************************************************************************/
/**
\brief			DeleteCollectionsForPI
 
				This function clears MN PDO collection index and process image collection
				
\retval			void
*/
/*****************************************************************************/
 

void CNode::DeleteCollectionsForPI()
{
	if (0 != MNPDOINVarCollection.Count())
	{
		MNPDOINVarCollection.Clear();
	}
	if (0 != MNPDOOUTVarCollection.Count())
	{
		MNPDOOUTVarCollection.Clear();
	}
	if (0 != ProcessImageCollection.Count())
	{
		ProcessImageCollection.Clear();
	}
}


/*****************************************************************************/
/**
\brief			DeleteCollectionsForPI
 
				This function clears NETprocess image collection
				
\retval			void
*/
/*****************************************************************************/

void CNode::DeleteCollectionsForNETPI()
{
	if (0 != NETProcessImageCollection.Count())
	{
		NETProcessImageCollection.Clear();
	}
}


/*****************************************************************************/
/**
\brief			getStationType
 
				Returns the station type of the Node
				
\retval			EStationType
*/
/*****************************************************************************/

EStationType CNode::getStationType()
{
	return m_StationType;
}


/*****************************************************************************/
/**
\brief			setNodeType
 
				sets the Node Type of the Node.Value can be either CN or MN
				
\param			StationType				
\retval			EStationType
*/
/*****************************************************************************/

void CNode::setStationType(EStationType StationType)
{
	m_StationType = StationType;
}


/*****************************************************************************/
/**
\brief			getForcedCycle
 
				Returns the forced cycle of the Node
				
\retval			char*
*/
/*****************************************************************************/

char* CNode::getForcedCycle()
{
	return m_ForcedCycle;
}


/*****************************************************************************/
/**
\brief			setForcedCycle
 
				sets the cycle of the Node

\param			pbForcedCycle
\retval			void
*/
/*****************************************************************************/

ocfmRetCode CNode::setForcedCycle(char* pbForcedCycle)
{
	//add or update 1f9b
	ocfmRetCode stErrorResult;
	ocfmRetCode stErrorInfoMN;
	ocfmRetCode stErrorInfoCN;
	INT32 IndexPos = 0;
	INT32 subIndexPos = 0;

	char strCustomErrString[200];
	char strCustomErrStringMN[100];
	strCustomErrStringMN[0] = 0;
	char strCustomErrStringCN[100];
	strCustomErrStringCN[0] = 0;

	char* strConvertedValue = new char[SUBINDEX_LEN];
	char acMultiCycleAssignObj[] = MULTIPL_CYCLE_ASSIGN_OBJECT;

	stErrorResult.code = OCFM_ERR_SUCCESS;

	strConvertedValue = _IntToAscii(this->getNodeId(), strConvertedValue, 16);
	strConvertedValue = padLeft(strConvertedValue, '0', 2);

	stErrorInfoMN = IfSubIndexExists(MN_NODEID, MN, acMultiCycleAssignObj,
			strConvertedValue, &subIndexPos, &IndexPos);
	stErrorInfoCN = IfSubIndexExists(this->getNodeId(), CN,
			acMultiCycleAssignObj, strConvertedValue, &subIndexPos, &IndexPos);
	if ((OCFM_ERR_SUCCESS == stErrorInfoMN.code)
			&& (OCFM_ERR_SUCCESS == stErrorInfoCN.code))
	{
		// both the index and subindex are present continue
	}
	else
	{
		if (OCFM_ERR_INDEXID_NOT_FOUND == stErrorInfoMN.code)
		{
			strcpy((char*) strCustomErrStringMN,
					"The Index 1F9B does not exist in MN. ");
		}
		if (OCFM_ERR_INDEXID_NOT_FOUND == stErrorInfoCN.code)
		{
			strcpy((char*) strCustomErrStringCN,
					"The Index 1F9B does not exist in CN. ");
		}
		if (OCFM_ERR_SUBINDEXID_NOT_FOUND == stErrorInfoMN.code)
		{
			sprintf(strCustomErrStringMN,
					"The Subindex %s in Index 1F9B does not exist in MN. ",
					strConvertedValue);
		}
		if (OCFM_ERR_SUBINDEXID_NOT_FOUND == stErrorInfoCN.code)
		{
			sprintf(strCustomErrStringCN,
					"The Subindex %s in Index 1F9B does not exist in CN. ",
					strConvertedValue);
		}
		strcpy(strCustomErrString, strCustomErrStringMN);
		strcat(strCustomErrString, strCustomErrStringCN);
		strcat(strCustomErrString, "Unable to assign the multiplexing cycle");

		stErrorResult.code = OCFM_ERR_MULTIPLEX_ASSIGN_ERROR;
		CopyCustomErrorString(&stErrorResult, (char*) &strCustomErrString);
		return stErrorResult;
	}

	char* subIndName = new char[50];
	subIndName[0] = 0;

	GetSubIndexAttributes(240, MN, acMultiCycleAssignObj, strConvertedValue,
			NAME, subIndName);
	SetSubIndexAttributes(240, MN, acMultiCycleAssignObj, strConvertedValue,
			pbForcedCycle, subIndName, TRUE);

	CIndex* objMN1F9BIndex = NULL;
	objMN1F9BIndex = getMNIndexValues(acMultiCycleAssignObj);
	if (NULL != objMN1F9BIndex)
	{
		//UpdateNumberOfEnteriesSIdx(objMN1F92Index, MN);
		objMN1F9BIndex->setFlagIfIncludedCdc(TRUE);
		if (NULL != objMN1F9BIndex->getSubIndexbyIndexValue((char*) "00"))
		{
			objMN1F9BIndex->getSubIndexbyIndexValue((char*) "00")->setFlagIfIncludedCdc(
					TRUE);
		}
	}

	strcpy(subIndName, "");
	GetSubIndexAttributes(this->getNodeId(), CN, acMultiCycleAssignObj,
			strConvertedValue, NAME, subIndName);
	SetSubIndexAttributes(this->getNodeId(), CN, acMultiCycleAssignObj,
			strConvertedValue, pbForcedCycle, subIndName, TRUE);

	CIndex* objThis1F9BIndex;
	objThis1F9BIndex = this->m_IndexCollection->getIndexbyIndexValue(
			acMultiCycleAssignObj);
	if (NULL != objThis1F9BIndex)
	{
		objThis1F9BIndex->setFlagIfIncludedCdc(TRUE);
		if (NULL != objThis1F9BIndex->getSubIndexbyIndexValue((char*) "00"))
		{
			objThis1F9BIndex->getSubIndexbyIndexValue((char*) "00")->setFlagIfIncludedCdc(
					TRUE);
		}
	}

	m_ForcedCycle = new char[strlen(pbForcedCycle) + ALLOC_BUFFER];
	strcpy((char*) m_ForcedCycle, pbForcedCycle);

	delete[] subIndName;
	return stErrorResult;
}


/*****************************************************************************/
/**
\brief			resetForcedCycleValue
 
				sets the cycle of the Node

\retval			void
*/
/*****************************************************************************/

void CNode::resetForcedCycleValue()
{
	if (NULL == m_ForcedCycle)
	{
		m_ForcedCycle = new char[strlen("") + STR_ALLOC_BUFFER];
	}
	strcpy((char*) m_ForcedCycle, "");

	char* cSIdx = new char[SUBINDEX_LEN];
	cSIdx = _IntToAscii(m_NodeId, cSIdx, 16);
	cSIdx = padLeft(cSIdx, '0', 2);
	SetSubIndexAttributesByAttribute(MN_NODEID, MN,
			(char *) MULTIPL_CYCLE_ASSIGN_OBJECT, cSIdx, ACTUALVALUE,
			m_ForcedCycle);
	SetSubIndexAttributesByAttribute(m_NodeId, m_NodeType,
			(char *) MULTIPL_CYCLE_ASSIGN_OBJECT, cSIdx, ACTUALVALUE,
			m_ForcedCycle);

	delete[] cSIdx;
}


/*****************************************************************************/
/**
\brief			setPollResponseTimeout
 
				sets the poll response timeout value in MN
				
\param			pbPollResponseTimeout
\retval			void
*/
/*****************************************************************************/

void CNode::setPollResponseTimeout(char* pbPollResponseTimeout)
{
	if (NULL != m_PollResponseTimeout)
	{
		delete[] m_PollResponseTimeout;
	}
	m_PollResponseTimeout = new char[strlen(pbPollResponseTimeout)
			+ STR_ALLOC_BUFFER];
	strcpy((char*) m_PollResponseTimeout, pbPollResponseTimeout);

	//add or update 1f92 subobjects in MN
	ocfmRetCode stErrorInfo;
	INT32 IndexPos = 0;
	INT32 subIndexPos = 0;
	char* strConvertedValue = new char[SUBINDEX_LEN];
	char acMNCNPollresponseTimeoutObj[] = MNCN_POLLRESPONSE_TIMEOUT_OBJECT;

	strConvertedValue = _IntToAscii(this->getNodeId(), strConvertedValue, 16);
	strConvertedValue = padLeft(strConvertedValue, '0', 2);

	stErrorInfo = IfSubIndexExists(MN_NODEID, MN, acMNCNPollresponseTimeoutObj,
			strConvertedValue, &subIndexPos, &IndexPos);
	if (OCFM_ERR_SUCCESS != stErrorInfo.code)
	{
		return;
	}

	char* subIndName = new char[50];
	subIndName[0] = 0;

	GetSubIndexAttributes(240, MN, acMNCNPollresponseTimeoutObj,
			strConvertedValue, NAME, subIndName);
	SetSubIndexAttributes(MN_NODEID, MN, acMNCNPollresponseTimeoutObj,
			strConvertedValue, pbPollResponseTimeout, subIndName, TRUE);
	////////////////No exception is handled from the "SetSubIndexAttributes"
	CIndex* objMN1F92Index = NULL;
	objMN1F92Index = getMNIndexValues(acMNCNPollresponseTimeoutObj);
	if (NULL != objMN1F92Index)
	{
		UpdateNumberOfEnteriesSIdx(objMN1F92Index, MN);
		objMN1F92Index->setFlagIfIncludedCdc(TRUE);
	}

	delete[] subIndName;
}

/*****************************************************************************/
/**
\brief			getForceCycleFlag
 
				This function returns the value of forced cycle flag
				
\retval			BOOL
\retval			TRUE			if successful
\retval			FALSE			if there is already a message pending	
*/
/*****************************************************************************/

bool CNode::getForceCycleFlag()
{
	return m_ForcedCycleFlag;
}


/*****************************************************************************/
/**
\brief			setForceCycleFlag
 
				This function sets the value for forced cycle flag
				
\param			bForceCycleFlag
\retval			void
*/
/*****************************************************************************/

void CNode::setForceCycleFlag(bool bForceCycleFlag)
{
	m_ForcedCycleFlag = bForceCycleFlag;
}


/*****************************************************************************/
/**
\brief			setPResActPayloadValue
 
				This function sets the value for Presact payload
				
\param			iValue
\retval			void
*/
/*****************************************************************************/

void CNode::setPResActPayloadValue(INT32 iValue)
{
	m_PResActPayload = iValue;
}


/*****************************************************************************/
/**
\brief			getPResActPayloadValue
 
				This function returns the value for Presact payload
				
\retval			INT32
*/
/*****************************************************************************/

INT32 CNode::getPResActPayloadValue()
{
	return m_PResActPayload;
}


/*****************************************************************************/
/**
\brief			setPReqActPayloadValue
 
				This function sets the value for Preqact payload
				
\param			iValue
\retval			void
*/
/*****************************************************************************/

void CNode::setPReqActPayloadValue(INT32 iValue)
{
	m_PReqActPayload = iValue;

}


/*****************************************************************************/
/**
\brief			getPReqActPayloadValue
 
				This function returns the value for Preqact payload
				
\retval			INT32
*/
/*****************************************************************************/

INT32 CNode::getPReqActPayloadValue()
{
	return m_PReqActPayload;
}
#ifndef __GNUC__
#pragma endregion Properties
#endif
