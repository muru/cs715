
/*
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h" //
#include "rtl.h"
#include "tree.h"
#include "tm_p.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "real.h"
#include "insn-config.h"
#include "conditions.h"
#include "output.h"
#include "insn-codes.h"//
#include "insn-attr.h"
#include "flags.h"
#include "except.h"//
#include "function.h"
#include "recog.h"
#include "expr.h"
#include "optabs.h"
#include "toplev.h"
#include "basic-block.h"//
#include "ggc.h"
#include "target.h"
#include "target-def.h"
#include "langhooks.h"
#include "cgraph.h"//

#include "gimple.h"//
*/




#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include <signal.h>
#include "rtl.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "real.h"
#include "insn-config.h"
#include "conditions.h"
#include "insn-attr.h"
#include "recog.h"
#include "toplev.h"
#include "output.h"
#include "tree.h"
#include "function.h"
#include "expr.h"
#include "optabs.h"
#include "libfuncs.h"
#include "flags.h"
#include "reload.h"
#include "tm_p.h"
#include "ggc.h"
#include "gstab.h"
#include "hashtab.h"
#include "debug.h"
#include "target.h"
#include "target-def.h"
#include "integrate.h"
#include "langhooks.h"
#include "cfglayout.h"
#include "sched-int.h"
#include "gimple.h"
#include "bitmap.h"
#include "diagnostic.h"

#define IITB_YES 1
#define IITB_NO 0


/* Initialize the GCC target structure.  
 * All macros tagged as target_hook are defined here, 
 * instead of being defined in .h file. */

#undef TARGET_STRUCT_VALUE_RTX
#define TARGET_STRUCT_VALUE_RTX \
spim_struct_value_rtx
#undef TARGET_ASM_GLOBALIZE_LABEL
#define TARGET_ASM_GLOBALIZE_LABEL \
spim_asm_globalize_label
#undef TARGET_ASM_INTERNAL_LABEL 
#define TARGET_ASM_INTERNAL_LABEL \
spim_asm_internal_label

void
spim_asm_internal_label(FILE *stream, const char *prefix, unsigned long labelno)
{
	return;
}

void
spim_asm_globalize_label(FILE *stream, const char *name)
{
	return;
}

rtx
spim_struct_value_rtx(tree fndecl, int incoming)
{
	return 0;
}


int
hard_regno_mode_ok (int REGN, enum machine_mode MODE)
{
        return IITB_YES;
}

int 
modes_tieable_p(enum machine_mode MODE1, enum machine_mode MODE2)
{
        return IITB_YES;
}

enum reg_class
regno_reg_class(int REGN)
{ 
        return ALL_REGS;
}

rtx
function_value ()
{
	return 0;
}

int
constant_address_p (rtx X)
{
	return 0;
}

/* This function corresponds to the macro GO_IF_LEGITIMATE_ADDRESS. There 
 * are two varients of this macro: one when the registers used may or may 
 * not be hard registers, and second when the registers must be LEGITIMATE 
 * HARD REGISTERS. This function checks if the address is legitimate or not. */

int
legitimate_address1(enum machine_mode MODE,rtx X)
{
	return 0;

}

int
legitimate_address2(enum machine_mode MODE,rtx X)
{
        return 0;
}


/* Here also, strict and non-strict varients are needed.*/

int 
reg_ok_for_base_p1(rtx x)
{
	return IITB_YES;
}

int
reg_ok_for_base_p2(rtx x)
{
	return IITB_YES;
}

/* Here also, strict and non-strict varients are needed.*/
int 
reg_ok_for_index_p1(rtx x)
{
	return IITB_NO;
}

int
reg_ok_for_index_p2(rtx x)
{
	return IITB_NO;
}

rtx 
legitimize_address(rtx X,rtx OLDX, enum machine_mode MODE)
{
	return 0;
}


int
legitimate_constant_p (rtx X)
{
        return 0;
}

struct gcc_target targetm = TARGET_INITIALIZER;

rtx
gen_jump (rtx operand0 ATTRIBUTE_UNUSED)
{
  return 0;
}

rtx
gen_indirect_jump (rtx operand0 ATTRIBUTE_UNUSED)
{
	  return 0;
}
