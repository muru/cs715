#include "config.h"
#include "system.h"
#include "cgraph.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "tree-flow.h"
#include "diagnostic.h"
#include "tree-pass.h"
#include "timevar.h"
#include "alloc-pool.h"
#include "params.h"
#include "ggc.h"
#include "vec.h"

/* Changes from tree-ssa-structalias.c. Lookup a heap var for FROM, and return it if we find one.

struct GTY(()) heapvar_map {
  struct tree_map map;
  unsigned HOST_WIDE_INT offset;
};

static GTY ((if_marked ("tree_map_marked_p"), param_is (struct heapvar_map)))
htab_t heapvar_for_stmt;

static int
heapvar_map_eq (const void *p1, const void *p2)
{
  const struct heapvar_map *h1 = (const struct heapvar_map *)p1;
  const struct heapvar_map *h2 = (const struct heapvar_map *)p2;
  return (h1->map.base.from == h2->map.base.from
          && h1->offset == h2->offset);
}

static unsigned int
heapvar_map_hash (struct heapvar_map *h)
{
  return iterative_hash_host_wide_int (h->offset,
                                       htab_hash_pointer (h->map.base.from));
}

//fibheap_t heap;
*/
/* Changes from tree-ssa-structalias.c. Map from trees to variable infos.  */
struct pointer_map_t *vi_for_tree;


enum vec_l
{maylist,tmplist};


typedef enum vec_l vec_list;

enum bb_in_out {IN, OUT};

typedef enum bb_in_out in_out;

/* A generic constraint container. A constraint can be :
   - an alias
   - A live variable */
struct df_container
{
    unsigned int val;
    struct df_container *next;
};
typedef struct df_container *df_list;

/* A data structure to store the statements of interest.
   They can be of two types :
	* Alias statements
	* Statements that generate liveness
   In case of PHI node, we can have a single LHS and multiple RHS in
   our constraints. Also, consecutive liveness is stored in chain.
   For example, a PHI node has a structure like a_1 = PHI (a_3, a_5)
   which essentially means that the alias information will be
   a_1 -> a_3 or a_5. This info is stored in stmt_constraint_chain.
   The alias chain will thus consist of {a_1 -> a_5} linked to
   {a_1 -> a_3}. */
struct constraint_list_def
{
    df_list stmt_constraint_chain;
    bool is_alias;
    struct constraint_list_def *next;
    struct constraint_list_def *prev;
};
typedef struct constraint_list_def *constraint_list;
constraint_list last_constraint=NULL, fptr_constraint_list=NULL;

/* Data structure to represent pointsto values. Our typical representation
   in this implementation is of the form {a -> b, c, d} which means a can
   point to b, c, and d. Here, a forms the LHS element of the pointsto
   value and {b, c, d} form the RHS element. */

/* The RHS of a pointsto value. It is a list of rhs values. */


typedef struct pointsto_val_rhs_def *pointsto_val_rhs;

/* The final pointsto value. */
struct pointsto_val_def
{
    unsigned int lhs;
    bitmap rhs;
    struct pointsto_val_def *next;
};
typedef struct pointsto_val_def *pointsto_val;

struct cs_dfa
{
    int inc_linfo;
    int linfo;
    int pinfo;
    bool new_lvdf;
    bool new_ptadf;
    int cs_index;
    struct cs_dfa *next;
};
typedef struct cs_dfa *csdfa_info;

struct bb_worklist_def
{
    bool process_pta;
    bool process_lv;
    basic_block bb;
};
typedef struct bb_worklist_def *bb_worklist;

/* The auxillary data structre associated with each basic block.
   This consists of the cgraph node which this basic block belongs to. The IN and
   OUT pointsto information associated with the basic block, the callstring map
   (if the basic block is Sp), and flags to determine the type of the block
   (call block, return block, end block, start block) */
struct bb_csipdfa_info_def
{
    int rp_index;
    struct cgraph_node *cnode;
    csdfa_info in_ipdfa, out_ipdfa;
    constraint_list constraint_start, constraint_end;
    bool call_block, return_block;
    bool start_block, end_block;
    bool lv_bypass_block, pta_bypass_block;
    bool bb_has_deref_stmts;
};
typedef struct bb_csipdfa_info_def *bb_csipdfa_info;

/* Callstrings are represented as a sequence of callblocks. */
struct callstring_def
{
    basic_block bb;
    struct callstring_def *next;
};
typedef struct callstring_def *callstring;

/* A callstring map is stored at Sp (start of each procedure p). This contains a
   representative callstring index at LHS, and the callstring indices of all
   pointsto values at RHS that have the same must-may info as the representative
   callstring. For example, if callstrings W, X, Y, Z have the same pointsto value
   associated with them, and we choose X to be the representative callstring, the
   lhs of pcs_map_def will be the index of W in callstring pool, and the rhs
   will be the csmap_rhs consisting of the linked list of indices of X, Y and Z. */
struct csmap_rhs_def
{
    csdfa_info rhsel;
    bool present_in_out;
    struct csmap_rhs_def *next;
};
typedef struct csmap_rhs_def *csmap_rhs;

struct cs_map_def
{
    csdfa_info lhs;
    bool present_in_out;
    csmap_rhs rhs;
    struct cs_map_def *next;
};
typedef struct cs_map_def *cs_map;

/* Auxillary data structure associated with each cgraph node. This data
   structure stores the callstring map and the array of blocks to be
   processed */
struct cgraph_info_def
{
    bb_worklist bb_list;
    cs_map pcsmap, lcsmap;
    bool pcsmap_required;
    bool lcsmap_required;
};
typedef struct cgraph_info_def *cgraph_info;

/* The worklist queue. It consists of list of cgraph nodes that need to be serviced. */
struct worklist_queue
{
    struct cgraph_node *called_fn;
    bool process_pta;
    bool process_lv;
    struct worklist_queue *next;
};
typedef struct worklist_queue *worklist;
worklist start, lv_node, pta_node;

/* -----------------------------------------------------------------------------------
   Data structures picked up (and possibly modified) from tree-ssa-structalias.h file.
   -----------------------------------------------------------------------------------*/

struct csvariable_info
{
    /* ID of this variable  */
    unsigned int id;

    /* True for variables whose size is not known or variable.  */
    unsigned int is_unknown_size_var : 1;

    /* True for (sub-)fields that represent a whole variable.  */
    unsigned int is_full_var : 1;

    /* True if this field may contain pointers.  */
    unsigned int may_have_pointers : 1;

    /* True if it is a heap var. */
    unsigned int is_heap_var : 1;

    /* True if this represents a global variable.  */
    unsigned int is_global_var : 1;

    /* A link to the variable for the next field in this structure.  */
    struct csvariable_info *next;

    /* Offset of this variable, in bits, from the base variable  */
    unsigned HOST_WIDE_INT offset;

    /* Size of the variable, in bits.  */
    unsigned HOST_WIDE_INT size;

    /* Full size of the base variable, in bits.  */
    unsigned HOST_WIDE_INT fullsize;

    /* Name of this variable */
    const char *name;

    /* Tree that this variable is associated with.  */
    tree decl;

    /* The list of all the constraints {aliases} that have this
       variable as their LHS */
    df_list constraints_with_vi_as_lhs;

    /* The function in whose scope this variable is defined. */
    struct cgraph_node *scoping_function;
};
typedef struct csvariable_info *csvarinfo_t;

typedef enum {SCALAR, DEREF, ADDRESSOF} constraint_expr_type;

/* Use 0x8000... as special unknown offset.  */
#define UNKNOWN_OFFSET ((HOST_WIDE_INT)-1 << (HOST_BITS_PER_WIDE_INT-1))

/* An expression that appears in a constraint.  */

struct constraint_expr
{
    /* Constraint type.  */
    constraint_expr_type type;

    /* Variable we are referring to in the constraint.  */
    unsigned int var;

    /* Offset, in bits, of this constraint from the beginning of
       variables it ends up referring to.

       IOW, in a deref constraint, we would deref, get the result set,
       then add OFFSET to each member.   */
    HOST_WIDE_INT offset;
};

typedef struct constraint_expr ce_s;
DEF_VEC_O(ce_s);
DEF_VEC_ALLOC_O(ce_s, heap);

struct constraint
{
    struct constraint_expr lhs;
    struct constraint_expr rhs;
};
typedef struct constraint *constraint_t;

/* This structure is used during pushing fields onto the fieldstack
   to track the offset of the field, since bitpos_of_field gives it
   relative to its immediate containing type, and we want it relative
   to the ultimate containing object.  */

struct fieldoff
{
    /* Offset from the base of the base containing object to this field.  */
    HOST_WIDE_INT offset;

    /* Size, in bits, of the field.  */
    unsigned HOST_WIDE_INT size;

    unsigned has_unknown_size : 1;

    unsigned must_have_pointers : 1;

    unsigned may_have_pointers : 1;

    unsigned only_restrict_pointers : 1;
};
typedef struct fieldoff fieldoff_s;

DEF_VEC_O(fieldoff_s);
DEF_VEC_ALLOC_O(fieldoff_s,heap);


/*----------------------
  Variable Declarations.
  ---------------------*/
#define SSAVAR(x) (TREE_CODE (x) == SSA_NAME ? SSA_NAME_VAR (x) : x)
#define alloc_mem ggc_alloc_cleared_atomic
#define total_bbs n_basic_blocks-NUM_FIXED_BLOCKS

enum {undef_id = 0, nothing_id = 1, integer_id = 2};
struct cgraph_node *cnode = NULL;
basic_block cbb = NULL, main_firstbb = NULL;
bool multi_rhs = false;
bool compute_only_pinfo = false;
bool compute_alias_and_pinfo = false;
bool all_contexts_together = true;
bool check_deref = false;
bool deref_stmt = false;
bool recompute_liveness = false;
csdfa_info cpinfo = NULL;
struct timeval tstart, tend, tresult;

extern struct pointer_map_t *vi_for_tree;
//extern alloc_pool constraint_pool;
static alloc_pool csvarinfo_pool;

/*--------------------------------
  VEC data structure declarations.
  -------------------------------*/
DEF_VEC_P(csvarinfo_t);
DEF_VEC_ALLOC_P(csvarinfo_t, heap);
static VEC(csvarinfo_t, heap) *csvarmap;
DEF_VEC_P(constraint_t);
DEF_VEC_ALLOC_P(constraint_t,heap);
static VEC(constraint_t, heap) *aliases;
DEF_VEC_P(pointsto_val);
DEF_VEC_ALLOC_P(pointsto_val, heap);
static VEC(pointsto_val, heap) *mayinfo;
static VEC(pointsto_val, heap) *tmpinfo;
DEF_VEC_P(callstring);
DEF_VEC_ALLOC_P(callstring, heap);
static VEC(callstring, heap) *callstrings;
DEF_VEC_P(df_list);
DEF_VEC_ALLOC_P(df_list, heap);
static VEC(df_list, heap) *liveinfo;


VEC(constraint_t,heap) *constraints;
alloc_pool constraint_pool;

/*-----------------------
   Function Declarations.
 ------------------------*/
const char * alias_get_name (tree);
bool field_must_have_pointers (tree);
//tree heapvar_lookup (tree, unsigned HOST_WIDE_INT);
//void heapvar_insert (tree, unsigned HOST_WIDE_INT, tree);
unsigned int count_num_arguments (tree, bool *);
//void init_alias_heapvars (void);
constraint_t new_constraint (const struct constraint_expr, const struct constraint_expr);
bool constraint_equal (struct constraint, struct constraint);
bool constraint_expr_equal (struct constraint_expr, struct constraint_expr);
inline bool var_can_have_subvars (const_tree);
bool push_fields_onto_fieldstack (tree, VEC(fieldoff_s,heap) **, HOST_WIDE_INT);
bool check_for_overlaps (VEC (fieldoff_s,heap) *);
void sort_fieldstack (VEC(fieldoff_s,heap) *);

static void cs_get_constraint_for_address_of (tree, VEC (ce_s, heap) **);
static void cs_get_constraint_for_rhs (tree, VEC (ce_s, heap) **);
static void cs_get_constraint_for_1 (tree, VEC (ce_s, heap) **, bool, bool);
static void cs_process_constraint (constraint_t);
static void compute_stmt_out_1 (csdfa_info, constraint_t);
static void compute_stmt_out_2 (csdfa_info, constraint_t);
static int copy_pointsto_set_to_temp(int);
static void compute_liveness (bool);
static void modify_lcsmap_at_endblock (struct cgraph_node *, csdfa_info, bool);
static void print_lcsmap_at_endblock (basic_block);
static void print_df_list(int);
static df_list copy_df_list(df_list);
static bitmap copy_pointsto_rhs (bitmap);
static bitmap normalize_rhs_val (bitmap);
static bool element_equal(unsigned int, unsigned int);

static csdfa_info in_of_bb(basic_block);
static csdfa_info out_of_bb(basic_block);

/*-------------------------------------------------
	FUNCTIONS FROM tree-ssa-structalias.c
---------------------------------------------------*/

/* Return the position, in bits, of FIELD_DECL from the beginning of its
   structure.  */

HOST_WIDE_INT
bitpos_of_field (const tree fdecl)
{
    if (!host_integerp (DECL_FIELD_OFFSET (fdecl), 0)
            || !host_integerp (DECL_FIELD_BIT_OFFSET (fdecl), 0))
        return -1;

    return (TREE_INT_CST_LOW (DECL_FIELD_OFFSET (fdecl)) * BITS_PER_UNIT
            + TREE_INT_CST_LOW (DECL_FIELD_BIT_OFFSET (fdecl)));
}


/* Lookup a heap var for FROM, and return it if we find one.

tree
heapvar_lookup (tree from, unsigned HOST_WIDE_INT offset)
{
  struct heapvar_map *h, in;
  in.map.base.from = from;
  in.offset = offset;
  h = (struct heapvar_map *) htab_find_with_hash (heapvar_for_stmt, &in,
                                                  heapvar_map_hash (&in));
  if (h)
    return h->map.to;
  return NULL_TREE;
}*/

/* Insert a mapping FROM->TO in the heap var for statement
   hashtable.

void
heapvar_insert (tree from, unsigned HOST_WIDE_INT offset, tree to)
{
  struct heapvar_map *h;
  void **loc;

  h = ggc_alloc_heapvar_map ();
  h->map.base.from = from;
  h->offset = offset;
  h->map.hash = heapvar_map_hash (h);
  h->map.to = to;
  loc = htab_find_slot_with_hash (heapvar_for_stmt, h, h->map.hash, INSERT);
  gcc_assert (*loc == NULL);
  *(struct heapvar_map **) loc = h;
} */

/* Create a new constraint consisting of LHS and RHS expressions.  */

constraint_t
new_constraint (const struct constraint_expr lhs,
                const struct constraint_expr rhs)
{
    constraint_t ret = (constraint_t) pool_alloc (constraint_pool);
    ret->lhs = lhs;
    ret->rhs = rhs;
    return ret;
}

/* Return true if two constraint expressions A and B are equal.  */

bool
constraint_expr_equal (struct constraint_expr a, struct constraint_expr b)
{
    return a.type == b.type && a.var == b.var && a.offset == b.offset;
}

/* Return true if two constraints A and B are equal.  */

bool
constraint_equal (struct constraint a, struct constraint b)
{
    return constraint_expr_equal (a.lhs, b.lhs)
           && constraint_expr_equal (a.rhs, b.rhs);
}

/* Return a printable name for DECL  */

const char *
alias_get_name (tree decl)
{
    const char *res;
    char *temp;
    int num_printed = 0;

    if (DECL_ASSEMBLER_NAME_SET_P (decl))
        res = IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (decl));
    else
        res= get_name (decl);
    if (res != NULL)
        return res;

    res = "NULL";
    if (!dump_file)
        return res;

    if (TREE_CODE (decl) == SSA_NAME)
    {
        num_printed = asprintf (&temp, "%s_%u",
                                alias_get_name (SSA_NAME_VAR (decl)),
                                SSA_NAME_VERSION (decl));
    }
    else if (DECL_P (decl))
    {
        num_printed = asprintf (&temp, "D.%u", DECL_UID (decl));
    }
    if (num_printed > 0)
    {
        res = ggc_strdup (temp);
        free (temp);
    }
    return res;
}

/* Return true if V is a tree that we can have subvars for.
   Normally, this is any aggregate type.  Also complex
   types which are not gimple registers can have subvars.  */

inline bool
var_can_have_subvars (const_tree v)
{
    /* Volatile variables should never have subvars.  */
    if (TREE_THIS_VOLATILE (v))
        return false;

    /* Non decls or memory tags can never have subvars.  */
    if (!DECL_P (v))
        return false;

    /* Aggregates without overlapping fields can have subvars.  */
    if (TREE_CODE (TREE_TYPE (v)) == RECORD_TYPE)
        return true;

    return false;
}

/* Return true if T is a type that does contain pointers.  */

bool
type_must_have_pointers (tree type)
{
    //fprintf(stderr,"\nin field must have pointers 1type\n");

    if (POINTER_TYPE_P (type))
    {
      //  fprintf(stderr,"\nin field must have pointers 1\n");

        return true;
    }

    if (TREE_CODE (type) == ARRAY_TYPE)
    {
        //fprintf(stderr,"\nin field must have pointers 2\n");

        return type_must_have_pointers (TREE_TYPE (type));
    }

    /* A function or method can have pointers as arguments, so track
       those separately.  */
    if (TREE_CODE (type) == FUNCTION_TYPE
            || TREE_CODE (type) == METHOD_TYPE)
    {
       // fprintf(stderr,"\nin field must have pointers 3\n");

        return true;
    }

    return false;
}

bool
field_must_have_pointers (tree t)
{
    tree tmp = TREE_TYPE(t);
 //   fprintf(stderr,"\nafter \n");
    return type_must_have_pointers (tmp);
}

/* Given a TYPE, and a vector of field offsets FIELDSTACK, push all
   the fields of TYPE onto fieldstack, recording their offsets along
   the way.

   OFFSET is used to keep track of the offset in this entire
   structure, rather than just the immediately containing structure.
   Returns false if the caller is supposed to handle the field we
   recursed for.  */

bool
push_fields_onto_fieldstack (tree type, VEC(fieldoff_s,heap) **fieldstack,
                             HOST_WIDE_INT offset)
{
    tree field;
    bool empty_p = true;

    if (TREE_CODE (type) != RECORD_TYPE)
        return false;

    /* If the vector of fields is growing too big, bail out early.
       Callers check for VEC_length <= MAX_FIELDS_FOR_FIELD_SENSITIVE, make
       sure this fails.  */
    if (VEC_length (fieldoff_s, *fieldstack) > MAX_FIELDS_FOR_FIELD_SENSITIVE)
        return false;

    for (field = TYPE_FIELDS (type); field; field = DECL_CHAIN (field))
        if (TREE_CODE (field) == FIELD_DECL)
        {
            bool push = false;
            HOST_WIDE_INT foff = bitpos_of_field (field);

            if (!var_can_have_subvars (field)
                    || TREE_CODE (TREE_TYPE (field)) == QUAL_UNION_TYPE
                    || TREE_CODE (TREE_TYPE (field)) == UNION_TYPE)
                push = true;
            else if (!push_fields_onto_fieldstack
                     (TREE_TYPE (field), fieldstack, offset + foff)
                     && (DECL_SIZE (field)
                         && !integer_zerop (DECL_SIZE (field))))
                /* Empty structures may have actual size, like in C++.  So
                   see if we didn't push any subfields and the size is
                   nonzero, push the field onto the stack.  */
                push = true;

            if (push)
            {
                fieldoff_s *pair = NULL;
                bool has_unknown_size = false;
                bool must_have_pointers_p;

                if (!VEC_empty (fieldoff_s, *fieldstack))
                    pair = VEC_last (fieldoff_s, *fieldstack);

                /* If there isn't anything at offset zero, create sth.  */
                if (!pair
                        && offset + foff != 0)
                {
                    pair = VEC_safe_push (fieldoff_s, heap, *fieldstack, NULL);
                    pair->offset = 0;
                    pair->size = offset + foff;
                    pair->has_unknown_size = false;
                    pair->must_have_pointers = false;
                    pair->may_have_pointers = false;
                    pair->only_restrict_pointers = false;
                }

                if (!DECL_SIZE (field)
                        || !host_integerp (DECL_SIZE (field), 1))
                    has_unknown_size = true;

                /* If adjacent fields do not contain pointers merge them.  */
                must_have_pointers_p = field_must_have_pointers (field);
                if (pair
                        && !has_unknown_size
                        && !must_have_pointers_p
                        && !pair->must_have_pointers
                        && !pair->has_unknown_size
                        && pair->offset + (HOST_WIDE_INT)pair->size == offset + foff)
                {
                    pair->size += TREE_INT_CST_LOW (DECL_SIZE (field));
                }
                else
                {
                    pair = VEC_safe_push (fieldoff_s, heap, *fieldstack, NULL);
                    pair->offset = offset + foff;
                    pair->has_unknown_size = has_unknown_size;
                    if (!has_unknown_size)
                        pair->size = TREE_INT_CST_LOW (DECL_SIZE (field));
                    else
                        pair->size = -1;
                    pair->must_have_pointers = must_have_pointers_p;
                    pair->may_have_pointers = true;
                    pair->only_restrict_pointers
                        = (!has_unknown_size
                           && POINTER_TYPE_P (TREE_TYPE (field))
                           && TYPE_RESTRICT (TREE_TYPE (field)));
                }
            }

            empty_p = false;
        }

    return !empty_p;
}

/* Count the number of arguments DECL has, and set IS_VARARGS to true
   if it is a varargs function.  */

unsigned int
count_num_arguments (tree decl, bool *is_varargs)
{
    unsigned int num = 0;
    tree t;

    /* Capture named arguments for K&R functions.  They do not
       have a prototype and thus no TYPE_ARG_TYPES.  */
    for (t = DECL_ARGUMENTS (decl); t; t = DECL_CHAIN (t))
        ++num;

    /* Check if the function has variadic arguments.  */
    for (t = TYPE_ARG_TYPES (TREE_TYPE (decl)); t; t = TREE_CHAIN (t))
        if (TREE_VALUE (t) == void_type_node)
            break;
    if (!t)
        *is_varargs = true;

    return num;
}

/* Return true if FIELDSTACK contains fields that overlap.
   FIELDSTACK is assumed to be sorted by offset.  */

bool
check_for_overlaps (VEC (fieldoff_s,heap) *fieldstack)
{
    fieldoff_s *fo = NULL;
    unsigned int i;
    HOST_WIDE_INT lastoffset = -1;

    FOR_EACH_VEC_ELT (fieldoff_s, fieldstack, i, fo)
    {
        if (fo->offset == lastoffset)
            return true;
        lastoffset = fo->offset;
    }
    return false;
}

/* Initialize the heapvar for statement mapping.

void
init_alias_heapvars (void)
{
  if (!heapvar_for_stmt)
    heapvar_for_stmt = htab_create_ggc (11, tree_map_hash, heapvar_map_eq,
                                        NULL);
} */

/* qsort comparison function for two fieldoff's PA and PB */

int
fieldoff_compare (const void *pa, const void *pb)
{
    const fieldoff_s *foa = (const fieldoff_s *)pa;
    const fieldoff_s *fob = (const fieldoff_s *)pb;
    unsigned HOST_WIDE_INT foasize, fobsize;

    if (foa->offset < fob->offset)
        return -1;
    else if (foa->offset > fob->offset)
        return 1;

    foasize = foa->size;
    fobsize = fob->size;
    if (foasize < fobsize)
        return -1;
    else if (foasize > fobsize)
        return 1;
    return 0;
}

/* Sort a fieldstack according to the field offset and sizes.  */
void
sort_fieldstack (VEC(fieldoff_s,heap) *fieldstack)
{
    VEC_qsort (fieldoff_s, fieldstack, fieldoff_compare);
}

/*******HELPER FUNCTIONS FOR APIS***********/

static csdfa_info
get_csdfa_info_bb(basic_block bb, int cs_index , in_out io)
{
    csdfa_info ai=NULL,bi;
    if(io==IN) ai = in_of_bb(bb);
    if(io == OUT) ai = out_of_bb(bb);
    if(cs_index == -2)
        return ai;
    for(bi = ai; bi ; bi= bi->next)
    {
        if(bi->cs_index >= cs_index)
            break;
    }
    if(cs_index == bi->cs_index)
        return bi;
    return NULL;
}

static bool
df_list_equal (df_list a, df_list b)
{
    while (a)
    {
        if (b == NULL)
            return false;
        if (a->val != b->val)
            return false;
        if (a->val == b->val)
        {
            a = a->next;
            b = b->next;
            continue;
        }
    }
    if (a || b)
        return false;

    return true;
}

static df_list
copy_df_list (df_list a)
{
    df_list root=NULL, ri=NULL;
    for (; a; a=a->next)
    {
        df_list tmp = (df_list) alloc_mem (sizeof (struct df_container));
        tmp->val = a->val;
        if (root == NULL)
            ri = root = tmp;
        else
        {
            ri->next = tmp;
            ri = ri->next;
        }
    }
    return root;
}


static int
index_in_livepool (df_list a)
{
    unsigned int i;
    for (i=0; i<VEC_length (df_list, liveinfo); i++)
    {
        if (df_list_equal (a, VEC_index (df_list, liveinfo, i)))
            return i;
    }
    return -1;
}



static int
compute_liveness_index (df_list in)
{
    int index;
    if (in == NULL)
        return -1;
    else
    {
        index = index_in_livepool (in);
        if (index < 0)
        {
            index = VEC_length (df_list, liveinfo);
            VEC_safe_push (df_list, heap, liveinfo, in);
        }
    }
    return index;
}

/* Return a copy of the given pointsto information. */
static pointsto_val
copy_pointsto_val (pointsto_val a)
{
    pointsto_val root=NULL, ri=NULL;
    for (; a; a=a->next)
    {
        pointsto_val tmp;
        tmp = (pointsto_val) alloc_mem (sizeof (struct pointsto_val_def));
        tmp->lhs = a->lhs;
        tmp->rhs = copy_pointsto_rhs (a->rhs);
        if (root == NULL)
            ri = root = tmp;
        else
        {
            ri->next = tmp;
            ri = ri->next;
        }
    }
    return root;
}

static bool
pointsto_val_equal (pointsto_val a, pointsto_val b)
{

    while (a)
    {
        if (b == NULL)
            return false;
        if (!element_equal (a->lhs, b->lhs))
            return false;
        else
        {
            if(!bitmap_equal_p(a->rhs,b->rhs))
                return false;
        }
        a = a->next;
        b = b->next;
    }
    if (b)
        return false;
    return true;
}

/* Prepend undef to the RHS value if it contains only one
   element (which is not undef). */
static bitmap
prepend_undef_to_rhs (bitmap a)
{
    if(bitmap_count_bits(a)==1)
    {
        bitmap_iterator bi;
        unsigned int bit;
        EXECUTE_IF_SET_IN_BITMAP(a, 0 , bit, bi)
        {
            if(bit != undef_id)
                bitmap_set_bit(a,undef_id);
        }

    }
    return a;
}


static int
index_in_maypool (pointsto_val a)
{

    unsigned int i;
    for (i=0; i<VEC_length (pointsto_val, mayinfo); i++)
    {
        if (pointsto_val_equal (a, VEC_index (pointsto_val, mayinfo, i)))
            return i;
    }
    return -1;
}


static int
index_in_tmppool (pointsto_val a)
{
    unsigned int i;
    for (i=0; i<VEC_length (pointsto_val, tmpinfo); i++)
    {
        if (pointsto_val_equal (a, VEC_index (pointsto_val, tmpinfo, i)))
            return i;
    }
    return -1;
}




static int
compute_index_in_maypool(pointsto_val out)
{
    int index =-1;
    /*pointsto_val out = (index < 0) ? NULL : VEC_index (pointsto_val, mayinfo, index);*/

    if (out == NULL)
        return -1;
    else
    {
        index = index_in_maypool (out);
        if (index < 0)
        {
            index = VEC_length (pointsto_val, mayinfo);
            VEC_safe_push (pointsto_val, heap, mayinfo, out);
        }
    }
    return index;


}


static int
compute_index_in_tmppool(pointsto_val out)
{

    /*pointsto_val out = (index < 0) ? NULL : VEC_index (pointsto_val, tmpinfo, index);*/
    int index =-1;
    if (out == NULL)
        return -1;
    else
    {
        index = index_in_tmppool (out);
        if (index < 0)
        {
            index = VEC_length (pointsto_val, tmpinfo);
            VEC_safe_push (pointsto_val, heap, tmpinfo, out);
        }
    }
    return index;


}



static void empty_temp_set(void)
{
    VEC_free(pointsto_val,heap,tmpinfo);
}

/***** APIs for Pointer Information ***********/

static  bitmap
get_pointee_set(int in_index,vec_list list, unsigned int varid)
{

    /*printf("\nget_pointee_set\n");*/
    pointsto_val ai,in = NULL;
    bitmap pointee_set = BITMAP_ALLOC (NULL);
    if(list == tmplist)
        in = (in_index < 0) ? NULL :
             copy_pointsto_val (VEC_index (pointsto_val, tmpinfo, in_index));
    if(list == maylist)
        in = (in_index < 0) ? NULL :
             copy_pointsto_val (VEC_index (pointsto_val, mayinfo, in_index));




    for (ai=in; ai && ai->lhs<=varid; ai=ai->next)
    {
        if (element_equal (ai->lhs, varid))
        {
            return ai->rhs;
        }
    }

    return pointee_set;

}


static bitmap
get_indirect_pointee_set(int in_index,vec_list list, unsigned int varid)
{

    /*printf("\nget_indirect_pointee_set\n");*/

    bitmap pointee_set = get_pointee_set(in_index, list ,varid);

    bitmap_iterator bi,ci;
    unsigned int var,pointee;
    bitmap indirect_pointee_set, temp_set;
    indirect_pointee_set = BITMAP_ALLOC(NULL);
    EXECUTE_IF_SET_IN_BITMAP(pointee_set,0,var,bi)
    {
        temp_set = get_pointee_set(in_index,list,var);
        EXECUTE_IF_SET_IN_BITMAP(temp_set,0,pointee,ci)
        {
            bitmap_set_bit(indirect_pointee_set,pointee);
        }
    }

    return indirect_pointee_set;
}

static bool
has_must_pointee(int in_index, vec_list list, unsigned int varid)
{
    /*printf("\nhas_must_pointee\n");*/

    pointsto_val ai,in= NULL;
    if(list == tmplist)
        in = (in_index < 0) ? NULL :  copy_pointsto_val (VEC_index (pointsto_val, tmpinfo, in_index));
    if(list == maylist)
        in = (in_index < 0) ? NULL :  copy_pointsto_val (VEC_index (pointsto_val, mayinfo, in_index));


    for (ai=in; ai && ai->lhs<=varid; ai=ai->next)
    {
        if (element_equal (ai->lhs, varid))
        {
            if(bitmap_count_bits(ai->rhs)==1)
                return true;
        }
    }

    return false;
}


static int
get_must_pointee(int in_index, vec_list list, unsigned int varid)
{
    pointsto_val in, ai;
    if(list == maylist)
        in = (in_index < 0) ? NULL :  copy_pointsto_val (VEC_index (pointsto_val, mayinfo, in_index));
    if(list == tmplist)
        in = (in_index < 0) ? NULL :  copy_pointsto_val (VEC_index (pointsto_val, tmpinfo, in_index));

    for (ai=in; ai && ai->lhs<=varid; ai=ai->next)
    {
        if (element_equal (ai->lhs, varid))
        {
            if(bitmap_count_bits(ai->rhs) == 1)
            {
                return bitmap_first_set_bit(ai->rhs);
            }
            break;
        }
    }
    return -1;
}


static bool
set_must_pointee(int in_index, vec_list list, unsigned int varid, unsigned int pointee)
{
    pointsto_val in, ai;
    if(list == maylist)
        in = (in_index < 0) ? NULL :  copy_pointsto_val (VEC_index (pointsto_val, mayinfo, in_index));
    if(list == tmplist)
        in = (in_index < 0) ? NULL :  copy_pointsto_val (VEC_index (pointsto_val, tmpinfo, in_index));

    for (ai=in; ai && ai->lhs<=varid; ai=ai->next)
    {
        if (element_equal (ai->lhs, varid))
        {
            ai->rhs = BITMAP_ALLOC(NULL);
            bitmap_set_bit(ai->rhs,pointee);
            break;
        }
    }


}


static bool
pointer_with_non_empty_pointee_set(int in_index,vec_list list, unsigned int varid)
{

    /*printf("\nvar_has_pointee\n");*/

    pointsto_val ai,in=NULL;
    if(list == tmplist)
        in = (in_index < 0) ? NULL :  copy_pointsto_val (VEC_index (pointsto_val, tmpinfo, in_index));
    if(list == maylist)
        in = (in_index < 0) ? NULL :  copy_pointsto_val (VEC_index (pointsto_val, mayinfo, in_index));

    for (ai=in; ai && ai->lhs<=varid; ai=ai->next)
    {
        if (element_equal (ai->lhs, varid))
        {
            if(bitmap_count_bits(ai->rhs)>=1)
                return true;
            else
                return false;
        }
    }

    return false;
}

static bool
is_x_in_pointsto_set (int in_index,vec_list list, unsigned int x)
{

    /*printf("\nis_x_in\n");*/

    pointsto_val ci,in=NULL;
    if(list == tmplist)
        in = (in_index < 0) ? NULL :  copy_pointsto_val (VEC_index (pointsto_val, tmpinfo, in_index));
    if(list == maylist)
        in = (in_index < 0) ? NULL :  copy_pointsto_val (VEC_index (pointsto_val, mayinfo, in_index));

    for (ci=in; ci && ci->lhs<=x; ci=ci->next)
    {
        if (element_equal (ci->lhs, x))
        {
            return true;
        }
    }

    return false;
}



static bool
has_undef_pointee(int index, vec_list list , unsigned int lhs)
{
    /*printf("has undef pointee");*/
    bitmap pointee_set = get_pointee_set(index,list,lhs);
    if(bitmap_first_set_bit (pointee_set) == undef_id)
        return true;

    return false;
}


static bool
has_only_undef_pointee(int index, vec_list list , unsigned int lhs)
{

    /*printf("\nget_undef_pointee_set\n");*/


    bitmap pointee_set = get_pointee_set(index,list,lhs);
    if(bitmap_single_bit_set_p(pointee_set))
    {
        if(bitmap_first_set_bit (pointee_set) == undef_id)
            return true;

    }

    return false;
}



static int
add_pointee_set(int index, vec_list list, unsigned int var, bitmap pointee_set)
{


    pointsto_val in = NULL,bi;
    bool is_last = false, is_equal = false;

    if(list == maylist)
    {
        in = (index < 0) ? NULL :
             copy_pointsto_val (VEC_index (pointsto_val, mayinfo, index));

    }
    else if(list == tmplist)
    {
        in = (index < 0) ? NULL :
             copy_pointsto_val (VEC_index (pointsto_val, tmpinfo, index));
    }

    /*printf("\nadd_pointee_set\n");*/

    if (in == NULL)
    {
        pointsto_val tmp = (pointsto_val) alloc_mem (sizeof (struct pointsto_val_def));
        tmp->lhs = var;
        tmp->rhs = copy_pointsto_rhs (pointee_set);
        in = tmp;
    }
    else
    {
        for (bi=in; bi; bi=bi->next)
        {
            if (element_equal (bi->lhs, var))
            {
                is_equal = true;
                break;
            }
            if (bi->lhs > var)
                break;
            if (bi->next == NULL)
            {
                is_last = true;
                break;
            }
        }
        if (is_last)
        {
            pointsto_val temp = (pointsto_val) alloc_mem (sizeof (struct pointsto_val_def));
            temp->lhs = var;
            temp->rhs = copy_pointsto_rhs (pointee_set);
            /*if(dump_file) fprintf(dump_file,"IDHAR");*/
            bi->next = temp;
            is_last = false;
        }
        /* If the LHS of bi and ai are the same, then append the RHS of ai to the RHS of bi. */
        else if (is_equal)
        {
            bitmap_iterator ci;
            unsigned int cival;
            EXECUTE_IF_SET_IN_BITMAP(pointee_set,0,cival,ci)
            {
                if(!bitmap_bit_p (bi->rhs, cival))
                    bitmap_set_bit(bi->rhs,cival);
            }
            bi->rhs = normalize_rhs_val (bi->rhs);
        }
        else
        {
            pointsto_val temp = (pointsto_val) alloc_mem (sizeof (struct pointsto_val_def));
            temp->next = bi->next;
            bi->next = temp;
            temp->lhs = bi->lhs;
            temp->rhs = bi->rhs;
            bi->lhs = var;
            bi->rhs = copy_pointsto_rhs (pointee_set);
        }

    }
    if(list == maylist)
    {
        index = index_in_maypool(in);
        if(index < 0)
        {
            index = VEC_length (pointsto_val, mayinfo);
            VEC_safe_push (pointsto_val, heap, mayinfo, in);
        }
    }
    else
    {
        index = index_in_tmppool(in);
        if(index < 0)
        {
            index = VEC_length (pointsto_val, tmpinfo);
            VEC_safe_push (pointsto_val, heap, tmpinfo, in);
        }
    }

    return index;


}

static bool
is_pointee_list_empty(int index)
{

    /*printf("\nis_pointee_set_empty\n");*/

    pointsto_val in = (index < 0) ? NULL : VEC_index (pointsto_val, tmpinfo, index);
    if(in == NULL)
        return true;
    else
        return false;

}


static int
remove_pointee_set(int in_index, vec_list list , unsigned int var)
{


    /*printf("\nremove_pointee_set\n");*/

    pointsto_val in = NULL, ai, prev = NULL;
    bool var_found=false;
    if(list == tmplist)
        in = (in_index < 0) ? NULL :  copy_pointsto_val (VEC_index (pointsto_val, tmpinfo, in_index));
    if(list == maylist)
        in = (in_index < 0) ? NULL :  copy_pointsto_val (VEC_index (pointsto_val, mayinfo, in_index));

    ai = in;
    while(ai)
    {
        if(element_equal(var,ai->lhs))
        {
            if(prev==NULL)
            {
                in=in->next;
                ggc_free(ai);
            }
            else
            {
                pointsto_val temp = ai;
                prev->next=ai->next;
                ggc_free(temp);
            }
            var_found = true;
            break;
        }

        else if(ai->lhs > var)
            break;

        else
        {
            prev = ai;
            ai = ai->next;
        }
    }

    if(var_found)
    {
        if(in)
        {
            if(list == maylist)
            {
                in_index = index_in_maypool(in);
                if(in_index < 0)
                {
                    in_index = VEC_length (pointsto_val, mayinfo);
                    VEC_safe_push (pointsto_val, heap, mayinfo, in);
                }
            }
            else
            {
                in_index = index_in_tmppool(in);
                if(in_index < 0)
                {
                    in_index = VEC_length (pointsto_val, tmpinfo);
                    VEC_safe_push (pointsto_val, heap, tmpinfo, in);
                }
            }
        }
        else
            return -2;
    }

    return in_index;
}


static bitmap
get_bitmap_for_lhs(int index , vec_list list)
{
    /*printf("\ngeT_bitmap_lhs\n");*/

    pointsto_val ai = NULL;
    bitmap var_map = BITMAP_ALLOC(NULL);
    if(list == maylist)	ai = (index < 0) ? NULL : copy_pointsto_val (VEC_index (pointsto_val, mayinfo, index));
    if(list == tmplist)  ai = (index < 0) ? NULL : copy_pointsto_val (VEC_index (pointsto_val, tmpinfo, index));

    while(ai)
    {
        bitmap_set_bit(var_map,ai->lhs);
        ai = ai->next;
    }

    return var_map;
}




static int
remove_pointee(int index,vec_list list,unsigned lhs,bitmap tmp_set)
{

    /* printf("\nremove_pointee\n");*/

    pointsto_val in = NULL,ai;
    if(list == maylist) in = (index < 0) ? NULL : copy_pointsto_val(VEC_index (pointsto_val, mayinfo, index));
    if(list == tmplist) in = (index < 0) ? NULL : copy_pointsto_val(VEC_index (pointsto_val, tmpinfo, index));
    ai = in;
    if(!is_x_in_pointsto_set(index,list,lhs))
    {
        return index;
    }
    while(ai && ai->lhs <=lhs)
    {

        if(ai->lhs == lhs)
        {
            bitmap_iterator bir;
            unsigned int bit;
            EXECUTE_IF_SET_IN_BITMAP(tmp_set,0,bit,bir)
            {
                bitmap_clear_bit(ai->rhs,bit);
            }
            break;
        }

        ai = ai->next;
    }
    if(list == tmplist)
    {
        index = index_in_tmppool(in);
        if(index < 0)
        {
            index = VEC_length (pointsto_val, tmpinfo);
            VEC_safe_push (pointsto_val, heap, tmpinfo, in);
        }
    }
    else
    {
        index = index_in_maypool(in);
        if(index < 0)
        {
            index = VEC_length (pointsto_val, mayinfo);
            VEC_safe_push (pointsto_val, heap, mayinfo, in);
        }

    }

    return index;
}


static int
add_pointee(int index,vec_list list,unsigned lhs,unsigned int rhs)
{

    /* printf("\nadd_pointee_set\n");*/

    pointsto_val in = NULL,ai;
    if(list == maylist) in = (index < 0) ? NULL : copy_pointsto_val(VEC_index (pointsto_val, mayinfo, index));
    if(list == tmplist) in = (index < 0) ? NULL : copy_pointsto_val(VEC_index (pointsto_val, tmpinfo, index));
    ai = in;
    if(is_x_in_pointsto_set(index,list,rhs))
    {
        return index;
    }
    while(ai && ai->lhs <=lhs)
    {

        if(ai->lhs == lhs)
        {
            bitmap_set_bit(ai->rhs,rhs);
            break;

        }
        ai = ai->next;
    }
    if(list == tmplist)
    {
        index = index_in_tmppool(in);
        if(index < 0)
        {
            index = VEC_length (pointsto_val, tmpinfo);
            VEC_safe_push (pointsto_val, heap, tmpinfo, in);
        }
    }
    else
    {
        index = index_in_maypool(in);
        if(index < 0)
        {
            index = VEC_length (pointsto_val, mayinfo);
            VEC_safe_push (pointsto_val, heap, mayinfo, in);
        }

    }

    return index;
}


/** APIs with basic block and callstring **/

static bool
has_undef_pointee_bb_cs (basic_block bb, int callstring, unsigned int lhs, in_out io)
{

    /*printf("\nhas_undef_pointee_set\n");*/

    csdfa_info bi = NULL;
    bi = get_csdfa_info_bb(bb,callstring,io);
    if(bi)
        return has_undef_pointee(bi->pinfo,maylist,lhs);

    return false;
}

static bitmap
get_pointee_set_bb_cs (basic_block bb, int callstring, unsigned int lhs, in_out io)
{
    csdfa_info bi = NULL;
    bitmap ret = BITMAP_ALLOC(NULL);
    bi = get_csdfa_info_bb(bb,callstring,io);
    if(bi)
        ret = get_pointee_set(bi->pinfo,maylist,lhs);
    return ret;
}


static void
add_pointee_set_bb_cs (basic_block bb, int callstring, unsigned int lhs, bitmap pointee_set, in_out io)
{

    csdfa_info ai=NULL;
    ai = get_csdfa_info_bb(bb,callstring,io);
    if(ai)
        ai->pinfo = add_pointee_set(ai->pinfo,maylist,lhs,pointee_set);
}

static void
remove_pointee_set_bb_cs (basic_block bb, int callstring, unsigned int lhs, in_out io)
{

    csdfa_info ai=NULL;
    ai = get_csdfa_info_bb(bb,callstring,io);
    if(ai)
        ai->pinfo = remove_pointee_set(ai->pinfo,maylist,lhs);
}


/** APIs with basic block **/

static bitmap
get_pointee_set_bb (basic_block bb, unsigned int lhs, in_out io)
{
    csdfa_info ai = NULL, bi;
    bitmap pointee_set = BITMAP_ALLOC(NULL);
    bitmap tmp_set =  BITMAP_ALLOC(NULL);
    bitmap_iterator bir;
    unsigned int val;
    /*if(io == IN) ai = in_of_bb(bb);
        if(io == OUT) ai = out_of_bb(bb);*/
    ai = get_csdfa_info_bb(bb,-2,io);
    for(bi = ai; bi; bi = bi->next)
    {
        tmp_set = get_pointee_set_bb_cs (bb,bi->cs_index,lhs,io);
        EXECUTE_IF_SET_IN_BITMAP(tmp_set,0,val,bir)
        {
            bitmap_set_bit(pointee_set,val);
        }
    }
    return tmp_set;
}


static void
add_pointee_set_bb (basic_block bb, unsigned int lhs, bitmap pointee_set, in_out io)
{
    csdfa_info ai = NULL, bi;
    /*if(io == IN) ai = in_of_bb(bb);
    if(io == OUT) ai = out_of_bb(bb);*/
    ai = get_csdfa_info_bb(bb,-2,io);
    for(bi = ai; bi; bi = bi->next)
    {
        add_pointee_set_bb_cs (bb,bi->cs_index,lhs,pointee_set,io);
    }
}




static void
remove_pointee_set_bb (basic_block bb, unsigned int lhs, in_out io)
{
    csdfa_info ai = NULL, bi;
    /*if(io == IN) ai = in_of_bb(bb);
    if(io == OUT) ai = out_of_bb(bb);*/
    for(bi = ai; bi; bi = bi->next)
    {
        remove_pointee_set_bb_cs (bb,bi->cs_index,lhs,io);
    }
}


static bool
has_must_pointee_bb_cs(basic_block bb,int callstring, unsigned int lhs, in_out io)
{
    csdfa_info ai=NULL;
    ai = get_csdfa_info_bb(bb,callstring,io);
    if(ai)
        return has_must_pointee(ai->pinfo,maylist,lhs);

    return false;

}

static bool
has_must_pointee_bb (basic_block bb,unsigned int lhs, in_out io)
{
    csdfa_info ai = NULL, bi;
    bool must_lhs = false;
    /*if(io == IN) ai = in_of_bb(bb);
    if(io == OUT) ai = out_of_bb(bb);*/
    ai = get_csdfa_info_bb(bb,-2,io);
    for(bi = ai; bi; bi = bi->next)
    {
        must_lhs = must_lhs & has_must_pointee_bb_cs (bb,bi->cs_index,lhs,io);
    }

    return must_lhs;
}


static int
merge_pointsto_info(int in1, int in2, vec_list list)
{

    int index;
    pointsto_val update, in, ai, bi, last_lhs;

    if (in1 < 0)
    {
        if (in2 < 0)
            return -1;
        else
            return in2;
    }
    /*if (in_index == -1) {
        if (update_index == -2)
            return -1;
        else
            return update_index;
    }*/
    if (in1 == in2)
        return in1;
    if(list == maylist)
    {
        in = (in1 < 0) ? NULL : copy_pointsto_val (VEC_index (pointsto_val, mayinfo, in1));
        update = (in2 < 0) ? NULL : VEC_index (pointsto_val, mayinfo, in2);
    }
    else if(list == tmplist)
    {
        in = (in1 < 0) ? NULL : copy_pointsto_val (VEC_index (pointsto_val, tmpinfo, in1));
        update = (in2 < 0) ? NULL : VEC_index (pointsto_val, tmpinfo, in2);
    }

    ai = in;
    bi = update;
    last_lhs = NULL;

    /* Compute the confluence of LHS. */
    while (ai)
    {
        if (bi == NULL)
            break;
        if (element_equal (ai->lhs, bi->lhs))
        {
            bitmap_iterator ci;
            unsigned int cival;
            EXECUTE_IF_SET_IN_BITMAP(bi->rhs,0,cival,ci)
            {
                bitmap_set_bit(ai->rhs,cival);
            }

            ai->rhs = normalize_rhs_val (ai->rhs);
            last_lhs = ai;
            ai = ai->next;
            bi = bi->next;
            continue;
        }
        if (ai->lhs < bi->lhs)
        {
            ai->rhs = prepend_undef_to_rhs (ai->rhs);
            last_lhs = ai;
            ai = ai->next;
            continue;
        }
        if (ai->lhs > bi->lhs)
        {
            pointsto_val temp = (pointsto_val) alloc_mem (sizeof (struct pointsto_val_def));
            temp->next = ai->next;
            ai->next = temp;
            temp->lhs = ai->lhs;
            temp->rhs = ai->rhs;
            ai->lhs = bi->lhs;
            ai->rhs = prepend_undef_to_rhs (copy_pointsto_rhs (bi->rhs));
            ai = ai->next;
            last_lhs = ai;
            bi = bi->next;
            continue;
        }
    }
    if (bi)
    {
        pointsto_val di;
        if (!last_lhs)
        {
            pointsto_val temp = (pointsto_val) alloc_mem (sizeof (struct pointsto_val_def));
            temp->lhs = bi->lhs;
            temp->rhs = prepend_undef_to_rhs (copy_pointsto_rhs (bi->rhs));
            in = last_lhs = temp;
            bi = bi->next;
        }
        for (di=bi; di; di=di->next)
        {
            pointsto_val temp = (pointsto_val) alloc_mem (sizeof (struct pointsto_val_def));
            temp->lhs = di->lhs;
            temp->rhs = prepend_undef_to_rhs (copy_pointsto_rhs (di->rhs));
            last_lhs->next = temp;
            last_lhs = temp;
        }
    }
    if (ai)
    {
        pointsto_val di;
        for (di=ai; di; di=di->next)
            di->rhs = prepend_undef_to_rhs (di->rhs);
    }

    /* Find the index of mayin in maypool. */
    if(list == maylist)
    {
        index = compute_index_in_maypool(in);
    }
    else if(list == tmplist)
    {
        index = compute_index_in_tmppool(in);


    }

    return index;
}

/*** APIs for live information *****/



static int
init_live_set (int val)
{

    /* printf("\ninit_live_set\n");*/

    df_list ret = (df_list) alloc_mem (sizeof (struct df_container));
    ret->val = val;
    return compute_liveness_index(ret);
}


static df_list
init_live_set_1 (int val)
{

    /*printf("\ninit_live_set 1\n");*/

    df_list ret = (df_list) alloc_mem (sizeof (struct df_container));
    ret->val = val;
    return ret;
}

static int
remove_var_from_live_set(int index, unsigned int var)
{
    /* printf("\nreemove var from live set\n");*/

    df_list in =  (index < 0) ? NULL : copy_df_list (VEC_index (df_list, liveinfo, index));
    df_list ai = in, prev_ai = NULL;
    while (ai)
    {
        if (var == ai->val)
        {
            if (prev_ai == NULL)
            {
                in = in->next;
                ggc_free(ai);
            }
            else
            {
                prev_ai->next = ai->next;
                ggc_free(ai);
            }
            break;
        }
        else if (var < ai->val)
            break;
        else if (ai->val < var)
        {
            prev_ai = ai;
            ai = ai->next;
        }
    }

    return compute_liveness_index(in);
}


static bitmap
get_live_set(int index)
{

    /*printf("\nget_live_set\n");*/

    df_list in =  (index < 0) ? NULL : copy_df_list (VEC_index (df_list, liveinfo, index));
    bitmap live = BITMAP_ALLOC(NULL);
    while(in)
    {
        bitmap_set_bit(live,in->val);
        in = in->next;
    }
    return live;
}

static int
get_live_set_size(int index)
{
    df_list in =  (index < 0) ? NULL : copy_df_list (VEC_index (df_list, liveinfo, index));
    int count = 0;
    while(in)
    {
        count = count +1 ;
        in = in->next;
    }
    return count;
}

static unsigned int
get_var_at_index_in_liveset(int lin, int index)
{
    df_list in =  (lin < 0) ? NULL : copy_df_list (VEC_index (df_list, liveinfo, lin));
    int count = 0;
    while(in)
    {
        if(index == count)
        {
            return in->val;
        }
        count = count + 1;
        in = in -> next;
    }
    return -1;
}


static int
add_var_to_liveset (int lin, unsigned int var)
{
    /*printf("\nadd_var_to_liveset\n");*/

    df_list in = (lin < 0) ? NULL : copy_df_list (VEC_index (df_list, liveinfo, lin));
    df_list ai = in,prev_ai = NULL;
    bool added = false;
    while (ai)
    {
        /*if (ai == NULL)
            break;*/
        if (ai->val == var)
        {
            added = true;
            break;
        }
        else if (ai->val > var)
        {
            df_list tmp = (df_list) alloc_mem (sizeof (struct df_container));
            tmp->val = var;
            tmp->next = ai;
            if (prev_ai)
            {
                prev_ai->next = tmp;
                prev_ai = tmp;
            }
            else
            {
                prev_ai = in = tmp;
            }
            added = true;
            break;
        }
        else if (ai->val < var)
        {
            prev_ai = ai;
            ai = ai->next;
        }
    }

    if(!added)
    {
        df_list tmp = (df_list) alloc_mem (sizeof (struct df_container));
        tmp->val = var;
        tmp->next = NULL;
        prev_ai->next = tmp;

    }

    return compute_liveness_index(in);

}



static bool
add_var_to_liveset_bb_cs (basic_block bb , int cs_index , unsigned int var,  in_out io)
{
    csdfa_info ai = NULL,bi;
    ai = get_csdfa_info_bb(bb,cs_index,io);
    ai->linfo = add_var_to_liveset(ai->linfo,var);

}


static bool
add_var_to_liveset_bb (basic_block bb, unsigned int var, in_out io)
{
    csdfa_info ai = NULL, bi;
    for (bi = ai; bi; bi = bi->next)
    {
        add_var_to_liveset_bb_cs(bb, bi->cs_index,var,io);
    }

}

static bool
is_var_live(int lin, unsigned int var)
{

    /* printf("\nis_var_live\n");*/

    df_list in = (lin < 0) ? NULL : copy_df_list (VEC_index (df_list, liveinfo, lin));
    while(in && in->val <= var)
    {
        if(in->val == var)
            return true;
        in = in->next;

    }

    return false;

}


static bool
is_var_live_bb_cs(basic_block bb, int callstring, unsigned int var, in_out io)
{

    csdfa_info ai = get_csdfa_info_bb(bb,callstring,io);

    df_list in = (ai->linfo < 0) ? NULL : copy_df_list (VEC_index (df_list, liveinfo, ai->linfo));
    while(in && in->val <= var)
    {
        if(in->val == var)
            return true;
        in = in->next;

    }

    return false;

}

static bool
is_var_live_bb(basic_block bb, unsigned int var, in_out io)
{

    csdfa_info ai = NULL, bi;
    bool found;
    df_list in;
    /*if(io == IN) ai = in_of_bb(bb);
    if(io == OUT) ai = out_of_bb(bb);*/
    ai = get_csdfa_info_bb(bb,-2,io);

    for(bi = ai; bi; bi=bi->next)
    {
        found = false;
        in = (bi->linfo < 0) ? NULL : copy_df_list (VEC_index (df_list, liveinfo, bi->linfo));
        while(in && in->val <= var)
        {
            if(in->val == var)
                found = true;
            in = in->next;
        }
        if(!found)
            return false;
    }


    if(found)
        return true;

    return false;

}


static int
merge_liveness_info(int in1, int in2)
{

    df_list ai , bi, prev_ai = NULL,in ,update;

    if (in1 < 0 && in2 < 0)
        return -1;

    if (in1 < 0)
        return in2;

    if (in2 < 0 || in1 == in2)
        return in1;



    in = (in1 < 0) ? NULL : copy_df_list (VEC_index (df_list, liveinfo, in1));
    update = (in2 < 0) ? NULL : copy_df_list (VEC_index (df_list, liveinfo, in2));

    ai = in;
    bi = update;

    while (bi)
    {
        if (ai == NULL)
            break;
        if (ai->val == bi->val)
        {
            prev_ai = ai;
            ai = ai->next;
            bi = bi->next;
            continue;
        }
        else if (ai->val > bi->val)
        {
            df_list tmp = (df_list) alloc_mem (sizeof (struct df_container));
            tmp -> val = bi->val;
            tmp->next = ai;
            if (prev_ai)
            {
                prev_ai->next = tmp;
                prev_ai = tmp;
            }
            else
            {
                prev_ai = in = tmp;
            }
            bi = bi->next;
            continue;
        }
        else if (ai->val < bi->val)
        {
            prev_ai = ai;
            ai = ai->next;
        }
    }
    if (bi)
    {
        df_list ci;
        gcc_assert (prev_ai);
        for (ci = bi; ci; ci=ci->next)
        {
            df_list tmp = (df_list) alloc_mem (sizeof (struct df_container));
            tmp->val = ci->val;
            prev_ai->next = tmp;
            prev_ai = prev_ai->next;
        }
    }

    return compute_liveness_index(in);

}

