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
#include "dbxout.h"
#include "target.h"
#include "target-def.h"
#include "langhooks.h"
#include "cgraph.h"//
#include "gimple.h"//

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

/* This function now changes. */
void
spim_asm_globalize_label(FILE *stream, const char *name)
{
	fprintf(stream,"\t.globl %s\n",name);
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

/*int
legitimate_address2(enum machine_mode MODE,rtx X)
{
        return 0;
}
*/

int
legitimate_address2(enum machine_mode MODE,rtx X)
{
	rtx op1,op2;
        if(CONSTANT_ADDRESS_P(X))
                return 1;
        
	if(GET_CODE(X)==REG)
                return 1;
        
       
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

/*************************** New Functions *******************************/

struct gcc_target targetm = TARGET_INITIALIZER;

void
spim_epilogue()
{
	emit_jump_insn(gen_IITB_return());
}

void
print_operand(FILE *STREAM,rtx X,char CODE)
{
	return ;
}


void
print_operand_address(FILE *STREAM,rtx X)
{
	return ;
}

void
asm_output_align(FILE *STREAM, int POWER)
{
	return ;
}

void
asm_output_skip(FILE  *STREAM,int NBYTES)
{
	return ;
}

void
asm_output_local(FILE *STREAM,const char *NAME,int SIZE,int ROUNDED)
{
	return ;
}

void
asm_output_common(FILE *STREAM, const char *NAME,int SIZE,int ROUNDED)
{
	return ;
}

void
asm_generate_internal_label(const char *STRING,const char *PREFIX,int NUM)
{
	return ;
}

void
function_profiler(FILE*asm_file,int labelno)
{

	return ;
}

void
initialize_trampoline()
{
        return ;
}
