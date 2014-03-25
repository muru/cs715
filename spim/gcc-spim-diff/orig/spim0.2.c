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

#define IITB_YES 1
#define IITB_NO 0

int
is_index_reg(int REGN)
{
	return IITB_NO;
}


int 
is_base_reg(int REGN)
{
	if(is_caller_saved_reg(REGN) 
			|| is_callee_saved_reg(REGN) 
			|| is_arg_reg(REGN) 
			|| is_return_val_reg(REGN)
			|| (REGN>=28 && REGN<=31))
		return IITB_YES;
	return IITB_NO;
}

int
is_arg_reg(int REGN)
{
        if( (REGN>=4 && REGN<=7))
                return IITB_YES;
        return IITB_NO;
}

int
is_return_val_reg(int REGN)
{
        if((REGN==2) || (REGN==3))
                return IITB_YES;
        return IITB_NO;
}

int
non_strict_index_reg(int REGN)
{
	return IITB_NO;
}


int
non_strict_base_reg(int regn)
{
	if(is_base_reg(regn))
		return IITB_YES;
	if(regn>=FIRST_PSEUDO_REGISTER)
		return IITB_YES;
	return IITB_NO;
}

int
is_caller_saved_reg(int REGN)
{
        if((REGN>=8 && REGN<=15)||(REGN==24)||(REGN==25))
                return IITB_YES;
        return IITB_NO;
}

int
is_callee_saved_reg(int REGN)
{
        if((REGN>=16 && REGN<=23))
                return IITB_YES;
        return IITB_NO;
}

int
is_general_reg(int REGN)
{
        if(REGN<FIRST_PSEUDO_REGISTER)
                return IITB_YES;
        return IITB_NO;
}

/* Initialize the GCC target structure. 
 * All macros taged as target_hook are defined here, instead of defining
 * in .h file. */

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
	fprintf(stream,"%s%d:",prefix,labelno);
	return;
}

void
spim_asm_globalize_label(FILE *stream, const char *name)
{
	fprintf(stream,"\t.globl %s\n",name);
	return;
}

rtx
spim_struct_value_rtx(tree fndecl, int incoming)
{
	return gen_rtx_REG(Pmode, 2);
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

enum reg_class
reg_class_from_letter (char ch)
{
	return NO_REGS;
}

int
IITB_regno_ok_for_base_p (int REGN)
{
        return IITB_YES;
}

int
regno_ok_for_index_p (int REGN)
{
        return IITB_NO;
}

int
const_ok_for_letter_p(int VALUE,char CH)
{
	return IITB_NO;
}

int
starting_frame_offset ()
{
	return 0;
}

int
initial_frame_pointer_offset (int DEPTH)
{
	int size;
	size = get_frame_size();
	return size;	
}

int registers_to_be_saved()
{
        int i,num;
        for(i=0,num=0;i<FIRST_PSEUDO_REGISTER;i++)
        {
                if(df_regs_ever_live_p(i) && !call_used_regs[i] && !fixed_regs[i])
                        num++;
        }
        return num;
}

/* Functions related to activation records could be empty in this level, but we have
 * chosen to define activation records completely and hence, functions have been 
 * defined fully.*/
int
initial_elimination_offset(int from, int to)
{
	if(from == FRAME_POINTER_REGNUM && to == STACK_POINTER_REGNUM)
	{
		return (get_frame_size());
	}
	else if(from == FRAME_POINTER_REGNUM && to == HARD_FRAME_POINTER_REGNUM)
	{
		return -(3+registers_to_be_saved())*4;
	}
	else if(from == ARG_POINTER_REGNUM && to == STACK_POINTER_REGNUM)
	{
		return ((3+registers_to_be_saved())*4+get_frame_size());
	}
	else if(from == HARD_FRAME_POINTER_REGNUM && to == STACK_POINTER_REGNUM)
	{
		return ((3+registers_to_be_saved())*4+get_frame_size());
	}
	else 
		printf("\nIt should not come here... Trying to eliminate non-eliminable register!\n");
		return 0;
}

rtx
function_value ()
{
	//Return register is register 2 when value is of type SImode.
	return (gen_rtx_REG(SImode,2));
}

int
constant_address_p (rtx X)
{
	return (CONSTANT_P(X) && 
		GET_CODE(X)!=CONST_DOUBLE
		&& GET_CODE(X)!=CONST_VECTOR);
}

/*This function corresponds to the macro GO_IF_LEGITIMATE_ADDRESS. There are
 * two varients of this macro: one when the registers used may or may not be 
 * hard registers, and second when the registers must be LEGITIMATE HARD 
 * REGISTERS. This function checks if the address is legitimate or not.*/
int
legitimate_address1(enum machine_mode MODE,rtx X)
{
	return 0;

}
/*Non-strict definition*/
int
legitimate_address2(enum machine_mode MODE,rtx X)
{
        
rtx op1,op2;
        if(CONSTANT_ADDRESS_P(X))
                return 1;
        
	if(GET_CODE(X)==REG && non_strict_base_reg(REGNO(X)))
                return 1;
        
	   
	return 0;
}


/*Here also, strict and non-strict varients are needed.*/
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

/*Here also, strict and non-strict varients are needed.*/
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
	return X;
}


int
legitimate_constant_p (rtx X)
{
        return (GET_CODE(X)!= CONST_DOUBLE);
}

void
asm_output_align(FILE *STREAM, int POWER)
{
        int num,i;
        num=1;
        for(i=0;i<POWER;i++)
        {
                num=num*2;
        }
         fprintf (STREAM, "\t.align %d\n",POWER);
}

void
asm_output_skip(FILE  *STREAM,int NBYTES)
{
}


void
print_operand(FILE *STREAM,rtx X,char CODE)
{
}


void
print_operand_address(FILE *STREAM,rtx X)
{
}

void
asm_generate_internal_label(char *STRING,char *PREFIX,int NUM)
{
        sprintf(STRING,"%s%d", PREFIX,NUM);
}
void
asm_output_local(FILE *STREAM,char *NAME,int SIZE,int ROUNDED)
{
}

void
asm_output_common(FILE *STREAM,char *NAME,int SIZE,int ROUNDED)
{
}

void
function_profiler(FILE*asm_file,int labelno)
{
}

void
initialize_trampoline()
{
        return;
}

struct gcc_target targetm = TARGET_INITIALIZER;

void
spim_epilogue()
{
        emit_jump_insn(gen_IITB_return());
}


