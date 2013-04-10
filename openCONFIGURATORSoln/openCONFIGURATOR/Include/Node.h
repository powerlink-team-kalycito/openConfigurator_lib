/**
 *****************************************************************************************************
 \file			Node.h

 \brief			Handles each node's information on id,name,type present inside the network
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
#ifndef Node_h
#define Node_h

/****************************************************************************************************/
/* Includes */

#include "DataTypeCollection.h"
#include "IndexCollection.h"
#include "Declarations.h"
#include "ApplicationProcess.h"
#include "ProcessImage.h"
#include "NetworkManagement.h"

/**
 ******************************************************************************************************
 \class			CNode
 \brief			This class includes each node's information on id,name,type present inside the network
 
 ******************************************************************************************************/
class Node
{
	public:
		Node(void);
		~Node(void);
	private:
		DataTypeCollection *VarDataTypeCollection;
		IndexCollection *VarIndexCollection;
		ApplicationProcess* VarApplicationProcess;
		NetworkManagement *VarNetworkManagement;

		INT32 VarNodeId;
		NodeType VarNodeType;
		INT32 NodeIndex;
		bool VarHasPdoObjects;
		char* VarNodeName;
		StationType VarStationType;
		char* VarForcedCycle;
		bool VarForcedCycleFlag;
		char* VarPollResponseTimeout;
		INT32 VarPResActPayload;
		INT32 VarPReqActPayload;
		/*	typedef TCollection<ProcessImage> PICollection;*/
	public:
		/*typedef _PICollection PICollection;*/
		TCollection<ProcessImage> ProcessImageCollection;
		TCollection<MNPdoVariable> MNPDOINVarCollection;
		TCollection<MNPdoVariable> MNPDOOUTVarCollection;
		TCollection<NETProcessImage> NETProcessImageCollection;
	public:
		DataTypeCollection* GetDataTypeCollection();
		IndexCollection* GetIndexCollection();
		ApplicationProcess* GetApplicationProcess();
		NetworkManagement *GetNetworkManagement();

		INT32 GetNodeId();
		void SetNodeId(INT32 NodeId);

		INT32 GetNodeIndex();
		void SetNodeIndex(INT32 nodeIndex);

		char* GetNodeName();
		void SetNodeName(char* nodeName);

		NodeType GetNodeType();
		void SetNodeType(NodeType varNodeType);

		bool HasPdoObjects();
		void SetFlagForPdoObjects(bool flag);

		void CreateIndexCollection();
		void CreateDataTypeCollection();
		void CreateApplicationProcess();
		void CreateNetworkManagament();

		void AddProcessImage(ProcessImage processImage);
		void AddNETProcessImage(NETProcessImage objNETProcessImage);
		void AddMNPDOvar(MNPdoVariable variable, PDOType varPDOType);

		IndexCollection* GetPDOIndexCollection(PDOType varPDOType);
		IndexCollection* GetIndexCollectionWithoutPDO();
		IndexCollection* getPDOIndexCollection(INT32 *rpdoCount, INT32 *tpdoCount);

		ProcessImage* GetPIbyParaIndex(INT32 paraIndex);
		void DeleteCollectionsForPI();
		StationType GetStationType();
		void SetStationType(StationType varStationType);

		char* GetForcedCycle();
		bool GetForceCycleFlag();
		ocfmRetCode SetForcedCycle(char* forcedCycle);
		void ResetForcedCycleValue();
		void SetForceCycleFlag(bool forceCycleFlag);
		void SetPollResponseTimeout(char* pollResponseTimeout);
		bool IsNull();
		void DeleteCollectionsForNETPI();
		void SetPResActPayloadValue(INT32);
		INT32 GetPResActPayloadValue();
		void SetPReqActPayloadValue(INT32);
		INT32 GetPReqActPayloadValue();
};
#endif // Node_h
