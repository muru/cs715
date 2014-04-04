#ifndef __SUPPORT_HH__
#define __SUPPORT_HH__


typedef enum {extends, instantiates, overrides, cmd_spec} refineOp;

typedef enum {no_qualifier, insn, expand, peephole2, split, insn_and_split, code_iterator} insnQualifier;

typedef enum {rtl_op, user_defined} patNameCategory;

typedef enum {all_constraints, all_predicates, node_access, mode_modification,node_access_mode,node_access_predicate,node_access_constraint,node_access_const_unspec} lhsCategory;

typedef enum {asgn, pat_instantiation, rtl_instantiation, mode_modifier, constraint_modification} stmtCategory;

typedef enum {p_m_c_spec, dup_num_spec, num_spec, reg_spec, const_spec, scratch_spec,bracket_start,bracket_end,extra_reg_spec,reg_num_spec} opdCategory;

typedef enum {single_operand, single_predicate, single_constraint, single_mode, rtl_operator,
                predicate_list, constraint_list,abstract_pattern, instantiation,const_unspec} rhsCategory;

typedef enum {define_code_iterator, define_mode_iterator,define_code_attr,define_mode_attr ,define_constants,define_c_enum,define_asm_attributes,define_reservation,define_cpu_unit,define_automaton } listPatType;




#endif
