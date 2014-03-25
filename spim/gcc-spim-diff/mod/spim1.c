#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h" 
#include "rtl.h"
#include "tree.h"
#include "tm_p.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "real.h"
#include "insn-config.h"
#include "conditions.h"
#include "output.h"
#include "insn-codes.h"
#include "insn-attr.h"
#include "flags.h"
#include "except.h"
#include "function.h"
#include "recog.h"
#include "expr.h"
#include "optabs.h"
#include "toplev.h"
#include "basic-block.h"
#include "ggc.h"
#include "dbxout.h"
#include "target.h"
#include "target-def.h"
//#include "targhooks.h"
#include "langhooks.h"
#include "cgraph.h"
#include "gimple.h"
#include "df.h"
#include "rtl.h"

#define IITB_YES 1
#define IITB_NO 0
#define return_addr_rtx gen_rtx_REG(SImode,31)

rtx compare_op0;
rtx compare_op1;

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
			|| (REGN>=28 && REGN<=31)
			|| (REGN == 1)
	)
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
        if((REGN>=8 && REGN<=15)||(REGN==24)||(REGN==25) || (REGN>=32 && REGN<=39))
                return IITB_YES;
        return IITB_NO;
}

int
is_callee_saved_reg(int REGN)
{
        if((REGN>=16 && REGN<=23) || (REGN>=40 && REGN<=47))
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


#undef TARGET_FUNCTION_ARG
#define TARGET_FUNCTION_ARG spim_function_arg

#undef TARGET_FUNCTION_ARG_ADVANCE
#define TARGET_FUNCTION_ARG_ADVANCE spim_function_arg_advance

#undef FUNCTION_RETURN_POPS_ARGS
#define TARGET_RETURN_POPS_ARGS spim_return_pops_args

static int spim_return_pops_args(tree funcdecl, tree functype, int size){
    return 0;
}

static void spim_function_arg_advance(cumulative_args_t cum_v, enum machine_mode mode,
	const_tree type, bool named){
//  CUMULATIVE_ARGS *cum = get_cumulative_args (cum_v);
//	cum += 1;
	//cum_v.p++;
}

rtx_def* spim_function_arg(cumulative_args_t cum_v, enum machine_mode mode,
	const_tree type, bool named){
	return NULL;
}

//Float addition :: outputs labels and directives for float
void
spim_output_literal (FILE *file, rtx x, enum machine_mode mode, int labelno)
{
	char s[60];
	long value_long[2];
	REAL_VALUE_TYPE r;
	int size;

#ifdef _M_USE_TARGET_INIT
	(*targetm.asm_out.internal_label) (file, "LC", labelno );
#endif

	switch (GET_MODE_CLASS (mode))
	{
		case MODE_FLOAT:
			if (GET_CODE (x) != CONST_DOUBLE)
				abort ();
			REAL_VALUE_FROM_CONST_DOUBLE (r, x);
			switch (mode)
			{
				case SFmode:
					real_to_decimal(s, CONST_DOUBLE_REAL_VALUE(x), sizeof(s), 0, 1);
					fprintf(file,"\t.float %s\n", s);
					break;
				case DFmode:
					REAL_VALUE_TO_TARGET_DOUBLE (r, value_long);
					fprintf (file, "\t.word 0x%08lx, 0x%08lx\n",
					value_long[0], value_long[1]);
					break;
				default:
					abort ();
			}
			break;
		case MODE_INT:
		case MODE_PARTIAL_INT:
			size = GET_MODE_SIZE (mode);
			if (size == 4)
			{
				fprintf(file,"\t.word ");
				output_addr_const (file, x);
				fputs ("\n", file);
			}
			else if (size == 8)
			{
				output_addr_const (file, operand_subword (x, 0, 0, DImode));
				fputs (", ", file);
				output_addr_const (file, operand_subword (x, 1, 0, DImode));
				fputs ("\n", file);
			}
			else
				abort ();
			break;
		default:
			abort ();
	}
}

void
spim_asm_internal_label(FILE *stream, const char *prefix, long unsigned int labelno)
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

//Float addition :: macro defined for mode float
int
hard_regno_mode_ok (int REGN, enum machine_mode MODE)
{
        if(GET_MODE_CLASS(MODE) == MODE_INT)
	{
            if(GET_MODE_SIZE(MODE) >= UNITS_PER_WORD)  /*Double Integer value.*/
            {
                if(REGN >= 0 && REGN <= FIRST_PSEUDO_REGISTER && (REGN % 2) == 0)
                     return IITB_YES;
                return IITB_NO;
            }
            else
            {
                if(REGN >= 0 && REGN <= FIRST_PSEUDO_REGISTER)
                    return IITB_YES;
                return IITB_NO;
            }
        }else
        if(GET_MODE_CLASS(MODE) == MODE_FLOAT)
	{
            if(GET_MODE_SIZE(MODE) >= UNITS_PER_WORD)  
            {
                if(REGN >= FIRST_FLOAT_REGNO && REGN <=LAST_FLOAT_REGNO && (REGN % 2) == 0)
                     return IITB_YES;
                return IITB_NO;
            }else
	    {
		if(REGN >= FIRST_FLOAT_REGNO && REGN <=LAST_FLOAT_REGNO)
			return IITB_YES;
	    }
	}
	return IITB_NO;
}
//Float addition :: FP registers added.
const enum reg_class spim_regno_to_class[] =
{
	ZERO_REGS, BASE_REGS, BASE_REGS, BASE_REGS,
	BASE_REGS, BASE_REGS, BASE_REGS, BASE_REGS,
	CALLER_SAVED_REGS, CALLER_SAVED_REGS, CALLER_SAVED_REGS, CALLER_SAVED_REGS,
	CALLER_SAVED_REGS, CALLER_SAVED_REGS, CALLER_SAVED_REGS, CALLER_SAVED_REGS,
	CALLEE_SAVED_REGS, CALLEE_SAVED_REGS, CALLEE_SAVED_REGS, CALLEE_SAVED_REGS,
	CALLEE_SAVED_REGS, CALLEE_SAVED_REGS, CALLEE_SAVED_REGS, CALLEE_SAVED_REGS,
	CALLER_SAVED_REGS, CALLER_SAVED_REGS, NO_REGS, NO_REGS,
	BASE_REGS, BASE_REGS, BASE_REGS, BASE_REGS,
	FP_REGS, FP_REGS, CALLER_SAVED_REGS, CALLER_SAVED_REGS,
	CALLER_SAVED_REGS, CALLER_SAVED_REGS, CALLER_SAVED_REGS, CALLER_SAVED_REGS,
	CALLEE_SAVED_REGS, CALLEE_SAVED_REGS, CALLEE_SAVED_REGS, CALLEE_SAVED_REGS,
	CALLEE_SAVED_REGS, CALLEE_SAVED_REGS, CALLEE_SAVED_REGS, CALLEE_SAVED_REGS,
	FP_REGS, FP_REGS, FP_REGS, FP_REGS,
	FP_REGS, FP_REGS, FP_REGS, FP_REGS,
	FP_REGS, FP_REGS, FP_REGS, FP_REGS,
	FP_REGS, FP_REGS, FP_REGS, FP_REGS
};


int 
modes_tieable_p(enum machine_mode MODE1, enum machine_mode MODE2)
{
        if((MODE1 == MODE2)
      		|| (GET_MODE_SIZE(MODE1) <= GET_MODE_SIZE(MODE2)
                  && GET_MODE_CLASS(MODE1) == GET_MODE_CLASS(MODE2)))
                return IITB_YES;
        return IITB_NO;
}

enum reg_class
regno_reg_class(int REGN)
{ 
	if(REGN==0)
                return ZERO_REGS;
        if(is_callee_saved_reg(REGN))
                return CALLEE_SAVED_REGS;
	if(is_caller_saved_reg(REGN))
                return CALLER_SAVED_REGS;
        if(is_base_reg(REGN))
                return BASE_REGS;
        if(is_general_reg(REGN))
                return GENERAL_REGS;
        return NO_REGS;
}

enum reg_class
reg_class_from_letter (char ch)
{
        switch(ch)
        {
                case 'b':return BASE_REGS;
                case 'x':return CALLEE_SAVED_REGS;
                case 'y':return CALLER_SAVED_REGS;
		case 'z':return ZERO_REGS;
		case 'f':return FP_REGS;
        }
	return NO_REGS;
}

int
IITB_regno_ok_for_base_p (int REGN)
{
        if(is_base_reg(REGN) 
                || (REGN >= FIRST_PSEUDO_REGISTER 
                && is_base_reg(reg_renumber[REGN])))
                return IITB_YES;
        return IITB_NO;
}

int
regno_ok_for_index_p (int REGN)
{
        if(is_index_reg(REGN) 
                || (REGN >= FIRST_PSEUDO_REGISTER 
	               && is_index_reg(reg_renumber[REGN])))
                return IITB_YES;
        return IITB_NO;
}

int
const_ok_for_letter_p(int VALUE,char CH)
{
        switch(CH)
        {
                case 'I':if(abs(VALUE) < 0xff) return IITB_YES;
                                  return IITB_NO;
                case 'J':if(abs(VALUE) < 0x01f) return IITB_YES;
                                  return IITB_NO;
                case 'K':if(abs(VALUE) < 0xffff) return IITB_YES;
                                  return IITB_NO;
        }
	return IITB_NO;
}

int
starting_frame_offset (void)
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

int registers_to_be_saved(void)
{
        int i,num;
        for(i=0,num=0;i<FIRST_PSEUDO_REGISTER;i++)
        {
                if(df_regs_ever_live_p(i) && !call_used_regs[i] && !fixed_regs[i])
                        num++;
        }
        return num;
}

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
	else if(from == ARG_POINTER_REGNUM && to == HARD_FRAME_POINTER_REGNUM)
        {
                return 0;
        }
	else if(from == HARD_FRAME_POINTER_REGNUM && to == STACK_POINTER_REGNUM)
	{
		return ((3+registers_to_be_saved())*4+get_frame_size());
	}
	else 
		printf("\nIt should not come here... Trying to eliminate non-eliminable register!\n");
		return 0;
}

//Float addition :: 
rtx
function_value (const_tree valtype, const_tree func ATTRIBUTE_UNUSED, enum machine_mode mode)
{
	if(valtype)
	{
		mode = TYPE_MODE (valtype);
		if(mode == SFmode)
			return (gen_rtx_REG(SFmode,FP_RETURN));
		else 
			return (gen_rtx_REG(SImode,GP_RETURN));
	}
	if (GET_MODE_CLASS (mode) == MODE_FLOAT)
		return gen_rtx_REG (mode, FP_RETURN);

	return gen_rtx_REG(mode,GP_RETURN);
}

//Float addition :: 
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
	rtx op1,op2;
	if(CONSTANT_ADDRESS_P(X))
		return 1;
	if(GET_CODE(X)==REG && is_base_reg(REGNO(X)))
		return 1;
	if(GET_CODE(X)==PLUS)
	{
		op1=XEXP(X,0);
		op2=XEXP(X,1);
		if(GET_CODE(op1)==REG && CONSTANT_ADDRESS_P(op2) && is_base_reg(REGNO(op1)))
			return 1;
		if(GET_CODE(op2)==REG && CONSTANT_ADDRESS_P(op1) && is_base_reg(REGNO(op2)))
			return 1;
	}
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
        if(GET_CODE(X)==PLUS)
        {
                op1=XEXP(X,0);
                op2=XEXP(X,1);
                if(GET_CODE(op1)==REG && CONSTANT_ADDRESS_P(op2) && non_strict_base_reg(REGNO(op1)))
                        return 1;
                if(GET_CODE(op2)==REG && CONSTANT_ADDRESS_P(op1) && non_strict_base_reg(REGNO(op2)))
                        return 1;
        }
        return 0;
}


/*Here also, strict and non-strict varients are needed.*/
int 
reg_ok_for_base_p1(rtx x)
{
	if(is_base_reg(REGNO(x)))
		return IITB_YES;
	return IITB_NO;
}
int
reg_ok_for_base_p2(rtx x)
{
	if(non_strict_base_reg(REGNO(x)))
		return IITB_YES;
	return IITB_NO;
}

/*Here also, strict and non-strict varients are needed.*/
int 
reg_ok_for_index_p1(rtx x)
{
	if(is_index_reg(REGNO(x)))
		return IITB_YES;
	return IITB_NO;
}
int
reg_ok_for_index_p2(rtx x)
{
	if(non_strict_index_reg(REGNO(x)))
		return IITB_YES;
	return IITB_NO;
}


int
legitimate_constant_p (rtx X)
{
        return (GET_CODE(X) == CONST_DOUBLE || GET_CODE(X) == CONST_INT);
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
        fprintf(STREAM,"\t.skip %u\n", NBYTES);
}


//Float addition ::
void
print_operand(FILE *STREAM,rtx X,char CODE)
{
	rtx op;
	switch(CODE)
	{
		case 0:/* Generally, if there is no code after % character, then
			  it is considered as register operand. But, for safe case,
			  at this place also, I am taking care of all other operands.*/
			if(GET_CODE(X) == REG)
			{
				fprintf(STREAM,"%s",reg_names[XINT(X,0)]);
				break;
			}
			else if(GET_CODE(X) == MEM)
			{
			        op=XEXP(X,0);
		                PRINT_OPERAND_ADDRESS(STREAM,op);
			}
			else if(GET_CODE(X)==CONST_INT)
	                {
	                        fprintf(STREAM,"%d",XINT(X,0));
	                }
			else if(GET_CODE(X)==CONST_DOUBLE)
	                {
				char s[60];
				real_to_decimal(s, CONST_DOUBLE_REAL_VALUE(X), sizeof(s), 0, 1);
	                        fprintf(STREAM,"%s", s);
	                }
			else if(CONSTANT_ADDRESS_P(X))
			{
				PRINT_OPERAND_ADDRESS(STREAM,X);
			}
			else if(legitimate_constant_p(X))
			{
				if(GET_CODE(X)==LABEL_REF)
				{
					ASM_OUTPUT_LABELREF(STREAM,XSTR(X,0));
				}
				else
				{
					op=XEXP(X,0);
					PRINT_OPERAND(STREAM,op,0);
					printf("#Legitimate address");
				}
			}
	                else
	                {
				  printf("other case"
					  " in CODE = 0");
	                }
			break;			
		case 'm':
			if(GET_CODE(X) != MEM)
			{
				printf("Invalid operand : Not a memory operand");
				return;
			}
			op=XEXP(X,0);
			PRINT_OPERAND_ADDRESS(STREAM,op);
			break;
		case 's':
			if(GET_CODE(X)==SYMBOL_REF)
			{
				output_addr_const(STREAM,X);
			}
			else
			{
				printf("Other than symbol ref not allowed");
			}
			break;
		default:
			printf("In print operand default");
	}
}

void
print_operand_address(FILE *STREAM,rtx X)
{
	rtx op1,op2,temp;
	int num;
	switch(GET_CODE(X))
	{
		case SUBREG:
			/*As in case of register indirect mode, where address 
			  of operand is present in subreg.*/
			fprintf(STREAM,"0(%s)",reg_names[REGNO(XEXP(X,0))]);
			break;
		case REG:
			/*As in case of register indirect mode, address of operand
			  in memory is persent in register REGNO(X).*/
			fprintf(STREAM,"0(%s)",	reg_names[REGNO(X)]);
			break;
		case PLUS:
			/*The address can be in base displacement or base - index
			  form of addressing.*/
			op1 = XEXP(X,0);
			op2 = XEXP(X,1);
			if(GET_CODE(op1) == CONST_INT 
				&& (GET_CODE(op2) == REG 
					|| GET_CODE(op2) == SUBREG))
				/*base displacement*/
			{
				fprintf(STREAM,"%d(%s)", INTVAL(op1),
						((GET_CODE(op2)==REG)
						?reg_names[REGNO(op2)]
						:reg_names[REGNO(XEXP(op2,0))]));
			}
			else if (GET_CODE(op2) == CONST_INT 
					&& (GET_CODE(op1) == REG 
						|| GET_CODE(op1) == SUBREG))
				/*base displacement*/
			{
				fprintf(STREAM,"%d(%s)", INTVAL(op2),
						((GET_CODE(op1) == REG)
						?reg_names[REGNO(op1)]
						:reg_names[REGNO(XEXP(op1,0))]));
			}
			break;
		default:
			if(CONSTANT_ADDRESS_P(X))
			{
				output_addr_const(STREAM,X);
			}
			else
				fprintf(STREAM,"Coming in default part of" 
						" print_operand_address");
			break;
	}
}

void
asm_generate_internal_label(char *STRING,const char *PREFIX,int NUM)
{
        sprintf(STRING,"%s%d", PREFIX,NUM);
}
void
asm_output_local(FILE *STREAM,const char *NAME,int SIZE,int ROUNDED)
{
        fprintf(STREAM,"\t.reserve ");
        assemble_name (STREAM, NAME);
        fprintf (STREAM, ",%u,\"bss\"\n", SIZE);
}

void
asm_output_common(FILE *STREAM,const char *NAME,int SIZE,int ROUNDED)
{
	int i;
	fprintf(STREAM, "\t.data\n");
        assemble_name(STREAM,NAME);
        fprintf(STREAM, ":\t.word \t");
          for(i=0;i<SIZE/UNITS_PER_WORD;i++)
          {
                  fprintf(STREAM, "0 \t");
          }
          fprintf(STREAM,"\n");
}

int
asm_output_symbol_ref(FILE *stream, rtx sym)
{
        //fprintf(stream,"_");
        assemble_name(stream, XSTR((sym),0));
}

void
function_profiler(FILE*asm_file,int labelno)
{
}

struct gcc_target targetm = TARGET_INITIALIZER;

int
symbolic_operand (rtx op, enum machine_mode mode)
{
	if(GET_CODE(op) == SYMBOL_REF || GET_CODE(op) == LABEL_REF)
        {
                return 1;
        }
        return 0;
}
int
constant_operand (rtx op, enum machine_mode mode)
{
        if(GET_CODE(op)!=CONST_INT)
                return 0;
        if(INTVAL(op) <=0xff)
        {
                return 1;
        }
        return 0;
}

int
zero_register_operand (rtx op, enum machine_mode mode)
{
        if(GET_CODE(op)!=REG)
                return 0;
        if(REGNO(op) == 0)
        {
                return 1;
        }
        return 0;
}

void
spim_prologue(void)
{
        int i,j;
	
        emit_move_insn(gen_rtx_MEM(SImode,plus_constant(SImode, stack_pointer_rtx,-4)),return_addr_rtx);
        emit_move_insn(gen_rtx_MEM(SImode,plus_constant(SImode, stack_pointer_rtx,-8)),stack_pointer_rtx);
        emit_move_insn(gen_rtx_MEM(SImode,plus_constant(SImode, stack_pointer_rtx,-12)),hard_frame_pointer_rtx);
        emit_move_insn(hard_frame_pointer_rtx, plus_constant(SImode, stack_pointer_rtx,0));
        for(i=0,j=4;i<FIRST_PSEUDO_REGISTER;i++)
        {
                if(df_regs_ever_live_p(i) && !call_used_regs[i] && !fixed_regs[i])
                {
			if(REGNO_REG_CLASS(i) == FP_REGS)
	                        emit_move_insn(gen_rtx_MEM(SFmode,plus_constant(SImode, hard_frame_pointer_rtx,-4*j)), gen_rtx_REG(SFmode,i));  
			else
	                        emit_move_insn(gen_rtx_MEM(SImode,plus_constant(SImode, hard_frame_pointer_rtx,-4*j)), gen_rtx_REG(SImode,i));

       	                j++;
                }
        }
//ishaktremove       
//emit_move_insn(stack_pointer_rtx, plus_constant(SImode, hard_frame_pointer_rtx,-((3+j)*4+get_frame_size())));
/******Jaishri added*/
//emit_insn(gen_rtx_SET(SImode, stack_pointer_rtx, plus_constant(SImode, hard_frame_pointer_rtx,-((3+j)*4+get_frame_size())) ));
        emit_move_insn(stack_pointer_rtx, plus_constant(SImode, hard_frame_pointer_rtx,-((4+j)*4+get_frame_size())));
}


//Float addition ::
void
spim_epilogue(void)
{
        int i,j;
       
        for(i=0,j=3;i<FIRST_PSEUDO_REGISTER;i++) /*Restore all the callee-registers from stack frame*/
        {
                if(df_regs_ever_live_p(i) && !call_used_regs[i] && !fixed_regs[i])
                {
			if(REGNO_REG_CLASS(i) == FP_REGS)
	                        emit_move_insn(gen_rtx_REG(SFmode,i), gen_rtx_MEM(SFmode,plus_constant(SImode, hard_frame_pointer_rtx,-4*j)));   
			else
	                        emit_move_insn(gen_rtx_REG(SImode,i), gen_rtx_MEM(SImode,plus_constant(SImode, hard_frame_pointer_rtx,-4*j)));
                        j++;
                }
        }
	/*Restore stack pointer*/
	emit_move_insn(stack_pointer_rtx, plus_constant(SImode, hard_frame_pointer_rtx,0));
	/*Restore frame pointer*/
	emit_move_insn(hard_frame_pointer_rtx, gen_rtx_MEM(SImode,plus_constant(SImode, stack_pointer_rtx,-12)));
	/*Restore return address*/
	emit_move_insn(return_addr_rtx, gen_rtx_MEM(SImode,plus_constant(SImode, stack_pointer_rtx,-4)));
	/*Jump instruction*/
	emit_jump_insn(gen_IITB_return());
}

#undef TARGET_ASM_ALIGNED_SI_OP
#define TARGET_ASM_ALIGNED_SI_OP "\t.word\t"
