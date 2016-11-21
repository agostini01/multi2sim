#define VPI_1995 0 /* set to non-zero for Verilog-1995 compatibility */

#include <cstdlib>    /* ANSI C standard library */
#include "vpi_user.h"  /* IEEE 1364 PLI VPI routine library  */
#include "/home/agostini/multi2sim/multi2sim/src/m2s.h"

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
    Multi2Sim::getInstance().m2sInitialize();
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
    return(32);
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
    PLI_INT32   mod, type, address;

    systf_handle = vpi_handle(vpiSysTfCall, NULL);
    arg_itr = vpi_iterate(vpiArgument, systf_handle);
    if (arg_itr == NULL) {
      vpi_printf("ERROR: $pow failed to obtain systf arg handles\n");
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

    Multi2Sim::getInstance().m2sAccess((unsigned int) mod
				      ,(unsigned int) type
				      ,(unsigned int) address
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
	vpi_printf("ERROR: $access() requires 3 arguments; has none\n");
	err_flag = 1;
	break;
      }
      arg_handle = vpi_scan(arg_itr);
      tfarg_type = vpi_get(vpiType, arg_handle);
      if ( (tfarg_type != vpiReg) &&
	   (tfarg_type != vpiIntegerVar) &&
	   (tfarg_type != vpiConstant)   ) {
	vpi_printf("ERROR: $access() arg1 must be number, variable or net\n");
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
      if (vpi_scan(arg_itr) != NULL) {
	vpi_printf("ERROR: $access() requires 3 arguments; has too many\n");
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


