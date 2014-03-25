;; The dummy instruction has been supported to ensure that no empty
;; arrays are generated in source files of the generated compiler in the
;; initial levels..

(define_insn "dummy_pattern"
        [(reg:SI 0)]
        "1"
        "This stmnt should not be emitted!"
)

;; Functions in genemit.c read .md file for desired target and generates
;; gen_<insn-name> function  for each standard named  pattern defined in
;; .md file.  The function gen_<insn-name>  is in turn used  to generate
;; RTLs at the  time of transforming input program into  RTL. The source
;; files  cfgrtl.c,  cse.c,expmed.c which  contribute  in  cc1, use  the
;; function gen_jump to generate insn corresponding to jump instruction.
;; If this pattern is not  defined in machine description, the compiler,
;; fails in the linking phase because gen_jump is not defined.


;; For compiling _any_ program, jumps are a must.

(define_insn "jump"
        [(set (pc) (label_ref (match_operand 0 "" "")))]
        ""
	{
                return "j \\t%l0";
        }
)

(define_insn "indirect_jump"
	[(set (pc) (match_operand:SI 0 "register_operand" ""))]
	""
	"jr \\t%0"
)

(define_expand "epilogue"
        [(clobber (const_int 0))]
        ""
        {
                spim_epilogue();
                DONE;
        }
)

(define_insn "IITB_return"
	[(return)]
	""
	"jr \\t\\$ra"
)

(define_expand "movsi"
	[(set (match_operand:SI 0 "nonimmediate_operand" "")
	      (match_operand:SI 1 "general_operand" "")
	)]
	""
	{
	  
	  if(GET_CODE(operands[0])==MEM && GET_CODE(operands[1])!=REG)
	  {
	    
	    if(can_create_pseudo_p())
	    {
		operands[1]=force_reg(SImode,operands[1]);
	    }
	  }
	}
)

(define_insn "nop"
  [(const_int 0)]
  ""
  "%(nop%)"
)

;;Load patterns

(define_insn "*load_word"
        [(set (match_operand:SI 0 "register_operand" "=r")
              (match_operand:SI 1 "memory_operand" "m")
        )]
        ""
        "lw \\t%0, %m1"
)

;;store patterns

(define_insn "*store_word"
        [(set (match_operand:SI 0 "memory_operand" "=m")
              (match_operand:SI 1 "register_operand" "r")
        )]
        ""
        "sw \\t%1, %m0"
)

