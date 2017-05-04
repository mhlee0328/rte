/******************************************************************************
*
*   FILE
*   ----
*   HMIHelperPVSL.c
*
*   History
*   -------
*   2017-03-08   File created
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
#include "libov/ov_macros.h"
#include "libov/ov_time.h"


OV_DLLFNCEXPORT void openaas_HMIHelperPVSL_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    /*    

    *   local variables
    */


	OV_INSTPTR_openaas_HMIHelperPVSL pinst = Ov_StaticPtrCast(openaas_HMIHelperPVSL, pfb);
	OV_INSTPTR_ov_object pobj = NULL;
	OV_INSTPTR_propertyValueStatement_PropertyValueStatementList pList = NULL;
	OV_INSTPTR_propertyValueStatement_PropertyValueStatement pchild = NULL;

	ov_string_setvalue(&pinst->v_ExpressionLogic, "");
	ov_string_setvalue(&pinst->v_ExpressionSemantic, "");
	ov_string_setvalue(&pinst->v_IDSpecification, "");
	ov_string_setvalue(&pinst->v_IDType, "");
	ov_string_setvalue(&pinst->v_TimeStamp, "");
	ov_string_setvalue(&pinst->v_Unit, "");
	ov_string_setvalue(&pinst->v_View, "");
	ov_string_setvalue(&pinst->v_Visibility, "");
	ov_string_setvalue(&pinst->v_Name, "");
	ov_string_setvalue(&pinst->v_Value, "");
	ov_string_setvalue(&pinst->v_ErrorText, "");
	pinst->v_Error = FALSE;


	OV_UINT len = 0;
	OV_UINT len2 = 0;
	OV_STRING tmpString_header = NULL ;
	OV_STRING *pathList = NULL;
	OV_STRING *pathList2 = NULL;
	OV_STRING path = NULL;
	pathList = ov_string_split(pinst->v_Path, "/", &len);

	ov_string_setvalue(&tmpString_header, ".Header.");

	if (len == 8){
		pathList2 = ov_string_split(pathList[7], ".",&len2);
		for (OV_UINT i = 4; i < len; i++){
			if (i == 4)
				ov_string_setvalue(&path, "/");
			else
				ov_string_append(&path, "/");
			ov_string_append(&path, pathList[i]);
			if ( (i==6) && (len2>1) ){
				ov_string_append(&path, "/");
				ov_string_append(&path, pathList2[0]);
				ov_string_append(&path, tmpString_header);
				ov_string_append(&path, pathList2[1]);
				break;
			}
		}
	}else if (len == 9){
		for (OV_UINT i = 4; i < len; i++){
			if (i == 4)
				ov_string_setvalue(&path, "/");
			else
				ov_string_append(&path, "/");
			ov_string_append(&path, pathList[i]);
			if(i==7)
				ov_string_append(&path, ".Body");
		}
	}

	pobj = ov_path_getobjectpointer(path,2);
	if (!pobj){
		pinst->v_Error = TRUE;
		ov_string_setvalue(&pinst->v_ErrorText, "Could not find an object for this path");
		ov_string_freelist(pathList);
		ov_string_freelist(pathList2);
		ov_database_free(path);
		ov_database_free(tmpString_header);
		return;
	}
	pList = Ov_DynamicPtrCast(propertyValueStatement_PropertyValueStatementList, pobj);
	if (!pList){
		pinst->v_Error = TRUE;
		ov_string_setvalue(&pinst->v_ErrorText, "Object is not of PVSL-Type");
		ov_string_freelist(pathList);
		ov_string_freelist(pathList2);
		ov_database_free(path);
		ov_database_free(tmpString_header);
		return;
	}

	OV_STRING tmpString = NULL;
	OV_UINT i = 0;
	Ov_ForEachChildEx(ov_containment, Ov_DynamicPtrCast(ov_domain,pList), pchild, propertyValueStatement_PropertyValueStatement){
		if (i != 0){
			ov_string_append(&pinst->v_ExpressionLogic, ";");
			ov_string_append(&pinst->v_ExpressionSemantic, ";");
			ov_string_append(&pinst->v_IDSpecification, ";");
			ov_string_append(&pinst->v_IDType, ";");
			ov_string_append(&pinst->v_TimeStamp, ";");
			ov_string_append(&pinst->v_Unit, ";");
			ov_string_append(&pinst->v_View, ";");
			ov_string_append(&pinst->v_Visibility, ";");
			ov_string_append(&pinst->v_Name, ";");
			ov_string_append(&pinst->v_Value, ";");
		}

		if (i == 0)
			ov_string_setvalue(&pinst->v_IDSpecification, pchild->v_IDIdString);
		else
			ov_string_append(&pinst->v_IDSpecification, pchild->v_IDIdString);

		ov_string_print(&tmpString, "%i", pchild->v_IDIdType);
		if (i == 0)
			ov_string_setvalue(&pinst->v_IDType, tmpString);
		else
			ov_string_append(&pinst->v_IDType, tmpString);

		if (i == 0)
			ov_string_setvalue(&pinst->v_TimeStamp, ov_time_timetoascii_local(&pchild->v_TimeStamp));
		else
			ov_string_append(&pinst->v_TimeStamp, ov_time_timetoascii_local(&pchild->v_TimeStamp));

		if (i == 0)
			ov_string_setvalue(&pinst->v_Unit, pchild->v_Unit);
		else
			ov_string_append(&pinst->v_Unit, pchild->v_Unit);

		if (i == 0)
			ov_string_setvalue(&pinst->v_Name, pchild->v_identifier);
		else
			ov_string_append(&pinst->v_Name, pchild->v_identifier);

		switch(pchild->v_ExpressionSemantic){
			case 0:
				if (i == 0)
					ov_string_setvalue(&pinst->v_ExpressionSemantic, "A");
				else
					ov_string_append(&pinst->v_ExpressionSemantic, "A");
				break;
			case 1:
				if (i == 0)
					ov_string_setvalue(&pinst->v_ExpressionSemantic, "S");
				else
					ov_string_append(&pinst->v_ExpressionSemantic, "S");
				break;
			case 2:
				if (i == 0)
					ov_string_setvalue(&pinst->v_ExpressionSemantic, "M");
				else
					ov_string_append(&pinst->v_ExpressionSemantic, "M");
				break;
			case 3:
				if (i == 0)
					ov_string_setvalue(&pinst->v_ExpressionSemantic, "R");
				else
					ov_string_append(&pinst->v_ExpressionSemantic, "R");
				break;
			default:
				if (pinst->v_Error == FALSE){
					pinst->v_Error = TRUE;
					ov_string_setvalue(&pinst->v_ErrorText, "ExpressionSemantic not supported");
				}
			break;
			}

		switch(pchild->v_ExpressionLogic){
			case 0:
				if (i == 0)
					ov_string_setvalue(&pinst->v_ExpressionLogic, "GT");
				else
					ov_string_append(&pinst->v_ExpressionLogic, "GT");
				break;
			case 1:
				if (i == 0)
					ov_string_setvalue(&pinst->v_ExpressionLogic, "GE");
				else
					ov_string_append(&pinst->v_ExpressionLogic, "GE");
				break;
			case 2:
				if (i == 0)
					ov_string_setvalue(&pinst->v_ExpressionLogic, "EQ");
				else
					ov_string_append(&pinst->v_ExpressionLogic, "EQ");
				break;
			case 3:
				if (i == 0)
					ov_string_setvalue(&pinst->v_ExpressionLogic, "NE");
				else
					ov_string_append(&pinst->v_ExpressionLogic, "NE");
				break;
			case 4:
				if (i == 0)
					ov_string_setvalue(&pinst->v_ExpressionLogic, "LE");
				else
					ov_string_append(&pinst->v_ExpressionLogic, "LE");
				break;
			case 5:
				if (i == 0)
					ov_string_setvalue(&pinst->v_ExpressionLogic, "LT");
				else
					ov_string_append(&pinst->v_ExpressionLogic, "LT");
				break;
			default:
				if (pinst->v_Error == FALSE){
					pinst->v_Error = TRUE;
					ov_string_setvalue(&pinst->v_ErrorText, "ExpressionLogic not supported");
				}
			break;
			}

		switch(pchild->v_View){
			case 0:
				if (i == 0)
					ov_string_setvalue(&pinst->v_View, "B");
				else
					ov_string_append(&pinst->v_View, "B");
				break;
			case 1:
				if (i == 0)
					ov_string_setvalue(&pinst->v_View, "C");
				else
					ov_string_append(&pinst->v_View, "C");
				break;
			case 2:
				if (i == 0)
					ov_string_setvalue(&pinst->v_View, "P");
				else
					ov_string_append(&pinst->v_View, "P");
				break;
			case 3:
				if (i == 0)
					ov_string_setvalue(&pinst->v_View, "F");
				else
					ov_string_append(&pinst->v_View, "F");
				break;
			case 4:
				if (i == 0)
					ov_string_setvalue(&pinst->v_View, "L");
				else
					ov_string_append(&pinst->v_View, "L");
				break;
			case 5:
				if (i == 0)
					ov_string_setvalue(&pinst->v_View, "S");
				else
					ov_string_append(&pinst->v_View, "S");
				break;
			case 6:
				if (i == 0)
					ov_string_setvalue(&pinst->v_View, "N");
				else
					ov_string_append(&pinst->v_View, "N");
				break;
			case 7:
				if (i == 0)
					ov_string_setvalue(&pinst->v_View, "L");
				else
					ov_string_append(&pinst->v_View, "L");
				break;
			case 8:
				if (i == 0)
					ov_string_setvalue(&pinst->v_View, "H");
				else
					ov_string_append(&pinst->v_View, "H");
				break;
			default:
				if (pinst->v_Error == FALSE){
					pinst->v_Error = TRUE;
					ov_string_setvalue(&pinst->v_ErrorText, "View not supported");
				}
			break;
			}

		switch(pchild->v_Visibility){
			case 0:
				if (i == 0)
					ov_string_setvalue(&pinst->v_Visibility, "-");
				else
					ov_string_append(&pinst->v_Visibility, "-");
				break;
			case 1:
				if (i == 0)
					ov_string_setvalue(&pinst->v_Visibility, "o");
				else
					ov_string_append(&pinst->v_Visibility, "o");
				break;
			case 2:
				if (i == 0)
					ov_string_setvalue(&pinst->v_Visibility, "+");
				else
					ov_string_append(&pinst->v_Visibility, "+");
				break;
			default:
				if (pinst->v_Error == FALSE){
					pinst->v_Error = TRUE;
					ov_string_setvalue(&pinst->v_ErrorText, "Visibility not supported");
				}
			break;
			}

		switch(pchild->v_Value.value.vartype & OV_VT_KSMASK){
			case OV_VT_BOOL:
				if (pchild->v_Value.value.valueunion.val_bool == TRUE)
					if (i == 0)
						ov_string_setvalue(&pinst->v_Value, "TRUE");
					else
						ov_string_append(&pinst->v_Value, "TRUE");
				else
					if (i == 0)
						ov_string_setvalue(&pinst->v_Value, "FALSE");
					else
						ov_string_append(&pinst->v_Value, "FALSE");
			break;
			case OV_VT_STRING:
				if (i == 0)
					ov_string_setvalue(&pinst->v_Value, pchild->v_Value.value.valueunion.val_string);
				else
					ov_string_append(&pinst->v_Value, pchild->v_Value.value.valueunion.val_string);
			break;
			case OV_VT_DOUBLE:
			    ov_string_print(&tmpString, "%lf", pchild->v_Value.value.valueunion.val_double);
			    if (i == 0)
					ov_string_setvalue(&pinst->v_Value, tmpString);
				else
					ov_string_append(&pinst->v_Value, tmpString);
			break;
			case OV_VT_INT:
				ov_string_print(&tmpString, "%i", pchild->v_Value.value.valueunion.val_int);
				if (i == 0)
					ov_string_setvalue(&pinst->v_Value, tmpString);
				else
					ov_string_append(&pinst->v_Value, tmpString);
			break;
			case OV_VT_UINT:
				ov_string_print(&tmpString, "%u", pchild->v_Value.value.valueunion.val_uint);
				if (i == 0)
					ov_string_setvalue(&pinst->v_Value, tmpString);
				else
					ov_string_append(&pinst->v_Value, tmpString);
			break;
			case OV_VT_SINGLE:
				ov_string_print(&tmpString, "%f", pchild->v_Value.value.valueunion.val_single);
				if (i == 0)
					ov_string_setvalue(&pinst->v_Value, tmpString);
				else
					ov_string_append(&pinst->v_Value, tmpString);
			break;
			default:
				if (pinst->v_Error == FALSE){
					pinst->v_Error = TRUE;
					ov_string_setvalue(&pinst->v_ErrorText, "DataTye not supported");
				}
			break;
		}

	i++;
	}

	ov_database_free(tmpString);
	ov_string_freelist(pathList);
	ov_string_freelist(pathList2);
	ov_database_free(path);
	ov_database_free(tmpString_header);
    return;
}
