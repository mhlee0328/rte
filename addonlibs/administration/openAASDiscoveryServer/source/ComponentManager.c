
/******************************************************************************
*
*   FILE
*   ----
*   ComponentManager.c
*
*   History
*   -------
*   2017-05-19   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_openAASDiscoveryServer
#define OV_COMPILE_LIBRARY_openAASDiscoveryServer
#endif


#include "openAASDiscoveryServer.h"
#include "libov/ov_macros.h"
#include "MessageSys_helpers.h"
#include "fb_database.h"
#include "libov/ov_result.h"

OV_DLLFNCEXPORT OV_ACCESS openAASDiscoveryServer_ComponentManager_getaccess(
	OV_INSTPTR_ov_object	pobj,
	const OV_ELEMENT		*pelem,
	const OV_TICKET			*pticket
) {
    /*    
    *   local variables
    */
	return (OV_ACCESS)OV_AC_WRITE | OV_AC_READ | OV_AC_LINKABLE | OV_AC_UNLINKABLE | OV_AC_DELETEABLE | OV_AC_RENAMEABLE;
}

OV_DLLFNCEXPORT OV_RESULT openAASDiscoveryServer_ComponentManager_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_openAASDiscoveryServer_ComponentManager pinst = Ov_StaticPtrCast(openAASDiscoveryServer_ComponentManager, pobj);
    OV_RESULT    result;

    /* do what the base class does first */
    result = fb_functionblock_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */
    pinst->v_iexreq = TRUE;
	pinst->v_actimode = 1;
	OV_INSTPTR_fb_task pTaskParent = NULL;
	pinst->v_messageBoxIsFiFo = TRUE;
	pinst->v_messageBoxSize = 10;
	pTaskParent = (OV_INSTPTR_fb_task)fb_database_geturtask();
	if (pTaskParent != NULL){
		Ov_Link(fb_tasklist, pTaskParent, pinst);
	}
	else {
		ov_logfile_error("Cannot link StateMachine to tasklist", pinst->v_identifier);
	}


    return OV_ERR_OK;
}


static OV_INSTPTR_MessageSys_Message getNextMessage(OV_INSTPTR_openAASDiscoveryServer_ComponentManager this) {
	OV_INSTPTR_ov_object child = NULL;

	//Check if we have a Message if(FiFo) else(Stack)
	if (this->v_messageBoxIsFiFo == TRUE) {
		// Get the first child that can be casted to Message.
		child = Ov_GetFirstChild(ov_containment, &this->p_INBOX);
		while (child && !Ov_CanCastTo(MessageSys_Message , child)) {
			child = Ov_GetNextChild(ov_containment, child);
		}
	} else {
		// Get the last child that can be casted to Message.
		child = Ov_GetLastChild(ov_containment, &this->p_INBOX);
		while (child && !Ov_CanCastTo(MessageSys_Message , child)) {
			child = Ov_GetPreviousChild(ov_containment, child);
		}
	}
	//After getting next child, because it might have been the outbox check if it is a valid child now.
	//If not jump out, because the only childs of ServiceAdministrator are messages or the single outbox.
	if (child == NULL || !Ov_CanCastTo(MessageSys_Message , child)) {
		return NULL;
	}
	return (OV_INSTPTR_MessageSys_Message) child;
}

OV_DLLFNCEXPORT void openAASDiscoveryServer_ComponentManager_cleanupMessageBox(OV_INSTPTR_openAASDiscoveryServer_ComponentManager this) {
	int nMessagesToDelete = 0;
	int nMessagesDeleted = 0;
	OV_INSTPTR_ov_object pChild = NULL;
	OV_INSTPTR_MessageSys_Message pMessageToDelete = NULL;

	// Calculate the number of messages to delete.
	Ov_ForEachChild(ov_containment, &this->p_INBOX, pChild) {
		if (Ov_CanCastTo(MessageSys_Message, pChild)) {
			nMessagesToDelete++;
		}
	}
	nMessagesToDelete = nMessagesToDelete - this->v_messageBoxSize;

	// Delete the messages.
	// TODO: To be optimized.
	while (nMessagesDeleted < nMessagesToDelete) {
		// Find a message to delete.
		pMessageToDelete = NULL;
		Ov_ForEachChild(ov_containment, &this->p_INBOX, pChild) {
			if (!Ov_CanCastTo(MessageSys_Message, pChild)) {
				continue;
			}
			if (this->v_messageBoxIsFiFo) {
				if (pMessageToDelete == NULL || ov_time_compare(&pMessageToDelete->v_creationtime, &pChild->v_creationtime) == OV_TIMECMP_AFTER) {
					pMessageToDelete = (OV_INSTPTR_MessageSys_Message) pChild;
				}
			} else {
				if (pMessageToDelete == NULL || ov_time_compare(&pMessageToDelete->v_creationtime, &pChild->v_creationtime) == OV_TIMECMP_BEFORE) {
					pMessageToDelete = (OV_INSTPTR_MessageSys_Message) pChild;
				}
			}
		}
		// Delete the message.
		Ov_DeleteObject(pMessageToDelete);
		nMessagesDeleted++;
	}
	return;
}



OV_DLLFNCEXPORT void openAASDiscoveryServer_ComponentManager_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    /*    
    *   local variables
    */
	OV_INSTPTR_openAASDiscoveryServer_ComponentManager pinst = Ov_StaticPtrCast(openAASDiscoveryServer_ComponentManager, pfb);
	OV_RESULT resultOV = OV_ERR_OK;
	OV_INSTPTR_MessageSys_Message message = NULL;
	OV_STRING tempString = NULL;
	OV_INSTPTR_MessageSys_Message panswerMessage = NULL;
	OV_INSTPTR_MessageSys_MsgDelivery pmsgDelivery = NULL;

	// First clean message box, so only up-to-date messages are handled.
	openAASDiscoveryServer_ComponentManager_cleanupMessageBox(pinst);
	// Get the next message, if any.
	message = getNextMessage(pinst);
	if (message == NULL) {
		// Nothing to do.
		return;
	}

	// Decoding the Message
	OV_STRING messageContent = NULL;
	OV_STRING answerMessage = NULL;

	// XML Decoding
	if (strncmp(message->v_msgBody, "<bdy>", 5) != 0){ // <bdy> not found
		Ov_DeleteObject((OV_INSTPTR_ov_object) message);
		ov_logfile_info("ComponentManager: <bdy> not found in msg");
		return;
	}
	OV_UINT messageLength = ov_string_getlength(message->v_msgBody);
	if (strncmp(&message->v_msgBody[messageLength-6], "</bdy>", 6) != 0){ // </bdy> not found
		Ov_DeleteObject((OV_INSTPTR_ov_object) message);
		ov_logfile_info("ComponentManager: </bdy> not found in msg");
		return;
	}
	messageContent = malloc((messageLength-11+1)*sizeof(char));
	memcpy(messageContent, (message->v_msgBody+5), messageLength-11);
	messageContent[messageLength-11] = '\0';

	OV_UINT len = 0;
	OV_STRING *plistReq = NULL;
	plistReq= ov_string_split(messageContent, ":", &len);
	OV_UINT len2 = 0;
	OV_STRING *plistParameter = NULL;
	OV_STRING parameter = NULL;
	for (OV_UINT i = 1; i < len; i++){
		ov_string_append(&parameter, plistReq[i]);
		if (i != len-1)
			ov_string_append(&parameter, ":");
	}
	plistParameter= ov_string_split(parameter, ",", &len2);
	ov_string_setvalue(&parameter, NULL);
	free(messageContent);
	if (ov_string_compare(plistReq[0], "RegisterAASReq") == OV_STRCMP_EQUAL){ // register Request
		if (len2 == 7){
			// TODO: Check of parameter format

			OV_STRING tmpHexStringAAS = NULL;
			ov_string_print(&tmpHexStringAAS, "%x", *plistParameter[0]);

			for (OV_UINT i = 0; i < ov_string_getlength(plistParameter[1]); i++){
				OV_STRING tmpHexString2 = NULL;
				ov_string_print(&tmpHexString2, "%x", plistParameter[1][i]);
				ov_string_append(&tmpHexStringAAS, tmpHexString2);
				ov_string_setvalue(&tmpHexString2, NULL);
			}

			OV_STRING tmpHexStringAsset = NULL;
			ov_string_print(&tmpHexStringAsset, "%x", *plistParameter[2]);

			for (OV_UINT i = 0; i < ov_string_getlength(plistParameter[3]); i++){
				OV_STRING tmpHexString2 = NULL;
				ov_string_print(&tmpHexString2, "%x", plistParameter[3][i]);
				ov_string_append(&tmpHexStringAsset, tmpHexString2);
				ov_string_setvalue(&tmpHexString2, NULL);
			}

			OV_RESULT result;
			OV_INSTPTR_openAASDiscoveryServer_OVDataForAAS pOvDataForAAS = NULL;
			OV_INSTPTR_openAASDiscoveryServer_DiscoveryServer pDiscoveryServer = NULL;
			pDiscoveryServer = Ov_StaticPtrCast(openAASDiscoveryServer_DiscoveryServer, pinst->v_pouterobject);

			pOvDataForAAS = Ov_StaticPtrCast(openAASDiscoveryServer_OVDataForAAS, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, &pDiscoveryServer->p_AASIDs), tmpHexStringAAS));
			if(!pOvDataForAAS){
				OV_INSTPTR_openAASDiscoveryServer_OVDataForAssetID pOvDataForAsset = NULL;
				pOvDataForAsset = Ov_StaticPtrCast(openAASDiscoveryServer_OVDataForAssetID, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, &pDiscoveryServer->p_AssetIDs), tmpHexStringAsset));
				if(!pOvDataForAsset){
					result = Ov_CreateObject(openAASDiscoveryServer_OVDataForAAS, pOvDataForAAS, Ov_StaticPtrCast(ov_domain, &pDiscoveryServer->p_AASIDs), tmpHexStringAAS);
					if (result){
						Ov_DeleteObject(pOvDataForAAS);
						ov_string_setvalue(&answerMessage, "RegisterAASRes:Failed,Internal");
						ov_logfile_info("ComponentManager:Could not create OvDataForAAS object");
					}else{
						result = Ov_CreateObject(openAASDiscoveryServer_OVDataForAssetID, pOvDataForAsset, Ov_StaticPtrCast(ov_domain, &pDiscoveryServer->p_AssetIDs), tmpHexStringAsset);
						if (result){
							ov_string_setvalue(&answerMessage, "RegisterAASRes:Failed,Internal");
							resultOV = Ov_DeleteObject(pOvDataForAAS);
							ov_logfile_info("ComponentManager:Could not create OvDataForAsset object");
						}else{
							pOvDataForAsset->v_AASIDType = atoi(plistParameter[0]);
							ov_string_setvalue(&pOvDataForAsset->v_AASIDString, plistParameter[1]);
							ov_string_setvalue(&pOvDataForAAS->v_ServerHost, plistParameter[4]);
							ov_string_setvalue(&pOvDataForAAS->v_ServerName, plistParameter[5]);
							ov_string_setvalue(&pOvDataForAAS->v_Path, plistParameter[6]);
							ov_string_setvalue(&answerMessage, "RegisterAASRes:OK");
						}
					}
				}else{
					resultOV = Ov_DeleteObject(pOvDataForAAS);
					ov_string_setvalue(&answerMessage, "RegisterAASRes:Failed, Asset-ID already exist");
				}
			}else{
				ov_string_setvalue(&answerMessage, "RegisterAASRes:Failed, ASS-ID already exist");
			}
			ov_string_setvalue(&tmpHexStringAAS, NULL);
			ov_string_setvalue(&tmpHexStringAsset, NULL);
		}else{// Error Parameter have to be: AASIdString, AASIdType, IP, ServerName, PathToComponentManager
			ov_string_setvalue(&answerMessage, "RegisterAASRes:Failed,Incorrect parameter size for RegisterAASReq");
			ov_logfile_info("ComponentManager:Incorrect parameter size for RegisterAASReq");
		}

	}else if (ov_string_compare(plistReq[0], "UnregisterAASReq") == OV_STRCMP_EQUAL){ // unregister Request
		if (len2 == 3){
			// TODO: Check of parameter format
			OV_STRING tmpHexStringAAS = NULL;
			ov_string_print(&tmpHexStringAAS, "%x", *plistParameter[0]);

			for (OV_UINT i = 0; i < ov_string_getlength(plistParameter[1]); i++){
				OV_STRING tmpHexString2 = NULL;
				ov_string_print(&tmpHexString2, "%x", plistParameter[1][i]);
				ov_string_append(&tmpHexStringAAS, tmpHexString2);
				ov_string_setvalue(&tmpHexString2, NULL);
			}

			OV_STRING tmpHexStringAsset = NULL;
			ov_string_print(&tmpHexStringAsset, "%x", *plistParameter[2]);

			for (OV_UINT i = 0; i < ov_string_getlength(plistParameter[3]); i++){
				OV_STRING tmpHexString2 = NULL;
				ov_string_print(&tmpHexString2, "%x", plistParameter[3][i]);
				ov_string_append(&tmpHexStringAsset, tmpHexString2);
				ov_string_setvalue(&tmpHexString2, NULL);
			}

			OV_INSTPTR_openAASDiscoveryServer_OVDataForAAS pOvDataForAAS = NULL;
			OV_INSTPTR_openAASDiscoveryServer_DiscoveryServer pDiscoveryServer = NULL;
			pDiscoveryServer = Ov_StaticPtrCast(openAASDiscoveryServer_DiscoveryServer, pinst->v_pouterobject);

			pOvDataForAAS = Ov_StaticPtrCast(openAASDiscoveryServer_OVDataForAAS, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, &pDiscoveryServer->p_AASIDs), tmpHexStringAAS));
			if(!pOvDataForAAS){
				OV_INSTPTR_openAASDiscoveryServer_OVDataForAssetID pOvDataForAsset = NULL;
				resultOV = Ov_DeleteObject(pOvDataForAAS);
				if (resultOV){
					ov_string_setvalue(&answerMessage, "UnregisterAASRes:Failed, Internal");
					ov_logfile_info("ComponentManager:Could not delete OvDataForAsset object");
				}
				pOvDataForAsset = Ov_StaticPtrCast(openAASDiscoveryServer_OVDataForAssetID, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, &pDiscoveryServer->p_AssetIDs), tmpHexStringAsset));
				if(!pOvDataForAsset){
					resultOV = Ov_DeleteObject(pOvDataForAsset);
					if (resultOV){
						ov_string_setvalue(&answerMessage, "UnregisterAASRes:Failed, Internal");
						ov_logfile_info("ComponentManager:Could not delete OvDataForAsset object");
					}
				}
			}else{
				ov_string_setvalue(&answerMessage, "UnregisterAASRes:Sucessfull");
			}
			ov_string_setvalue(&tmpHexStringAAS, NULL);
			ov_string_setvalue(&tmpHexStringAsset, NULL);
		}else{// Error Parameter have to be: AASIdString, AASIdType, AssetIDString, AssetIdType
			ov_string_setvalue(&answerMessage, "UnregisterAASRes:Failed,Incorrect parameter size for UnregisterAASReq");
			ov_logfile_info("ComponentManager:Incorrect parameter size for UnregisterAASReq");
		}
	}else if (ov_string_compare(plistReq[0], "GetAASReq") == OV_STRCMP_EQUAL){ // get Request
		if (len2 == 2){ // Error Parameter have to be: AASIdString, AASIdType
			// TODO: Check of parameter format
			OV_STRING tmpHexString = NULL;
			ov_string_print(&tmpHexString, "%x", *plistParameter[1]);

			for (OV_UINT i = 0; i < ov_string_getlength(plistParameter[0]); i++){
				OV_STRING tmpHexString2 = NULL;
				ov_string_print(&tmpHexString2, "%x", plistParameter[0][i]);
				ov_string_append(&tmpHexString, tmpHexString2);
				ov_string_setvalue(&tmpHexString2, NULL);
			}

			OV_INSTPTR_openAASDiscoveryServer_OVDataForAAS pOvDataForAAS = NULL;
			pOvDataForAAS = Ov_StaticPtrCast(openAASDiscoveryServer_OVDataForAAS, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, pinst->v_pouterobject), tmpHexString));
			ov_string_setvalue(&tmpHexString, NULL);
			if(pOvDataForAAS){
				ov_string_setvalue(&answerMessage, "GetAASRes:OK,");
				ov_string_append(&answerMessage, pOvDataForAAS->v_ServerHost);
				ov_string_append(&answerMessage, ",");
				ov_string_append(&answerMessage, pOvDataForAAS->v_ServerName);
				ov_string_append(&answerMessage, ",");
				ov_string_append(&answerMessage, pOvDataForAAS->v_Path);
			}else{
				ov_string_setvalue(&answerMessage, "GetAASRes:Failed,AASId not registered");
				ov_logfile_info("ComponentManager:AASId not found");
			}
		}else{
			ov_string_setvalue(&answerMessage, "UnregisterAASRes:Failed,Incorrect parameter size for GetAASReq");
			ov_logfile_info("ComponentManager:Incorrect parameter size for GetAASReq");
		}
	}else{ // Error
		Ov_DeleteObject((OV_INSTPTR_ov_object) message);
		ov_logfile_info("ComponentManager: Request unknown or not implemented yet");
		return;
	}

	ov_string_freelist(plistReq);
	ov_string_freelist(plistParameter);

	// Create MessageObject in Outbox
	ov_string_setvalue(&tempString, NULL);
	ov_string_setvalue(&tempString, "answerMessageTo");
	ov_string_append(&tempString, message->v_identifier);
	resultOV = Ov_CreateObject(MessageSys_Message, panswerMessage, &pinst->p_OUTBOX, tempString);
	ov_string_setvalue(&tempString, NULL);
	if(Ov_Fail(resultOV)){
		Ov_DeleteObject((OV_INSTPTR_ov_object) message);
		ov_logfile_error("Could not create an answerMessage. Reason: %s", ov_result_getresulttext(resultOV));
		return;
	}

	// Sender = Receiver of old message
	MessageSys_Message_senderAddress_set(panswerMessage, message->v_receiverAddress);
	MessageSys_Message_senderName_set(panswerMessage, message->v_receiverName);
	MessageSys_Message_senderComponent_set(panswerMessage, message->v_receiverComponent);

	// Receiver = Sender of old message
	MessageSys_Message_receiverAddress_set(panswerMessage, message->v_senderAddress);
	MessageSys_Message_receiverName_set(panswerMessage, message->v_senderName);
	MessageSys_Message_receiverComponent_set(panswerMessage, message->v_senderComponent);

	// reference Msg
	ov_string_setvalue(&panswerMessage->v_refMsgID, message->v_msgID);

	// Body
	// XML Encoding
	OV_STRING answerBody = NULL;
	ov_string_setvalue(&answerBody, "<bdy>");
	ov_string_append(&answerBody, answerMessage);
	ov_string_setvalue(&answerMessage, NULL);
	ov_string_append(&answerBody, "</bdy>");
	ov_string_setvalue(&panswerMessage->v_msgBody, answerBody);
	ov_string_setvalue(&answerBody, NULL);

	// Message ready for sending
	panswerMessage->v_msgStatus = MSGREADYFORSENDING;

	// Search for MsgDelivery object
	Ov_ForEachChildEx(ov_instantiation, pclass_MessageSys_MsgDelivery, pmsgDelivery, MessageSys_MsgDelivery){
		if(ov_string_compare(pmsgDelivery->v_identifier, "MessageSys") == OV_STRCMP_EQUAL){
			break;
		}
	}
	if (!pmsgDelivery){
		Ov_DeleteObject((OV_INSTPTR_ov_object) message);
		Ov_DeleteObject((OV_INSTPTR_ov_object) panswerMessage);
		ov_logfile_error("Could not find MessageSys");
		return;
	}

	// send Message
	if (MessageSys_MsgDelivery_sendMessage(pmsgDelivery, panswerMessage) == FALSE){
		Ov_DeleteObject((OV_INSTPTR_ov_object) message);
		Ov_DeleteObject((OV_INSTPTR_ov_object) panswerMessage);
		ov_logfile_error("Could not send Message");
		return;
	}

	// delete all used memory
	Ov_DeleteObject((OV_INSTPTR_ov_object) message);
	return;
}

