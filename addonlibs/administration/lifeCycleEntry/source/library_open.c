
/******************************************************************************
*
*   FILE
*   ----
*   library_open.c
*
*   History
*   -------
*   2014-10-13   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_lifeCycleEntry
#define OV_COMPILE_LIBRARY_lifeCycleEntry
#endif
#include "lifeCycleEntry.h"
#ifdef ov_library_open_lifeCycleEntry
#undef ov_library_open_lifeCycleEntry
#endif

#include "libov/ov_macros.h"
#include "libov/ov_result.h"
#include "libov/ov_logfile.h"
#include "libov/ov_memstack.h"
#include "libov/ov_result.h"
#include "libov/ov_time.h"

 OV_RESULT ov_library_setglobalvars_lifeCycleEntry_new(void) {
	OV_RESULT result = OV_ERR_OK;
	/*
	 *    set the global variables of the original version
	 *    and if successful, load other libraries
	 *    and create some objects
	 */
	 result = ov_library_setglobalvars_lifeCycleEntry();
	 if (Ov_Fail(result)){
		 return result;
	 }
	/*
	 * Your code goes here.
	 */

	 OV_INSTPTR_ov_library pLibIDENTIFICATION = NULL;
	 Ov_ForEachChildEx(ov_instantiation, pclass_ov_library, pLibIDENTIFICATION, ov_library){
		if(ov_string_compare(pLibIDENTIFICATION->v_identifier, "identification") == OV_STRCMP_EQUAL){
			break;
		}
	 }
	 if(!pLibIDENTIFICATION){
		result = Ov_CreateObject(ov_library, pLibIDENTIFICATION, &(pdb->acplt), "identification");
		if(Ov_Fail(result)){
			ov_memstack_lock();
			ov_logfile_error("lifeCycleEntry: Fatal: Couldn't load dependency Library identification Reason: %s", ov_result_getresulttext(result));
			ov_memstack_unlock();
			return result;
		}
	 }


	return result;
	}

 OV_DLLFNCEXPORT OV_LIBRARY_DEF *ov_library_open_lifeCycleEntry(void) {
 	/* local variables */
 	static OV_LIBRARY_DEF *OV_LIBRARY_DEF_lifeCycleEntry_new;
 	/*
 	*       replace the 'setglobalvars' function created by the code generator
 	*       with a new one.
 	*/
 	OV_LIBRARY_DEF_lifeCycleEntry_new = ov_library_open_lifeCycleEntry_old();
 	OV_LIBRARY_DEF_lifeCycleEntry_new->setglobalvarsfnc = ov_library_setglobalvars_lifeCycleEntry_new;
 	return OV_LIBRARY_DEF_lifeCycleEntry_new;
  }


