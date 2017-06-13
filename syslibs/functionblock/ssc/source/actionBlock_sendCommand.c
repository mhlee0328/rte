/******************************************************************************
*
*   FILE
*   ----
*   sendCommand.c
*
*   History
*   -------
*   2015-01-08   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_ssc
#define OV_COMPILE_LIBRARY_ssc
#endif


#include "ssclib.h"

/**
 * gets an object pointer from a sendcommand object
 * @param pinst
 * @param nameToCheck
 * @param pTargetObj will be NULL if object is not found
 * @return
 */
OV_RESULT ssc_getObjectFromSendCommand(
		const OV_INSTPTR_ssc_sendCommand pinst,
		const OV_STRING nameToCheck,
		OV_INSTPTR_ov_object *pTargetObj
) {
	OV_STRING pathRelativeobject = NULL;
	OV_INSTPTR_ssc_step pStep = Ov_DynamicPtrCast(ssc_step, Ov_GetParent(ov_containment, pinst));
	OV_INSTPTR_ssc_SequentialStateChart activeHeader = Ov_DynamicPtrCast(ssc_SequentialStateChart, Ov_GetParent(ov_containment, pStep));
	OV_INSTPTR_ov_domain containerDomain = NULL;
	OV_INSTPTR_fb_port pFbPort;

	OV_ELEMENT element;
	OV_ELEMENT varElement;

	*pTargetObj = NULL;
	if(nameToCheck == NULL){
			return OV_ERR_BADPARAM;
	}else if(nameToCheck[0] == '/'){
		//we have a full path
		*pTargetObj = ov_path_getobjectpointer(nameToCheck, 2);
	}else{
		//we have a relative path
		ov_memstack_lock();
		//all path are relative to the activeHeader
		containerDomain = Ov_PtrUpCast(ov_domain, activeHeader);
		ov_string_setvalue(&pathRelativeobject, ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object, containerDomain), 2));
		*pTargetObj = getrelativeobjectpointer(pathRelativeobject, nameToCheck);
		if(*pTargetObj == NULL){
			//perhaps the target is in the actions PART of the header
			containerDomain = Ov_GetPartPtr(actions, activeHeader);
			ov_string_setvalue(&pathRelativeobject, ov_path_getcanonicalpath(Ov_PtrUpCast(ov_object, containerDomain), 2));
			*pTargetObj = getrelativeobjectpointer(pathRelativeobject, nameToCheck);
		}
		ov_memstack_unlock();
	}
	ov_string_setvalue(&pathRelativeobject, NULL);

	if (Ov_CanCastTo(fb_functionchart, *pTargetObj)){
		//we have a variable in a functionchart
		pFbPort = Ov_SearchChildEx(ov_containment, Ov_StaticPtrCast(ov_domain, *pTargetObj), "sender", fb_port);
		if(pFbPort == NULL || !(Ov_CanCastTo(fb_stringport, pFbPort) || (Ov_CanCastTo(fb_anyport, pFbPort)) )){
			*pTargetObj = NULL;
			return OV_ERR_BADPARAM;
		}
		pFbPort = Ov_SearchChildEx(ov_containment, Ov_StaticPtrCast(ov_domain, *pTargetObj), "order", fb_port);
		if(pFbPort == NULL || !(Ov_CanCastTo(fb_stringport, pFbPort) || (Ov_CanCastTo(fb_anyport, pFbPort)) )){
			*pTargetObj = NULL;
			return OV_ERR_BADPARAM;
		}
		pFbPort = Ov_SearchChildEx(ov_containment, Ov_StaticPtrCast(ov_domain, *pTargetObj), "param", fb_port);
		if(pFbPort == NULL || !(Ov_CanCastTo(fb_stringport, pFbPort) || (Ov_CanCastTo(fb_anyport, pFbPort)) )){
			*pTargetObj = NULL;
			return OV_ERR_BADPARAM;
		}
		pFbPort = Ov_SearchChildEx(ov_containment, Ov_StaticPtrCast(ov_domain, *pTargetObj), "start", fb_port);
		if(pFbPort == NULL || !(Ov_CanCastTo(fb_boolport, pFbPort) || (Ov_CanCastTo(fb_anyport, pFbPort)) )){
			*pTargetObj = NULL;
			return OV_ERR_BADPARAM;
		}
	}else{
		//set variable in a object
		varElement.elemtype = OV_ET_NONE;
		element.elemtype = OV_ET_OBJECT;
		element.pobj = *pTargetObj;

		//search the variable
		ov_element_searchpart(&element, &varElement, OV_ET_VARIABLE, "sender");
		if(!( varElement.elemtype == OV_ET_VARIABLE && (varElement.elemunion.pvar->v_vartype == OV_VT_ANY || varElement.elemunion.pvar->v_vartype == OV_VT_STRING))) {
			//port not found or of wrong type
			*pTargetObj = NULL;
			return OV_ERR_BADPARAM;
		}
		ov_element_searchpart(&element, &varElement, OV_ET_VARIABLE, "order");
		if(!( varElement.elemtype == OV_ET_VARIABLE && (varElement.elemunion.pvar->v_vartype == OV_VT_ANY || varElement.elemunion.pvar->v_vartype == OV_VT_STRING))) {
			//port not found or of wrong type
			*pTargetObj = NULL;
			return OV_ERR_BADPARAM;
		}
		ov_element_searchpart(&element, &varElement, OV_ET_VARIABLE, "param");
		if(!( varElement.elemtype == OV_ET_VARIABLE && (varElement.elemunion.pvar->v_vartype == OV_VT_ANY || varElement.elemunion.pvar->v_vartype == OV_VT_STRING))) {
			//port not found or of wrong type
			*pTargetObj = NULL;
			return OV_ERR_BADPARAM;
		}
		ov_element_searchpart(&element, &varElement, OV_ET_VARIABLE, "start");
		if(!( varElement.elemtype == OV_ET_VARIABLE && (varElement.elemunion.pvar->v_vartype == OV_VT_ANY || varElement.elemunion.pvar->v_vartype == OV_VT_BOOL))) {
			//port not found or of wrong type
			*pTargetObj = NULL;
			return OV_ERR_BADPARAM;
		}
	}

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT ssc_sendCommand_targetObject_set(
		OV_INSTPTR_ssc_sendCommand          pinst,
		const OV_STRING  value
) {
	OV_RESULT result = OV_ERR_OK;

	result = ov_string_setvalue(&pinst->v_targetObject, value);
	if(Ov_OK(result)){
		//check this action. on loading of a FBD it is invalid till everything is loaded
		(void)ssc_sendCommand_checkAction(Ov_PtrUpCast(ssc_actionBlock, pinst));
	}
	return result;
}

OV_DLLFNCEXPORT void ssc_sendCommand_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_ssc_sendCommand pinst = Ov_StaticPtrCast(ssc_sendCommand, pfb);

	OV_INSTPTR_ssc_step  		pStep = Ov_DynamicPtrCast(ssc_step, Ov_GetParent(ov_containment, pinst));
	OV_INSTPTR_ssc_SequentialStateChart  	pOwnSSC = Ov_DynamicPtrCast(ssc_SequentialStateChart, Ov_GetParent(ov_containment, pStep));
	OV_INSTPTR_ov_object pTargetObj = NULL;
	OV_RESULT    			 result;
	OV_ANY ovvariable;
	ovvariable.value.vartype = OV_VT_STRING;
	ovvariable.value.valueunion.val_string = NULL;

	// check config
	if (ssc_execute_checkAction(Ov_PtrUpCast(ssc_actionBlock, pinst)) == FALSE){
		return;
	}

	ssc_getObjectFromSendCommand(pinst, pinst->v_targetObject, &pTargetObj);
	if(pTargetObj == NULL){
		return;
	}

	//setting sender
	ov_string_setvalue(&ovvariable.value.valueunion.val_string, pOwnSSC->v_identifier);
	result = ssc_setNamedVariable(pTargetObj, "sender", &ovvariable);
	if(Ov_Fail(result)){
		pinst->v_error=TRUE;
		ov_string_setvalue(&pinst->v_errorDetail, "Target found, but setting sender failed.");
		return;
	}
	//setting command
	ov_string_setvalue(&ovvariable.value.valueunion.val_string, pinst->v_command);
	result = ssc_setNamedVariable(pTargetObj, "order", &ovvariable);
	if(Ov_Fail(result)){
		pinst->v_error=TRUE;
		ov_string_setvalue(&pinst->v_errorDetail, "Target found, but setting command failed.");
		return;
	}
	//setting command
	ov_string_setvalue(&ovvariable.value.valueunion.val_string, pinst->v_parameter);
	result = ssc_setNamedVariable(pTargetObj, "param", &ovvariable);
	if(Ov_Fail(result)){
		pinst->v_error=TRUE;
		ov_string_setvalue(&pinst->v_errorDetail, "Target found, but setting command failed.");
		return;
	}
	//freeing string variable
	ov_string_setvalue(&ovvariable.value.valueunion.val_string, NULL);
	ovvariable.value.vartype = OV_VT_BOOL;
	ovvariable.value.valueunion.val_bool = TRUE;

	//activating object
	result = ssc_setNamedVariable(pTargetObj, "start", &ovvariable);
	if(Ov_Fail(result)){
		pinst->v_error=TRUE;
		ov_string_setvalue(&pinst->v_errorDetail, "Target found, but setting start failed.");
		return;
	}
	return;
}


OV_DLLFNCEXPORT OV_BOOL ssc_sendCommand_checkAction(
	OV_INSTPTR_ssc_actionBlock	pActionBlock
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_ov_object pTargetObj = NULL;
	OV_INSTPTR_ssc_sendCommand pinst = Ov_StaticPtrCast(ssc_sendCommand, pActionBlock);
	OV_INSTPTR_ssc_step pStep = Ov_DynamicPtrCast(ssc_step, Ov_GetParent(ov_containment, pinst));

	// check location
	if (pStep == NULL){
		pinst->v_error = TRUE;
		ov_string_setvalue(&pinst->v_errorDetail, "This ActionBlock is placed wrong.");
		return FALSE;
	}
	// check input
	if (ov_string_compare(pinst->v_targetObject, NULL) == OV_STRCMP_EQUAL){
		pinst->v_error=TRUE;
		ov_string_setvalue(&pinst->v_errorDetail, "Object to execute is not defined.");
		return FALSE;
	};

	ssc_getObjectFromSendCommand(pinst, pinst->v_targetObject, &pTargetObj);
	if(pTargetObj == NULL){
		pinst->v_error = TRUE;
		ov_string_setvalue(&pinst->v_errorDetail, "Configured Object not found.");
		return FALSE;
	}

	pinst->v_error = FALSE;
	ov_string_setvalue(&pinst->v_errorDetail, NULL);
	return TRUE;
}

