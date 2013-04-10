/**
 *****************************************************************************************************
 \file			Declarations.h

 \brief			Handles the declaration and definitions of all functions and datatypes used this application
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

#ifndef declarations_h
#define declarations_h

/****************************************************************************************************/
/* Includes */

#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include "Exports.h"
#include "Error.h"

//using namespace std;

/************************************************************************************************/
/* Constants */

#define MN_NODEID 240
#define BROADCAST_NODEID 0
//TODO: to be removed.Const's are not used.
const int OFFSET_INTEGER_8 = 0;
const int OFFSET_UNSIGNED_INTEGER_8 = 0;

/****************************************************************************************************/
/* Defines */

#define OFFSET_FIRST_INDEX_IN_MN_OBD	4

#define OFFSET_NUM_ENTRIES_MN 0
#define OFFSET_NUM_ENTRIES_CN 7

#define OFFSET_INDEX	0
#define OFFSET_SUBINDEX 2
#define OFFSET_SIZE		3
#define OFFSET_DATA		7
#define	INDEX_SIZE		4
#define	SUBINDEX_SIZE	2
#define NODE_ID			3
#define ERR_STRING_LEN	100
#define OFFSET_FIRST_INDEX_IN_1F22 11
#define MULTIPL_CYCLE_ASSIGN_OBJECT "1F9B"
#define MNCN_POLLRESPONSE_TIMEOUT_OBJECT "1F92"
#define PI_VAR_COUNT 4000
#define PRES_DEFAULT_PAYLOAD 36
#define PREQ_DEFAULT_PAYLOAD 36
#define MAX_CN_CROSS_TRAFFIC_STN 3

/****************************************************************************************************/
/* Typedefs */

typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef signed int INT32;
typedef long int LONG;
typedef unsigned long int ULONG;

/************************************************************************************************
 Enumerations
 ************************************************************************************************/

typedef enum
{
	DEFTYPE = 5, DEFSTRUCT = 6, VAR = 7, ARRAY = 8, RECORD = 9
} ObjectType;

DLLEXPORT typedef enum
{
		NO, DEFAULT, OPTIONAL, RPDO, TPDO
} PDOMapping;

typedef PDOMapping *pdoMapping;

typedef enum
{
	MN = 0, CN = 1
} NodeType;

typedef enum
{
	PDO_TPDO = 1, PDO_RPDO = 2
} PDOType;

typedef enum
{
	GENERAL_FEATURES = 0, MN_FEATURES = 1, CN_FEATURES = 2
} FeatureType;

typedef enum
{
	constant, ro, /* read access only (default value)*/
	wr, /* write access only */
	rw, /* both read and write access*/
	readWriteInput, /*both read and write access, but represents process input data*/
	readWriteOutput, /*both read and write, access, but represents process output data*/
	noAccess /*access denied				*/
} ParameterAccess;

typedef enum IEC_Datatype
{
	BITSTRING = 0,
	BOOL,
	BYTE,
	_CHAR,
	DWORD,
	LWORD,
	SINT,
	INT,
	DINT,
	LINT,
	USINT,
	UINT,
	UDINT,
	ULINT,
	REAL,
	LREAL,
	STRING,
	WSTRING
} IEC_Datatype;

struct DataType
{
		char* Name;
		char* dataTypeValue;
		INT32 dataSize;
		IEC_Datatype IEC_dt;
		char* GetName()
		{
			return (Name);
		}
		//TODO: Review. steName not called
		void SetName(char* dataTypeName)
		{
			Name = new char[strlen(dataTypeName) + 1];
			strcpy(Name, dataTypeName);
		}
};

typedef struct AppProcessCommon
{
		char* Name;
		char* uniqueId;
		char* dataType;
		char* dataTypeUniqueIDRef;

		char* GetUniqueID()
		{
			return (uniqueId);
		}

		void SetUniqueID(char* varUniqueID)
		{
			uniqueId = new char[strlen(varUniqueID) + 1];
			strcpy(uniqueId, varUniqueID);
		}

		char* GetName()
		{
			return (Name);
		}

		void SetName(char* attrName)
		{
			Name = new char[strlen(attrName) + 1];
			strcpy(Name, attrName);
		}

		char* GetDataType()
		{
			return (dataType);
		}

		void SetDataType(char* dt)
		{
			dataType = new char[strlen(dt) + 1];
			strcpy(dataType, dt);
		}

		char* GetDtUniqueRefId()
		{
			return (dataTypeUniqueIDRef);
		}

		void SetDtUniqueRefId(char* uniqueRefID)
		{
			dataTypeUniqueIDRef = new char[strlen(uniqueRefID) + 1];
			strcpy(dataTypeUniqueIDRef, uniqueRefID);
		}
} appProcessCommon;

struct varDeclaration
{
		AppProcessCommon* namIdDtAttr;
		char Size[5];
		char* initialValue; //TODO: unused
		char* structUniqueId;
		void Initialize()
		{
			namIdDtAttr = new AppProcessCommon;
			namIdDtAttr->dataType = NULL;
			namIdDtAttr->dataTypeUniqueIDRef = NULL;
			namIdDtAttr->Name = NULL;
			namIdDtAttr->uniqueId = NULL;
			strcpy(Size, "");
		}
};

typedef struct Parameter
{
		AppProcessCommon nameIdDtAttr;
		char* dataType;	//TODO: unused
		INT32 structIndex; //TODO: unused
		char* Access;
		INT32 paraIndex;
		//char*	dataTypeUniqueIDRef;
} Parameter;

typedef struct Feature
{
		char* Name;
		char* Value;
		FeatureType featureType;
} Feature;

typedef struct MNPdoVariable
{
		char* Index;
		char* SubIndex;
		INT32 Offset; //TODO: unused
		char* Value;
		INT32 DataSize;
		PDOType pdoType;
} MNPdoVariable;

typedef enum
{
	NAME = 0,	// Name of the Attribute
	OBJECTTYPE,		// Object Type of the Attribute
	DATATYPE,		// Data Type of the Attribute
	ACCESSTYPE,		// Access Type of the Attribute
	DEFAULTVALUE,	// Default value of the Attribute
	ACTUALVALUE,	// Actual value of the Attribute
	PDOMAPPING,		// PDO Mapping of the Attribute
	LOWLIMIT,
	HIGHLIMIT,
	FLAGIFINCDC,
} AttributeType;

typedef enum
{
	FALSE = 0, TRUE
} Flag;

typedef enum
{
	DATE = 0, TIME
} DateTime;

typedef enum
{
	NO_AG = 0, YES_AG = 1
} AutoGenerate;

typedef enum
{
	YES_AS = 0, PROMPT_AS, DISCARD_AS
} AutoSave;
typedef enum
{
	SIMPLE = 0, EXPERT

} ViewMode;

typedef enum
{
	AUTOGENERATE = 0, AUTOSAVE
} ProjectSettings;
typedef enum
{
	NORMAL = 0, MULTIPLEXED, CHAINED
} StationType;
/************************************************************************************************
 * Function Declarations
 ****************************************************************************************************/
DLLEXPORT ocfmRetCode ImportXML(
		char* fileName, INT32 varNodeID, NodeType varNodeType);
DLLEXPORT ocfmRetCode ReImportXML(char* fileName, INT32 varNodeID,
		NodeType varNodeType);
DLLEXPORT ocfmRetCode GenerateXAP(char* fileName);
DLLEXPORT ocfmRetCode GenerateCDC(char* CDCLocation);
DLLEXPORT ocfmRetCode GenerateNET(char* fileName);
DLLEXPORT ocfmRetCode CreateNode(INT32 varNodeID, NodeType varNodeType,
		char* nodeName);
DLLEXPORT ocfmRetCode DeleteNode(INT32 varNodeID, NodeType varNodeType);
DLLEXPORT ocfmRetCode DeleteNodeObjDict(INT32 varNodeID, NodeType varNodeType);
DLLEXPORT ocfmRetCode DeleteIndex(INT32 varNodeID, NodeType varNodeType,
		char* indexID);
DLLEXPORT ocfmRetCode DeleteSubIndex(INT32 varNodeID, NodeType varNodeType,
		char* indexID, char* subIndexID);
DLLEXPORT ocfmRetCode AddIndex(INT32 varNodeID, NodeType varNodeType, char* indexID);
DLLEXPORT ocfmRetCode AddSubIndex(INT32 varNodeID, NodeType varNodeType,
		char* iIndexID, char* subIndexID);
DLLEXPORT ocfmRetCode SetIndexAttributes(INT32 varNodeID, NodeType varNodeType,
		char* indexID, char* indexValue, char* indexName, Flag flagIfInCdc);
DLLEXPORT ocfmRetCode SetSubIndexAttributes(INT32 varNodeID, NodeType varNodeType,
		char* indexID, char* subIndexID, char* indexValue, char* indexName,
		Flag flagIfInCdc);
DLLEXPORT ocfmRetCode IfNodeExists(INT32 varNodeID, NodeType varNodeType,
		INT32* nodePos, bool& existfFlag);
DLLEXPORT ocfmRetCode IfIndexExists(INT32 varNodeID, NodeType varNodeType,
		char* indexID, INT32* indexPos);
DLLEXPORT ocfmRetCode IfSubIndexExists(INT32 varNodeID, NodeType varNodeType,
		char* indexID, char* subIndexID, INT32* subIndexPos, INT32* indexPos);
DLLEXPORT ocfmRetCode GetIndexAttributes(INT32 varNodeID, NodeType varNodeType,
		char* indexID, AttributeType attributeType, char* outAttributeValue);
DLLEXPORT ocfmRetCode GetSubIndexAttributes(INT32 varNodeID, NodeType varNodeType,
		char* indexID, char* subIndexID, AttributeType attributeType,
		char* outAttributeValue);
DLLEXPORT ocfmRetCode GetNodeCount(INT32 mnNodeID, INT32* outNodeCount);
DLLEXPORT ocfmRetCode GetIndexCount(INT32 varNodeID, NodeType varNodeType,
		INT32* outIndexCount);
DLLEXPORT ocfmRetCode GetSubIndexCount(INT32 varNodeID, NodeType varNodeType,
		char* indexID, INT32* outSubIndexCount);
DLLEXPORT ocfmRetCode GetNodeAttributesbyNodePos(INT32 nodePos,
		INT32* outNodeID, char* outNodeName, StationType *outEstationType,
		char* outForcedCycle, bool* bForcedCycleFlag);
DLLEXPORT ocfmRetCode GetIndexIDbyIndexPos(INT32 varNodeID, NodeType varNodeType,
		INT32 indexPos, char* outIndexID);
DLLEXPORT ocfmRetCode GetSubIndexIDbySubIndexPos(INT32 varNodeID,
		NodeType varNodeType, char* indexID, INT32 subIndexPos,
		char* outSubIndexID);
DLLEXPORT ocfmRetCode GetIndexIDbyPositions(INT32 nodePos, INT32 indexPos,
		char* outIndexID);
DLLEXPORT ocfmRetCode GetSubIndexIDbyPositions(INT32 nodePos, INT32 indexPos,
		INT32 subIndexPos, char* outSubIndexID);
DLLEXPORT ocfmRetCode GetIndexAttributesbyPositions(INT32 nodePos,
		INT32 indexPos, AttributeType attributeType, char* outAttributeValue);
DLLEXPORT ocfmRetCode GetSubIndexAttributesbyPositions(INT32 nodePos,
		INT32 indexPos, INT32 subIndexPos, AttributeType attributeType,
		char* outAttributeValue);

DLLEXPORT void LoadObjectDictionary(char* fileName);
DLLEXPORT ocfmRetCode SaveNode(const char* fileName, INT32 varNodeID,
		NodeType varNodeType);
DLLEXPORT ocfmRetCode SaveProject(char* projectPath, char* projectName);
DLLEXPORT ocfmRetCode OpenProject(char* pjtPath, char* projectXmlFileName);
DLLEXPORT ocfmRetCode FreeProjectMemory();
DLLEXPORT ocfmRetCode GetProjectSettings(AutoGenerate* autoGen,
		AutoSave* autoSave, ViewMode* viewMode, bool* bExpertViewAlreadySet);
DLLEXPORT ocfmRetCode SetProjectSettings(AutoGenerate autoGen,
		AutoSave autoSave, ViewMode viewMode, bool bExpertViewAlreadySet);
DLLEXPORT ocfmRetCode GenerateMNOBD();
DLLEXPORT ocfmRetCode SetAllIndexAttributes(INT32 varNodeID, NodeType varNodeType,
		char* indexID, char* actualValue, char* indexName, char* varAccess,
		char* dataTypeValue, char* pdoMappingVal, char* defaultValue,
		char* highLimit, char* lowLimit, char* objType,
		Flag flagIfIncludedInCdc);
DLLEXPORT ocfmRetCode SetAllSubIndexAttributes(INT32 varNodeID, NodeType varNodeType,
		char* indexID, char* subIndexID, char* actualValue, char* indexName,
		char* varAccess, char* dataTypeValue, char* pdoMappingVal,
		char* defaultValue, char* highLimit, char* lowLimit, char* objType,
		Flag flagIfIncludedInCdc);
DLLEXPORT ocfmRetCode GetFeatureValue(INT32 iNodeId, NodeType varNodeType,
		FeatureType varFeatureType, char* featureName, char* outFeatureValue);
DLLEXPORT ocfmRetCode UpdateNodeParams(INT32 iCurrNodeId, INT32 iNewNodeID,
		NodeType varNodeType, char* nodeName, StationType varStationType,
		char* forcedCycle, bool forcedCycleFlag, char* pollResponseTimeout);
DLLEXPORT ocfmRetCode GetNodeDataTypes(INT32 iNodeId, NodeType varNodeType,
		char* outDataTypes);
DLLEXPORT ocfmRetCode NewProjectNode(INT32 iNodeID, NodeType varNodeType,
		char* nodeName, char * importXmlFile);
DLLEXPORT INT32 GetDataSize(char* dataTypeVal);
#ifndef __GNUC__
#pragma warning( disable: 4251 )
#endif
#endif // declarations_h
