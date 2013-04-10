/**
 *****************************************************************************************************
 \file			NodeCollection.h

 \brief			Handles collection of each node's information on id,name,type present inside the network
 *****************************************************************************************************
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
#ifndef NodeCollection_h
#define NodeCollection_h

/****************************************************************************************************/
/* Includes */

#include "Declarations.h"
#include "Node.h"

/**
 ******************************************************************************************************
 \class			CNodeCollection
 \brief			Hanldes collection of each node's information on id,name,type present inside the network	
 
 ******************************************************************************************************/
class NodeCollection
{
	public:
		NodeCollection(void);
		~NodeCollection(void);

	private:
		TCollection<Node> collectionObj;
		INT32 NodeCount;
		static bool instanceFlag;
		static NodeCollection *objNodeCollection;
	public:
		INT32 GetNumberOfNodes();
		void AddNode(Node objNode);
		void DeleteNode(INT32 nodeID);
		static NodeCollection* GetNodeColObjectPointer();
		Node GetNode(NodeType varNodeType, INT32 varNodeId);
		Node GetNode(INT32 nodeId);
		Node GetNodebyCollectionIndex(INT32 colIndex);
		Node GetMNNode();
		Node* GetNodePtr(NodeType varNodeType, INT32 varNodeId);
		Node* GetNodebyColIndex(INT32 colIndex);
		INT32 GetCNNodesCount();
		static NodeCollection GetNodeColObject();
};
#endif // NodeCollection_h
