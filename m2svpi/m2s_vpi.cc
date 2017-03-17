#define VPI_1995 0 /* set to non-zero for Verilog-1995 compatibility */

#include <cstdlib>    /* ANSI C standard library */
#include "vpi_user.h"  /* IEEE 1364 PLI VPI routine library  */
#include <m2s.h>
#include <map>
#include <list>

struct a_access
{
	//unsigned access_module;
    //unsigned access_id;
    unsigned access_type;
    unsigned access_address;	
    unsigned access_data;
};

static std::map<unsigned, unsigned> memory_file; // address, data
static std::map<unsigned, std::map<unsigned, a_access>> inflight_accesses; // <module,<id,access>>

void SeeMemory()
{
		std::map<unsigned,unsigned>::iterator it2;
		for (it2 = memory_file.begin();it2!=memory_file.end();it2++)
			vpi_printf("\t\t Adr: %d \t\t Data: %d\n",it2->first,it2->second);
}

#if VPI_1995
#include "../vpi_1995_compat.h"  /* kludge new Verilog-2001 routines */
#endif

//==============================================================================
//==============================================================================
/* prototypes of the RESET PLI application routines */
PLI_INT32   m2s_reset_compiletf(PLI_BYTE8 *user_data),
	    m2s_reset_calltf(PLI_BYTE8 *user_data),
	    m2s_reset_sizetf(PLI_BYTE8 *user_data);


void m2s_reset_register()
{
    s_vpi_systf_data tf_data;

    tf_data.type        = vpiSysTask;
    tf_data.sysfunctype = 0;
    tf_data.tfname      = "$m2s_reset";
    tf_data.calltf      = m2s_reset_calltf;
    tf_data.compiletf   = m2s_reset_compiletf;
    tf_data.sizetf      = m2s_reset_sizetf;
    tf_data.user_data   = NULL;
    vpi_register_systf(&tf_data);
    return;
}

/**********************************************************************
 * compiletf routine
 *********************************************************************/
PLI_INT32 m2s_reset_calltf(PLI_BYTE8 *user_data)
{
    vpi_printf("VPI::In m2s_reset_calltf call\n");
    Multi2Sim::getInstance().m2sReset();
    return(0);
}

/**********************************************************************
 * calltf routine
 *********************************************************************/
PLI_INT32 m2s_reset_compiletf(PLI_BYTE8 *user_data)
{
    vpi_printf("VPI::In m2s_reset_compiletf call\n");
    return(0);
}

/**********************************************************************
 * sizetf routine
 *********************************************************************/
PLI_INT32 m2s_reset_sizetf(PLI_BYTE8 *user_data)
{
    vpi_printf("VPI::In m2s_reset_compiletf call\n");
    return(0);
}

//==============================================================================
//==============================================================================
/* prototypes of the INITIALIZE PLI application routines */
PLI_INT32   m2s_initialize_compiletf(PLI_BYTE8 *user_data),
m2s_initialize_calltf(PLI_BYTE8 *user_data),
m2s_initialize_sizetf(PLI_BYTE8 *user_data);

void m2s_initialize_register()
{
    s_vpi_systf_data tf_data;

    tf_data.type        = vpiSysTask;
    tf_data.sysfunctype = 0;
    tf_data.tfname      = "$m2s_initialize";
    tf_data.calltf      = m2s_initialize_calltf;
    tf_data.compiletf   = m2s_initialize_compiletf;
    tf_data.sizetf      = m2s_initialize_sizetf;
    tf_data.user_data   = NULL;
    vpi_register_systf(&tf_data);
    return;
}

/**********************************************************************
 * compiletf routine
 *********************************************************************/
PLI_INT32 m2s_initialize_calltf(PLI_BYTE8 *user_data)
{
    vpi_printf("VPI::In m2s_initialize_calltf call\n");
    Multi2Sim::getInstance().m2sInitialize("m2s --mem-debug debug-info.txt  --trace trace-info.gz --mem-report report-info.txt --mem-sim --mem-config mem-config");
    return(0);
}

/**********************************************************************
 * calltf routine
 *********************************************************************/
PLI_INT32 m2s_initialize_compiletf(PLI_BYTE8 *user_data)
{
    vpi_printf("VPI::In m2s_initialize_compiletf call\n");
    return(0);
}

/**********************************************************************
 * sizetf routine
 *********************************************************************/
PLI_INT32 m2s_initialize_sizetf(PLI_BYTE8 *user_data)
{
    vpi_printf("VPI::In m2s_initialize_compiletf call\n");
    return(0);
}

//==============================================================================
//==============================================================================
/* prototypes of the FINALIZE PLI application routines */
PLI_INT32   m2s_finalize_compiletf(PLI_BYTE8 *user_data),
m2s_finalize_calltf(PLI_BYTE8 *user_data),
m2s_finalize_sizetf(PLI_BYTE8 *user_data);

void m2s_finalize_register()
{
    s_vpi_systf_data tf_data;

    tf_data.type        = vpiSysTask;
    tf_data.sysfunctype = 0;
    tf_data.tfname      = "$m2s_finalize";
    tf_data.calltf      = m2s_finalize_calltf;
    tf_data.compiletf   = m2s_finalize_compiletf;
    tf_data.sizetf      = m2s_finalize_sizetf;
    tf_data.user_data   = NULL;
    vpi_register_systf(&tf_data);
    return;
}

/**********************************************************************
 * compiletf routine
 *********************************************************************/
PLI_INT32 m2s_finalize_calltf(PLI_BYTE8 *user_data)
{
    vpi_printf("VPI::In m2s_finalize_calltf call\n");
    Multi2Sim::getInstance().m2sFinalize();
    return(0);
}

/**********************************************************************
 * calltf routine
 *********************************************************************/
PLI_INT32 m2s_finalize_compiletf(PLI_BYTE8 *user_data)
{
    vpi_printf("VPI::In m2s_finalize_compiletf call\n");
    return(0);
}

/**********************************************************************
 * sizetf routine
 *********************************************************************/
PLI_INT32 m2s_finalize_sizetf(PLI_BYTE8 *user_data)
{
    vpi_printf("VPI::In m2s_finalize_compiletf call\n");
    return(0);
}

//==============================================================================
//==============================================================================
/* prototypes of the STEP PLI application routines */
PLI_INT32   m2s_step_compiletf(PLI_BYTE8 *user_data),
	    m2s_step_calltf(PLI_BYTE8 *user_data),
	    m2s_step_sizetf(PLI_BYTE8 *user_data);
void m2s_step_register()
{
    s_vpi_systf_data tf_data;

    tf_data.type        = vpiSysFunc;
    tf_data.sysfunctype = vpiSysFuncSized;
    tf_data.tfname      = "$m2s_step";
    tf_data.calltf      = m2s_step_calltf;
    tf_data.compiletf   = m2s_step_compiletf;
    tf_data.sizetf      = m2s_step_sizetf;
    tf_data.user_data   = NULL;
    vpi_register_systf(&tf_data);
    return;
}
/**********************************************************************
 * sizetf routine
 *********************************************************************/
PLI_INT32 m2s_step_sizetf(PLI_BYTE8 *user_data)
{
    vpi_printf("VPI::In m2s_step_compiletf call\n");
    return(0);
}
/**********************************************************************
 * compiletf routine
 *********************************************************************/
PLI_INT32 m2s_step_calltf(PLI_BYTE8 *user_data)
{
    vpi_printf("VPI::In m2s_step_calltf call\n");
    Multi2Sim::getInstance().m2sStep();
    return(0);
}

/**********************************************************************
 * calltf routine
 *********************************************************************/
PLI_INT32 m2s_step_compiletf(PLI_BYTE8 *user_data)
{
    vpi_printf("VPI::In m2s_step_compiletf call\n");
    return(0);
}

//==============================================================================
//==============================================================================
/* prototypes of the access PLI application routines */
PLI_INT32   m2s_access_compiletf(PLI_BYTE8 *user_data),
m2s_access_calltf(PLI_BYTE8 *user_data),
m2s_access_sizetf(PLI_BYTE8 *user_data);
void m2s_access_register()
{
    s_vpi_systf_data tf_data;

    tf_data.type        = vpiSysTask;
    tf_data.sysfunctype = 0;
    tf_data.tfname      = "$m2s_access";
    tf_data.calltf      = m2s_access_calltf;
    tf_data.compiletf   = m2s_access_compiletf;
    tf_data.sizetf      = m2s_access_sizetf;
    tf_data.user_data   = NULL;
    vpi_register_systf(&tf_data);
    return;
}
/**********************************************************************
 * sizetf routine
 *********************************************************************/
PLI_INT32 m2s_access_sizetf(PLI_BYTE8 *user_data)
{
    vpi_printf("VPI::In m2s_access_compiletf call\n");
    return(0);
}
/**********************************************************************
 * compiletf routine
 *********************************************************************/
PLI_INT32 m2s_access_calltf(PLI_BYTE8 *user_data)
{
    vpi_printf("VPI::In m2s_access_calltf call\n");
    s_vpi_value value_s;
    vpiHandle   systf_handle, arg_itr, arg_handle;
	PLI_INT32   mod, type, address, data, identifier;

    systf_handle = vpi_handle(vpiSysTfCall, NULL);
    arg_itr = vpi_iterate(vpiArgument, systf_handle);
    if (arg_itr == NULL) {
      vpi_printf("ERROR: $access failed to obtain systf arg handles\n");
      return(0);
    }

    arg_handle = vpi_scan(arg_itr);
    value_s.format = vpiIntVal;
    vpi_get_value(arg_handle, &value_s);
    mod = value_s.value.integer;

    arg_handle = vpi_scan(arg_itr);
    value_s.format = vpiIntVal;
    vpi_get_value(arg_handle, &value_s);
    type = value_s.value.integer;

    arg_handle = vpi_scan(arg_itr);
    value_s.format = vpiIntVal;
    vpi_get_value(arg_handle, &value_s);
    address = value_s.value.integer;

    arg_handle = vpi_scan(arg_itr);
    value_s.format = vpiIntVal;
    vpi_get_value(arg_handle, &value_s);
    data = value_s.value.integer;

    arg_handle = vpi_scan(arg_itr);
    value_s.format = vpiIntVal;
    vpi_get_value(arg_handle, &value_s);
	identifier = value_s.value.integer;

			// TODO: REMOVE
			vpi_printf("\tModule: %d\n",mod);
			vpi_printf("\tIdentifier: %d\n",identifier);
			vpi_printf("\tType: %d\n",type);
			vpi_printf("\tAddress: %d\n",address);
			vpi_printf("\tData: %d\n\n",data);

	// Save in the inflight accesses for the VPI	
	a_access the_access = {(unsigned)type,(unsigned)address,(unsigned)data};
	inflight_accesses[mod].insert(std::make_pair(identifier, the_access));

    Multi2Sim::getInstance().m2sAccess((unsigned int) mod
				      ,(unsigned int) type
				      ,(unsigned int) address
					  ,(unsigned int) identifier
				      );
    return(0);
}

/**********************************************************************
 * calltf routine
 *********************************************************************/
PLI_INT32 m2s_access_compiletf(PLI_BYTE8 *user_data)
{
    vpi_printf("VPI::In m2s_access_compiletf call\n");

    vpiHandle systf_handle, arg_itr, arg_handle;
    PLI_INT32 tfarg_type;
    int       err_flag = 0;

    do { /* group all tests, so can break out of group on error */
      systf_handle = vpi_handle(vpiSysTfCall, NULL);
      arg_itr = vpi_iterate(vpiArgument, systf_handle);
      if (systf_handle == NULL)
	  vpi_printf("something is wrong\n");
      if (arg_itr == NULL) {
	vpi_printf("ERROR: $access() requires 5 arguments; has none\n");
	err_flag = 1;
	break;
      }
      arg_handle = vpi_scan(arg_itr);
      tfarg_type = vpi_get(vpiType, arg_handle);
      if ( (tfarg_type != vpiReg) &&
	   (tfarg_type != vpiIntegerVar) &&
	   (tfarg_type != vpiConstant) &&
       (tfarg_type != vpiParameter)  ) {
	vpi_printf("ERROR: $access() arg1 must be number, variable, net or parameter\n");
	err_flag = 1;
	break;
      }
      arg_handle = vpi_scan(arg_itr);
      if (arg_handle == NULL) {
	vpi_printf("ERROR: $access() requires 2nd argument\n");
	err_flag = 1;
	break;
      }
      tfarg_type = vpi_get(vpiType, arg_handle);
      if ( (tfarg_type != vpiReg) &&
	   (tfarg_type != vpiIntegerVar) &&
	   (tfarg_type != vpiConstant)   ) {
	vpi_printf("ERROR: $access() arg2 must be number, variable or net\n");
	err_flag = 1;
	break;
      }
      arg_handle = vpi_scan(arg_itr);
      if (arg_handle == NULL) {
	vpi_printf("ERROR: $access() requires 3nd argument\n");
	err_flag = 1;
	break;
      }
      tfarg_type = vpi_get(vpiType, arg_handle);
      if ( (tfarg_type != vpiReg) &&
	   (tfarg_type != vpiIntegerVar) &&
	   (tfarg_type != vpiConstant)   ) {
	vpi_printf("ERROR: $access() arg3 must be number, variable or net\n");
	err_flag = 1;
	break;
      }
	  arg_handle = vpi_scan(arg_itr);
      if (arg_handle == NULL) {
	vpi_printf("ERROR: $access() requires 4nd argument\n");
	err_flag = 1;
	break;
      }
      tfarg_type = vpi_get(vpiType, arg_handle);
      if ( (tfarg_type != vpiReg) &&
	   (tfarg_type != vpiIntegerVar) &&
	   (tfarg_type != vpiConstant)   ) {
	vpi_printf("ERROR: $access() arg4 must be number, variable or net\n");
	err_flag = 1;
	break;
      }
	  arg_handle = vpi_scan(arg_itr);
      if (arg_handle == NULL) {
	vpi_printf("ERROR: $access() requires 5nd argument\n");
	err_flag = 1;
	break;
      }
      tfarg_type = vpi_get(vpiType, arg_handle);
      if ( (tfarg_type != vpiReg) &&
	   (tfarg_type != vpiIntegerVar) &&
	   (tfarg_type != vpiConstant)   ) {
	vpi_printf("ERROR: $access() arg5 must be number, variable or net\n");
	err_flag = 1;
	break;
      }
      if (vpi_scan(arg_itr) != NULL) {
	vpi_printf("ERROR: $access() requires 5 arguments; has too many\n");
	vpi_free_object(arg_itr);
	err_flag = 1;
	break;
      }
    } while (0 == 1); /* end of test group; only executed once */

    if (err_flag) {
      vpi_control(vpiFinish, 1);  /* abort simulation */
    }

    return(0);
}

//==============================================================================
//==============================================================================
/* prototypes of the getProcessed PLI application routines */
PLI_INT32   m2s_getProcessed_compiletf(PLI_BYTE8 *user_data),
	    m2s_getProcessed_calltf(PLI_BYTE8 *user_data),
	    m2s_getProcessed_sizetf(PLI_BYTE8 *user_data);
void m2s_getProcessed_register()
{
    s_vpi_systf_data tf_data;

    tf_data.type        = vpiSysFunc;
    tf_data.sysfunctype = vpiSysFuncSized;
    tf_data.tfname      = "$m2s_getProcessed";
    tf_data.calltf      = m2s_getProcessed_calltf;
    tf_data.compiletf   = m2s_getProcessed_compiletf;
    tf_data.sizetf      = m2s_getProcessed_sizetf;
    tf_data.user_data   = NULL;
    vpi_register_systf(&tf_data);
    return;
}

/**********************************************************************
 * sizetf routine
 *********************************************************************/
PLI_INT32 m2s_getProcessed_sizetf(PLI_BYTE8 *user_data)
{
    vpi_printf("VPI::In m2s_getProcessed_compiletf call\n");
    return(0);
}
/**********************************************************************
 * calltf routine
 *********************************************************************/
PLI_INT32 m2s_getProcessed_calltf(PLI_BYTE8 *user_data)
{
    vpi_printf("VPI::In m2s_getProcessed_calltf call\n");

    s_vpi_value value_s;
    vpiHandle   systf_handle, arg_itr, arg_handle;
	PLI_INT32   mod, identifier, data;

    systf_handle = vpi_handle(vpiSysTfCall, NULL);
    arg_itr = vpi_iterate(vpiArgument, systf_handle);
    if (arg_itr == NULL) {
      vpi_printf("ERROR: $m2s_getProcessed failed to obtain systf arg handles\n");
      return(0);
    }

    arg_handle = vpi_scan(arg_itr);
    value_s.format = vpiIntVal;
    vpi_get_value(arg_handle, &value_s);
    mod = value_s.value.integer;

	// Get the value from multi2sim
    identifier = (PLI_INT32) Multi2Sim::getInstance().m2sGetProcessedAccess((unsigned int)mod);

			// TODO: REMOVE
		    vpi_printf("VPI::(CONT) In m2s_getProcessed_calltf call\n"); 
			vpi_printf("\tModule: %d\n",mod);

	arg_handle = vpi_scan(arg_itr);
    value_s.format = vpiIntVal;
	value_s.value.integer = identifier; // Get the value for the returning IDENTIFIER
	vpi_put_value(arg_handle, &value_s, NULL, vpiNoDelay); // performs the actual writting

			// TODO: REMOVE
			vpi_printf("\tIdentifier: %d\n",identifier);

	// Now check if this idenfier exists for the module
	// If it does, checks if it is a read or write
	// If it is a read, returns the value to verilog
	// If it is a write, save the value on the table
	// Delete this element, as it is no longer necessary
	std::map<unsigned,a_access>::iterator it;		
	it = inflight_accesses[mod].find(value_s.value.integer);
	
	if (it != inflight_accesses[mod].end())
	{
		// Identifier exists!
		
			// TODO: REMOVE
			vpi_printf("\tType: %d\n",it->second.access_type);
			vpi_printf("\tAddress: %d\n",it->second.access_address);

			
		
		if (it->second.access_type == 1)  // read
		{
			vpi_printf("\tRead Data: %d\n\n",memory_file[it->second.access_address]);
			// get the value from the table and return it to verilog			
			arg_handle = vpi_scan(arg_itr);
			value_s.format = vpiIntVal;
			value_s.value.integer = (PLI_INT32) memory_file[it->second.access_address]; // Get the value for the returning DATA
			vpi_put_value(arg_handle, &value_s, NULL, vpiNoDelay); // performs the actual writting 
			
		}
		else if (it->second.access_type == 2) // write
		{
			memory_file[it->second.access_address] = it->second.access_data;
			vpi_printf("\tWrote Data: %d\n\n",memory_file[it->second.access_address]);
		}

		// Deleting the access element
		inflight_accesses[mod].erase (it);
	}
	else
	{
		vpi_printf("Access id %d was no found on current map, check everything is ok\n",value_s.value.integer);
	}

	return 0;	
}

/**********************************************************************
 * compiletf routine
 *********************************************************************/
PLI_INT32 m2s_getProcessed_compiletf(PLI_BYTE8 *user_data)
{
    vpi_printf("VPI::In m2s_getProcessed_compiletf call\n");

    vpiHandle systf_handle, arg_itr, arg_handle;
    PLI_INT32 tfarg_type;
    int       err_flag = 0;

    do { /* group all tests, so can break out of group on error */
      systf_handle = vpi_handle(vpiSysTfCall, NULL);
      arg_itr = vpi_iterate(vpiArgument, systf_handle);
      if (systf_handle == NULL)
	  vpi_printf("something is wrong\n");
      if (arg_itr == NULL) {
	vpi_printf("ERROR: $getProcessed() requires 3 argument; has none\n");
	err_flag = 1;
	break;
      }
      arg_handle = vpi_scan(arg_itr);
      tfarg_type = vpi_get(vpiType, arg_handle);
      if ( (tfarg_type != vpiReg) &&
	   (tfarg_type != vpiIntegerVar) &&
	   (tfarg_type != vpiConstant) &&
       (tfarg_type != vpiParameter)  ) {
	vpi_printf("ERROR: $getProcessed() arg1 must be number, variable, net or parameter\n");
	err_flag = 1;
	break;
      }
      arg_handle = vpi_scan(arg_itr);
      tfarg_type = vpi_get(vpiType, arg_handle);
      if ( (tfarg_type != vpiReg) &&
	   (tfarg_type != vpiIntegerVar) &&
	   (tfarg_type != vpiConstant)  ) {
	vpi_printf("ERROR: $getProcessed() arg2 must be number, variable, net\n");
	err_flag = 1;
	break;
      }
      arg_handle = vpi_scan(arg_itr);
      tfarg_type = vpi_get(vpiType, arg_handle);
      if ( (tfarg_type != vpiReg) &&
	   (tfarg_type != vpiIntegerVar) &&
	   (tfarg_type != vpiConstant)  ) {
	vpi_printf("ERROR: $getProcessed() arg3 must be number, variable, net\n");
	err_flag = 1;
	break;
      }
	  if (vpi_scan(arg_itr) != NULL) {
	vpi_printf("ERROR: $getProcessed() requires 3 argument; has too many\n");
	vpi_free_object(arg_itr);
	err_flag = 1;
	break;
      }
    } while (0 == 1); /* end of test group; only executed once */

    if (err_flag) {
      vpi_control(vpiFinish, 1);  /* abort simulation */
    }

    return(0);
}
