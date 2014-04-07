
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
(define_attr "arch_enabled"
  ""
	 (const_int 4) )


//Pattern#13
(define_attr "opt"
  "any,speed,size"
	 (const_string "any") )


//Pattern#14
(define_attr "opt_enabled"
  "any,speed,size"
	 (const_string "any") )


//Pattern#15
(define_attr "insn_enabled"
  "no,yes"
	 (const_string "yes") )


//Pattern#16
(define_attr "enabled"
  "any,speed,size"
	 (const_string "any") )


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
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#22
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#23 
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#24 
(define_attr "INSN"
  "mov,mvn,smulxy,smlaxy,smlalxy,smulwy,smlawx,mul,muls,mla,mlas,umull,umulls,umlal,umlals,smull,smulls,smlal,smlals,smlawy,smuad,smuadx,smlad,smladx,smusd,smusdx,smlsd,smlsdx,smmul,smmulr,smmla,umaal,smlald,smlsld,clz,mrs,msr,xtab,sdiv,udiv,sat,other"
	 (const_string "other") )


//Pattern#25 
(define_attr "type"
  ""
	 (const_int 0) )


//Pattern#26 
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#27
(define_attr "wtype"
  "none,wor,wxor,wand,wandn,wmov,tmcrr,tmrrc,wldr,wstr,tmcr,tmrc,wadd,wsub,wmul,wmac,wavg2,tinsr,textrm,wshufh,wcmpeq,wcmpgt,wmax,wmin,wpack,wunpckih,wunpckil,wunpckeh,wunpckel,wror,wsra,wsrl,wsll,wmadd,tmia,tmiaph,tmiaxy,tbcst,tmovmsk,wacc,waligni,walignr,tandc,textrc,torc,torvsc,wsad,wabs,wabsdiff,waddsubhx,wsubaddhx,wavg4,wmulw,wqmulm,wqmulwm,waddbhus,wqmiaxy,wmiaxy,wmiawxy,wmerge"
	 (const_string "none") )


//Pattern#28
(define_attr "ldsched"
  "no,yes"
	(const (symbol_ref "arm_ld_sched") ) )


//Pattern#29
(define_attr "wtype"
  "neon_int_1,neon_int_2,neon_int_3,neon_int_4,neon_int_5,neon_vqneg_vqabs,neon_vmov,neon_vaba,neon_vsma,neon_vaba_qqq,neon_mul_ddd_8_16_qdd_16_8_long_32_16_long,neon_mul_qqq_8_16_32_ddd_32,neon_mul_qdd_64_32_long_qqd_16_ddd_32_scalar_64_32_long_scalar,neon_mla_ddd_8_16_qdd_16_8_long_32_16_long,neon_mla_qqq_8_16,neon_mla_ddd_32_qqd_16_ddd_32_scalar_qdd_64_32_long_scalar_qdd_64_32_long,neon_mla_qqq_32_qqd_32_scalar,neon_mul_ddd_16_scalar_32_16_long_scalar,neon_mul_qqd_32_scalar,neon_mla_ddd_16_scalar_qdd_32_16_long_scalar,neon_shift_1,neon_shift_2,neon_shift_3,neon_vshl_ddd,neon_vqshl_vrshl_vqrshl_qqq,neon_vsra_vrsra,neon_fp_vadd_ddd_vabs_dd,neon_fp_vadd_qqq_vabs_qq,neon_fp_vsum,neon_fp_vmul_ddd,neon_fp_vmul_qqd,neon_fp_vmla_ddd,neon_fp_vmla_qqq,neon_fp_vmla_ddd_scalar,neon_fp_vmla_qqq_scalar,neon_fp_vrecps_vrsqrts_ddd,neon_fp_vrecps_vrsqrts_qqq,neon_bp_simple,neon_bp_2cycle,neon_bp_3cycle,neon_ldr,neon_str,neon_vld1_1_2_regs,neon_vld1_3_4_regs,neon_vld2_2_regs_vld1_vld2_all_lanes,neon_vld2_4_regs,neon_vld3_vld4,neon_vst1_1_2_regs_vst2_2_regs,neon_vst1_3_4_regs,neon_vst2_4_regs_vst3_vst4,neon_vst3_vst4,neon_vld1_vld2_lane,neon_vld3_vld4_lane,neon_vst1_vst2_lane,neon_vst3_vst4_lane,neon_vld3_vld4_all_lanes,neon_mcr,neon_mcr_2_mcrr,neon_mrc,neon_mrrc,neon_ldm_2,neon_stm_2,none"
	 (const_string "none") )


//Pattern#30
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#31
(define_attr "predicable"
  "no,yes"
	 (const_string "no") )


//Pattern#32
(define_attr "model_wbuf"
  "no,yes"
	(const (symbol_ref "arm_tune_wbuf") ) )


//Pattern#33 
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#34 
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#35
(define_attr "far_jump"
  ""
	 (const_int 1) )


//Pattern#36
(define_attr "ce_count"
  ""
	 (const_int 1) )


//Pattern#37 
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#38 
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#39 
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#40 
(define_insn "adddi3"
[(parallel [(set (match_operand:DI 0 "s_register_operand" "") (plus (match_operand:DI 1 "s_register_operand" "") (match_operand:DI 2 "arm_adddi_operand" "") ) ) (clobber (reg:CC CC_REGNUM) ) ]) ]
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
(define_insn "*arm_adddi3"
[ (set (match_operand:DI 0 "s_register_operand" "=&r,&r,&r,&r,&r") (plus (match_operand:DI 1 "s_register_operand" "%0, 0, r, 0, r") (match_operand:DI 2 "arm_adddi_operand" "r,  0, r, Dd, Dd") ) ) (clobber (reg:CC CC_REGNUM) ) ]
  "TARGET_32BIT && !TARGET_NEON"
  "#"
  "TARGET_32BIT && reload_completed
   && ! (TARGET_NEON && IS_VFP_REGNUM (REGNO (operands[0])))"
)


//Pattern#43
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#44
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#45
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#46
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#47
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#48
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#49
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#50
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#51
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#52
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#53
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#54
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#55
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#56
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#57
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#58
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#59
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#60
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#61
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#62
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#63
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#64
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#65
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#66
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#67
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


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
