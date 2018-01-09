
/******************************************************************************
*
*   FILE
*   ----
*   simpleMessageCreator.c
*
*   History
*   -------
*   2017-10-15   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_openaas
#define OV_COMPILE_LIBRARY_openaas
#endif


#include "openaas.h"
#include "MessageSys.h"
#include "libov/ov_macros.h"
#include "libov/ov_path.h"
#include "libov/ov_result.h"
OV_DLLFNCEXPORT void openaas_simpleMessageCreator_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_openaas_simpleMessageCreator pinst = Ov_StaticPtrCast(openaas_simpleMessageCreator, pfb);
    if(!pinst->v_message  || !pinst->v_targetPath){
    	return;
    }


    OV_INSTPTR_ov_domain ptargetPathObj =  Ov_StaticPtrCast(ov_domain,ov_path_getobjectpointer(pinst->v_targetPath,2));

    if(!ptargetPathObj){
    	return;
    }


    OV_INSTPTR_MessageSys_Message pobj = NULL;
    OV_RESULT result = Ov_CreateObject(
    		MessageSys_Message,
    		pobj, ptargetPathObj, "message");

    if(Ov_Fail(result)) {
    		return;
    }
    OV_INSTPTR_MessageSys_Message msg = Ov_StaticPtrCast(MessageSys_Message,pobj);
    ov_string_setvalue(&msg->v_msgBody,"<bdy>");
    ov_string_append(&msg->v_msgBody,pinst->v_message);
    ov_string_append(&msg->v_msgBody,"</bdy>");


    ov_string_setvalue(&msg->v_senderAddress,"127.0.0.1");
    ov_string_setvalue(&msg->v_receiverAddress,"127.0.0.1");
    ov_string_setvalue(&msg->v_senderName,"MANAGER");
    ov_string_setvalue(&msg->v_senderName,"");
    ov_string_setvalue(&msg->v_senderComponent,"me");
    ov_string_setvalue(&msg->v_receiverComponent,"");
    msg->v_msgStatus = 0;

    ov_string_setvalue(&msg->v_msgID,"43885401");
    ov_string_setvalue(&msg->v_msgID,"");
    ov_string_setvalue(&msg->v_auth,"");

    msg->v_expectAnswer = false;





    ov_string_setvalue(&pinst->v_message,"");

    return;
}
