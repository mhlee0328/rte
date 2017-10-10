/******************************************************************************
 *
 *   FILE
 *   ----
 *   nodeStoreFunctions.c
 *
 *   History
 *   -------
 *   2014-10-21   File created
 *
 *******************************************************************************
 *
 *   This file is generated by the 'acplt_builder' command
 *
 ******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_servicesOPCUAInterface
#define OV_COMPILE_LIBRARY_servicesOPCUAInterface
#endif

#include "servicesOPCUAInterface.h"
#include "libov/ov_macros.h"
#include "ksbase.h"
#include "opcua.h"
#include "opcua_helpers.h"
#include "NoneTicketAuthenticator.h"
#include "libov/ov_path.h"
#include "libov/ov_memstack.h"
#include "ks_logfile.h"

extern OV_INSTPTR_servicesOPCUAInterface_interface pinterface;

OV_DLLFNCEXPORT UA_StatusCode servicesOPCUAInterface_interface_ovServiceNodeToOPCUA(
		void *handle, const UA_NodeId *nodeId, UA_Node** opcuaNode) {
	UA_Node 				*newNode = NULL;
	UA_StatusCode 			result = UA_STATUSCODE_GOOD;
	OV_PATH 				path;
	OV_INSTPTR_ov_object	pobj = NULL;
	OV_TICKET 				*pTicket = NULL;
	OV_VTBLPTR_ov_object	pVtblObj = NULL;
	OV_ACCESS				access;
	UA_NodeClass 			nodeClass;
	OV_ELEMENT				element;

	ov_memstack_lock();
	result = opcua_nodeStoreFunctions_resolveNodeIdToPath(*nodeId, &path);
	if(result != UA_STATUSCODE_GOOD){
		ov_memstack_unlock();
		return result;
	}
	element = path.elements[path.size-1];
	ov_memstack_unlock();
	result = opcua_nodeStoreFunctions_getVtblPointerAndCheckAccess(&(element), pTicket, &pobj, &pVtblObj, &access);
	if(result != UA_STATUSCODE_GOOD){
		return result;
	}

	nodeClass = UA_NODECLASS_METHOD;
	newNode = (UA_Node*)UA_calloc(1, sizeof(UA_MethodNode));

	// Basic Attribute
	// BrowseName
	UA_QualifiedName qName;
	qName.name = UA_String_fromChars(pobj->v_identifier);
	qName.namespaceIndex = pinterface->v_interfacenamespace.index;
	newNode->browseName = qName;

	// Description
	OV_STRING tempString = pVtblObj->m_getcomment(pobj, &(element));
	UA_LocalizedText lText;
	lText.locale = UA_String_fromChars("en");
	if(tempString){
		lText.text = UA_String_fromChars(tempString);
	} else {
		lText.text = UA_String_fromChars("");
	}
	newNode->description = lText;

	// DisplayName
	lText.locale = UA_String_fromChars("en");
	lText.text = UA_String_fromChars(pobj->v_identifier);
	newNode->displayName = lText;

	// NodeId
	UA_NodeId_copy(nodeId, &newNode->nodeId);

	// NodeClass
	newNode->nodeClass 	= nodeClass;

	// WriteMask
	UA_UInt32 writeMask = 0;
	if(element.elemtype != OV_ET_VARIABLE){
		if(access & OV_AC_WRITE){
			writeMask |= (1<<2);	//	BrowseName
			writeMask |= (1<<6);	//	DisplayName
		}
		if(access & OV_AC_RENAMEABLE){
			writeMask |= (1<<14);	//	NodeId
		}
	}
	newNode->writeMask 	= writeMask;

	((UA_ObjectNode*)newNode)->eventNotifier = 0;

	((UA_MethodNode*)newNode)->executable = TRUE;
	((UA_MethodNode*)newNode)->attachedMethod = servicesOPCUAInterface_interface_MethodCallback;
	((UA_MethodNode*)newNode)->methodHandle = pobj;

	// References
	// References
	addReference(newNode);
	UA_NodeId tmpNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION);

	for (size_t i = 0; i < newNode->referencesSize; i++){
		if (UA_NodeId_equal(&newNode->references[i].referenceTypeId, &tmpNodeId)){
			newNode->references[i].targetId = UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_METHODNODE);
			break;
		}
	}
	UA_NodeId_deleteMembers(&tmpNodeId);

	newNode->referencesSize = newNode->referencesSize+2; //For Input-&Outputarguments
	newNode->referencesSize = newNode->referencesSize+1; //For Has_Component to parent node
	newNode->references = UA_realloc(newNode->references, newNode->referencesSize*sizeof(UA_ReferenceNode));

	// InputArguments
	newNode->references[newNode->referencesSize-3].referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY);
	newNode->references[newNode->referencesSize-3].isInverse = UA_FALSE;
	OV_STRING tmpString = NULL;
	copyOPCUAStringToOV(nodeId->identifier.string, &tmpString);
	ov_string_append(&tmpString, "||");
	ov_string_append(&tmpString, "InputArguments");
	newNode->references[newNode->referencesSize-3].targetId = UA_EXPANDEDNODEID_STRING_ALLOC(pinterface->v_interfacenamespace.index, tmpString);
	ov_string_setvalue(&tmpString, NULL);

	// OutputArguments
	newNode->references[newNode->referencesSize-2].referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY);
	newNode->references[newNode->referencesSize-2].isInverse = UA_FALSE;
	tmpString = NULL;
	copyOPCUAStringToOV(nodeId->identifier.string, &tmpString);
	ov_string_append(&tmpString, "||");
	ov_string_append(&tmpString, "OutputArguments");
	newNode->references[newNode->referencesSize-2].targetId = UA_EXPANDEDNODEID_STRING_ALLOC(pinterface->v_interfacenamespace.index, tmpString);
	ov_string_setvalue(&tmpString, NULL);

	// HasComponent to parent
	newNode->references[newNode->referencesSize-1].referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT);
	newNode->references[newNode->referencesSize-1].isInverse = UA_TRUE;
	tmpString = NULL;
	copyOPCUAStringToOV(nodeId->identifier.string, &tmpString);
	OV_STRING *plist = NULL;
	OV_UINT len = 0;
	plist  = ov_string_split(tmpString, "/", &len);
	ov_string_setvalue(&tmpString, "");
	for (OV_UINT i = 0; i < len-1; i++){
		if (i != 0)
			ov_string_append(&tmpString, "/");
		ov_string_append(&tmpString, plist[i]);
	}
	ov_string_freelist(plist);

	OV_INSTPTR_opcua_uaServer opcua_pUaServer = NULL;
	Ov_ForEachChildEx(ov_instantiation, pclass_opcua_uaServer, opcua_pUaServer, opcua_uaServer){
		break;
	}
	OV_INSTPTR_ov_object pobject = NULL;

	OV_UINT tmpNamespace = 0;
	for (OV_UINT i = 0; i < opcua_pUaServer->v_namespaceNames.veclen; i++){
		pobject = ov_path_getobjectpointer(tmpString, 2);
		OV_INSTPTR_ov_class pclass = Ov_GetClassPtr(pobject);
		OV_INSTPTR_ov_domain plibrary = Ov_GetParent(ov_containment, pclass);
		OV_STRING tmpString2 = NULL;
		ov_string_setvalue(&tmpString2, "http://acplt.org/");
		ov_string_append(&tmpString2, plibrary->v_identifier);
		ov_string_append(&tmpString2, "/Ov");
		if (ov_string_compare(opcua_pUaServer->v_namespaceNames.value[i], tmpString2) == OV_STRCMP_EQUAL){
			tmpNamespace = i;
			break;
		}else{
			tmpNamespace = opcua_pUaServer->v_namespace.index;
		}
		pclass = NULL;
		plibrary = NULL;
		ov_string_setvalue(&tmpString2, NULL);
	}

	newNode->references[newNode->referencesSize-1].targetId = UA_EXPANDEDNODEID_STRING_ALLOC(tmpNamespace, tmpString);
	ov_string_setvalue(&tmpString, NULL);

	*opcuaNode = newNode;
	return UA_STATUSCODE_GOOD;
}

