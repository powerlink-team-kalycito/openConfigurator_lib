%module openConfiguratorWrapper
#define DLLEXPORT
/* Windows Platform */
#if defined(_WIN32) && defined(_MSC_VER)
	 #undef DLLEXPORT
	 #define DLLEXPORT  __declspec(dllexport)
	 
#endif

%{

#include "../openCONFIGURATOR/Include/Exports.h"
#include "../openCONFIGURATOR/Include/Error.h"
#include "../openCONFIGURATOR/Include/Declarations.h"

%}

%include cpointer.i
%include cstring.i
%pointer_functions(char, charp)
%pointer_functions(long, longp)
%pointer_functions(int, intp)
%pointer_functions(bool, boolp)
%pointer_functions(AutoGenerate, AutoGeneratep)
%pointer_functions(AutoSave, AutoSavep)
%pointer_functions(ViewMode, ViewModep)
%pointer_functions(StationType, StationTypep)
%cstring_bounded_output(char *outAttributeValue, 512);
%cstring_bounded_output(char *outIndexID, 100);
%cstring_bounded_output(char *outSubIndexID, 100);
%cstring_bounded_output(char *outNodeName, 100);
%cstring_bounded_output(char *outFeatureValue, 512);
%cstring_bounded_output(char *outDataTypes, 1000);
%cstring_bounded_output(char *outForcedCycle, 10);


%include typemaps.i
%include exception.i
%include std_except.i

%include "../openCONFIGURATOR/Include/Declarations.h"
%include "../openCONFIGURATOR/Include/Error.h"
%include "../openCONFIGURATOR/Include/Exports.h"

using namespace std; 


%inline %{
 ocfmRetCode ImportXML(
		char* fileName, INT32 varNodeID, NodeType varNodeType);
 ocfmRetCode ReImportXML(char* fileName, INT32 varNodeID,
		NodeType varNodeType);
 ocfmRetCode GenerateXAP(char* fileName);
 ocfmRetCode GenerateCDC(char* CDCLocation);
 ocfmRetCode GenerateNET(char* fileName);
 ocfmRetCode CreateNode(INT32 varNodeID, NodeType varNodeType,
		char* nodeName);
 ocfmRetCode DeleteNode(INT32 varNodeID, NodeType varNodeType);
 ocfmRetCode DeleteNodeObjDict(INT32 varNodeID, NodeType varNodeType);
 ocfmRetCode DeleteIndex(INT32 varNodeID, NodeType varNodeType,
		char* indexID);
 ocfmRetCode DeleteSubIndex(INT32 varNodeID, NodeType varNodeType,
		char* indexID, char* subIndexID);
 ocfmRetCode AddIndex(INT32 varNodeID, NodeType varNodeType, char* indexID);
 ocfmRetCode AddSubIndex(INT32 varNodeID, NodeType varNodeType,
		char* iIndexID, char* subIndexID);
 ocfmRetCode SetIndexAttributes(INT32 varNodeID, NodeType varNodeType,
		char* indexID, char* indexValue, char* indexName, Flag flagIfInCdc);
 ocfmRetCode SetSubIndexAttributes(INT32 varNodeID, NodeType varNodeType,
		char* indexID, char* subIndexID, char* indexValue, char* indexName,
		Flag flagIfInCdc);
 ocfmRetCode IfNodeExists(INT32 varNodeID, NodeType varNodeType,
		INT32* nodePos, bool& existfFlag);
 ocfmRetCode IfIndexExists(INT32 varNodeID, NodeType varNodeType,
		char* indexID, INT32* indexPos);
 ocfmRetCode IfSubIndexExists(INT32 varNodeID, NodeType varNodeType,
		char* indexID, char* subIndexID, INT32* subIndexPos, INT32* indexPos);
 ocfmRetCode GetIndexAttributes(INT32 varNodeID, NodeType varNodeType,
		char* indexID, AttributeType attributeType, char* outAttributeValue);
 ocfmRetCode GetSubIndexAttributes(INT32 varNodeID, NodeType varNodeType,
		char* indexID, char* subIndexID, AttributeType attributeType,
		char* outAttributeValue);
 ocfmRetCode GetNodeCount(INT32 mnNodeID, INT32* outNodeCount);
 ocfmRetCode GetIndexCount(INT32 varNodeID, NodeType varNodeType,
		INT32* outIndexCount);
 ocfmRetCode GetSubIndexCount(INT32 varNodeID, NodeType varNodeType,
		char* indexID, INT32* outSubIndexCount);
 ocfmRetCode GetNodeAttributesbyNodePos(INT32 nodePos,
		INT32* outNodeID, char* outNodeName, StationType *outEstationType,
		char* outForcedCycle, bool* bForcedCycleFlag);
 ocfmRetCode GetIndexIDbyIndexPos(INT32 varNodeID, NodeType varNodeType,
		INT32 indexPos, char* outIndexID);
 ocfmRetCode GetSubIndexIDbySubIndexPos(INT32 varNodeID,
		NodeType varNodeType, char* indexID, INT32 subIndexPos,
		char* outSubIndexID);
 ocfmRetCode GetIndexIDbyPositions(INT32 nodePos, INT32 indexPos,
		char* outIndexID);
 ocfmRetCode GetSubIndexIDbyPositions(INT32 nodePos, INT32 indexPos,
		INT32 subIndexPos, char* outSubIndexID);
 ocfmRetCode GetIndexAttributesbyPositions(INT32 nodePos,
		INT32 indexPos, AttributeType attributeType, char* outAttributeValue);
 ocfmRetCode GetSubIndexAttributesbyPositions(INT32 nodePos,
		INT32 indexPos, INT32 subIndexPos, AttributeType attributeType,
		char* outAttributeValue);

 void LoadObjectDictionary(char* fileName);
 ocfmRetCode SaveNode(const char* fileName, INT32 varNodeID,
		NodeType varNodeType);
 ocfmRetCode SaveProject(char* projectPath, char* projectName);
 ocfmRetCode OpenProject(char* pjtPath, char* projectXmlFileName);
 ocfmRetCode FreeProjectMemory();
 ocfmRetCode GetProjectSettings(AutoGenerate* autoGen,
		AutoSave* autoSave, ViewMode* viewMode, bool* bExpertViewAlreadySet);
 ocfmRetCode SetProjectSettings(AutoGenerate autoGen,
		AutoSave autoSave, ViewMode viewMode, bool bExpertViewAlreadySet);
 ocfmRetCode GenerateMNOBD();
 ocfmRetCode SetAllIndexAttributes(INT32 varNodeID, NodeType varNodeType,
		char* indexID, char* actualValue, char* indexName, char* varAccess,
		char* dataTypeValue, char* pdoMappingVal, char* defaultValue,
		char* highLimit, char* lowLimit, char* objType,
		Flag flagIfIncludedInCdc);
 ocfmRetCode SetAllSubIndexAttributes(INT32 varNodeID, NodeType varNodeType,
		char* indexID, char* subIndexID, char* actualValue, char* indexName,
		char* varAccess, char* dataTypeValue, char* pdoMappingVal,
		char* defaultValue, char* highLimit, char* lowLimit, char* objType,
		Flag flagIfIncludedInCdc);
 ocfmRetCode GetFeatureValue(INT32 iNodeId, NodeType varNodeType,
		FeatureType varFeatureType, char* featureName, char* outFeatureValue);
 ocfmRetCode UpdateNodeParams(INT32 iCurrNodeId, INT32 iNewNodeID,
		NodeType varNodeType, char* nodeName, StationType varStationType,
		char* forcedCycle, bool forcedCycleFlag, char* pollResponseTimeout);
 ocfmRetCode GetNodeDataTypes(INT32 iNodeId, NodeType varNodeType,
		char* outDataTypes);
 ocfmRetCode NewProjectNode(INT32 iNodeID, NodeType varNodeType,
		char* nodeName, char * importXmlFile);
 INT32 GetDataSize(char* dataTypeVal);
%}

