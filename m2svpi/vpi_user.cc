/**********************************************************************
 * Example vpi_user.c file
 *
 * vpi_user.c file to register PLI applications using the VPI library.
 *
 * For the book, "The Verilog PLI Handbook" by Stuart Sutherland
 *  Copyright 1999 & 2001, Kluwer Academic Publishers, Norwell, MA, USA
 *   Contact: www.wkap.il
 *  Example copyright 1998, Sutherland HDL Inc, Portland, Oregon, USA
 *   Contact: www.sutherland-hdl.com
 *********************************************************************/

#include "vpi_user.h"

/* prototypes of the PLI registration routines */
extern void PLIbook_ShowSignals_register();
extern void m2s_initialize_register();
extern void m2s_reset_register();
extern void m2s_access_register();
extern void m2s_step_register();
extern void m2s_getProcessed_register();
extern void m2s_finalize_register();

void (*vlog_startup_routines[])() =
{
	/*** add user entries here ***/
	PLIbook_ShowSignals_register,
	m2s_initialize_register,
	m2s_reset_register,
	m2s_step_register,
	m2s_getProcessed_register,
	m2s_access_register,
	m2s_finalize_register,
	0 /*** final entry must be 0 ***/
};
