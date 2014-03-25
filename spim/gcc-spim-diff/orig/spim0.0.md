;; The dummy instruction has been supported to ensure that no empty
;; arrays are generated in source files of the generated compiler in the
;; initial levels..

(define_insn "dummy_pattern"
        [(reg:SI 0)]
        "1"
        "This stmnt should not be emitted!"
)


(define_insn "nop"
  [(const_int 0)]
  ""
  "%(nop%)"
)

