
//Pattern#1


//Pattern#2
(define_constants [  (R0_REGNUM	0)
 (R1_REGNUM	1)
 (IP_REGNUM	12)
 (SP_REGNUM	13)
 (LR_REGNUM	14)
 (PC_REGNUM	15)
 (LAST_ARM_REGNUM	15)
 (CC_REGNUM	100)
 (VFPCC_REGNUM	101)
                    ])

//Pattern#3
(define_constants [  (DOM_CC_X_AND_Y	0)
 (DOM_CC_NX_OR_Y	1)
 (DOM_CC_X_OR_Y	2)
                    ])

//Pattern#4
(define_constants [  (CMP_CMP	0)
 (CMN_CMP	1)
 (CMP_CMN	2)
 (CMN_CMN	3)
 (NUM_OF_COND_CMP	4)
                    ])

(include"arm-tune.md")

//Pattern#5
(define_attr "is_thumb"
  "no,yes"
	(const (symbol_ref "thumb_code") ) )


//Pattern#6
(define_attr "is_arch6"
  "no,yes"
	(const (symbol_ref "arm_arch6") ) )


//Pattern#7
(define_attr "is_thumb1"
  "no,yes"
	(const (symbol_ref "thumb1_code") ) )


//Pattern#8
(define_attr "shift"
  ""
	 (const_int 0) )


//Pattern#9
(define_attr "fpu"
  "none,vfp"
	(const (symbol_ref "arm_fpu_attr") ) )


//Pattern#10
(define_attr "length"
  ""
	 (const_int 4) )


//Pattern#11
(define_attr "arch"
  "any,a,t,32,t1,t2,v6,nov6,onlya8,neon_onlya8,nota8,neon_nota8,iwmmxt,iwmmxt2"
	 (const_string "any") )


//Pattern#12 
(define_attr "arch"
  "any,a,t,32,t1,t2,v6,nov6,onlya8,neon_onlya8,nota8,neon_nota8,iwmmxt,iwmmxt2"
	 (const_string "any") )


//Pattern#13
(define_attr "opt"
  "any,speed,size"
	 (const_string "any") )


//Pattern#14
(define_attr "opt_enabled"
  "no,yes"
	(cond [ (eq_attr "opt" "any"
		) (const_string "yes") (and (eq_attr "opt" "speed"
		) (match_test "optimize_function_for_speed_p (cfun)") ) (const_string "yes") (and (eq_attr "opt" "size"
		) (match_test "optimize_function_for_size_p (cfun)") ) (const_string "yes") ](const_string "no") ) )


//Pattern#15
(define_attr "insn_enabled"
  "no,yes"
	 (const_string "yes") )


//Pattern#16
(define_attr "enabled"
  "no,yes"
	(cond [ (eq_attr "insn_enabled" "no"
		) (const_string "no") (eq_attr "arch_enabled" "no"
		) (const_string "no") (eq_attr "opt_enabled" "no"
		) (const_string "no") ](const_string "yes") ) )


//Pattern#17
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#18
(define_attr "thumb2_pool_range"
  ""
	 (const_int 0) )


//Pattern#19
(define_attr "arm_neg_pool_range"
  ""
	 (const_int 0) )


//Pattern#20
(define_attr "thumb2_neg_pool_range"
  ""
	 (const_int 0) )


//Pattern#21 
(define_attr "pool_range"
  ""
	(cond [ (eq_attr "is_thumb" "yes"
		) (attr "thumb2_pool_range") ](attr "arm_pool_range") ) )


//Pattern#22
(define_attr "neg_pool_range"
  ""
	(cond [ (eq_attr "is_thumb" "yes"
		) (attr "thumb2_neg_pool_range") ](attr "arm_neg_pool_range") ) )


//Pattern#23 

(define_asm_attributes
 [(set_attr "conds" "clob")
  (set_attr "length" "4")
  (set_attr "pool_range" "250")])

//Pattern#24 
(define_attr "INSN"
  "mov,mvn,smulxy,smlaxy,smlalxy,smulwy,smlawx,mul,muls,mla,mlas,umull,umulls,umlal,umlals,smull,smulls,smlal,smlals,smlawy,smuad,smuadx,smlad,smladx,smusd,smusdx,smlsd,smlsdx,smmul,smmulr,smmla,umaal,smlald,smlsld,clz,mrs,msr,xtab,sdiv,udiv,sat,other"
	 (const_string "other") )


//Pattern#25 
(define_attr "type"
  "simple_alu_imm,\alu_reg,\simple_alu_shift,\alu_shift,\alu_shift_reg,\mult,\block,\float,\fdivd,\fdivs,\fmuls,\fmuld,\fmacs,\fmacd,\ffmas,\ffmad,\f_rints,\f_rintd,\f_minmaxs,\f_minmaxd,\f_flag,\f_loads,\f_loadd,\f_stores,\f_stored,\f_2_r,\r_2_f,\f_cvt,\branch,\call,\load_byte,\load1,\load2,\load3,\load4,\store1,\store2,\store3,\store4,\fconsts,\fconstd,\fadds,\faddd,\ffariths,\ffarithd,\fcmps,\fcmpd,\fcpys"
	(if_then_else (eq_attr "INSN" "smulxy,smlaxy,smlalxy,smulwy,smlawx,mul,muls,mla,mlas,\umull,umulls,umlal,umlals,smull,smulls,smlal,smlals"
		) (const_string "mult") (const_string "alu_reg") ) )


//Pattern#26 
(define_attr "mul64"
  "no,yes"
	(if_then_else (eq_attr "INSN" "smlalxy,umull,umulls,umlal,umlals,smull,smulls,smlal,smlals"
		) (const_string "yes") (const_string "no") ) )


//Pattern#27
(define_attr "wtype"
  "none,wor,wxor,wand,wandn,wmov,tmcrr,tmrrc,wldr,wstr,tmcr,tmrc,wadd,wsub,wmul,wmac,wavg2,tinsr,textrm,wshufh,wcmpeq,wcmpgt,wmax,wmin,wpack,wunpckih,wunpckil,wunpckeh,wunpckel,wror,wsra,wsrl,wsll,wmadd,tmia,tmiaph,tmiaxy,tbcst,tmovmsk,wacc,waligni,walignr,tandc,textrc,torc,torvsc,wsad,wabs,wabsdiff,waddsubhx,wsubaddhx,wavg4,wmulw,wqmulm,wqmulwm,waddbhus,wqmiaxy,wmiaxy,wmiawxy,wmerge"
	 (const_string "none") )


//Pattern#28
(define_attr "ldsched"
  "no,yes"
	(const (symbol_ref "arm_ld_sched") ) )


//Pattern#29
(define_attr "neon_type"
  "neon_int_1,\neon_int_2,\neon_int_3,\neon_int_4,\neon_int_5,\neon_vqneg_vqabs,\neon_vmov,\neon_vaba,\neon_vsma,\neon_vaba_qqq,\neon_mul_ddd_8_16_qdd_16_8_long_32_16_long,\neon_mul_qqq_8_16_32_ddd_32,\neon_mul_qdd_64_32_long_qqd_16_ddd_32_scalar_64_32_long_scalar,\neon_mla_ddd_8_16_qdd_16_8_long_32_16_long,\neon_mla_qqq_8_16,\neon_mla_ddd_32_qqd_16_ddd_32_scalar_qdd_64_32_long_scalar_qdd_64_32_long,\neon_mla_qqq_32_qqd_32_scalar,\neon_mul_ddd_16_scalar_32_16_long_scalar,\neon_mul_qqd_32_scalar,\neon_mla_ddd_16_scalar_qdd_32_16_long_scalar,\neon_shift_1,\neon_shift_2,\neon_shift_3,\neon_vshl_ddd,\neon_vqshl_vrshl_vqrshl_qqq,\neon_vsra_vrsra,\neon_fp_vadd_ddd_vabs_dd,\neon_fp_vadd_qqq_vabs_qq,\neon_fp_vsum,\neon_fp_vmul_ddd,\neon_fp_vmul_qqd,\neon_fp_vmla_ddd,\neon_fp_vmla_qqq,\neon_fp_vmla_ddd_scalar,\neon_fp_vmla_qqq_scalar,\neon_fp_vrecps_vrsqrts_ddd,\neon_fp_vrecps_vrsqrts_qqq,\neon_bp_simple,\neon_bp_2cycle,\neon_bp_3cycle,\neon_ldr,\neon_str,\neon_vld1_1_2_regs,\neon_vld1_3_4_regs,\neon_vld2_2_regs_vld1_vld2_all_lanes,\neon_vld2_4_regs,\neon_vld3_vld4,\neon_vst1_1_2_regs_vst2_2_regs,\neon_vst1_3_4_regs,\neon_vst2_4_regs_vst3_vst4,\neon_vst3_vst4,\neon_vld1_vld2_lane,\neon_vld3_vld4_lane,\neon_vst1_vst2_lane,\neon_vst3_vst4_lane,\neon_vld3_vld4_all_lanes,\neon_mcr,\neon_mcr_2_mcrr,\neon_mrc,\neon_mrrc,\neon_ldm_2,\neon_stm_2,\none"
	 (const_string "none") )


//Pattern#30
(define_attr "conds"
  "use,set,clob,unconditional,nocond"
	(if_then_else (ior (eq_attr "is_thumb1" "yes"
		) (eq_attr "type" "call"
		) ) (const_string "clob") (if_then_else (eq_attr "neon_type" "none"
		) (const_string "nocond") (const_string "unconditional") ) ) )


//Pattern#31
(define_attr "predicable"
  "no,yes"
	 (const_string "no") )


//Pattern#32
(define_attr "model_wbuf"
  "no,yes"
	(const (symbol_ref "arm_tune_wbuf") ) )


//Pattern#33 
(define_attr "write_conflict"
  "no,yes"
	(if_then_else (eq_attr "type" "block,call,load1"
		) (const_string "yes") (const_string "no") ) )


//Pattern#34 
(define_attr "core_cycles"
  "single,multi"
	(if_then_else (eq_attr "type" "simple_alu_imm,alu_reg,\simple_alu_shift,alu_shift,\float,fdivd,fdivs"
		) (const_string "single") (const_string "multi") ) )


//Pattern#35
(define_attr "far_jump"
  "yes,no"
	 (const_string "no") )


//Pattern#36
(define_attr "ce_count"
  ""
	 (const_int 1) )


(include "unspecs.md")
(include "iterators.md")
(include "predicates.md")
(include "constraints.md")
//Pattern#37 
(define_attr "tune_cortexr4"
  "yes,no"
	(const (if_then_else (eq_attr "tune" "cortexr4,cortexr4f,cortexr5"
		) (const_string "yes") (const_string "no") ) ) )


//Pattern#38 
(define_attr "generic_sched"
  "yes,no"
	(const (if_then_else (ior (eq_attr "tune" "fa526,fa626,fa606te,fa626te,fmp626,fa726te,arm926ejs,arm1020e,arm1026ejs,arm1136js,arm1136jfs,cortexa5,cortexa7,cortexa8,cortexa9,cortexa15,cortexm4,marvell_pj4"
		) (eq_attr "tune_cortexr4" "yes"
		) ) (const_string "no") (const_string "yes") ) ) )


(include "marvell-f-iwmmxt.md")
(include "arm-generic.md")
(include "arm926ejs.md")
(include "arm1020e.md")
(include "arm1026ejs.md")
(include "arm1136jfs.md")
(include "fa526.md")
(include "fa606te.md")
(include "fa626te.md")
(include "fmp626.md")
(include "fa726te.md")
(include "cortex-a5.md")
(include "cortex-a7.md")
(include "cortex-a8.md")
(include "cortex-a9.md")
(include "cortex-a15.md")
(include "cortex-r4.md")
(include "cortex-r4f.md")
(include "cortex-m4.md")
(include "cortex-m4-fpu.md")
(include "vfp11.md")
(include "marvell-pj4.md")
//Pattern#40 
(define_expand "adddi3"
[(parallel [(set (match_operand:DI 0 "s_register_operand" "") (plus:DI (match_operand:DI 1 "s_register_operand" "") (match_operand:DI 2 "arm_adddi_operand" "") ) ) (clobber (reg:CC CC_REGNUM) ) ]) ]
   "TARGET_EITHER"
  "
  if (TARGET_THUMB1)
    {
      if (!REG_P (operands[1]))
        operands[1] = force_reg (DImode, operands[1]);
      if (!REG_P (operands[2]))
        operands[2] = force_reg (DImode, operands[2]);
     }
  "
)


//Pattern#41
(define_insn "*thumb1_adddi3"
[ (set (match_operand:DI 0 "register_operand" "=l") (plus:DI (match_operand:DI 1 "register_operand" "%0") (match_operand:DI 2 "register_operand" "l") ) ) (clobber (reg:CC CC_REGNUM) ) ]
  "TARGET_THUMB1"
  "add\\t%Q0, %Q0, %Q2\;adc\\t%R0, %R0, %R2"
  [(set_attr "length" "4")]
)


//Pattern#42
(define_insn_and_split "*arm_adddi3"
[ (set (match_operand:DI 0 "s_register_operand" "=&r,&r,&r,&r,&r") (plus:DI (match_operand:DI 1 "s_register_operand" "%0, 0, r, 0, r") (match_operand:DI 2 "arm_adddi_operand" "r,  0, r, Dd, Dd") ) ) (clobber (reg:CC CC_REGNUM) ) ]
	"TARGET_32BIT && !TARGET_NEON"
		"#"
		"TARGET_32BIT && reload_completed
		&& ! (TARGET_NEON && IS_VFP_REGNUM (REGNO (operands[0])))"
[  (parallel [(set (reg:CC_C CC_REGNUM) (compare:CC_C (plus:SI (match_dup 1) (match_dup 2) ) (match_dup 1) ) ) (set (match_dup 0) (plus:SI (match_dup 1) (match_dup 2) ) ) ]) (set (match_dup 3) (plus:SI (plus:SI (match_dup 4) (match_dup 5) ) (ltu:SI (reg:CC_C CC_REGNUM) (const_int 0) ) ) ) ]
	"
	{
		operands[3] = gen_highpart (SImode, operands[0]);
		operands[0] = gen_lowpart (SImode, operands[0]);
		operands[4] = gen_highpart (SImode, operands[1]);
		operands[1] = gen_lowpart (SImode, operands[1]);
		operands[5] = gen_highpart_mode (SImode, DImode, operands[2]);
		operands[2] = gen_lowpart (SImode, operands[2]);
	}"
	[(set_attr "conds" "clob")
		(set_attr "length" "8")]
)


//Pattern#43
(define_insn_and_split "*adddi_sesidi_di"
[ (set (match_operand:DI 0 "s_register_operand" "=&r,&r") (plus:DI (sign_extend:DI (match_operand:SI 2 "s_register_operand" "r,r") ) (match_operand:DI 1 "s_register_operand" "0,r") ) ) (clobber (reg:CC CC_REGNUM) ) ]
	"TARGET_32BIT"
		"#"
		"TARGET_32BIT && reload_completed"
[  (parallel [(set (reg:CC_C CC_REGNUM) (compare:CC_C (plus:SI (match_dup 1) (match_dup 2) ) (match_dup 1) ) ) (set (match_dup 0) (plus:SI (match_dup 1) (match_dup 2) ) ) ]) (set (match_dup 3) (plus:SI (plus:SI (ashiftrt:SI (match_dup 2) (const_int 31) ) (match_dup 4) ) (ltu:SI (reg:CC_C CC_REGNUM) (const_int 0) ) ) ) ]
	"
	{
		operands[3] = gen_highpart (SImode, operands[0]);
		operands[0] = gen_lowpart (SImode, operands[0]);
		operands[4] = gen_highpart (SImode, operands[1]);
		operands[1] = gen_lowpart (SImode, operands[1]);
		operands[2] = gen_lowpart (SImode, operands[2]);
	}"
	[(set_attr "conds" "clob")
	(set_attr "length" "8")]
)


//Pattern#44
(define_insn_and_split "*adddi_zesidi_di"
[ (set (match_operand:DI 0 "s_register_operand" "=&r,&r") (plus:DI (zero_extend:DI (match_operand:SI 2 "s_register_operand" "r,r") ) (match_operand:DI 1 "s_register_operand" "0,r") ) ) (clobber (reg:CC CC_REGNUM) ) ]
	"TARGET_32BIT"
		"#"
		"TARGET_32BIT && reload_completed"
[  (parallel [(set (reg:CC_C CC_REGNUM) (compare:CC_C (plus:SI (match_dup 1) (match_dup 2) ) (match_dup 1) ) ) (set (match_dup 0) (plus:SI (match_dup 1) (match_dup 2) ) ) ]) (set (match_dup 3) (plus:SI (plus:SI (match_dup 4) (const_int 0) ) (ltu:SI (reg:CC_C CC_REGNUM) (const_int 0) ) ) ) ]
	"
	{
		operands[3] = gen_highpart (SImode, operands[0]);
		operands[0] = gen_lowpart (SImode, operands[0]);
		operands[4] = gen_highpart (SImode, operands[1]);
		operands[1] = gen_lowpart (SImode, operands[1]);
		operands[2] = gen_lowpart (SImode, operands[2]);
	}"
	[(set_attr "conds" "clob")
		(set_attr "length" "8")]
)


//Pattern#45
(define_expand "addsi3"
[(set (match_operand:SI 0 "s_register_operand" "") (plus:SI (match_operand:SI 1 "s_register_operand" "") (match_operand:SI 2 "reg_or_int_operand" "") ) ) ]
  "TARGET_EITHER"
  "
  if (TARGET_32BIT && CONST_INT_P (operands[2]))
    {
      arm_split_constant (PLUS, SImode, NULL_RTX,
                          INTVAL (operands[2]), operands[0], operands[1],
                          optimize && can_create_pseudo_p ());
      DONE;
    }
  "
)


//Pattern#46
(define_peephole2 
[ (match_scratch:SI 3 "r") (set (match_operand:SI 0 "arm_general_register_operand" "") (plus:SI (match_operand:SI 1 "arm_general_register_operand" "") (match_operand:SI 2 "const_int_operand" "") ) ) ]
  "TARGET_32BIT &&
   !(const_ok_for_arm (INTVAL (operands[2]))
     || const_ok_for_arm (-INTVAL (operands[2])))
    && const_ok_for_arm (~INTVAL (operands[2]))"
[  (set (match_dup 3) (match_dup 2) ) (set (match_dup 0) (plus:SI (match_dup 1) (match_dup 3) ) ) ]
""
)


//Pattern#47
(define_insn_and_split "*arm_addsi3"
[(set (match_operand:SI 0 "s_register_operand" "=rk, r,k, r,r, k, r, k,k,r, k, r") (plus:SI (match_operand:SI 1 "s_register_operand" "%0, rk,k, r,rk,k, rk,k,r,rk,k, rk") (match_operand:SI 2 "reg_or_int_operand" "rk, rI,rI,k,Pj,Pj,L, L,L,PJ,PJ,?n") ) ) ]
  "TARGET_32BIT"
  "@
   add%?\\t%0, %0, %2
   add%?\\t%0, %1, %2
   add%?\\t%0, %1, %2
   add%?\\t%0, %2, %1
   addw%?\\t%0, %1, %2
   addw%?\\t%0, %1, %2
   sub%?\\t%0, %1, #%n2
   sub%?\\t%0, %1, #%n2
   sub%?\\t%0, %1, #%n2
   subw%?\\t%0, %1, #%n2
   subw%?\\t%0, %1, #%n2
   #"
  "TARGET_32BIT
   && CONST_INT_P (operands[2])
   && !const_ok_for_op (INTVAL (operands[2]), PLUS)
   && (reload_completed || !arm_eliminable_register (operands[1]))"
[ (clobber (const_int 0) ) ]
    "
  arm_split_constant (PLUS, SImode, curr_insn,
	              INTVAL (operands[2]), operands[0],
		      operands[1], 0);
  DONE;
  "
  [(set_attr "length" "2,4,4,4,4,4,4,4,4,4,4,16")
   (set_attr "predicable" "yes")
   (set_attr "arch" "t2,*,*,*,t2,t2,*,*,a,t2,t2,*")
   (set (attr "type") (if_then_else (match_operand 2 "const_int_operand" "")
		      (const_string "simple_alu_imm")
		      (const_string "alu_reg")))
 ]
)


//Pattern#48
(define_insn_and_split "*thumb1_addsi3"
[(set (match_operand:SI 0 "register_operand" "=l,l,l,*rk,*hk,l,k,l,l,l") (plus:SI (match_operand:SI 1 "register_operand" "%0,0,l,*0,*0,k,k,0,l,k") (match_operand:SI 2 "nonmemory_operand" "I,J,lL,*hk,*rk,M,O,Pa,Pb,Pc") ) ) ]
   "TARGET_THUMB1"
  "*
   static const char * const asms[] = 
   {
     \"add\\t%0, %0, %2\",
     \"sub\\t%0, %0, #%n2\",
     \"add\\t%0, %1, %2\",
     \"add\\t%0, %0, %2\",
     \"add\\t%0, %0, %2\",
     \"add\\t%0, %1, %2\",
     \"add\\t%0, %1, %2\",
     \"#\",
     \"#\",
     \"#\"
   };
   if ((which_alternative == 2 || which_alternative == 6)
       && CONST_INT_P (operands[2])
       && INTVAL (operands[2]) < 0)
     return \"sub\\t%0, %1, #%n2\";
   return asms[which_alternative];
  "
  "&& reload_completed && CONST_INT_P (operands[2])
   && ((operands[1] != stack_pointer_rtx
        && (INTVAL (operands[2]) > 255 || INTVAL (operands[2]) < -255))
       || (operands[1] == stack_pointer_rtx
 	   && INTVAL (operands[2]) > 1020))"
[  (set (match_dup 0) (plus:SI (match_dup 1) (match_dup 2) ) ) (set (match_dup 0) (plus:SI (match_dup 0) (match_dup 3) ) ) ]
    {
    HOST_WIDE_INT offset = INTVAL (operands[2]);
    if (operands[1] == stack_pointer_rtx)
      offset -= 1020;
    else
      {
        if (offset > 255)
	  offset = 255;
	else if (offset < -255)
	  offset = -255;
      }
    operands[3] = GEN_INT (offset);
    operands[2] = GEN_INT (INTVAL (operands[2]) - offset);
  }
  [(set_attr "length" "2,2,2,2,2,2,2,4,4,4")]
)


//Pattern#49
(define_peephole2 
[ (set (match_operand:SI 0 "arm_general_register_operand" "") (match_operand:SI 1 "const_int_operand" "") ) (set (match_dup 0) (plus:SI (match_dup 0) (reg:SI SP_REGNUM) ) ) ]
   "TARGET_THUMB1
   && (unsigned HOST_WIDE_INT) (INTVAL (operands[1])) < 1024
   && (INTVAL (operands[1]) & 3) == 0"
[ (set (match_dup 0) (plus:SI (reg:SI SP_REGNUM) (match_dup 1) ) ) ]
""
)


//Pattern#50
(define_insn "addsi3_compare0"
[ (set (reg:CC_NOOV CC_REGNUM) (compare:CC_NOOV (plus:SI (match_operand:SI 1 "s_register_operand" "r, r,r") (match_operand:SI 2 "arm_add_operand" "I,L,r") ) (const_int 0) ) ) (set (match_operand:SI 0 "s_register_operand" "=r,r,r") (plus:SI (match_dup 1) (match_dup 2) ) ) ]
"TARGET_ARM"
  "@
   add%.\\t%0, %1, %2
   sub%.\\t%0, %1, #%n2
   add%.\\t%0, %1, %2"
  [(set_attr "conds" "set")
   (set_attr "type" "simple_alu_imm, simple_alu_imm, *")]
)


//Pattern#51
(define_insn "*addsi3_compare0_scratch"
[(set (reg:CC_NOOV CC_REGNUM) (compare:CC_NOOV (plus:SI (match_operand:SI 0 "s_register_operand" "r, r, r") (match_operand:SI 1 "arm_add_operand" "I,L, r") ) (const_int 0) ) ) ]
  "TARGET_ARM"
  "@
   cmn%?\\t%0, %1
   cmp%?\\t%0, #%n1
   cmn%?\\t%0, %1"
  [(set_attr "conds" "set")
   (set_attr "predicable" "yes")
   (set_attr "type" "simple_alu_imm, simple_alu_imm, *")
   ]
)


//Pattern#52
(define_insn "*compare_negsi_si"
[(set (reg:CC_Z CC_REGNUM) (compare:CC_Z (neg:SI (match_operand:SI 0 "s_register_operand" "r") ) (match_operand:SI 1 "s_register_operand" "r") ) ) ]
  "TARGET_32BIT"
  "cmn%?\\t%1, %0"
  [(set_attr "conds" "set")
   (set_attr "predicable" "yes")]
)


//Pattern#53
(define_insn "*cmpsi2_addneg"
[ (set (reg:CC CC_REGNUM) (compare:CC (match_operand:SI 1 "s_register_operand" "r,r") (match_operand:SI 2 "arm_addimm_operand" "L,I") ) ) (set (match_operand:SI 0 "s_register_operand" "=r,r") (plus:SI (match_dup 1) (match_operand:SI 3 "arm_addimm_operand" "I,L") ) ) ]
  "TARGET_32BIT && INTVAL (operands[2]) == -INTVAL (operands[3])"
  "@
   add%.\\t%0, %1, %3
   sub%.\\t%0, %1, #%n3"
  [(set_attr "conds" "set")]
)


//Pattern#54
(define_peephole2 
[ (set (match_operand:SI 0 "arm_general_register_operand" "") (plus:SI (match_operand:SI 1 "arm_general_register_operand" "") (const_int -1) ) ) (set (match_operand 2 "cc_register" "") (compare (match_dup 0) (const_int -1) ) ) (set (pc) (if_then_else  (match_operator 3 "equality_operator" [  (match_dup 2) (const_int 0) ]) (match_operand 4 "" "") (match_operand 5 "" "") ) ) ]
	"TARGET_32BIT && peep2_reg_dead_p (3, operands[2])"
[  (parallel [(set (match_dup 2) (compare:CC (match_dup 1) (const_int 1) ) ) (set (match_dup 0) (plus:SI (match_dup 1) (const_int -1) ) ) ]) (set (pc) (if_then_else (match_op_dup  3 [(match_dup 2) (const_int 0) ]) (match_dup 4) (match_dup 5) ) ) ]
  "operands[2] = gen_rtx_REG (CCmode, CC_REGNUM);
   operands[3] = gen_rtx_fmt_ee ((GET_CODE (operands[3]) == NE
				  ? GEU : LTU),
				 VOIDmode, 
				 operands[2], const0_rtx);"
)


//Pattern#55
(define_insn "*addsi3_compare_op1"
[ (set (reg:CC_C CC_REGNUM) (compare:CC_C (plus:SI (match_operand:SI 1 "s_register_operand" "r,r,r") (match_operand:SI 2 "arm_add_operand" "I,L,r") ) (match_dup 1) ) ) (set (match_operand:SI 0 "s_register_operand" "=r,r,r") (plus:SI (match_dup 1) (match_dup 2) ) ) ]
  "TARGET_32BIT"
  "@
   add%.\\t%0, %1, %2
   sub%.\\t%0, %1, #%n2
   add%.\\t%0, %1, %2"
  [(set_attr "conds" "set")
   (set_attr "type"  "simple_alu_imm,simple_alu_imm,*")]
)


//Pattern#56
(define_insn "*addsi3_compare_op2"
[ (set (reg:CC_C CC_REGNUM) (compare:CC_C (plus:SI (match_operand:SI 1 "s_register_operand" "r,r,r") (match_operand:SI 2 "arm_add_operand" "I,L,r") ) (match_dup 2) ) ) (set (match_operand:SI 0 "s_register_operand" "=r,r,r") (plus:SI (match_dup 1) (match_dup 2) ) ) ]
  "TARGET_32BIT"
  "@
   add%.\\t%0, %1, %2
   add%.\\t%0, %1, %2
   sub%.\\t%0, %1, #%n2"
  [(set_attr "conds" "set")
   (set_attr "type" "simple_alu_imm,simple_alu_imm,*")]
)


//Pattern#57
(define_insn "*compare_addsi2_op0"
[(set (reg:CC_C CC_REGNUM) (compare:CC_C (plus:SI (match_operand:SI 0 "s_register_operand" "r,r,r") (match_operand:SI 1 "arm_add_operand" "I,L,r") ) (match_dup 0) ) ) ]
  "TARGET_32BIT"
  "@
   cmn%?\\t%0, %1
   cmp%?\\t%0, #%n1
   cmn%?\\t%0, %1"
  [(set_attr "conds" "set")
   (set_attr "predicable" "yes")
   (set_attr "type" "simple_alu_imm,simple_alu_imm,*")]
)


//Pattern#58
(define_insn "*compare_addsi2_op1"
[(set (reg:CC_C CC_REGNUM) (compare:CC_C (plus:SI (match_operand:SI 0 "s_register_operand" "r,r,r") (match_operand:SI 1 "arm_add_operand" "I,L,r") ) (match_dup 1) ) ) ]
  "TARGET_32BIT"
  "@
   cmn%?\\t%0, %1
   cmp%?\\t%0, #%n1
   cmn%?\\t%0, %1"
  [(set_attr "conds" "set")
   (set_attr "predicable" "yes")
   (set_attr "type" "simple_alu_imm,simple_alu_imm,*")]
)


//Pattern#59
(define_insn "*addsi3_carryin_<optab>"
[(set (match_operand:SI 0 "s_register_operand" "=r,r") (plus:SI (plus:SI (match_operand:SI 1 "s_register_operand" "%r,r") (match_operand:SI 2 "arm_not_operand" "rI,K") ) (LTUGEU:SI (reg:<cnb> CC_REGNUM) (const_int 0) ) ) ) ]
 "TARGET_32BIT"
  "@
   adc%?\\t%0, %1, %2
   sbc%?\\t%0, %1, #%B2"
  [(set_attr "conds" "use")]
)


//Pattern#60
(define_insn "*addsi3_carryin_alt2_<optab>"
[(set (match_operand:SI 0 "s_register_operand" "=r,r") (plus:SI (plus:SI (LTUGEU:SI (reg:<cnb> CC_REGNUM) (const_int 0) ) (match_operand:SI 1 "s_register_operand" "%r,r") ) (match_operand:SI 2 "arm_rhs_operand" "rI,K") ) ) ]
 "TARGET_32BIT"
  "@
   adc%?\\t%0, %1, %2
   sbc%?\\t%0, %1, #%B2"
  [(set_attr "conds" "use")]
)


//Pattern#61
(define_insn "*addsi3_carryin_shift_<optab>"
[(set (match_operand:SI 0 "s_register_operand" "=r") (plus:SI (plus:SI  (match_operator:SI 2 "shift_operator" [  (match_operand:SI 3 "s_register_operand" "r") (match_operand:SI 4 "reg_or_int_operand" "rM") ]) (match_operand:SI 1 "s_register_operand" "r") ) (LTUGEU:SI (reg:<cnb> CC_REGNUM) (const_int 0) ) ) ) ]
"TARGET_32BIT"
  "adc%?\\t%0, %1, %3%S2"
  [(set_attr "conds" "use")
   (set (attr "type") (if_then_else (match_operand 4 "const_int_operand" "")
                      (const_string "alu_shift")
                      (const_string "alu_shift_reg")))]
)


//Pattern#62
(define_insn "*addsi3_carryin_clobercc_<optab>"
[ (set (match_operand:SI 0 "s_register_operand" "=r") (plus:SI (plus:SI (match_operand:SI 1 "s_register_operand" "%r") (match_operand:SI 2 "arm_rhs_operand" "rI") ) (LTUGEU:SI (reg:<cnb> CC_REGNUM) (const_int 0) ) ) ) (clobber (reg:CC CC_REGNUM) ) ]
 "TARGET_32BIT"
   "adc%.\\t%0, %1, %2"
   [(set_attr "conds" "set")]
)


//Pattern#63
(define_expand "incscc"
[(set (match_operand:SI 0 "s_register_operand" "=r,r") (plus:SI  (match_operator:SI 2 "arm_comparison_operator" [  (match_operand:CC 3 "cc_register" "") (const_int 0) ]) (match_operand:SI 1 "s_register_operand" "0,?r") ) ) ]
  "TARGET_32BIT"
  ""
)


//Pattern#64
(define_insn "*arm_incscc"
[(set (match_operand:SI 0 "s_register_operand" "=r,r") (plus:SI  (match_operator:SI 2 "arm_comparison_operator" [  (match_operand:CC 3 "cc_register" "") (const_int 0) ]) (match_operand:SI 1 "s_register_operand" "0,?r") ) ) ]
  "TARGET_ARM"
  "@
  add%d2\\t%0, %1, #1
  mov%D2\\t%0, %1\;add%d2\\t%0, %1, #1"
  [(set_attr "conds" "use")
   (set_attr "length" "4,8")]  
)


//Pattern#65
(define_split 
[ (set (match_operand:SI 0 "s_register_operand" "") (plus:SI (ashift:SI (match_operand:SI 1 "const_int_operand" "") (match_operand:SI 2 "s_register_operand" "") ) (const_int -1) ) ) (clobber (match_operand:SI 3 "s_register_operand" "") ) ]
  "TARGET_32BIT"
[  (set (match_dup 3) (match_dup 1) ) (set (match_dup 0) (not:SI (ashift:SI (match_dup 3) (match_dup 2) ) ) ) ]
  "
  operands[1] = GEN_INT (~(INTVAL (operands[1]) - 1));
"
)


//Pattern#66
(define_expand "addsf3"
[(set (match_operand:SF 0 "s_register_operand" "") (plus:SF (match_operand:SF 1 "s_register_operand" "") (match_operand:SF 2 "s_register_operand" "") ) ) ]
 "TARGET_32BIT && TARGET_HARD_FLOAT"
  "
"
)


//Pattern#67
(define_expand "adddf3"
[(set (match_operand:DF 0 "s_register_operand" "") (plus:DF (match_operand:DF 1 "s_register_operand" "") (match_operand:DF 2 "s_register_operand" "") ) ) ]
  
  "TARGET_32BIT && TARGET_HARD_FLOAT && !TARGET_VFP_SINGLE"
  ""
)


//Pattern#68
(define_expand "subdi3"
[(parallel [ (set (match_operand:DI 0 "s_register_operand" "") (minus:DI (match_operand:DI 1 "s_register_operand" "") (match_operand:DI 2 "s_register_operand" "") ) ) (clobber (reg:CC CC_REGNUM) ) ]) ]
  "TARGET_EITHER"
  "
  if (TARGET_THUMB1)
    {
      if (!REG_P (operands[1]))
        operands[1] = force_reg (DImode, operands[1]);
      if (!REG_P (operands[2]))
        operands[2] = force_reg (DImode, operands[2]);
     }
  "
)


//Pattern#69
(define_insn "*arm_subdi3"
[ (set (match_operand:DI 0 "s_register_operand" "=&r,&r,&r") (minus:DI (match_operand:DI 1 "s_register_operand" "0,r,0") (match_operand:DI 2 "s_register_operand" "r,0,0") ) ) (clobber (reg:CC CC_REGNUM) ) ]
  "TARGET_32BIT && !TARGET_NEON"
  "subs\\t%Q0, %Q1, %Q2\;sbc\\t%R0, %R1, %R2"
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)


//Pattern#70
(define_insn "*thumb_subdi3"
[ (set (match_operand:DI 0 "register_operand" "=l") (minus:DI (match_operand:DI 1 "register_operand" "0") (match_operand:DI 2 "register_operand" "l") ) ) (clobber (reg:CC CC_REGNUM) ) ]
  "TARGET_THUMB1"
  "sub\\t%Q0, %Q0, %Q2\;sbc\\t%R0, %R0, %R2"
  [(set_attr "length" "4")]
)


//Pattern#71
(define_insn "*subdi_di_zesidi"
[ (set (match_operand:DI 0 "s_register_operand" "=&r,&r") (minus:DI (match_operand:DI 1 "s_register_operand" "0,r") (zero_extend:DI (match_operand:SI 2 "s_register_operand" "r,r") ) ) ) (clobber (reg:CC CC_REGNUM) ) ]
  "TARGET_32BIT"
  "subs\\t%Q0, %Q1, %2\;sbc\\t%R0, %R1, #0"
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)


//Pattern#72
(define_insn "*subdi_di_sesidi"
[ (set (match_operand:DI 0 "s_register_operand" "=&r,&r") (minus:DI (match_operand:DI 1 "s_register_operand" "0,r") (sign_extend:DI (match_operand:SI 2 "s_register_operand" "r,r") ) ) ) (clobber (reg:CC CC_REGNUM) ) ]
  "TARGET_32BIT"
  "subs\\t%Q0, %Q1, %2\;sbc\\t%R0, %R1, %2, asr #31"
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)


//Pattern#73
(define_insn "*subdi_zesidi_di"
[ (set (match_operand:DI 0 "s_register_operand" "=&r,&r") (minus:DI (zero_extend:DI (match_operand:SI 2 "s_register_operand" "r,r") ) (match_operand:DI 1 "s_register_operand" "0,r") ) ) (clobber (reg:CC CC_REGNUM) ) ]
  "TARGET_ARM"
  "rsbs\\t%Q0, %Q1, %2\;rsc\\t%R0, %R1, #0"
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)


//Pattern#74
(define_insn "*subdi_sesidi_di"
[ (set (match_operand:DI 0 "s_register_operand" "=&r,&r") (minus:DI (sign_extend:DI (match_operand:SI 2 "s_register_operand" "r,r") ) (match_operand:DI 1 "s_register_operand" "0,r") ) ) (clobber (reg:CC CC_REGNUM) ) ]
  "TARGET_ARM"
  "rsbs\\t%Q0, %Q1, %2\;rsc\\t%R0, %R1, %2, asr #31"
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)


//Pattern#75
(define_insn "*subdi_zesidi_zesidi"
[ (set (match_operand:DI 0 "s_register_operand" "=r") (minus:DI (zero_extend:DI (match_operand:SI 1 "s_register_operand" "r") ) (zero_extend:DI (match_operand:SI 2 "s_register_operand" "r") ) ) ) (clobber (reg:CC CC_REGNUM) ) ]
  "TARGET_32BIT"
  "subs\\t%Q0, %1, %2\;sbc\\t%R0, %1, %1"
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)


//Pattern#76
(define_expand "subsi3"
[(set (match_operand:SI 0 "s_register_operand" "") (minus:SI (match_operand:SI 1 "reg_or_int_operand" "") (match_operand:SI 2 "s_register_operand" "") ) ) ] 
  "TARGET_EITHER"
  "
  if (CONST_INT_P (operands[1]))
    {
      if (TARGET_32BIT)
        {
          arm_split_constant (MINUS, SImode, NULL_RTX,
	                      INTVAL (operands[1]), operands[0],
	  		      operands[2], optimize && can_create_pseudo_p ());
          DONE;
	}
      else /* TARGET_THUMB1 */
        operands[1] = force_reg (SImode, operands[1]);
    }
  "
)


//Pattern#77
(define_insn "thumb1_subsi3_insn"
[(set (match_operand:SI 0 "register_operand" "=l") (minus:SI (match_operand:SI 1 "register_operand" "l") (match_operand:SI 2 "reg_or_int_operand" "lPd") ) ) ]
  "TARGET_THUMB1"
  "sub\\t%0, %1, %2"
  [(set_attr "length" "2")
   (set_attr "conds" "set")]
)


//Pattern#78
(define_insn_and_split "*arm_subsi3_insn"
[(set (match_operand:SI 0 "s_register_operand" "=r,r,r,rk,r") (minus:SI (match_operand:SI 1 "reg_or_int_operand" "rI,r,r,k,?n") (match_operand:SI 2 "reg_or_int_operand" "r,I,r,r, r") ) ) ]
  "TARGET_32BIT"
  "@
   rsb%?\\t%0, %2, %1
   sub%?\\t%0, %1, %2
   sub%?\\t%0, %1, %2
   sub%?\\t%0, %1, %2
   #"
  "&& (CONST_INT_P (operands[1])
       && !const_ok_for_arm (INTVAL (operands[1])))"
[ (clobber (const_int 0) ) ]
  "
  arm_split_constant (MINUS, SImode, curr_insn,
                      INTVAL (operands[1]), operands[0], operands[2], 0);
  DONE;
  "
  [(set_attr "length" "4,4,4,4,16")
   (set_attr "predicable" "yes")
   (set_attr "type"  "*,simple_alu_imm,*,*,*")]
)


//Pattern#79
(define_peephole2 
[  (match_scratch:SI 3 "r") (set (match_operand:SI 0 "arm_general_register_operand" "") (minus:SI (match_operand:SI 1 "const_int_operand" "") (match_operand:SI 2 "arm_general_register_operand" "") ) ) ]
  "TARGET_32BIT
   && !const_ok_for_arm (INTVAL (operands[1]))
   && const_ok_for_arm (~INTVAL (operands[1]))"
[  (set (match_dup 3) (match_dup 1) ) (set (match_dup 0) (minus:SI (match_dup 3) (match_dup 2) ) ) ]
  ""
)


//Pattern#80
(define_insn "*subsi3_compare0"
[ (set (reg:CC_NOOV CC_REGNUM) (compare:CC_NOOV (minus:SI (match_operand:SI 1 "arm_rhs_operand" "r,r,I") (match_operand:SI 2 "arm_rhs_operand" "I,r,r") ) (const_int 0) ) ) (set (match_operand:SI 0 "s_register_operand" "=r,r,r") (minus:SI (match_dup 1) (match_dup 2) ) ) ]
  "TARGET_32BIT"
  "@
   sub%.\\t%0, %1, %2
   sub%.\\t%0, %1, %2
   rsb%.\\t%0, %2, %1"
  [(set_attr "conds" "set")
   (set_attr "type"  "simple_alu_imm,*,*")]
)


//Pattern#81
(define_insn "*subsi3_compare"
[ (set (reg:CC CC_REGNUM) (compare:CC (match_operand:SI 1 "arm_rhs_operand" "r,r,I") (match_operand:SI 2 "arm_rhs_operand" "I,r,r") ) ) (set (match_operand:SI 0 "s_register_operand" "=r,r,r") (minus:SI (match_dup 1) (match_dup 2) ) ) ]
  "TARGET_32BIT"
  "@
   sub%.\\t%0, %1, %2
   sub%.\\t%0, %1, %2
   rsb%.\\t%0, %2, %1"
  [(set_attr "conds" "set")
   (set_attr "type"  "simple_alu_imm,*,*")]
)


//Pattern#82
(define_expand "decscc"
[(set (match_operand:SI 0 "s_register_operand" "=r,r") (minus:SI (match_operand:SI 1 "s_register_operand" "0,?r") (match_operator:SI 2 "arm_comparison_operator" [ (match_operand 3 "cc_register" "") (const_int 0) ]) ) ) ]
  "TARGET_32BIT"
  ""
)


//Pattern#83
(define_insn "*arm_decscc"
[(set (match_operand:SI 0 "s_register_operand" "=r,r") (minus:SI (match_operand:SI 1 "s_register_operand" "0,?r") (match_operator:SI 2 "arm_comparison_operator" [ (match_operand 3 "cc_register" "") (const_int 0) ]) ) ) ]
  "TARGET_ARM"
  "@
   sub%d2\\t%0, %1, #1
   mov%D2\\t%0, %1\;sub%d2\\t%0, %1, #1"
  [(set_attr "conds" "use")
   (set_attr "length" "*,8")
   (set_attr "type" "simple_alu_imm,*")]
)


//Pattern#84
(define_expand "subsf3"
[(set (match_operand:SF 0 "s_register_operand" "") (minus:SF (match_operand:SF 1 "s_register_operand" "") (match_operand:SF 2 "s_register_operand" "") ) ) ]
  "TARGET_32BIT && TARGET_HARD_FLOAT"
  "
"
)


//Pattern#85
(define_expand "subdf3"
[(set (match_operand:DF 0 "s_register_operand" "") (minus:DF (match_operand:DF 1 "s_register_operand" "") (match_operand:DF 2 "s_register_operand" "") ) ) ]
  "TARGET_32BIT && TARGET_HARD_FLOAT && !TARGET_VFP_SINGLE"
  "
"
)


//Pattern#86
(define_expand "mulsi3"
[(set (match_operand:SI 0 "s_register_operand" "") (mult:SI (match_operand:SI 2 "s_register_operand" "") (match_operand:SI 1 "s_register_operand" "") ) ) ]
  "TARGET_EITHER"
  ""
)


//Pattern#87
(define_insn "*arm_mulsi3"
[(set (match_operand:SI 0 "s_register_operand" "=&r,&r") (mult:SI (match_operand:SI 2 "s_register_operand" "r,r") (match_operand:SI 1 "s_register_operand" "%0,r") ) ) ]
  "TARGET_32BIT && !arm_arch6"
  "mul%?\\t%0, %2, %1"
  [(set_attr "insn" "mul")
   (set_attr "predicable" "yes")]
)


//Pattern#88
(define_insn "*arm_mulsi3_v6"
[(set (match_operand:SI 0 "s_register_operand" "=r") (mult:SI (match_operand:SI 1 "s_register_operand" "r") (match_operand:SI 2 "s_register_operand" "r") ) ) ]
  "TARGET_32BIT && arm_arch6"
  "mul%?\\t%0, %1, %2"
  [(set_attr "insn" "mul")
   (set_attr "predicable" "yes")]
)


//Pattern#89
(define_insn "*thumb_mulsi3"
[(set (match_operand:SI 0 "register_operand" "=&l,&l,&l") (mult:SI (match_operand:SI 1 "register_operand" "%l,*h,0") (match_operand:SI 2 "register_operand" "l,l,l") ) ) ]
  "TARGET_THUMB1 && !arm_arch6"
  "*
  if (which_alternative < 2)
    return \"mov\\t%0, %1\;mul\\t%0, %2\";
  else
    return \"mul\\t%0, %2\";
  "
  [(set_attr "length" "4,4,2")
   (set_attr "insn" "mul")]
)


//Pattern#90
(define_insn "*thumb_mulsi3_v6"
[(set (match_operand:SI 0 "register_operand" "=l,l,l") (mult:SI (match_operand:SI 1 "register_operand" "0,l,0") (match_operand:SI 2 "register_operand" "l,0,0") ) ) ]
  "TARGET_THUMB1 && arm_arch6"
  "@
   mul\\t%0, %2
   mul\\t%0, %1
   mul\\t%0, %1"
  [(set_attr "length" "2")
   (set_attr "insn" "mul")]
)


//Pattern#91
(define_insn "*mulsi3_compare0"
[ (set (reg:CC_NOOV CC_REGNUM) (compare:CC_NOOV (mult:SI (match_operand:SI 2 "s_register_operand" "r,r") (match_operand:SI 1 "s_register_operand" "%0,r") ) (const_int 0) ) ) (set (match_operand:SI 0 "s_register_operand" "=&r,&r") (mult:SI (match_dup 2) (match_dup 1) ) ) ]
  "TARGET_ARM && !arm_arch6"
  "mul%.\\t%0, %2, %1"
  [(set_attr "conds" "set")
   (set_attr "insn" "muls")]
)


//Pattern#92
(define_insn "*mulsi3_compare0_v6"
[ (set (reg:CC_NOOV CC_REGNUM) (compare:CC_NOOV (mult:SI (match_operand:SI 2 "s_register_operand" "r") (match_operand:SI 1 "s_register_operand" "r") ) (const_int 0) ) ) (set (match_operand:SI 0 "s_register_operand" "=r") (mult:SI (match_dup 2) (match_dup 1) ) ) ]
  "TARGET_ARM && arm_arch6 && optimize_size"
  "mul%.\\t%0, %2, %1"
  [(set_attr "conds" "set")
   (set_attr "insn" "muls")]
)


//Pattern#93
(define_insn "*mulsi_compare0_scratch"
[ (set (reg:CC_NOOV CC_REGNUM) (compare:CC_NOOV (mult:SI (match_operand:SI 2 "s_register_operand" "r,r") (match_operand:SI 1 "s_register_operand" "%0,r") ) (const_int 0) ) ) (clobber (match_scratch:SI 0 "=&r,&r") ) ]
  "TARGET_ARM && !arm_arch6"
  "mul%.\\t%0, %2, %1"
  [(set_attr "conds" "set")
   (set_attr "insn" "muls")]
)


//Pattern#94
(define_insn "*mulsi_compare0_scratch_v6"
[ (set (reg:CC_NOOV CC_REGNUM) (compare:CC_NOOV (mult:SI (match_operand:SI 2 "s_register_operand" "r") (match_operand:SI 1 "s_register_operand" "r") ) (const_int 0) ) ) (clobber (match_scratch:SI 0 "=r") ) ]
  "TARGET_ARM && arm_arch6 && optimize_size"
  "mul%.\\t%0, %2, %1"
  [(set_attr "conds" "set")
   (set_attr "insn" "muls")]
)


//Pattern#95
(define_insn "*mulsi3addsi"
[(set (match_operand:SI 0 "s_register_operand" "=&r,&r,&r,&r") (plus:SI (mult:SI (match_operand:SI 2 "s_register_operand" "r,r,r,r") (match_operand:SI 1 "s_register_operand" "%0,r,0,r") ) (match_operand:SI 3 "s_register_operand" "r,r,0,0") ) ) ]
  "TARGET_32BIT && !arm_arch6"
  "mla%?\\t%0, %2, %1, %3"
  [(set_attr "insn" "mla")
   (set_attr "predicable" "yes")]
)


//Pattern#96
(define_insn "*mulsi3addsi_v6"
[(set (match_operand:SI 0 "s_register_operand" "=r") (plus:SI (mult:SI (match_operand:SI 2 "s_register_operand" "r") (match_operand:SI 1 "s_register_operand" "r") ) (match_operand:SI 3 "s_register_operand" "r") ) ) ]
  "TARGET_32BIT && arm_arch6"
  "mla%?\\t%0, %2, %1, %3"
  [(set_attr "insn" "mla")
   (set_attr "predicable" "yes")]
)


//Pattern#97
(define_insn "*mulsi3addsi_compare0"
[ (set (reg:CC_NOOV CC_REGNUM) (compare:CC_NOOV (plus:SI (mult:SI (match_operand:SI 2 "s_register_operand" "r,r,r,r") (match_operand:SI 1 "s_register_operand" "%0,r,0,r") ) (match_operand:SI 3 "s_register_operand" "r,r,0,0") ) (const_int 0) ) ) (set (match_operand:SI 0 "s_register_operand" "=&r,&r,&r,&r") (plus:SI (mult:SI (match_dup 2) (match_dup 1) ) (match_dup 3) ) ) ]
  "TARGET_ARM && arm_arch6"
  "mla%.\\t%0, %2, %1, %3"
  [(set_attr "conds" "set")
   (set_attr "insn" "mlas")]
)


//Pattern#98
(define_insn "*mulsi3addsi_compare0_v6"
[ (set (reg:CC_NOOV CC_REGNUM) (compare:CC_NOOV (plus:SI (mult:SI (match_operand:SI 2 "s_register_operand" "r") (match_operand:SI 1 "s_register_operand" "r") ) (match_operand:SI 3 "s_register_operand" "r") ) (const_int 0) ) ) (set (match_operand:SI 0 "s_register_operand" "=r") (plus:SI (mult:SI (match_dup 2) (match_dup 1) ) (match_dup 3) ) ) ]
  "TARGET_ARM && arm_arch6 && optimize_size"
  "mla%.\\t%0, %2, %1, %3"
  [(set_attr "conds" "set")
   (set_attr "insn" "mlas")]
)


//Pattern#99
(define_insn "*mulsi3addsi_compare0_scratch"
[ (set (reg:CC_NOOV CC_REGNUM) (compare:CC_NOOV (plus:SI (mult:SI (match_operand:SI 2 "s_register_operand" "r,r,r,r") (match_operand:SI 1 "s_register_operand" "%0,r,0,r") ) (match_operand:SI 3 "s_register_operand" "?r,r,0,0") ) (const_int 0) ) ) (clobber (match_scratch:SI 0 "=&r,&r,&r,&r") ) ]
  "TARGET_ARM && !arm_arch6"
  "mla%.\\t%0, %2, %1, %3"
  [(set_attr "conds" "set")
   (set_attr "insn" "mlas")]
)


//Pattern#100
(define_insn "*mulsi3addsi_compare0_scratch_v6"
[ (set (reg:CC_NOOV CC_REGNUM) (compare:CC_NOOV (plus:SI (mult:SI (match_operand:SI 2 "s_register_operand" "r") (match_operand:SI 1 "s_register_operand" "r") ) (match_operand:SI 3 "s_register_operand" "r") ) (const_int 0) ) ) (clobber (match_scratch:SI 0 "=r") ) ]
  "TARGET_ARM && arm_arch6 && optimize_size"
  "mla%.\\t%0, %2, %1, %3"
  [(set_attr "conds" "set")
   (set_attr "insn" "mlas")]
)


//Pattern#101
(define_insn "*mulsi3subsi"
[(set (match_operand:SI 0 "s_register_operand" "=r") (minus:SI (match_operand:SI 3 "s_register_operand" "r") (mult:SI (match_operand:SI 2 "s_register_operand" "r") (match_operand:SI 1 "s_register_operand" "r") ) ) ) ]
  "TARGET_32BIT && arm_arch_thumb2"
  "mls%?\\t%0, %2, %1, %3"
  [(set_attr "insn" "mla")
   (set_attr "predicable" "yes")]
)


//Pattern#102
(define_expand "maddsidi4"
[(set (match_operand:DI 0 "s_register_operand" "") (plus:DI (mult:DI (sign_extend:DI (match_operand:SI 1 "s_register_operand" "") ) (sign_extend:DI (match_operand:SI 2 "s_register_operand" "") ) ) (match_operand:DI 3 "s_register_operand" "") ) ) ]
  "TARGET_32BIT && arm_arch3m"
  ""
)


//Pattern#103
(define_insn "*mulsidi3adddi"
[(set (match_operand:DI 0 "s_register_operand" "=&r") (plus:DI (mult:DI (sign_extend:DI (match_operand:SI 2 "s_register_operand" "%r") ) (sign_extend:DI (match_operand:SI 3 "s_register_operand" "r") ) ) (match_operand:DI 1 "s_register_operand" "0") ) ) ]
  "TARGET_32BIT && arm_arch3m && !arm_arch6"
  "smlal%?\\t%Q0, %R0, %3, %2"
  [(set_attr "insn" "smlal")
   (set_attr "predicable" "yes")]
)


//Pattern#104
(define_insn "*mulsidi3adddi_v6"
[(set (match_operand:DI 0 "s_register_operand" "=r") (plus:DI (mult:DI (sign_extend:DI (match_operand:SI 2 "s_register_operand" "r") ) (sign_extend:DI (match_operand:SI 3 "s_register_operand" "r") ) ) (match_operand:DI 1 "s_register_operand" "0") ) ) ]
  "TARGET_32BIT && arm_arch6"
  "smlal%?\\t%Q0, %R0, %3, %2"
  [(set_attr "insn" "smlal")
   (set_attr "predicable" "yes")]
)


//Pattern#105
(define_expand "mulsidi3"
[(set (match_operand:DI 0 "s_register_operand" "") (mult:DI (sign_extend:DI (match_operand:SI 1 "s_register_operand" "") ) (sign_extend:DI (match_operand:SI 2 "s_register_operand" "") ) ) ) ]
  "TARGET_32BIT && arm_arch3m"
  ""
)


//Pattern#106
(define_insn "*mulsidi3_nov6"
[(set (match_operand:DI 0 "s_register_operand" "=&r") (mult:DI (sign_extend:DI (match_operand:SI 1 "s_register_operand" "%r") ) (sign_extend:DI (match_operand:SI 2 "s_register_operand" "r") ) ) ) ]
  "TARGET_32BIT && arm_arch3m && !arm_arch6"
  "smull%?\\t%Q0, %R0, %1, %2"
  [(set_attr "insn" "smull")
   (set_attr "predicable" "yes")]
)


//Pattern#107
(define_insn "*mulsidi3_v6"
[(set (match_operand:DI 0 "s_register_operand" "=r") (mult:DI (sign_extend:DI (match_operand:SI 1 "s_register_operand" "r") ) (sign_extend:DI (match_operand:SI 2 "s_register_operand" "r") ) ) ) ]
  "TARGET_32BIT && arm_arch6"
  "smull%?\\t%Q0, %R0, %1, %2"
  [(set_attr "insn" "smull")
   (set_attr "predicable" "yes")]
)


//Pattern#108
(define_expand "umulsidi3"
[(set (match_operand:DI 0 "s_register_operand" "") (mult:DI (zero_extend:DI (match_operand:SI 1 "s_register_operand" "") ) (zero_extend:DI (match_operand:SI 2 "s_register_operand" "") ) ) ) ]
  "TARGET_32BIT && arm_arch3m"
  ""
)


//Pattern#109
(define_insn "*umulsidi3_nov6"
[(set (match_operand:DI 0 "s_register_operand" "=&r") (mult:DI (zero_extend:DI (match_operand:SI 1 "s_register_operand" "%r") ) (zero_extend:DI (match_operand:SI 2 "s_register_operand" "r") ) ) ) ]
  "TARGET_32BIT && arm_arch3m && !arm_arch6"
  "umull%?\\t%Q0, %R0, %1, %2"
  [(set_attr "insn" "umull")
   (set_attr "predicable" "yes")]
)


//Pattern#110
(define_insn "*umulsidi3_v6"
[(set (match_operand:DI 0 "s_register_operand" "=r") (mult:DI (zero_extend:DI (match_operand:SI 1 "s_register_operand" "r") ) (zero_extend:DI (match_operand:SI 2 "s_register_operand" "r") ) ) ) ]
  "TARGET_32BIT && arm_arch6"
  "umull%?\\t%Q0, %R0, %1, %2"
  [(set_attr "insn" "umull")
   (set_attr "predicable" "yes")]
)


//Pattern#111
(define_expand "umaddsidi4"
[(set (match_operand:DI 0 "s_register_operand" "") (plus:DI (mult:DI (zero_extend:DI (match_operand:SI 1 "s_register_operand" "") ) (zero_extend:DI (match_operand:SI 2 "s_register_operand" "") ) ) (match_operand:DI 3 "s_register_operand" "") ) ) ]
  "TARGET_32BIT && arm_arch3m"
  ""
)


//Pattern#112
(define_insn "*umulsidi3adddi"
[(set (match_operand:DI 0 "s_register_operand" "=&r") (plus:DI (mult:DI (zero_extend:DI (match_operand:SI 2 "s_register_operand" "%r") ) (zero_extend:DI (match_operand:SI 3 "s_register_operand" "r") ) ) (match_operand:DI 1 "s_register_operand" "0") ) ) ]
  "TARGET_32BIT && arm_arch3m && !arm_arch6"
  "umlal%?\\t%Q0, %R0, %3, %2"
  [(set_attr "insn" "umlal")
   (set_attr "predicable" "yes")]
)


//Pattern#113
(define_insn "*umulsidi3adddi_v6"
[(set (match_operand:DI 0 "s_register_operand" "=r") (plus:DI (mult:DI (zero_extend:DI (match_operand:SI 2 "s_register_operand" "r") ) (zero_extend:DI (match_operand:SI 3 "s_register_operand" "r") ) ) (match_operand:DI 1 "s_register_operand" "0") ) ) ]
  "TARGET_32BIT && arm_arch6"
  "umlal%?\\t%Q0, %R0, %3, %2"
  [(set_attr "insn" "umlal")
   (set_attr "predicable" "yes")]
)


//Pattern#114
(define_expand "smulsi3_highpart"
[(parallel [(set (match_operand:SI 0 "s_register_operand" "") (truncate:SI (lshiftrt:DI (mult:DI (sign_extend:DI (match_operand:SI 1 "s_register_operand" "") ) (sign_extend:DI (match_operand:SI 2 "s_register_operand" "") ) ) (const_int 32) ) ) ) (clobber (match_scratch:SI 3 "") ) ]) ]
  "TARGET_32BIT && arm_arch3m"
  ""
)


//Pattern#115
(define_insn "*smulsi3_highpart_nov6"
[ (set (match_operand:SI 0 "s_register_operand" "=&r,&r") (truncate:SI (lshiftrt:DI (mult:DI (sign_extend:DI (match_operand:SI 1 "s_register_operand" "%0,r") ) (sign_extend:DI (match_operand:SI 2 "s_register_operand" "r,r") ) ) (const_int 32) ) ) ) (clobber (match_scratch:SI 3 "=&r,&r") ) ]
  "TARGET_32BIT && arm_arch3m && !arm_arch6"
  "smull%?\\t%3, %0, %2, %1"
  [(set_attr "insn" "smull")
   (set_attr "predicable" "yes")]
)


//Pattern#116
(define_insn "*smulsi3_highpart_v6"
[ (set (match_operand:SI 0 "s_register_operand" "=r") (truncate:SI (lshiftrt:DI (mult:DI (sign_extend:DI (match_operand:SI 1 "s_register_operand" "r") ) (sign_extend:DI (match_operand:SI 2 "s_register_operand" "r") ) ) (const_int 32) ) ) ) (clobber (match_scratch:SI 3 "=r") ) ]
  "TARGET_32BIT && arm_arch6"
  "smull%?\\t%3, %0, %2, %1"
  [(set_attr "insn" "smull")
   (set_attr "predicable" "yes")]
)


//Pattern#117
(define_expand "umulsi3_highpart"
[(parallel [(set (match_operand:SI 0 "s_register_operand" "") (truncate:SI (lshiftrt:DI (mult:DI (zero_extend:DI (match_operand:SI 1 "s_register_operand" "") ) (zero_extend:DI (match_operand:SI 2 "s_register_operand" "") ) ) (const_int 32) ) ) ) (clobber (match_scratch:SI 3 "") ) ]) ]
  "TARGET_32BIT && arm_arch3m"
  ""
)


//Pattern#118
(define_insn "*umulsi3_highpart_nov6"
[ (set (match_operand:SI 0 "s_register_operand" "=&r,&r") (truncate:SI (lshiftrt:DI (mult:DI (zero_extend:DI (match_operand:SI 1 "s_register_operand" "%0,r") ) (zero_extend:DI (match_operand:SI 2 "s_register_operand" "r,r") ) ) (const_int 32) ) ) ) (clobber (match_scratch:SI 3 "=&r,&r") ) ]
  "TARGET_32BIT && arm_arch3m && !arm_arch6"
  "umull%?\\t%3, %0, %2, %1"
  [(set_attr "insn" "umull")
   (set_attr "predicable" "yes")]
)


//Pattern#119
(define_insn "*umulsi3_highpart_v6"
[ (set (match_operand:SI 0 "s_register_operand" "=r") (truncate:SI (lshiftrt:DI (mult:DI (zero_extend:DI (match_operand:SI 1 "s_register_operand" "r") ) (zero_extend:DI (match_operand:SI 2 "s_register_operand" "r") ) ) (const_int 32) ) ) ) (clobber (match_scratch:SI 3 "=r") ) ]
  "TARGET_32BIT && arm_arch6"
  "umull%?\\t%3, %0, %2, %1"
  [(set_attr "insn" "umull")
   (set_attr "predicable" "yes")]
)


//Pattern#120
(define_insn "mulhisi3"
[(set (match_operand:SI 0 "s_register_operand" "=r") (mult:SI (sign_extend:SI (match_operand:HI 1 "s_register_operand" "%r") ) (sign_extend:SI (match_operand:HI 2 "s_register_operand" "r") ) ) ) ]
  "TARGET_DSP_MULTIPLY"
  "smulbb%?\\t%0, %1, %2"
  [(set_attr "insn" "smulxy")
   (set_attr "predicable" "yes")]
)


//Pattern#121
(define_insn "*mulhisi3tb"
[(set (match_operand:SI 0 "s_register_operand" "=r") (mult:SI (ashiftrt:SI (match_operand:SI 1 "s_register_operand" "r") (const_int 16) ) (sign_extend:SI (match_operand:HI 2 "s_register_operand" "r") ) ) ) ]
  "TARGET_DSP_MULTIPLY"
  "smultb%?\\t%0, %1, %2"
  [(set_attr "insn" "smulxy")
   (set_attr "predicable" "yes")]
)


//Pattern#122
(define_insn "*mulhisi3bt"
[(set (match_operand:SI 0 "s_register_operand" "=r") (mult:SI (sign_extend:SI (match_operand:HI 1 "s_register_operand" "r") ) (ashiftrt:SI (match_operand:SI 2 "s_register_operand" "r") (const_int 16) ) ) ) ]
  "TARGET_DSP_MULTIPLY"
  "smulbt%?\\t%0, %1, %2"
  [(set_attr "insn" "smulxy")
   (set_attr "predicable" "yes")]
)


//Pattern#123
(define_insn "*mulhisi3tt"
[(set (match_operand:SI 0 "s_register_operand" "=r") (mult:SI (ashiftrt:SI (match_operand:SI 1 "s_register_operand" "r") (const_int 16) ) (ashiftrt:SI (match_operand:SI 2 "s_register_operand" "r") (const_int 16) ) ) ) ]
  "TARGET_DSP_MULTIPLY"
  "smultt%?\\t%0, %1, %2"
  [(set_attr "insn" "smulxy")
   (set_attr "predicable" "yes")]
)


//Pattern#124
(define_insn "maddhisi4"
[(set (match_operand:SI 0 "s_register_operand" "=r") (plus:SI (mult:SI (sign_extend:SI (match_operand:HI 1 "s_register_operand" "r") ) (sign_extend:SI (match_operand:HI 2 "s_register_operand" "r") ) ) (match_operand:SI 3 "s_register_operand" "r") ) ) ]
  "TARGET_DSP_MULTIPLY"
  "smlabb%?\\t%0, %1, %2, %3"
  [(set_attr "insn" "smlaxy")
   (set_attr "predicable" "yes")]
)


//Pattern#125
(define_insn "*maddhisi4tb"
[(set (match_operand:SI 0 "s_register_operand" "=r") (plus:SI (mult:SI (ashiftrt:SI (match_operand:SI 1 "s_register_operand" "r") (const_int 16) ) (sign_extend:SI (match_operand:HI 2 "s_register_operand" "r") ) ) (match_operand:SI 3 "s_register_operand" "r") ) ) ]
  "TARGET_DSP_MULTIPLY"
  "smlatb%?\\t%0, %1, %2, %3"
  [(set_attr "insn" "smlaxy")
   (set_attr "predicable" "yes")]
)


//Pattern#126
(define_insn "*maddhisi4tt"
[(set (match_operand:SI 0 "s_register_operand" "=r") (plus:SI (mult:SI (ashiftrt:SI (match_operand:SI 1 "s_register_operand" "r") (const_int 16) ) (ashiftrt:SI (match_operand:SI 2 "s_register_operand" "r") (const_int 16) ) ) (match_operand:SI 3 "s_register_operand" "r") ) ) ]
  "TARGET_DSP_MULTIPLY"
  "smlatt%?\\t%0, %1, %2, %3"
  [(set_attr "insn" "smlaxy")
   (set_attr "predicable" "yes")]
)


//Pattern#127
(define_insn "maddhidi4"
[(set (match_operand:DI 0 "s_register_operand" "=r") (plus:DI (mult:DI (sign_extend:DI (match_operand:HI 1 "s_register_operand" "r") ) (sign_extend:DI (match_operand:HI 2 "s_register_operand" "r") ) ) (match_operand:DI 3 "s_register_operand" "0") ) ) ]
  "TARGET_DSP_MULTIPLY"
  "smlalbb%?\\t%Q0, %R0, %1, %2"
  [(set_attr "insn" "smlalxy")
   (set_attr "predicable" "yes")]
)


//Pattern#128
(define_insn "*maddhidi4tb"
[(set (match_operand:DI 0 "s_register_operand" "=r") (plus:DI (mult:DI (sign_extend:DI (ashiftrt:SI (match_operand:SI 1 "s_register_operand" "r") (const_int 16) ) ) (sign_extend:DI (match_operand:HI 2 "s_register_operand" "r") ) ) (match_operand:DI 3 "s_register_operand" "0") ) ) ]
  "TARGET_DSP_MULTIPLY"
  "smlaltb%?\\t%Q0, %R0, %1, %2"
  [(set_attr "insn" "smlalxy")
   (set_attr "predicable" "yes")]
)


//Pattern#129
(define_insn "*maddhidi4tt"
[(set (match_operand:DI 0 "s_register_operand" "=r") (plus:DI (mult:DI (sign_extend:DI (ashiftrt:SI (match_operand:SI 1 "s_register_operand" "r") (const_int 16) ) ) (sign_extend:DI (ashiftrt:SI (match_operand:SI 2 "s_register_operand" "r") (const_int 16) ) ) ) (match_operand:DI 3 "s_register_operand" "0") ) ) ]
  "TARGET_DSP_MULTIPLY"
  "smlaltt%?\\t%Q0, %R0, %1, %2"
  [(set_attr "insn" "smlalxy")
   (set_attr "predicable" "yes")]
)


//Pattern#130
(define_expand "mulsf3"
[(set (match_operand:SF 0 "s_register_operand" "") (mult:SF (match_operand:SF 1 "s_register_operand" "") (match_operand:SF 2 "s_register_operand" "") ) ) ]
  "TARGET_32BIT && TARGET_HARD_FLOAT"
  ""
)


//Pattern#131
(define_expand "muldf3"
[(set (match_operand:DF 0 "s_register_operand" "") (mult:DF (match_operand:DF 1 "s_register_operand" "") (match_operand:DF 2 "s_register_operand" "") ) ) ]
  "TARGET_32BIT && TARGET_HARD_FLOAT && !TARGET_VFP_SINGLE"
  ""
)


//Pattern#132
(define_expand "divsf3"
[(set (match_operand:SF 0 "s_register_operand" "") (div:SF (match_operand:SF 1 "s_register_operand" "") (match_operand:SF 2 "s_register_operand" "") ) ) ]
  "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP"
  ""
)


//Pattern#133
(define_expand "divdf3"
[(set (match_operand:DF 0 "s_register_operand" "") (div:DF (match_operand:DF 1 "s_register_operand" "") (match_operand:DF 2 "s_register_operand" "") ) ) ]
  "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP_DOUBLE"
  ""
)


//Pattern#243
(define_insn "divsi3"
[(set (match_operand:SI 0 "s_register_operand" "=r") (div:SI (match_operand:SI 1 "s_register_operand" "r") (match_operand:SI 2 "s_register_operand" "r") ) ) ]
  "TARGET_IDIV"
  "sdiv%?\t%0, %1, %2"
  [(set_attr "predicable" "yes")
   (set_attr "insn" "sdiv")]
)


//Pattern#244
(define_insn "udivsi3"
[(set (match_operand:SI 0 "s_register_operand" "=r") (udiv:SI (match_operand:SI 1 "s_register_operand" "r") (match_operand:SI 2 "s_register_operand" "r") ) ) ]
  "TARGET_IDIV"
  "udiv%?\t%0, %1, %2"
  [(set_attr "predicable" "yes")
   (set_attr "insn" "udiv")]
)


//Pattern#245
(define_expand "negdi2"
[(parallel [(set (match_operand:DI 0 "s_register_operand" "") (neg:DI (match_operand:DI 1 "s_register_operand" "") ) ) (clobber (reg:CC CC_REGNUM) ) ]) ]
 "TARGET_EITHER"
  {
    if (TARGET_NEON)
      {
        emit_insn (gen_negdi2_neon (operands[0], operands[1]));
        DONE;
      }
  }
)


//Pattern#246
(define_insn "*arm_negdi2"
[ (set (match_operand:DI 0 "s_register_operand" "=r,&r") (neg:DI (match_operand:DI 1 "s_register_operand" "0,r") ) ) (clobber (reg:CC CC_REGNUM) ) ]
 "TARGET_ARM"
  "rsbs\\t%Q0, %Q1, #0\;rsc\\t%R0, %R1, #0"
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)


//Pattern#247
(define_insn "*thumb1_negdi2"
[ (set (match_operand:DI 0 "register_operand" "=&l") (neg:DI (match_operand:DI 1 "register_operand" "l") ) ) (clobber (reg:CC CC_REGNUM) ) ]
 "TARGET_THUMB1"
  "mov\\t%R0, #0\;neg\\t%Q0, %Q1\;sbc\\t%R0, %R1"
  [(set_attr "length" "6")]
)


//Pattern#248
(define_expand "negsi2"
[(set (match_operand:SI 0 "s_register_operand" "") (neg:SI (match_operand:SI 1 "s_register_operand" "") ) ) ]
 "TARGET_EITHER"
  ""
)


//Pattern#249
(define_insn "*arm_negsi2"
[(set (match_operand:SI 0 "s_register_operand" "=r") (neg:SI (match_operand:SI 1 "s_register_operand" "r") ) ) ]
 "TARGET_32BIT"
  "rsb%?\\t%0, %1, #0"
  [(set_attr "predicable" "yes")]
)


//Pattern#250
(define_insn "*thumb1_negsi2"
[(set (match_operand:SI 0 "register_operand" "=l") (neg:SI (match_operand:SI 1 "register_operand" "l") ) ) ]
 "TARGET_THUMB1"
  "neg\\t%0, %1"
  [(set_attr "length" "2")]
)


//Pattern#251
(define_expand "negsf2"
[(set (match_operand:SF 0 "s_register_operand" "") (neg:SF (match_operand:SF 1 "s_register_operand" "") ) ) ]
 "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP"
  ""
)


//Pattern#252
(define_expand "negdf2"
[(set (match_operand:DF 0 "s_register_operand" "") (neg:DF (match_operand:DF 1 "s_register_operand" "") ) ) ]
"TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP_DOUBLE"
  ""
)


//Pattern#253
(define_expand "abssi2"
[(parallel [(set (match_operand:SI 0 "s_register_operand" "") (abs:SI (match_operand:SI 1 "s_register_operand" "") ) ) (clobber (match_dup 2) ) ]) ]
 "TARGET_EITHER"
  "
  if (TARGET_THUMB1)
    operands[2] = gen_rtx_SCRATCH (SImode);
  else
    operands[2] = gen_rtx_REG (CCmode, CC_REGNUM);
"
)


//Pattern#254
(define_insn "*arm_abssi2"
[ (set (match_operand:SI 0 "s_register_operand" "=r,&r") (abs:SI (match_operand:SI 1 "s_register_operand" "0,r") ) ) (clobber (reg:CC CC_REGNUM) ) ]
 "TARGET_ARM"
  "@
   cmp\\t%0, #0\;rsblt\\t%0, %0, #0
   eor%?\\t%0, %1, %1, asr #31\;sub%?\\t%0, %0, %1, asr #31"
  [(set_attr "conds" "clob,*")
   (set_attr "shift" "1")
   (set_attr "predicable" "no, yes")
   (set_attr "length" "8")]
)


//Pattern#255
(define_insn_and_split "*thumb1_abssi2"
[ (set (match_operand:SI 0 "s_register_operand" "=l") (abs:SI (match_operand:SI 1 "s_register_operand" "l") ) ) (clobber (match_scratch:SI 2 "=&l") ) ]
"TARGET_THUMB1"
  "#"
  "TARGET_THUMB1 && reload_completed"
[  (set (match_dup 2) (ashiftrt:SI (match_dup 1) (const_int 31) ) ) (set (match_dup 0) (plus:SI (match_dup 1) (match_dup 2) ) ) (set (match_dup 0) (xor:SI (match_dup 0) (match_dup 2) ) ) ]
""
  [(set_attr "length" "6")]
)


//Pattern#256
(define_insn "*arm_neg_abssi2"
[ (set (match_operand:SI 0 "s_register_operand" "=r,&r") (neg:SI (abs:SI (match_operand:SI 1 "s_register_operand" "0,r") ) ) ) (clobber (reg:CC CC_REGNUM) ) ]
 "TARGET_ARM"
  "@
   cmp\\t%0, #0\;rsbgt\\t%0, %0, #0
   eor%?\\t%0, %1, %1, asr #31\;rsb%?\\t%0, %0, %1, asr #31"
  [(set_attr "conds" "clob,*")
   (set_attr "shift" "1")
   (set_attr "predicable" "no, yes")
   (set_attr "length" "8")]
)


//Pattern#257
(define_insn_and_split "*thumb1_neg_abssi2"
[ (set (match_operand:SI 0 "s_register_operand" "=l") (neg:SI (abs:SI (match_operand:SI 1 "s_register_operand" "l") ) ) ) (clobber (match_scratch:SI 2 "=&l") ) ]
"TARGET_THUMB1"
  "#"
  "TARGET_THUMB1 && reload_completed"
[  (set (match_dup 2) (ashiftrt:SI (match_dup 1) (const_int 31) ) ) (set (match_dup 0) (minus:SI (match_dup 2) (match_dup 1) ) ) (set (match_dup 0) (xor:SI (match_dup 0) (match_dup 2) ) ) ]
 ""
  [(set_attr "length" "6")]
)


//Pattern#258
(define_expand "abssf2"
[(set (match_operand:SF 0 "s_register_operand" "") (abs:SF (match_operand:SF 1 "s_register_operand" "") ) ) ]
 "TARGET_32BIT && TARGET_HARD_FLOAT"
  ""
)


//Pattern#259
(define_expand "absdf2"
[(set (match_operand:DF 0 "s_register_operand" "") (abs:DF (match_operand:DF 1 "s_register_operand" "") ) ) ]
 "TARGET_32BIT && TARGET_HARD_FLOAT && !TARGET_VFP_SINGLE"
  ""
)


//Pattern#260
(define_expand "sqrtsf2"
[(set (match_operand:SF 0 "s_register_operand" "") (sqrt:SF (match_operand:SF 1 "s_register_operand" "") ) ) ]
 "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP"
  ""
)


//Pattern#261
(define_expand "sqrtdf2"
[(set (match_operand:DF 0 "s_register_operand" "") (sqrt:DF (match_operand:DF 1 "s_register_operand" "") ) ) ]
 "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP_DOUBLE"
  ""
)


//Pattern#262
(define_insn_and_split "one_cmpldi2"
[(set (match_operand:DI 0 "s_register_operand" "=w,&r,&r,?w") (not:DI (match_operand:DI 1 "s_register_operand" " w, 0, r, w") ) ) ]
 "TARGET_32BIT"
  "@
   vmvn\t%P0, %P1
   #
   #
   vmvn\t%P0, %P1"
  "TARGET_32BIT && reload_completed
   && arm_general_register_operand (operands[0], DImode)"
[  (set (match_dup 0) (not:SI (match_dup 1) ) ) (set (match_dup 2) (not:SI (match_dup 3) ) ) ]
 "
  {
    operands[2] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[3] = gen_highpart (SImode, operands[1]);
    operands[1] = gen_lowpart (SImode, operands[1]);
  }"
  [(set_attr "length" "*,8,8,*")
   (set_attr "predicable" "no,yes,yes,no")
   (set_attr "neon_type" "neon_int_1,*,*,neon_int_1")
   (set_attr "arch" "neon_nota8,*,*,neon_onlya8")]
)


//Pattern#263
(define_expand "one_cmplsi2"
[(set (match_operand:SI 0 "s_register_operand" "") (not:SI (match_operand:SI 1 "s_register_operand" "") ) ) ]
 "TARGET_EITHER"
  ""
)


//Pattern#264
(define_insn "*arm_one_cmplsi2"
[(set (match_operand:SI 0 "s_register_operand" "=r") (not:SI (match_operand:SI 1 "s_register_operand" "r") ) ) ]
"TARGET_32BIT"
  "mvn%?\\t%0, %1"
  [(set_attr "predicable" "yes")
   (set_attr "insn" "mvn")]
)


//Pattern#265
(define_insn "*thumb1_one_cmplsi2"
[(set (match_operand:SI 0 "register_operand" "=l") (not:SI (match_operand:SI 1 "register_operand" "l") ) ) ]
 "TARGET_THUMB1"
  "mvn\\t%0, %1"
  [(set_attr "length" "2")
   (set_attr "insn" "mvn")]
)


//Pattern#266
(define_insn "*notsi_compare0"
[ (set (reg:CC_NOOV CC_REGNUM) (compare:CC_NOOV (not:SI (match_operand:SI 1 "s_register_operand" "r") ) (const_int 0) ) ) (set (match_operand:SI 0 "s_register_operand" "=r") (not:SI (match_dup 1) ) ) ]
 "TARGET_32BIT"
  "mvn%.\\t%0, %1"
  [(set_attr "conds" "set")
   (set_attr "insn" "mvn")]
)


//Pattern#267
(define_insn "*notsi_compare0_scratch"
[ (set (reg:CC_NOOV CC_REGNUM) (compare:CC_NOOV (not:SI (match_operand:SI 1 "s_register_operand" "r") ) (const_int 0) ) ) (clobber (match_scratch:SI 0 "=r") ) ]
 "TARGET_32BIT"
  "mvn%.\\t%0, %1"
  [(set_attr "conds" "set")
   (set_attr "insn" "mvn")]
)


//Pattern#276
(define_expand "truncdfsf2"
[(set (match_operand:SF 0 "s_register_operand" "") (float_truncate:SF (match_operand:DF 1 "s_register_operand" "") ) ) ]
 "TARGET_32BIT && TARGET_HARD_FLOAT && !TARGET_VFP_SINGLE"
  ""
)


//Pattern#277
(define_expand "truncdfhf2"
[(set (match_operand:HF 0 "general_operand" "") (float_truncate:HF (match_operand:DF 1 "general_operand" "") ) ) ]
   "TARGET_EITHER"
  "
  {
    rtx op1;
    op1 = convert_to_mode (SFmode, operands[1], 0);
    op1 = convert_to_mode (HFmode, op1, 0);
    emit_move_insn (operands[0], op1);
    DONE;
  }"
)


//Pattern#39
