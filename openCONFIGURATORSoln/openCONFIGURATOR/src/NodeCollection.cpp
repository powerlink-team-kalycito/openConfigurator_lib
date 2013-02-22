/**
************************************************************************************************
\file			NodeCollection.cpp

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
#include "../Include/NodeCollection.h"
#include "../Include/Node.h"
//$S Only for testing


/****************************************************************************************************/
/* Global Variables */

bool CNodeCollection::instanceFlag = false;
CNodeCollection* CNodeCollection::objNodeCollection = NULL;



				//==========================================================================//
				// 				F U N C T I O N  D E F I N I T I O N S  					//
				//==========================================================================//
				
				
				
/*************************************************************************/
/* Constructor */

/**
        
    @param void
*/

CNodeCollection::CNodeCollection(void)
{
	m_NodeCount = collectionObj.Count();
}


/*************************************************************************/
/* Destructor */

/**
        
    @param void
*/

CNodeCollection::~CNodeCollection(void)
{
	instanceFlag = false;
}


/*****************************************************************************/
/**
\brief			getNumberOfNodes
 
				This function returns node count

\retval			INT32
*/
/*****************************************************************************/

INT32 CNodeCollection::getNumberOfNodes()
{
	return m_NodeCount;
}


 
/*****************************************************************************/
/**
\brief			addNode
 
				This function collects objects under node count
				
\param			objNode
\retval			void
*/
/*****************************************************************************/

void CNodeCollection::addNode(CNode objNode)
{
	INT32 iItemPosition = collectionObj.Add();

	collectionObj[iItemPosition] = objNode;
	m_NodeCount = collectionObj.Count();
}



/*****************************************************************************/
/**
\brief			deleteNode
 
				This function deletes the node ids
				
\param			iNodeID
\retval			void
*/
/*****************************************************************************/

void CNodeCollection::deleteNode(INT32 iNodeID)
{
	collectionObj.Remove(iNodeID);
	m_NodeCount = collectionObj.Count();
}


/*****************************************************************************/
/**
\brief			getNodeColObjectPointer
 
				This function collects the location of node
				
\retval			CNodeCollection*
*/
/*****************************************************************************/

CNodeCollection* CNodeCollection::getNodeColObjectPointer()
{
	if (!instanceFlag)
	{
		objNodeCollection = new CNodeCollection();
		instanceFlag = true;
	}
	return objNodeCollection;
}


/*****************************************************************************/
/**
\brief			getNodeColObject
 
				This function collects value of node objects
				
\retval			CNodeCollection
*/
/*****************************************************************************/

CNodeCollection CNodeCollection::getNodeColObject()
{
	if (!instanceFlag)
	{
		objNodeCollection = new CNodeCollection();
		instanceFlag = true;
	}
	return *objNodeCollection;
}


/*****************************************************************************/
/**
\brief			getNode
 
				This function collects the type of node objects

\param			enumNodeType
\param			iNodeID				
\retval			CNode
*/
/*****************************************************************************/

CNode CNodeCollection::getNode(ENodeType enumNodeType, INT32 iNodeID)
{
	INT32 iLoopCount = 0;
	CNode objNode;

	for (iLoopCount = 0; iLoopCount < m_NodeCount; iLoopCount++)
	{
		objNode = objNodeCollection->collectionObj[iLoopCount];

		if (objNode.getNodeType() == enumNodeType
				&& objNode.getNodeId() == iNodeID)
		{
			return objNode;
		}
	}
	return objNode;
}


/*****************************************************************************/
/**
\brief			getNodePtr
 
				This function collects the location of node objects type 

\param			enumNodeType
\param			iNodeID				
\retval			CNode*
*/
/*****************************************************************************/

CNode* CNodeCollection::getNodePtr(ENodeType enumNodeType, INT32 iNodeID)
{
	INT32 iLoopCount = 0;
	CNode* pobjNode = NULL;

	for (iLoopCount = 0; iLoopCount < m_NodeCount; iLoopCount++)
	{
		pobjNode = &(objNodeCollection->collectionObj[iLoopCount]);

		if (pobjNode->getNodeType() == enumNodeType
				&& pobjNode->getNodeId() == iNodeID)
		{
			return pobjNode;
		}
	}
	return NULL;
}


/*****************************************************************************/
/**
\brief			getNode
 
				This function collects the type of node objects 

\param			iNodeID				
\retval			CNode
*/
/*****************************************************************************/

CNode CNodeCollection::getNode(INT32 iNodeID)
{
	INT32 iLoopCount = 0;
	CNode objNode;

	for (iLoopCount = 0; iLoopCount < m_NodeCount; iLoopCount++)
	{
		objNode = objNodeCollection->collectionObj[iLoopCount];

		if (objNode.getNodeId() == iNodeID)
		{
			return objNode;
		}
	}
	return objNode;
}


/*****************************************************************************/
/**
\brief			getMNNode
 
				This function collects the type of node objects for MN

\retval			CNode
*/
/*****************************************************************************/

CNode CNodeCollection::getMNNode()
{
	INT32 iLoopCount;
	CNode objNode;

	for (iLoopCount = 0; iLoopCount < m_NodeCount; iLoopCount++)
	{
		objNode = objNodeCollection->collectionObj[iLoopCount];

		if (objNode.getNodeType() == MN)
		{
			return objNode;
		}

	}
	return objNode;
}


/*****************************************************************************/
/**
\brief			getNodebyCollectionIndex
 
				This function returns node from the node collection index

\param			iColIndex
\retval			CNode
*/
/*****************************************************************************/

CNode CNodeCollection::getNodebyCollectionIndex(INT32 iColIndex)
{
	return collectionObj[iColIndex];
}



/*****************************************************************************/
/**
\brief			getNodebyColIndex
 
				This function collects the node with reference to the index location 

\param			iColIndex
\retval			CNode*
*/
/*****************************************************************************/


CNode* CNodeCollection::getNodebyColIndex(INT32 iColIndex)
{
	return &collectionObj[iColIndex];
}


/*****************************************************************************/
/**
\brief			getCNNodesCount
 
				This function collects the CN node count 

\retval			INT32
*/
/*****************************************************************************/

INT32 CNodeCollection::getCNNodesCount()
{
	INT32 iCNNodecount = 0;

	for (INT32 iLoopCount = 0; iLoopCount < collectionObj.Count(); iLoopCount++)
	{
		if (collectionObj[iLoopCount].getNodeType() == CN)
		{
			iCNNodecount++;
		}
	}
	return iCNNodecount;
}
