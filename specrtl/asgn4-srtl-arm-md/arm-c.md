;;- Machine description for ARM for GNU compiler
;;  Copyright (C) 1991-2013 Free Software Foundation, Inc.
;;  Contributed by Pieter `Tiggr' Schoenmakers (rcpieter@win.tue.nl)
;;  and Martin Simmons (@harleqn.co.uk).
;;  More major hacks by Richard Earnshaw (rearnsha@arm.com).

;; This file is part of GCC.

;; GCC is free software; you can redistribute it and/or modify it
;; under the terms of the GNU General Public License as published
;; by the Free Software Foundation; either version 3, or (at your
;; option) any later version.

;; GCC is distributed in the hope that it will be useful, but WITHOUT
;; ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;; or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
;; License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

//Pattern#1
;;- See file "rtl.def" for documentation on define_insn, match_*, et. al.


;;---------------------------------------------------------------------------
;; Constants

;; Register numbers -- All machine registers should be defined here
//Pattern#2
(define_constants
  [(R0_REGNUM         0)	; First CORE register
   (R1_REGNUM	      1)	; Second CORE register
   (IP_REGNUM	     12)	; Scratch register
   (SP_REGNUM	     13)	; Stack pointer
   (LR_REGNUM        14)	; Return address register
   (PC_REGNUM	     15)	; Program counter
   (LAST_ARM_REGNUM  15)	;
   (CC_REGNUM       100)	; Condition code pseudo register
   (VFPCC_REGNUM    101)	; VFP Condition code pseudo register
  ]
)
;; 3rd operand to select_dominance_cc_mode
//Pattern#3
(define_constants
  [(DOM_CC_X_AND_Y  0)
   (DOM_CC_NX_OR_Y  1)
   (DOM_CC_X_OR_Y   2)
  ]
)
;; conditional compare combination
//Pattern#4
(define_constants
  [(CMP_CMP 0)
   (CMN_CMP 1)
   (CMP_CMN 2)
   (CMN_CMN 3)
   (NUM_OF_COND_CMP 4)
  ]
)


;;---------------------------------------------------------------------------
;; Attributes

;; Processor type.  This is created automatically from arm-cores.def.
(include "arm-tune.md")

; IS_THUMB is set to 'yes' when we are generating Thumb code, and 'no' when
; generating ARM code.  This is used to control the length of some insn
; patterns that share the same RTL in both ARM and Thumb code.
//Pattern#5
(define_attr "is_thumb" "no,yes" (const (symbol_ref "thumb_code")))

; IS_ARCH6 is set to 'yes' when we are generating code form ARMv6.
//Pattern#6
(define_attr "is_arch6" "no,yes" (const (symbol_ref "arm_arch6")))

; IS_THUMB1 is set to 'yes' iff we are generating Thumb-1 code.
//Pattern#7
(define_attr "is_thumb1" "no,yes" (const (symbol_ref "thumb1_code")))

;; Operand number of an input operand that is shifted.  Zero if the
;; given instruction does not shift one of its input operands.
//Pattern#8
(define_attr "shift" "" (const_int 0))

; Floating Point Unit.  If we only have floating point emulation, then there
; is no point in scheduling the floating point insns.  (Well, for best
; performance we should try and group them together).
//Pattern#9
(define_attr "fpu" "none,vfp"
  (const (symbol_ref "arm_fpu_attr")))

; LENGTH of an instruction (in bytes)
//Pattern#10
(define_attr "length" ""
  (const_int 4))

; The architecture which supports the instruction (or alternative).
; This can be "a" for ARM, "t" for either of the Thumbs, "32" for
; TARGET_32BIT, "t1" or "t2" to specify a specific Thumb mode.  "v6"
; for ARM or Thumb-2 with arm_arch6, and nov6 for ARM without
; arm_arch6.  This attribute is used to compute attribute "enabled",
; use type "any" to enable an alternative in all cases.
//Pattern#11
(define_attr "arch" "any,a,t,32,t1,t2,v6,nov6,onlya8,neon_onlya8,nota8,neon_nota8,iwmmxt,iwmmxt2"
  (const_string "any"))

//Pattern#12
(define_attr "arch_enabled" "no,yes"
  (cond [(eq_attr "arch" "any")
	 (const_string "yes")

	 (and (eq_attr "arch" "a")
	      (match_test "TARGET_ARM"))
	 (const_string "yes")

	 (and (eq_attr "arch" "t")
	      (match_test "TARGET_THUMB"))
	 (const_string "yes")

	 (and (eq_attr "arch" "t1")
	      (match_test "TARGET_THUMB1"))
	 (const_string "yes")

	 (and (eq_attr "arch" "t2")
	      (match_test "TARGET_THUMB2"))
	 (const_string "yes")

	 (and (eq_attr "arch" "32")
	      (match_test "TARGET_32BIT"))
	 (const_string "yes")

	 (and (eq_attr "arch" "v6")
	      (match_test "TARGET_32BIT && arm_arch6"))
	 (const_string "yes")

	 (and (eq_attr "arch" "nov6")
	      (match_test "TARGET_32BIT && !arm_arch6"))
	 (const_string "yes")

	 (and (eq_attr "arch" "onlya8")
	      (eq_attr "tune" "cortexa8"))
	 (const_string "yes")

	 (and (eq_attr "arch" "neon_onlya8")
	      (eq_attr "tune" "cortexa8")
	      (match_test "TARGET_NEON"))
	 (const_string "yes")

	 (and (eq_attr "arch" "nota8")
	      (not (eq_attr "tune" "cortexa8")))
	 (const_string "yes")

	 (and (eq_attr "arch" "neon_nota8")
	      (not (eq_attr "tune" "cortexa8"))
	      (match_test "TARGET_NEON"))
	 (const_string "yes")

	 (and (eq_attr "arch" "iwmmxt2")
	      (match_test "TARGET_REALLY_IWMMXT2"))
	 (const_string "yes")]

	(const_string "no")))

//Pattern#13
(define_attr "opt" "any,speed,size"
  (const_string "any"))

//Pattern#14
(define_attr "opt_enabled" "no,yes"
  (cond [(eq_attr "opt" "any")
         (const_string "yes")

	 (and (eq_attr "opt" "speed")
	      (match_test "optimize_function_for_speed_p (cfun)"))
	 (const_string "yes")

	 (and (eq_attr "opt" "size")
	      (match_test "optimize_function_for_size_p (cfun)"))
	 (const_string "yes")]
	(const_string "no")))

; Allows an insn to disable certain alternatives for reasons other than
; arch support.
//Pattern#15
(define_attr "insn_enabled" "no,yes"
  (const_string "yes"))

; Enable all alternatives that are both arch_enabled and insn_enabled.
//Pattern#16
 (define_attr "enabled" "no,yes"
   (cond [(eq_attr "insn_enabled" "no")
	  (const_string "no")

	  (eq_attr "arch_enabled" "no")
	  (const_string "no")

	  (eq_attr "opt_enabled" "no")
	  (const_string "no")]
	 (const_string "yes")))

; POOL_RANGE is how far away from a constant pool entry that this insn
; can be placed.  If the distance is zero, then this insn will never
; reference the pool.
; Note that for Thumb constant pools the PC value is rounded down to the
; nearest multiple of four.  Therefore, THUMB2_POOL_RANGE (and POOL_RANGE for
; Thumb insns) should be set to <max_range> - 2.
; NEG_POOL_RANGE is nonzero for insns that can reference a constant pool entry
; before its address.  It is set to <max_range> - (8 + <data_size>).
//Pattern#17
(define_attr "arm_pool_range" "" (const_int 0))
//Pattern#18
(define_attr "thumb2_pool_range" "" (const_int 0))
//Pattern#19
(define_attr "arm_neg_pool_range" "" (const_int 0))
//Pattern#20
(define_attr "thumb2_neg_pool_range" "" (const_int 0))

//Pattern#21
(define_attr "pool_range" ""
  (cond [(eq_attr "is_thumb" "yes") (attr "thumb2_pool_range")]
	(attr "arm_pool_range")))
//Pattern#22
(define_attr "neg_pool_range" ""
  (cond [(eq_attr "is_thumb" "yes") (attr "thumb2_neg_pool_range")]
	(attr "arm_neg_pool_range")))

; An assembler sequence may clobber the condition codes without us knowing.
; If such an insn references the pool, then we have no way of knowing how,
; so use the most conservative value for pool_range.
//Pattern#23
(define_asm_attributes
 [(set_attr "conds" "clob")
  (set_attr "length" "4")
  (set_attr "pool_range" "250")])

;; The instruction used to implement a particular pattern.  This
;; information is used by pipeline descriptions to provide accurate
;; scheduling information.

//Pattern#24
(define_attr "insn"
        "mov,mvn,smulxy,smlaxy,smlalxy,smulwy,smlawx,mul,muls,mla,mlas,umull,umulls,umlal,umlals,smull,smulls,smlal,smlals,smlawy,smuad,smuadx,smlad,smladx,smusd,smusdx,smlsd,smlsdx,smmul,smmulr,smmla,umaal,smlald,smlsld,clz,mrs,msr,xtab,sdiv,udiv,sat,other"
        (const_string "other"))

; TYPE attribute is used to detect floating point instructions which, if
; running on a co-processor can run in parallel with other, basic instructions
; If write-buffer scheduling is enabled then it can also be used in the
; scheduling of writes.

; Classification of each insn
; Note: vfp.md has different meanings for some of these, and some further
; types as well.  See that file for details.
; simple_alu_imm  a simple alu instruction that doesn't hit memory or fp
;               regs or have a shifted source operand and has an immediate
;               operand. This currently only tracks very basic immediate
;               alu operations.
; alu_reg       any alu instruction that doesn't hit memory or fp
;               regs or have a shifted source operand
;               and does not have an immediate operand. This is
;               also the default
; simple_alu_shift covers UXTH, UXTB, SXTH, SXTB
; alu_shift	any data instruction that doesn't hit memory or fp
;		regs, but has a source operand shifted by a constant
; alu_shift_reg	any data instruction that doesn't hit memory or fp
;		regs, but has a source operand shifted by a register value
; mult		a multiply instruction
; block		blockage insn, this blocks all functional units
; float		a floating point arithmetic operation (subject to expansion)
; fdivd		DFmode floating point division
; fdivs		SFmode floating point division
; f_load[sd]	A single/double load from memory. Used for VFP unit.
; f_store[sd]	A single/double store to memory. Used for VFP unit.
; f_flag	a transfer of co-processor flags to the CPSR
; f_2_r		transfer float to core (no memory needed)
; r_2_f		transfer core to float
; f_cvt		convert floating<->integral
; branch	a branch
; call		a subroutine call
; load_byte	load byte(s) from memory to arm registers
; load1		load 1 word from memory to arm registers
; load2         load 2 words from memory to arm registers
; load3         load 3 words from memory to arm registers
; load4         load 4 words from memory to arm registers
; store		store 1 word to memory from arm registers
; store2	store 2 words
; store3	store 3 words
; store4	store 4 (or more) words
;

//Pattern#25
(define_attr "type"
 "simple_alu_imm,\
  alu_reg,\
  simple_alu_shift,\
  alu_shift,\
  alu_shift_reg,\
  mult,\
  block,\
  float,\
  fdivd,\
  fdivs,\
  fmuls,\
  fmuld,\
  fmacs,\
  fmacd,\
  ffmas,\
  ffmad,\
  f_rints,\
  f_rintd,\
  f_minmaxs,\
  f_minmaxd,\
  f_flag,\
  f_loads,\
  f_loadd,\
  f_stores,\
  f_stored,\
  f_2_r,\
  r_2_f,\
  f_cvt,\
  branch,\
  call,\
  load_byte,\
  load1,\
  load2,\
  load3,\
  load4,\
  store1,\
  store2,\
  store3,\
  store4,\
  fconsts,\
  fconstd,\
  fadds,\
  faddd,\
  ffariths,\
  ffarithd,\
  fcmps,\
  fcmpd,\
  fcpys"
 (if_then_else 
    (eq_attr "insn" "smulxy,smlaxy,smlalxy,smulwy,smlawx,mul,muls,mla,mlas,\
	     	     umull,umulls,umlal,umlals,smull,smulls,smlal,smlals")
    (const_string "mult")
    (const_string "alu_reg")))

; Is this an (integer side) multiply with a 64-bit result?
//Pattern#26
(define_attr "mul64" "no,yes"
  (if_then_else
    (eq_attr "insn"
     "smlalxy,umull,umulls,umlal,umlals,smull,smulls,smlal,smlals")
    (const_string "yes")
    (const_string "no")))

; wtype for WMMX insn scheduling purposes.
//Pattern#27
(define_attr "wtype"
        "none,wor,wxor,wand,wandn,wmov,tmcrr,tmrrc,wldr,wstr,tmcr,tmrc,wadd,wsub,wmul,wmac,wavg2,tinsr,textrm,wshufh,wcmpeq,wcmpgt,wmax,wmin,wpack,wunpckih,wunpckil,wunpckeh,wunpckel,wror,wsra,wsrl,wsll,wmadd,tmia,tmiaph,tmiaxy,tbcst,tmovmsk,wacc,waligni,walignr,tandc,textrc,torc,torvsc,wsad,wabs,wabsdiff,waddsubhx,wsubaddhx,wavg4,wmulw,wqmulm,wqmulwm,waddbhus,wqmiaxy,wmiaxy,wmiawxy,wmerge" (const_string "none"))

; Load scheduling, set from the arm_ld_sched variable
; initialized by arm_option_override()
//Pattern#28
(define_attr "ldsched" "no,yes" (const (symbol_ref "arm_ld_sched")))

;; Classification of NEON instructions for scheduling purposes.
//Pattern#29
(define_attr "neon_type"
   "neon_int_1,\
   neon_int_2,\
   neon_int_3,\
   neon_int_4,\
   neon_int_5,\
   neon_vqneg_vqabs,\
   neon_vmov,\
   neon_vaba,\
   neon_vsma,\
   neon_vaba_qqq,\
   neon_mul_ddd_8_16_qdd_16_8_long_32_16_long,\
   neon_mul_qqq_8_16_32_ddd_32,\
   neon_mul_qdd_64_32_long_qqd_16_ddd_32_scalar_64_32_long_scalar,\
   neon_mla_ddd_8_16_qdd_16_8_long_32_16_long,\
   neon_mla_qqq_8_16,\
   neon_mla_ddd_32_qqd_16_ddd_32_scalar_qdd_64_32_long_scalar_qdd_64_32_long,\
   neon_mla_qqq_32_qqd_32_scalar,\
   neon_mul_ddd_16_scalar_32_16_long_scalar,\
   neon_mul_qqd_32_scalar,\
   neon_mla_ddd_16_scalar_qdd_32_16_long_scalar,\
   neon_shift_1,\
   neon_shift_2,\
   neon_shift_3,\
   neon_vshl_ddd,\
   neon_vqshl_vrshl_vqrshl_qqq,\
   neon_vsra_vrsra,\
   neon_fp_vadd_ddd_vabs_dd,\
   neon_fp_vadd_qqq_vabs_qq,\
   neon_fp_vsum,\
   neon_fp_vmul_ddd,\
   neon_fp_vmul_qqd,\
   neon_fp_vmla_ddd,\
   neon_fp_vmla_qqq,\
   neon_fp_vmla_ddd_scalar,\
   neon_fp_vmla_qqq_scalar,\
   neon_fp_vrecps_vrsqrts_ddd,\
   neon_fp_vrecps_vrsqrts_qqq,\
   neon_bp_simple,\
   neon_bp_2cycle,\
   neon_bp_3cycle,\
   neon_ldr,\
   neon_str,\
   neon_vld1_1_2_regs,\
   neon_vld1_3_4_regs,\
   neon_vld2_2_regs_vld1_vld2_all_lanes,\
   neon_vld2_4_regs,\
   neon_vld3_vld4,\
   neon_vst1_1_2_regs_vst2_2_regs,\
   neon_vst1_3_4_regs,\
   neon_vst2_4_regs_vst3_vst4,\
   neon_vst3_vst4,\
   neon_vld1_vld2_lane,\
   neon_vld3_vld4_lane,\
   neon_vst1_vst2_lane,\
   neon_vst3_vst4_lane,\
   neon_vld3_vld4_all_lanes,\
   neon_mcr,\
   neon_mcr_2_mcrr,\
   neon_mrc,\
   neon_mrrc,\
   neon_ldm_2,\
   neon_stm_2,\
   none"
 (const_string "none"))

; condition codes: this one is used by final_prescan_insn to speed up
; conditionalizing instructions.  It saves having to scan the rtl to see if
; it uses or alters the condition codes.
; 
; USE means that the condition codes are used by the insn in the process of
;   outputting code, this means (at present) that we can't use the insn in
;   inlined branches
;
; SET means that the purpose of the insn is to set the condition codes in a
;   well defined manner.
;
; CLOB means that the condition codes are altered in an undefined manner, if
;   they are altered at all
;
; UNCONDITIONAL means the instruction can not be conditionally executed and
;   that the instruction does not use or alter the condition codes.
;
; NOCOND means that the instruction does not use or alter the condition
;   codes but can be converted into a conditionally exectuted instruction.

//Pattern#30
(define_attr "conds" "use,set,clob,unconditional,nocond"
	(if_then_else
	 (ior (eq_attr "is_thumb1" "yes")
	      (eq_attr "type" "call"))
	 (const_string "clob")
	 (if_then_else (eq_attr "neon_type" "none")
	  (const_string "nocond")
	  (const_string "unconditional"))))

; Predicable means that the insn can be conditionally executed based on
; an automatically added predicate (additional patterns are generated by 
; gen...).  We default to 'no' because no Thumb patterns match this rule
; and not all ARM patterns do.
//Pattern#31
(define_attr "predicable" "no,yes" (const_string "no"))

; Only model the write buffer for ARM6 and ARM7.  Earlier processors don't
; have one.  Later ones, such as StrongARM, have write-back caches, so don't
; suffer blockages enough to warrant modelling this (and it can adversely
; affect the schedule).
//Pattern#32
(define_attr "model_wbuf" "no,yes" (const (symbol_ref "arm_tune_wbuf")))

; WRITE_CONFLICT implies that a read following an unrelated write is likely
; to stall the processor.  Used with model_wbuf above.
//Pattern#33
(define_attr "write_conflict" "no,yes"
  (if_then_else (eq_attr "type"
		 "block,call,load1")
		(const_string "yes")
		(const_string "no")))

; Classify the insns into those that take one cycle and those that take more
; than one on the main cpu execution unit.
//Pattern#34
(define_attr "core_cycles" "single,multi"
  (if_then_else (eq_attr "type"
		 "simple_alu_imm,alu_reg,\
                  simple_alu_shift,alu_shift,\
                  float,fdivd,fdivs")
		(const_string "single")
	        (const_string "multi")))

;; FAR_JUMP is "yes" if a BL instruction is used to generate a branch to a
;; distant label.  Only applicable to Thumb code.
//Pattern#35
(define_attr "far_jump" "yes,no" (const_string "no"))


;; The number of machine instructions this pattern expands to.
;; Used for Thumb-2 conditional execution.
//Pattern#36
(define_attr "ce_count" "" (const_int 1))

;;---------------------------------------------------------------------------
;; Unspecs

(include "unspecs.md")

;;---------------------------------------------------------------------------
;; Mode iterators

(include "iterators.md")

;;---------------------------------------------------------------------------
;; Predicates

(include "predicates.md")
(include "constraints.md")

;;---------------------------------------------------------------------------
;; Pipeline descriptions

//Pattern#37
(define_attr "tune_cortexr4" "yes,no"
  (const (if_then_else
	  (eq_attr "tune" "cortexr4,cortexr4f,cortexr5")
	  (const_string "yes")
	  (const_string "no"))))

;; True if the generic scheduling description should be used.

//Pattern#38
(define_attr "generic_sched" "yes,no"
  (const (if_then_else
          (ior (eq_attr "tune" "fa526,fa626,fa606te,fa626te,fmp626,fa726te,arm926ejs,arm1020e,arm1026ejs,arm1136js,arm1136jfs,cortexa5,cortexa7,cortexa8,cortexa9,cortexa15,cortexm4,marvell_pj4")
	       (eq_attr "tune_cortexr4" "yes"))
          (const_string "no")
          (const_string "yes"))))

//Pattern#39
(define_attr "generic_vfp" "yes,no"
  (const (if_then_else
	  (and (eq_attr "fpu" "vfp")
	       (eq_attr "tune" "!arm1020e,arm1022e,cortexa5,cortexa7,cortexa8,cortexa9,cortexm4,marvell_pj4")
	       (eq_attr "tune_cortexr4" "no"))
	  (const_string "yes")
	  (const_string "no"))))

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


;;---------------------------------------------------------------------------
;; Insn patterns
;;
;; Addition insns.

;; Note: For DImode insns, there is normally no reason why operands should
;; not be in the same register, what we don't want is for something being
;; written to partially overlap something that is an input.

//Pattern#40
(define_expand "adddi3"
 [(parallel
   [(set (match_operand:DI           0 "s_register_operand" "")
	  (plus:DI (match_operand:DI 1 "s_register_operand" "")
	           (match_operand:DI 2 "arm_adddi_operand"  "")))
    (clobber (reg:CC CC_REGNUM))])]
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
  [(set (match_operand:DI          0 "register_operand" "=l")
	(plus:DI (match_operand:DI 1 "register_operand" "%0")
		 (match_operand:DI 2 "register_operand" "l")))
   (clobber (reg:CC CC_REGNUM))
  ]
  "TARGET_THUMB1"
  "add\\t%Q0, %Q0, %Q2\;adc\\t%R0, %R0, %R2"
  [(set_attr "length" "4")]
)

//Pattern#42
(define_insn_and_split "*arm_adddi3"
  [(set (match_operand:DI          0 "s_register_operand" "=&r,&r,&r,&r,&r")
	(plus:DI (match_operand:DI 1 "s_register_operand" "%0, 0, r, 0, r")
		 (match_operand:DI 2 "arm_adddi_operand"  "r,  0, r, Dd, Dd")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT && !TARGET_NEON"
  "#"
  "TARGET_32BIT && reload_completed
   && ! (TARGET_NEON && IS_VFP_REGNUM (REGNO (operands[0])))"
  [(parallel [(set (reg:CC_C CC_REGNUM)
		   (compare:CC_C (plus:SI (match_dup 1) (match_dup 2))
				 (match_dup 1)))
	      (set (match_dup 0) (plus:SI (match_dup 1) (match_dup 2)))])
   (set (match_dup 3) (plus:SI (plus:SI (match_dup 4) (match_dup 5))
			       (ltu:SI (reg:CC_C CC_REGNUM) (const_int 0))))]
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
  [(set (match_operand:DI 0 "s_register_operand" "=&r,&r")
	(plus:DI (sign_extend:DI
		  (match_operand:SI 2 "s_register_operand" "r,r"))
		 (match_operand:DI 1 "s_register_operand" "0,r")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT"
  "#"
  "TARGET_32BIT && reload_completed"
  [(parallel [(set (reg:CC_C CC_REGNUM)
		   (compare:CC_C (plus:SI (match_dup 1) (match_dup 2))
				 (match_dup 1)))
	      (set (match_dup 0) (plus:SI (match_dup 1) (match_dup 2)))])
   (set (match_dup 3) (plus:SI (plus:SI (ashiftrt:SI (match_dup 2)
						     (const_int 31))
					(match_dup 4))
			       (ltu:SI (reg:CC_C CC_REGNUM) (const_int 0))))]
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
  [(set (match_operand:DI 0 "s_register_operand" "=&r,&r")
	(plus:DI (zero_extend:DI
		  (match_operand:SI 2 "s_register_operand" "r,r"))
		 (match_operand:DI 1 "s_register_operand" "0,r")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT"
  "#"
  "TARGET_32BIT && reload_completed"
  [(parallel [(set (reg:CC_C CC_REGNUM)
		   (compare:CC_C (plus:SI (match_dup 1) (match_dup 2))
				 (match_dup 1)))
	      (set (match_dup 0) (plus:SI (match_dup 1) (match_dup 2)))])
   (set (match_dup 3) (plus:SI (plus:SI (match_dup 4) (const_int 0))
			       (ltu:SI (reg:CC_C CC_REGNUM) (const_int 0))))]
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
  [(set (match_operand:SI          0 "s_register_operand" "")
	(plus:SI (match_operand:SI 1 "s_register_operand" "")
		 (match_operand:SI 2 "reg_or_int_operand" "")))]
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

; If there is a scratch available, this will be faster than synthesizing the
; addition.
//Pattern#46
(define_peephole2
  [(match_scratch:SI 3 "r")
   (set (match_operand:SI          0 "arm_general_register_operand" "")
	(plus:SI (match_operand:SI 1 "arm_general_register_operand" "")
		 (match_operand:SI 2 "const_int_operand"  "")))]
  "TARGET_32BIT &&
   !(const_ok_for_arm (INTVAL (operands[2]))
     || const_ok_for_arm (-INTVAL (operands[2])))
    && const_ok_for_arm (~INTVAL (operands[2]))"
  [(set (match_dup 3) (match_dup 2))
   (set (match_dup 0) (plus:SI (match_dup 1) (match_dup 3)))]
  ""
)

;; The r/r/k alternative is required when reloading the address
;;  (plus (reg rN) (reg sp)) into (reg rN).  In this case reload will
;; put the duplicated register first, and not try the commutative version.
//Pattern#47
(define_insn_and_split "*arm_addsi3"
  [(set (match_operand:SI          0 "s_register_operand" "=rk, r,k, r,r, k, r, k,k,r, k, r")
	(plus:SI (match_operand:SI 1 "s_register_operand" "%0, rk,k, r,rk,k, rk,k,r,rk,k, rk")
		 (match_operand:SI 2 "reg_or_int_operand" "rk, rI,rI,k,Pj,Pj,L, L,L,PJ,PJ,?n")))]
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
  [(clobber (const_int 0))]
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
  [(set (match_operand:SI          0 "register_operand" "=l,l,l,*rk,*hk,l,k,l,l,l")
	(plus:SI (match_operand:SI 1 "register_operand" "%0,0,l,*0,*0,k,k,0,l,k")
		 (match_operand:SI 2 "nonmemory_operand" "I,J,lL,*hk,*rk,M,O,Pa,Pb,Pc")))]
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
  [(set (match_dup 0) (plus:SI (match_dup 1) (match_dup 2)))
   (set (match_dup 0) (plus:SI (match_dup 0) (match_dup 3)))]
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

;; Reloading and elimination of the frame pointer can
;; sometimes cause this optimization to be missed.
//Pattern#49
(define_peephole2
  [(set (match_operand:SI 0 "arm_general_register_operand" "")
	(match_operand:SI 1 "const_int_operand" ""))
   (set (match_dup 0)
	(plus:SI (match_dup 0) (reg:SI SP_REGNUM)))]
  "TARGET_THUMB1
   && (unsigned HOST_WIDE_INT) (INTVAL (operands[1])) < 1024
   && (INTVAL (operands[1]) & 3) == 0"
  [(set (match_dup 0) (plus:SI (reg:SI SP_REGNUM) (match_dup 1)))]
  ""
)

//Pattern#50
(define_insn "addsi3_compare0"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV
	 (plus:SI (match_operand:SI 1 "s_register_operand" "r, r,r")
		  (match_operand:SI 2 "arm_add_operand"    "I,L,r"))
	 (const_int 0)))
   (set (match_operand:SI 0 "s_register_operand" "=r,r,r")
	(plus:SI (match_dup 1) (match_dup 2)))]
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
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV
	 (plus:SI (match_operand:SI 0 "s_register_operand" "r, r, r")
		  (match_operand:SI 1 "arm_add_operand"    "I,L, r"))
	 (const_int 0)))]
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
  [(set (reg:CC_Z CC_REGNUM)
	(compare:CC_Z
	 (neg:SI (match_operand:SI 0 "s_register_operand" "r"))
	 (match_operand:SI 1 "s_register_operand" "r")))]
  "TARGET_32BIT"
  "cmn%?\\t%1, %0"
  [(set_attr "conds" "set")
   (set_attr "predicable" "yes")]
)

;; This is the canonicalization of addsi3_compare0_for_combiner when the
;; addend is a constant.
//Pattern#53
(define_insn "*cmpsi2_addneg"
  [(set (reg:CC CC_REGNUM)
	(compare:CC
	 (match_operand:SI 1 "s_register_operand" "r,r")
	 (match_operand:SI 2 "arm_addimm_operand" "L,I")))
   (set (match_operand:SI 0 "s_register_operand" "=r,r")
	(plus:SI (match_dup 1)
		 (match_operand:SI 3 "arm_addimm_operand" "I,L")))]
  "TARGET_32BIT && INTVAL (operands[2]) == -INTVAL (operands[3])"
  "@
   add%.\\t%0, %1, %3
   sub%.\\t%0, %1, #%n3"
  [(set_attr "conds" "set")]
)

;; Convert the sequence
;;  sub  rd, rn, #1
;;  cmn  rd, #1	(equivalent to cmp rd, #-1)
;;  bne  dest
;; into
;;  subs rd, rn, #1
;;  bcs  dest	((unsigned)rn >= 1)
;; similarly for the beq variant using bcc.
;; This is a common looping idiom (while (n--))
//Pattern#54
(define_peephole2
  [(set (match_operand:SI 0 "arm_general_register_operand" "")
	(plus:SI (match_operand:SI 1 "arm_general_register_operand" "")
		 (const_int -1)))
   (set (match_operand 2 "cc_register" "")
	(compare (match_dup 0) (const_int -1)))
   (set (pc)
	(if_then_else (match_operator 3 "equality_operator"
		       [(match_dup 2) (const_int 0)])
		      (match_operand 4 "" "")
		      (match_operand 5 "" "")))]
  "TARGET_32BIT && peep2_reg_dead_p (3, operands[2])"
  [(parallel[
    (set (match_dup 2)
	 (compare:CC
	  (match_dup 1) (const_int 1)))
    (set (match_dup 0) (plus:SI (match_dup 1) (const_int -1)))])
   (set (pc)
	(if_then_else (match_op_dup 3 [(match_dup 2) (const_int 0)])
		      (match_dup 4)
		      (match_dup 5)))]
  "operands[2] = gen_rtx_REG (CCmode, CC_REGNUM);
   operands[3] = gen_rtx_fmt_ee ((GET_CODE (operands[3]) == NE
				  ? GEU : LTU),
				 VOIDmode, 
				 operands[2], const0_rtx);"
)

;; The next four insns work because they compare the result with one of
;; the operands, and we know that the use of the condition code is
;; either GEU or LTU, so we can use the carry flag from the addition
;; instead of doing the compare a second time.
//Pattern#55
(define_insn "*addsi3_compare_op1"
  [(set (reg:CC_C CC_REGNUM)
	(compare:CC_C
	 (plus:SI (match_operand:SI 1 "s_register_operand" "r,r,r")
		  (match_operand:SI 2 "arm_add_operand" "I,L,r"))
	 (match_dup 1)))
   (set (match_operand:SI 0 "s_register_operand" "=r,r,r")
	(plus:SI (match_dup 1) (match_dup 2)))]
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
  [(set (reg:CC_C CC_REGNUM)
	(compare:CC_C
	 (plus:SI (match_operand:SI 1 "s_register_operand" "r,r,r")
		  (match_operand:SI 2 "arm_add_operand" "I,L,r"))
	 (match_dup 2)))
   (set (match_operand:SI 0 "s_register_operand" "=r,r,r")
	(plus:SI (match_dup 1) (match_dup 2)))]
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
  [(set (reg:CC_C CC_REGNUM)
	(compare:CC_C
	 (plus:SI (match_operand:SI 0 "s_register_operand" "r,r,r")
		  (match_operand:SI 1 "arm_add_operand" "I,L,r"))
	 (match_dup 0)))]
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
  [(set (reg:CC_C CC_REGNUM)
	(compare:CC_C
	 (plus:SI (match_operand:SI 0 "s_register_operand" "r,r,r")
		  (match_operand:SI 1 "arm_add_operand" "I,L,r"))
	 (match_dup 1)))]
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
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(plus:SI (plus:SI (match_operand:SI 1 "s_register_operand" "%r,r")
			  (match_operand:SI 2 "arm_not_operand" "rI,K"))
		 (LTUGEU:SI (reg:<cnb> CC_REGNUM) (const_int 0))))]
  "TARGET_32BIT"
  "@
   adc%?\\t%0, %1, %2
   sbc%?\\t%0, %1, #%B2"
  [(set_attr "conds" "use")]
)

//Pattern#60
(define_insn "*addsi3_carryin_alt2_<optab>"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(plus:SI (plus:SI (LTUGEU:SI (reg:<cnb> CC_REGNUM) (const_int 0))
			  (match_operand:SI 1 "s_register_operand" "%r,r"))
		 (match_operand:SI 2 "arm_rhs_operand" "rI,K")))]
  "TARGET_32BIT"
  "@
   adc%?\\t%0, %1, %2
   sbc%?\\t%0, %1, #%B2"
  [(set_attr "conds" "use")]
)

//Pattern#61
(define_insn "*addsi3_carryin_shift_<optab>"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(plus:SI (plus:SI
		  (match_operator:SI 2 "shift_operator"
		    [(match_operand:SI 3 "s_register_operand" "r")
		     (match_operand:SI 4 "reg_or_int_operand" "rM")])
		  (match_operand:SI 1 "s_register_operand" "r"))
		 (LTUGEU:SI (reg:<cnb> CC_REGNUM) (const_int 0))))]
  "TARGET_32BIT"
  "adc%?\\t%0, %1, %3%S2"
  [(set_attr "conds" "use")
   (set (attr "type") (if_then_else (match_operand 4 "const_int_operand" "")
		      (const_string "alu_shift")
		      (const_string "alu_shift_reg")))]
)

//Pattern#62
(define_insn "*addsi3_carryin_clobercc_<optab>"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(plus:SI (plus:SI (match_operand:SI 1 "s_register_operand" "%r")
			  (match_operand:SI 2 "arm_rhs_operand" "rI"))
		 (LTUGEU:SI (reg:<cnb> CC_REGNUM) (const_int 0))))
   (clobber (reg:CC CC_REGNUM))]
   "TARGET_32BIT"
   "adc%.\\t%0, %1, %2"
   [(set_attr "conds" "set")]
)

//Pattern#63
(define_expand "incscc"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
        (plus:SI (match_operator:SI 2 "arm_comparison_operator"
                    [(match_operand:CC 3 "cc_register" "") (const_int 0)])
                 (match_operand:SI 1 "s_register_operand" "0,?r")))]
  "TARGET_32BIT"
  ""
)

//Pattern#64
(define_insn "*arm_incscc"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
        (plus:SI (match_operator:SI 2 "arm_comparison_operator"
                    [(match_operand:CC 3 "cc_register" "") (const_int 0)])
                 (match_operand:SI 1 "s_register_operand" "0,?r")))]
  "TARGET_ARM"
  "@
  add%d2\\t%0, %1, #1
  mov%D2\\t%0, %1\;add%d2\\t%0, %1, #1"
  [(set_attr "conds" "use")
   (set_attr "length" "4,8")]
)

; transform ((x << y) - 1) to ~(~(x-1) << y)  Where X is a constant.
//Pattern#65
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(plus:SI (ashift:SI (match_operand:SI 1 "const_int_operand" "")
			    (match_operand:SI 2 "s_register_operand" ""))
		 (const_int -1)))
   (clobber (match_operand:SI 3 "s_register_operand" ""))]
  "TARGET_32BIT"
  [(set (match_dup 3) (match_dup 1))
   (set (match_dup 0) (not:SI (ashift:SI (match_dup 3) (match_dup 2))))]
  "
  operands[1] = GEN_INT (~(INTVAL (operands[1]) - 1));
")

//Pattern#66
(define_expand "addsf3"
  [(set (match_operand:SF          0 "s_register_operand" "")
	(plus:SF (match_operand:SF 1 "s_register_operand" "")
		 (match_operand:SF 2 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT"
  "
")

//Pattern#67
(define_expand "adddf3"
  [(set (match_operand:DF          0 "s_register_operand" "")
	(plus:DF (match_operand:DF 1 "s_register_operand" "")
		 (match_operand:DF 2 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT && !TARGET_VFP_SINGLE"
  "
")

//Pattern#68
(define_expand "subdi3"
 [(parallel
   [(set (match_operand:DI            0 "s_register_operand" "")
	  (minus:DI (match_operand:DI 1 "s_register_operand" "")
	            (match_operand:DI 2 "s_register_operand" "")))
    (clobber (reg:CC CC_REGNUM))])]
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
  [(set (match_operand:DI           0 "s_register_operand" "=&r,&r,&r")
	(minus:DI (match_operand:DI 1 "s_register_operand" "0,r,0")
		  (match_operand:DI 2 "s_register_operand" "r,0,0")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT && !TARGET_NEON"
  "subs\\t%Q0, %Q1, %Q2\;sbc\\t%R0, %R1, %R2"
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)

//Pattern#70
(define_insn "*thumb_subdi3"
  [(set (match_operand:DI           0 "register_operand" "=l")
	(minus:DI (match_operand:DI 1 "register_operand"  "0")
		  (match_operand:DI 2 "register_operand"  "l")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_THUMB1"
  "sub\\t%Q0, %Q0, %Q2\;sbc\\t%R0, %R0, %R2"
  [(set_attr "length" "4")]
)

//Pattern#71
(define_insn "*subdi_di_zesidi"
  [(set (match_operand:DI           0 "s_register_operand" "=&r,&r")
	(minus:DI (match_operand:DI 1 "s_register_operand"  "0,r")
		  (zero_extend:DI
		   (match_operand:SI 2 "s_register_operand"  "r,r"))))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT"
  "subs\\t%Q0, %Q1, %2\;sbc\\t%R0, %R1, #0"
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)

//Pattern#72
(define_insn "*subdi_di_sesidi"
  [(set (match_operand:DI            0 "s_register_operand" "=&r,&r")
	(minus:DI (match_operand:DI  1 "s_register_operand"  "0,r")
		  (sign_extend:DI
		   (match_operand:SI 2 "s_register_operand"  "r,r"))))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT"
  "subs\\t%Q0, %Q1, %2\;sbc\\t%R0, %R1, %2, asr #31"
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)

//Pattern#73
(define_insn "*subdi_zesidi_di"
  [(set (match_operand:DI            0 "s_register_operand" "=&r,&r")
	(minus:DI (zero_extend:DI
		   (match_operand:SI 2 "s_register_operand"  "r,r"))
		  (match_operand:DI  1 "s_register_operand" "0,r")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "rsbs\\t%Q0, %Q1, %2\;rsc\\t%R0, %R1, #0"
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)

//Pattern#74
(define_insn "*subdi_sesidi_di"
  [(set (match_operand:DI            0 "s_register_operand" "=&r,&r")
	(minus:DI (sign_extend:DI
		   (match_operand:SI 2 "s_register_operand"   "r,r"))
		  (match_operand:DI  1 "s_register_operand"  "0,r")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "rsbs\\t%Q0, %Q1, %2\;rsc\\t%R0, %R1, %2, asr #31"
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)

//Pattern#75
(define_insn "*subdi_zesidi_zesidi"
  [(set (match_operand:DI            0 "s_register_operand" "=r")
	(minus:DI (zero_extend:DI
		   (match_operand:SI 1 "s_register_operand"  "r"))
		  (zero_extend:DI
		   (match_operand:SI 2 "s_register_operand"  "r"))))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT"
  "subs\\t%Q0, %1, %2\;sbc\\t%R0, %1, %1"
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)

//Pattern#76
(define_expand "subsi3"
  [(set (match_operand:SI           0 "s_register_operand" "")
	(minus:SI (match_operand:SI 1 "reg_or_int_operand" "")
		  (match_operand:SI 2 "s_register_operand" "")))]
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
  [(set (match_operand:SI           0 "register_operand" "=l")
	(minus:SI (match_operand:SI 1 "register_operand" "l")
		  (match_operand:SI 2 "reg_or_int_operand" "lPd")))]
  "TARGET_THUMB1"
  "sub\\t%0, %1, %2"
  [(set_attr "length" "2")
   (set_attr "conds" "set")])

; ??? Check Thumb-2 split length
//Pattern#78
(define_insn_and_split "*arm_subsi3_insn"
  [(set (match_operand:SI           0 "s_register_operand" "=r,r,r,rk,r")
	(minus:SI (match_operand:SI 1 "reg_or_int_operand" "rI,r,r,k,?n")
		  (match_operand:SI 2 "reg_or_int_operand" "r,I,r,r, r")))]
  "TARGET_32BIT"
  "@
   rsb%?\\t%0, %2, %1
   sub%?\\t%0, %1, %2
   sub%?\\t%0, %1, %2
   sub%?\\t%0, %1, %2
   #"
  "&& (CONST_INT_P (operands[1])
       && !const_ok_for_arm (INTVAL (operands[1])))"
  [(clobber (const_int 0))]
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
  [(match_scratch:SI 3 "r")
   (set (match_operand:SI 0 "arm_general_register_operand" "")
	(minus:SI (match_operand:SI 1 "const_int_operand" "")
		  (match_operand:SI 2 "arm_general_register_operand" "")))]
  "TARGET_32BIT
   && !const_ok_for_arm (INTVAL (operands[1]))
   && const_ok_for_arm (~INTVAL (operands[1]))"
  [(set (match_dup 3) (match_dup 1))
   (set (match_dup 0) (minus:SI (match_dup 3) (match_dup 2)))]
  ""
)

//Pattern#80
(define_insn "*subsi3_compare0"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV
	 (minus:SI (match_operand:SI 1 "arm_rhs_operand" "r,r,I")
		   (match_operand:SI 2 "arm_rhs_operand" "I,r,r"))
	 (const_int 0)))
   (set (match_operand:SI 0 "s_register_operand" "=r,r,r")
	(minus:SI (match_dup 1) (match_dup 2)))]
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
  [(set (reg:CC CC_REGNUM)
	(compare:CC (match_operand:SI 1 "arm_rhs_operand" "r,r,I")
		    (match_operand:SI 2 "arm_rhs_operand" "I,r,r")))
   (set (match_operand:SI 0 "s_register_operand" "=r,r,r")
	(minus:SI (match_dup 1) (match_dup 2)))]
  "TARGET_32BIT"
  "@
   sub%.\\t%0, %1, %2
   sub%.\\t%0, %1, %2
   rsb%.\\t%0, %2, %1"
  [(set_attr "conds" "set")
   (set_attr "type" "simple_alu_imm,*,*")]
)

//Pattern#82
(define_expand "decscc"
  [(set (match_operand:SI            0 "s_register_operand" "=r,r")
        (minus:SI (match_operand:SI  1 "s_register_operand" "0,?r")
		  (match_operator:SI 2 "arm_comparison_operator"
                   [(match_operand   3 "cc_register" "") (const_int 0)])))]
  "TARGET_32BIT"
  ""
)

//Pattern#83
(define_insn "*arm_decscc"
  [(set (match_operand:SI            0 "s_register_operand" "=r,r")
        (minus:SI (match_operand:SI  1 "s_register_operand" "0,?r")
		  (match_operator:SI 2 "arm_comparison_operator"
                   [(match_operand   3 "cc_register" "") (const_int 0)])))]
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
  [(set (match_operand:SF           0 "s_register_operand" "")
	(minus:SF (match_operand:SF 1 "s_register_operand" "")
		  (match_operand:SF 2 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT"
  "
")

//Pattern#85
(define_expand "subdf3"
  [(set (match_operand:DF           0 "s_register_operand" "")
	(minus:DF (match_operand:DF 1 "s_register_operand" "")
		  (match_operand:DF 2 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT && !TARGET_VFP_SINGLE"
  "
")


;; Multiplication insns

//Pattern#86
(define_expand "mulsi3"
  [(set (match_operand:SI          0 "s_register_operand" "")
	(mult:SI (match_operand:SI 2 "s_register_operand" "")
		 (match_operand:SI 1 "s_register_operand" "")))]
  "TARGET_EITHER"
  ""
)

;; Use `&' and then `0' to prevent the operands 0 and 1 being the same
//Pattern#87
(define_insn "*arm_mulsi3"
  [(set (match_operand:SI          0 "s_register_operand" "=&r,&r")
	(mult:SI (match_operand:SI 2 "s_register_operand" "r,r")
		 (match_operand:SI 1 "s_register_operand" "%0,r")))]
  "TARGET_32BIT && !arm_arch6"
  "mul%?\\t%0, %2, %1"
  [(set_attr "insn" "mul")
   (set_attr "predicable" "yes")]
)

//Pattern#88
(define_insn "*arm_mulsi3_v6"
  [(set (match_operand:SI          0 "s_register_operand" "=r")
	(mult:SI (match_operand:SI 1 "s_register_operand" "r")
		 (match_operand:SI 2 "s_register_operand" "r")))]
  "TARGET_32BIT && arm_arch6"
  "mul%?\\t%0, %1, %2"
  [(set_attr "insn" "mul")
   (set_attr "predicable" "yes")]
)

; Unfortunately with the Thumb the '&'/'0' trick can fails when operands 
; 1 and 2; are the same, because reload will make operand 0 match 
; operand 1 without realizing that this conflicts with operand 2.  We fix 
; this by adding another alternative to match this case, and then `reload' 
; it ourselves.  This alternative must come first.
//Pattern#89
(define_insn "*thumb_mulsi3"
  [(set (match_operand:SI          0 "register_operand" "=&l,&l,&l")
	(mult:SI (match_operand:SI 1 "register_operand" "%l,*h,0")
		 (match_operand:SI 2 "register_operand" "l,l,l")))]
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
  [(set (match_operand:SI          0 "register_operand" "=l,l,l")
	(mult:SI (match_operand:SI 1 "register_operand" "0,l,0")
		 (match_operand:SI 2 "register_operand" "l,0,0")))]
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
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV (mult:SI
			  (match_operand:SI 2 "s_register_operand" "r,r")
			  (match_operand:SI 1 "s_register_operand" "%0,r"))
			 (const_int 0)))
   (set (match_operand:SI 0 "s_register_operand" "=&r,&r")
	(mult:SI (match_dup 2) (match_dup 1)))]
  "TARGET_ARM && !arm_arch6"
  "mul%.\\t%0, %2, %1"
  [(set_attr "conds" "set")
   (set_attr "insn" "muls")]
)

//Pattern#92
(define_insn "*mulsi3_compare0_v6"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV (mult:SI
			  (match_operand:SI 2 "s_register_operand" "r")
			  (match_operand:SI 1 "s_register_operand" "r"))
			 (const_int 0)))
   (set (match_operand:SI 0 "s_register_operand" "=r")
	(mult:SI (match_dup 2) (match_dup 1)))]
  "TARGET_ARM && arm_arch6 && optimize_size"
  "mul%.\\t%0, %2, %1"
  [(set_attr "conds" "set")
   (set_attr "insn" "muls")]
)

//Pattern#93
(define_insn "*mulsi_compare0_scratch"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV (mult:SI
			  (match_operand:SI 2 "s_register_operand" "r,r")
			  (match_operand:SI 1 "s_register_operand" "%0,r"))
			 (const_int 0)))
   (clobber (match_scratch:SI 0 "=&r,&r"))]
  "TARGET_ARM && !arm_arch6"
  "mul%.\\t%0, %2, %1"
  [(set_attr "conds" "set")
   (set_attr "insn" "muls")]
)

//Pattern#94
(define_insn "*mulsi_compare0_scratch_v6"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV (mult:SI
			  (match_operand:SI 2 "s_register_operand" "r")
			  (match_operand:SI 1 "s_register_operand" "r"))
			 (const_int 0)))
   (clobber (match_scratch:SI 0 "=r"))]
  "TARGET_ARM && arm_arch6 && optimize_size"
  "mul%.\\t%0, %2, %1"
  [(set_attr "conds" "set")
   (set_attr "insn" "muls")]
)

;; Unnamed templates to match MLA instruction.

//Pattern#95
(define_insn "*mulsi3addsi"
  [(set (match_operand:SI 0 "s_register_operand" "=&r,&r,&r,&r")
	(plus:SI
	  (mult:SI (match_operand:SI 2 "s_register_operand" "r,r,r,r")
		   (match_operand:SI 1 "s_register_operand" "%0,r,0,r"))
	  (match_operand:SI 3 "s_register_operand" "r,r,0,0")))]
  "TARGET_32BIT && !arm_arch6"
  "mla%?\\t%0, %2, %1, %3"
  [(set_attr "insn" "mla")
   (set_attr "predicable" "yes")]
)

//Pattern#96
(define_insn "*mulsi3addsi_v6"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(plus:SI
	  (mult:SI (match_operand:SI 2 "s_register_operand" "r")
		   (match_operand:SI 1 "s_register_operand" "r"))
	  (match_operand:SI 3 "s_register_operand" "r")))]
  "TARGET_32BIT && arm_arch6"
  "mla%?\\t%0, %2, %1, %3"
  [(set_attr "insn" "mla")
   (set_attr "predicable" "yes")]
)

//Pattern#97
(define_insn "*mulsi3addsi_compare0"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV
	 (plus:SI (mult:SI
		   (match_operand:SI 2 "s_register_operand" "r,r,r,r")
		   (match_operand:SI 1 "s_register_operand" "%0,r,0,r"))
		  (match_operand:SI 3 "s_register_operand" "r,r,0,0"))
	 (const_int 0)))
   (set (match_operand:SI 0 "s_register_operand" "=&r,&r,&r,&r")
	(plus:SI (mult:SI (match_dup 2) (match_dup 1))
		 (match_dup 3)))]
  "TARGET_ARM && arm_arch6"
  "mla%.\\t%0, %2, %1, %3"
  [(set_attr "conds" "set")
   (set_attr "insn" "mlas")]
)

//Pattern#98
(define_insn "*mulsi3addsi_compare0_v6"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV
	 (plus:SI (mult:SI
		   (match_operand:SI 2 "s_register_operand" "r")
		   (match_operand:SI 1 "s_register_operand" "r"))
		  (match_operand:SI 3 "s_register_operand" "r"))
	 (const_int 0)))
   (set (match_operand:SI 0 "s_register_operand" "=r")
	(plus:SI (mult:SI (match_dup 2) (match_dup 1))
		 (match_dup 3)))]
  "TARGET_ARM && arm_arch6 && optimize_size"
  "mla%.\\t%0, %2, %1, %3"
  [(set_attr "conds" "set")
   (set_attr "insn" "mlas")]
)

//Pattern#99
(define_insn "*mulsi3addsi_compare0_scratch"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV
	 (plus:SI (mult:SI
		   (match_operand:SI 2 "s_register_operand" "r,r,r,r")
		   (match_operand:SI 1 "s_register_operand" "%0,r,0,r"))
		  (match_operand:SI 3 "s_register_operand" "?r,r,0,0"))
	 (const_int 0)))
   (clobber (match_scratch:SI 0 "=&r,&r,&r,&r"))]
  "TARGET_ARM && !arm_arch6"
  "mla%.\\t%0, %2, %1, %3"
  [(set_attr "conds" "set")
   (set_attr "insn" "mlas")]
)

//Pattern#100
(define_insn "*mulsi3addsi_compare0_scratch_v6"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV
	 (plus:SI (mult:SI
		   (match_operand:SI 2 "s_register_operand" "r")
		   (match_operand:SI 1 "s_register_operand" "r"))
		  (match_operand:SI 3 "s_register_operand" "r"))
	 (const_int 0)))
   (clobber (match_scratch:SI 0 "=r"))]
  "TARGET_ARM && arm_arch6 && optimize_size"
  "mla%.\\t%0, %2, %1, %3"
  [(set_attr "conds" "set")
   (set_attr "insn" "mlas")]
)

//Pattern#101
(define_insn "*mulsi3subsi"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(minus:SI
	  (match_operand:SI 3 "s_register_operand" "r")
	  (mult:SI (match_operand:SI 2 "s_register_operand" "r")
		   (match_operand:SI 1 "s_register_operand" "r"))))]
  "TARGET_32BIT && arm_arch_thumb2"
  "mls%?\\t%0, %2, %1, %3"
  [(set_attr "insn" "mla")
   (set_attr "predicable" "yes")]
)

//Pattern#102
(define_expand "maddsidi4"
  [(set (match_operand:DI 0 "s_register_operand" "")
	(plus:DI
	 (mult:DI
	  (sign_extend:DI (match_operand:SI 1 "s_register_operand" ""))
	  (sign_extend:DI (match_operand:SI 2 "s_register_operand" "")))
	 (match_operand:DI 3 "s_register_operand" "")))]
  "TARGET_32BIT && arm_arch3m"
  "")

//Pattern#103
(define_insn "*mulsidi3adddi"
  [(set (match_operand:DI 0 "s_register_operand" "=&r")
	(plus:DI
	 (mult:DI
	  (sign_extend:DI (match_operand:SI 2 "s_register_operand" "%r"))
	  (sign_extend:DI (match_operand:SI 3 "s_register_operand" "r")))
	 (match_operand:DI 1 "s_register_operand" "0")))]
  "TARGET_32BIT && arm_arch3m && !arm_arch6"
  "smlal%?\\t%Q0, %R0, %3, %2"
  [(set_attr "insn" "smlal")
   (set_attr "predicable" "yes")]
)

//Pattern#104
(define_insn "*mulsidi3adddi_v6"
  [(set (match_operand:DI 0 "s_register_operand" "=r")
	(plus:DI
	 (mult:DI
	  (sign_extend:DI (match_operand:SI 2 "s_register_operand" "r"))
	  (sign_extend:DI (match_operand:SI 3 "s_register_operand" "r")))
	 (match_operand:DI 1 "s_register_operand" "0")))]
  "TARGET_32BIT && arm_arch6"
  "smlal%?\\t%Q0, %R0, %3, %2"
  [(set_attr "insn" "smlal")
   (set_attr "predicable" "yes")]
)

;; 32x32->64 widening multiply.
;; As with mulsi3, the only difference between the v3-5 and v6+
;; versions of these patterns is the requirement that the output not
;; overlap the inputs, but that still means we have to have a named
;; expander and two different starred insns.

//Pattern#105
(define_expand "mulsidi3"
  [(set (match_operand:DI 0 "s_register_operand" "")
	(mult:DI
	 (sign_extend:DI (match_operand:SI 1 "s_register_operand" ""))
	 (sign_extend:DI (match_operand:SI 2 "s_register_operand" ""))))]
  "TARGET_32BIT && arm_arch3m"
  ""
)

//Pattern#106
(define_insn "*mulsidi3_nov6"
  [(set (match_operand:DI 0 "s_register_operand" "=&r")
	(mult:DI
	 (sign_extend:DI (match_operand:SI 1 "s_register_operand" "%r"))
	 (sign_extend:DI (match_operand:SI 2 "s_register_operand" "r"))))]
  "TARGET_32BIT && arm_arch3m && !arm_arch6"
  "smull%?\\t%Q0, %R0, %1, %2"
  [(set_attr "insn" "smull")
   (set_attr "predicable" "yes")]
)

//Pattern#107
(define_insn "*mulsidi3_v6"
  [(set (match_operand:DI 0 "s_register_operand" "=r")
	(mult:DI
	 (sign_extend:DI (match_operand:SI 1 "s_register_operand" "r"))
	 (sign_extend:DI (match_operand:SI 2 "s_register_operand" "r"))))]
  "TARGET_32BIT && arm_arch6"
  "smull%?\\t%Q0, %R0, %1, %2"
  [(set_attr "insn" "smull")
   (set_attr "predicable" "yes")]
)

//Pattern#108
(define_expand "umulsidi3"
  [(set (match_operand:DI 0 "s_register_operand" "")
	(mult:DI
	 (zero_extend:DI (match_operand:SI 1 "s_register_operand" ""))
	 (zero_extend:DI (match_operand:SI 2 "s_register_operand" ""))))]
  "TARGET_32BIT && arm_arch3m"
  ""
)

//Pattern#109
(define_insn "*umulsidi3_nov6"
  [(set (match_operand:DI 0 "s_register_operand" "=&r")
	(mult:DI
	 (zero_extend:DI (match_operand:SI 1 "s_register_operand" "%r"))
	 (zero_extend:DI (match_operand:SI 2 "s_register_operand" "r"))))]
  "TARGET_32BIT && arm_arch3m && !arm_arch6"
  "umull%?\\t%Q0, %R0, %1, %2"
  [(set_attr "insn" "umull")
   (set_attr "predicable" "yes")]
)

//Pattern#110
(define_insn "*umulsidi3_v6"
  [(set (match_operand:DI 0 "s_register_operand" "=r")
	(mult:DI
	 (zero_extend:DI (match_operand:SI 1 "s_register_operand" "r"))
	 (zero_extend:DI (match_operand:SI 2 "s_register_operand" "r"))))]
  "TARGET_32BIT && arm_arch6"
  "umull%?\\t%Q0, %R0, %1, %2"
  [(set_attr "insn" "umull")
   (set_attr "predicable" "yes")]
)

//Pattern#111
(define_expand "umaddsidi4"
  [(set (match_operand:DI 0 "s_register_operand" "")
	(plus:DI
	 (mult:DI
	  (zero_extend:DI (match_operand:SI 1 "s_register_operand" ""))
	  (zero_extend:DI (match_operand:SI 2 "s_register_operand" "")))
	 (match_operand:DI 3 "s_register_operand" "")))]
  "TARGET_32BIT && arm_arch3m"
  "")

//Pattern#112
(define_insn "*umulsidi3adddi"
  [(set (match_operand:DI 0 "s_register_operand" "=&r")
	(plus:DI
	 (mult:DI
	  (zero_extend:DI (match_operand:SI 2 "s_register_operand" "%r"))
	  (zero_extend:DI (match_operand:SI 3 "s_register_operand" "r")))
	 (match_operand:DI 1 "s_register_operand" "0")))]
  "TARGET_32BIT && arm_arch3m && !arm_arch6"
  "umlal%?\\t%Q0, %R0, %3, %2"
  [(set_attr "insn" "umlal")
   (set_attr "predicable" "yes")]
)

//Pattern#113
(define_insn "*umulsidi3adddi_v6"
  [(set (match_operand:DI 0 "s_register_operand" "=r")
	(plus:DI
	 (mult:DI
	  (zero_extend:DI (match_operand:SI 2 "s_register_operand" "r"))
	  (zero_extend:DI (match_operand:SI 3 "s_register_operand" "r")))
	 (match_operand:DI 1 "s_register_operand" "0")))]
  "TARGET_32BIT && arm_arch6"
  "umlal%?\\t%Q0, %R0, %3, %2"
  [(set_attr "insn" "umlal")
   (set_attr "predicable" "yes")]
)

//Pattern#114
(define_expand "smulsi3_highpart"
  [(parallel
    [(set (match_operand:SI 0 "s_register_operand" "")
	  (truncate:SI
	   (lshiftrt:DI
	    (mult:DI
	     (sign_extend:DI (match_operand:SI 1 "s_register_operand" ""))
	     (sign_extend:DI (match_operand:SI 2 "s_register_operand" "")))
	    (const_int 32))))
     (clobber (match_scratch:SI 3 ""))])]
  "TARGET_32BIT && arm_arch3m"
  ""
)

//Pattern#115
(define_insn "*smulsi3_highpart_nov6"
  [(set (match_operand:SI 0 "s_register_operand" "=&r,&r")
	(truncate:SI
	 (lshiftrt:DI
	  (mult:DI
	   (sign_extend:DI (match_operand:SI 1 "s_register_operand" "%0,r"))
	   (sign_extend:DI (match_operand:SI 2 "s_register_operand" "r,r")))
	  (const_int 32))))
   (clobber (match_scratch:SI 3 "=&r,&r"))]
  "TARGET_32BIT && arm_arch3m && !arm_arch6"
  "smull%?\\t%3, %0, %2, %1"
  [(set_attr "insn" "smull")
   (set_attr "predicable" "yes")]
)

//Pattern#116
(define_insn "*smulsi3_highpart_v6"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(truncate:SI
	 (lshiftrt:DI
	  (mult:DI
	   (sign_extend:DI (match_operand:SI 1 "s_register_operand" "r"))
	   (sign_extend:DI (match_operand:SI 2 "s_register_operand" "r")))
	  (const_int 32))))
   (clobber (match_scratch:SI 3 "=r"))]
  "TARGET_32BIT && arm_arch6"
  "smull%?\\t%3, %0, %2, %1"
  [(set_attr "insn" "smull")
   (set_attr "predicable" "yes")]
)

//Pattern#117
(define_expand "umulsi3_highpart"
  [(parallel
    [(set (match_operand:SI 0 "s_register_operand" "")
	  (truncate:SI
	   (lshiftrt:DI
	    (mult:DI
	     (zero_extend:DI (match_operand:SI 1 "s_register_operand" ""))
	      (zero_extend:DI (match_operand:SI 2 "s_register_operand" "")))
	    (const_int 32))))
     (clobber (match_scratch:SI 3 ""))])]
  "TARGET_32BIT && arm_arch3m"
  ""
)

//Pattern#118
(define_insn "*umulsi3_highpart_nov6"
  [(set (match_operand:SI 0 "s_register_operand" "=&r,&r")
	(truncate:SI
	 (lshiftrt:DI
	  (mult:DI
	   (zero_extend:DI (match_operand:SI 1 "s_register_operand" "%0,r"))
	   (zero_extend:DI (match_operand:SI 2 "s_register_operand" "r,r")))
	  (const_int 32))))
   (clobber (match_scratch:SI 3 "=&r,&r"))]
  "TARGET_32BIT && arm_arch3m && !arm_arch6"
  "umull%?\\t%3, %0, %2, %1"
  [(set_attr "insn" "umull")
   (set_attr "predicable" "yes")]
)

//Pattern#119
(define_insn "*umulsi3_highpart_v6"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(truncate:SI
	 (lshiftrt:DI
	  (mult:DI
	   (zero_extend:DI (match_operand:SI 1 "s_register_operand" "r"))
	   (zero_extend:DI (match_operand:SI 2 "s_register_operand" "r")))
	  (const_int 32))))
   (clobber (match_scratch:SI 3 "=r"))]
  "TARGET_32BIT && arm_arch6"
  "umull%?\\t%3, %0, %2, %1"
  [(set_attr "insn" "umull")
   (set_attr "predicable" "yes")]
)

//Pattern#120
(define_insn "mulhisi3"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(mult:SI (sign_extend:SI
		  (match_operand:HI 1 "s_register_operand" "%r"))
		 (sign_extend:SI
		  (match_operand:HI 2 "s_register_operand" "r"))))]
  "TARGET_DSP_MULTIPLY"
  "smulbb%?\\t%0, %1, %2"
  [(set_attr "insn" "smulxy")
   (set_attr "predicable" "yes")]
)

//Pattern#121
(define_insn "*mulhisi3tb"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(mult:SI (ashiftrt:SI
		  (match_operand:SI 1 "s_register_operand" "r")
		  (const_int 16))
		 (sign_extend:SI
		  (match_operand:HI 2 "s_register_operand" "r"))))]
  "TARGET_DSP_MULTIPLY"
  "smultb%?\\t%0, %1, %2"
  [(set_attr "insn" "smulxy")
   (set_attr "predicable" "yes")]
)

//Pattern#122
(define_insn "*mulhisi3bt"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(mult:SI (sign_extend:SI
		  (match_operand:HI 1 "s_register_operand" "r"))
		 (ashiftrt:SI
		  (match_operand:SI 2 "s_register_operand" "r")
		  (const_int 16))))]
  "TARGET_DSP_MULTIPLY"
  "smulbt%?\\t%0, %1, %2"
  [(set_attr "insn" "smulxy")
   (set_attr "predicable" "yes")]
)

//Pattern#123
(define_insn "*mulhisi3tt"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(mult:SI (ashiftrt:SI
		  (match_operand:SI 1 "s_register_operand" "r")
		  (const_int 16))
		 (ashiftrt:SI
		  (match_operand:SI 2 "s_register_operand" "r")
		  (const_int 16))))]
  "TARGET_DSP_MULTIPLY"
  "smultt%?\\t%0, %1, %2"
  [(set_attr "insn" "smulxy")
   (set_attr "predicable" "yes")]
)

//Pattern#124
(define_insn "maddhisi4"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(plus:SI (mult:SI (sign_extend:SI
			   (match_operand:HI 1 "s_register_operand" "r"))
			  (sign_extend:SI
			   (match_operand:HI 2 "s_register_operand" "r")))
		 (match_operand:SI 3 "s_register_operand" "r")))]
  "TARGET_DSP_MULTIPLY"
  "smlabb%?\\t%0, %1, %2, %3"
  [(set_attr "insn" "smlaxy")
   (set_attr "predicable" "yes")]
)

;; Note: there is no maddhisi4ibt because this one is canonical form
//Pattern#125
(define_insn "*maddhisi4tb"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(plus:SI (mult:SI (ashiftrt:SI
			   (match_operand:SI 1 "s_register_operand" "r")
			   (const_int 16))
			  (sign_extend:SI
			   (match_operand:HI 2 "s_register_operand" "r")))
		 (match_operand:SI 3 "s_register_operand" "r")))]
  "TARGET_DSP_MULTIPLY"
  "smlatb%?\\t%0, %1, %2, %3"
  [(set_attr "insn" "smlaxy")
   (set_attr "predicable" "yes")]
)

//Pattern#126
(define_insn "*maddhisi4tt"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(plus:SI (mult:SI (ashiftrt:SI
			   (match_operand:SI 1 "s_register_operand" "r")
			   (const_int 16))
			  (ashiftrt:SI
			   (match_operand:SI 2 "s_register_operand" "r")
			   (const_int 16)))
		 (match_operand:SI 3 "s_register_operand" "r")))]
  "TARGET_DSP_MULTIPLY"
  "smlatt%?\\t%0, %1, %2, %3"
  [(set_attr "insn" "smlaxy")
   (set_attr "predicable" "yes")]
)

//Pattern#127
(define_insn "maddhidi4"
  [(set (match_operand:DI 0 "s_register_operand" "=r")
	(plus:DI
	  (mult:DI (sign_extend:DI
	 	    (match_operand:HI 1 "s_register_operand" "r"))
		   (sign_extend:DI
		    (match_operand:HI 2 "s_register_operand" "r")))
	  (match_operand:DI 3 "s_register_operand" "0")))]
  "TARGET_DSP_MULTIPLY"
  "smlalbb%?\\t%Q0, %R0, %1, %2"
  [(set_attr "insn" "smlalxy")
   (set_attr "predicable" "yes")])

;; Note: there is no maddhidi4ibt because this one is canonical form
//Pattern#128
(define_insn "*maddhidi4tb"
  [(set (match_operand:DI 0 "s_register_operand" "=r")
	(plus:DI
	  (mult:DI (sign_extend:DI
		    (ashiftrt:SI
		     (match_operand:SI 1 "s_register_operand" "r")
		     (const_int 16)))
		   (sign_extend:DI
		    (match_operand:HI 2 "s_register_operand" "r")))
	  (match_operand:DI 3 "s_register_operand" "0")))]
  "TARGET_DSP_MULTIPLY"
  "smlaltb%?\\t%Q0, %R0, %1, %2"
  [(set_attr "insn" "smlalxy")
   (set_attr "predicable" "yes")])

//Pattern#129
(define_insn "*maddhidi4tt"
  [(set (match_operand:DI 0 "s_register_operand" "=r")
	(plus:DI
	  (mult:DI (sign_extend:DI
		    (ashiftrt:SI
		     (match_operand:SI 1 "s_register_operand" "r")
		     (const_int 16)))
		   (sign_extend:DI
		    (ashiftrt:SI
		     (match_operand:SI 2 "s_register_operand" "r")
		     (const_int 16))))
	  (match_operand:DI 3 "s_register_operand" "0")))]
  "TARGET_DSP_MULTIPLY"
  "smlaltt%?\\t%Q0, %R0, %1, %2"
  [(set_attr "insn" "smlalxy")
   (set_attr "predicable" "yes")])

//Pattern#130
(define_expand "mulsf3"
  [(set (match_operand:SF          0 "s_register_operand" "")
	(mult:SF (match_operand:SF 1 "s_register_operand" "")
		 (match_operand:SF 2 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT"
  "
")

//Pattern#131
(define_expand "muldf3"
  [(set (match_operand:DF          0 "s_register_operand" "")
	(mult:DF (match_operand:DF 1 "s_register_operand" "")
		 (match_operand:DF 2 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT && !TARGET_VFP_SINGLE"
  "
")

;; Division insns

//Pattern#132
(define_expand "divsf3"
  [(set (match_operand:SF 0 "s_register_operand" "")
	(div:SF (match_operand:SF 1 "s_register_operand" "")
		(match_operand:SF 2 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP"
  "")

//Pattern#133
(define_expand "divdf3"
  [(set (match_operand:DF 0 "s_register_operand" "")
	(div:DF (match_operand:DF 1 "s_register_operand" "")
		(match_operand:DF 2 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP_DOUBLE"
  "")

;; Boolean and,ior,xor insns

;; Split up double word logical operations

;; Split up simple DImode logical operations.  Simply perform the logical
;; operation on the upper and lower halves of the registers.
//Pattern#134
(define_split
  [(set (match_operand:DI 0 "s_register_operand" "")
	(match_operator:DI 6 "logical_binary_operator"
	  [(match_operand:DI 1 "s_register_operand" "")
	   (match_operand:DI 2 "s_register_operand" "")]))]
  "TARGET_32BIT && reload_completed
   && ! (TARGET_NEON && IS_VFP_REGNUM (REGNO (operands[0])))
   && ! IS_IWMMXT_REGNUM (REGNO (operands[0]))"
  [(set (match_dup 0) (match_op_dup:SI 6 [(match_dup 1) (match_dup 2)]))
   (set (match_dup 3) (match_op_dup:SI 6 [(match_dup 4) (match_dup 5)]))]
  "
  {
    operands[3] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[4] = gen_highpart (SImode, operands[1]);
    operands[1] = gen_lowpart (SImode, operands[1]);
    operands[5] = gen_highpart (SImode, operands[2]);
    operands[2] = gen_lowpart (SImode, operands[2]);
  }"
)

//Pattern#135
(define_split
  [(set (match_operand:DI 0 "s_register_operand" "")
	(match_operator:DI 6 "logical_binary_operator"
	  [(sign_extend:DI (match_operand:SI 2 "s_register_operand" ""))
	   (match_operand:DI 1 "s_register_operand" "")]))]
  "TARGET_32BIT && reload_completed"
  [(set (match_dup 0) (match_op_dup:SI 6 [(match_dup 1) (match_dup 2)]))
   (set (match_dup 3) (match_op_dup:SI 6
			[(ashiftrt:SI (match_dup 2) (const_int 31))
			 (match_dup 4)]))]
  "
  {
    operands[3] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[4] = gen_highpart (SImode, operands[1]);
    operands[1] = gen_lowpart (SImode, operands[1]);
    operands[5] = gen_highpart (SImode, operands[2]);
    operands[2] = gen_lowpart (SImode, operands[2]);
  }"
)

;; The zero extend of operand 2 means we can just copy the high part of
;; operand1 into operand0.
//Pattern#136
(define_split
  [(set (match_operand:DI 0 "s_register_operand" "")
	(ior:DI
	  (zero_extend:DI (match_operand:SI 2 "s_register_operand" ""))
	  (match_operand:DI 1 "s_register_operand" "")))]
  "TARGET_32BIT && operands[0] != operands[1] && reload_completed"
  [(set (match_dup 0) (ior:SI (match_dup 1) (match_dup 2)))
   (set (match_dup 3) (match_dup 4))]
  "
  {
    operands[4] = gen_highpart (SImode, operands[1]);
    operands[3] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[1] = gen_lowpart (SImode, operands[1]);
  }"
)

;; The zero extend of operand 2 means we can just copy the high part of
;; operand1 into operand0.
//Pattern#137
(define_split
  [(set (match_operand:DI 0 "s_register_operand" "")
	(xor:DI
	  (zero_extend:DI (match_operand:SI 2 "s_register_operand" ""))
	  (match_operand:DI 1 "s_register_operand" "")))]
  "TARGET_32BIT && operands[0] != operands[1] && reload_completed"
  [(set (match_dup 0) (xor:SI (match_dup 1) (match_dup 2)))
   (set (match_dup 3) (match_dup 4))]
  "
  {
    operands[4] = gen_highpart (SImode, operands[1]);
    operands[3] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[1] = gen_lowpart (SImode, operands[1]);
  }"
)

//Pattern#138
(define_expand "anddi3"
  [(set (match_operand:DI         0 "s_register_operand" "")
	(and:DI (match_operand:DI 1 "s_register_operand" "")
		(match_operand:DI 2 "neon_inv_logic_op2" "")))]
  "TARGET_32BIT"
  ""
)

//Pattern#139
(define_insn "*anddi3_insn"
  [(set (match_operand:DI         0 "s_register_operand" "=&r,&r")
	(and:DI (match_operand:DI 1 "s_register_operand"  "%0,r")
		(match_operand:DI 2 "s_register_operand"   "r,r")))]
  "TARGET_32BIT && !TARGET_IWMMXT && !TARGET_NEON"
  "#"
  [(set_attr "length" "8")]
)

//Pattern#140
(define_insn_and_split "*anddi_zesidi_di"
  [(set (match_operand:DI 0 "s_register_operand" "=&r,&r")
	(and:DI (zero_extend:DI
		 (match_operand:SI 2 "s_register_operand" "r,r"))
		(match_operand:DI 1 "s_register_operand" "0,r")))]
  "TARGET_32BIT"
  "#"
  "TARGET_32BIT && reload_completed"
  ; The zero extend of operand 2 clears the high word of the output
  ; operand.
  [(set (match_dup 0) (and:SI (match_dup 1) (match_dup 2)))
   (set (match_dup 3) (const_int 0))]
  "
  {
    operands[3] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[1] = gen_lowpart (SImode, operands[1]);
  }"
  [(set_attr "length" "8")]
)

//Pattern#141
(define_insn "*anddi_sesdi_di"
  [(set (match_operand:DI          0 "s_register_operand" "=&r,&r")
	(and:DI (sign_extend:DI
		 (match_operand:SI 2 "s_register_operand" "r,r"))
		(match_operand:DI  1 "s_register_operand" "0,r")))]
  "TARGET_32BIT"
  "#"
  [(set_attr "length" "8")]
)

//Pattern#142
(define_expand "andsi3"
  [(set (match_operand:SI         0 "s_register_operand" "")
	(and:SI (match_operand:SI 1 "s_register_operand" "")
		(match_operand:SI 2 "reg_or_int_operand" "")))]
  "TARGET_EITHER"
  "
  if (TARGET_32BIT)
    {
      if (CONST_INT_P (operands[2]))
        {
	  if (INTVAL (operands[2]) == 255 && arm_arch6)
	    {
	      operands[1] = convert_to_mode (QImode, operands[1], 1);
	      emit_insn (gen_thumb2_zero_extendqisi2_v6 (operands[0],
							 operands[1]));
	    }
	  else
	    arm_split_constant (AND, SImode, NULL_RTX,
				INTVAL (operands[2]), operands[0],
				operands[1],
				optimize && can_create_pseudo_p ());

          DONE;
        }
    }
  else /* TARGET_THUMB1 */
    {
      if (!CONST_INT_P (operands[2]))
        {
          rtx tmp = force_reg (SImode, operands[2]);
	  if (rtx_equal_p (operands[0], operands[1]))
	    operands[2] = tmp;
	  else
	    {
              operands[2] = operands[1];
              operands[1] = tmp;
	    }
        }
      else
        {
          int i;
	  
          if (((unsigned HOST_WIDE_INT) ~INTVAL (operands[2])) < 256)
  	    {
	      operands[2] = force_reg (SImode,
				       GEN_INT (~INTVAL (operands[2])));
	      
	      emit_insn (gen_thumb1_bicsi3 (operands[0], operands[2], operands[1]));
	      
	      DONE;
	    }

          for (i = 9; i <= 31; i++)
	    {
	      if ((((HOST_WIDE_INT) 1) << i) - 1 == INTVAL (operands[2]))
	        {
	          emit_insn (gen_extzv (operands[0], operands[1], GEN_INT (i),
			 	        const0_rtx));
	          DONE;
	        }
	      else if ((((HOST_WIDE_INT) 1) << i) - 1
		       == ~INTVAL (operands[2]))
	        {
	          rtx shift = GEN_INT (i);
	          rtx reg = gen_reg_rtx (SImode);

	          emit_insn (gen_lshrsi3 (reg, operands[1], shift));
	          emit_insn (gen_ashlsi3 (operands[0], reg, shift));
		  
	          DONE;
	        }
	    }

          operands[2] = force_reg (SImode, operands[2]);
        }
    }
  "
)

; ??? Check split length for Thumb-2
//Pattern#143
(define_insn_and_split "*arm_andsi3_insn"
  [(set (match_operand:SI         0 "s_register_operand" "=r,r,r,r")
	(and:SI (match_operand:SI 1 "s_register_operand" "r,r,r,r")
		(match_operand:SI 2 "reg_or_int_operand" "I,K,r,?n")))]
  "TARGET_32BIT"
  "@
   and%?\\t%0, %1, %2
   bic%?\\t%0, %1, #%B2
   and%?\\t%0, %1, %2
   #"
  "TARGET_32BIT
   && CONST_INT_P (operands[2])
   && !(const_ok_for_arm (INTVAL (operands[2]))
	|| const_ok_for_arm (~INTVAL (operands[2])))"
  [(clobber (const_int 0))]
  "
  arm_split_constant  (AND, SImode, curr_insn, 
	               INTVAL (operands[2]), operands[0], operands[1], 0);
  DONE;
  "
  [(set_attr "length" "4,4,4,16")
   (set_attr "predicable" "yes")
   (set_attr "type" 	"simple_alu_imm,simple_alu_imm,*,simple_alu_imm")]
)

//Pattern#144
(define_insn "*thumb1_andsi3_insn"
  [(set (match_operand:SI         0 "register_operand" "=l")
	(and:SI (match_operand:SI 1 "register_operand" "%0")
		(match_operand:SI 2 "register_operand" "l")))]
  "TARGET_THUMB1"
  "and\\t%0, %2"
  [(set_attr "length" "2")
   (set_attr "type"  "simple_alu_imm")
   (set_attr "conds" "set")])

//Pattern#145
(define_insn "*andsi3_compare0"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV
	 (and:SI (match_operand:SI 1 "s_register_operand" "r,r,r")
		 (match_operand:SI 2 "arm_not_operand" "I,K,r"))
	 (const_int 0)))
   (set (match_operand:SI          0 "s_register_operand" "=r,r,r")
	(and:SI (match_dup 1) (match_dup 2)))]
  "TARGET_32BIT"
  "@
   and%.\\t%0, %1, %2
   bic%.\\t%0, %1, #%B2
   and%.\\t%0, %1, %2"
  [(set_attr "conds" "set")
   (set_attr "type" "simple_alu_imm,simple_alu_imm,*")]
)

//Pattern#146
(define_insn "*andsi3_compare0_scratch"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV
	 (and:SI (match_operand:SI 0 "s_register_operand" "r,r,r")
		 (match_operand:SI 1 "arm_not_operand" "I,K,r"))
	 (const_int 0)))
   (clobber (match_scratch:SI 2 "=X,r,X"))]
  "TARGET_32BIT"
  "@
   tst%?\\t%0, %1
   bic%.\\t%2, %0, #%B1
   tst%?\\t%0, %1"
  [(set_attr "conds" "set")
   (set_attr "type"  "simple_alu_imm,simple_alu_imm,*")]
)

//Pattern#147
(define_insn "*zeroextractsi_compare0_scratch"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV (zero_extract:SI
			  (match_operand:SI 0 "s_register_operand" "r")
		 	  (match_operand 1 "const_int_operand" "n")
			  (match_operand 2 "const_int_operand" "n"))
			 (const_int 0)))]
  "TARGET_32BIT
  && (INTVAL (operands[2]) >= 0 && INTVAL (operands[2]) < 32
      && INTVAL (operands[1]) > 0 
      && INTVAL (operands[1]) + (INTVAL (operands[2]) & 1) <= 8
      && INTVAL (operands[1]) + INTVAL (operands[2]) <= 32)"
  "*
  operands[1] = GEN_INT (((1 << INTVAL (operands[1])) - 1)
			 << INTVAL (operands[2]));
  output_asm_insn (\"tst%?\\t%0, %1\", operands);
  return \"\";
  "
  [(set_attr "conds" "set")
   (set_attr "predicable" "yes")
   (set_attr "type" "simple_alu_imm")]
)

//Pattern#148
(define_insn_and_split "*ne_zeroextractsi"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(ne:SI (zero_extract:SI
		(match_operand:SI 1 "s_register_operand" "r")
		(match_operand:SI 2 "const_int_operand" "n")
		(match_operand:SI 3 "const_int_operand" "n"))
	       (const_int 0)))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT
   && (INTVAL (operands[3]) >= 0 && INTVAL (operands[3]) < 32
       && INTVAL (operands[2]) > 0 
       && INTVAL (operands[2]) + (INTVAL (operands[3]) & 1) <= 8
       && INTVAL (operands[2]) + INTVAL (operands[3]) <= 32)"
  "#"
  "TARGET_32BIT
   && (INTVAL (operands[3]) >= 0 && INTVAL (operands[3]) < 32
       && INTVAL (operands[2]) > 0 
       && INTVAL (operands[2]) + (INTVAL (operands[3]) & 1) <= 8
       && INTVAL (operands[2]) + INTVAL (operands[3]) <= 32)"
  [(parallel [(set (reg:CC_NOOV CC_REGNUM)
		   (compare:CC_NOOV (and:SI (match_dup 1) (match_dup 2))
				    (const_int 0)))
	      (set (match_dup 0) (and:SI (match_dup 1) (match_dup 2)))])
   (set (match_dup 0)
	(if_then_else:SI (eq (reg:CC_NOOV CC_REGNUM) (const_int 0))
			 (match_dup 0) (const_int 1)))]
  "
  operands[2] = GEN_INT (((1 << INTVAL (operands[2])) - 1)
			 << INTVAL (operands[3])); 
  "
  [(set_attr "conds" "clob")
   (set (attr "length")
	(if_then_else (eq_attr "is_thumb" "yes")
		      (const_int 12)
		      (const_int 8)))]
)

//Pattern#149
(define_insn_and_split "*ne_zeroextractsi_shifted"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(ne:SI (zero_extract:SI
		(match_operand:SI 1 "s_register_operand" "r")
		(match_operand:SI 2 "const_int_operand" "n")
		(const_int 0))
	       (const_int 0)))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "#"
  "TARGET_ARM"
  [(parallel [(set (reg:CC_NOOV CC_REGNUM)
		   (compare:CC_NOOV (ashift:SI (match_dup 1) (match_dup 2))
				    (const_int 0)))
	      (set (match_dup 0) (ashift:SI (match_dup 1) (match_dup 2)))])
   (set (match_dup 0)
	(if_then_else:SI (eq (reg:CC_NOOV CC_REGNUM) (const_int 0))
			 (match_dup 0) (const_int 1)))]
  "
  operands[2] = GEN_INT (32 - INTVAL (operands[2]));
  "
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)

//Pattern#150
(define_insn_and_split "*ite_ne_zeroextractsi"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(if_then_else:SI (ne (zero_extract:SI
			      (match_operand:SI 1 "s_register_operand" "r")
			      (match_operand:SI 2 "const_int_operand" "n")
			      (match_operand:SI 3 "const_int_operand" "n"))
			     (const_int 0))
			 (match_operand:SI 4 "arm_not_operand" "rIK")
			 (const_int 0)))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM
   && (INTVAL (operands[3]) >= 0 && INTVAL (operands[3]) < 32
       && INTVAL (operands[2]) > 0 
       && INTVAL (operands[2]) + (INTVAL (operands[3]) & 1) <= 8
       && INTVAL (operands[2]) + INTVAL (operands[3]) <= 32)
   && !reg_overlap_mentioned_p (operands[0], operands[4])"
  "#"
  "TARGET_ARM
   && (INTVAL (operands[3]) >= 0 && INTVAL (operands[3]) < 32
       && INTVAL (operands[2]) > 0 
       && INTVAL (operands[2]) + (INTVAL (operands[3]) & 1) <= 8
       && INTVAL (operands[2]) + INTVAL (operands[3]) <= 32)
   && !reg_overlap_mentioned_p (operands[0], operands[4])"
  [(parallel [(set (reg:CC_NOOV CC_REGNUM)
		   (compare:CC_NOOV (and:SI (match_dup 1) (match_dup 2))
				    (const_int 0)))
	      (set (match_dup 0) (and:SI (match_dup 1) (match_dup 2)))])
   (set (match_dup 0)
	(if_then_else:SI (eq (reg:CC_NOOV CC_REGNUM) (const_int 0))
			 (match_dup 0) (match_dup 4)))]
  "
  operands[2] = GEN_INT (((1 << INTVAL (operands[2])) - 1)
			 << INTVAL (operands[3])); 
  "
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)

//Pattern#151
(define_insn_and_split "*ite_ne_zeroextractsi_shifted"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(if_then_else:SI (ne (zero_extract:SI
			      (match_operand:SI 1 "s_register_operand" "r")
			      (match_operand:SI 2 "const_int_operand" "n")
			      (const_int 0))
			     (const_int 0))
			 (match_operand:SI 3 "arm_not_operand" "rIK")
			 (const_int 0)))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM && !reg_overlap_mentioned_p (operands[0], operands[3])"
  "#"
  "TARGET_ARM && !reg_overlap_mentioned_p (operands[0], operands[3])"
  [(parallel [(set (reg:CC_NOOV CC_REGNUM)
		   (compare:CC_NOOV (ashift:SI (match_dup 1) (match_dup 2))
				    (const_int 0)))
	      (set (match_dup 0) (ashift:SI (match_dup 1) (match_dup 2)))])
   (set (match_dup 0)
	(if_then_else:SI (eq (reg:CC_NOOV CC_REGNUM) (const_int 0))
			 (match_dup 0) (match_dup 3)))]
  "
  operands[2] = GEN_INT (32 - INTVAL (operands[2]));
  "
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)

//Pattern#152
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(zero_extract:SI (match_operand:SI 1 "s_register_operand" "")
			 (match_operand:SI 2 "const_int_operand" "")
			 (match_operand:SI 3 "const_int_operand" "")))
   (clobber (match_operand:SI 4 "s_register_operand" ""))]
  "TARGET_THUMB1"
  [(set (match_dup 4) (ashift:SI (match_dup 1) (match_dup 2)))
   (set (match_dup 0) (lshiftrt:SI (match_dup 4) (match_dup 3)))]
  "{
     HOST_WIDE_INT temp = INTVAL (operands[2]);

     operands[2] = GEN_INT (32 - temp - INTVAL (operands[3]));
     operands[3] = GEN_INT (32 - temp);
   }"
)

;; ??? Use Thumb-2 has bitfield insert/extract instructions.
//Pattern#153
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(match_operator:SI 1 "shiftable_operator"
	 [(zero_extract:SI (match_operand:SI 2 "s_register_operand" "")
			   (match_operand:SI 3 "const_int_operand" "")
			   (match_operand:SI 4 "const_int_operand" ""))
	  (match_operand:SI 5 "s_register_operand" "")]))
   (clobber (match_operand:SI 6 "s_register_operand" ""))]
  "TARGET_ARM"
  [(set (match_dup 6) (ashift:SI (match_dup 2) (match_dup 3)))
   (set (match_dup 0)
	(match_op_dup 1
	 [(lshiftrt:SI (match_dup 6) (match_dup 4))
	  (match_dup 5)]))]
  "{
     HOST_WIDE_INT temp = INTVAL (operands[3]);

     operands[3] = GEN_INT (32 - temp - INTVAL (operands[4]));
     operands[4] = GEN_INT (32 - temp);
   }"
)
  
//Pattern#154
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(sign_extract:SI (match_operand:SI 1 "s_register_operand" "")
			 (match_operand:SI 2 "const_int_operand" "")
			 (match_operand:SI 3 "const_int_operand" "")))]
  "TARGET_THUMB1"
  [(set (match_dup 0) (ashift:SI (match_dup 1) (match_dup 2)))
   (set (match_dup 0) (ashiftrt:SI (match_dup 0) (match_dup 3)))]
  "{
     HOST_WIDE_INT temp = INTVAL (operands[2]);

     operands[2] = GEN_INT (32 - temp - INTVAL (operands[3]));
     operands[3] = GEN_INT (32 - temp);
   }"
)

//Pattern#155
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(match_operator:SI 1 "shiftable_operator"
	 [(sign_extract:SI (match_operand:SI 2 "s_register_operand" "")
			   (match_operand:SI 3 "const_int_operand" "")
			   (match_operand:SI 4 "const_int_operand" ""))
	  (match_operand:SI 5 "s_register_operand" "")]))
   (clobber (match_operand:SI 6 "s_register_operand" ""))]
  "TARGET_ARM"
  [(set (match_dup 6) (ashift:SI (match_dup 2) (match_dup 3)))
   (set (match_dup 0)
	(match_op_dup 1
	 [(ashiftrt:SI (match_dup 6) (match_dup 4))
	  (match_dup 5)]))]
  "{
     HOST_WIDE_INT temp = INTVAL (operands[3]);

     operands[3] = GEN_INT (32 - temp - INTVAL (operands[4]));
     operands[4] = GEN_INT (32 - temp);
   }"
)
  
;;; ??? This pattern is bogus.  If operand3 has bits outside the range
;;; represented by the bitfield, then this will produce incorrect results.
;;; Somewhere, the value needs to be truncated.  On targets like the m68k,
;;; which have a real bit-field insert instruction, the truncation happens
;;; in the bit-field insert instruction itself.  Since arm does not have a
;;; bit-field insert instruction, we would have to emit code here to truncate
;;; the value before we insert.  This loses some of the advantage of having
;;; this insv pattern, so this pattern needs to be reevalutated.

//Pattern#156
(define_expand "insv"
  [(set (zero_extract (match_operand 0 "nonimmediate_operand" "")
                      (match_operand 1 "general_operand" "")
                      (match_operand 2 "general_operand" ""))
        (match_operand 3 "reg_or_int_operand" ""))]
  "TARGET_ARM || arm_arch_thumb2"
  "
  {
    int start_bit = INTVAL (operands[2]);
    int width = INTVAL (operands[1]);
    HOST_WIDE_INT mask = (((HOST_WIDE_INT)1) << width) - 1;
    rtx target, subtarget;

    if (arm_arch_thumb2)
      {
        if (unaligned_access && MEM_P (operands[0])
	    && s_register_operand (operands[3], GET_MODE (operands[3]))
	    && (width == 16 || width == 32) && (start_bit % BITS_PER_UNIT) == 0)
	  {
	    rtx base_addr;

	    if (BYTES_BIG_ENDIAN)
	      start_bit = GET_MODE_BITSIZE (GET_MODE (operands[3])) - width
			  - start_bit;

	    if (width == 32)
	      {
	        base_addr = adjust_address (operands[0], SImode,
					    start_bit / BITS_PER_UNIT);
		emit_insn (gen_unaligned_storesi (base_addr, operands[3]));
	      }
	    else
	      {
	        rtx tmp = gen_reg_rtx (HImode);

	        base_addr = adjust_address (operands[0], HImode,
					    start_bit / BITS_PER_UNIT);
		emit_move_insn (tmp, gen_lowpart (HImode, operands[3]));
		emit_insn (gen_unaligned_storehi (base_addr, tmp));
	      }
	    DONE;
	  }
	else if (s_register_operand (operands[0], GET_MODE (operands[0])))
	  {
	    bool use_bfi = TRUE;

	    if (CONST_INT_P (operands[3]))
	      {
		HOST_WIDE_INT val = INTVAL (operands[3]) & mask;

		if (val == 0)
		  {
		    emit_insn (gen_insv_zero (operands[0], operands[1],
					      operands[2]));
		    DONE;
		  }

		/* See if the set can be done with a single orr instruction.  */
		if (val == mask && const_ok_for_arm (val << start_bit))
		  use_bfi = FALSE;
	      }

	    if (use_bfi)
	      {
		if (!REG_P (operands[3]))
		  operands[3] = force_reg (SImode, operands[3]);

		emit_insn (gen_insv_t2 (operands[0], operands[1], operands[2],
					operands[3]));
		DONE;
	      }
	  }
	else
	  FAIL;
      }

    if (!s_register_operand (operands[0], GET_MODE (operands[0])))
      FAIL;

    target = copy_rtx (operands[0]);
    /* Avoid using a subreg as a subtarget, and avoid writing a paradoxical 
       subreg as the final target.  */
    if (GET_CODE (target) == SUBREG)
      {
	subtarget = gen_reg_rtx (SImode);
	if (GET_MODE_SIZE (GET_MODE (SUBREG_REG (target)))
	    < GET_MODE_SIZE (SImode))
	  target = SUBREG_REG (target);
      }
    else
      subtarget = target;    

    if (CONST_INT_P (operands[3]))
      {
	/* Since we are inserting a known constant, we may be able to
	   reduce the number of bits that we have to clear so that
	   the mask becomes simple.  */
	/* ??? This code does not check to see if the new mask is actually
	   simpler.  It may not be.  */
	rtx op1 = gen_reg_rtx (SImode);
	/* ??? Truncate operand3 to fit in the bitfield.  See comment before
	   start of this pattern.  */
	HOST_WIDE_INT op3_value = mask & INTVAL (operands[3]);
	HOST_WIDE_INT mask2 = ((mask & ~op3_value) << start_bit);

	emit_insn (gen_andsi3 (op1, operands[0],
			       gen_int_mode (~mask2, SImode)));
	emit_insn (gen_iorsi3 (subtarget, op1,
			       gen_int_mode (op3_value << start_bit, SImode)));
      }
    else if (start_bit == 0
	     && !(const_ok_for_arm (mask)
		  || const_ok_for_arm (~mask)))
      {
	/* A Trick, since we are setting the bottom bits in the word,
	   we can shift operand[3] up, operand[0] down, OR them together
	   and rotate the result back again.  This takes 3 insns, and
	   the third might be mergeable into another op.  */
	/* The shift up copes with the possibility that operand[3] is
           wider than the bitfield.  */
	rtx op0 = gen_reg_rtx (SImode);
	rtx op1 = gen_reg_rtx (SImode);

	emit_insn (gen_ashlsi3 (op0, operands[3], GEN_INT (32 - width)));
	emit_insn (gen_lshrsi3 (op1, operands[0], operands[1]));
	emit_insn (gen_iorsi3  (op1, op1, op0));
	emit_insn (gen_rotlsi3 (subtarget, op1, operands[1]));
      }
    else if ((width + start_bit == 32)
	     && !(const_ok_for_arm (mask)
		  || const_ok_for_arm (~mask)))
      {
	/* Similar trick, but slightly less efficient.  */

	rtx op0 = gen_reg_rtx (SImode);
	rtx op1 = gen_reg_rtx (SImode);

	emit_insn (gen_ashlsi3 (op0, operands[3], GEN_INT (32 - width)));
	emit_insn (gen_ashlsi3 (op1, operands[0], operands[1]));
	emit_insn (gen_lshrsi3 (op1, op1, operands[1]));
	emit_insn (gen_iorsi3 (subtarget, op1, op0));
      }
    else
      {
	rtx op0 = gen_int_mode (mask, SImode);
	rtx op1 = gen_reg_rtx (SImode);
	rtx op2 = gen_reg_rtx (SImode);

	if (!(const_ok_for_arm (mask) || const_ok_for_arm (~mask)))
	  {
	    rtx tmp = gen_reg_rtx (SImode);

	    emit_insn (gen_movsi (tmp, op0));
	    op0 = tmp;
	  }

	/* Mask out any bits in operand[3] that are not needed.  */
	   emit_insn (gen_andsi3 (op1, operands[3], op0));

	if (CONST_INT_P (op0)
	    && (const_ok_for_arm (mask << start_bit)
		|| const_ok_for_arm (~(mask << start_bit))))
	  {
	    op0 = gen_int_mode (~(mask << start_bit), SImode);
	    emit_insn (gen_andsi3 (op2, operands[0], op0));
	  }
	else
	  {
	    if (CONST_INT_P (op0))
	      {
		rtx tmp = gen_reg_rtx (SImode);

		emit_insn (gen_movsi (tmp, op0));
		op0 = tmp;
	      }

	    if (start_bit != 0)
	      emit_insn (gen_ashlsi3 (op0, op0, operands[2]));
	    
	    emit_insn (gen_andsi_notsi_si (op2, operands[0], op0));
	  }

	if (start_bit != 0)
          emit_insn (gen_ashlsi3 (op1, op1, operands[2]));

	emit_insn (gen_iorsi3 (subtarget, op1, op2));
      }

    if (subtarget != target)
      {
	/* If TARGET is still a SUBREG, then it must be wider than a word,
	   so we must be careful only to set the subword we were asked to.  */
	if (GET_CODE (target) == SUBREG)
	  emit_move_insn (target, subtarget);
	else
	  emit_move_insn (target, gen_lowpart (GET_MODE (target), subtarget));
      }

    DONE;
  }"
)

//Pattern#157
(define_insn "insv_zero"
  [(set (zero_extract:SI (match_operand:SI 0 "s_register_operand" "+r")
                         (match_operand:SI 1 "const_int_operand" "M")
                         (match_operand:SI 2 "const_int_operand" "M"))
        (const_int 0))]
  "arm_arch_thumb2"
  "bfc%?\t%0, %2, %1"
  [(set_attr "length" "4")
   (set_attr "predicable" "yes")]
)

//Pattern#158
(define_insn "insv_t2"
  [(set (zero_extract:SI (match_operand:SI 0 "s_register_operand" "+r")
                         (match_operand:SI 1 "const_int_operand" "M")
                         (match_operand:SI 2 "const_int_operand" "M"))
        (match_operand:SI 3 "s_register_operand" "r"))]
  "arm_arch_thumb2"
  "bfi%?\t%0, %3, %2, %1"
  [(set_attr "length" "4")
   (set_attr "predicable" "yes")]
)

; constants for op 2 will never be given to these patterns.
//Pattern#159
(define_insn_and_split "*anddi_notdi_di"
  [(set (match_operand:DI 0 "s_register_operand" "=&r,&r")
	(and:DI (not:DI (match_operand:DI 1 "s_register_operand" "0,r"))
		(match_operand:DI 2 "s_register_operand" "r,0")))]
  "TARGET_32BIT"
  "#"
  "TARGET_32BIT && reload_completed
   && ! (TARGET_NEON && IS_VFP_REGNUM (REGNO (operands[0])))
   && ! IS_IWMMXT_REGNUM (REGNO (operands[0]))"
  [(set (match_dup 0) (and:SI (not:SI (match_dup 1)) (match_dup 2)))
   (set (match_dup 3) (and:SI (not:SI (match_dup 4)) (match_dup 5)))]
  "
  {
    operands[3] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[4] = gen_highpart (SImode, operands[1]);
    operands[1] = gen_lowpart (SImode, operands[1]);
    operands[5] = gen_highpart (SImode, operands[2]);
    operands[2] = gen_lowpart (SImode, operands[2]);
  }"
  [(set_attr "length" "8")
   (set_attr "predicable" "yes")]
)
  
//Pattern#160
(define_insn_and_split "*anddi_notzesidi_di"
  [(set (match_operand:DI 0 "s_register_operand" "=&r,&r")
	(and:DI (not:DI (zero_extend:DI
			 (match_operand:SI 2 "s_register_operand" "r,r")))
		(match_operand:DI 1 "s_register_operand" "0,?r")))]
  "TARGET_32BIT"
  "@
   bic%?\\t%Q0, %Q1, %2
   #"
  ; (not (zero_extend ...)) allows us to just copy the high word from
  ; operand1 to operand0.
  "TARGET_32BIT
   && reload_completed
   && operands[0] != operands[1]"
  [(set (match_dup 0) (and:SI (not:SI (match_dup 2)) (match_dup 1)))
   (set (match_dup 3) (match_dup 4))]
  "
  {
    operands[3] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[4] = gen_highpart (SImode, operands[1]);
    operands[1] = gen_lowpart (SImode, operands[1]);
  }"
  [(set_attr "length" "4,8")
   (set_attr "predicable" "yes")]
)
  
//Pattern#161
(define_insn_and_split "*anddi_notsesidi_di"
  [(set (match_operand:DI 0 "s_register_operand" "=&r,&r")
	(and:DI (not:DI (sign_extend:DI
			 (match_operand:SI 2 "s_register_operand" "r,r")))
		(match_operand:DI 1 "s_register_operand" "0,r")))]
  "TARGET_32BIT"
  "#"
  "TARGET_32BIT && reload_completed"
  [(set (match_dup 0) (and:SI (not:SI (match_dup 2)) (match_dup 1)))
   (set (match_dup 3) (and:SI (not:SI
				(ashiftrt:SI (match_dup 2) (const_int 31)))
			       (match_dup 4)))]
  "
  {
    operands[3] = gen_highpart (SImode, operands[0]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[4] = gen_highpart (SImode, operands[1]);
    operands[1] = gen_lowpart (SImode, operands[1]);
  }"
  [(set_attr "length" "8")
   (set_attr "predicable" "yes")]
)
  
//Pattern#162
(define_insn "andsi_notsi_si"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(and:SI (not:SI (match_operand:SI 2 "s_register_operand" "r"))
		(match_operand:SI 1 "s_register_operand" "r")))]
  "TARGET_32BIT"
  "bic%?\\t%0, %1, %2"
  [(set_attr "predicable" "yes")]
)

//Pattern#163
(define_insn "thumb1_bicsi3"
  [(set (match_operand:SI                 0 "register_operand" "=l")
	(and:SI (not:SI (match_operand:SI 1 "register_operand" "l"))
		(match_operand:SI         2 "register_operand" "0")))]
  "TARGET_THUMB1"
  "bic\\t%0, %1"
  [(set_attr "length" "2")
   (set_attr "conds" "set")])

//Pattern#164
(define_insn "andsi_not_shiftsi_si"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(and:SI (not:SI (match_operator:SI 4 "shift_operator"
			 [(match_operand:SI 2 "s_register_operand" "r")
			  (match_operand:SI 3 "arm_rhs_operand" "rM")]))
		(match_operand:SI 1 "s_register_operand" "r")))]
  "TARGET_ARM"
  "bic%?\\t%0, %1, %2%S4"
  [(set_attr "predicable" "yes")
   (set_attr "shift" "2")
   (set (attr "type") (if_then_else (match_operand 3 "const_int_operand" "")
		      (const_string "alu_shift")
		      (const_string "alu_shift_reg")))]
)

//Pattern#165
(define_insn "*andsi_notsi_si_compare0"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV
	 (and:SI (not:SI (match_operand:SI 2 "s_register_operand" "r"))
		 (match_operand:SI 1 "s_register_operand" "r"))
	 (const_int 0)))
   (set (match_operand:SI 0 "s_register_operand" "=r")
	(and:SI (not:SI (match_dup 2)) (match_dup 1)))]
  "TARGET_32BIT"
  "bic%.\\t%0, %1, %2"
  [(set_attr "conds" "set")]
)

//Pattern#166
(define_insn "*andsi_notsi_si_compare0_scratch"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV
	 (and:SI (not:SI (match_operand:SI 2 "s_register_operand" "r"))
		 (match_operand:SI 1 "s_register_operand" "r"))
	 (const_int 0)))
   (clobber (match_scratch:SI 0 "=r"))]
  "TARGET_32BIT"
  "bic%.\\t%0, %1, %2"
  [(set_attr "conds" "set")]
)

//Pattern#167
(define_expand "iordi3"
  [(set (match_operand:DI         0 "s_register_operand" "")
	(ior:DI (match_operand:DI 1 "s_register_operand" "")
		(match_operand:DI 2 "neon_logic_op2" "")))]
  "TARGET_32BIT"
  ""
)

//Pattern#168
(define_insn "*iordi3_insn"
  [(set (match_operand:DI         0 "s_register_operand" "=&r,&r")
	(ior:DI (match_operand:DI 1 "s_register_operand"  "%0,r")
		(match_operand:DI 2 "s_register_operand"   "r,r")))]
  "TARGET_32BIT && !TARGET_IWMMXT && !TARGET_NEON"
  "#"
  [(set_attr "length" "8")
   (set_attr "predicable" "yes")]
)

//Pattern#169
(define_insn "*iordi_zesidi_di"
  [(set (match_operand:DI 0 "s_register_operand" "=&r,&r")
	(ior:DI (zero_extend:DI
		 (match_operand:SI 2 "s_register_operand" "r,r"))
		(match_operand:DI 1 "s_register_operand" "0,?r")))]
  "TARGET_32BIT"
  "@
   orr%?\\t%Q0, %Q1, %2
   #"
  [(set_attr "length" "4,8")
   (set_attr "predicable" "yes")]
)

//Pattern#170
(define_insn "*iordi_sesidi_di"
  [(set (match_operand:DI 0 "s_register_operand" "=&r,&r")
	(ior:DI (sign_extend:DI
		 (match_operand:SI 2 "s_register_operand" "r,r"))
		(match_operand:DI 1 "s_register_operand" "0,r")))]
  "TARGET_32BIT"
  "#"
  [(set_attr "length" "8")
   (set_attr "predicable" "yes")]
)

//Pattern#171
(define_expand "iorsi3"
  [(set (match_operand:SI         0 "s_register_operand" "")
	(ior:SI (match_operand:SI 1 "s_register_operand" "")
		(match_operand:SI 2 "reg_or_int_operand" "")))]
  "TARGET_EITHER"
  "
  if (CONST_INT_P (operands[2]))
    {
      if (TARGET_32BIT)
        {
          arm_split_constant (IOR, SImode, NULL_RTX,
	                      INTVAL (operands[2]), operands[0], operands[1],
			      optimize && can_create_pseudo_p ());
          DONE;
	}
      else /* TARGET_THUMB1 */
        {
          rtx tmp = force_reg (SImode, operands[2]);
	  if (rtx_equal_p (operands[0], operands[1]))
	    operands[2] = tmp;
	  else
	    {
              operands[2] = operands[1];
              operands[1] = tmp;
	    }
        }
    }
  "
)

//Pattern#172
(define_insn_and_split "*iorsi3_insn"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r,r,r")
	(ior:SI (match_operand:SI 1 "s_register_operand" "%r,r,r,r")
		(match_operand:SI 2 "reg_or_int_operand" "I,K,r,?n")))]
  "TARGET_32BIT"
  "@
   orr%?\\t%0, %1, %2
   orn%?\\t%0, %1, #%B2
   orr%?\\t%0, %1, %2
   #"
  "TARGET_32BIT
   && CONST_INT_P (operands[2])
   && !(const_ok_for_arm (INTVAL (operands[2]))
        || (TARGET_THUMB2 && const_ok_for_arm (~INTVAL (operands[2]))))"
  [(clobber (const_int 0))]
{
  arm_split_constant (IOR, SImode, curr_insn, 
                      INTVAL (operands[2]), operands[0], operands[1], 0);
  DONE;
}
  [(set_attr "length" "4,4,4,16")
   (set_attr "arch" "32,t2,32,32")
   (set_attr "predicable" "yes")
   (set_attr "type" "simple_alu_imm,simple_alu_imm,*,*")]
)

//Pattern#173
(define_insn "*thumb1_iorsi3_insn"
  [(set (match_operand:SI         0 "register_operand" "=l")
	(ior:SI (match_operand:SI 1 "register_operand" "%0")
		(match_operand:SI 2 "register_operand" "l")))]
  "TARGET_THUMB1"
  "orr\\t%0, %2"
  [(set_attr "length" "2")
   (set_attr "conds" "set")])

//Pattern#174
(define_peephole2
  [(match_scratch:SI 3 "r")
   (set (match_operand:SI 0 "arm_general_register_operand" "")
	(ior:SI (match_operand:SI 1 "arm_general_register_operand" "")
		(match_operand:SI 2 "const_int_operand" "")))]
  "TARGET_ARM
   && !const_ok_for_arm (INTVAL (operands[2]))
   && const_ok_for_arm (~INTVAL (operands[2]))"
  [(set (match_dup 3) (match_dup 2))
   (set (match_dup 0) (ior:SI (match_dup 1) (match_dup 3)))]
  ""
)

//Pattern#175
(define_insn "*iorsi3_compare0"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV (ior:SI (match_operand:SI 1 "s_register_operand" "%r,r")
				 (match_operand:SI 2 "arm_rhs_operand" "I,r"))
			 (const_int 0)))
   (set (match_operand:SI 0 "s_register_operand" "=r,r")
	(ior:SI (match_dup 1) (match_dup 2)))]
  "TARGET_32BIT"
  "orr%.\\t%0, %1, %2"
  [(set_attr "conds" "set")
   (set_attr "type" "simple_alu_imm,*")]
)

//Pattern#176
(define_insn "*iorsi3_compare0_scratch"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV (ior:SI (match_operand:SI 1 "s_register_operand" "%r,r")
				 (match_operand:SI 2 "arm_rhs_operand" "I,r"))
			 (const_int 0)))
   (clobber (match_scratch:SI 0 "=r,r"))]
  "TARGET_32BIT"
  "orr%.\\t%0, %1, %2"
  [(set_attr "conds" "set")
   (set_attr "type" "simple_alu_imm, *")]
)

//Pattern#177
(define_expand "xordi3"
  [(set (match_operand:DI         0 "s_register_operand" "")
	(xor:DI (match_operand:DI 1 "s_register_operand" "")
		(match_operand:DI 2 "s_register_operand" "")))]
  "TARGET_32BIT"
  ""
)

//Pattern#178
(define_insn "*xordi3_insn"
  [(set (match_operand:DI         0 "s_register_operand" "=&r,&r")
	(xor:DI (match_operand:DI 1 "s_register_operand"  "%0,r")
		(match_operand:DI 2 "s_register_operand"   "r,r")))]
  "TARGET_32BIT && !TARGET_IWMMXT && !TARGET_NEON"
  "#"
  [(set_attr "length" "8")
   (set_attr "predicable" "yes")]
)

//Pattern#179
(define_insn "*xordi_zesidi_di"
  [(set (match_operand:DI 0 "s_register_operand" "=&r,&r")
	(xor:DI (zero_extend:DI
		 (match_operand:SI 2 "s_register_operand" "r,r"))
		(match_operand:DI 1 "s_register_operand" "0,?r")))]
  "TARGET_32BIT"
  "@
   eor%?\\t%Q0, %Q1, %2
   #"
  [(set_attr "length" "4,8")
   (set_attr "predicable" "yes")]
)

//Pattern#180
(define_insn "*xordi_sesidi_di"
  [(set (match_operand:DI 0 "s_register_operand" "=&r,&r")
	(xor:DI (sign_extend:DI
		 (match_operand:SI 2 "s_register_operand" "r,r"))
		(match_operand:DI 1 "s_register_operand" "0,r")))]
  "TARGET_32BIT"
  "#"
  [(set_attr "length" "8")
   (set_attr "predicable" "yes")]
)

//Pattern#181
(define_expand "xorsi3"
  [(set (match_operand:SI         0 "s_register_operand" "")
	(xor:SI (match_operand:SI 1 "s_register_operand" "")
		(match_operand:SI 2 "reg_or_int_operand" "")))]
  "TARGET_EITHER"
  "if (CONST_INT_P (operands[2]))
    {
      if (TARGET_32BIT)
        {
          arm_split_constant (XOR, SImode, NULL_RTX,
	                      INTVAL (operands[2]), operands[0], operands[1],
			      optimize && can_create_pseudo_p ());
          DONE;
	}
      else /* TARGET_THUMB1 */
        {
          rtx tmp = force_reg (SImode, operands[2]);
	  if (rtx_equal_p (operands[0], operands[1]))
	    operands[2] = tmp;
	  else
	    {
              operands[2] = operands[1];
              operands[1] = tmp;
	    }
        }
    }"
)

//Pattern#182
(define_insn_and_split "*arm_xorsi3"
  [(set (match_operand:SI         0 "s_register_operand" "=r,r,r")
	(xor:SI (match_operand:SI 1 "s_register_operand" "%r,r,r")
		(match_operand:SI 2 "reg_or_int_operand" "I,r,?n")))]
  "TARGET_32BIT"
  "@
   eor%?\\t%0, %1, %2
   eor%?\\t%0, %1, %2
   #"
  "TARGET_32BIT
   && CONST_INT_P (operands[2])
   && !const_ok_for_arm (INTVAL (operands[2]))"
  [(clobber (const_int 0))]
{
  arm_split_constant (XOR, SImode, curr_insn,
                      INTVAL (operands[2]), operands[0], operands[1], 0);
  DONE;
}
  [(set_attr "length" "4,4,16")
   (set_attr "predicable" "yes")
   (set_attr "type"  "simple_alu_imm,*,*")]
)

//Pattern#183
(define_insn "*thumb1_xorsi3_insn"
  [(set (match_operand:SI         0 "register_operand" "=l")
	(xor:SI (match_operand:SI 1 "register_operand" "%0")
		(match_operand:SI 2 "register_operand" "l")))]
  "TARGET_THUMB1"
  "eor\\t%0, %2"
  [(set_attr "length" "2")
   (set_attr "conds" "set")
   (set_attr "type" "simple_alu_imm")]
)

//Pattern#184
(define_insn "*xorsi3_compare0"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV (xor:SI (match_operand:SI 1 "s_register_operand" "r,r")
				 (match_operand:SI 2 "arm_rhs_operand" "I,r"))
			 (const_int 0)))
   (set (match_operand:SI 0 "s_register_operand" "=r,r")
	(xor:SI (match_dup 1) (match_dup 2)))]
  "TARGET_32BIT"
  "eor%.\\t%0, %1, %2"
  [(set_attr "conds" "set")
   (set_attr "type" "simple_alu_imm,*")]
)

//Pattern#185
(define_insn "*xorsi3_compare0_scratch"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV (xor:SI (match_operand:SI 0 "s_register_operand" "r,r")
				 (match_operand:SI 1 "arm_rhs_operand" "I,r"))
			 (const_int 0)))]
  "TARGET_32BIT"
  "teq%?\\t%0, %1"
  [(set_attr "conds" "set")
   (set_attr "type" "simple_alu_imm, *")]
)

; By splitting (IOR (AND (NOT A) (NOT B)) C) as D = AND (IOR A B) (NOT C), 
; (NOT D) we can sometimes merge the final NOT into one of the following
; insns.

//Pattern#186
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(ior:SI (and:SI (not:SI (match_operand:SI 1 "s_register_operand" ""))
			(not:SI (match_operand:SI 2 "arm_rhs_operand" "")))
		(match_operand:SI 3 "arm_rhs_operand" "")))
   (clobber (match_operand:SI 4 "s_register_operand" ""))]
  "TARGET_32BIT"
  [(set (match_dup 4) (and:SI (ior:SI (match_dup 1) (match_dup 2))
			      (not:SI (match_dup 3))))
   (set (match_dup 0) (not:SI (match_dup 4)))]
  ""
)

//Pattern#187
(define_insn "*andsi_iorsi3_notsi"
  [(set (match_operand:SI 0 "s_register_operand" "=&r,&r,&r")
	(and:SI (ior:SI (match_operand:SI 1 "s_register_operand" "%0,r,r")
			(match_operand:SI 2 "arm_rhs_operand" "rI,0,rI"))
		(not:SI (match_operand:SI 3 "arm_rhs_operand" "rI,rI,rI"))))]
  "TARGET_32BIT"
  "orr%?\\t%0, %1, %2\;bic%?\\t%0, %0, %3"
  [(set_attr "length" "8")
   (set_attr "ce_count" "2")
   (set_attr "predicable" "yes")]
)

; ??? Are these four splitters still beneficial when the Thumb-2 bitfield
; insns are available?
//Pattern#188
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(match_operator:SI 1 "logical_binary_operator"
	 [(zero_extract:SI (match_operand:SI 2 "s_register_operand" "")
			   (match_operand:SI 3 "const_int_operand" "")
			   (match_operand:SI 4 "const_int_operand" ""))
	  (match_operator:SI 9 "logical_binary_operator"
	   [(lshiftrt:SI (match_operand:SI 5 "s_register_operand" "")
			 (match_operand:SI 6 "const_int_operand" ""))
	    (match_operand:SI 7 "s_register_operand" "")])]))
   (clobber (match_operand:SI 8 "s_register_operand" ""))]
  "TARGET_32BIT
   && GET_CODE (operands[1]) == GET_CODE (operands[9])
   && INTVAL (operands[3]) == 32 - INTVAL (operands[6])"
  [(set (match_dup 8)
	(match_op_dup 1
	 [(ashift:SI (match_dup 2) (match_dup 4))
	  (match_dup 5)]))
   (set (match_dup 0)
	(match_op_dup 1
	 [(lshiftrt:SI (match_dup 8) (match_dup 6))
	  (match_dup 7)]))]
  "
  operands[4] = GEN_INT (32 - (INTVAL (operands[3]) + INTVAL (operands[4])));
")

//Pattern#189
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(match_operator:SI 1 "logical_binary_operator"
	 [(match_operator:SI 9 "logical_binary_operator"
	   [(lshiftrt:SI (match_operand:SI 5 "s_register_operand" "")
			 (match_operand:SI 6 "const_int_operand" ""))
	    (match_operand:SI 7 "s_register_operand" "")])
	  (zero_extract:SI (match_operand:SI 2 "s_register_operand" "")
			   (match_operand:SI 3 "const_int_operand" "")
			   (match_operand:SI 4 "const_int_operand" ""))]))
   (clobber (match_operand:SI 8 "s_register_operand" ""))]
  "TARGET_32BIT
   && GET_CODE (operands[1]) == GET_CODE (operands[9])
   && INTVAL (operands[3]) == 32 - INTVAL (operands[6])"
  [(set (match_dup 8)
	(match_op_dup 1
	 [(ashift:SI (match_dup 2) (match_dup 4))
	  (match_dup 5)]))
   (set (match_dup 0)
	(match_op_dup 1
	 [(lshiftrt:SI (match_dup 8) (match_dup 6))
	  (match_dup 7)]))]
  "
  operands[4] = GEN_INT (32 - (INTVAL (operands[3]) + INTVAL (operands[4])));
")

//Pattern#190
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(match_operator:SI 1 "logical_binary_operator"
	 [(sign_extract:SI (match_operand:SI 2 "s_register_operand" "")
			   (match_operand:SI 3 "const_int_operand" "")
			   (match_operand:SI 4 "const_int_operand" ""))
	  (match_operator:SI 9 "logical_binary_operator"
	   [(ashiftrt:SI (match_operand:SI 5 "s_register_operand" "")
			 (match_operand:SI 6 "const_int_operand" ""))
	    (match_operand:SI 7 "s_register_operand" "")])]))
   (clobber (match_operand:SI 8 "s_register_operand" ""))]
  "TARGET_32BIT
   && GET_CODE (operands[1]) == GET_CODE (operands[9])
   && INTVAL (operands[3]) == 32 - INTVAL (operands[6])"
  [(set (match_dup 8)
	(match_op_dup 1
	 [(ashift:SI (match_dup 2) (match_dup 4))
	  (match_dup 5)]))
   (set (match_dup 0)
	(match_op_dup 1
	 [(ashiftrt:SI (match_dup 8) (match_dup 6))
	  (match_dup 7)]))]
  "
  operands[4] = GEN_INT (32 - (INTVAL (operands[3]) + INTVAL (operands[4])));
")

//Pattern#191
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(match_operator:SI 1 "logical_binary_operator"
	 [(match_operator:SI 9 "logical_binary_operator"
	   [(ashiftrt:SI (match_operand:SI 5 "s_register_operand" "")
			 (match_operand:SI 6 "const_int_operand" ""))
	    (match_operand:SI 7 "s_register_operand" "")])
	  (sign_extract:SI (match_operand:SI 2 "s_register_operand" "")
			   (match_operand:SI 3 "const_int_operand" "")
			   (match_operand:SI 4 "const_int_operand" ""))]))
   (clobber (match_operand:SI 8 "s_register_operand" ""))]
  "TARGET_32BIT
   && GET_CODE (operands[1]) == GET_CODE (operands[9])
   && INTVAL (operands[3]) == 32 - INTVAL (operands[6])"
  [(set (match_dup 8)
	(match_op_dup 1
	 [(ashift:SI (match_dup 2) (match_dup 4))
	  (match_dup 5)]))
   (set (match_dup 0)
	(match_op_dup 1
	 [(ashiftrt:SI (match_dup 8) (match_dup 6))
	  (match_dup 7)]))]
  "
  operands[4] = GEN_INT (32 - (INTVAL (operands[3]) + INTVAL (operands[4])));
")


;; Minimum and maximum insns

//Pattern#192
(define_expand "smaxsi3"
  [(parallel [
    (set (match_operand:SI 0 "s_register_operand" "")
	 (smax:SI (match_operand:SI 1 "s_register_operand" "")
		  (match_operand:SI 2 "arm_rhs_operand" "")))
    (clobber (reg:CC CC_REGNUM))])]
  "TARGET_32BIT"
  "
  if (operands[2] == const0_rtx || operands[2] == constm1_rtx)
    {
      /* No need for a clobber of the condition code register here.  */
      emit_insn (gen_rtx_SET (VOIDmode, operands[0],
			      gen_rtx_SMAX (SImode, operands[1],
					    operands[2])));
      DONE;
    }
")

//Pattern#193
(define_insn "*smax_0"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(smax:SI (match_operand:SI 1 "s_register_operand" "r")
		 (const_int 0)))]
  "TARGET_32BIT"
  "bic%?\\t%0, %1, %1, asr #31"
  [(set_attr "predicable" "yes")]
)

//Pattern#194
(define_insn "*smax_m1"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(smax:SI (match_operand:SI 1 "s_register_operand" "r")
		 (const_int -1)))]
  "TARGET_32BIT"
  "orr%?\\t%0, %1, %1, asr #31"
  [(set_attr "predicable" "yes")]
)

//Pattern#195
(define_insn "*arm_smax_insn"
  [(set (match_operand:SI          0 "s_register_operand" "=r,r")
	(smax:SI (match_operand:SI 1 "s_register_operand"  "%0,?r")
		 (match_operand:SI 2 "arm_rhs_operand"    "rI,rI")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "@
   cmp\\t%1, %2\;movlt\\t%0, %2
   cmp\\t%1, %2\;movge\\t%0, %1\;movlt\\t%0, %2"
  [(set_attr "conds" "clob")
   (set_attr "length" "8,12")]
)

//Pattern#196
(define_expand "sminsi3"
  [(parallel [
    (set (match_operand:SI 0 "s_register_operand" "")
	 (smin:SI (match_operand:SI 1 "s_register_operand" "")
		  (match_operand:SI 2 "arm_rhs_operand" "")))
    (clobber (reg:CC CC_REGNUM))])]
  "TARGET_32BIT"
  "
  if (operands[2] == const0_rtx)
    {
      /* No need for a clobber of the condition code register here.  */
      emit_insn (gen_rtx_SET (VOIDmode, operands[0],
			      gen_rtx_SMIN (SImode, operands[1],
					    operands[2])));
      DONE;
    }
")

//Pattern#197
(define_insn "*smin_0"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(smin:SI (match_operand:SI 1 "s_register_operand" "r")
		 (const_int 0)))]
  "TARGET_32BIT"
  "and%?\\t%0, %1, %1, asr #31"
  [(set_attr "predicable" "yes")]
)

//Pattern#198
(define_insn "*arm_smin_insn"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(smin:SI (match_operand:SI 1 "s_register_operand" "%0,?r")
		 (match_operand:SI 2 "arm_rhs_operand" "rI,rI")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "@
   cmp\\t%1, %2\;movge\\t%0, %2
   cmp\\t%1, %2\;movlt\\t%0, %1\;movge\\t%0, %2"
  [(set_attr "conds" "clob")
   (set_attr "length" "8,12")]
)

//Pattern#199
(define_expand "umaxsi3"
  [(parallel [
    (set (match_operand:SI 0 "s_register_operand" "")
	 (umax:SI (match_operand:SI 1 "s_register_operand" "")
		  (match_operand:SI 2 "arm_rhs_operand" "")))
    (clobber (reg:CC CC_REGNUM))])]
  "TARGET_32BIT"
  ""
)

//Pattern#200
(define_insn "*arm_umaxsi3"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r,r")
	(umax:SI (match_operand:SI 1 "s_register_operand" "0,r,?r")
		 (match_operand:SI 2 "arm_rhs_operand" "rI,0,rI")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "@
   cmp\\t%1, %2\;movcc\\t%0, %2
   cmp\\t%1, %2\;movcs\\t%0, %1
   cmp\\t%1, %2\;movcs\\t%0, %1\;movcc\\t%0, %2"
  [(set_attr "conds" "clob")
   (set_attr "length" "8,8,12")]
)

//Pattern#201
(define_expand "uminsi3"
  [(parallel [
    (set (match_operand:SI 0 "s_register_operand" "")
	 (umin:SI (match_operand:SI 1 "s_register_operand" "")
		  (match_operand:SI 2 "arm_rhs_operand" "")))
    (clobber (reg:CC CC_REGNUM))])]
  "TARGET_32BIT"
  ""
)

//Pattern#202
(define_insn "*arm_uminsi3"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r,r")
	(umin:SI (match_operand:SI 1 "s_register_operand" "0,r,?r")
		 (match_operand:SI 2 "arm_rhs_operand" "rI,0,rI")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "@
   cmp\\t%1, %2\;movcs\\t%0, %2
   cmp\\t%1, %2\;movcc\\t%0, %1
   cmp\\t%1, %2\;movcc\\t%0, %1\;movcs\\t%0, %2"
  [(set_attr "conds" "clob")
   (set_attr "length" "8,8,12")]
)

//Pattern#203
(define_insn "*store_minmaxsi"
  [(set (match_operand:SI 0 "memory_operand" "=m")
	(match_operator:SI 3 "minmax_operator"
	 [(match_operand:SI 1 "s_register_operand" "r")
	  (match_operand:SI 2 "s_register_operand" "r")]))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT"
  "*
  operands[3] = gen_rtx_fmt_ee (minmax_code (operands[3]), SImode,
				operands[1], operands[2]);
  output_asm_insn (\"cmp\\t%1, %2\", operands);
  if (TARGET_THUMB2)
    output_asm_insn (\"ite\t%d3\", operands);
  output_asm_insn (\"str%d3\\t%1, %0\", operands);
  output_asm_insn (\"str%D3\\t%2, %0\", operands);
  return \"\";
  "
  [(set_attr "conds" "clob")
   (set (attr "length")
	(if_then_else (eq_attr "is_thumb" "yes")
		      (const_int 14)
		      (const_int 12)))
   (set_attr "type" "store1")]
)

; Reject the frame pointer in operand[1], since reloading this after
; it has been eliminated can cause carnage.
//Pattern#204
(define_insn "*minmax_arithsi"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(match_operator:SI 4 "shiftable_operator"
	 [(match_operator:SI 5 "minmax_operator"
	   [(match_operand:SI 2 "s_register_operand" "r,r")
	    (match_operand:SI 3 "arm_rhs_operand" "rI,rI")])
	  (match_operand:SI 1 "s_register_operand" "0,?r")]))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT && !arm_eliminable_register (operands[1])"
  "*
  {
    enum rtx_code code = GET_CODE (operands[4]);
    bool need_else;

    if (which_alternative != 0 || operands[3] != const0_rtx
        || (code != PLUS && code != IOR && code != XOR))
      need_else = true;
    else
      need_else = false;

    operands[5] = gen_rtx_fmt_ee (minmax_code (operands[5]), SImode,
				  operands[2], operands[3]);
    output_asm_insn (\"cmp\\t%2, %3\", operands);
    if (TARGET_THUMB2)
      {
	if (need_else)
	  output_asm_insn (\"ite\\t%d5\", operands);
	else
	  output_asm_insn (\"it\\t%d5\", operands);
      }
    output_asm_insn (\"%i4%d5\\t%0, %1, %2\", operands);
    if (need_else)
      output_asm_insn (\"%i4%D5\\t%0, %1, %3\", operands);
    return \"\";
  }"
  [(set_attr "conds" "clob")
   (set (attr "length")
	(if_then_else (eq_attr "is_thumb" "yes")
		      (const_int 14)
		      (const_int 12)))]
)

//Pattern#205
(define_code_iterator SAT [smin smax])
//Pattern#206
(define_code_iterator SATrev [smin smax])
//Pattern#207
(define_code_attr SATlo [(smin "1") (smax "2")])
//Pattern#208
(define_code_attr SAThi [(smin "2") (smax "1")])

//Pattern#209
(define_insn "*satsi_<SAT:code>"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
        (SAT:SI (SATrev:SI (match_operand:SI 3 "s_register_operand" "r")
                           (match_operand:SI 1 "const_int_operand" "i"))
                (match_operand:SI 2 "const_int_operand" "i")))]
  "TARGET_32BIT && arm_arch6 && <SAT:CODE> != <SATrev:CODE>
   && arm_sat_operator_match (operands[<SAT:SATlo>], operands[<SAT:SAThi>], NULL, NULL)"
{
  int mask;
  bool signed_sat;
  if (!arm_sat_operator_match (operands[<SAT:SATlo>], operands[<SAT:SAThi>],
                               &mask, &signed_sat))
    gcc_unreachable ();

  operands[1] = GEN_INT (mask);
  if (signed_sat)
    return "ssat%?\t%0, %1, %3";
  else
    return "usat%?\t%0, %1, %3";
}
  [(set_attr "predicable" "yes")
   (set_attr "insn" "sat")])

//Pattern#210
(define_insn "*satsi_<SAT:code>_shift"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
        (SAT:SI (SATrev:SI (match_operator:SI 3 "sat_shift_operator"
                             [(match_operand:SI 4 "s_register_operand" "r")
                              (match_operand:SI 5 "const_int_operand" "i")])
                           (match_operand:SI 1 "const_int_operand" "i"))
                (match_operand:SI 2 "const_int_operand" "i")))]
  "TARGET_32BIT && arm_arch6 && <SAT:CODE> != <SATrev:CODE>
   && arm_sat_operator_match (operands[<SAT:SATlo>], operands[<SAT:SAThi>], NULL, NULL)"
{
  int mask;
  bool signed_sat;
  if (!arm_sat_operator_match (operands[<SAT:SATlo>], operands[<SAT:SAThi>],
                               &mask, &signed_sat))
    gcc_unreachable ();

  operands[1] = GEN_INT (mask);
  if (signed_sat)
    return "ssat%?\t%0, %1, %4%S3";
  else
    return "usat%?\t%0, %1, %4%S3";
}
  [(set_attr "predicable" "yes")
   (set_attr "insn" "sat")
   (set_attr "shift" "3")
   (set_attr "type" "alu_shift")])

;; Shift and rotation insns

//Pattern#211
(define_expand "ashldi3"
  [(set (match_operand:DI            0 "s_register_operand" "")
        (ashift:DI (match_operand:DI 1 "s_register_operand" "")
                   (match_operand:SI 2 "general_operand" "")))]
  "TARGET_32BIT"
  "
  if (TARGET_NEON)
    {
      /* Delay the decision whether to use NEON or core-regs until
	 register allocation.  */
      emit_insn (gen_ashldi3_neon (operands[0], operands[1], operands[2]));
      DONE;
    }
  else
    {
      /* Only the NEON case can handle in-memory shift counts.  */
      if (!reg_or_int_operand (operands[2], SImode))
        operands[2] = force_reg (SImode, operands[2]);
    }

  if (!CONST_INT_P (operands[2]) && TARGET_REALLY_IWMMXT)
    ; /* No special preparation statements; expand pattern as above.  */
  else
    {
      rtx scratch1, scratch2;

      if (CONST_INT_P (operands[2])
	  && (HOST_WIDE_INT) INTVAL (operands[2]) == 1)
        {
          emit_insn (gen_arm_ashldi3_1bit (operands[0], operands[1]));
          DONE;
        }

      /* Ideally we should use iwmmxt here if we could know that operands[1]
         ends up already living in an iwmmxt register. Otherwise it's
         cheaper to have the alternate code being generated than moving
         values to iwmmxt regs and back.  */

      /* If we're optimizing for size, we prefer the libgcc calls.  */
      if (optimize_function_for_size_p (cfun))
	FAIL;

      /* Expand operation using core-registers.
	 'FAIL' would achieve the same thing, but this is a bit smarter.  */
      scratch1 = gen_reg_rtx (SImode);
      scratch2 = gen_reg_rtx (SImode);
      arm_emit_coreregs_64bit_shift (ASHIFT, operands[0], operands[1],
				     operands[2], scratch1, scratch2);
      DONE;
    }
  "
)

//Pattern#212
(define_insn "arm_ashldi3_1bit"
  [(set (match_operand:DI            0 "s_register_operand" "=r,&r")
        (ashift:DI (match_operand:DI 1 "s_register_operand" "0,r")
                   (const_int 1)))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT"
  "movs\\t%Q0, %Q1, asl #1\;adc\\t%R0, %R1, %R1"
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)

//Pattern#213
(define_expand "ashlsi3"
  [(set (match_operand:SI            0 "s_register_operand" "")
	(ashift:SI (match_operand:SI 1 "s_register_operand" "")
		   (match_operand:SI 2 "arm_rhs_operand" "")))]
  "TARGET_EITHER"
  "
  if (CONST_INT_P (operands[2])
      && ((unsigned HOST_WIDE_INT) INTVAL (operands[2])) > 31)
    {
      emit_insn (gen_movsi (operands[0], const0_rtx));
      DONE;
    }
  "
)

//Pattern#214
(define_insn "*thumb1_ashlsi3"
  [(set (match_operand:SI            0 "register_operand" "=l,l")
	(ashift:SI (match_operand:SI 1 "register_operand" "l,0")
		   (match_operand:SI 2 "nonmemory_operand" "N,l")))]
  "TARGET_THUMB1"
  "lsl\\t%0, %1, %2"
  [(set_attr "length" "2")
   (set_attr "conds" "set")])

//Pattern#215
(define_expand "ashrdi3"
  [(set (match_operand:DI              0 "s_register_operand" "")
        (ashiftrt:DI (match_operand:DI 1 "s_register_operand" "")
                     (match_operand:SI 2 "reg_or_int_operand" "")))]
  "TARGET_32BIT"
  "
  if (TARGET_NEON)
    {
      /* Delay the decision whether to use NEON or core-regs until
	 register allocation.  */
      emit_insn (gen_ashrdi3_neon (operands[0], operands[1], operands[2]));
      DONE;
    }

  if (!CONST_INT_P (operands[2]) && TARGET_REALLY_IWMMXT)
    ; /* No special preparation statements; expand pattern as above.  */
  else
    {
      rtx scratch1, scratch2;

      if (CONST_INT_P (operands[2])
	  && (HOST_WIDE_INT) INTVAL (operands[2]) == 1)
        {
          emit_insn (gen_arm_ashrdi3_1bit (operands[0], operands[1]));
          DONE;
        }

      /* Ideally we should use iwmmxt here if we could know that operands[1]
         ends up already living in an iwmmxt register. Otherwise it's
         cheaper to have the alternate code being generated than moving
         values to iwmmxt regs and back.  */

      /* If we're optimizing for size, we prefer the libgcc calls.  */
      if (optimize_function_for_size_p (cfun))
	FAIL;

      /* Expand operation using core-registers.
	 'FAIL' would achieve the same thing, but this is a bit smarter.  */
      scratch1 = gen_reg_rtx (SImode);
      scratch2 = gen_reg_rtx (SImode);
      arm_emit_coreregs_64bit_shift (ASHIFTRT, operands[0], operands[1],
				     operands[2], scratch1, scratch2);
      DONE;
    }
  "
)

//Pattern#216
(define_insn "arm_ashrdi3_1bit"
  [(set (match_operand:DI              0 "s_register_operand" "=r,&r")
        (ashiftrt:DI (match_operand:DI 1 "s_register_operand" "0,r")
                     (const_int 1)))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT"
  "movs\\t%R0, %R1, asr #1\;mov\\t%Q0, %Q1, rrx"
  [(set_attr "conds" "clob")
   (set_attr "insn" "mov")
   (set_attr "length" "8")]
)

//Pattern#217
(define_expand "ashrsi3"
  [(set (match_operand:SI              0 "s_register_operand" "")
	(ashiftrt:SI (match_operand:SI 1 "s_register_operand" "")
		     (match_operand:SI 2 "arm_rhs_operand" "")))]
  "TARGET_EITHER"
  "
  if (CONST_INT_P (operands[2])
      && ((unsigned HOST_WIDE_INT) INTVAL (operands[2])) > 31)
    operands[2] = GEN_INT (31);
  "
)

//Pattern#218
(define_insn "*thumb1_ashrsi3"
  [(set (match_operand:SI              0 "register_operand" "=l,l")
	(ashiftrt:SI (match_operand:SI 1 "register_operand" "l,0")
		     (match_operand:SI 2 "nonmemory_operand" "N,l")))]
  "TARGET_THUMB1"
  "asr\\t%0, %1, %2"
  [(set_attr "length" "2")
   (set_attr "conds" "set")])

//Pattern#219
(define_expand "lshrdi3"
  [(set (match_operand:DI              0 "s_register_operand" "")
        (lshiftrt:DI (match_operand:DI 1 "s_register_operand" "")
                     (match_operand:SI 2 "reg_or_int_operand" "")))]
  "TARGET_32BIT"
  "
  if (TARGET_NEON)
    {
      /* Delay the decision whether to use NEON or core-regs until
	 register allocation.  */
      emit_insn (gen_lshrdi3_neon (operands[0], operands[1], operands[2]));
      DONE;
    }

  if (!CONST_INT_P (operands[2]) && TARGET_REALLY_IWMMXT)
    ; /* No special preparation statements; expand pattern as above.  */
  else
    {
      rtx scratch1, scratch2;

      if (CONST_INT_P (operands[2])
	  && (HOST_WIDE_INT) INTVAL (operands[2]) == 1)
        {
          emit_insn (gen_arm_lshrdi3_1bit (operands[0], operands[1]));
          DONE;
        }

      /* Ideally we should use iwmmxt here if we could know that operands[1]
         ends up already living in an iwmmxt register. Otherwise it's
         cheaper to have the alternate code being generated than moving
         values to iwmmxt regs and back.  */

      /* If we're optimizing for size, we prefer the libgcc calls.  */
      if (optimize_function_for_size_p (cfun))
	FAIL;

      /* Expand operation using core-registers.
	 'FAIL' would achieve the same thing, but this is a bit smarter.  */
      scratch1 = gen_reg_rtx (SImode);
      scratch2 = gen_reg_rtx (SImode);
      arm_emit_coreregs_64bit_shift (LSHIFTRT, operands[0], operands[1],
				     operands[2], scratch1, scratch2);
      DONE;
    }
  "
)

//Pattern#220
(define_insn "arm_lshrdi3_1bit"
  [(set (match_operand:DI              0 "s_register_operand" "=r,&r")
        (lshiftrt:DI (match_operand:DI 1 "s_register_operand" "0,r")
                     (const_int 1)))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT"
  "movs\\t%R0, %R1, lsr #1\;mov\\t%Q0, %Q1, rrx"
  [(set_attr "conds" "clob")
   (set_attr "insn" "mov")
   (set_attr "length" "8")]
)

//Pattern#221
(define_expand "lshrsi3"
  [(set (match_operand:SI              0 "s_register_operand" "")
	(lshiftrt:SI (match_operand:SI 1 "s_register_operand" "")
		     (match_operand:SI 2 "arm_rhs_operand" "")))]
  "TARGET_EITHER"
  "
  if (CONST_INT_P (operands[2])
      && ((unsigned HOST_WIDE_INT) INTVAL (operands[2])) > 31)
    {
      emit_insn (gen_movsi (operands[0], const0_rtx));
      DONE;
    }
  "
)

//Pattern#222
(define_insn "*thumb1_lshrsi3"
  [(set (match_operand:SI              0 "register_operand" "=l,l")
	(lshiftrt:SI (match_operand:SI 1 "register_operand" "l,0")
		     (match_operand:SI 2 "nonmemory_operand" "N,l")))]
  "TARGET_THUMB1"
  "lsr\\t%0, %1, %2"
  [(set_attr "length" "2")
   (set_attr "conds" "set")])

//Pattern#223
(define_expand "rotlsi3"
  [(set (match_operand:SI              0 "s_register_operand" "")
	(rotatert:SI (match_operand:SI 1 "s_register_operand" "")
		     (match_operand:SI 2 "reg_or_int_operand" "")))]
  "TARGET_32BIT"
  "
  if (CONST_INT_P (operands[2]))
    operands[2] = GEN_INT ((32 - INTVAL (operands[2])) % 32);
  else
    {
      rtx reg = gen_reg_rtx (SImode);
      emit_insn (gen_subsi3 (reg, GEN_INT (32), operands[2]));
      operands[2] = reg;
    }
  "
)

//Pattern#224
(define_expand "rotrsi3"
  [(set (match_operand:SI              0 "s_register_operand" "")
	(rotatert:SI (match_operand:SI 1 "s_register_operand" "")
		     (match_operand:SI 2 "arm_rhs_operand" "")))]
  "TARGET_EITHER"
  "
  if (TARGET_32BIT)
    {
      if (CONST_INT_P (operands[2])
          && ((unsigned HOST_WIDE_INT) INTVAL (operands[2])) > 31)
        operands[2] = GEN_INT (INTVAL (operands[2]) % 32);
    }
  else /* TARGET_THUMB1 */
    {
      if (CONST_INT_P (operands [2]))
        operands [2] = force_reg (SImode, operands[2]);
    }
  "
)

//Pattern#225
(define_insn "*thumb1_rotrsi3"
  [(set (match_operand:SI              0 "register_operand" "=l")
	(rotatert:SI (match_operand:SI 1 "register_operand" "0")
		     (match_operand:SI 2 "register_operand" "l")))]
  "TARGET_THUMB1"
  "ror\\t%0, %0, %2"
  [(set_attr "length" "2")]
)

//Pattern#226
(define_insn "*arm_shiftsi3"
  [(set (match_operand:SI   0 "s_register_operand" "=r")
	(match_operator:SI  3 "shift_operator"
	 [(match_operand:SI 1 "s_register_operand"  "r")
	  (match_operand:SI 2 "reg_or_int_operand" "rM")]))]
  "TARGET_32BIT"
  "* return arm_output_shift(operands, 0);"
  [(set_attr "predicable" "yes")
   (set_attr "shift" "1")
   (set (attr "type") (if_then_else (match_operand 2 "const_int_operand" "")
		      (const_string "alu_shift")
		      (const_string "alu_shift_reg")))]
)

//Pattern#227
(define_insn "*shiftsi3_compare0"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV (match_operator:SI 3 "shift_operator"
			  [(match_operand:SI 1 "s_register_operand" "r")
			   (match_operand:SI 2 "arm_rhs_operand" "rM")])
			 (const_int 0)))
   (set (match_operand:SI 0 "s_register_operand" "=r")
	(match_op_dup 3 [(match_dup 1) (match_dup 2)]))]
  "TARGET_32BIT"
  "* return arm_output_shift(operands, 1);"
  [(set_attr "conds" "set")
   (set_attr "shift" "1")
   (set (attr "type") (if_then_else (match_operand 2 "const_int_operand" "")
		      (const_string "alu_shift")
		      (const_string "alu_shift_reg")))]
)

//Pattern#228
(define_insn "*shiftsi3_compare0_scratch"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV (match_operator:SI 3 "shift_operator"
			  [(match_operand:SI 1 "s_register_operand" "r")
			   (match_operand:SI 2 "arm_rhs_operand" "rM")])
			 (const_int 0)))
   (clobber (match_scratch:SI 0 "=r"))]
  "TARGET_32BIT"
  "* return arm_output_shift(operands, 1);"
  [(set_attr "conds" "set")
   (set_attr "shift" "1")]
)

//Pattern#229
(define_insn "*not_shiftsi"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(not:SI (match_operator:SI 3 "shift_operator"
		 [(match_operand:SI 1 "s_register_operand" "r,r")
		  (match_operand:SI 2 "shift_amount_operand" "M,rM")])))]
  "TARGET_32BIT"
  "mvn%?\\t%0, %1%S3"
  [(set_attr "predicable" "yes")
   (set_attr "shift" "1")
   (set_attr "insn" "mvn")
   (set_attr "arch" "32,a")
   (set_attr "type" "alu_shift,alu_shift_reg")])

//Pattern#230
(define_insn "*not_shiftsi_compare0"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV
	 (not:SI (match_operator:SI 3 "shift_operator"
		  [(match_operand:SI 1 "s_register_operand" "r,r")
		   (match_operand:SI 2 "shift_amount_operand" "M,rM")]))
	 (const_int 0)))
   (set (match_operand:SI 0 "s_register_operand" "=r,r")
	(not:SI (match_op_dup 3 [(match_dup 1) (match_dup 2)])))]
  "TARGET_32BIT"
  "mvn%.\\t%0, %1%S3"
  [(set_attr "conds" "set")
   (set_attr "shift" "1")
   (set_attr "insn" "mvn")
   (set_attr "arch" "32,a")
   (set_attr "type" "alu_shift,alu_shift_reg")])

//Pattern#231
(define_insn "*not_shiftsi_compare0_scratch"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV
	 (not:SI (match_operator:SI 3 "shift_operator"
		  [(match_operand:SI 1 "s_register_operand" "r,r")
		   (match_operand:SI 2 "shift_amount_operand" "M,rM")]))
	 (const_int 0)))
   (clobber (match_scratch:SI 0 "=r,r"))]
  "TARGET_32BIT"
  "mvn%.\\t%0, %1%S3"
  [(set_attr "conds" "set")
   (set_attr "shift" "1")
   (set_attr "insn" "mvn")
   (set_attr "arch" "32,a")
   (set_attr "type" "alu_shift,alu_shift_reg")])

;; We don't really have extzv, but defining this using shifts helps
;; to reduce register pressure later on.

//Pattern#232
(define_expand "extzv"
  [(set (match_operand 0 "s_register_operand" "")
	(zero_extract (match_operand 1 "nonimmediate_operand" "")
		      (match_operand 2 "const_int_operand" "")
		      (match_operand 3 "const_int_operand" "")))]
  "TARGET_THUMB1 || arm_arch_thumb2"
  "
  {
    HOST_WIDE_INT lshift = 32 - INTVAL (operands[2]) - INTVAL (operands[3]);
    HOST_WIDE_INT rshift = 32 - INTVAL (operands[2]);
    
    if (arm_arch_thumb2)
      {
	HOST_WIDE_INT width = INTVAL (operands[2]);
	HOST_WIDE_INT bitpos = INTVAL (operands[3]);

	if (unaligned_access && MEM_P (operands[1])
	    && (width == 16 || width == 32) && (bitpos % BITS_PER_UNIT) == 0)
	  {
	    rtx base_addr;

	    if (BYTES_BIG_ENDIAN)
	      bitpos = GET_MODE_BITSIZE (GET_MODE (operands[0])) - width
		       - bitpos;

	    if (width == 32)
              {
		base_addr = adjust_address (operands[1], SImode,
					    bitpos / BITS_PER_UNIT);
		emit_insn (gen_unaligned_loadsi (operands[0], base_addr));
              }
	    else
              {
		rtx dest = operands[0];
		rtx tmp = gen_reg_rtx (SImode);

		/* We may get a paradoxical subreg here.  Strip it off.  */
		if (GET_CODE (dest) == SUBREG
		    && GET_MODE (dest) == SImode
		    && GET_MODE (SUBREG_REG (dest)) == HImode)
		  dest = SUBREG_REG (dest);

		if (GET_MODE_BITSIZE (GET_MODE (dest)) != width)
		  FAIL;

		base_addr = adjust_address (operands[1], HImode,
					    bitpos / BITS_PER_UNIT);
		emit_insn (gen_unaligned_loadhiu (tmp, base_addr));
		emit_move_insn (gen_lowpart (SImode, dest), tmp);
	      }
	    DONE;
	  }
	else if (s_register_operand (operands[1], GET_MODE (operands[1])))
	  {
	    emit_insn (gen_extzv_t2 (operands[0], operands[1], operands[2],
				     operands[3]));
	    DONE;
	  }
	else
	  FAIL;
      }
    
    if (!s_register_operand (operands[1], GET_MODE (operands[1])))
      FAIL;

    operands[3] = GEN_INT (rshift);
    
    if (lshift == 0)
      {
        emit_insn (gen_lshrsi3 (operands[0], operands[1], operands[3]));
        DONE;
      }
      
    emit_insn (gen_extzv_t1 (operands[0], operands[1], GEN_INT (lshift),
			     operands[3], gen_reg_rtx (SImode)));
    DONE;
  }"
)

;; Helper for extzv, for the Thumb-1 register-shifts case.

//Pattern#233
(define_expand "extzv_t1"
  [(set (match_operand:SI 4 "s_register_operand" "")
	(ashift:SI (match_operand:SI 1 "nonimmediate_operand" "")
		   (match_operand:SI 2 "const_int_operand" "")))
   (set (match_operand:SI 0 "s_register_operand" "")
	(lshiftrt:SI (match_dup 4)
		     (match_operand:SI 3 "const_int_operand" "")))]
  "TARGET_THUMB1"
  "")

//Pattern#234
(define_expand "extv"
  [(set (match_operand 0 "s_register_operand" "")
	(sign_extract (match_operand 1 "nonimmediate_operand" "")
		      (match_operand 2 "const_int_operand" "")
		      (match_operand 3 "const_int_operand" "")))]
  "arm_arch_thumb2"
{
  HOST_WIDE_INT width = INTVAL (operands[2]);
  HOST_WIDE_INT bitpos = INTVAL (operands[3]);

  if (unaligned_access && MEM_P (operands[1]) && (width == 16 || width == 32)
      && (bitpos % BITS_PER_UNIT)  == 0)
    {
      rtx base_addr;
      
      if (BYTES_BIG_ENDIAN)
	bitpos = GET_MODE_BITSIZE (GET_MODE (operands[0])) - width - bitpos;
      
      if (width == 32)
        {
	  base_addr = adjust_address (operands[1], SImode,
				      bitpos / BITS_PER_UNIT);
	  emit_insn (gen_unaligned_loadsi (operands[0], base_addr));
        }
      else
        {
	  rtx dest = operands[0];
	  rtx tmp = gen_reg_rtx (SImode);
	  
	  /* We may get a paradoxical subreg here.  Strip it off.  */
	  if (GET_CODE (dest) == SUBREG
	      && GET_MODE (dest) == SImode
	      && GET_MODE (SUBREG_REG (dest)) == HImode)
	    dest = SUBREG_REG (dest);
	  
	  if (GET_MODE_BITSIZE (GET_MODE (dest)) != width)
	    FAIL;
	  
	  base_addr = adjust_address (operands[1], HImode,
				      bitpos / BITS_PER_UNIT);
	  emit_insn (gen_unaligned_loadhis (tmp, base_addr));
	  emit_move_insn (gen_lowpart (SImode, dest), tmp);
	}

      DONE;
    }
  else if (!s_register_operand (operands[1], GET_MODE (operands[1])))
    FAIL;
  else if (GET_MODE (operands[0]) == SImode
	   && GET_MODE (operands[1]) == SImode)
    {
      emit_insn (gen_extv_regsi (operands[0], operands[1], operands[2],
				 operands[3]));
      DONE;
    }

  FAIL;
})

; Helper to expand register forms of extv with the proper modes.

//Pattern#235
(define_expand "extv_regsi"
  [(set (match_operand:SI 0 "s_register_operand" "")
	(sign_extract:SI (match_operand:SI 1 "s_register_operand" "")
			 (match_operand 2 "const_int_operand" "")
			 (match_operand 3 "const_int_operand" "")))]
  ""
{
})

; ARMv6+ unaligned load/store instructions (used for packed structure accesses).

(define_insn "divsi3"
  [(set (match_operand:SI	  0 "s_register_operand" "=r")
	(div:SI (match_operand:SI 1 "s_register_operand"  "r")
		(match_operand:SI 2 "s_register_operand"  "r")))]
  "TARGET_IDIV"
  "sdiv%?\t%0, %1, %2"
  [(set_attr "predicable" "yes")
   (set_attr "insn" "sdiv")]
)

//Pattern#244
(define_insn "udivsi3"
  [(set (match_operand:SI	   0 "s_register_operand" "=r")
	(udiv:SI (match_operand:SI 1 "s_register_operand"  "r")
		 (match_operand:SI 2 "s_register_operand"  "r")))]
  "TARGET_IDIV"
  "udiv%?\t%0, %1, %2"
  [(set_attr "predicable" "yes")
   (set_attr "insn" "udiv")]
)


;; Unary arithmetic insns

//Pattern#245
(define_expand "negdi2"
 [(parallel
   [(set (match_operand:DI 0 "s_register_operand" "")
	 (neg:DI (match_operand:DI 1 "s_register_operand" "")))
    (clobber (reg:CC CC_REGNUM))])]
  "TARGET_EITHER"
  {
    if (TARGET_NEON)
      {
        emit_insn (gen_negdi2_neon (operands[0], operands[1]));
	DONE;
      }
  }
)

;; The constraints here are to prevent a *partial* overlap (where %Q0 == %R1).
;; The first alternative allows the common case of a *full* overlap.
//Pattern#246
(define_insn "*arm_negdi2"
  [(set (match_operand:DI         0 "s_register_operand" "=r,&r")
	(neg:DI (match_operand:DI 1 "s_register_operand"  "0,r")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "rsbs\\t%Q0, %Q1, #0\;rsc\\t%R0, %R1, #0"
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)

//Pattern#247
(define_insn "*thumb1_negdi2"
  [(set (match_operand:DI 0 "register_operand" "=&l")
	(neg:DI (match_operand:DI 1 "register_operand" "l")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_THUMB1"
  "mov\\t%R0, #0\;neg\\t%Q0, %Q1\;sbc\\t%R0, %R1"
  [(set_attr "length" "6")]
)

//Pattern#248
(define_expand "negsi2"
  [(set (match_operand:SI         0 "s_register_operand" "")
	(neg:SI (match_operand:SI 1 "s_register_operand" "")))]
  "TARGET_EITHER"
  ""
)

//Pattern#249
(define_insn "*arm_negsi2"
  [(set (match_operand:SI         0 "s_register_operand" "=r")
	(neg:SI (match_operand:SI 1 "s_register_operand" "r")))]
  "TARGET_32BIT"
  "rsb%?\\t%0, %1, #0"
  [(set_attr "predicable" "yes")]
)

//Pattern#250
(define_insn "*thumb1_negsi2"
  [(set (match_operand:SI         0 "register_operand" "=l")
	(neg:SI (match_operand:SI 1 "register_operand" "l")))]
  "TARGET_THUMB1"
  "neg\\t%0, %1"
  [(set_attr "length" "2")]
)

//Pattern#251
(define_expand "negsf2"
  [(set (match_operand:SF         0 "s_register_operand" "")
	(neg:SF (match_operand:SF 1 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP"
  ""
)

//Pattern#252
(define_expand "negdf2"
  [(set (match_operand:DF         0 "s_register_operand" "")
	(neg:DF (match_operand:DF 1 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP_DOUBLE"
  "")

;; abssi2 doesn't really clobber the condition codes if a different register
;; is being set.  To keep things simple, assume during rtl manipulations that
;; it does, but tell the final scan operator the truth.  Similarly for
;; (neg (abs...))

//Pattern#253
(define_expand "abssi2"
  [(parallel
    [(set (match_operand:SI         0 "s_register_operand" "")
	  (abs:SI (match_operand:SI 1 "s_register_operand" "")))
     (clobber (match_dup 2))])]
  "TARGET_EITHER"
  "
  if (TARGET_THUMB1)
    operands[2] = gen_rtx_SCRATCH (SImode);
  else
    operands[2] = gen_rtx_REG (CCmode, CC_REGNUM);
")

//Pattern#254
(define_insn "*arm_abssi2"
  [(set (match_operand:SI 0 "s_register_operand" "=r,&r")
	(abs:SI (match_operand:SI 1 "s_register_operand" "0,r")))
   (clobber (reg:CC CC_REGNUM))]
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
  [(set (match_operand:SI 0 "s_register_operand" "=l")
	(abs:SI (match_operand:SI 1 "s_register_operand" "l")))
   (clobber (match_scratch:SI 2 "=&l"))]
  "TARGET_THUMB1"
  "#"
  "TARGET_THUMB1 && reload_completed"
  [(set (match_dup 2) (ashiftrt:SI (match_dup 1) (const_int 31)))
   (set (match_dup 0) (plus:SI (match_dup 1) (match_dup 2)))
   (set (match_dup 0) (xor:SI (match_dup 0) (match_dup 2)))]
  ""
  [(set_attr "length" "6")]
)

//Pattern#256
(define_insn "*arm_neg_abssi2"
  [(set (match_operand:SI 0 "s_register_operand" "=r,&r")
	(neg:SI (abs:SI (match_operand:SI 1 "s_register_operand" "0,r"))))
   (clobber (reg:CC CC_REGNUM))]
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
  [(set (match_operand:SI 0 "s_register_operand" "=l")
	(neg:SI (abs:SI (match_operand:SI 1 "s_register_operand" "l"))))
   (clobber (match_scratch:SI 2 "=&l"))]
  "TARGET_THUMB1"
  "#"
  "TARGET_THUMB1 && reload_completed"
  [(set (match_dup 2) (ashiftrt:SI (match_dup 1) (const_int 31)))
   (set (match_dup 0) (minus:SI (match_dup 2) (match_dup 1)))
   (set (match_dup 0) (xor:SI (match_dup 0) (match_dup 2)))]
  ""
  [(set_attr "length" "6")]
)

//Pattern#258
(define_expand "abssf2"
  [(set (match_operand:SF         0 "s_register_operand" "")
	(abs:SF (match_operand:SF 1 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT"
  "")

//Pattern#259
(define_expand "absdf2"
  [(set (match_operand:DF         0 "s_register_operand" "")
	(abs:DF (match_operand:DF 1 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT && !TARGET_VFP_SINGLE"
  "")

//Pattern#260
(define_expand "sqrtsf2"
  [(set (match_operand:SF 0 "s_register_operand" "")
	(sqrt:SF (match_operand:SF 1 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP"
  "")

//Pattern#261
(define_expand "sqrtdf2"
  [(set (match_operand:DF 0 "s_register_operand" "")
	(sqrt:DF (match_operand:DF 1 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP_DOUBLE"
  "")

//Pattern#262
(define_insn_and_split "one_cmpldi2"
  [(set (match_operand:DI 0 "s_register_operand"	 "=w,&r,&r,?w")
	(not:DI (match_operand:DI 1 "s_register_operand" " w, 0, r, w")))]
  "TARGET_32BIT"
  "@
   vmvn\t%P0, %P1
   #
   #
   vmvn\t%P0, %P1"
  "TARGET_32BIT && reload_completed
   && arm_general_register_operand (operands[0], DImode)"
  [(set (match_dup 0) (not:SI (match_dup 1)))
   (set (match_dup 2) (not:SI (match_dup 3)))]
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
  [(set (match_operand:SI         0 "s_register_operand" "")
	(not:SI (match_operand:SI 1 "s_register_operand" "")))]
  "TARGET_EITHER"
  ""
)

//Pattern#264
(define_insn "*arm_one_cmplsi2"
  [(set (match_operand:SI         0 "s_register_operand" "=r")
	(not:SI (match_operand:SI 1 "s_register_operand"  "r")))]
  "TARGET_32BIT"
  "mvn%?\\t%0, %1"
  [(set_attr "predicable" "yes")
   (set_attr "insn" "mvn")]
)

//Pattern#265
(define_insn "*thumb1_one_cmplsi2"
  [(set (match_operand:SI         0 "register_operand" "=l")
	(not:SI (match_operand:SI 1 "register_operand"  "l")))]
  "TARGET_THUMB1"
  "mvn\\t%0, %1"
  [(set_attr "length" "2")
   (set_attr "insn" "mvn")]
)

//Pattern#266
(define_insn "*notsi_compare0"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV (not:SI (match_operand:SI 1 "s_register_operand" "r"))
			 (const_int 0)))
   (set (match_operand:SI 0 "s_register_operand" "=r")
	(not:SI (match_dup 1)))]
  "TARGET_32BIT"
  "mvn%.\\t%0, %1"
  [(set_attr "conds" "set")
   (set_attr "insn" "mvn")]
)

//Pattern#267
(define_insn "*notsi_compare0_scratch"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV (not:SI (match_operand:SI 1 "s_register_operand" "r"))
			 (const_int 0)))
   (clobber (match_scratch:SI 0 "=r"))]
  "TARGET_32BIT"
  "mvn%.\\t%0, %1"
  [(set_attr "conds" "set")
   (set_attr "insn" "mvn")]
)

;; Fixed <--> Floating conversion insns

//Pattern#268
(define_expand "floatsihf2"
  [(set (match_operand:HF           0 "general_operand" "")
	(float:HF (match_operand:SI 1 "general_operand" "")))]
  "TARGET_EITHER"
  "
  {
    rtx op1 = gen_reg_rtx (SFmode);
    expand_float (op1, operands[1], 0);
    op1 = convert_to_mode (HFmode, op1, 0);
    emit_move_insn (operands[0], op1);
    DONE;
  }"
)

//Pattern#269
(define_expand "floatdihf2"
  [(set (match_operand:HF           0 "general_operand" "")
	(float:HF (match_operand:DI 1 "general_operand" "")))]
  "TARGET_EITHER"
  "
  {
    rtx op1 = gen_reg_rtx (SFmode);
    expand_float (op1, operands[1], 0);
    op1 = convert_to_mode (HFmode, op1, 0);
    emit_move_insn (operands[0], op1);
    DONE;
  }"
)

//Pattern#270
(define_expand "floatsisf2"
  [(set (match_operand:SF           0 "s_register_operand" "")
	(float:SF (match_operand:SI 1 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT"
  "
")

//Pattern#271
(define_expand "floatsidf2"
  [(set (match_operand:DF           0 "s_register_operand" "")
	(float:DF (match_operand:SI 1 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT && !TARGET_VFP_SINGLE"
  "
")

//Pattern#272
(define_expand "fix_trunchfsi2"
  [(set (match_operand:SI         0 "general_operand" "")
	(fix:SI (fix:HF (match_operand:HF 1 "general_operand"  ""))))]
  "TARGET_EITHER"
  "
  {
    rtx op1 = convert_to_mode (SFmode, operands[1], 0);
    expand_fix (operands[0], op1, 0);
    DONE;
  }"
)

//Pattern#273
(define_expand "fix_trunchfdi2"
  [(set (match_operand:DI         0 "general_operand" "")
	(fix:DI (fix:HF (match_operand:HF 1 "general_operand"  ""))))]
  "TARGET_EITHER"
  "
  {
    rtx op1 = convert_to_mode (SFmode, operands[1], 0);
    expand_fix (operands[0], op1, 0);
    DONE;
  }"
)

//Pattern#274
(define_expand "fix_truncsfsi2"
  [(set (match_operand:SI         0 "s_register_operand" "")
	(fix:SI (fix:SF (match_operand:SF 1 "s_register_operand"  ""))))]
  "TARGET_32BIT && TARGET_HARD_FLOAT"
  "
")

//Pattern#275
(define_expand "fix_truncdfsi2"
  [(set (match_operand:SI         0 "s_register_operand" "")
	(fix:SI (fix:DF (match_operand:DF 1 "s_register_operand"  ""))))]
  "TARGET_32BIT && TARGET_HARD_FLOAT && !TARGET_VFP_SINGLE"
  "
")

;; Truncation insns

//Pattern#276
(define_expand "truncdfsf2"
  [(set (match_operand:SF  0 "s_register_operand" "")
	(float_truncate:SF
 	 (match_operand:DF 1 "s_register_operand" "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT && !TARGET_VFP_SINGLE"
  ""
)

/* DFmode -> HFmode conversions have to go through SFmode.  */
//Pattern#277
(define_expand "truncdfhf2"
  [(set (match_operand:HF  0 "general_operand" "")
	(float_truncate:HF
 	 (match_operand:DF 1 "general_operand" "")))]
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

;; Zero and sign extension instructions.

//Pattern#278
(define_insn "zero_extend<mode>di2"
  [(set (match_operand:DI 0 "s_register_operand" "=w,r,?r,w")
        (zero_extend:DI (match_operand:QHSI 1 "<qhs_zextenddi_op>"
					    "<qhs_zextenddi_cstr>")))]
  "TARGET_32BIT <qhs_zextenddi_cond>"
  "#"
  [(set_attr "length" "8,4,8,8")
   (set_attr "arch" "neon_nota8,*,*,neon_onlya8")
   (set_attr "ce_count" "2")
   (set_attr "predicable" "yes")]
)

//Pattern#279
(define_insn "extend<mode>di2"
  [(set (match_operand:DI 0 "s_register_operand" "=w,r,?r,?r,w")
        (sign_extend:DI (match_operand:QHSI 1 "<qhs_extenddi_op>"
					    "<qhs_extenddi_cstr>")))]
  "TARGET_32BIT <qhs_sextenddi_cond>"
  "#"
  [(set_attr "length" "8,4,8,8,8")
   (set_attr "ce_count" "2")
   (set_attr "shift" "1")
   (set_attr "predicable" "yes")
   (set_attr "arch" "neon_nota8,*,a,t,neon_onlya8")]
)

;; Splits for all extensions to DImode
//Pattern#280
(define_split
  [(set (match_operand:DI 0 "s_register_operand" "")
        (zero_extend:DI (match_operand 1 "nonimmediate_operand" "")))]
  "TARGET_32BIT && reload_completed && !IS_VFP_REGNUM (REGNO (operands[0]))"
  [(set (match_dup 0) (match_dup 1))]
{
  rtx lo_part = gen_lowpart (SImode, operands[0]);
  enum machine_mode src_mode = GET_MODE (operands[1]);

  if (REG_P (operands[0])
      && !reg_overlap_mentioned_p (operands[0], operands[1]))
    emit_clobber (operands[0]);
  if (!REG_P (lo_part) || src_mode != SImode
      || !rtx_equal_p (lo_part, operands[1]))
    {
      if (src_mode == SImode)
        emit_move_insn (lo_part, operands[1]);
      else
        emit_insn (gen_rtx_SET (VOIDmode, lo_part,
				gen_rtx_ZERO_EXTEND (SImode, operands[1])));
      operands[1] = lo_part;
    }
  operands[0] = gen_highpart (SImode, operands[0]);
  operands[1] = const0_rtx;
})

//Pattern#281
(define_split
  [(set (match_operand:DI 0 "s_register_operand" "")
        (sign_extend:DI (match_operand 1 "nonimmediate_operand" "")))]
  "TARGET_32BIT && reload_completed && !IS_VFP_REGNUM (REGNO (operands[0]))"
  [(set (match_dup 0) (ashiftrt:SI (match_dup 1) (const_int 31)))]
{
  rtx lo_part = gen_lowpart (SImode, operands[0]);
  enum machine_mode src_mode = GET_MODE (operands[1]);

  if (REG_P (operands[0])
      && !reg_overlap_mentioned_p (operands[0], operands[1]))
    emit_clobber (operands[0]);

  if (!REG_P (lo_part) || src_mode != SImode
      || !rtx_equal_p (lo_part, operands[1]))
    {
      if (src_mode == SImode)
        emit_move_insn (lo_part, operands[1]);
      else
        emit_insn (gen_rtx_SET (VOIDmode, lo_part,
				gen_rtx_SIGN_EXTEND (SImode, operands[1])));
      operands[1] = lo_part;
    }
  operands[0] = gen_highpart (SImode, operands[0]);
})

//Pattern#282
(define_expand "zero_extendhisi2"
  [(set (match_operand:SI 0 "s_register_operand" "")
	(zero_extend:SI (match_operand:HI 1 "nonimmediate_operand" "")))]
  "TARGET_EITHER"
{
  if (TARGET_ARM && !arm_arch4 && MEM_P (operands[1]))
    {
      emit_insn (gen_movhi_bytes (operands[0], operands[1]));
      DONE;
    }
  if (!arm_arch6 && !MEM_P (operands[1]))
    {
      rtx t = gen_lowpart (SImode, operands[1]);
      rtx tmp = gen_reg_rtx (SImode);
      emit_insn (gen_ashlsi3 (tmp, t, GEN_INT (16)));
      emit_insn (gen_lshrsi3 (operands[0], tmp, GEN_INT (16)));
      DONE;
    }
})

//Pattern#283
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(zero_extend:SI (match_operand:HI 1 "s_register_operand" "")))]
  "!TARGET_THUMB2 && !arm_arch6"
  [(set (match_dup 0) (ashift:SI (match_dup 2) (const_int 16)))
   (set (match_dup 0) (lshiftrt:SI (match_dup 0) (const_int 16)))]
{
  operands[2] = gen_lowpart (SImode, operands[1]);
})

//Pattern#284
(define_insn "*thumb1_zero_extendhisi2"
  [(set (match_operand:SI 0 "register_operand" "=l,l")
	(zero_extend:SI (match_operand:HI 1 "nonimmediate_operand" "l,m")))]
  "TARGET_THUMB1"
{
  rtx mem;

  if (which_alternative == 0 && arm_arch6)
    return "uxth\t%0, %1";
  if (which_alternative == 0)
    return "#";

  mem = XEXP (operands[1], 0);

  if (GET_CODE (mem) == CONST)
    mem = XEXP (mem, 0);
    
  if (GET_CODE (mem) == PLUS)
    {
      rtx a = XEXP (mem, 0);

      /* This can happen due to bugs in reload.  */
      if (REG_P (a) && REGNO (a) == SP_REGNUM)
        {
          rtx ops[2];
          ops[0] = operands[0];
          ops[1] = a;
      
          output_asm_insn ("mov\t%0, %1", ops);

          XEXP (mem, 0) = operands[0];
       }
    }
    
  return "ldrh\t%0, %1";
}
  [(set_attr_alternative "length"
			 [(if_then_else (eq_attr "is_arch6" "yes")
				       (const_int 2) (const_int 4))
			 (const_int 4)])
   (set_attr "type" "simple_alu_shift, load_byte")]
)

//Pattern#285
(define_insn "*arm_zero_extendhisi2"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(zero_extend:SI (match_operand:HI 1 "nonimmediate_operand" "r,m")))]
  "TARGET_ARM && arm_arch4 && !arm_arch6"
  "@
   #
   ldr%(h%)\\t%0, %1"
  [(set_attr "type" "alu_shift,load_byte")
   (set_attr "predicable" "yes")]
)

//Pattern#286
(define_insn "*arm_zero_extendhisi2_v6"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(zero_extend:SI (match_operand:HI 1 "nonimmediate_operand" "r,m")))]
  "TARGET_ARM && arm_arch6"
  "@
   uxth%?\\t%0, %1
   ldr%(h%)\\t%0, %1"
  [(set_attr "predicable" "yes")
   (set_attr "type" "simple_alu_shift,load_byte")]
)

//Pattern#287
(define_insn "*arm_zero_extendhisi2addsi"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(plus:SI (zero_extend:SI (match_operand:HI 1 "s_register_operand" "r"))
		 (match_operand:SI 2 "s_register_operand" "r")))]
  "TARGET_INT_SIMD"
  "uxtah%?\\t%0, %2, %1"
  [(set_attr "type" "alu_shift")
   (set_attr "predicable" "yes")]
)

//Pattern#288
(define_expand "zero_extendqisi2"
  [(set (match_operand:SI 0 "s_register_operand" "")
	(zero_extend:SI (match_operand:QI 1 "nonimmediate_operand" "")))]
  "TARGET_EITHER"
{
  if (TARGET_ARM && !arm_arch6 && !MEM_P (operands[1]))
    {
      emit_insn (gen_andsi3 (operands[0],
			     gen_lowpart (SImode, operands[1]),
					  GEN_INT (255)));
      DONE;
    }
  if (!arm_arch6 && !MEM_P (operands[1]))
    {
      rtx t = gen_lowpart (SImode, operands[1]);
      rtx tmp = gen_reg_rtx (SImode);
      emit_insn (gen_ashlsi3 (tmp, t, GEN_INT (24)));
      emit_insn (gen_lshrsi3 (operands[0], tmp, GEN_INT (24)));
      DONE;
    }
})

//Pattern#289
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(zero_extend:SI (match_operand:QI 1 "s_register_operand" "")))]
  "!arm_arch6"
  [(set (match_dup 0) (ashift:SI (match_dup 2) (const_int 24)))
   (set (match_dup 0) (lshiftrt:SI (match_dup 0) (const_int 24)))]
{
  operands[2] = simplify_gen_subreg (SImode, operands[1], QImode, 0);
  if (TARGET_ARM)
    {
      emit_insn (gen_andsi3 (operands[0], operands[2], GEN_INT (255)));
      DONE;
    }
})

//Pattern#290
(define_insn "*thumb1_zero_extendqisi2"
  [(set (match_operand:SI 0 "register_operand" "=l,l")
	(zero_extend:SI (match_operand:QI 1 "nonimmediate_operand" "l,m")))]
  "TARGET_THUMB1 && !arm_arch6"
  "@
   #
   ldrb\\t%0, %1"
  [(set_attr "length" "4,2")
   (set_attr "type" "alu_shift,load_byte")
   (set_attr "pool_range" "*,32")]
)

//Pattern#291
(define_insn "*thumb1_zero_extendqisi2_v6"
  [(set (match_operand:SI 0 "register_operand" "=l,l")
	(zero_extend:SI (match_operand:QI 1 "nonimmediate_operand" "l,m")))]
  "TARGET_THUMB1 && arm_arch6"
  "@
   uxtb\\t%0, %1
   ldrb\\t%0, %1"
  [(set_attr "length" "2")
   (set_attr "type" "simple_alu_shift,load_byte")]
)

//Pattern#292
(define_insn "*arm_zero_extendqisi2"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(zero_extend:SI (match_operand:QI 1 "nonimmediate_operand" "r,m")))]
  "TARGET_ARM && !arm_arch6"
  "@
   #
   ldr%(b%)\\t%0, %1\\t%@ zero_extendqisi2"
  [(set_attr "length" "8,4")
   (set_attr "type" "alu_shift,load_byte")
   (set_attr "predicable" "yes")]
)

//Pattern#293
(define_insn "*arm_zero_extendqisi2_v6"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(zero_extend:SI (match_operand:QI 1 "nonimmediate_operand" "r,m")))]
  "TARGET_ARM && arm_arch6"
  "@
   uxtb%(%)\\t%0, %1
   ldr%(b%)\\t%0, %1\\t%@ zero_extendqisi2"
  [(set_attr "type" "simple_alu_shift,load_byte")
   (set_attr "predicable" "yes")]
)

//Pattern#294
(define_insn "*arm_zero_extendqisi2addsi"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(plus:SI (zero_extend:SI (match_operand:QI 1 "s_register_operand" "r"))
		 (match_operand:SI 2 "s_register_operand" "r")))]
  "TARGET_INT_SIMD"
  "uxtab%?\\t%0, %2, %1"
  [(set_attr "predicable" "yes")
   (set_attr "insn" "xtab")
   (set_attr "type" "alu_shift")]
)

//Pattern#295
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(zero_extend:SI (subreg:QI (match_operand:SI 1 "" "") 0)))
   (clobber (match_operand:SI 2 "s_register_operand" ""))]
  "TARGET_32BIT && (!MEM_P (operands[1])) && ! BYTES_BIG_ENDIAN"
  [(set (match_dup 2) (match_dup 1))
   (set (match_dup 0) (and:SI (match_dup 2) (const_int 255)))]
  ""
)

//Pattern#296
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(zero_extend:SI (subreg:QI (match_operand:SI 1 "" "") 3)))
   (clobber (match_operand:SI 2 "s_register_operand" ""))]
  "TARGET_32BIT && (!MEM_P (operands[1])) && BYTES_BIG_ENDIAN"
  [(set (match_dup 2) (match_dup 1))
   (set (match_dup 0) (and:SI (match_dup 2) (const_int 255)))]
  ""
)


//Pattern#297
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(ior_xor:SI (and:SI (ashift:SI
			     (match_operand:SI 1 "s_register_operand" "")
			     (match_operand:SI 2 "const_int_operand" ""))
			    (match_operand:SI 3 "const_int_operand" ""))
		    (zero_extend:SI
		     (match_operator 5 "subreg_lowpart_operator"
		      [(match_operand:SI 4 "s_register_operand" "")]))))]
  "TARGET_32BIT
   && ((unsigned HOST_WIDE_INT) INTVAL (operands[3])
       == (GET_MODE_MASK (GET_MODE (operands[5]))
           & (GET_MODE_MASK (GET_MODE (operands[5]))
	      << (INTVAL (operands[2])))))"
  [(set (match_dup 0) (ior_xor:SI (ashift:SI (match_dup 1) (match_dup 2))
				  (match_dup 4)))
   (set (match_dup 0) (zero_extend:SI (match_dup 5)))]
  "operands[5] = gen_lowpart (GET_MODE (operands[5]), operands[0]);"
)

//Pattern#298
(define_insn "*compareqi_eq0"
  [(set (reg:CC_Z CC_REGNUM)
	(compare:CC_Z (match_operand:QI 0 "s_register_operand" "r")
			 (const_int 0)))]
  "TARGET_32BIT"
  "tst%?\\t%0, #255"
  [(set_attr "conds" "set")
   (set_attr "predicable" "yes")]
)

//Pattern#299
(define_expand "extendhisi2"
  [(set (match_operand:SI 0 "s_register_operand" "")
	(sign_extend:SI (match_operand:HI 1 "nonimmediate_operand" "")))]
  "TARGET_EITHER"
{
  if (TARGET_THUMB1)
    {
      emit_insn (gen_thumb1_extendhisi2 (operands[0], operands[1]));
      DONE;
    }
  if (MEM_P (operands[1]) && TARGET_ARM && !arm_arch4)
    {
      emit_insn (gen_extendhisi2_mem (operands[0], operands[1]));
      DONE;
    }

  if (!arm_arch6 && !MEM_P (operands[1]))
    {
      rtx t = gen_lowpart (SImode, operands[1]);
      rtx tmp = gen_reg_rtx (SImode);
      emit_insn (gen_ashlsi3 (tmp, t, GEN_INT (16)));
      emit_insn (gen_ashrsi3 (operands[0], tmp, GEN_INT (16)));
      DONE;
    }
})

//Pattern#300
(define_split
  [(parallel
    [(set (match_operand:SI 0 "register_operand" "")
	  (sign_extend:SI (match_operand:HI 1 "register_operand" "")))
     (clobber (match_scratch:SI 2 ""))])]
  "!arm_arch6"
  [(set (match_dup 0) (ashift:SI (match_dup 2) (const_int 16)))
   (set (match_dup 0) (ashiftrt:SI (match_dup 0) (const_int 16)))]
{
  operands[2] = simplify_gen_subreg (SImode, operands[1], HImode, 0);
})

;; We used to have an early-clobber on the scratch register here.
;; However, there's a bug somewhere in reload which means that this
;; can be partially ignored during spill allocation if the memory
;; address also needs reloading; this causes us to die later on when
;; we try to verify the operands.  Fortunately, we don't really need
;; the early-clobber: we can always use operand 0 if operand 2
;; overlaps the address.
//Pattern#301
(define_insn "thumb1_extendhisi2"
  [(set (match_operand:SI 0 "register_operand" "=l,l")
	(sign_extend:SI (match_operand:HI 1 "nonimmediate_operand" "l,m")))
   (clobber (match_scratch:SI 2 "=X,l"))]
  "TARGET_THUMB1"
  "*
  {
    rtx ops[4];
    rtx mem;

    if (which_alternative == 0 && !arm_arch6)
      return \"#\";
    if (which_alternative == 0)
      return \"sxth\\t%0, %1\";

    mem = XEXP (operands[1], 0);

    /* This code used to try to use 'V', and fix the address only if it was
       offsettable, but this fails for e.g. REG+48 because 48 is outside the
       range of QImode offsets, and offsettable_address_p does a QImode
       address check.  */
       
    if (GET_CODE (mem) == CONST)
      mem = XEXP (mem, 0);
    
    if (GET_CODE (mem) == LABEL_REF)
      return \"ldr\\t%0, %1\";
    
    if (GET_CODE (mem) == PLUS)
      {
        rtx a = XEXP (mem, 0);
        rtx b = XEXP (mem, 1);

        if (GET_CODE (a) == LABEL_REF
	    && CONST_INT_P (b))
          return \"ldr\\t%0, %1\";

        if (REG_P (b))
          return \"ldrsh\\t%0, %1\";
	  
        ops[1] = a;
        ops[2] = b;
      }
    else
      {
        ops[1] = mem;
        ops[2] = const0_rtx;
      }
      
    gcc_assert (REG_P (ops[1]));

    ops[0] = operands[0];
    if (reg_mentioned_p (operands[2], ops[1]))
      ops[3] = ops[0];
    else
      ops[3] = operands[2];
    output_asm_insn (\"mov\\t%3, %2\;ldrsh\\t%0, [%1, %3]\", ops);
    return \"\";
  }"
  [(set_attr_alternative "length"
			 [(if_then_else (eq_attr "is_arch6" "yes")
					(const_int 2) (const_int 4))
			  (const_int 4)])
   (set_attr "type" "simple_alu_shift,load_byte")
   (set_attr "pool_range" "*,1018")]
)

;; This pattern will only be used when ldsh is not available
//Pattern#302
(define_expand "extendhisi2_mem"
  [(set (match_dup 2) (zero_extend:SI (match_operand:HI 1 "" "")))
   (set (match_dup 3)
	(zero_extend:SI (match_dup 7)))
   (set (match_dup 6) (ashift:SI (match_dup 4) (const_int 24)))
   (set (match_operand:SI 0 "" "")
	(ior:SI (ashiftrt:SI (match_dup 6) (const_int 16)) (match_dup 5)))]
  "TARGET_ARM"
  "
  {
    rtx mem1, mem2;
    rtx addr = copy_to_mode_reg (SImode, XEXP (operands[1], 0));

    mem1 = change_address (operands[1], QImode, addr);
    mem2 = change_address (operands[1], QImode,
			   plus_constant (Pmode, addr, 1));
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[1] = mem1;
    operands[2] = gen_reg_rtx (SImode);
    operands[3] = gen_reg_rtx (SImode);
    operands[6] = gen_reg_rtx (SImode);
    operands[7] = mem2;

    if (BYTES_BIG_ENDIAN)
      {
	operands[4] = operands[2];
	operands[5] = operands[3];
      }
    else
      {
	operands[4] = operands[3];
	operands[5] = operands[2];
      }
  }"
)

//Pattern#303
(define_split
  [(set (match_operand:SI 0 "register_operand" "")
	(sign_extend:SI (match_operand:HI 1 "register_operand" "")))]
  "!arm_arch6"
  [(set (match_dup 0) (ashift:SI (match_dup 2) (const_int 16)))
   (set (match_dup 0) (ashiftrt:SI (match_dup 0) (const_int 16)))]
{
  operands[2] = simplify_gen_subreg (SImode, operands[1], HImode, 0);
})

//Pattern#304
(define_insn "*arm_extendhisi2"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(sign_extend:SI (match_operand:HI 1 "nonimmediate_operand" "r,m")))]
  "TARGET_ARM && arm_arch4 && !arm_arch6"
  "@
   #
   ldr%(sh%)\\t%0, %1"
  [(set_attr "length" "8,4")
   (set_attr "type" "alu_shift,load_byte")
   (set_attr "predicable" "yes")
   (set_attr "pool_range" "*,256")
   (set_attr "neg_pool_range" "*,244")]
)

;; ??? Check Thumb-2 pool range
//Pattern#305
(define_insn "*arm_extendhisi2_v6"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(sign_extend:SI (match_operand:HI 1 "nonimmediate_operand" "r,m")))]
  "TARGET_32BIT && arm_arch6"
  "@
   sxth%?\\t%0, %1
   ldr%(sh%)\\t%0, %1"
  [(set_attr "type" "simple_alu_shift,load_byte")
   (set_attr "predicable" "yes")
   (set_attr "pool_range" "*,256")
   (set_attr "neg_pool_range" "*,244")]
)

//Pattern#306
(define_insn "*arm_extendhisi2addsi"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(plus:SI (sign_extend:SI (match_operand:HI 1 "s_register_operand" "r"))
		 (match_operand:SI 2 "s_register_operand" "r")))]
  "TARGET_INT_SIMD"
  "sxtah%?\\t%0, %2, %1"
)

//Pattern#307
(define_expand "extendqihi2"
  [(set (match_dup 2)
	(ashift:SI (match_operand:QI 1 "arm_reg_or_extendqisi_mem_op" "")
		   (const_int 24)))
   (set (match_operand:HI 0 "s_register_operand" "")
	(ashiftrt:SI (match_dup 2)
		     (const_int 24)))]
  "TARGET_ARM"
  "
  {
    if (arm_arch4 && MEM_P (operands[1]))
      {
	emit_insn (gen_rtx_SET (VOIDmode,
				operands[0],
				gen_rtx_SIGN_EXTEND (HImode, operands[1])));
	DONE;
      }
    if (!s_register_operand (operands[1], QImode))
      operands[1] = copy_to_mode_reg (QImode, operands[1]);
    operands[0] = gen_lowpart (SImode, operands[0]);
    operands[1] = gen_lowpart (SImode, operands[1]);
    operands[2] = gen_reg_rtx (SImode);
  }"
)

//Pattern#308
(define_insn "*arm_extendqihi_insn"
  [(set (match_operand:HI 0 "s_register_operand" "=r")
	(sign_extend:HI (match_operand:QI 1 "arm_extendqisi_mem_op" "Uq")))]
  "TARGET_ARM && arm_arch4"
  "ldr%(sb%)\\t%0, %1"
  [(set_attr "type" "load_byte")
   (set_attr "predicable" "yes")
   (set_attr "pool_range" "256")
   (set_attr "neg_pool_range" "244")]
)

//Pattern#309
(define_expand "extendqisi2"
  [(set (match_operand:SI 0 "s_register_operand" "")
	(sign_extend:SI (match_operand:QI 1 "arm_reg_or_extendqisi_mem_op" "")))]
  "TARGET_EITHER"
{
  if (!arm_arch4 && MEM_P (operands[1]))
    operands[1] = copy_to_mode_reg (QImode, operands[1]);

  if (!arm_arch6 && !MEM_P (operands[1]))
    {
      rtx t = gen_lowpart (SImode, operands[1]);
      rtx tmp = gen_reg_rtx (SImode);
      emit_insn (gen_ashlsi3 (tmp, t, GEN_INT (24)));
      emit_insn (gen_ashrsi3 (operands[0], tmp, GEN_INT (24)));
      DONE;
    }
})

//Pattern#310
(define_split
  [(set (match_operand:SI 0 "register_operand" "")
	(sign_extend:SI (match_operand:QI 1 "register_operand" "")))]
  "!arm_arch6"
  [(set (match_dup 0) (ashift:SI (match_dup 2) (const_int 24)))
   (set (match_dup 0) (ashiftrt:SI (match_dup 0) (const_int 24)))]
{
  operands[2] = simplify_gen_subreg (SImode, operands[1], QImode, 0);
})

//Pattern#311
(define_insn "*arm_extendqisi"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(sign_extend:SI (match_operand:QI 1 "arm_reg_or_extendqisi_mem_op" "r,Uq")))]
  "TARGET_ARM && arm_arch4 && !arm_arch6"
  "@
   #
   ldr%(sb%)\\t%0, %1"
  [(set_attr "length" "8,4")
   (set_attr "type" "alu_shift,load_byte")
   (set_attr "predicable" "yes")
   (set_attr "pool_range" "*,256")
   (set_attr "neg_pool_range" "*,244")]
)

//Pattern#312
(define_insn "*arm_extendqisi_v6"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(sign_extend:SI
	 (match_operand:QI 1 "arm_reg_or_extendqisi_mem_op" "r,Uq")))]
  "TARGET_ARM && arm_arch6"
  "@
   sxtb%?\\t%0, %1
   ldr%(sb%)\\t%0, %1"
  [(set_attr "type" "simple_alu_shift,load_byte")
   (set_attr "predicable" "yes")
   (set_attr "pool_range" "*,256")
   (set_attr "neg_pool_range" "*,244")]
)

//Pattern#313
(define_insn "*arm_extendqisi2addsi"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(plus:SI (sign_extend:SI (match_operand:QI 1 "s_register_operand" "r"))
		 (match_operand:SI 2 "s_register_operand" "r")))]
  "TARGET_INT_SIMD"
  "sxtab%?\\t%0, %2, %1"
  [(set_attr "type" "alu_shift")
   (set_attr "insn" "xtab")
   (set_attr "predicable" "yes")]
)

//Pattern#314
(define_split
  [(set (match_operand:SI 0 "register_operand" "")
	(sign_extend:SI (match_operand:QI 1 "memory_operand" "")))]
  "TARGET_THUMB1 && reload_completed"
  [(set (match_dup 0) (match_dup 2))
   (set (match_dup 0) (sign_extend:SI (match_dup 3)))]
{
  rtx addr = XEXP (operands[1], 0);

  if (GET_CODE (addr) == CONST)
    addr = XEXP (addr, 0);

  if (GET_CODE (addr) == PLUS
      && REG_P (XEXP (addr, 0)) && REG_P (XEXP (addr, 1)))
    /* No split necessary.  */
    FAIL;

  if (GET_CODE (addr) == PLUS
      && !REG_P (XEXP (addr, 0)) && !REG_P (XEXP (addr, 1)))
    FAIL;

  if (reg_overlap_mentioned_p (operands[0], addr))
    {
      rtx t = gen_lowpart (QImode, operands[0]);
      emit_move_insn (t, operands[1]);
      emit_insn (gen_thumb1_extendqisi2 (operands[0], t));
      DONE;
    }

  if (REG_P (addr))
    {
      addr = gen_rtx_PLUS (Pmode, addr, operands[0]);
      operands[2] = const0_rtx;
    }
  else if (GET_CODE (addr) != PLUS)
    FAIL;
  else if (REG_P (XEXP (addr, 0)))
    {
      operands[2] = XEXP (addr, 1);
      addr = gen_rtx_PLUS (Pmode, XEXP (addr, 0), operands[0]);
    }
  else
    {
      operands[2] = XEXP (addr, 0);
      addr = gen_rtx_PLUS (Pmode, XEXP (addr, 1), operands[0]);
    }

  operands[3] = change_address (operands[1], QImode, addr);
})

//Pattern#315
(define_peephole2
  [(set (match_operand:SI 0 "register_operand" "")
	(plus:SI (match_dup 0) (match_operand 1 "const_int_operand")))
   (set (match_operand:SI 2 "register_operand" "") (const_int 0))
   (set (match_operand:SI 3 "register_operand" "")
	(sign_extend:SI (match_operand:QI 4 "memory_operand" "")))]
  "TARGET_THUMB1
   && GET_CODE (XEXP (operands[4], 0)) == PLUS
   && rtx_equal_p (operands[0], XEXP (XEXP (operands[4], 0), 0))
   && rtx_equal_p (operands[2], XEXP (XEXP (operands[4], 0), 1))
   && (peep2_reg_dead_p (3, operands[0])
       || rtx_equal_p (operands[0], operands[3]))
   && (peep2_reg_dead_p (3, operands[2])
       || rtx_equal_p (operands[2], operands[3]))"
  [(set (match_dup 2) (match_dup 1))
   (set (match_dup 3) (sign_extend:SI (match_dup 4)))]
{
  rtx addr = gen_rtx_PLUS (Pmode, operands[0], operands[2]);
  operands[4] = change_address (operands[4], QImode, addr);
})

//Pattern#316
(define_insn "thumb1_extendqisi2"
  [(set (match_operand:SI 0 "register_operand" "=l,l,l")
	(sign_extend:SI (match_operand:QI 1 "nonimmediate_operand" "l,V,m")))]
  "TARGET_THUMB1"
{
  rtx addr;

  if (which_alternative == 0 && arm_arch6)
    return "sxtb\\t%0, %1";
  if (which_alternative == 0)
    return "#";

  addr = XEXP (operands[1], 0);
  if (GET_CODE (addr) == PLUS
      && REG_P (XEXP (addr, 0)) && REG_P (XEXP (addr, 1)))
    return "ldrsb\\t%0, %1";
      
  return "#";
}
  [(set_attr_alternative "length"
			 [(if_then_else (eq_attr "is_arch6" "yes")
					(const_int 2) (const_int 4))
			  (const_int 2)
			  (if_then_else (eq_attr "is_arch6" "yes")
					(const_int 4) (const_int 6))])
   (set_attr "type" "simple_alu_shift,load_byte,load_byte")]
)

//Pattern#317
(define_expand "extendsfdf2"
  [(set (match_operand:DF                  0 "s_register_operand" "")
	(float_extend:DF (match_operand:SF 1 "s_register_operand"  "")))]
  "TARGET_32BIT && TARGET_HARD_FLOAT && !TARGET_VFP_SINGLE"
  ""
)

/* HFmode -> DFmode conversions have to go through SFmode.  */
//Pattern#318
(define_expand "extendhfdf2"
  [(set (match_operand:DF                  0 "general_operand" "")
	(float_extend:DF (match_operand:HF 1 "general_operand"  "")))]
  "TARGET_EITHER"
  "
  {
    rtx op1;
    op1 = convert_to_mode (SFmode, operands[1], 0);
    op1 = convert_to_mode (DFmode, op1, 0);
    emit_insn (gen_movdf (operands[0], op1));
    DONE;
  }"
)

;; Move insns (including loads and stores)

;; XXX Just some ideas about movti.
;; I don't think these are a good idea on the arm, there just aren't enough
;; registers
(define_insn "nop"
  [(const_int 0)]
  "TARGET_EITHER"
  "*
  if (TARGET_UNIFIED_ASM)
    return \"nop\";
  if (TARGET_ARM)
    return \"mov%?\\t%|r0, %|r0\\t%@ nop\";
  return  \"mov\\tr8, r8\";
  "
  [(set (attr "length")
	(if_then_else (eq_attr "is_thumb" "yes")
		      (const_int 2)
		      (const_int 4)))]
)


;; Patterns to allow combination of arithmetic, cond code and shifts

//Pattern#473
(define_insn "*arith_shiftsi"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r,r,r")
        (match_operator:SI 1 "shiftable_operator"
          [(match_operator:SI 3 "shift_operator"
             [(match_operand:SI 4 "s_register_operand" "r,r,r,r")
              (match_operand:SI 5 "shift_amount_operand" "M,M,M,r")])
           (match_operand:SI 2 "s_register_operand" "rk,rk,r,rk")]))]
  "TARGET_32BIT"
  "%i1%?\\t%0, %2, %4%S3"
  [(set_attr "predicable" "yes")
   (set_attr "shift" "4")
   (set_attr "arch" "a,t2,t2,a")
   ;; Thumb2 doesn't allow the stack pointer to be used for 
   ;; operand1 for all operations other than add and sub. In this case 
   ;; the minus operation is a candidate for an rsub and hence needs
   ;; to be disabled.
   ;; We have to make sure to disable the fourth alternative if
   ;; the shift_operator is MULT, since otherwise the insn will
   ;; also match a multiply_accumulate pattern and validate_change
   ;; will allow a replacement of the constant with a register
   ;; despite the checks done in shift_operator.
   (set_attr_alternative "insn_enabled"
			 [(const_string "yes")
			  (if_then_else
			   (match_operand:SI 1 "add_operator" "")
			   (const_string "yes") (const_string "no"))
			  (const_string "yes")
			  (if_then_else
			   (match_operand:SI 3 "mult_operator" "")
			   (const_string "no") (const_string "yes"))])
   (set_attr "type" "alu_shift,alu_shift,alu_shift,alu_shift_reg")])

//Pattern#474
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(match_operator:SI 1 "shiftable_operator"
	 [(match_operator:SI 2 "shiftable_operator"
	   [(match_operator:SI 3 "shift_operator"
	     [(match_operand:SI 4 "s_register_operand" "")
	      (match_operand:SI 5 "reg_or_int_operand" "")])
	    (match_operand:SI 6 "s_register_operand" "")])
	  (match_operand:SI 7 "arm_rhs_operand" "")]))
   (clobber (match_operand:SI 8 "s_register_operand" ""))]
  "TARGET_32BIT"
  [(set (match_dup 8)
	(match_op_dup 2 [(match_op_dup 3 [(match_dup 4) (match_dup 5)])
			 (match_dup 6)]))
   (set (match_dup 0)
	(match_op_dup 1 [(match_dup 8) (match_dup 7)]))]
  "")

//Pattern#475
(define_insn "*arith_shiftsi_compare0"
  [(set (reg:CC_NOOV CC_REGNUM)
        (compare:CC_NOOV
	 (match_operator:SI 1 "shiftable_operator"
	  [(match_operator:SI 3 "shift_operator"
	    [(match_operand:SI 4 "s_register_operand" "r,r")
	     (match_operand:SI 5 "shift_amount_operand" "M,r")])
	   (match_operand:SI 2 "s_register_operand" "r,r")])
	 (const_int 0)))
   (set (match_operand:SI 0 "s_register_operand" "=r,r")
	(match_op_dup 1 [(match_op_dup 3 [(match_dup 4) (match_dup 5)])
			 (match_dup 2)]))]
  "TARGET_32BIT"
  "%i1%.\\t%0, %2, %4%S3"
  [(set_attr "conds" "set")
   (set_attr "shift" "4")
   (set_attr "arch" "32,a")
   (set_attr "type" "alu_shift,alu_shift_reg")])

//Pattern#476
(define_insn "*arith_shiftsi_compare0_scratch"
  [(set (reg:CC_NOOV CC_REGNUM)
        (compare:CC_NOOV
	 (match_operator:SI 1 "shiftable_operator"
	  [(match_operator:SI 3 "shift_operator"
	    [(match_operand:SI 4 "s_register_operand" "r,r")
	     (match_operand:SI 5 "shift_amount_operand" "M,r")])
	   (match_operand:SI 2 "s_register_operand" "r,r")])
	 (const_int 0)))
   (clobber (match_scratch:SI 0 "=r,r"))]
  "TARGET_32BIT"
  "%i1%.\\t%0, %2, %4%S3"
  [(set_attr "conds" "set")
   (set_attr "shift" "4")
   (set_attr "arch" "32,a")
   (set_attr "type" "alu_shift,alu_shift_reg")])

//Pattern#477
(define_insn "*sub_shiftsi"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(minus:SI (match_operand:SI 1 "s_register_operand" "r,r")
		  (match_operator:SI 2 "shift_operator"
		   [(match_operand:SI 3 "s_register_operand" "r,r")
		    (match_operand:SI 4 "shift_amount_operand" "M,r")])))]
  "TARGET_32BIT"
  "sub%?\\t%0, %1, %3%S2"
  [(set_attr "predicable" "yes")
   (set_attr "shift" "3")
   (set_attr "arch" "32,a")
   (set_attr "type" "alu_shift,alu_shift_reg")])

//Pattern#478
(define_insn "*sub_shiftsi_compare0"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV
	 (minus:SI (match_operand:SI 1 "s_register_operand" "r,r")
		   (match_operator:SI 2 "shift_operator"
		    [(match_operand:SI 3 "s_register_operand" "r,r")
		     (match_operand:SI 4 "shift_amount_operand" "M,rM")]))
	 (const_int 0)))
   (set (match_operand:SI 0 "s_register_operand" "=r,r")
	(minus:SI (match_dup 1)
		  (match_op_dup 2 [(match_dup 3) (match_dup 4)])))]
  "TARGET_32BIT"
  "sub%.\\t%0, %1, %3%S2"
  [(set_attr "conds" "set")
   (set_attr "shift" "3")
   (set_attr "arch" "32,a")
   (set_attr "type" "alu_shift,alu_shift_reg")])

//Pattern#479
(define_insn "*sub_shiftsi_compare0_scratch"
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV
	 (minus:SI (match_operand:SI 1 "s_register_operand" "r,r")
		   (match_operator:SI 2 "shift_operator"
		    [(match_operand:SI 3 "s_register_operand" "r,r")
		     (match_operand:SI 4 "shift_amount_operand" "M,rM")]))
	 (const_int 0)))
   (clobber (match_scratch:SI 0 "=r,r"))]
  "TARGET_32BIT"
  "sub%.\\t%0, %1, %3%S2"
  [(set_attr "conds" "set")
   (set_attr "shift" "3")
   (set_attr "arch" "32,a")
   (set_attr "type" "alu_shift,alu_shift_reg")])


//Pattern#480
(define_insn "*and_scc"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(and:SI (match_operator:SI 1 "arm_comparison_operator"
		 [(match_operand 3 "cc_register" "") (const_int 0)])
		(match_operand:SI 2 "s_register_operand" "r")))]
  "TARGET_ARM"
  "mov%D1\\t%0, #0\;and%d1\\t%0, %2, #1"
  [(set_attr "conds" "use")
   (set_attr "insn" "mov")
   (set_attr "length" "8")]
)

//Pattern#481
(define_insn "*ior_scc"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(ior:SI (match_operator:SI 2 "arm_comparison_operator"
		 [(match_operand 3 "cc_register" "") (const_int 0)])
		(match_operand:SI 1 "s_register_operand" "0,?r")))]
  "TARGET_ARM"
  "@
   orr%d2\\t%0, %1, #1
   mov%D2\\t%0, %1\;orr%d2\\t%0, %1, #1"
  [(set_attr "conds" "use")
   (set_attr "length" "4,8")]
)

; A series of splitters for the compare_scc pattern below.  Note that
; order is important.
//Pattern#482
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(lt:SI (match_operand:SI 1 "s_register_operand" "")
	       (const_int 0)))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT && reload_completed"
  [(set (match_dup 0) (lshiftrt:SI (match_dup 1) (const_int 31)))])

//Pattern#483
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(ge:SI (match_operand:SI 1 "s_register_operand" "")
	       (const_int 0)))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT && reload_completed"
  [(set (match_dup 0) (not:SI (match_dup 1)))
   (set (match_dup 0) (lshiftrt:SI (match_dup 0) (const_int 31)))])

//Pattern#484
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(eq:SI (match_operand:SI 1 "s_register_operand" "")
	       (const_int 0)))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT && reload_completed"
  [(parallel
    [(set (reg:CC CC_REGNUM)
	  (compare:CC (const_int 1) (match_dup 1)))
     (set (match_dup 0)
	  (minus:SI (const_int 1) (match_dup 1)))])
   (cond_exec (ltu:CC (reg:CC CC_REGNUM) (const_int 0))
	      (set (match_dup 0) (const_int 0)))])

//Pattern#485
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(ne:SI (match_operand:SI 1 "s_register_operand" "")
	       (match_operand:SI 2 "const_int_operand" "")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT && reload_completed"
  [(parallel
    [(set (reg:CC CC_REGNUM)
	  (compare:CC (match_dup 1) (match_dup 2)))
     (set (match_dup 0) (plus:SI (match_dup 1) (match_dup 3)))])
   (cond_exec (ne:CC (reg:CC CC_REGNUM) (const_int 0))
	      (set (match_dup 0) (const_int 1)))]
{
  operands[3] = GEN_INT (-INTVAL (operands[2]));
})

//Pattern#486
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(ne:SI (match_operand:SI 1 "s_register_operand" "")
	       (match_operand:SI 2 "arm_add_operand" "")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT && reload_completed"
  [(parallel
    [(set (reg:CC_NOOV CC_REGNUM)
	  (compare:CC_NOOV (minus:SI (match_dup 1) (match_dup 2))
			   (const_int 0)))
     (set (match_dup 0) (minus:SI (match_dup 1) (match_dup 2)))])
   (cond_exec (ne:CC_NOOV (reg:CC_NOOV CC_REGNUM) (const_int 0))
	      (set (match_dup 0) (const_int 1)))])

//Pattern#487
(define_insn_and_split "*compare_scc"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(match_operator:SI 1 "arm_comparison_operator"
	 [(match_operand:SI 2 "s_register_operand" "r,r")
	  (match_operand:SI 3 "arm_add_operand" "rI,L")]))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT"
  "#"
  "&& reload_completed"
  [(set (reg:CC CC_REGNUM) (compare:CC (match_dup 2) (match_dup 3)))
   (cond_exec (match_dup 4) (set (match_dup 0) (const_int 0)))
   (cond_exec (match_dup 5) (set (match_dup 0) (const_int 1)))]
{
  rtx tmp1;
  enum machine_mode mode = SELECT_CC_MODE (GET_CODE (operands[1]),
					   operands[2], operands[3]);
  enum rtx_code rc = GET_CODE (operands[1]);

  tmp1 = gen_rtx_REG (mode, CC_REGNUM);

  operands[5] = gen_rtx_fmt_ee (rc, VOIDmode, tmp1, const0_rtx);
  if (mode == CCFPmode || mode == CCFPEmode)
    rc = reverse_condition_maybe_unordered (rc);
  else
    rc = reverse_condition (rc);
  operands[4] = gen_rtx_fmt_ee (rc, VOIDmode, tmp1, const0_rtx);
})

;; Attempt to improve the sequence generated by the compare_scc splitters
;; not to use conditional execution.
//Pattern#488
(define_peephole2
  [(set (reg:CC CC_REGNUM)
	(compare:CC (match_operand:SI 1 "register_operand" "")
		    (match_operand:SI 2 "arm_rhs_operand" "")))
   (cond_exec (ne (reg:CC CC_REGNUM) (const_int 0))
	      (set (match_operand:SI 0 "register_operand" "") (const_int 0)))
   (cond_exec (eq (reg:CC CC_REGNUM) (const_int 0))
	      (set (match_dup 0) (const_int 1)))
   (match_scratch:SI 3 "r")]
  "TARGET_32BIT"
  [(parallel
    [(set (reg:CC CC_REGNUM)
	  (compare:CC (match_dup 1) (match_dup 2)))
     (set (match_dup 3) (minus:SI (match_dup 1) (match_dup 2)))])
   (parallel
    [(set (reg:CC CC_REGNUM)
	  (compare:CC (const_int 0) (match_dup 3)))
     (set (match_dup 0) (minus:SI (const_int 0) (match_dup 3)))])
   (parallel
    [(set (match_dup 0)
	  (plus:SI (plus:SI (match_dup 0) (match_dup 3))
		   (geu:SI (reg:CC CC_REGNUM) (const_int 0))))
     (clobber (reg:CC CC_REGNUM))])])

//Pattern#489
(define_insn "*cond_move"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r,r")
	(if_then_else:SI (match_operator 3 "equality_operator"
			  [(match_operator 4 "arm_comparison_operator"
			    [(match_operand 5 "cc_register" "") (const_int 0)])
			   (const_int 0)])
			 (match_operand:SI 1 "arm_rhs_operand" "0,rI,?rI")
			 (match_operand:SI 2 "arm_rhs_operand" "rI,0,rI")))]
  "TARGET_ARM"
  "*
    if (GET_CODE (operands[3]) == NE)
      {
        if (which_alternative != 1)
	  output_asm_insn (\"mov%D4\\t%0, %2\", operands);
        if (which_alternative != 0)
	  output_asm_insn (\"mov%d4\\t%0, %1\", operands);
        return \"\";
      }
    if (which_alternative != 0)
      output_asm_insn (\"mov%D4\\t%0, %1\", operands);
    if (which_alternative != 1)
      output_asm_insn (\"mov%d4\\t%0, %2\", operands);
    return \"\";
  "
  [(set_attr "conds" "use")
   (set_attr "insn" "mov")
   (set_attr "length" "4,4,8")]
)

//Pattern#490
(define_insn "*cond_arith"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
        (match_operator:SI 5 "shiftable_operator" 
	 [(match_operator:SI 4 "arm_comparison_operator"
           [(match_operand:SI 2 "s_register_operand" "r,r")
	    (match_operand:SI 3 "arm_rhs_operand" "rI,rI")])
          (match_operand:SI 1 "s_register_operand" "0,?r")]))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "*
    if (GET_CODE (operands[4]) == LT && operands[3] == const0_rtx)
      return \"%i5\\t%0, %1, %2, lsr #31\";

    output_asm_insn (\"cmp\\t%2, %3\", operands);
    if (GET_CODE (operands[5]) == AND)
      output_asm_insn (\"mov%D4\\t%0, #0\", operands);
    else if (GET_CODE (operands[5]) == MINUS)
      output_asm_insn (\"rsb%D4\\t%0, %1, #0\", operands);
    else if (which_alternative != 0)
      output_asm_insn (\"mov%D4\\t%0, %1\", operands);
    return \"%i5%d4\\t%0, %1, #1\";
  "
  [(set_attr "conds" "clob")
   (set_attr "length" "12")]
)

//Pattern#491
(define_insn "*cond_sub"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
        (minus:SI (match_operand:SI 1 "s_register_operand" "0,?r")
		  (match_operator:SI 4 "arm_comparison_operator"
                   [(match_operand:SI 2 "s_register_operand" "r,r")
		    (match_operand:SI 3 "arm_rhs_operand" "rI,rI")])))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "*
    output_asm_insn (\"cmp\\t%2, %3\", operands);
    if (which_alternative != 0)
      output_asm_insn (\"mov%D4\\t%0, %1\", operands);
    return \"sub%d4\\t%0, %1, #1\";
  "
  [(set_attr "conds" "clob")
   (set_attr "length" "8,12")]
)

//Pattern#492
(define_insn "*cmp_ite0"
  [(set (match_operand 6 "dominant_cc_register" "")
	(compare
	 (if_then_else:SI
	  (match_operator 4 "arm_comparison_operator"
	   [(match_operand:SI 0 "s_register_operand"
	        "l,l,l,r,r,r,r,r,r")
	    (match_operand:SI 1 "arm_add_operand"
	        "lPy,lPy,lPy,rI,L,rI,L,rI,L")])
	  (match_operator:SI 5 "arm_comparison_operator"
	   [(match_operand:SI 2 "s_register_operand"
	        "l,r,r,l,l,r,r,r,r")
	    (match_operand:SI 3 "arm_add_operand"
	        "lPy,rI,L,lPy,lPy,rI,rI,L,L")])
	  (const_int 0))
	 (const_int 0)))]
  "TARGET_32BIT"
  "*
  {
    static const char * const cmp1[NUM_OF_COND_CMP][2] =
    {
      {\"cmp%d5\\t%0, %1\",
       \"cmp%d4\\t%2, %3\"},
      {\"cmn%d5\\t%0, #%n1\",
       \"cmp%d4\\t%2, %3\"},
      {\"cmp%d5\\t%0, %1\",
       \"cmn%d4\\t%2, #%n3\"},
      {\"cmn%d5\\t%0, #%n1\",
       \"cmn%d4\\t%2, #%n3\"}
    };
    static const char * const cmp2[NUM_OF_COND_CMP][2] =
    {
      {\"cmp\\t%2, %3\",
       \"cmp\\t%0, %1\"},
      {\"cmp\\t%2, %3\",
       \"cmn\\t%0, #%n1\"},
      {\"cmn\\t%2, #%n3\",
       \"cmp\\t%0, %1\"},
      {\"cmn\\t%2, #%n3\",
       \"cmn\\t%0, #%n1\"}
    };
    static const char * const ite[2] =
    {
      \"it\\t%d5\",
      \"it\\t%d4\"
    };
    static const int cmp_idx[9] = {CMP_CMP, CMP_CMP, CMP_CMN,
                                   CMP_CMP, CMN_CMP, CMP_CMP,
                                   CMN_CMP, CMP_CMN, CMN_CMN};
    int swap =
      comparison_dominates_p (GET_CODE (operands[5]), GET_CODE (operands[4]));

    output_asm_insn (cmp2[cmp_idx[which_alternative]][swap], operands);
    if (TARGET_THUMB2) {
      output_asm_insn (ite[swap], operands);
    }
    output_asm_insn (cmp1[cmp_idx[which_alternative]][swap], operands);
    return \"\";
  }"
  [(set_attr "conds" "set")
   (set_attr "arch" "t2,t2,t2,t2,t2,any,any,any,any")
   (set_attr_alternative "length"
      [(const_int 6)
       (const_int 8)
       (const_int 8)
       (const_int 8)
       (const_int 8)
       (if_then_else (eq_attr "is_thumb" "no")
           (const_int 8)
           (const_int 10))
       (if_then_else (eq_attr "is_thumb" "no")
           (const_int 8)
           (const_int 10))
       (if_then_else (eq_attr "is_thumb" "no")
           (const_int 8)
           (const_int 10))
       (if_then_else (eq_attr "is_thumb" "no")
           (const_int 8)
           (const_int 10))])]
)

//Pattern#493
(define_insn "*cmp_ite1"
  [(set (match_operand 6 "dominant_cc_register" "")
	(compare
	 (if_then_else:SI
	  (match_operator 4 "arm_comparison_operator"
	   [(match_operand:SI 0 "s_register_operand"
	        "l,l,l,r,r,r,r,r,r")
	    (match_operand:SI 1 "arm_add_operand"
	        "lPy,lPy,lPy,rI,L,rI,L,rI,L")])
	  (match_operator:SI 5 "arm_comparison_operator"
	   [(match_operand:SI 2 "s_register_operand"
	        "l,r,r,l,l,r,r,r,r")
	    (match_operand:SI 3 "arm_add_operand"
	        "lPy,rI,L,lPy,lPy,rI,rI,L,L")])
	  (const_int 1))
	 (const_int 0)))]
  "TARGET_32BIT"
  "*
  {
    static const char * const cmp1[NUM_OF_COND_CMP][2] =
    {
      {\"cmp\\t%0, %1\",
       \"cmp\\t%2, %3\"},
      {\"cmn\\t%0, #%n1\",
       \"cmp\\t%2, %3\"},
      {\"cmp\\t%0, %1\",
       \"cmn\\t%2, #%n3\"},
      {\"cmn\\t%0, #%n1\",
       \"cmn\\t%2, #%n3\"}
    };
    static const char * const cmp2[NUM_OF_COND_CMP][2] =
    {
      {\"cmp%d4\\t%2, %3\",
       \"cmp%D5\\t%0, %1\"},
      {\"cmp%d4\\t%2, %3\",
       \"cmn%D5\\t%0, #%n1\"},
      {\"cmn%d4\\t%2, #%n3\",
       \"cmp%D5\\t%0, %1\"},
      {\"cmn%d4\\t%2, #%n3\",
       \"cmn%D5\\t%0, #%n1\"}
    };
    static const char * const ite[2] =
    {
      \"it\\t%d4\",
      \"it\\t%D5\"
    };
    static const int cmp_idx[9] = {CMP_CMP, CMP_CMP, CMP_CMN,
                                   CMP_CMP, CMN_CMP, CMP_CMP,
                                   CMN_CMP, CMP_CMN, CMN_CMN};
    int swap =
      comparison_dominates_p (GET_CODE (operands[5]),
			      reverse_condition (GET_CODE (operands[4])));

    output_asm_insn (cmp1[cmp_idx[which_alternative]][swap], operands);
    if (TARGET_THUMB2) {
      output_asm_insn (ite[swap], operands);
    }
    output_asm_insn (cmp2[cmp_idx[which_alternative]][swap], operands);
    return \"\";
  }"
  [(set_attr "conds" "set")
   (set_attr "arch" "t2,t2,t2,t2,t2,any,any,any,any")
   (set_attr_alternative "length"
      [(const_int 6)
       (const_int 8)
       (const_int 8)
       (const_int 8)
       (const_int 8)
       (if_then_else (eq_attr "is_thumb" "no")
           (const_int 8)
           (const_int 10))
       (if_then_else (eq_attr "is_thumb" "no")
           (const_int 8)
           (const_int 10))
       (if_then_else (eq_attr "is_thumb" "no")
           (const_int 8)
           (const_int 10))
       (if_then_else (eq_attr "is_thumb" "no")
           (const_int 8)
           (const_int 10))])]
)

//Pattern#494
(define_insn "*cmp_and"
  [(set (match_operand 6 "dominant_cc_register" "")
	(compare
	 (and:SI
	  (match_operator 4 "arm_comparison_operator"
	   [(match_operand:SI 0 "s_register_operand" 
	        "l,l,l,r,r,r,r,r,r")
	    (match_operand:SI 1 "arm_add_operand" 
	        "lPy,lPy,lPy,rI,L,rI,L,rI,L")])
	  (match_operator:SI 5 "arm_comparison_operator"
	   [(match_operand:SI 2 "s_register_operand" 
	        "l,r,r,l,l,r,r,r,r")
	    (match_operand:SI 3 "arm_add_operand" 
	        "lPy,rI,L,lPy,lPy,rI,rI,L,L")]))
	 (const_int 0)))]
  "TARGET_32BIT"
  "*
  {
    static const char *const cmp1[NUM_OF_COND_CMP][2] =
    {
      {\"cmp%d5\\t%0, %1\",
       \"cmp%d4\\t%2, %3\"},
      {\"cmn%d5\\t%0, #%n1\",
       \"cmp%d4\\t%2, %3\"},
      {\"cmp%d5\\t%0, %1\",
       \"cmn%d4\\t%2, #%n3\"},
      {\"cmn%d5\\t%0, #%n1\",
       \"cmn%d4\\t%2, #%n3\"}
    };
    static const char *const cmp2[NUM_OF_COND_CMP][2] =
    {
      {\"cmp\\t%2, %3\",
       \"cmp\\t%0, %1\"},
      {\"cmp\\t%2, %3\",
       \"cmn\\t%0, #%n1\"},
      {\"cmn\\t%2, #%n3\",
       \"cmp\\t%0, %1\"},
      {\"cmn\\t%2, #%n3\",
       \"cmn\\t%0, #%n1\"}
    };
    static const char *const ite[2] =
    {
      \"it\\t%d5\",
      \"it\\t%d4\"
    };
    static const int cmp_idx[9] = {CMP_CMP, CMP_CMP, CMP_CMN,
                                   CMP_CMP, CMN_CMP, CMP_CMP,
                                   CMN_CMP, CMP_CMN, CMN_CMN};
    int swap =
      comparison_dominates_p (GET_CODE (operands[5]), GET_CODE (operands[4]));

    output_asm_insn (cmp2[cmp_idx[which_alternative]][swap], operands);
    if (TARGET_THUMB2) {
      output_asm_insn (ite[swap], operands);
    }
    output_asm_insn (cmp1[cmp_idx[which_alternative]][swap], operands);
    return \"\";
  }"
  [(set_attr "conds" "set")
   (set_attr "predicable" "no")
   (set_attr "arch" "t2,t2,t2,t2,t2,any,any,any,any")
   (set_attr_alternative "length"
      [(const_int 6)
       (const_int 8)
       (const_int 8)
       (const_int 8)
       (const_int 8)
       (if_then_else (eq_attr "is_thumb" "no")
           (const_int 8)
           (const_int 10))
       (if_then_else (eq_attr "is_thumb" "no")
           (const_int 8)
           (const_int 10))
       (if_then_else (eq_attr "is_thumb" "no")
           (const_int 8)
           (const_int 10))
       (if_then_else (eq_attr "is_thumb" "no")
           (const_int 8)
           (const_int 10))])]
)

//Pattern#495
(define_insn "*cmp_ior"
  [(set (match_operand 6 "dominant_cc_register" "")
	(compare
	 (ior:SI
	  (match_operator 4 "arm_comparison_operator"
	   [(match_operand:SI 0 "s_register_operand"
	        "l,l,l,r,r,r,r,r,r")
	    (match_operand:SI 1 "arm_add_operand"
	        "lPy,lPy,lPy,rI,L,rI,L,rI,L")])
	  (match_operator:SI 5 "arm_comparison_operator"
	   [(match_operand:SI 2 "s_register_operand"
	        "l,r,r,l,l,r,r,r,r")
	    (match_operand:SI 3 "arm_add_operand"
	        "lPy,rI,L,lPy,lPy,rI,rI,L,L")]))
	 (const_int 0)))]
  "TARGET_32BIT"
  "*
  {
    static const char *const cmp1[NUM_OF_COND_CMP][2] =
    {
      {\"cmp\\t%0, %1\",
       \"cmp\\t%2, %3\"},
      {\"cmn\\t%0, #%n1\",
       \"cmp\\t%2, %3\"},
      {\"cmp\\t%0, %1\",
       \"cmn\\t%2, #%n3\"},
      {\"cmn\\t%0, #%n1\",
       \"cmn\\t%2, #%n3\"}
    };
    static const char *const cmp2[NUM_OF_COND_CMP][2] =
    {
      {\"cmp%D4\\t%2, %3\",
       \"cmp%D5\\t%0, %1\"},
      {\"cmp%D4\\t%2, %3\",
       \"cmn%D5\\t%0, #%n1\"},
      {\"cmn%D4\\t%2, #%n3\",
       \"cmp%D5\\t%0, %1\"},
      {\"cmn%D4\\t%2, #%n3\",
       \"cmn%D5\\t%0, #%n1\"}
    };
    static const char *const ite[2] =
    {
      \"it\\t%D4\",
      \"it\\t%D5\"
    };
    static const int cmp_idx[9] = {CMP_CMP, CMP_CMP, CMP_CMN,
                                   CMP_CMP, CMN_CMP, CMP_CMP,
                                   CMN_CMP, CMP_CMN, CMN_CMN};
    int swap =
      comparison_dominates_p (GET_CODE (operands[5]), GET_CODE (operands[4]));

    output_asm_insn (cmp1[cmp_idx[which_alternative]][swap], operands);
    if (TARGET_THUMB2) {
      output_asm_insn (ite[swap], operands);
    }
    output_asm_insn (cmp2[cmp_idx[which_alternative]][swap], operands);
    return \"\";
  }
  "
  [(set_attr "conds" "set")
   (set_attr "arch" "t2,t2,t2,t2,t2,any,any,any,any")
   (set_attr_alternative "length"
      [(const_int 6)
       (const_int 8)
       (const_int 8)
       (const_int 8)
       (const_int 8)
       (if_then_else (eq_attr "is_thumb" "no")
           (const_int 8)
           (const_int 10))
       (if_then_else (eq_attr "is_thumb" "no")
           (const_int 8)
           (const_int 10))
       (if_then_else (eq_attr "is_thumb" "no")
           (const_int 8)
           (const_int 10))
       (if_then_else (eq_attr "is_thumb" "no")
           (const_int 8)
           (const_int 10))])]
)

//Pattern#496
(define_insn_and_split "*ior_scc_scc"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(ior:SI (match_operator:SI 3 "arm_comparison_operator"
		 [(match_operand:SI 1 "s_register_operand" "r")
		  (match_operand:SI 2 "arm_add_operand" "rIL")])
		(match_operator:SI 6 "arm_comparison_operator"
		 [(match_operand:SI 4 "s_register_operand" "r")
		  (match_operand:SI 5 "arm_add_operand" "rIL")])))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT
   && (arm_select_dominance_cc_mode (operands[3], operands[6], DOM_CC_X_OR_Y)
       != CCmode)"
  "#"
  "TARGET_32BIT && reload_completed"
  [(set (match_dup 7)
	(compare
	 (ior:SI
	  (match_op_dup 3 [(match_dup 1) (match_dup 2)])
	  (match_op_dup 6 [(match_dup 4) (match_dup 5)]))
	 (const_int 0)))
   (set (match_dup 0) (ne:SI (match_dup 7) (const_int 0)))]
  "operands[7]
     = gen_rtx_REG (arm_select_dominance_cc_mode (operands[3], operands[6],
						  DOM_CC_X_OR_Y),
		    CC_REGNUM);"
  [(set_attr "conds" "clob")
   (set_attr "length" "16")])

; If the above pattern is followed by a CMP insn, then the compare is 
; redundant, since we can rework the conditional instruction that follows.
//Pattern#497
(define_insn_and_split "*ior_scc_scc_cmp"
  [(set (match_operand 0 "dominant_cc_register" "")
	(compare (ior:SI (match_operator:SI 3 "arm_comparison_operator"
			  [(match_operand:SI 1 "s_register_operand" "r")
			   (match_operand:SI 2 "arm_add_operand" "rIL")])
			 (match_operator:SI 6 "arm_comparison_operator"
			  [(match_operand:SI 4 "s_register_operand" "r")
			   (match_operand:SI 5 "arm_add_operand" "rIL")]))
		 (const_int 0)))
   (set (match_operand:SI 7 "s_register_operand" "=r")
	(ior:SI (match_op_dup 3 [(match_dup 1) (match_dup 2)])
		(match_op_dup 6 [(match_dup 4) (match_dup 5)])))]
  "TARGET_32BIT"
  "#"
  "TARGET_32BIT && reload_completed"
  [(set (match_dup 0)
	(compare
	 (ior:SI
	  (match_op_dup 3 [(match_dup 1) (match_dup 2)])
	  (match_op_dup 6 [(match_dup 4) (match_dup 5)]))
	 (const_int 0)))
   (set (match_dup 7) (ne:SI (match_dup 0) (const_int 0)))]
  ""
  [(set_attr "conds" "set")
   (set_attr "length" "16")])

//Pattern#498
(define_insn_and_split "*and_scc_scc"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(and:SI (match_operator:SI 3 "arm_comparison_operator"
		 [(match_operand:SI 1 "s_register_operand" "r")
		  (match_operand:SI 2 "arm_add_operand" "rIL")])
		(match_operator:SI 6 "arm_comparison_operator"
		 [(match_operand:SI 4 "s_register_operand" "r")
		  (match_operand:SI 5 "arm_add_operand" "rIL")])))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT
   && (arm_select_dominance_cc_mode (operands[3], operands[6], DOM_CC_X_AND_Y)
       != CCmode)"
  "#"
  "TARGET_32BIT && reload_completed
   && (arm_select_dominance_cc_mode (operands[3], operands[6], DOM_CC_X_AND_Y)
       != CCmode)"
  [(set (match_dup 7)
	(compare
	 (and:SI
	  (match_op_dup 3 [(match_dup 1) (match_dup 2)])
	  (match_op_dup 6 [(match_dup 4) (match_dup 5)]))
	 (const_int 0)))
   (set (match_dup 0) (ne:SI (match_dup 7) (const_int 0)))]
  "operands[7]
     = gen_rtx_REG (arm_select_dominance_cc_mode (operands[3], operands[6],
						  DOM_CC_X_AND_Y),
		    CC_REGNUM);"
  [(set_attr "conds" "clob")
   (set_attr "length" "16")])

; If the above pattern is followed by a CMP insn, then the compare is 
; redundant, since we can rework the conditional instruction that follows.
//Pattern#499
(define_insn_and_split "*and_scc_scc_cmp"
  [(set (match_operand 0 "dominant_cc_register" "")
	(compare (and:SI (match_operator:SI 3 "arm_comparison_operator"
			  [(match_operand:SI 1 "s_register_operand" "r")
			   (match_operand:SI 2 "arm_add_operand" "rIL")])
			 (match_operator:SI 6 "arm_comparison_operator"
			  [(match_operand:SI 4 "s_register_operand" "r")
			   (match_operand:SI 5 "arm_add_operand" "rIL")]))
		 (const_int 0)))
   (set (match_operand:SI 7 "s_register_operand" "=r")
	(and:SI (match_op_dup 3 [(match_dup 1) (match_dup 2)])
		(match_op_dup 6 [(match_dup 4) (match_dup 5)])))]
  "TARGET_32BIT"
  "#"
  "TARGET_32BIT && reload_completed"
  [(set (match_dup 0)
	(compare
	 (and:SI
	  (match_op_dup 3 [(match_dup 1) (match_dup 2)])
	  (match_op_dup 6 [(match_dup 4) (match_dup 5)]))
	 (const_int 0)))
   (set (match_dup 7) (ne:SI (match_dup 0) (const_int 0)))]
  ""
  [(set_attr "conds" "set")
   (set_attr "length" "16")])

;; If there is no dominance in the comparison, then we can still save an
;; instruction in the AND case, since we can know that the second compare
;; need only zero the value if false (if true, then the value is already
;; correct).
//Pattern#500
(define_insn_and_split "*and_scc_scc_nodom"
  [(set (match_operand:SI 0 "s_register_operand" "=&r,&r,&r")
	(and:SI (match_operator:SI 3 "arm_comparison_operator"
		 [(match_operand:SI 1 "s_register_operand" "r,r,0")
		  (match_operand:SI 2 "arm_add_operand" "rIL,0,rIL")])
		(match_operator:SI 6 "arm_comparison_operator"
		 [(match_operand:SI 4 "s_register_operand" "r,r,r")
		  (match_operand:SI 5 "arm_add_operand" "rIL,rIL,rIL")])))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_32BIT
   && (arm_select_dominance_cc_mode (operands[3], operands[6], DOM_CC_X_AND_Y)
       == CCmode)"
  "#"
  "TARGET_32BIT && reload_completed"
  [(parallel [(set (match_dup 0)
		   (match_op_dup 3 [(match_dup 1) (match_dup 2)]))
	      (clobber (reg:CC CC_REGNUM))])
   (set (match_dup 7) (match_op_dup 8 [(match_dup 4) (match_dup 5)]))
   (set (match_dup 0)
	(if_then_else:SI (match_op_dup 6 [(match_dup 7) (const_int 0)])
			 (match_dup 0)
			 (const_int 0)))]
  "operands[7] = gen_rtx_REG (SELECT_CC_MODE (GET_CODE (operands[6]),
					      operands[4], operands[5]),
			      CC_REGNUM);
   operands[8] = gen_rtx_COMPARE (GET_MODE (operands[7]), operands[4],
				  operands[5]);"
  [(set_attr "conds" "clob")
   (set_attr "length" "20")])

//Pattern#501
(define_split
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV (ior:SI
			  (and:SI (match_operand:SI 0 "s_register_operand" "")
				  (const_int 1))
			  (match_operator:SI 1 "arm_comparison_operator"
			   [(match_operand:SI 2 "s_register_operand" "")
			    (match_operand:SI 3 "arm_add_operand" "")]))
			 (const_int 0)))
   (clobber (match_operand:SI 4 "s_register_operand" ""))]
  "TARGET_ARM"
  [(set (match_dup 4)
	(ior:SI (match_op_dup 1 [(match_dup 2) (match_dup 3)])
		(match_dup 0)))
   (set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV (and:SI (match_dup 4) (const_int 1))
			 (const_int 0)))]
  "")

//Pattern#502
(define_split
  [(set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV (ior:SI
			  (match_operator:SI 1 "arm_comparison_operator"
			   [(match_operand:SI 2 "s_register_operand" "")
			    (match_operand:SI 3 "arm_add_operand" "")])
			  (and:SI (match_operand:SI 0 "s_register_operand" "")
				  (const_int 1)))
			 (const_int 0)))
   (clobber (match_operand:SI 4 "s_register_operand" ""))]
  "TARGET_ARM"
  [(set (match_dup 4)
	(ior:SI (match_op_dup 1 [(match_dup 2) (match_dup 3)])
		(match_dup 0)))
   (set (reg:CC_NOOV CC_REGNUM)
	(compare:CC_NOOV (and:SI (match_dup 4) (const_int 1))
			 (const_int 0)))]
  "")
;; ??? The conditional patterns above need checking for Thumb-2 usefulness

//Pattern#503
(define_insn "*negscc"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(neg:SI (match_operator 3 "arm_comparison_operator"
		 [(match_operand:SI 1 "s_register_operand" "r")
		  (match_operand:SI 2 "arm_rhs_operand" "rI")])))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "*
  if (GET_CODE (operands[3]) == LT && operands[2] == const0_rtx)
    return \"mov\\t%0, %1, asr #31\";

  if (GET_CODE (operands[3]) == NE)
    return \"subs\\t%0, %1, %2\;mvnne\\t%0, #0\";

  output_asm_insn (\"cmp\\t%1, %2\", operands);
  output_asm_insn (\"mov%D3\\t%0, #0\", operands);
  return \"mvn%d3\\t%0, #0\";
  "
  [(set_attr "conds" "clob")
   (set_attr "length" "12")]
)

//Pattern#504
(define_insn "movcond"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r,r")
	(if_then_else:SI
	 (match_operator 5 "arm_comparison_operator"
	  [(match_operand:SI 3 "s_register_operand" "r,r,r")
	   (match_operand:SI 4 "arm_add_operand" "rIL,rIL,rIL")])
	 (match_operand:SI 1 "arm_rhs_operand" "0,rI,?rI")
	 (match_operand:SI 2 "arm_rhs_operand" "rI,0,rI")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "*
  if (GET_CODE (operands[5]) == LT
      && (operands[4] == const0_rtx))
    {
      if (which_alternative != 1 && REG_P (operands[1]))
	{
	  if (operands[2] == const0_rtx)
	    return \"and\\t%0, %1, %3, asr #31\";
	  return \"ands\\t%0, %1, %3, asr #32\;movcc\\t%0, %2\";
	}
      else if (which_alternative != 0 && REG_P (operands[2]))
	{
	  if (operands[1] == const0_rtx)
	    return \"bic\\t%0, %2, %3, asr #31\";
	  return \"bics\\t%0, %2, %3, asr #32\;movcs\\t%0, %1\";
	}
      /* The only case that falls through to here is when both ops 1 & 2
	 are constants.  */
    }

  if (GET_CODE (operands[5]) == GE
      && (operands[4] == const0_rtx))
    {
      if (which_alternative != 1 && REG_P (operands[1]))
	{
	  if (operands[2] == const0_rtx)
	    return \"bic\\t%0, %1, %3, asr #31\";
	  return \"bics\\t%0, %1, %3, asr #32\;movcs\\t%0, %2\";
	}
      else if (which_alternative != 0 && REG_P (operands[2]))
	{
	  if (operands[1] == const0_rtx)
	    return \"and\\t%0, %2, %3, asr #31\";
	  return \"ands\\t%0, %2, %3, asr #32\;movcc\\t%0, %1\";
	}
      /* The only case that falls through to here is when both ops 1 & 2
	 are constants.  */
    }
  if (CONST_INT_P (operands[4])
      && !const_ok_for_arm (INTVAL (operands[4])))
    output_asm_insn (\"cmn\\t%3, #%n4\", operands);
  else
    output_asm_insn (\"cmp\\t%3, %4\", operands);
  if (which_alternative != 0)
    output_asm_insn (\"mov%d5\\t%0, %1\", operands);
  if (which_alternative != 1)
    output_asm_insn (\"mov%D5\\t%0, %2\", operands);
  return \"\";
  "
  [(set_attr "conds" "clob")
   (set_attr "length" "8,8,12")]
)

;; ??? The patterns below need checking for Thumb-2 usefulness.

//Pattern#505
(define_insn "*ifcompare_plus_move"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(if_then_else:SI (match_operator 6 "arm_comparison_operator"
			  [(match_operand:SI 4 "s_register_operand" "r,r")
			   (match_operand:SI 5 "arm_add_operand" "rIL,rIL")])
			 (plus:SI
			  (match_operand:SI 2 "s_register_operand" "r,r")
			  (match_operand:SI 3 "arm_add_operand" "rIL,rIL"))
			 (match_operand:SI 1 "arm_rhs_operand" "0,?rI")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "#"
  [(set_attr "conds" "clob")
   (set_attr "length" "8,12")]
)

//Pattern#506
(define_insn "*if_plus_move"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r,r,r")
	(if_then_else:SI
	 (match_operator 4 "arm_comparison_operator"
	  [(match_operand 5 "cc_register" "") (const_int 0)])
	 (plus:SI
	  (match_operand:SI 2 "s_register_operand" "r,r,r,r")
	  (match_operand:SI 3 "arm_add_operand" "rI,L,rI,L"))
	 (match_operand:SI 1 "arm_rhs_operand" "0,0,?rI,?rI")))]
  "TARGET_ARM"
  "@
   add%d4\\t%0, %2, %3
   sub%d4\\t%0, %2, #%n3
   add%d4\\t%0, %2, %3\;mov%D4\\t%0, %1
   sub%d4\\t%0, %2, #%n3\;mov%D4\\t%0, %1"
  [(set_attr "conds" "use")
   (set_attr "length" "4,4,8,8")
   (set_attr_alternative "type"
                         [(if_then_else (match_operand 3 "const_int_operand" "")
                                        (const_string "simple_alu_imm" )
                                        (const_string "*"))
                          (const_string "simple_alu_imm")
                          (const_string "*")
                          (const_string "*")])]
)

//Pattern#507
(define_insn "*ifcompare_move_plus"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(if_then_else:SI (match_operator 6 "arm_comparison_operator"
			  [(match_operand:SI 4 "s_register_operand" "r,r")
			   (match_operand:SI 5 "arm_add_operand" "rIL,rIL")])
			 (match_operand:SI 1 "arm_rhs_operand" "0,?rI")
			 (plus:SI
			  (match_operand:SI 2 "s_register_operand" "r,r")
			  (match_operand:SI 3 "arm_add_operand" "rIL,rIL"))))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "#"
  [(set_attr "conds" "clob")
   (set_attr "length" "8,12")]
)

//Pattern#508
(define_insn "*if_move_plus"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r,r,r")
	(if_then_else:SI
	 (match_operator 4 "arm_comparison_operator"
	  [(match_operand 5 "cc_register" "") (const_int 0)])
	 (match_operand:SI 1 "arm_rhs_operand" "0,0,?rI,?rI")
	 (plus:SI
	  (match_operand:SI 2 "s_register_operand" "r,r,r,r")
	  (match_operand:SI 3 "arm_add_operand" "rI,L,rI,L"))))]
  "TARGET_ARM"
  "@
   add%D4\\t%0, %2, %3
   sub%D4\\t%0, %2, #%n3
   add%D4\\t%0, %2, %3\;mov%d4\\t%0, %1
   sub%D4\\t%0, %2, #%n3\;mov%d4\\t%0, %1"
  [(set_attr "conds" "use")
   (set_attr "length" "4,4,8,8")
   (set_attr_alternative "type"
                         [(if_then_else (match_operand 3 "const_int_operand" "")
                                        (const_string "simple_alu_imm" )
                                        (const_string "*"))
                          (const_string "simple_alu_imm")
                          (const_string "*")
                          (const_string "*")])]
)

//Pattern#509
(define_insn "*ifcompare_arith_arith"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(if_then_else:SI (match_operator 9 "arm_comparison_operator"
			  [(match_operand:SI 5 "s_register_operand" "r")
			   (match_operand:SI 6 "arm_add_operand" "rIL")])
			 (match_operator:SI 8 "shiftable_operator"
			  [(match_operand:SI 1 "s_register_operand" "r")
			   (match_operand:SI 2 "arm_rhs_operand" "rI")])
			 (match_operator:SI 7 "shiftable_operator"
			  [(match_operand:SI 3 "s_register_operand" "r")
			   (match_operand:SI 4 "arm_rhs_operand" "rI")])))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "#"
  [(set_attr "conds" "clob")
   (set_attr "length" "12")]
)

//Pattern#510
(define_insn "*if_arith_arith"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(if_then_else:SI (match_operator 5 "arm_comparison_operator"
			  [(match_operand 8 "cc_register" "") (const_int 0)])
			 (match_operator:SI 6 "shiftable_operator"
			  [(match_operand:SI 1 "s_register_operand" "r")
			   (match_operand:SI 2 "arm_rhs_operand" "rI")])
			 (match_operator:SI 7 "shiftable_operator"
			  [(match_operand:SI 3 "s_register_operand" "r")
			   (match_operand:SI 4 "arm_rhs_operand" "rI")])))]
  "TARGET_ARM"
  "%I6%d5\\t%0, %1, %2\;%I7%D5\\t%0, %3, %4"
  [(set_attr "conds" "use")
   (set_attr "length" "8")]
)

//Pattern#511
(define_insn "*ifcompare_arith_move"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(if_then_else:SI (match_operator 6 "arm_comparison_operator"
			  [(match_operand:SI 2 "s_register_operand" "r,r")
			   (match_operand:SI 3 "arm_add_operand" "rIL,rIL")])
			 (match_operator:SI 7 "shiftable_operator"
			  [(match_operand:SI 4 "s_register_operand" "r,r")
			   (match_operand:SI 5 "arm_rhs_operand" "rI,rI")])
			 (match_operand:SI 1 "arm_rhs_operand" "0,?rI")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "*
  /* If we have an operation where (op x 0) is the identity operation and
     the conditional operator is LT or GE and we are comparing against zero and
     everything is in registers then we can do this in two instructions.  */
  if (operands[3] == const0_rtx
      && GET_CODE (operands[7]) != AND
      && REG_P (operands[5])
      && REG_P (operands[1])
      && REGNO (operands[1]) == REGNO (operands[4])
      && REGNO (operands[4]) != REGNO (operands[0]))
    {
      if (GET_CODE (operands[6]) == LT)
	return \"and\\t%0, %5, %2, asr #31\;%I7\\t%0, %4, %0\";
      else if (GET_CODE (operands[6]) == GE)
	return \"bic\\t%0, %5, %2, asr #31\;%I7\\t%0, %4, %0\";
    }
  if (CONST_INT_P (operands[3])
      && !const_ok_for_arm (INTVAL (operands[3])))
    output_asm_insn (\"cmn\\t%2, #%n3\", operands);
  else
    output_asm_insn (\"cmp\\t%2, %3\", operands);
  output_asm_insn (\"%I7%d6\\t%0, %4, %5\", operands);
  if (which_alternative != 0)
    return \"mov%D6\\t%0, %1\";
  return \"\";
  "
  [(set_attr "conds" "clob")
   (set_attr "length" "8,12")]
)

//Pattern#512
(define_insn "*if_arith_move"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(if_then_else:SI (match_operator 4 "arm_comparison_operator"
			  [(match_operand 6 "cc_register" "") (const_int 0)])
			 (match_operator:SI 5 "shiftable_operator"
			  [(match_operand:SI 2 "s_register_operand" "r,r")
			   (match_operand:SI 3 "arm_rhs_operand" "rI,rI")])
			 (match_operand:SI 1 "arm_rhs_operand" "0,?rI")))]
  "TARGET_ARM"
  "@
   %I5%d4\\t%0, %2, %3
   %I5%d4\\t%0, %2, %3\;mov%D4\\t%0, %1"
  [(set_attr "conds" "use")
   (set_attr "length" "4,8")
   (set_attr "type" "*,*")]
)

//Pattern#513
(define_insn "*ifcompare_move_arith"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(if_then_else:SI (match_operator 6 "arm_comparison_operator"
			  [(match_operand:SI 4 "s_register_operand" "r,r")
			   (match_operand:SI 5 "arm_add_operand" "rIL,rIL")])
			 (match_operand:SI 1 "arm_rhs_operand" "0,?rI")
			 (match_operator:SI 7 "shiftable_operator"
			  [(match_operand:SI 2 "s_register_operand" "r,r")
			   (match_operand:SI 3 "arm_rhs_operand" "rI,rI")])))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "*
  /* If we have an operation where (op x 0) is the identity operation and
     the conditional operator is LT or GE and we are comparing against zero and
     everything is in registers then we can do this in two instructions */
  if (operands[5] == const0_rtx
      && GET_CODE (operands[7]) != AND
      && REG_P (operands[3])
      && REG_P (operands[1])
      && REGNO (operands[1]) == REGNO (operands[2])
      && REGNO (operands[2]) != REGNO (operands[0]))
    {
      if (GET_CODE (operands[6]) == GE)
	return \"and\\t%0, %3, %4, asr #31\;%I7\\t%0, %2, %0\";
      else if (GET_CODE (operands[6]) == LT)
	return \"bic\\t%0, %3, %4, asr #31\;%I7\\t%0, %2, %0\";
    }

  if (CONST_INT_P (operands[5])
      && !const_ok_for_arm (INTVAL (operands[5])))
    output_asm_insn (\"cmn\\t%4, #%n5\", operands);
  else
    output_asm_insn (\"cmp\\t%4, %5\", operands);

  if (which_alternative != 0)
    output_asm_insn (\"mov%d6\\t%0, %1\", operands);
  return \"%I7%D6\\t%0, %2, %3\";
  "
  [(set_attr "conds" "clob")
   (set_attr "length" "8,12")]
)

//Pattern#514
(define_insn "*if_move_arith"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(if_then_else:SI
	 (match_operator 4 "arm_comparison_operator"
	  [(match_operand 6 "cc_register" "") (const_int 0)])
	 (match_operand:SI 1 "arm_rhs_operand" "0,?rI")
	 (match_operator:SI 5 "shiftable_operator"
	  [(match_operand:SI 2 "s_register_operand" "r,r")
	   (match_operand:SI 3 "arm_rhs_operand" "rI,rI")])))]
  "TARGET_ARM"
  "@
   %I5%D4\\t%0, %2, %3
   %I5%D4\\t%0, %2, %3\;mov%d4\\t%0, %1"
  [(set_attr "conds" "use")
   (set_attr "length" "4,8")
   (set_attr "type" "*,*")]
)

//Pattern#515
(define_insn "*ifcompare_move_not"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(if_then_else:SI
	 (match_operator 5 "arm_comparison_operator"
	  [(match_operand:SI 3 "s_register_operand" "r,r")
	   (match_operand:SI 4 "arm_add_operand" "rIL,rIL")])
	 (match_operand:SI 1 "arm_not_operand" "0,?rIK")
	 (not:SI
	  (match_operand:SI 2 "s_register_operand" "r,r"))))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "#"
  [(set_attr "conds" "clob")
   (set_attr "length" "8,12")]
)

//Pattern#516
(define_insn "*if_move_not"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r,r")
	(if_then_else:SI
	 (match_operator 4 "arm_comparison_operator"
	  [(match_operand 3 "cc_register" "") (const_int 0)])
	 (match_operand:SI 1 "arm_not_operand" "0,?rI,K")
	 (not:SI (match_operand:SI 2 "s_register_operand" "r,r,r"))))]
  "TARGET_ARM"
  "@
   mvn%D4\\t%0, %2
   mov%d4\\t%0, %1\;mvn%D4\\t%0, %2
   mvn%d4\\t%0, #%B1\;mvn%D4\\t%0, %2"
  [(set_attr "conds" "use")
   (set_attr "insn" "mvn")
   (set_attr "length" "4,8,8")]
)

//Pattern#517
(define_insn "*ifcompare_not_move"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(if_then_else:SI 
	 (match_operator 5 "arm_comparison_operator"
	  [(match_operand:SI 3 "s_register_operand" "r,r")
	   (match_operand:SI 4 "arm_add_operand" "rIL,rIL")])
	 (not:SI
	  (match_operand:SI 2 "s_register_operand" "r,r"))
	 (match_operand:SI 1 "arm_not_operand" "0,?rIK")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "#"
  [(set_attr "conds" "clob")
   (set_attr "length" "8,12")]
)

//Pattern#518
(define_insn "*if_not_move"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r,r")
	(if_then_else:SI
	 (match_operator 4 "arm_comparison_operator"
	  [(match_operand 3 "cc_register" "") (const_int 0)])
	 (not:SI (match_operand:SI 2 "s_register_operand" "r,r,r"))
	 (match_operand:SI 1 "arm_not_operand" "0,?rI,K")))]
  "TARGET_ARM"
  "@
   mvn%d4\\t%0, %2
   mov%D4\\t%0, %1\;mvn%d4\\t%0, %2
   mvn%D4\\t%0, #%B1\;mvn%d4\\t%0, %2"
  [(set_attr "conds" "use")
   (set_attr "insn" "mvn")
   (set_attr "length" "4,8,8")]
)

//Pattern#519
(define_insn "*ifcompare_shift_move"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(if_then_else:SI
	 (match_operator 6 "arm_comparison_operator"
	  [(match_operand:SI 4 "s_register_operand" "r,r")
	   (match_operand:SI 5 "arm_add_operand" "rIL,rIL")])
	 (match_operator:SI 7 "shift_operator"
	  [(match_operand:SI 2 "s_register_operand" "r,r")
	   (match_operand:SI 3 "arm_rhs_operand" "rM,rM")])
	 (match_operand:SI 1 "arm_not_operand" "0,?rIK")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "#"
  [(set_attr "conds" "clob")
   (set_attr "length" "8,12")]
)

//Pattern#520
(define_insn "*if_shift_move"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r,r")
	(if_then_else:SI
	 (match_operator 5 "arm_comparison_operator"
	  [(match_operand 6 "cc_register" "") (const_int 0)])
	 (match_operator:SI 4 "shift_operator"
	  [(match_operand:SI 2 "s_register_operand" "r,r,r")
	   (match_operand:SI 3 "arm_rhs_operand" "rM,rM,rM")])
	 (match_operand:SI 1 "arm_not_operand" "0,?rI,K")))]
  "TARGET_ARM"
  "@
   mov%d5\\t%0, %2%S4
   mov%D5\\t%0, %1\;mov%d5\\t%0, %2%S4
   mvn%D5\\t%0, #%B1\;mov%d5\\t%0, %2%S4"
  [(set_attr "conds" "use")
   (set_attr "shift" "2")
   (set_attr "length" "4,8,8")
   (set_attr "insn" "mov")
   (set (attr "type") (if_then_else (match_operand 3 "const_int_operand" "")
		      (const_string "alu_shift")
		      (const_string "alu_shift_reg")))]
)

//Pattern#521
(define_insn "*ifcompare_move_shift"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(if_then_else:SI
	 (match_operator 6 "arm_comparison_operator"
	  [(match_operand:SI 4 "s_register_operand" "r,r")
	   (match_operand:SI 5 "arm_add_operand" "rIL,rIL")])
	 (match_operand:SI 1 "arm_not_operand" "0,?rIK")
	 (match_operator:SI 7 "shift_operator"
	  [(match_operand:SI 2 "s_register_operand" "r,r")
	   (match_operand:SI 3 "arm_rhs_operand" "rM,rM")])))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "#"
  [(set_attr "conds" "clob")
   (set_attr "length" "8,12")]
)

//Pattern#522
(define_insn "*if_move_shift"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r,r")
	(if_then_else:SI
	 (match_operator 5 "arm_comparison_operator"
	  [(match_operand 6 "cc_register" "") (const_int 0)])
	 (match_operand:SI 1 "arm_not_operand" "0,?rI,K")
	 (match_operator:SI 4 "shift_operator"
	  [(match_operand:SI 2 "s_register_operand" "r,r,r")
	   (match_operand:SI 3 "arm_rhs_operand" "rM,rM,rM")])))]
  "TARGET_ARM"
  "@
   mov%D5\\t%0, %2%S4
   mov%d5\\t%0, %1\;mov%D5\\t%0, %2%S4
   mvn%d5\\t%0, #%B1\;mov%D5\\t%0, %2%S4"
  [(set_attr "conds" "use")
   (set_attr "shift" "2")
   (set_attr "length" "4,8,8")
   (set_attr "insn" "mov")
   (set (attr "type") (if_then_else (match_operand 3 "const_int_operand" "")
		      (const_string "alu_shift")
		      (const_string "alu_shift_reg")))]
)

//Pattern#523
(define_insn "*ifcompare_shift_shift"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(if_then_else:SI
	 (match_operator 7 "arm_comparison_operator"
	  [(match_operand:SI 5 "s_register_operand" "r")
	   (match_operand:SI 6 "arm_add_operand" "rIL")])
	 (match_operator:SI 8 "shift_operator"
	  [(match_operand:SI 1 "s_register_operand" "r")
	   (match_operand:SI 2 "arm_rhs_operand" "rM")])
	 (match_operator:SI 9 "shift_operator"
	  [(match_operand:SI 3 "s_register_operand" "r")
	   (match_operand:SI 4 "arm_rhs_operand" "rM")])))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "#"
  [(set_attr "conds" "clob")
   (set_attr "length" "12")]
)

//Pattern#524
(define_insn "*if_shift_shift"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(if_then_else:SI
	 (match_operator 5 "arm_comparison_operator"
	  [(match_operand 8 "cc_register" "") (const_int 0)])
	 (match_operator:SI 6 "shift_operator"
	  [(match_operand:SI 1 "s_register_operand" "r")
	   (match_operand:SI 2 "arm_rhs_operand" "rM")])
	 (match_operator:SI 7 "shift_operator"
	  [(match_operand:SI 3 "s_register_operand" "r")
	   (match_operand:SI 4 "arm_rhs_operand" "rM")])))]
  "TARGET_ARM"
  "mov%d5\\t%0, %1%S6\;mov%D5\\t%0, %3%S7"
  [(set_attr "conds" "use")
   (set_attr "shift" "1")
   (set_attr "length" "8")
   (set_attr "insn" "mov")
   (set (attr "type") (if_then_else
		        (and (match_operand 2 "const_int_operand" "")
                             (match_operand 4 "const_int_operand" ""))
		      (const_string "alu_shift")
		      (const_string "alu_shift_reg")))]
)

//Pattern#525
(define_insn "*ifcompare_not_arith"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(if_then_else:SI
	 (match_operator 6 "arm_comparison_operator"
	  [(match_operand:SI 4 "s_register_operand" "r")
	   (match_operand:SI 5 "arm_add_operand" "rIL")])
	 (not:SI (match_operand:SI 1 "s_register_operand" "r"))
	 (match_operator:SI 7 "shiftable_operator"
	  [(match_operand:SI 2 "s_register_operand" "r")
	   (match_operand:SI 3 "arm_rhs_operand" "rI")])))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "#"
  [(set_attr "conds" "clob")
   (set_attr "length" "12")]
)

//Pattern#526
(define_insn "*if_not_arith"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(if_then_else:SI
	 (match_operator 5 "arm_comparison_operator"
	  [(match_operand 4 "cc_register" "") (const_int 0)])
	 (not:SI (match_operand:SI 1 "s_register_operand" "r"))
	 (match_operator:SI 6 "shiftable_operator"
	  [(match_operand:SI 2 "s_register_operand" "r")
	   (match_operand:SI 3 "arm_rhs_operand" "rI")])))]
  "TARGET_ARM"
  "mvn%d5\\t%0, %1\;%I6%D5\\t%0, %2, %3"
  [(set_attr "conds" "use")
   (set_attr "insn" "mvn")
   (set_attr "length" "8")]
)

//Pattern#527
(define_insn "*ifcompare_arith_not"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(if_then_else:SI
	 (match_operator 6 "arm_comparison_operator"
	  [(match_operand:SI 4 "s_register_operand" "r")
	   (match_operand:SI 5 "arm_add_operand" "rIL")])
	 (match_operator:SI 7 "shiftable_operator"
	  [(match_operand:SI 2 "s_register_operand" "r")
	   (match_operand:SI 3 "arm_rhs_operand" "rI")])
	 (not:SI (match_operand:SI 1 "s_register_operand" "r"))))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "#"
  [(set_attr "conds" "clob")
   (set_attr "length" "12")]
)

//Pattern#528
(define_insn "*if_arith_not"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(if_then_else:SI
	 (match_operator 5 "arm_comparison_operator"
	  [(match_operand 4 "cc_register" "") (const_int 0)])
	 (match_operator:SI 6 "shiftable_operator"
	  [(match_operand:SI 2 "s_register_operand" "r")
	   (match_operand:SI 3 "arm_rhs_operand" "rI")])
	 (not:SI (match_operand:SI 1 "s_register_operand" "r"))))]
  "TARGET_ARM"
  "mvn%D5\\t%0, %1\;%I6%d5\\t%0, %2, %3"
  [(set_attr "conds" "use")
   (set_attr "insn" "mvn")
   (set_attr "length" "8")]
)

//Pattern#529
(define_insn "*ifcompare_neg_move"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(if_then_else:SI
	 (match_operator 5 "arm_comparison_operator"
	  [(match_operand:SI 3 "s_register_operand" "r,r")
	   (match_operand:SI 4 "arm_add_operand" "rIL,rIL")])
	 (neg:SI (match_operand:SI 2 "s_register_operand" "r,r"))
	 (match_operand:SI 1 "arm_not_operand" "0,?rIK")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "#"
  [(set_attr "conds" "clob")
   (set_attr "length" "8,12")]
)

//Pattern#530
(define_insn "*if_neg_move"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r,r")
	(if_then_else:SI
	 (match_operator 4 "arm_comparison_operator"
	  [(match_operand 3 "cc_register" "") (const_int 0)])
	 (neg:SI (match_operand:SI 2 "s_register_operand" "r,r,r"))
	 (match_operand:SI 1 "arm_not_operand" "0,?rI,K")))]
  "TARGET_ARM"
  "@
   rsb%d4\\t%0, %2, #0
   mov%D4\\t%0, %1\;rsb%d4\\t%0, %2, #0
   mvn%D4\\t%0, #%B1\;rsb%d4\\t%0, %2, #0"
  [(set_attr "conds" "use")
   (set_attr "length" "4,8,8")]
)

//Pattern#531
(define_insn "*ifcompare_move_neg"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(if_then_else:SI
	 (match_operator 5 "arm_comparison_operator"
	  [(match_operand:SI 3 "s_register_operand" "r,r")
	   (match_operand:SI 4 "arm_add_operand" "rIL,rIL")])
	 (match_operand:SI 1 "arm_not_operand" "0,?rIK")
	 (neg:SI (match_operand:SI 2 "s_register_operand" "r,r"))))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "#"
  [(set_attr "conds" "clob")
   (set_attr "length" "8,12")]
)

//Pattern#532
(define_insn "*if_move_neg"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r,r")
	(if_then_else:SI
	 (match_operator 4 "arm_comparison_operator"
	  [(match_operand 3 "cc_register" "") (const_int 0)])
	 (match_operand:SI 1 "arm_not_operand" "0,?rI,K")
	 (neg:SI (match_operand:SI 2 "s_register_operand" "r,r,r"))))]
  "TARGET_ARM"
  "@
   rsb%D4\\t%0, %2, #0
   mov%d4\\t%0, %1\;rsb%D4\\t%0, %2, #0
   mvn%d4\\t%0, #%B1\;rsb%D4\\t%0, %2, #0"
  [(set_attr "conds" "use")
   (set_attr "length" "4,8,8")]
)

//Pattern#533
(define_insn "*arith_adjacentmem"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(match_operator:SI 1 "shiftable_operator"
	 [(match_operand:SI 2 "memory_operand" "m")
	  (match_operand:SI 3 "memory_operand" "m")]))
   (clobber (match_scratch:SI 4 "=r"))]
  "TARGET_ARM && adjacent_mem_locations (operands[2], operands[3])"
  "*
  {
    rtx ldm[3];
    rtx arith[4];
    rtx base_reg;
    HOST_WIDE_INT val1 = 0, val2 = 0;

    if (REGNO (operands[0]) > REGNO (operands[4]))
      {
	ldm[1] = operands[4];
	ldm[2] = operands[0];
      }
    else
      {
	ldm[1] = operands[0];
	ldm[2] = operands[4];
      }

    base_reg = XEXP (operands[2], 0);

    if (!REG_P (base_reg))
      {
	val1 = INTVAL (XEXP (base_reg, 1));
	base_reg = XEXP (base_reg, 0);
      }

    if (!REG_P (XEXP (operands[3], 0)))
      val2 = INTVAL (XEXP (XEXP (operands[3], 0), 1));

    arith[0] = operands[0];
    arith[3] = operands[1];

    if (val1 < val2)
      {
	arith[1] = ldm[1];
	arith[2] = ldm[2];
      }
    else
      {
	arith[1] = ldm[2];
	arith[2] = ldm[1];
      }

    ldm[0] = base_reg;
    if (val1 !=0 && val2 != 0)
      {
	rtx ops[3];

	if (val1 == 4 || val2 == 4)
	  /* Other val must be 8, since we know they are adjacent and neither
	     is zero.  */
	  output_asm_insn (\"ldm%(ib%)\\t%0, {%1, %2}\", ldm);
	else if (const_ok_for_arm (val1) || const_ok_for_arm (-val1))
	  {
	    ldm[0] = ops[0] = operands[4];
	    ops[1] = base_reg;
	    ops[2] = GEN_INT (val1);
	    output_add_immediate (ops);
	    if (val1 < val2)
	      output_asm_insn (\"ldm%(ia%)\\t%0, {%1, %2}\", ldm);
	    else
	      output_asm_insn (\"ldm%(da%)\\t%0, {%1, %2}\", ldm);
	  }
	else
	  {
	    /* Offset is out of range for a single add, so use two ldr.  */
	    ops[0] = ldm[1];
	    ops[1] = base_reg;
	    ops[2] = GEN_INT (val1);
	    output_asm_insn (\"ldr%?\\t%0, [%1, %2]\", ops);
	    ops[0] = ldm[2];
	    ops[2] = GEN_INT (val2);
	    output_asm_insn (\"ldr%?\\t%0, [%1, %2]\", ops);
	  }
      }
    else if (val1 != 0)
      {
	if (val1 < val2)
	  output_asm_insn (\"ldm%(da%)\\t%0, {%1, %2}\", ldm);
	else
	  output_asm_insn (\"ldm%(ia%)\\t%0, {%1, %2}\", ldm);
      }
    else
      {
	if (val1 < val2)
	  output_asm_insn (\"ldm%(ia%)\\t%0, {%1, %2}\", ldm);
	else
	  output_asm_insn (\"ldm%(da%)\\t%0, {%1, %2}\", ldm);
      }
    output_asm_insn (\"%I3%?\\t%0, %1, %2\", arith);
    return \"\";
  }"
  [(set_attr "length" "12")
   (set_attr "predicable" "yes")
   (set_attr "type" "load1")]
)

; This pattern is never tried by combine, so do it as a peephole

//Pattern#534
(define_peephole2
  [(set (match_operand:SI 0 "arm_general_register_operand" "")
	(match_operand:SI 1 "arm_general_register_operand" ""))
   (set (reg:CC CC_REGNUM)
	(compare:CC (match_dup 1) (const_int 0)))]
  "TARGET_ARM"
  [(parallel [(set (reg:CC CC_REGNUM) (compare:CC (match_dup 1) (const_int 0)))
	      (set (match_dup 0) (match_dup 1))])]
  ""
)

//Pattern#535
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(and:SI (ge:SI (match_operand:SI 1 "s_register_operand" "")
		       (const_int 0))
		(neg:SI (match_operator:SI 2 "arm_comparison_operator"
			 [(match_operand:SI 3 "s_register_operand" "")
			  (match_operand:SI 4 "arm_rhs_operand" "")]))))
   (clobber (match_operand:SI 5 "s_register_operand" ""))]
  "TARGET_ARM"
  [(set (match_dup 5) (not:SI (ashiftrt:SI (match_dup 1) (const_int 31))))
   (set (match_dup 0) (and:SI (match_op_dup 2 [(match_dup 3) (match_dup 4)])
			      (match_dup 5)))]
  ""
)

;; This split can be used because CC_Z mode implies that the following
;; branch will be an equality, or an unsigned inequality, so the sign
;; extension is not needed.

//Pattern#536
(define_split
  [(set (reg:CC_Z CC_REGNUM)
	(compare:CC_Z
	 (ashift:SI (subreg:SI (match_operand:QI 0 "memory_operand" "") 0)
		    (const_int 24))
	 (match_operand 1 "const_int_operand" "")))
   (clobber (match_scratch:SI 2 ""))]
  "TARGET_ARM
   && (((unsigned HOST_WIDE_INT) INTVAL (operands[1]))
       == (((unsigned HOST_WIDE_INT) INTVAL (operands[1])) >> 24) << 24)"
  [(set (match_dup 2) (zero_extend:SI (match_dup 0)))
   (set (reg:CC CC_REGNUM) (compare:CC (match_dup 2) (match_dup 1)))]
  "
  operands[1] = GEN_INT (((unsigned long) INTVAL (operands[1])) >> 24);
  "
)
;; ??? Check the patterns above for Thumb-2 usefulness

//Pattern#537
(define_expand "prologue"
  [(clobber (const_int 0))]
  "TARGET_EITHER"
  "if (TARGET_32BIT)
     arm_expand_prologue ();
   else
     thumb1_expand_prologue ();
  DONE;
  "
)

//Pattern#538
(define_expand "epilogue"
  [(clobber (const_int 0))]
  "TARGET_EITHER"
  "
  if (crtl->calls_eh_return)
    emit_insn (gen_force_register_use (gen_rtx_REG (Pmode, 2)));
  if (TARGET_THUMB1)
   {
     thumb1_expand_epilogue ();
     emit_jump_insn (gen_rtx_UNSPEC_VOLATILE (VOIDmode,
                     gen_rtvec (1, ret_rtx), VUNSPEC_EPILOGUE));
   }
  else if (HAVE_return)
   {
     /* HAVE_return is testing for USE_RETURN_INSN (FALSE).  Hence,
        no need for explicit testing again.  */
     emit_jump_insn (gen_return ());
   }
  else if (TARGET_32BIT)
   {
    arm_expand_epilogue (true);
   }
  DONE;
  "
)

//Pattern#539
(define_insn "prologue_thumb1_interwork"
  [(unspec_volatile [(const_int 0)] VUNSPEC_THUMB1_INTERWORK)]
  "TARGET_THUMB1"
  "* return thumb1_output_interwork ();"
  [(set_attr "length" "8")]
)

;; Note - although unspec_volatile's USE all hard registers,
;; USEs are ignored after relaod has completed.  Thus we need
;; to add an unspec of the link register to ensure that flow
;; does not think that it is unused by the sibcall branch that
;; will replace the standard function epilogue.
//Pattern#540
(define_expand "sibcall_epilogue"
   [(parallel [(unspec:SI [(reg:SI LR_REGNUM)] UNSPEC_REGISTER_USE)
               (unspec_volatile [(return)] VUNSPEC_EPILOGUE)])]
   "TARGET_32BIT"
   "
   arm_expand_epilogue (false);
   DONE;
   "
)

//Pattern#541
(define_insn "*epilogue_insns"
  [(unspec_volatile [(return)] VUNSPEC_EPILOGUE)]
  "TARGET_THUMB1"
  "*
    return thumb1_unexpanded_epilogue ();
  "
  ; Length is absolute worst case
  [(set_attr "length" "44")
   (set_attr "type" "block")
   ;; We don't clobber the conditions, but the potential length of this
   ;; operation is sufficient to make conditionalizing the sequence 
   ;; unlikely to be profitable.
   (set_attr "conds" "clob")]
)

//Pattern#542
(define_expand "eh_epilogue"
  [(use (match_operand:SI 0 "register_operand" ""))
   (use (match_operand:SI 1 "register_operand" ""))
   (use (match_operand:SI 2 "register_operand" ""))]
  "TARGET_EITHER"
  "
  {
    cfun->machine->eh_epilogue_sp_ofs = operands[1];
    if (!REG_P (operands[2]) || REGNO (operands[2]) != 2)
      {
	rtx ra = gen_rtx_REG (Pmode, 2);

	emit_move_insn (ra, operands[2]);
	operands[2] = ra;
      }
    /* This is a hack -- we may have crystalized the function type too
       early.  */
    cfun->machine->func_type = 0;
  }"
)

;; This split is only used during output to reduce the number of patterns
;; that need assembler instructions adding to them.  We allowed the setting
;; of the conditions to be implicit during rtl generation so that
;; the conditional compare patterns would work.  However this conflicts to
;; some extent with the conditional data operations, so we have to split them
;; up again here.

;; ??? Need to audit these splitters for Thumb-2.  Why isn't normal
;; conditional execution sufficient?

//Pattern#543
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(if_then_else:SI (match_operator 1 "arm_comparison_operator"
			  [(match_operand 2 "" "") (match_operand 3 "" "")])
			 (match_dup 0)
			 (match_operand 4 "" "")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM && reload_completed"
  [(set (match_dup 5) (match_dup 6))
   (cond_exec (match_dup 7)
	      (set (match_dup 0) (match_dup 4)))]
  "
  {
    enum machine_mode mode = SELECT_CC_MODE (GET_CODE (operands[1]),
					     operands[2], operands[3]);
    enum rtx_code rc = GET_CODE (operands[1]);

    operands[5] = gen_rtx_REG (mode, CC_REGNUM);
    operands[6] = gen_rtx_COMPARE (mode, operands[2], operands[3]);
    if (mode == CCFPmode || mode == CCFPEmode)
      rc = reverse_condition_maybe_unordered (rc);
    else
      rc = reverse_condition (rc);

    operands[7] = gen_rtx_fmt_ee (rc, VOIDmode, operands[5], const0_rtx);
  }"
)

//Pattern#544
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(if_then_else:SI (match_operator 1 "arm_comparison_operator"
			  [(match_operand 2 "" "") (match_operand 3 "" "")])
			 (match_operand 4 "" "")
			 (match_dup 0)))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM && reload_completed"
  [(set (match_dup 5) (match_dup 6))
   (cond_exec (match_op_dup 1 [(match_dup 5) (const_int 0)])
	      (set (match_dup 0) (match_dup 4)))]
  "
  {
    enum machine_mode mode = SELECT_CC_MODE (GET_CODE (operands[1]),
					     operands[2], operands[3]);

    operands[5] = gen_rtx_REG (mode, CC_REGNUM);
    operands[6] = gen_rtx_COMPARE (mode, operands[2], operands[3]);
  }"
)

//Pattern#545
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(if_then_else:SI (match_operator 1 "arm_comparison_operator"
			  [(match_operand 2 "" "") (match_operand 3 "" "")])
			 (match_operand 4 "" "")
			 (match_operand 5 "" "")))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM && reload_completed"
  [(set (match_dup 6) (match_dup 7))
   (cond_exec (match_op_dup 1 [(match_dup 6) (const_int 0)])
	      (set (match_dup 0) (match_dup 4)))
   (cond_exec (match_dup 8)
	      (set (match_dup 0) (match_dup 5)))]
  "
  {
    enum machine_mode mode = SELECT_CC_MODE (GET_CODE (operands[1]),
					     operands[2], operands[3]);
    enum rtx_code rc = GET_CODE (operands[1]);

    operands[6] = gen_rtx_REG (mode, CC_REGNUM);
    operands[7] = gen_rtx_COMPARE (mode, operands[2], operands[3]);
    if (mode == CCFPmode || mode == CCFPEmode)
      rc = reverse_condition_maybe_unordered (rc);
    else
      rc = reverse_condition (rc);

    operands[8] = gen_rtx_fmt_ee (rc, VOIDmode, operands[6], const0_rtx);
  }"
)

//Pattern#546
(define_split
  [(set (match_operand:SI 0 "s_register_operand" "")
	(if_then_else:SI (match_operator 1 "arm_comparison_operator"
			  [(match_operand:SI 2 "s_register_operand" "")
			   (match_operand:SI 3 "arm_add_operand" "")])
			 (match_operand:SI 4 "arm_rhs_operand" "")
			 (not:SI
			  (match_operand:SI 5 "s_register_operand" ""))))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM && reload_completed"
  [(set (match_dup 6) (match_dup 7))
   (cond_exec (match_op_dup 1 [(match_dup 6) (const_int 0)])
	      (set (match_dup 0) (match_dup 4)))
   (cond_exec (match_dup 8)
	      (set (match_dup 0) (not:SI (match_dup 5))))]
  "
  {
    enum machine_mode mode = SELECT_CC_MODE (GET_CODE (operands[1]),
					     operands[2], operands[3]);
    enum rtx_code rc = GET_CODE (operands[1]);

    operands[6] = gen_rtx_REG (mode, CC_REGNUM);
    operands[7] = gen_rtx_COMPARE (mode, operands[2], operands[3]);
    if (mode == CCFPmode || mode == CCFPEmode)
      rc = reverse_condition_maybe_unordered (rc);
    else
      rc = reverse_condition (rc);

    operands[8] = gen_rtx_fmt_ee (rc, VOIDmode, operands[6], const0_rtx);
  }"
)

//Pattern#547
(define_insn "*cond_move_not"
  [(set (match_operand:SI 0 "s_register_operand" "=r,r")
	(if_then_else:SI (match_operator 4 "arm_comparison_operator"
			  [(match_operand 3 "cc_register" "") (const_int 0)])
			 (match_operand:SI 1 "arm_rhs_operand" "0,?rI")
			 (not:SI
			  (match_operand:SI 2 "s_register_operand" "r,r"))))]
  "TARGET_ARM"
  "@
   mvn%D4\\t%0, %2
   mov%d4\\t%0, %1\;mvn%D4\\t%0, %2"
  [(set_attr "conds" "use")
   (set_attr "insn" "mvn")
   (set_attr "length" "4,8")]
)

;; The next two patterns occur when an AND operation is followed by a
;; scc insn sequence 

//Pattern#548
(define_insn "*sign_extract_onebit"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(sign_extract:SI (match_operand:SI 1 "s_register_operand" "r")
			 (const_int 1)
			 (match_operand:SI 2 "const_int_operand" "n")))
    (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "*
    operands[2] = GEN_INT (1 << INTVAL (operands[2]));
    output_asm_insn (\"ands\\t%0, %1, %2\", operands);
    return \"mvnne\\t%0, #0\";
  "
  [(set_attr "conds" "clob")
   (set_attr "length" "8")]
)

//Pattern#549
(define_insn "*not_signextract_onebit"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(not:SI
	 (sign_extract:SI (match_operand:SI 1 "s_register_operand" "r")
			  (const_int 1)
			  (match_operand:SI 2 "const_int_operand" "n"))))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_ARM"
  "*
    operands[2] = GEN_INT (1 << INTVAL (operands[2]));
    output_asm_insn (\"tst\\t%1, %2\", operands);
    output_asm_insn (\"mvneq\\t%0, #0\", operands);
    return \"movne\\t%0, #0\";
  "
  [(set_attr "conds" "clob")
   (set_attr "length" "12")]
)
;; ??? The above patterns need auditing for Thumb-2

;; Push multiple registers to the stack.  Registers are in parallel (use ...)
;; expressions.  For simplicity, the first register is also in the unspec
;; part.
;; To avoid the usage of GNU extension, the length attribute is computed
;; in a C function arm_attr_length_push_multi.
//Pattern#550
(define_insn "*push_multi"
  [(match_parallel 2 "multi_register_push"
    [(set (match_operand:BLK 0 "push_mult_memory_operand" "")
	  (unspec:BLK [(match_operand:SI 1 "s_register_operand" "")]
		      UNSPEC_PUSH_MULT))])]
  ""
  "*
  {
    int num_saves = XVECLEN (operands[2], 0);
     
    /* For the StrongARM at least it is faster to
       use STR to store only a single register.
       In Thumb mode always use push, and the assembler will pick
       something appropriate.  */
    if (num_saves == 1 && TARGET_ARM)
      output_asm_insn (\"str%?\\t%1, [%m0, #-4]!\", operands);
    else
      {
	int i;
	char pattern[100];

	if (TARGET_ARM)
	    strcpy (pattern, \"stm%(fd%)\\t%m0!, {%1\");
	else if (TARGET_THUMB2)
	    strcpy (pattern, \"push%?\\t{%1\");
	else
	    strcpy (pattern, \"push\\t{%1\");

	for (i = 1; i < num_saves; i++)
	  {
	    strcat (pattern, \", %|\");
	    strcat (pattern,
		    reg_names[REGNO (XEXP (XVECEXP (operands[2], 0, i), 0))]);
	  }

	strcat (pattern, \"}\");
	output_asm_insn (pattern, operands);
      }

    return \"\";
  }"
  [(set_attr "type" "store4")
   (set (attr "length")
	(symbol_ref "arm_attr_length_push_multi (operands[2], operands[1])"))]
)

//Pattern#551
(define_insn "stack_tie"
  [(set (mem:BLK (scratch))
	(unspec:BLK [(match_operand:SI 0 "s_register_operand" "rk")
		     (match_operand:SI 1 "s_register_operand" "rk")]
		    UNSPEC_PRLG_STK))]
  ""
  ""
  [(set_attr "length" "0")]
)

;; Pop (as used in epilogue RTL)
;;
//Pattern#552
(define_insn "*load_multiple_with_writeback"
  [(match_parallel 0 "load_multiple_operation"
    [(set (match_operand:SI 1 "s_register_operand" "+rk")
          (plus:SI (match_dup 1)
                   (match_operand:SI 2 "const_int_operand" "I")))
     (set (match_operand:SI 3 "s_register_operand" "=rk")
          (mem:SI (match_dup 1)))
        ])]
  "TARGET_32BIT && (reload_in_progress || reload_completed)"
  "*
  {
    arm_output_multireg_pop (operands, /*return_pc=*/false,
                                       /*cond=*/const_true_rtx,
                                       /*reverse=*/false,
                                       /*update=*/true);
    return \"\";
  }
  "
  [(set_attr "type" "load4")
   (set_attr "predicable" "yes")]
)

;; Pop with return (as used in epilogue RTL)
;;
;; This instruction is generated when the registers are popped at the end of
;; epilogue.  Here, instead of popping the value into LR and then generating
;; jump to LR, value is popped into PC directly.  Hence, the pattern is combined
;;  with (return).
//Pattern#553
(define_insn "*pop_multiple_with_writeback_and_return"
  [(match_parallel 0 "pop_multiple_return"
    [(return)
     (set (match_operand:SI 1 "s_register_operand" "+rk")
          (plus:SI (match_dup 1)
                   (match_operand:SI 2 "const_int_operand" "I")))
     (set (match_operand:SI 3 "s_register_operand" "=rk")
          (mem:SI (match_dup 1)))
        ])]
  "TARGET_32BIT && (reload_in_progress || reload_completed)"
  "*
  {
    arm_output_multireg_pop (operands, /*return_pc=*/true,
                                       /*cond=*/const_true_rtx,
                                       /*reverse=*/false,
                                       /*update=*/true);
    return \"\";
  }
  "
  [(set_attr "type" "load4")
   (set_attr "predicable" "yes")]
)

//Pattern#554
(define_insn "*pop_multiple_with_return"
  [(match_parallel 0 "pop_multiple_return"
    [(return)
     (set (match_operand:SI 2 "s_register_operand" "=rk")
          (mem:SI (match_operand:SI 1 "s_register_operand" "rk")))
        ])]
  "TARGET_32BIT && (reload_in_progress || reload_completed)"
  "*
  {
    arm_output_multireg_pop (operands, /*return_pc=*/true,
                                       /*cond=*/const_true_rtx,
                                       /*reverse=*/false,
                                       /*update=*/false);
    return \"\";
  }
  "
  [(set_attr "type" "load4")
   (set_attr "predicable" "yes")]
)

;; Load into PC and return
//Pattern#555
(define_insn "*ldr_with_return"
  [(return)
   (set (reg:SI PC_REGNUM)
        (mem:SI (post_inc:SI (match_operand:SI 0 "s_register_operand" "+rk"))))]
  "TARGET_32BIT && (reload_in_progress || reload_completed)"
  "ldr%?\t%|pc, [%0], #4"
  [(set_attr "type" "load1")
   (set_attr "predicable" "yes")]
)
;; Pop for floating point registers (as used in epilogue RTL)
//Pattern#556
(define_insn "*vfp_pop_multiple_with_writeback"
  [(match_parallel 0 "pop_multiple_fp"
    [(set (match_operand:SI 1 "s_register_operand" "+rk")
          (plus:SI (match_dup 1)
                   (match_operand:SI 2 "const_int_operand" "I")))
     (set (match_operand:DF 3 "arm_hard_register_operand" "")
          (mem:DF (match_dup 1)))])]
  "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP"
  "*
  {
    int num_regs = XVECLEN (operands[0], 0);
    char pattern[100];
    rtx op_list[2];
    strcpy (pattern, \"fldmfdd\\t\");
    strcat (pattern, reg_names[REGNO (SET_DEST (XVECEXP (operands[0], 0, 0)))]);
    strcat (pattern, \"!, {\");
    op_list[0] = XEXP (XVECEXP (operands[0], 0, 1), 0);
    strcat (pattern, \"%P0\");
    if ((num_regs - 1) > 1)
      {
        strcat (pattern, \"-%P1\");
        op_list [1] = XEXP (XVECEXP (operands[0], 0, num_regs - 1), 0);
      }

    strcat (pattern, \"}\");
    output_asm_insn (pattern, op_list);
    return \"\";
  }
  "
  [(set_attr "type" "load4")
   (set_attr "conds" "unconditional")
   (set_attr "predicable" "no")]
)

;; Special patterns for dealing with the constant pool

//Pattern#557
(define_insn "align_4"
  [(unspec_volatile [(const_int 0)] VUNSPEC_ALIGN)]
  "TARGET_EITHER"
  "*
  assemble_align (32);
  return \"\";
  "
)

//Pattern#558
(define_insn "align_8"
  [(unspec_volatile [(const_int 0)] VUNSPEC_ALIGN8)]
  "TARGET_EITHER"
  "*
  assemble_align (64);
  return \"\";
  "
)

//Pattern#559
(define_insn "consttable_end"
  [(unspec_volatile [(const_int 0)] VUNSPEC_POOL_END)]
  "TARGET_EITHER"
  "*
  making_const_table = FALSE;
  return \"\";
  "
)

//Pattern#560
(define_insn "consttable_1"
  [(unspec_volatile [(match_operand 0 "" "")] VUNSPEC_POOL_1)]
  "TARGET_THUMB1"
  "*
  making_const_table = TRUE;
  assemble_integer (operands[0], 1, BITS_PER_WORD, 1);
  assemble_zeros (3);
  return \"\";
  "
  [(set_attr "length" "4")]
)

//Pattern#561
(define_insn "consttable_2"
  [(unspec_volatile [(match_operand 0 "" "")] VUNSPEC_POOL_2)]
  "TARGET_THUMB1"
  "*
  making_const_table = TRUE;
  gcc_assert (GET_MODE_CLASS (GET_MODE (operands[0])) != MODE_FLOAT);
  assemble_integer (operands[0], 2, BITS_PER_WORD, 1);
  assemble_zeros (2);
  return \"\";
  "
  [(set_attr "length" "4")]
)

//Pattern#562
(define_insn "consttable_4"
  [(unspec_volatile [(match_operand 0 "" "")] VUNSPEC_POOL_4)]
  "TARGET_EITHER"
  "*
  {
    rtx x = operands[0];
    making_const_table = TRUE;
    switch (GET_MODE_CLASS (GET_MODE (x)))
      {
      case MODE_FLOAT:
 	if (GET_MODE (x) == HFmode)
 	  arm_emit_fp16_const (x);
 	else
 	  {
 	    REAL_VALUE_TYPE r;
 	    REAL_VALUE_FROM_CONST_DOUBLE (r, x);
 	    assemble_real (r, GET_MODE (x), BITS_PER_WORD);
 	  }
 	break;
      default:
	/* XXX: Sometimes gcc does something really dumb and ends up with
	   a HIGH in a constant pool entry, usually because it's trying to
	   load into a VFP register.  We know this will always be used in
	   combination with a LO_SUM which ignores the high bits, so just
	   strip off the HIGH.  */
	if (GET_CODE (x) == HIGH)
	  x = XEXP (x, 0);
        assemble_integer (x, 4, BITS_PER_WORD, 1);
	mark_symbol_refs_as_used (x);
        break;
      }
    return \"\";
  }"
  [(set_attr "length" "4")]
)

//Pattern#563
(define_insn "consttable_8"
  [(unspec_volatile [(match_operand 0 "" "")] VUNSPEC_POOL_8)]
  "TARGET_EITHER"
  "*
  {
    making_const_table = TRUE;
    switch (GET_MODE_CLASS (GET_MODE (operands[0])))
      {
       case MODE_FLOAT:
        {
          REAL_VALUE_TYPE r;
          REAL_VALUE_FROM_CONST_DOUBLE (r, operands[0]);
          assemble_real (r, GET_MODE (operands[0]), BITS_PER_WORD);
          break;
        }
      default:
        assemble_integer (operands[0], 8, BITS_PER_WORD, 1);
        break;
      }
    return \"\";
  }"
  [(set_attr "length" "8")]
)

//Pattern#564
(define_insn "consttable_16"
  [(unspec_volatile [(match_operand 0 "" "")] VUNSPEC_POOL_16)]
  "TARGET_EITHER"
  "*
  {
    making_const_table = TRUE;
    switch (GET_MODE_CLASS (GET_MODE (operands[0])))
      {
       case MODE_FLOAT:
        {
          REAL_VALUE_TYPE r;
          REAL_VALUE_FROM_CONST_DOUBLE (r, operands[0]);
          assemble_real (r, GET_MODE (operands[0]), BITS_PER_WORD);
          break;
        }
      default:
        assemble_integer (operands[0], 16, BITS_PER_WORD, 1);
        break;
      }
    return \"\";
  }"
  [(set_attr "length" "16")]
)

;; Miscellaneous Thumb patterns

//Pattern#565
(define_expand "tablejump"
  [(parallel [(set (pc) (match_operand:SI 0 "register_operand" ""))
	      (use (label_ref (match_operand 1 "" "")))])]
  "TARGET_THUMB1"
  "
  if (flag_pic)
    {
      /* Hopefully, CSE will eliminate this copy.  */
      rtx reg1 = copy_addr_to_reg (gen_rtx_LABEL_REF (Pmode, operands[1]));
      rtx reg2 = gen_reg_rtx (SImode);

      emit_insn (gen_addsi3 (reg2, operands[0], reg1));
      operands[0] = reg2;
    }
  "
)

;; NB never uses BX.
//Pattern#566
(define_insn "*thumb1_tablejump"
  [(set (pc) (match_operand:SI 0 "register_operand" "l*r"))
   (use (label_ref (match_operand 1 "" "")))]
  "TARGET_THUMB1"
  "mov\\t%|pc, %0"
  [(set_attr "length" "2")]
)

;; V5 Instructions,

//Pattern#567
(define_insn "clzsi2"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(clz:SI (match_operand:SI 1 "s_register_operand" "r")))]
  "TARGET_32BIT && arm_arch5"
  "clz%?\\t%0, %1"
  [(set_attr "predicable" "yes")
   (set_attr "insn" "clz")])

//Pattern#568
(define_insn "rbitsi2"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(unspec:SI [(match_operand:SI 1 "s_register_operand" "r")] UNSPEC_RBIT))]
  "TARGET_32BIT && arm_arch_thumb2"
  "rbit%?\\t%0, %1"
  [(set_attr "predicable" "yes")
   (set_attr "insn" "clz")])

//Pattern#569
(define_expand "ctzsi2"
 [(set (match_operand:SI           0 "s_register_operand" "")
       (ctz:SI (match_operand:SI  1 "s_register_operand" "")))]
  "TARGET_32BIT && arm_arch_thumb2"
  "
   {
     rtx tmp = gen_reg_rtx (SImode); 
     emit_insn (gen_rbitsi2 (tmp, operands[1]));
     emit_insn (gen_clzsi2 (operands[0], tmp));
   }
   DONE;
  "
)

;; V5E instructions.

//Pattern#570
(define_insn "prefetch"
  [(prefetch (match_operand:SI 0 "address_operand" "p")
	     (match_operand:SI 1 "" "")
	     (match_operand:SI 2 "" ""))]
  "TARGET_32BIT && arm_arch5e"
  "pld\\t%a0")

;; General predication pattern

//Pattern#571
(define_cond_exec
  [(match_operator 0 "arm_comparison_operator"
    [(match_operand 1 "cc_register" "")
     (const_int 0)])]
  "TARGET_32BIT"
  ""
)

//Pattern#572
(define_insn "force_register_use"
  [(unspec:SI [(match_operand:SI 0 "register_operand" "")] UNSPEC_REGISTER_USE)]
  ""
  "%@ %0 needed"
  [(set_attr "length" "0")]
)


;; Patterns for exception handling

//Pattern#573
(define_expand "eh_return"
  [(use (match_operand 0 "general_operand" ""))]
  "TARGET_EITHER"
  "
  {
    if (TARGET_32BIT)
      emit_insn (gen_arm_eh_return (operands[0]));
    else
      emit_insn (gen_thumb_eh_return (operands[0]));
    DONE;
  }"
)
				   
;; We can't expand this before we know where the link register is stored.
//Pattern#574
(define_insn_and_split "arm_eh_return"
  [(unspec_volatile [(match_operand:SI 0 "s_register_operand" "r")]
		    VUNSPEC_EH_RETURN)
   (clobber (match_scratch:SI 1 "=&r"))]
  "TARGET_ARM"
  "#"
  "&& reload_completed"
  [(const_int 0)]
  "
  {
    arm_set_return_address (operands[0], operands[1]);
    DONE;
  }"
)

//Pattern#575
(define_insn_and_split "thumb_eh_return"
  [(unspec_volatile [(match_operand:SI 0 "s_register_operand" "l")]
		    VUNSPEC_EH_RETURN)
   (clobber (match_scratch:SI 1 "=&l"))]
  "TARGET_THUMB1"
  "#"
  "&& reload_completed"
  [(const_int 0)]
  "
  {
    thumb_set_return_address (operands[0], operands[1]);
    DONE;
  }"
)


;; TLS support

//Pattern#576
(define_insn "load_tp_hard"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(unspec:SI [(const_int 0)] UNSPEC_TLS))]
  "TARGET_HARD_TP"
  "mrc%?\\tp15, 0, %0, c13, c0, 3\\t@ load_tp_hard"
  [(set_attr "predicable" "yes")]
)

;; Doesn't clobber R1-R3.  Must use r0 for the first operand.
//Pattern#577
(define_insn "load_tp_soft"
  [(set (reg:SI 0) (unspec:SI [(const_int 0)] UNSPEC_TLS))
   (clobber (reg:SI LR_REGNUM))
   (clobber (reg:SI IP_REGNUM))
   (clobber (reg:CC CC_REGNUM))]
  "TARGET_SOFT_TP"
  "bl\\t__aeabi_read_tp\\t@ load_tp_soft"
  [(set_attr "conds" "clob")]
)

;; tls descriptor call
//Pattern#578
(define_insn "tlscall"
  [(set (reg:SI R0_REGNUM)
        (unspec:SI [(reg:SI R0_REGNUM)
                    (match_operand:SI 0 "" "X")
	            (match_operand 1 "" "")] UNSPEC_TLS))
   (clobber (reg:SI R1_REGNUM))
   (clobber (reg:SI LR_REGNUM))
   (clobber (reg:SI CC_REGNUM))]
  "TARGET_GNU2_TLS"
  {
    targetm.asm_out.internal_label (asm_out_file, "LPIC",
				    INTVAL (operands[1]));
    return "bl\\t%c0(tlscall)";
  }
  [(set_attr "conds" "clob")
   (set_attr "length" "4")]
)

;; For thread pointer builtin
//Pattern#579
(define_expand "get_thread_pointersi"
  [(match_operand:SI 0 "s_register_operand" "=r")]
 ""
 "
 {
   arm_load_tp (operands[0]);
   DONE;
 }")

;;

;; We only care about the lower 16 bits of the constant 
;; being inserted into the upper 16 bits of the register.
//Pattern#580
(define_insn "*arm_movtas_ze" 
  [(set (zero_extract:SI (match_operand:SI 0 "s_register_operand" "+r")
                   (const_int 16)
                   (const_int 16))
        (match_operand:SI 1 "const_int_operand" ""))]
  "arm_arch_thumb2"
  "movt%?\t%0, %L1"
 [(set_attr "predicable" "yes")
   (set_attr "length" "4")]
)

//Pattern#581
(define_insn "*arm_rev"
  [(set (match_operand:SI 0 "s_register_operand" "=l,l,r")
	(bswap:SI (match_operand:SI 1 "s_register_operand" "l,l,r")))]
  "arm_arch6"
  "@
   rev\t%0, %1
   rev%?\t%0, %1
   rev%?\t%0, %1"
  [(set_attr "arch" "t1,t2,32")
   (set_attr "length" "2,2,4")]
)

//Pattern#582
(define_expand "arm_legacy_rev"
  [(set (match_operand:SI 2 "s_register_operand" "")
	(xor:SI (rotatert:SI (match_operand:SI 1 "s_register_operand" "")
			     (const_int 16))
		(match_dup 1)))
   (set (match_dup 2)
	(lshiftrt:SI (match_dup 2)
		     (const_int 8)))
   (set (match_operand:SI 3 "s_register_operand" "")
	(rotatert:SI (match_dup 1)
		     (const_int 8)))
   (set (match_dup 2)
	(and:SI (match_dup 2)
		(const_int -65281)))
   (set (match_operand:SI 0 "s_register_operand" "")
	(xor:SI (match_dup 3)
		(match_dup 2)))]
  "TARGET_32BIT"
  ""
)

;; Reuse temporaries to keep register pressure down.
//Pattern#583
(define_expand "thumb_legacy_rev"
  [(set (match_operand:SI 2 "s_register_operand" "")
     (ashift:SI (match_operand:SI 1 "s_register_operand" "")
                (const_int 24)))
   (set (match_operand:SI 3 "s_register_operand" "")
     (lshiftrt:SI (match_dup 1)
		  (const_int 24)))
   (set (match_dup 3)
     (ior:SI (match_dup 3)
	     (match_dup 2)))
   (set (match_operand:SI 4 "s_register_operand" "")
     (const_int 16))
   (set (match_operand:SI 5 "s_register_operand" "")
     (rotatert:SI (match_dup 1)
		  (match_dup 4)))
   (set (match_dup 2)
     (ashift:SI (match_dup 5)
                (const_int 24)))
   (set (match_dup 5)
     (lshiftrt:SI (match_dup 5)
		  (const_int 24)))
   (set (match_dup 5)
     (ior:SI (match_dup 5)
	     (match_dup 2)))
   (set (match_dup 5)
     (rotatert:SI (match_dup 5)
		  (match_dup 4)))
   (set (match_operand:SI 0 "s_register_operand" "")
     (ior:SI (match_dup 5)
             (match_dup 3)))]
  "TARGET_THUMB"
  ""
)

//Pattern#584
(define_expand "bswapsi2"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
  	(bswap:SI (match_operand:SI 1 "s_register_operand" "r")))]
"TARGET_EITHER && (arm_arch6 || !optimize_size)"
"
    if (!arm_arch6)
      {
	rtx op2 = gen_reg_rtx (SImode);
	rtx op3 = gen_reg_rtx (SImode);

	if (TARGET_THUMB)
	  {
	    rtx op4 = gen_reg_rtx (SImode);
	    rtx op5 = gen_reg_rtx (SImode);

	    emit_insn (gen_thumb_legacy_rev (operands[0], operands[1],
					     op2, op3, op4, op5));
	  }
	else
	  {
	    emit_insn (gen_arm_legacy_rev (operands[0], operands[1],
					   op2, op3));
	  }

	DONE;
      }
  "
)

;; bswap16 patterns: use revsh and rev16 instructions for the signed
;; and unsigned variants, respectively. For rev16, expose
;; byte-swapping in the lower 16 bits only.
//Pattern#585
(define_insn "*arm_revsh"
  [(set (match_operand:SI 0 "s_register_operand" "=l,l,r")
	(sign_extend:SI (bswap:HI (match_operand:HI 1 "s_register_operand" "l,l,r"))))]
  "arm_arch6"
  "@
  revsh\t%0, %1
  revsh%?\t%0, %1
  revsh%?\t%0, %1"
  [(set_attr "arch" "t1,t2,32")
   (set_attr "length" "2,2,4")]
)

//Pattern#586
(define_insn "*arm_rev16"
  [(set (match_operand:HI 0 "s_register_operand" "=l,l,r")
	(bswap:HI (match_operand:HI 1 "s_register_operand" "l,l,r")))]
  "arm_arch6"
  "@
   rev16\t%0, %1
   rev16%?\t%0, %1
   rev16%?\t%0, %1"
  [(set_attr "arch" "t1,t2,32")
   (set_attr "length" "2,2,4")]
)

//Pattern#587
(define_expand "bswaphi2"
  [(set (match_operand:HI 0 "s_register_operand" "=r")
	(bswap:HI (match_operand:HI 1 "s_register_operand" "r")))]
"arm_arch6"
""
)

;; Patterns for LDRD/STRD in Thumb2 mode

//Pattern#588
(define_insn "*thumb2_ldrd"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
        (mem:SI (plus:SI (match_operand:SI 1 "s_register_operand" "rk")
                         (match_operand:SI 2 "ldrd_strd_offset_operand" "Do"))))
   (set (match_operand:SI 3 "s_register_operand" "=r")
        (mem:SI (plus:SI (match_dup 1)
                         (match_operand:SI 4 "const_int_operand" ""))))]
  "TARGET_LDRD && TARGET_THUMB2 && reload_completed
     && current_tune->prefer_ldrd_strd
     && ((INTVAL (operands[2]) + 4) == INTVAL (operands[4]))
     && (operands_ok_ldrd_strd (operands[0], operands[3],
                                  operands[1], INTVAL (operands[2]),
                                  false, true))"
  "ldrd%?\t%0, %3, [%1, %2]"
  [(set_attr "type" "load2")
   (set_attr "predicable" "yes")])

//Pattern#589
(define_insn "*thumb2_ldrd_base"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
        (mem:SI (match_operand:SI 1 "s_register_operand" "rk")))
   (set (match_operand:SI 2 "s_register_operand" "=r")
        (mem:SI (plus:SI (match_dup 1)
                         (const_int 4))))]
  "TARGET_LDRD && TARGET_THUMB2 && reload_completed
     && current_tune->prefer_ldrd_strd
     && (operands_ok_ldrd_strd (operands[0], operands[2],
                                  operands[1], 0, false, true))"
  "ldrd%?\t%0, %2, [%1]"
  [(set_attr "type" "load2")
   (set_attr "predicable" "yes")])

//Pattern#590
(define_insn "*thumb2_ldrd_base_neg"
  [(set (match_operand:SI 0 "s_register_operand" "=r")
	(mem:SI (plus:SI (match_operand:SI 1 "s_register_operand" "rk")
                         (const_int -4))))
   (set (match_operand:SI 2 "s_register_operand" "=r")
        (mem:SI (match_dup 1)))]
  "TARGET_LDRD && TARGET_THUMB2 && reload_completed
     && current_tune->prefer_ldrd_strd
     && (operands_ok_ldrd_strd (operands[0], operands[2],
                                  operands[1], -4, false, true))"
  "ldrd%?\t%0, %2, [%1, #-4]"
  [(set_attr "type" "load2")
   (set_attr "predicable" "yes")])

//Pattern#591
(define_insn "*thumb2_strd"
  [(set (mem:SI (plus:SI (match_operand:SI 0 "s_register_operand" "rk")
                         (match_operand:SI 1 "ldrd_strd_offset_operand" "Do")))
        (match_operand:SI 2 "s_register_operand" "r"))
   (set (mem:SI (plus:SI (match_dup 0)
                         (match_operand:SI 3 "const_int_operand" "")))
        (match_operand:SI 4 "s_register_operand" "r"))]
  "TARGET_LDRD && TARGET_THUMB2 && reload_completed
     && current_tune->prefer_ldrd_strd
     && ((INTVAL (operands[1]) + 4) == INTVAL (operands[3]))
     && (operands_ok_ldrd_strd (operands[2], operands[4],
                                  operands[0], INTVAL (operands[1]),
                                  false, false))"
  "strd%?\t%2, %4, [%0, %1]"
  [(set_attr "type" "store2")
   (set_attr "predicable" "yes")])

//Pattern#592
(define_insn "*thumb2_strd_base"
  [(set (mem:SI (match_operand:SI 0 "s_register_operand" "rk"))
        (match_operand:SI 1 "s_register_operand" "r"))
   (set (mem:SI (plus:SI (match_dup 0)
                         (const_int 4)))
        (match_operand:SI 2 "s_register_operand" "r"))]
  "TARGET_LDRD && TARGET_THUMB2 && reload_completed
     && current_tune->prefer_ldrd_strd
     && (operands_ok_ldrd_strd (operands[1], operands[2],
                                  operands[0], 0, false, false))"
  "strd%?\t%1, %2, [%0]"
  [(set_attr "type" "store2")
   (set_attr "predicable" "yes")])

//Pattern#593
(define_insn "*thumb2_strd_base_neg"
  [(set (mem:SI (plus:SI (match_operand:SI 0 "s_register_operand" "rk")
                         (const_int -4)))
        (match_operand:SI 1 "s_register_operand" "r"))
   (set (mem:SI (match_dup 0))
        (match_operand:SI 2 "s_register_operand" "r"))]
  "TARGET_LDRD && TARGET_THUMB2 && reload_completed
     && current_tune->prefer_ldrd_strd
     && (operands_ok_ldrd_strd (operands[1], operands[2],
                                  operands[0], -4, false, false))"
  "strd%?\t%1, %2, [%0, #-4]"
  [(set_attr "type" "store2")
   (set_attr "predicable" "yes")])


;; Load the load/store multiple patterns
(include "ldmstm.md")

;; Patterns in ldmstm.md don't cover more than 4 registers. This pattern covers
;; large lists without explicit writeback generated for APCS_FRAME epilogue.
//Pattern#594
(define_insn "*load_multiple"
  [(match_parallel 0 "load_multiple_operation"
    [(set (match_operand:SI 2 "s_register_operand" "=rk")
          (mem:SI (match_operand:SI 1 "s_register_operand" "rk")))
        ])]
  "TARGET_32BIT"
  "*
  {
    arm_output_multireg_pop (operands, /*return_pc=*/false,
                                       /*cond=*/const_true_rtx,
                                       /*reverse=*/false,
                                       /*update=*/false);
    return \"\";
  }
  "
  [(set_attr "predicable" "yes")]
)

;; Vector bits common to IWMMXT and Neon
(include "vec-common.md")
;; Load the Intel Wireless Multimedia Extension patterns
(include "iwmmxt.md")
;; Load the VFP co-processor patterns
(include "vfp.md")
;; Thumb-2 patterns
(include "thumb2.md")
;; Neon patterns
(include "neon.md")
;; Synchronization Primitives
(include "sync.md")
;; Fixed-point patterns
(include "arm-fixed.md")

