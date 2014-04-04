
; Register numbers -- All machine registers should be defined here
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

; 3rd operand to select_dominance_cc_mode
//Pattern#3
(define_constants [  (DOM_CC_X_AND_Y	0)
 (DOM_CC_NX_OR_Y	1)
 (DOM_CC_X_OR_Y	2)
                    ])

; conditional compare combination
//Pattern#4
(define_constants [  (CMP_CMP	0)
 (CMN_CMP	1)
 (CMP_CMN	2)
 (CMN_CMN	3)
 (NUM_OF_COND_CMP	4)
                    ])

; IS_THUMB is set to 'yes' when we are generating Thumb code, and 'no' when
; generating ARM code.  This is used to control the length of some insn
; patterns that share the same RTL in both ARM and Thumb code.
//Pattern#5
(define_attr "is_thumb"
  "no,yes"
	(const (symbol_ref "thumb_code") ) )


; IS_THUMB is set to 'yes' when we are generating Thumb code, and 'no' when
; generating ARM code.  This is used to control the length of some insn
; patterns that share the same RTL in both ARM and Thumb code.
//Pattern#6
(define_attr "is_arch6"
  "no,yes"
	(const (symbol_ref "arm_arch6") ) )


; IS_THUMB is set to 'yes' when we are generating Thumb code, and 'no' when
; generating ARM code.  This is used to control the length of some insn
; patterns that share the same RTL in both ARM and Thumb code.
//Pattern#7
(define_attr "is_thumb1"
  "no,yes"
	(const (symbol_ref "thumb1_code") ) )


; IS_THUMB is set to 'yes' when we are generating Thumb code, and 'no' when
; generating ARM code.  This is used to control the length of some insn
; patterns that share the same RTL in both ARM and Thumb code.
//Pattern#8
(define_attr "shift"
  ""
	 (const_int 0) )


; IS_THUMB is set to 'yes' when we are generating Thumb code, and 'no' when
; generating ARM code.  This is used to control the length of some insn
; patterns that share the same RTL in both ARM and Thumb code.
//Pattern#9
(define_attr "fpu"
  "none,vfp"
	(const (symbol_ref "arm_fpu_attr") ) )


; IS_THUMB is set to 'yes' when we are generating Thumb code, and 'no' when
; generating ARM code.  This is used to control the length of some insn
; patterns that share the same RTL in both ARM and Thumb code.
//Pattern#10
(define_attr "length"
  ""
	 (const_int 4) )


; IS_THUMB is set to 'yes' when we are generating Thumb code, and 'no' when
; generating ARM code.  This is used to control the length of some insn
; patterns that share the same RTL in both ARM and Thumb code.
//Pattern#11
(define_attr "arch"
  "any,a,t,32,t1,t2,v6,nov6,onlya8,neon_onlya8,nota8,neon_nota8,iwmmxt,iwmmxt2"
	 (const_string "any") )


; IS_THUMB is set to 'yes' when we are generating Thumb code, and 'no' when
; generating ARM code.  This is used to control the length of some insn
; patterns that share the same RTL in both ARM and Thumb code.
//Pattern#12 TODO
(define_attr "arch_enabled"
  ""
	 (const_int 4) )


; IS_THUMB is set to 'yes' when we are generating Thumb code, and 'no' when
; generating ARM code.  This is used to control the length of some insn
; patterns that share the same RTL in both ARM and Thumb code.
//Pattern#13
(define_attr "opt"
  "any,speed,size"
	 (const_string "any") )


; IS_THUMB is set to 'yes' when we are generating Thumb code, and 'no' when
; generating ARM code.  This is used to control the length of some insn
; patterns that share the same RTL in both ARM and Thumb code.
//Pattern#14 TODO
(define_attr "opt"
  "any,speed,size"
	 (const_string "any") )


; IS_THUMB is set to 'yes' when we are generating Thumb code, and 'no' when
; generating ARM code.  This is used to control the length of some insn
; patterns that share the same RTL in both ARM and Thumb code.
//Pattern#15
(define_attr "insn_enabled"
  "no,yes"
	 (const_string "yes") )


; IS_THUMB is set to 'yes' when we are generating Thumb code, and 'no' when
; generating ARM code.  This is used to control the length of some insn
; patterns that share the same RTL in both ARM and Thumb code.
//Pattern#16 TODO
(define_attr "opt"
  "any,speed,size"
	 (const_string "any") )


; POOL_RANGE is how far away from a constant pool entry that this insn
; can be placed.  If the distance is zero, then this insn will never
; reference the pool.
; Note that for Thumb constant pools the PC value is rounded down to the
; nearest multiple of four.  Therefore, THUMB2_POOL_RANGE (and POOL_RANGE for
; Thumb insns) should be set to <max_range> - 2.
; NEG_POOL_RANGE is nonzero for insns that can reference a constant pool entry
; before its address.  It is set to <max_range> - (8 + <data_size>).
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


//Pattern#21 TODO
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#22 TODO
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


//Pattern#23 TODO
(define_attr "arm_pool_range"
  ""
	 (const_int 0) )


;; The instruction used to implement a particular pattern.  This
;; information is used by pipeline descriptions to provide accurate
;; scheduling information.
//Pattern#24 TODO
(define_attr "INSN"
  "mov,mvn,smulxy,smlaxy,smlalxy,smulwy,smlawx,mul,muls,mla,mlas,umull,umulls,umlal,umlals,smull,smulls,smlal,smlals,smlawy,smuad,smuadx,smlad,smladx,smusd,smusdx,smlsd,smlsdx,smmul,smmulr,smmla,umaal,smlald,smlsld,clz,mrs,msr,xtab,sdiv,udiv,sat,other"
	 (const_string "other") )


//Pattern#25 TODO

//Pattern#26 TODO

; wtype for WMMX insn scheduling purposes.
//Pattern#27
(define_attr "wtype"
  "none,wor,wxor,wand,wandn,wmov,tmcrr,tmrrc,wldr,wstr,tmcr,tmrc,wadd,wsub,wmul,wmac,wavg2,tinsr,textrm,wshufh,wcmpeq,wcmpgt,wmax,wmin,wpack,wunpckih,wunpckil,wunpckeh,wunpckel,wror,wsra,wsrl,wsll,wmadd,tmia,tmiaph,tmiaxy,tbcst,tmovmsk,wacc,waligni,walignr,tandc,textrc,torc,torvsc,wsad,wabs,wabsdiff,waddsubhx,wsubaddhx,wavg4,wmulw,wqmulm,wqmulwm,waddbhus,wqmiaxy,wmiaxy,wmiawxy,wmerge"
	 (const_string "none") )


; Load scheduling, set from the arm_ld_sched variable
; initialized by arm_option_override()
//Pattern#28
(define_attr "ldsched"
  "no,yes"
	(const (symbol_ref "arm_ld_sched") ) )


;; Classification of NEON instructions for scheduling purposes.
//Pattern#29
(define_attr "wtype"
  "neon_int_1,neon_int_2,neon_int_3,neon_int_4,neon_int_5,neon_vqneg_vqabs,neon_vmov,neon_vaba,neon_vsma,neon_vaba_qqq,neon_mul_ddd_8_16_qdd_16_8_long_32_16_long,neon_mul_qqq_8_16_32_ddd_32,neon_mul_qdd_64_32_long_qqd_16_ddd_32_scalar_64_32_long_scalar,neon_mla_ddd_8_16_qdd_16_8_long_32_16_long,neon_mla_qqq_8_16,neon_mla_ddd_32_qqd_16_ddd_32_scalar_qdd_64_32_long_scalar_qdd_64_32_long,neon_mla_qqq_32_qqd_32_scalar,neon_mul_ddd_16_scalar_32_16_long_scalar,neon_mul_qqd_32_scalar,neon_mla_ddd_16_scalar_qdd_32_16_long_scalar,neon_shift_1,neon_shift_2,neon_shift_3,neon_vshl_ddd,neon_vqshl_vrshl_vqrshl_qqq,neon_vsra_vrsra,neon_fp_vadd_ddd_vabs_dd,neon_fp_vadd_qqq_vabs_qq,neon_fp_vsum,neon_fp_vmul_ddd,neon_fp_vmul_qqd,neon_fp_vmla_ddd,neon_fp_vmla_qqq,neon_fp_vmla_ddd_scalar,neon_fp_vmla_qqq_scalar,neon_fp_vrecps_vrsqrts_ddd,neon_fp_vrecps_vrsqrts_qqq,neon_bp_simple,neon_bp_2cycle,neon_bp_3cycle,neon_ldr,neon_str,neon_vld1_1_2_regs,neon_vld1_3_4_regs,neon_vld2_2_regs_vld1_vld2_all_lanes,neon_vld2_4_regs,neon_vld3_vld4,neon_vst1_1_2_regs_vst2_2_regs,neon_vst1_3_4_regs,neon_vst2_4_regs_vst3_vst4,neon_vst3_vst4,neon_vld1_vld2_lane,neon_vld3_vld4_lane,neon_vst1_vst2_lane,neon_vst3_vst4_lane,neon_vld3_vld4_all_lanes,\
   neon_mcr,neon_mcr_2_mcrr,neon_mrc,neon_mrrc,neon_ldm_2,neon_stm_2,none"
	 (const_string "none") )


//Pattern#30 TODO

; Predicable means that the insn can be conditionally executed based on
; an automatically added predicate (additional patterns are generated by 
; gen...).  We default to 'no' because no Thumb patterns match this rule
; and not all ARM patterns do.
//Pattern#31
(define_attr "predicable"
  "no,yes"
	 (const_string "no") )


; Only model the write buffer for ARM6 and ARM7.  Earlier processors don't
; have one.  Later ones, such as StrongARM, have write-back caches, so don't
; suffer blockages enough to warrant modelling this (and it can adversely
; affect the schedule).
//Pattern#32
(define_attr "model_wbuf"
  "no,yes"
	(const (symbol_ref "arm_tune_wbuf") ) )


; WRITE_CONFLICT implies that a read following an unrelated write is likely
; to stall the processor.  Used with model_wbuf above.
//Pattern#33 TODO

; Classify the insns into those that take one cycle and those that take more
; than one on the main cpu execution unit.
//Pattern#34 TODO

;; FAR_JUMP is "yes" if a BL instruction is used to generate a branch to a
;; distant label.  Only applicable to Thumb code.
//Pattern#35
(define_attr "far_jump"
  ""
	 (const_int 1) )


;; The number of machine instructions this pattern expands to.
;; Used for Thumb-2 conditional execution.
//Pattern#36
(define_attr "ce_count"
  ""
	 (const_int 1) )


;;---------------------------------------------------------------------------
;; Pipeline descriptions
//Pattern#37 TODO

;; True if the generic scheduling description should be used.
//Pattern#38 TODO

//Pattern#39 TODO
