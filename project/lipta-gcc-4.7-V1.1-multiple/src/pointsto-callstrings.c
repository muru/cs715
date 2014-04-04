#include "gcc-plugin.h"
#include "pointsto-callstrings.h"
#include "gimple-pretty-print.h"
/*-----------------------------------------------------------------------------
 *  Each plugin MUST define this global int to assert compatibility with GPL; 
 *  else the compiler throws a fatal error 
 *-----------------------------------------------------------------------------*/
int plugin_is_GPL_compatible;  

/*----------------------------------------------------------------------
  The base implementation. The method implements points-to analysis
  using callstrings method. All the functions that have _cs_ 
  prepended to their names have been lifted from tree-ssa-structalias.c
  ---------------------------------------------------------------------*/

static unsigned int print_liveness(void);
static void process_function (void);


static unsigned int print_liveness(void) 
{
	struct cgraph_node *node;
	for (node = cgraph_nodes; node; node = node->next)
	{
		if (gimple_has_body_p(node->decl))
		{
			push_cfun (DECL_STRUCT_FUNCTION (node->decl));
			process_function ();
			pop_cfun ();
		}
	}

	dump_pta_stats (dump_file);

	return 0;
}

/*static void process_function (void)
  {
  basic_block bb;
  gimple_stmt_iterator si;
  int a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  int *k = 0;
  int j = 0;
  k     = &a[j];

  FOR_EACH_BB (bb)
  {
  for (si = gsi_start_phis (bb); !gsi_end_p (si); gsi_next (&si))
  {
  gimple phi = gsi_stmt (si);
  int n      = gimple_phi_num_args (phi);
  use_operand_p use_p;
  ssa_op_iter iter;

  k = &a[++j];

  fprintf(dump_file, "%d %d:\t", bb->index, *k);
  print_gimple_stmt(dump_file, phi, 0, TDF_SLIM);
  dump_immediate_uses_for (dump_file, gimple_phi_result (phi));

  FOR_EACH_PHI_ARG (use_p, phi, iter, SSA_OP_ALL_DEFS)
  {	
  struct ptr_info_def * def = SSA_NAME_PTR_INFO (use_p->loc.ssa_name);
  if (def->pt.anything)
  fprintf(stderr, "ANYTHING\t");
  if (def->pt.nonlocal)
  fprintf(stderr, "NONLOCAL\t");
  if (def->pt.escaped)
  fprintf(stderr, "ESCAPED\t");
  if (def->pt.ipa_escaped)
  fprintf(stderr, "IPA_ESCAPED\t");
  if (def->pt.null)
  fprintf(stderr, "NULL\t");
  fprintf(stderr, "\n");
  dump_points_to_solution (dump_file, &(def->pt));
  }
  }
  }
  return;
  }*/


static void process_function (void)
{
	fprintf (dump_file,"num_ssa_name=%d\n",num_ssa_names);

	fprintf (dump_file, "\n\npoints-to information\n\n");
	int i;
	for (i = 0; i < num_ssa_names; i++)
	{
		tree ptr = ssa_name (i);
		struct ptr_info_def *pi;
		//fprintf(dump_file,"num_ssa_name=%d\n",num_ssa_name); 
		if (ptr == NULL_TREE
			/*	|| SSA_NAME_IN_FREE_LIST (ptr)*/)
		{
			fprintf(dump_file, "Tree is NULL.\n");
			continue;
		}

		pi = SSA_NAME_PTR_INFO (ptr);
		if (pi)
			dump_points_to_info_for (dump_file, ptr);
		else
			fprintf(dump_file, "Pointor info NULL.\n");

	}

	fprintf (dump_file, "-----------------------------------------------------\n\n");

	basic_block bb;
	gimple_stmt_iterator si;
	FOR_EACH_BB (bb)
	{
		for (si = gsi_start_phis (bb); !gsi_end_p (si); gsi_next (&si))
		{    
			gimple phi = gsi_stmt(si);
			int n =	gimple_phi_num_args (phi);
			use_operand_p use_p;
			ssa_op_iter iter;
			print_gimple_stmt(dump_file, phi, 0, TDF_SLIM);
			dump_immediate_uses_for (dump_file, gimple_phi_result (phi));
			FOR_EACH_PHI_ARG (use_p, phi, iter, SSA_OP_ALL_DEFS)
			{	
				struct ptr_info_def * def = SSA_NAME_PTR_INFO (use_p->loc.ssa_name);
				if (def->pt.anything)
					fprintf(stderr, "ANYTHING\t");
				if (def->pt.nonlocal)
					fprintf(stderr, "NONLOCAL\t");
				if (def->pt.escaped)
					fprintf(stderr, "ESCAPED\t");
				if (def->pt.ipa_escaped)
					fprintf(stderr, "IPA_ESCAPED\t");
				if (def->pt.null)
					fprintf(stderr, "NULL\t");
				fprintf(stderr, "\n");
				dump_points_to_solution (dump_file, &(def->pt));
			}
		}
	}
	return;
}



/*******************************************************************************/

/* Return the varmap element N */
static inline csvarinfo_t
cs_get_varinfo (unsigned int n)
{
   return VEC_index (csvarinfo_t, csvarmap, n);
}

/* Insert ID as the variable id for tree T in the vi_for_tree map.  */
static void
cs_insert_vi_for_tree (tree t, csvarinfo_t vi)
{
   void **slot = pointer_map_insert (vi_for_tree, t);
   gcc_assert (vi);
   gcc_assert (*slot == NULL);
   *slot = vi;
}

static bool
is_proper_var (unsigned int varid)
{
   return (varid > 2);
}

static bool
parm_decl (unsigned int varid)
{
   return (TREE_CODE (SSAVAR (cs_get_varinfo (varid)->decl)) 
	  == PARM_DECL);
}

static struct cgraph_node *
scoping_fn (unsigned int varid)
{
   return cs_get_varinfo (varid)->scoping_function;
}

static bool
var_defined_in_cfun (unsigned int varid)
{
   return (cnode == scoping_fn (varid));
}

static bool
global_var (unsigned int varid)
{
   return (cs_get_varinfo (varid)->is_global_var);
}

/* Return the pcsmap of the cgraph node. */
static cs_map
pcsmap_of_cnode (struct cgraph_node *node)
{
   return ((cgraph_info)(node->aux))->pcsmap;
}

static cs_map
lcsmap_of_cnode (struct cgraph_node *node)
{
   return ((cgraph_info)(node->aux))->lcsmap;
}

static bool
pcsmap_required (struct cgraph_node *node)
{
   return ((cgraph_info)(node->aux))->pcsmap_required;
}

static bool
lcsmap_required (struct cgraph_node *node)
{
   return ((cgraph_info)(node->aux))->lcsmap_required;
}

static basic_block 
get_bb_from_callstring (int cs)
{
   callstring ai = VEC_index (callstring, callstrings, cs);
   return ai->bb;
}

/* Return the basic block worlist of the given cgraph node. */
static bb_worklist
worklist_of_cnode (struct cgraph_node *node)
{
   return ((cgraph_info)(node->aux))->bb_list;
}

/* Returns the end basic block of the function (The end block, i.e the block
   before EXIT_BLOCK_PTR). */
static inline basic_block
end_bb_of_fn (struct cgraph_node *node)
{
   bb_worklist bb_list = worklist_of_cnode (node);
   int i = n_basic_blocks_for_function (DECL_STRUCT_FUNCTION (node->decl)) - NUM_FIXED_BLOCKS;
   return bb_list[i].bb;
}

static inline basic_block
start_bb_of_fn (struct cgraph_node *node)
{
   return ENTRY_BLOCK_PTR_FOR_FUNCTION (DECL_STRUCT_FUNCTION (node->decl))->next_bb;
}

/* Return a new worklist node. */
static worklist
new_worklist_node (struct cgraph_node *node)
{
   worklist temp = (worklist) alloc_mem (sizeof (struct worklist_queue));
   temp->called_fn = node;
   temp->process_lv = true;
   temp->process_pta = true;
   return temp;
}

/* Return the node to which the basic block belongs. */
static struct cgraph_node *
cnode_of_bb (basic_block bb)
{
   return ((bb_csipdfa_info)(bb->aux))->cnode;
}

/* Initialize the in liveness information. */
static csdfa_info
initialize_out (void)
{
   csdfa_info temp = (csdfa_info) alloc_mem (sizeof (struct cs_dfa));
   temp->linfo = -1;
   temp->pinfo = -2;
   temp->cs_index = -1;
   temp->new_lvdf = true;
   temp->new_ptadf = true;
   return temp;
}

/* Returns true if two pointsto elements are equal. */
static bool
element_equal (unsigned int lhs_var, unsigned int rhs_var)
{
   return (lhs_var == rhs_var);
}

/* Return true if all the successors of a node in worklist have been processed. */
static bool
lv_callee_nodes_processed (void)
{
   worklist tmp;
   for (tmp=lv_node->next; tmp; tmp=tmp->next) {
       if (tmp->process_lv)
           return false;
   }
   return true;
}

static bool
pta_callee_nodes_processed (void)
{
   worklist tmp;
   for (tmp=pta_node->next; tmp; tmp=tmp->next) {
       if (tmp->process_pta)
           return false;
   }
   return true;
}

/* Return a node from pta worklist whose successors have been processed. */
static struct cgraph_node *
prev_node_from_pta_worklist (void)
{
   worklist ai, bi = NULL;

   /* Reset the process_pta flag of pta_node. */
   pta_node->process_pta = false;
   for (ai=start; ai!=pta_node; ai=ai->next) {
       if (ai->process_pta) 
	   bi = ai;
   }
   pta_node = bi;
   if (pta_node)
       return pta_node->called_fn;
   return NULL;
}

/* Return a node from lv worklist whose successors have been processed. */
static struct cgraph_node *
prev_node_from_lv_worklist (void)
{
   worklist ai, bi = NULL;

   /* Reset the process_pta flag of pta_node. */
   lv_node->process_lv = false;
   for (ai=start; ai!=lv_node; ai=ai->next) {
       if (ai->process_lv)
	   bi = ai;
   }
   lv_node = bi;
   if (lv_node)
       return lv_node->called_fn;
   return NULL;
}

/* Get a node from lv worklist. */
static struct cgraph_node *
first_node_from_lv_worklist (void)
{
   worklist tmp;
   for (tmp=start; tmp; tmp=tmp->next) {
       if (tmp->process_lv) {
           lv_node = tmp;
           return tmp->called_fn;
       }
   }
   return NULL;
}

/* Get a node from pta worklist. */
static struct cgraph_node *
first_node_from_pta_worklist (void)
{
   worklist tmp;
   for (tmp=start; tmp; tmp=tmp->next) {
       if (tmp->process_pta) {
           pta_node = tmp;
           return tmp->called_fn;
       }
   }
   return NULL;
}

/* Get a node from lv worklist. */
static struct cgraph_node *
next_node_from_lv_worklist (worklist node)
{
   worklist tmp;
   for (tmp=node->next; tmp; tmp=tmp->next) {
       if (tmp->process_lv) {
	   lv_node = tmp;
	   return tmp->called_fn;
       }
   }
   return NULL;
}

/* Get a node from pta worklist. */
static struct cgraph_node *
next_node_from_pta_worklist (worklist node)
{
   worklist tmp;
   for (tmp=node->next; tmp; tmp=tmp->next) {
       if (tmp->process_pta) {
           pta_node = tmp;
           return tmp->called_fn;
       }
   }
   return NULL;
}

/* Return the in_ipdfa of the basic block. */
static csdfa_info
in_of_bb (basic_block bb)
{
   return ((bb_csipdfa_info)(bb->aux))->in_ipdfa;
}

/* Return the out_ipdfa of the basic block. */
static csdfa_info
out_of_bb (basic_block bb)
{
   return ((bb_csipdfa_info)(bb->aux))->out_ipdfa;
}

static bool
unexpandable_var (unsigned int var, HOST_WIDE_INT offset)
{
   return (offset == 0 ||
           !is_proper_var (var) ||
           offset == UNKNOWN_OFFSET ||
           cs_get_varinfo (var)->is_heap_var);
}

/* Return the constraint list of the basic block. */
static constraint_list
get_constraint_start (basic_block bb)
{
   return ((bb_csipdfa_info)(bb->aux))->constraint_start;
}

static constraint_list
get_constraint_end (basic_block bb)
{
   return ((bb_csipdfa_info)(bb->aux))->constraint_end;
}





static void
print_pointsto_list (int index)
{

   bitmap_iterator bi;
   unsigned int rhsbit;
   int cnt ,i=1;
   if(index < 0)
       fprintf (dump_file, " null");
   else {
        /*int size = size_of_pointsto_set(index,maylist);*/
        bitmap lhs_bit = get_bitmap_for_lhs(index,maylist);
	bitmap_iterator bir;
	unsigned int var;
      EXECUTE_IF_SET_IN_BITMAP(lhs_bit,0,var,bir)
      {
	        /*unsigned int var = get_var_at_index_in_pointsto_set(index,maylist,j);
	*/	bitmap pointee = get_pointee_set(index,maylist,var);
		i = 1;cnt =0;

	           cnt = bitmap_count_bits(pointee);
	           fprintf (dump_file, "  %s", cs_get_varinfo (var)->name);
	           fprintf (dump_file, "->{");
	           EXECUTE_IF_SET_IN_BITMAP(pointee, 0, rhsbit, bi)
	           {
	               fprintf (dump_file, "%s", cs_get_varinfo (rhsbit)->name);
	                if(i++ < cnt)
	                    fprintf (dump_file, ",");
	           }
	          if(!cnt){
	                fprintf (dump_file, " null");
		  }
	          fprintf(dump_file,"}");
		         
       }
    }
/*   if (info == NULL)
       fprintf (dump_file, " null");
   else {
       for (ai=info; ai; ai=ai->next) {
           i = 1;cnt =0;

           cnt = bitmap_count_bits(ai->rhs);
           fprintf (dump_file, "  %s", cs_get_varinfo (ai->lhs)->name);
           fprintf (dump_file, "->{");
           EXECUTE_IF_SET_IN_BITMAP(ai->rhs, 0, rhsbit, bi)
           {
               fprintf (dump_file, "%s", cs_get_varinfo (rhsbit)->name);
                if(i++ < cnt)
                    fprintf (dump_file, ",");
           }
          if(!cnt){
                fprintf (dump_file, " null");
	  }
          fprintf(dump_file,"}");

      }
   }*/
}






static bitmap
normalize_rhs_val (bitmap in)
{
	if(bitmap_count_bits(in)>=3)
	{
		bitmap_iterator bi;
		unsigned int bit;
		EXECUTE_IF_SET_IN_BITMAP(in, 0, bit, bi)
		{
			if(bit == undef_id)
			{
				bitmap_clear_bit(in,undef_id);
				break;
			}
		}
	}

	return in;
}




/* If the may pointsto information already exists in the mayinfo pool,
   return its index. Else return -1. */

static bitmap
copy_pointsto_rhs(bitmap from)
{
        bitmap to = BITMAP_ALLOC(NULL);
        bitmap_copy(to,from);
        return to;
}




static int
compute_pointsto_index_from_templist (int index)
{

     int root = -1;

     bitmap lhs = get_bitmap_for_lhs(index,tmplist);
     bitmap_iterator bir;
     unsigned int var;
     EXECUTE_IF_SET_IN_BITMAP(lhs,0,var,bir){
                bitmap pointee = get_pointee_set(index,tmplist,var);
                root = add_pointee_set(root,maylist,var,pointee);

    }
    empty_temp_set();
    return root;
}

static int
copy_pointsto_set_to_temp(int index)
{
        

     int root = -1;
     	
     bitmap lhs = get_bitmap_for_lhs(index,maylist);
     bitmap_iterator bir;
     unsigned int var;
     EXECUTE_IF_SET_IN_BITMAP(lhs,0,var,bir){
                bitmap pointee = get_pointee_set(index,maylist,var);
                root = add_pointee_set(root,tmplist,var,pointee);
    
    }
    return root;


}


/* Return a copy of the given pointsto information. */
static csdfa_info
copy_csdfa_info (csdfa_info ci)
{
   csdfa_info temp = (csdfa_info) alloc_mem (sizeof (struct cs_dfa));
   temp->linfo = ci->linfo;
   temp->pinfo = ci->pinfo;
   temp->cs_index = ci->cs_index;
   temp->new_lvdf = ci->new_lvdf;
   temp->new_ptadf = ci->new_ptadf;
   return temp;
}

/* Given a gimple tree T, return the constraint expression vector for it
   to be used as the rhs of a constraint.  */
static void
cs_get_constraint_for_rhs (tree t, VEC (ce_s, heap) **results)
{
   gcc_assert (VEC_length (ce_s, *results) == 0);
   cs_get_constraint_for_1 (t, results, false, false);
}

/* Return a new variable info structure consisting for a variable
   named NAME, and using constraint graph node NODE.  Append it
   to the vector of variable info structures.  */
static csvarinfo_t
cs_new_var_info (tree t, const char *name)
{
   unsigned index = VEC_length (csvarinfo_t, csvarmap);
   csvarinfo_t ret = (csvarinfo_t) pool_alloc (csvarinfo_pool);

   ret->id = index;
   ret->name = name;
   ret->decl = t;
   ret->is_unknown_size_var = false;
   ret->is_full_var = (t == NULL_TREE);
   ret->is_heap_var = false;
   ret->may_have_pointers = true;
   ret->is_global_var = (t == NULL_TREE);
   /* Vars without decl are artificial and do not have sub-variables.  */
   if (t && DECL_P (t))
     ret->is_global_var = (is_global_var (t)
                          /* We have to treat even local register variables
                             as escape points.  */
                          || (TREE_CODE (t) == VAR_DECL
                              && DECL_HARD_REGISTER (t)));
   ret->constraints_with_vi_as_lhs = NULL;
   ret->scoping_function = (ret->is_global_var) ? NULL : cnode;
   ret->next = NULL;

   VEC_safe_push (csvarinfo_t, heap, csvarmap, ret);

   return ret;
}

/* Create a varinfo structure for NAME and DECL, and add it to VARMAP.
   This will also create any varinfo structures necessary for fields
   of DECL.  */
static csvarinfo_t
cs_create_variable_info_for_1 (tree decl, const char *name)
{
   csvarinfo_t vi, newvi;
   tree decl_type = TREE_TYPE (decl);
   tree declsize = DECL_P (decl) ? DECL_SIZE (decl) : TYPE_SIZE (decl_type);
   VEC (fieldoff_s,heap) *fieldstack = NULL;
   fieldoff_s *fo;
   unsigned int i;

   if (!declsize || !host_integerp (declsize, 1)) {
      vi = cs_new_var_info (decl, name);
      vi->offset = 0;
      vi->size = ~0;
      vi->fullsize = ~0;
      vi->is_unknown_size_var = true;
      vi->is_full_var = true;
      vi->may_have_pointers = true;
      return vi;
   }

   /* Collect field information.  */
   if (var_can_have_subvars (decl)
      /* ???  Force us to not use subfields for global initializers
	 in IPA mode.  Else we'd have to parse arbitrary initializers.  */
      && !(is_global_var (decl) && DECL_INITIAL (decl))) {
       fieldoff_s *fo = NULL;
       bool notokay = false;
       unsigned int i;

       push_fields_onto_fieldstack (decl_type, &fieldstack, 0);

       for (i = 0; !notokay && VEC_iterate (fieldoff_s, fieldstack, i, fo); i++)
	   if (fo->has_unknown_size || fo->offset < 0) {
	       notokay = true;
	       break;
	   }

          /* We can't sort them if we have a field with a variable sized type,
 	  which will make notokay = true.  In that case, we are going to return
	  without creating varinfos for the fields anyway, so sorting them is a
	  waste to boot.  */
       if (!notokay) {
	   sort_fieldstack (fieldstack);
	   /* Due to some C++ FE issues, like PR 22488, we might end up
	      what appear to be overlapping fields even though they,
	      in reality, do not overlap.  Until the C++ FE is fixed,
	      we will simply disable field-sensitivity for these cases.  */
	   notokay = check_for_overlaps (fieldstack);
       }

       if (notokay)
	   VEC_free (fieldoff_s, heap, fieldstack);
   }

   /* If we didn't end up collecting sub-variables create a full
      variable for the decl.  */
   if (VEC_length (fieldoff_s, fieldstack) <= 1
      || VEC_length (fieldoff_s, fieldstack) > MAX_FIELDS_FOR_FIELD_SENSITIVE) {
       vi = cs_new_var_info (decl, name);
       vi->offset = 0;
       vi->may_have_pointers = true;
       vi->fullsize = TREE_INT_CST_LOW (declsize);
       vi->size = vi->fullsize;
       vi->is_full_var = true;
       VEC_free (fieldoff_s, heap, fieldstack);
       return vi;
   }

   vi = cs_new_var_info (decl, name);
   vi->fullsize = TREE_INT_CST_LOW (declsize);
   for (i = 0, newvi = vi;
       VEC_iterate (fieldoff_s, fieldstack, i, fo);
       ++i, newvi = newvi->next) {
       const char *newname = "NULL";
       char *tempname;

       if (dump_file) {
	   asprintf (&tempname, "%s." HOST_WIDE_INT_PRINT_DEC
		    "+" HOST_WIDE_INT_PRINT_DEC, name, fo->offset, fo->size);
	   newname = ggc_strdup (tempname);
	   free (tempname);
       }
       newvi->name = newname;
       newvi->offset = fo->offset;
       newvi->size = fo->size;
       newvi->fullsize = vi->fullsize;
       newvi->may_have_pointers = fo->may_have_pointers;
       if (i + 1 < VEC_length (fieldoff_s, fieldstack))
	   newvi->next = cs_new_var_info (decl, name);
   }

   VEC_free (fieldoff_s, heap, fieldstack);

   return vi;
}

static unsigned int
cs_create_variable_info_for (tree decl, const char *name)
{
   csvarinfo_t vi = cs_create_variable_info_for_1 (decl, name);
   unsigned int id = vi->id;

   cs_insert_vi_for_tree (decl, vi);

   /* Create initial constraints for globals.  */
   for (; vi; vi = vi->next) {
       if (!vi->may_have_pointers || !vi->is_global_var)
	   continue;

       /* If this is a global variable with an initializer,
 	  generate constraints for it. */
       if (DECL_INITIAL (decl)) {
	   VEC (ce_s, heap) *rhsc = NULL;
	   struct constraint_expr lhs, *rhsp;
	   unsigned i;
	   cs_get_constraint_for_rhs (DECL_INITIAL (decl), &rhsc);
	   lhs.var = vi->id;
	   lhs.offset = 0;
	   lhs.type = SCALAR;
	   FOR_EACH_VEC_ELT (ce_s, rhsc, i, rhsp)
	       cs_process_constraint (new_constraint (lhs, *rhsp));
	   VEC_free (ce_s, heap, rhsc);
       }
    }

   return id;
}

/* Find the variable id for tree T in the map. If T doesn't 
  exist in the map, create an entry for it and return it. */
static csvarinfo_t
cs_get_vi_for_tree (tree stmt)
{
   tree t = SSAVAR (stmt);
   void **slot = pointer_map_contains (vi_for_tree, t);
   if (slot == NULL)
       return cs_get_varinfo (cs_create_variable_info_for (t, alias_get_name (t)));

   return (csvarinfo_t) *slot;
}

/* Find the variable info for tree T in VI_FOR_TREE. If T does not
   exist in the map, return NULL, otherwise, return the varinfo 
   we found.  */
static csvarinfo_t
cs_lookup_vi_for_tree (tree t)
{
   void **slot = pointer_map_contains (vi_for_tree, t);
   if (slot == NULL)
       return NULL;

   return (csvarinfo_t) *slot;
}

/* Get a scalar constraint expression for a new temporary variable.  */
static struct constraint_expr
cs_new_scalar_tmp_constraint_exp (const char *name)
{
   struct constraint_expr tmp;
   csvarinfo_t vi;

   vi = cs_new_var_info (NULL_TREE, name);
   vi->offset = 0;
   vi->size = -1;
   vi->fullsize = -1;
   vi->is_full_var = 1;

   tmp.var = vi->id;
   tmp.type = SCALAR;
   tmp.offset = 0;

   return tmp;
}

/* Create a new artificial heap variable with NAME.
   Return the created variable.  
static csvarinfo_t
cs_make_heapvar_for (csvarinfo_t lhs, const char *name)
{
   csvarinfo_t vi;
   const char *newname = "NULL";
   char *tempname;
   tree heapvar = heapvar_lookup (lhs->decl, lhs->offset);

   if (heapvar != NULL_TREE)
       return cs_lookup_vi_for_tree (heapvar);

   if (heapvar == NULL_TREE) {
       var_ann_t ann;
       heapvar = create_tmp_var_raw (ptr_type_node, name);
       DECL_EXTERNAL (heapvar) = 1;
       heapvar_insert (lhs->decl, lhs->offset, heapvar);
       ann = get_var_ann (heapvar);
       ann->is_heapvar = 1;
   }

   // For global vars we need to add a heapvar to the list of referenced
   //   vars of a different function than it was created for originally.  
   if (cfun && gimple_referenced_vars (cfun))
       add_referenced_var (heapvar);

   // Append 'heap' with the its index in csvarinfo. 
   asprintf (&tempname, "%s.%d", name, VEC_length (csvarinfo_t, csvarmap));
   newname = ggc_strdup (tempname);
   vi = cs_new_var_info (heapvar, newname);
   vi->is_unknown_size_var = true;
   vi->offset = 0;
   vi->fullsize = ~0;
   vi->size = ~0;
   vi->is_full_var = true;
   vi->is_heap_var = true;

   cs_insert_vi_for_tree (heapvar, vi);
   return vi;
} */

/* CHANGE DUE TO GCC-4.7.2
   function make_heapvar_for of gcc-4.6.* is modified to make_heapvar in gcc-4.7.2.
   This cs_make_heapvar_for is also modified */

/* Temporary storage for fake var decls.  */
struct obstack fake_var_decl_obstack;

/* Build a fake VAR_DECL acting as referrer to a DECL_UID.  */

static tree
build_fake_var_decl (tree type)
{
  tree decl = (tree) XOBNEW (&fake_var_decl_obstack, struct tree_var_decl);
  memset (decl, 0, sizeof (struct tree_var_decl));
  TREE_SET_CODE (decl, VAR_DECL);
  TREE_TYPE (decl) = type;
  DECL_UID (decl) = allocate_decl_uid ();
  SET_DECL_PT_UID (decl, -1);
  layout_decl (decl, 0);
  return decl;
}

/* Create a new artificial heap variable with NAME.
   Return the created variable.  */

static csvarinfo_t
cs_make_heapvar_for (csvarinfo_t lhs, const char *name)
{
  csvarinfo_t vi;
  tree heapvar;
  const char *newname = "NULL";
  char *tempname;

  heapvar = build_fake_var_decl (ptr_type_node);
  DECL_EXTERNAL (heapvar) = 1;

  /* Append 'heap' with the its index in csvarinfo. */
  asprintf (&tempname, "%s.%d", name, VEC_length (csvarinfo_t, csvarmap));
  newname = ggc_strdup (tempname);

  vi = cs_new_var_info (heapvar, newname);
  //vi->is_artificial_var = true;
  vi->is_heap_var = true;
  vi->is_unknown_size_var = true;
  vi->offset = 0;
  vi->fullsize = ~0;
  vi->size = ~0;
  vi->is_full_var = true;
  cs_insert_vi_for_tree (heapvar, vi);

  return vi;
}

/* Create a constraint ID = &FROM. */
static void
cs_make_constraint_from (csvarinfo_t vi, int from)
{
   struct constraint_expr lhs, rhs;

   lhs.var = vi->id;
   lhs.offset = 0;
   lhs.type = SCALAR;

   rhs.var = from;
   rhs.offset = 0;
   rhs.type = ADDRESSOF;
   cs_process_constraint (new_constraint (lhs, rhs));
}

/* Create a new artificial heap variable with NAME and make a
   constraint from it to LHS.  Return the created variable.  */
static csvarinfo_t
cs_make_constraint_from_heapvar (csvarinfo_t lhs, const char *name)
{
   csvarinfo_t vi = cs_make_heapvar_for (lhs, name);
   cs_make_constraint_from (lhs, vi->id);

   return vi;
}

/* Find the first varinfo in the same variable as START that overlaps with
   OFFSET.  If there is no such varinfo the varinfo directly preceding
   OFFSET is returned.  */
static csvarinfo_t				/* Look into */
cs_first_or_preceding_vi_for_offset (csvarinfo_t start,
				  unsigned HOST_WIDE_INT offset)
{
   /* If we cannot reach offset from start, lookup the first field
      and start from there.  */
   if (start->offset > offset)
       start = cs_lookup_vi_for_tree (start->decl);

   /* We may not find a variable in the field list with the actual
      offset when when we have glommed a structure to a variable.
      In that case, however, offset should still be within the size
      of the variable.
      If we got beyond the offset we look for return the field
      directly preceding offset which may be the last field.  */
   while (start->next && offset >= start->offset
	 && !((offset - start->offset) < start->size))
       start = start->next;
  
   return start;
}

/* Dereference the constraint expression CONS, and return the result.
   DEREF (ADDRESSOF) = SCALAR
   DEREF (SCALAR) = DEREF
   DEREF (DEREF) = (temp = DEREF1; result = DEREF(temp))
   This is needed so that we can handle dereferencing DEREF constraints.  */
static void
cs_do_deref (VEC (ce_s, heap) **constraints)
{
   struct constraint_expr *c;
   unsigned int i = 0;

   FOR_EACH_VEC_ELT (ce_s, *constraints, i, c) {
       if (c->type == SCALAR)
	   c->type = DEREF;
       else if (c->type == ADDRESSOF)
	   c->type = SCALAR;
       else if (c->type == DEREF) {
	   struct constraint_expr tmplhs;
	   tmplhs = cs_new_scalar_tmp_constraint_exp ("dereftmp");
	   cs_process_constraint (new_constraint (tmplhs, *c));
	   c->var = tmplhs.var;
       }
       else
	   gcc_unreachable ();
   }
}

/* Get constraint expressions for offsetting PTR by OFFSET.  Stores the
   resulting constraint expressions in *RESULTS.  */
static void
cs_get_constraint_for_ptr_offset (tree ptr, tree offset,
			       VEC (ce_s, heap) **results)
{
   struct constraint_expr c;
   unsigned int j, n;
   HOST_WIDE_INT rhsunitoffset, rhsoffset;

   if (offset == NULL_TREE || !host_integerp (offset, 0))
       rhsoffset = UNKNOWN_OFFSET;
   else {
       /* Make sure the bit-offset also fits.  */
       rhsunitoffset = TREE_INT_CST_LOW (offset);
       rhsoffset = rhsunitoffset * BITS_PER_UNIT;
       if (rhsunitoffset != rhsoffset / BITS_PER_UNIT)
	   rhsoffset = UNKNOWN_OFFSET;
   }

   cs_get_constraint_for_rhs (ptr, results);
   if (rhsoffset == 0)
       return;

   /* As we are eventually appending to the solution do not use
      VEC_iterate here. */
   n = VEC_length (ce_s, *results);
   for (j = 0; j < n; j++) {
       csvarinfo_t curr;
       c = *VEC_index (ce_s, *results, j);
       curr = cs_get_varinfo (c.var);

       /* If this varinfo represents a full variable just use it. */
       if (c.type == ADDRESSOF && curr->is_full_var)
	   c.offset = 0;
       /* If we do not know the offset add all subfields. */
       else if (c.type == ADDRESSOF && rhsoffset == UNKNOWN_OFFSET) {
	   csvarinfo_t temp = cs_lookup_vi_for_tree (curr->decl);
	   do {
	       struct constraint_expr c2;
	       c2.var = temp->id;
	       c2.type = ADDRESSOF;
	       c2.offset = 0;
	       if (c2.var != c.var)
		   VEC_safe_push (ce_s, heap, *results, &c2);
	       temp = temp->next;
	   } while (temp);
       }
       else if (c.type == ADDRESSOF) {
	   csvarinfo_t temp;
	   unsigned HOST_WIDE_INT offset = curr->offset + rhsoffset;

	   /* Search the sub-field which overlaps with the
	      pointed-to offset.  If the result is outside of the variable
	      we have to provide a conservative result, as the variable is
	      still reachable from the resulting pointer (even though it
	      technically cannot point to anything).  The last and first
	      sub-fields are such conservative results.
	      ???  If we always had a sub-field for &object + 1 then
	      we could represent this in a more precise way.  */
	   if (rhsoffset < 0 && curr->offset < offset)
	       offset = 0;
	   temp = cs_first_or_preceding_vi_for_offset (curr, offset);

	   /* If the found variable is not exactly at the pointed to
	     result, we have to include the next variable in the
	     solution as well.  Otherwise two increments by offset / 2
	     do not result in the same or a conservative superset
	     solution.  */
	   if (temp->offset != offset && temp->next != NULL) {
	       struct constraint_expr c2;
	       c2.var = temp->next->id;
	       c2.type = ADDRESSOF;
	       c2.offset = 0;
	       VEC_safe_push (ce_s, heap, *results, &c2);
	   }
	   c.var = temp->id;
	   c.offset = 0;
       }
       else
	   c.offset = rhsoffset;

       VEC_replace (ce_s, *results, j, &c);
   }
}

/* Given a COMPONENT_REF T, return the constraint_expr vector for it.
   If address_p is true the result will be taken its address of.
   If lhs_p is true then the constraint expression is assumed to be used
   as the lhs.  */
static void
cs_get_constraint_for_component_ref (tree t, VEC(ce_s, heap) **results,
				  bool address_p, bool lhs_p)
{
   tree orig_t = t;
   HOST_WIDE_INT bitsize = -1;
   HOST_WIDE_INT bitmaxsize = -1;
   HOST_WIDE_INT bitpos;
   tree forzero;
   struct constraint_expr *result;

   /* Some people like to do cute things like take the address of
     &0->a.b */
   forzero = t;
   while (handled_component_p (forzero)
	 || INDIRECT_REF_P (forzero)
	 || TREE_CODE (forzero) == MEM_REF)
       forzero = TREE_OPERAND (forzero, 0);

   if (CONSTANT_CLASS_P (forzero) && integer_zerop (forzero)) {
       struct constraint_expr temp;
       temp.offset = 0;
       temp.var = readonly_id;
       temp.type = SCALAR;
       VEC_safe_push (ce_s, heap, *results, &temp);
       return;
   }

   /* Handle type-punning through unions. If we are extracting a pointer
      from a union via a possibly type-punning access that pointer
      points to anything, similar to a conversion of an integer to
      a pointer.  */
   if (!lhs_p) {
      tree u;
      for (u = t;
	   TREE_CODE (u) == COMPONENT_REF || TREE_CODE (u) == ARRAY_REF;
	   u = TREE_OPERAND (u, 0))
	if (TREE_CODE (u) == COMPONENT_REF
	    && TREE_CODE (TREE_TYPE (TREE_OPERAND (u, 0))) == UNION_TYPE) 
	   return;
   }

   t = get_ref_base_and_extent (t, &bitpos, &bitsize, &bitmaxsize);

   /* Pretend to take the address of the base, we'll take care of
      adding the required subset of sub-fields below.  */
   cs_get_constraint_for_1 (t, results, true, lhs_p);
   if (VEC_length (ce_s, *results) == 0)
       return;
   else
       gcc_assert (VEC_length (ce_s, *results) == 1);
   
   result = VEC_last (ce_s, *results);

   if (result->type == SCALAR
       && cs_get_varinfo (result->var)->is_full_var)
       /* For single-field vars do not bother about the offset.  */
       result->offset = 0;
   else if (result->type == SCALAR) {
      /* In languages like C, you can access one past the end of an
	 array.  You aren't allowed to dereference it, so we can
	 ignore this constraint. When we handle pointer subtraction,
	 we may have to do something cute here.  */

      if ((unsigned HOST_WIDE_INT)bitpos < cs_get_varinfo (result->var)->fullsize
	  && bitmaxsize != 0) {
	  /* It's also not true that the constraint will actually start at the
	     right offset, it may start in some padding.  We only care about
	     setting the constraint to the first actual field it touches, so
	     walk to find it.  */
	  struct constraint_expr cexpr = *result;
	  csvarinfo_t curr;
	  VEC_pop (ce_s, *results);
	  cexpr.offset = 0;
	  for (curr = cs_get_varinfo (cexpr.var); curr; curr = curr->next) {
	      if (ranges_overlap_p (curr->offset, curr->size,
				    bitpos, bitmaxsize)) {
		  cexpr.var = curr->id;
		  VEC_safe_push (ce_s, heap, *results, &cexpr);
		  if (address_p)
		     break;
	       }
	   }
	   /* If we are going to take the address of this field then
	      to be able to compute reachability correctly add at least
	      the last field of the variable.  */
	   if (address_p && VEC_length (ce_s, *results) == 0) {
	       curr = cs_get_varinfo (cexpr.var);
	       while (curr->next)
		   curr = curr->next;
	       cexpr.var = curr->id;
	       VEC_safe_push (ce_s, heap, *results, &cexpr);
	   }
       }
       else if (bitmaxsize == 0) {
	  if (dump_file && (dump_flags & TDF_DETAILS))
	      fprintf (dump_file, "Access to zero-sized part of variable, ignoring\n");
       }
       else
	  if (dump_file && (dump_flags & TDF_DETAILS))
	      fprintf (dump_file, "Access to past the end of variable, ignoring\n");
   }
   else if (result->type == DEREF) {
      /* If we do not know exactly where the access goes say so.  Note
	 that only for non-structure accesses we know that we access
	 at most one subfiled of any variable.  */
       if (bitpos == -1 || bitsize != bitmaxsize
	  || AGGREGATE_TYPE_P (TREE_TYPE (orig_t))	/* Look into : Structure variables */
	  || result->offset == UNKNOWN_OFFSET)
	   result->offset = UNKNOWN_OFFSET;
       else
	   result->offset += bitpos;
   }
   else
       gcc_unreachable ();
}

/* Get a constraint expression vector from an SSA_VAR_P node.
   If address_p is true, the result will be taken its address of.  */
static void
cs_get_constraint_for_ssa_var (tree t, VEC(ce_s, heap) **results, bool address_p)
{
   struct constraint_expr cexpr;
   csvarinfo_t vi;

   /* We allow FUNCTION_DECLs here even though it doesn't make much sense. */
   gcc_assert (SSA_VAR_P (t) || DECL_P (t));

   /* For parameters, get at the points-to set for the actual parm decl. */
   if (TREE_CODE (t) == SSA_NAME
       && (TREE_CODE (SSA_NAME_VAR (t)) == PARM_DECL
 	  || TREE_CODE (SSA_NAME_VAR (t)) == RESULT_DECL)
       && SSA_NAME_IS_DEFAULT_DEF (t)) {
       cs_get_constraint_for_ssa_var (SSA_NAME_VAR (t), results, address_p);
       return;
   }

   vi = cs_get_vi_for_tree (t);
   cexpr.var = vi->id;
   cexpr.type = SCALAR;
   cexpr.offset = 0;

   /* If we are not taking the address of the constraint expr, add all
      sub-fiels of the variable as well.  */
   if (!address_p && !vi->is_full_var) {
      for (; vi; vi = vi->next) {
	   cexpr.var = vi->id;
	   VEC_safe_push (ce_s, heap, *results, &cexpr);
      }
      return;
   }

   VEC_safe_push (ce_s, heap, *results, &cexpr);
}

/* Given a tree T, return the constraint expression for it.  */
static void
cs_get_constraint_for_1 (tree t, VEC (ce_s, heap) **results, bool address_p,
		      bool lhs_p)
{
   struct constraint_expr temp;

   /* x = integer is all glommed to a single variable, which doesn't
     point to anything by itself.  That is, of course, unless it is an
     integer constant being treated as a pointer, in which case, we
     will return that this is really the addressof anything.  This
     happens below, since it will fall into the default case. The only
     case we know something about an integer treated like a pointer is
     when it is the NULL pointer, and then we just say it points to
     NULL.

     Do not do that if -fno-delete-null-pointer-checks though, because
     in that case *NULL does not fail, so it _should_ alias *anything.
     It is not worth adding a new option or renaming the existing one,
     since this case is relatively obscure.  */
   if ((TREE_CODE (t) == INTEGER_CST && integer_zerop (t))
      /* The only valid CONSTRUCTORs in gimple with pointer typed
	 elements are zero-initializer.  But in IPA mode we also
	 process global initializers, so verify at least.  */
      || (TREE_CODE (t) == CONSTRUCTOR
	  && CONSTRUCTOR_NELTS (t) == 0)) {
       if (flag_delete_null_pointer_checks) {
	   temp.var = nothing_id;
           temp.type = ADDRESSOF;
           temp.offset = 0;
           VEC_safe_push (ce_s, heap, *results, &temp);
       }
       return;
   }

  /* String constants are read-only. Don't consider them. 
   if (TREE_CODE (t) == STRING_CST)
       return;*/

   /* String constants are read-only. */
   if (TREE_CODE (t) == STRING_CST) {
      temp.var = readonly_id;
      temp.type = SCALAR;
      temp.offset = 0;
      VEC_safe_push (ce_s, heap, *results, &temp);
      return;
   }


   switch (TREE_CODE_CLASS (TREE_CODE (t))) {
       case tcc_expression:
       {
           switch (TREE_CODE (t)) {
	       case ADDR_EXPR:
	           cs_get_constraint_for_address_of (TREE_OPERAND (t, 0), results);
	           return;
	        default:;
	   }
 	   break;
       }
       case tcc_reference:
       {
	   switch (TREE_CODE (t)) {
	       case MEM_REF:
	       {
	           struct constraint_expr cs;
	      	   csvarinfo_t vi, curr;
	           tree off = double_int_to_tree (sizetype, mem_ref_offset (t));
	      	   cs_get_constraint_for_ptr_offset (TREE_OPERAND (t, 0), off, results);
                   if (VEC_length (ce_s, *results) == 0)
                       return;
	      	   cs_do_deref (results);

	           /* If we are not taking the address then make sure to process
		      all subvariables we might access.  */
	      	   cs = *VEC_last (ce_s, *results);
	      	   if (address_p || cs.type != SCALAR)
		       return;

	      	   vi = cs_get_varinfo (cs.var);
	      	   curr = vi->next;
	      	   if (!vi->is_full_var && curr) {
		       unsigned HOST_WIDE_INT size;
		       if (host_integerp (TYPE_SIZE (TREE_TYPE (t)), 1))
		           size = TREE_INT_CST_LOW (TYPE_SIZE (TREE_TYPE (t)));
		       else
		           size = -1;
		       for (; curr; curr = curr->next) {
		      	   if (curr->offset - vi->offset < size) {
			       cs.var = curr->id;
			       VEC_safe_push (ce_s, heap, *results, &cs);
			   }
		           else
			       break;
		       }
		   }
	           return;
	       }
	       case ARRAY_REF:
	       case ARRAY_RANGE_REF:
	       case COMPONENT_REF:
	           cs_get_constraint_for_component_ref (t, results, address_p, lhs_p);
	           return;
	       case VIEW_CONVERT_EXPR:
	           cs_get_constraint_for_1 (TREE_OPERAND (t, 0), results, address_p, lhs_p);
	    	   return;
	       /* We are missing handling for TARGET_MEM_REF here.  */
	       default:;
	   }
	   break;
       }
       case tcc_exceptional:
       {
	   switch (TREE_CODE (t)) {
	       case SSA_NAME:
	       {
	           cs_get_constraint_for_ssa_var (t, results, address_p);
	           return;
	       }
	       case CONSTRUCTOR:
	       {
	           unsigned int i;
	      	   tree val;
	      	   VEC (ce_s, heap) *tmp = NULL;
	      	   FOR_EACH_CONSTRUCTOR_VALUE (CONSTRUCTOR_ELTS (t), i, val) {
		       struct constraint_expr *rhsp;
		       unsigned j;
		       cs_get_constraint_for_1 (val, &tmp, address_p, lhs_p);
		       FOR_EACH_VEC_ELT (ce_s, tmp, j, rhsp)
		           VEC_safe_push (ce_s, heap, *results, rhsp);
		       VEC_truncate (ce_s, tmp, 0);
		   }
	           VEC_free (ce_s, heap, tmp);
	           /* We do not know whether the constructor was complete,
	              so technically we have to add &NOTHinG or &ANYTHinG
		      like we do for an empty constructor as well.  */
	           return;
	       }
	       default:;
	   }
	   break;
       }
       case tcc_declaration:
       {
	   cs_get_constraint_for_ssa_var (t, results, address_p);
	   return;
       }
       case tcc_constant:
	   return;
       default:;
   }
}

/* Efficiently generates constraints from all entries in *RHSC to all
   entries in *LHSC.  */
static void
cs_process_all_all_constraints (VEC (ce_s, heap) *lhsc, VEC (ce_s, heap) *rhsc)
{
   struct constraint_expr *lhsp, *rhsp;
   unsigned i, j;

   FOR_EACH_VEC_ELT (ce_s, lhsc, i, lhsp) {
       FOR_EACH_VEC_ELT (ce_s, rhsc, j, rhsp) {
           cs_process_constraint (new_constraint (*lhsp, *rhsp));
           multi_rhs = true;
       }
       multi_rhs = false;
   }
}

/* Given a tree T, return the constraint expression for taking the
   address of it. */
static void
cs_get_constraint_for_address_of (tree t, VEC (ce_s, heap) **results)
{
   struct constraint_expr *c;
   unsigned int i;

   cs_get_constraint_for_1 (t, results, true, true);
   FOR_EACH_VEC_ELT (ce_s, *results, i, c) {
       if (c->type == DEREF)
	   c->type = SCALAR;
       else
	   c->type = ADDRESSOF;
   }
}

/* Given a gimple tree T, return the constraint expression vector for it.  */
static void
cs_get_constraint_for (tree t, VEC (ce_s, heap) **results)
{
  gcc_assert (VEC_length (ce_s, *results) == 0);
  cs_get_constraint_for_1 (t, results, false, true);
}

/* Creation function node for DECL, using NAME, and return the index
   of the variable we've created for the function.  */
static csvarinfo_t
cs_create_func_info_for (tree decl, const char *name)
{
   csvarinfo_t vi, prev_vi;
   tree arg;
   unsigned int i;
   bool is_varargs = false;
   unsigned int num_args = count_num_arguments (decl, &is_varargs);

   /* Create the variable info.  */
   vi = cs_new_var_info (decl, name);
   vi->offset = 0;
   vi->size = 1;
   vi->fullsize = num_args + 1;
   vi->may_have_pointers = false;
   if (is_varargs)
       vi->fullsize = ~0;
   cs_insert_vi_for_tree (vi->decl, vi);

   prev_vi = vi;

   /* Set up variables for each argument.  */
   arg = DECL_ARGUMENTS (decl);
   for (i = 1; i < num_args + 1; i++) {
       csvarinfo_t argvi;
       tree argdecl = decl;

       if (arg)
           argdecl = arg;

       argvi = cs_new_var_info (argdecl, alias_get_name (argdecl));
       argvi->offset = i;
       argvi->size = 1;
       argvi->is_full_var = true;
       argvi->fullsize = vi->fullsize;
       if (arg)
           argvi->may_have_pointers = true;
       gcc_assert (prev_vi->offset < argvi->offset);
       prev_vi->next = argvi;
       prev_vi = argvi;
       if (arg) {
           cs_insert_vi_for_tree (arg, argvi);
           arg = DECL_CHAIN (arg);
       }
   }

   /* Add one representative for all further args.  */
   if (is_varargs) {
       csvarinfo_t argvi;
       const char *newname;
       char *tempname;
       tree decl;

       asprintf (&tempname, "%s.varargs", name);
       newname = ggc_strdup (tempname);
       free (tempname);

       /* CHANGE DUE TO GCC-4.7.2 */
       /* We need sth that can be pointed to for va_start.  */
       decl = build_fake_var_decl (ptr_type_node);

       /* According to gcc-4.6.*
       decl = create_tmp_var_raw (ptr_type_node, name);
       get_var_ann (decl); */

       argvi = cs_new_var_info (decl, newname);
       argvi->offset = 1 + num_args;
       argvi->size = ~0;
       argvi->is_full_var = true;
       argvi->is_heap_var = true;
       argvi->fullsize = vi->fullsize;
       gcc_assert (prev_vi->offset < argvi->offset);
       prev_vi->next = argvi;
       prev_vi = argvi;
   }

   return vi;
}

/* Find the first varinfo in the same variable as START that overlaps with
   OFFSET.  Return NULL if we can't find one.  */
static csvarinfo_t
cs_first_vi_for_offset (csvarinfo_t start, unsigned HOST_WIDE_INT offset)	/* Look into */
{
   offset += start->offset;
 
   /* If the offset is outside of the variable, bail out.  */
   if (offset >= start->fullsize)
       return NULL;

   /* If we cannot reach offset from start, lookup the first field
      and start from there.  */
   if (start->offset > offset)
       start = cs_lookup_vi_for_tree (start->decl);

   while (start) {
      /* We may not find a variable in the field list with the actual
         offset when when we have glommed a structure to a variable.
         In that case, however, offset should still be within the size
         of the variable. */
       if (offset >= start->offset
           && (offset - start->offset) < start->size)
           return start;

       start= start->next;
   }

   return NULL;
}

/* Handle aggregate copies by expanding into copies of the respective
   fields of the structures.  */
static void
cs_do_structure_copy (tree lhsop, tree rhsop)  /* Look into : Structure variables */
{
   struct constraint_expr *lhsp, *rhsp;
   VEC (ce_s, heap) *lhsc = NULL, *rhsc = NULL;
   unsigned j;

	fprintf(dump_file, "cs_do_structure_copy\n");
   cs_get_constraint_for (lhsop, &lhsc);
   cs_get_constraint_for_rhs (rhsop, &rhsc);
   lhsp = VEC_index (ce_s, lhsc, 0);
   rhsp = VEC_index (ce_s, rhsc, 0);
   if (lhsp->type == DEREF)
       return;
   if (rhsp->type == DEREF) {
       gcc_assert (VEC_length (ce_s, rhsc) == 1);
       rhsp->var = undef_id;
       rhsp->offset = 0;
       rhsp->type = ADDRESSOF;
       cs_process_all_all_constraints (lhsc, rhsc);
   }
   else if (lhsp->type == SCALAR &&
            (rhsp->type == SCALAR || rhsp->type == ADDRESSOF)) {
       HOST_WIDE_INT lhssize, lhsmaxsize, lhsoffset;
       HOST_WIDE_INT rhssize, rhsmaxsize, rhsoffset;
       unsigned k = 0;
       get_ref_base_and_extent (lhsop, &lhsoffset, &lhssize, &lhsmaxsize);
       get_ref_base_and_extent (rhsop, &rhsoffset, &rhssize, &rhsmaxsize);
       for (j = 0; VEC_iterate (ce_s, lhsc, j, lhsp);) {
           csvarinfo_t lhsv, rhsv;
           rhsp = VEC_index (ce_s, rhsc, k);
           lhsv = cs_get_varinfo (lhsp->var);
           rhsv = cs_get_varinfo (rhsp->var);
           if (lhsv->may_have_pointers
               && (lhsv->is_full_var
                  || rhsv->is_full_var
                  || ranges_overlap_p (lhsv->offset + rhsoffset, lhsv->size,
                                       rhsv->offset + lhsoffset, rhsv->size)))
               cs_process_constraint (new_constraint (*lhsp, *rhsp));
           if (!rhsv->is_full_var && (lhsv->is_full_var
                  || (lhsv->offset + rhsoffset + lhsv->size
                      > rhsv->offset + lhsoffset + rhsv->size))) {
               ++k;
               if (k >= VEC_length (ce_s, rhsc))
                   break;
           }
           else
               ++j;
       }
   }
   else
       gcc_unreachable ();

   VEC_free (ce_s, heap, lhsc);
   VEC_free (ce_s, heap, rhsc);
}

static void
cs_init_base_vars (void)
{
  // csvarinfo_t var_nothing, var_integer, var_undef;
 csvarinfo_t var_nothing, var_readonly, var_escaped, var_undef;

   /* Create an UNKNOWN variable, for unknown pointer values. */
   var_undef = cs_new_var_info (NULL_TREE, "undef");
   gcc_assert (var_undef->id == undef_id);
   var_undef->offset = 0;
   var_undef->size = ~0;
   var_undef->fullsize = ~0;
   var_undef->next = NULL;

   /* Create the NULL variable, used to represent that a variable points
      to NULL.  */
   var_nothing = cs_new_var_info (NULL_TREE, "null");
   gcc_assert (var_nothing->id == nothing_id);
   var_nothing->offset = 0;
   var_nothing->size = ~0;
   var_nothing->fullsize = ~0;

   /* Create the INTEGER variable, used to represent that a variable points
     to what an INTEGER "points to".  
   var_integer = cs_new_var_info (NULL_TREE, "integer");
   gcc_assert (var_integer->id == integer_id);
   var_integer->size = ~0;
   var_integer->fullsize = ~0;
   var_integer->offset = 0;
   var_integer->next = NULL;*/

   /* Create an ESCAPED variable, for escaped pointer values. */
   var_escaped = cs_new_var_info (NULL_TREE, "escaped");
   gcc_assert (var_escaped->id == escaped_id);
   var_escaped->offset = 0;
   var_escaped->size = ~0;
   var_escaped->fullsize = ~0;
   var_escaped->next = NULL;

   /* Create the READONLY variable, used to represent string constants
      and integer pointers. */
   var_readonly = cs_new_var_info (NULL_TREE, "readonly");
   gcc_assert (var_readonly->id == readonly_id);
   var_readonly->size = ~0;
   var_readonly->fullsize = ~0;
   var_readonly->offset = 0;
   var_readonly->next = NULL;

}

static void
cs_init_alias_vars (void)
{
   csvarmap = VEC_alloc (csvarinfo_t, heap, 200);
   callstrings = VEC_alloc (callstring, heap, 500);
   aliases = VEC_alloc (constraint_t, heap, 200);
   mayinfo = VEC_alloc (pointsto_val, heap, 200);
   liveinfo = VEC_alloc (df_list, heap, 200);
   constraint_pool = create_alloc_pool ("Constraint pool", sizeof (struct constraint), 200);
   csvarinfo_pool = create_alloc_pool ("Variable pool", sizeof (struct csvariable_info), 200);
   vi_for_tree = pointer_map_create ();
   cs_init_base_vars ();
   gcc_obstack_init (&fake_var_decl_obstack);
}

/* returns true if the statement is the end block's return statement. */
static bool
is_gimple_endblock (gimple t)
{
   return (gimple_code (t) == GIMPLE_RETURN);
}

/* Returns true if the basic block is a call block. */
static bool
call_block (basic_block bb)
{
   return ((bb_csipdfa_info)(bb->aux))->call_block;
}

/* Returns true if the basic block is a return block. */
static bool
return_block (basic_block bb)
{
   return ((bb_csipdfa_info)(bb->aux))->return_block;
}

/*---------------------------------------------
  Check if the given two callstrings are equal.
  --------------------------------------------*/
static bool
callstrings_equal (callstring a, callstring b)
{
   while (a) {
       if (b == NULL)
           return false;
       if (a->bb != b->bb)
           return false;
       a = a->next;
       b = b->next;
   }
   if (b)
      return false;
   return true;
}

/*----------------------------------------------------------------------------
  Check if the given callstring already exists in the callstrings at in of bb.
  If it does, return the index from the pool, else return -1.
  ---------------------------------------------------------------------------*/
static int
check_callstring_existance (callstring cs, basic_block bb, bool in_ipdfa)
{
   csdfa_info ai = (in_ipdfa) ? in_of_bb (bb) : out_of_bb (bb);
   for (; ai; ai=ai->next) {
       callstring bi = VEC_index (callstring, callstrings, ai->cs_index);
       if (callstrings_equal (bi, cs)) 
           return ai->cs_index;
   }
   return -1;
}

/* Returns true if the basic block is a start block. */
static bool
start_block (basic_block bb)
{
   return ((bb_csipdfa_info)(bb->aux))->start_block;
}

/* Returns true if the basic block is an end block (i.e, has the statement 'return'
   of the cfg). */
static bool
end_block (basic_block bb)
{
   return ((bb_csipdfa_info)(bb->aux))->end_block;
}

static bool
has_deref_stmt (basic_block bb)
{ 
   return ((bb_csipdfa_info)(bb->aux))->bb_has_deref_stmts; 
}

/* Returns true if the block has no pointer statements, is neither
   call nor end block, and has a single predecessor. Such blocks
   can be bypassed. */
static bool
pta_bypass_block (basic_block bb)
{
   return ((bb_csipdfa_info)(bb->aux))->pta_bypass_block;
}

static bool
lv_bypass_block (basic_block bb)
{
   return ((bb_csipdfa_info)(bb->aux))->lv_bypass_block;
}

static bool
is_pta_bypass_block (basic_block bb)
{
   return (single_pred_p (bb) && !get_constraint_start (bb)
         && !end_block (bb) && !return_block (bb));
}

static bool
is_lv_bypass_block (basic_block bb, bool single_succ)
{
   if (!single_succ)
       return (single_succ_p (bb) && !get_constraint_end (bb) 
            && !return_block (bb) && !start_block (bb)); 
   return (!get_constraint_end (bb)
            && !return_block (bb) && !start_block (bb));
}

/* Set the reverse postorder index of the basic block. */
static void
set_rp_index (basic_block bb, int i)
{
   ((bb_csipdfa_info)(bb->aux))->rp_index = i;
}

/* Get the reverse postorder index of the basic block. */
static int
get_rp_index (basic_block bb)
{
   return ((bb_csipdfa_info)(bb->aux))->rp_index;
}

static void
set_bb_to_process_pta (struct cgraph_node *node, basic_block bb)
{
   bb_worklist bb_list = worklist_of_cnode (node);
   int index = get_rp_index (bb);
   bb_list[index].process_pta = true;
}

static void
set_bb_to_process_lv (struct cgraph_node *node, basic_block bb)
{
   bb_worklist bb_list = worklist_of_cnode (node);
   int index = get_rp_index (bb);
   bb_list[index].process_lv = true;
   bb_list[index].process_pta = true;
}

/* To start with the worklist, we create a doubly linked
list - main <=> main */
static void
initialize_fn_worklist (void)
{
   start = new_worklist_node (cnode);
   set_bb_to_process_lv (cnode, end_bb_of_fn (cnode));
   set_bb_to_process_pta (cnode, start_bb_of_fn (cnode));
   /* Initialize the global worklist pointers. */
   pta_node = lv_node = start;
}

/* Set the function in worklist. */
static void
set_in_lv_worklist (struct cgraph_node *called_fn)
{
   worklist tmp;
   for (tmp=start; tmp; tmp=tmp->next) {
       if (tmp->called_fn == called_fn) {
           tmp->process_lv = true;
	   tmp->process_pta = true;
           return;
       }
   }
}

static void
set_in_pta_worklist (struct cgraph_node *called_fn)
{
   worklist tmp;
   for (tmp=pta_node; tmp; tmp=tmp->next) {
      if (tmp->called_fn == called_fn) {
          tmp->process_pta = true;
          return;
       }
   }
   /* Create a new worklist node, and add it to the worklist. */
   tmp = new_worklist_node (called_fn);
   tmp->next = pta_node->next;
   pta_node->next = tmp;
}

static void
add_to_fn_worklist (struct cgraph_node *called_fn)
{
   worklist tmp;
   for (tmp=lv_node; tmp; tmp=tmp->next) {
       if (tmp->called_fn == called_fn) {
           tmp->process_pta = true;
           tmp->process_lv = true;
           set_bb_to_process_pta (called_fn, start_bb_of_fn (called_fn));
           return;
       }
   }
   /* Create a new worklist node, and add it to the worklist. */
   tmp = new_worklist_node (called_fn);
   tmp->next = lv_node->next;
   lv_node->next = tmp;
}

static void
add_to_fn_worklist_1 (struct cgraph_node *called_fn)
{
   worklist tmp;
   for (tmp=start; tmp; tmp=tmp->next) {
       if (tmp->called_fn == called_fn) {
           tmp->process_pta = true;
           tmp->process_lv = true;
           return;
       }
   }
   /* Create a new worklist node, and add it to the worklist. */
   tmp = new_worklist_node (called_fn);
   tmp->next = lv_node->next;
   lv_node->next = tmp;
}

static tree
cs_get_var (tree t)
{
   if (TREE_CODE (t) == MEM_REF) {
       t = TREE_OPERAND (t, 0);
       return cs_get_var (t);
   }
   return t;
}

/* create a new constraint list holder. */
static constraint_list
new_constraint_list (df_list constraint, constraint_list *lst, bool is_alias)
{
   constraint_list ret = (constraint_list) alloc_mem (sizeof (struct constraint_list_def));
   ret->stmt_constraint_chain = constraint;
   ret->prev = *lst;
   ret->is_alias = is_alias;
   return ret;
}

static constraint_list
new_constraint_list_1 (df_list constraint)
{
   constraint_list ret = (constraint_list) alloc_mem (sizeof (struct constraint_list_def));
   ret->stmt_constraint_chain = constraint;
   ret->is_alias = true;
   return ret;
}

/* ----------------------------------------------------------------------------------
   In case of multiple RHS, or a PHI node, we need to extend the alias information of
   the last_alias by appending the passed alias constraint to the stmt_alias_chain. For
   example, if we have a_1 -> a_3 as the last_alias, and we the new_alias is a_1 = a_5,
   we need to update the stmt_alias_chain of last_alias to create a linked list given as
   [a_1 -> a_3] ------> [a_1 = a_5]
   ---------------------------------------------------------------------------------*/
static void
extend_stmt_constraint_chain (constraint_list lst, df_list constraint)
{
    df_list ai;
    for (ai=lst->stmt_constraint_chain; ai->next; ai=ai->next);
    ai->next = constraint;
}

/* -------------------------------------------------------------------------------
   Append the newly discovered alias to the bb_alias_list of the current basic block.
   'last_alias' points to the last appended alias of the basic block.
   ------------------------------------------------------------------------------- */
static void
append_to_bb_constraints (df_list constraint, bool is_alias)
{
   constraint_list lst = get_constraint_end (cbb);
   if (get_constraint_start (cbb) == NULL) {
       constraint_list elt = new_constraint_list (constraint, &lst, is_alias);
       ((bb_csipdfa_info)(cbb->aux))->constraint_start = elt;
       ((bb_csipdfa_info)(cbb->aux))->constraint_end = elt;
   }
   else {
       if (is_alias) {
           if (!multi_rhs) {
               lst->next = new_constraint_list (constraint, &lst, is_alias); 
               ((bb_csipdfa_info)(cbb->aux))->constraint_end = lst->next;
           }
           else
               extend_stmt_constraint_chain (lst, constraint);
       }
       else {
	   if (lst->is_alias) {
	       lst->next = new_constraint_list (constraint, &lst, is_alias);
	       ((bb_csipdfa_info)(cbb->aux))->constraint_end = lst->next;
           }
	   else
	       extend_stmt_constraint_chain (lst, constraint);
       }
   }
}

static void
append_to_fptr_constraints (df_list constraint)
{
   if (fptr_constraint_list == NULL) {
       constraint_list elt = new_constraint_list_1 (constraint);
       fptr_constraint_list = elt;
       last_constraint = elt;
   }
   else {
       if (!multi_rhs) {
           last_constraint->next = new_constraint_list_1 (constraint);
           last_constraint = last_constraint->next;
       }
       else
           extend_stmt_constraint_chain (last_constraint, constraint);
   }
}

/* Create and return a constriant container with val as the 
   passed integer value. */
static df_list
create_df_constraint (int val)
{
   df_list ret = (df_list) alloc_mem (sizeof (struct df_container));
   ret->val = val;
   return ret;
}

/* -------------------------------------------------------------------
   Check whether the given alias is already present in the alias pool.
   -------------------------------------------------------------------*/
static bool 
check_alias_inclusion (constraint_t t, csvarinfo_t vi, unsigned int *loc)
{
   df_list ai;
   for (ai=vi->constraints_with_vi_as_lhs; ai; ai=ai->next) {
       if (constraint_equal (*t, *VEC_index (constraint_t, aliases, ai->val))) {
	   *loc = ai->val;
           return true;
       }
   }
   return false;
}

/* -----------------------------------------------------------------------------------
   function to append the constraint index to the 'constriants_with_vi_as_lhs' of the 
   varinfo on the LHS of the constraint.
   ----------------------------------------------------------------------------------*/
static void
append_constraint_to_varinfo (csvarinfo_t t, int alias_no)
{
    df_list elt = create_df_constraint (alias_no);
    elt->next = t->constraints_with_vi_as_lhs;
    t->constraints_with_vi_as_lhs = elt;
}

/*-------------------------------------------------------------------------
  Insert the constraint t in the alias pool. Update the alias list for the
  current basic block. Also, update the bb_alias_list of variable vi (forming 
  the LHS of the constraint) to reflect the fact that variable vi is the
  lhs of some constraint t.
  ------------------------------------------------------------------------*/
static void
insert_alias_in_pool (constraint_t t, csvarinfo_t vi)
{
     df_list new_alias;
     unsigned int loc;
     bool alias_found = check_alias_inclusion (t, vi, &loc);
     if (!alias_found) {
         loc = VEC_length (constraint_t, aliases);
         VEC_safe_push (constraint_t, heap, aliases, t);
         append_constraint_to_varinfo (vi, loc);
     }
     new_alias = create_df_constraint (loc);
     if (!compute_alias_and_pinfo)
         append_to_bb_constraints (new_alias, true);
     else 
	 append_to_fptr_constraints (new_alias);
}

/*-------------------------------------------------------------------------------------
  Function which processes the constraint t, retrieves the lhs and rhs of the pointsto
  constraint, and updates the alias pool. 
  ------------------------------------------------------------------------------------*/
static void
cs_process_constraint (constraint_t t)
{
   struct constraint_expr rhs = t->rhs;
   struct constraint_expr lhs = t->lhs;

   gcc_assert (rhs.var < VEC_length (csvarinfo_t, csvarmap));
   gcc_assert (lhs.var < VEC_length (csvarinfo_t, csvarmap));

   if (!is_proper_var (lhs.var))
       return;
   /* ADDRESSOF on the lhs is invalid.  */
   gcc_assert (lhs.type != ADDRESSOF);

   if (check_deref) 
       deref_stmt = (rhs.type == DEREF || lhs.type == DEREF);

   if (!compute_only_pinfo)
       insert_alias_in_pool (t, cs_get_varinfo (lhs.var));

   if (compute_alias_and_pinfo)
       compute_stmt_out_1 (cpinfo, t);
   
   if (compute_only_pinfo)
       compute_stmt_out_2 (cpinfo, t);
}

/* Initialize the bbaux info for each basic block. */
static void
initialize_bbaux_info (struct cgraph_node *node, basic_block bb)
{
   bb->aux = (void *) alloc_mem (sizeof (struct bb_csipdfa_info_def));
   ((bb_csipdfa_info)(bb->aux))->rp_index = -1;
   ((bb_csipdfa_info)(bb->aux))->cnode = node;
   ((bb_csipdfa_info)(bb->aux))->call_block = false;
   ((bb_csipdfa_info)(bb->aux))->return_block = false;
   ((bb_csipdfa_info)(bb->aux))->start_block = false;
   ((bb_csipdfa_info)(bb->aux))->end_block = false;
   /*((bb_csipdfa_info)(bb->aux))->has_alias = false;*/
   ((bb_csipdfa_info)(bb->aux))->pta_bypass_block = false;
   ((bb_csipdfa_info)(bb->aux))->lv_bypass_block = false;
   ((bb_csipdfa_info)(bb->aux))->bb_has_deref_stmts = false;
}

/* Initialize the cgraphaux info for each cgraph node. */
static void
initialize_cgraphaux_info (struct cgraph_node *node, basic_block endblock, 
			   int *rp, int num_bb)
{
   int i;
   basic_block bb;
   bb_worklist bb_list = XNEWVEC (struct bb_worklist_def, num_bb+1);
   node->aux = (void *) alloc_mem (sizeof (struct cgraph_info_def));
   for (i=0; i<num_bb; i++) {
       bb = BASIC_BLOCK (rp[i]);
       set_rp_index (bb, i);
       bb_list[i].bb = bb;
       bb_list[i].process_pta = false;
       bb_list[i].process_lv = false;
   }

   /* Make the endblock the last block in reverse postorder. */
   set_rp_index (endblock, i);

   bb_list[i].bb = endblock;
   bb_list[i].process_pta = false;
   bb_list[i].process_lv = false;
 
   ((cgraph_info)(node->aux))->bb_list = bb_list;
   ((cgraph_info)(node->aux))->pcsmap_required = false;
   ((cgraph_info)(node->aux))->lcsmap_required = false;
}

/*--------------------------------------------------------------------
  Returns the called function's decl tree. If it is a direct function 
  call, the TREE_CODE of the returned decl will be FUNCTION_DECL. If 
  it is a call via function pointer, it will be VAR_DECL. 
  -------------------------------------------------------------------*/
static tree
get_called_fn_decl (gimple stmt)
{
    /* If we can resolve it here, its a simple function call. */
    tree decl = gimple_call_fndecl (stmt);
    /* The call is through function pointer. */
    if (!decl)
        decl = gimple_call_fn (stmt);
    return decl;
}

static void
generate_liveness_constraint (unsigned int id)
{
   df_list live_var = create_df_constraint (id);
   append_to_bb_constraints (live_var, false);
}

static void
map_arguments_at_call (gimple stmt, tree decl, bool generate_liveness)
{
   VEC(ce_s, heap) *rhsc = NULL;
   size_t j;
   int argoffset = 1;
   csvarinfo_t fi;

   /* Generate liveness for call via function pointers and 
      library routines. */
   if (generate_liveness) {
       struct constraint_expr *exp;
       unsigned i;

       for (j = 0; j < gimple_call_num_args (stmt); j++) {
	   tree arg = gimple_call_arg (stmt, j);
           if (field_must_have_pointers (arg) && TREE_CODE (arg) != ADDR_EXPR) {
	       VEC (ce_s, heap) *results = NULL;
	       cs_get_constraint_for (arg, &results);
	       FOR_EACH_VEC_ELT (ce_s, results, i, exp)
		   generate_liveness_constraint (exp->var);
	       VEC_free (ce_s, heap, results);
	   }
       }
       return;
   }
 
   /* Map call arguments. */
   fi = cs_get_vi_for_tree (decl);

   for (j = 0; j < gimple_call_num_args (stmt); j++) {
       struct constraint_expr lhs ;
       struct constraint_expr *rhsp;
       tree arg = gimple_call_arg (stmt, j);
       if (field_must_have_pointers (arg)) {
           cs_get_constraint_for (arg, &rhsc);
           lhs.type = SCALAR;
           lhs.var = cs_first_vi_for_offset (fi, argoffset)->id;
           lhs.offset = 0;
           while (VEC_length (ce_s, rhsc) != 0) {
               rhsp = VEC_last (ce_s, rhsc);
               cs_process_constraint (new_constraint (lhs, *rhsp));
               VEC_pop (ce_s, rhsc);
               multi_rhs = true;
           }
           multi_rhs = false;
       }
       argoffset++;
   }
   VEC_free (ce_s, heap, rhsc);
}

static bool
possibly_deref (gimple stmt)
{
   tree lhsop = gimple_assign_lhs (stmt);
   tree rhsop = (gimple_num_ops (stmt) == 2) ? gimple_assign_rhs1 (stmt) : NULL;
   return ((TREE_CODE (lhsop) == MEM_REF) ||
           (rhsop && TREE_CODE (rhsop) == MEM_REF));
}

static void 
process_gimple_assign_stmt (gimple stmt)
{
   tree lhsop = gimple_assign_lhs (stmt);
   tree rhsop = (gimple_num_ops (stmt) == 2) ? gimple_assign_rhs1 (stmt) : NULL;

   if (field_must_have_pointers (lhsop)) {
       VEC(ce_s, heap) *lhsc = NULL;
       VEC(ce_s, heap) *rhsc = NULL;
       if (rhsop && AGGREGATE_TYPE_P (TREE_TYPE (lhsop)))  /* Look into : Structure variables */
 	{
	//gcc_assert (1);
           cs_do_structure_copy (lhsop, rhsop); }
       else {
           cs_get_constraint_for (lhsop, &lhsc);
           if (gimple_assign_rhs_code (stmt) == POINTER_PLUS_EXPR)
               cs_get_constraint_for_ptr_offset (gimple_assign_rhs1 (stmt),
                                           gimple_assign_rhs2 (stmt), &rhsc);
           else if ((CONVERT_EXPR_CODE_P (gimple_assign_rhs_code (stmt))
                     && !(POINTER_TYPE_P (gimple_expr_type (stmt))
                     && !POINTER_TYPE_P (TREE_TYPE (rhsop))))
                     || gimple_assign_single_p (stmt))
               cs_get_constraint_for (rhsop, &rhsc);
           cs_process_all_all_constraints (lhsc, rhsc);
       }
       VEC_free (ce_s, heap, rhsc);
       VEC_free (ce_s, heap, lhsc);
   }
   else {
       if (TREE_CODE (lhsop) == MEM_REF) 
	   generate_liveness_constraint (cs_get_vi_for_tree (cs_get_var (lhsop))->id); 
       if (rhsop && TREE_CODE (rhsop) == MEM_REF) 
	   generate_liveness_constraint (cs_get_vi_for_tree (cs_get_var (rhsop))->id);
   }
}

/* Find out aliases for PHI statements. */
static void
process_gimple_phi_stmt (gimple stmt)
{
   VEC(ce_s, heap) *lhsc = NULL;
   VEC(ce_s, heap) *rhsc = NULL;
   struct constraint_expr *c;
   size_t i;
   unsigned int j;

   /* For a phi node, assign all the arguments to the result. */
   cs_get_constraint_for (gimple_phi_result (stmt), &lhsc);
   for (i = 0; i < gimple_phi_num_args (stmt); i++) {
       tree strippedrhs = PHI_ARG_DEF (stmt, i);
       STRIP_NOPS (strippedrhs);
       cs_get_constraint_for (strippedrhs, &rhsc);
       for (j = 0; VEC_iterate (ce_s, lhsc, j, c); j++) {
           struct constraint_expr *c2;
           while (VEC_length (ce_s, rhsc) > 0) {
               c2 = VEC_last (ce_s, rhsc);
               cs_process_constraint (new_constraint (*c, *c2));
               VEC_pop (ce_s, rhsc);
               multi_rhs = true;
           }
       }
   }
   multi_rhs = false;
   VEC_free (ce_s, heap, rhsc);
   VEC_free (ce_s, heap, lhsc);
}

/* Generate liveness out of conditional statements. */
static void
process_gimple_cond_stmt (gimple stmt)
{
   struct constraint_expr *exp;
   unsigned i;

   tree op0 = gimple_cond_lhs (stmt);
   tree op1 = gimple_cond_rhs (stmt);

   if (field_must_have_pointers (op0) && TREE_CODE (op0) != ADDR_EXPR) {
       VEC (ce_s, heap) *results = NULL;
       cs_get_constraint_for (op0, &results);
       FOR_EACH_VEC_ELT (ce_s, results, i, exp) 
           generate_liveness_constraint (exp->var);
       VEC_free (ce_s, heap, results);
   }
   if (field_must_have_pointers (op1) && TREE_CODE (op1) != ADDR_EXPR) {
       VEC (ce_s, heap) *results = NULL;
       cs_get_constraint_for (op1, &results);
       FOR_EACH_VEC_ELT (ce_s, results, i, exp) 
           generate_liveness_constraint (exp->var);
       VEC_free (ce_s, heap, results);
   }
}

/* --------------------------------------------------------------------
   Perform necessary initializations for the callstrings pointsto pass.
   -------------------------------------------------------------------*/

/* CHANGE DUE TO GCC-4.7.2 */

/* Associate node with varinfo DATA. Worker for
   cgraph_for_node_and_aliases.  */
static bool
associate_varinfo_to_alias (struct cgraph_node *node, void *data)
{
  if (node->alias || node->thunk.thunk_p)
    cs_insert_vi_for_tree (node->decl, (csvarinfo_t)data);
  return false;
}

static void
init_info (void)
{
  // init_alias_heapvars ();
   cs_init_alias_vars ();

   for (cnode = cgraph_nodes; cnode; cnode = cnode->next) {
       struct cgraph_node *alias;
       csvarinfo_t vi;

       /* Nodes without a body, and clone nodes are not interesting. */
       if (!gimple_has_body_p (cnode->decl) || cnode->clone_of)
           continue;

       /* locating main function. */
       if (strcmp (cgraph_node_name (cnode), "main") == 0)
           main_firstbb = start_bb_of_fn (cnode);

       vi = cs_create_func_info_for (cnode->decl, cgraph_node_name (cnode));
     
	/* CHANGE due gcc-4.7.2 */
       cgraph_for_node_and_aliases (cnode, associate_varinfo_to_alias, vi, true);

       /* Associate the varinfo node with all aliases.  
       for (alias = cnode->same_body; alias; alias = alias->next)
           cs_insert_vi_for_tree (alias->decl, vi);*/
   }
}

/* Iterate over all the PHI nodes of the basic block and 
   calculate alias info for them. */
static bool
process_phi_pointers (void)
{
   gimple_stmt_iterator gsi;
   bool has_phi = false;
   for (gsi = gsi_start_phis (cbb); !gsi_end_p (gsi); gsi_next (&gsi)) {
       gimple phi = gsi_stmt (gsi);
       tree phi_result = gimple_phi_result (phi);
       if (is_gimple_reg (phi_result)) {
           if (field_must_have_pointers (phi_result)) {
               has_phi = true;
               process_gimple_phi_stmt (phi);
           }
       }
   }
   return has_phi;
}

static void
process_library_call (gimple stmt)
{
   /* Generate liveness. */
   map_arguments_at_call (stmt, NULL, true);
   /* Handle malloc by introducing a points to to heap. */
   if (gimple_call_flags (stmt) & ECF_MALLOC) {
       tree lhs = gimple_call_lhs (stmt);
       if (lhs && field_must_have_pointers (lhs))
           cs_make_constraint_from_heapvar (cs_get_vi_for_tree (lhs), "heap");
   }
}

static gimple_stmt_iterator
split_bb_at_stmt (gimple stmt)
{
   edge e = split_block (cbb, stmt);
   cbb = e->dest;
   initialize_bbaux_info (cnode, cbb);
   return gsi_start_bb (cbb);
}

static void
generate_retval_liveness (gimple stmt)
{
   tree retval = gimple_return_retval (stmt);
   if (retval!=NULL_TREE && field_must_have_pointers (retval)) {
       VEC(ce_s, heap) *rhsc = NULL;
       struct constraint_expr *rhs;
       unsigned int i;

       cs_get_constraint_for (retval, &rhsc);
       FOR_EACH_VEC_ELT (ce_s, rhsc, i, rhs) 
	  generate_liveness_constraint (rhs->var);
   }     
}

static void
preprocess_cfg (void)
{
   bool is_start_block;

   for (cnode=cgraph_nodes; cnode; cnode=cnode->next) {
       int *rp;
       basic_block endblock = NULL;
 
       /* Nodes without a body, and clone nodes are not interesting. */
       if (!gimple_has_body_p (cnode->decl) || cnode->clone_of)
          continue;

       set_cfun (DECL_STRUCT_FUNCTION (cnode->decl));
       current_function_decl = cnode->decl;
       is_start_block = true;

       FOR_EACH_BB (cbb) {
	   gimple_stmt_iterator gsi;
	   bool has_phi = false;

	   /* Initialize auxillary info. */
           if (!cbb->aux)
               initialize_bbaux_info (cnode, cbb);
           /* identification of start block, Sp */
           if (is_start_block) {
               ((bb_csipdfa_info)(cbb->aux))->start_block = true;
               is_start_block = false;
	   }
	   has_phi = process_phi_pointers ();
	   /* Iterate over the statements of bb. */
	   for (gsi = gsi_start_bb (cbb); !gsi_end_p (gsi); gsi_next (&gsi)) {
	       gimple stmt = gsi_stmt (gsi);

               /* Break at boundaries of callbb and returnbb. */
	       if (is_gimple_call (stmt) || is_gimple_endblock (stmt)) {
                   gimple_stmt_iterator origgsi = gsi;
                   tree decl = NULL;

                   /* Need not break in case of library routines. */
                   if (is_gimple_call (stmt)) {
                       decl = get_called_fn_decl (stmt);
                       if (TREE_CODE (decl) == FUNCTION_DECL) {
                           if (!DECL_STRUCT_FUNCTION (decl)) {
			       process_library_call (stmt);
                               continue;
                           }
                       }
                   }

		   gsi_prev (&gsi);
		   /* Check if the call/return stmt is the first stmt in the bb. */
		   if (gsi_end_p (gsi) || gimple_code (gsi_stmt (gsi)) == GIMPLE_LABEL) {
		       if (has_phi) {
			   ((bb_csipdfa_info)(cbb->aux))->pta_bypass_block = is_pta_bypass_block (cbb);
                           ((bb_csipdfa_info)(cbb->aux))->lv_bypass_block = is_lv_bypass_block (cbb, true);

			   gsi = split_bb_at_stmt (NULL);
			   has_phi = false;
		       }
		       /*else if (start_block (cbb)) 
			   gsi = split_bb_at_stmt (NULL);*/
		       else
			   gsi = origgsi;
		   }
                   else {
                       /* Split before the call/return stmt. */
                       ((bb_csipdfa_info)(cbb->aux))->pta_bypass_block = is_pta_bypass_block (cbb);
                       ((bb_csipdfa_info)(cbb->aux))->lv_bypass_block = is_lv_bypass_block (cbb, true);
                       gsi = split_bb_at_stmt (gsi_stmt (gsi));
                   }

                   /* Split the call into a new bb if its not the last stmt. */
                   origgsi = gsi;
                   gsi_next (&gsi);
                   if (!is_gimple_endblock (stmt) && !gsi_end_p (gsi)) {
                       gsi = origgsi;
                       split_block (cbb, gsi_stmt (gsi));
                   }
		   else 
                       gsi = origgsi;

                   if (is_gimple_call (stmt)) {
		       bool fptr_call = (TREE_CODE (decl) != FUNCTION_DECL);
		       /* Mark the calling function pointer as live. */
   		       if (fptr_call) {
       			   unsigned int var = cs_get_vi_for_tree (decl)->id;
       			   generate_liveness_constraint (var);
		       }
		       /* Mark call bb with its properties. */
		       ((bb_csipdfa_info)(cbb->aux))->bb_has_deref_stmts = fptr_call;
                       ((bb_csipdfa_info)(cbb->aux))->call_block = true; 
                       /* Discover the static call argument mapping. */ 
                       map_arguments_at_call (stmt, decl, fptr_call); 
                       /* Create an empty return block. */
                       gsi = split_bb_at_stmt (gsi_stmt (gsi));
                       ((bb_csipdfa_info)(cbb->aux))->return_block = true;
                       break;
                   }

                   if (is_gimple_endblock (stmt)) {
		       generate_retval_liveness (stmt);
                       ((bb_csipdfa_info)(cbb->aux))->end_block = true;
                       endblock = cbb;
                   }
               }

               /* Inspect other statements for possible pointers. */
               else if (is_gimple_assign (stmt)) {
                   /* Split in case of possible deref statement. */
                   if (possibly_deref (stmt)) {
                       gimple_stmt_iterator origgsi = gsi;
                       gsi_prev (&gsi);
                       if (!gsi_end_p (gsi)) {
                           ((bb_csipdfa_info)(cbb->aux))->pta_bypass_block = is_pta_bypass_block (cbb);
                           ((bb_csipdfa_info)(cbb->aux))->lv_bypass_block = is_lv_bypass_block (cbb, true);
                            gsi = split_bb_at_stmt (gsi_stmt (gsi));
                       }
                       else
                           gsi = origgsi;
                   }
		   check_deref = true; 
		   process_gimple_assign_stmt (stmt);
                   /* If the stmt is deref stmt, mark the bb as a bb containing
		      deref stmts. */
		   if (deref_stmt)
		       ((bb_csipdfa_info)(cbb->aux))->bb_has_deref_stmts = true;
		   check_deref = false;
               }

	       else if (gimple_code (stmt) == GIMPLE_COND)
		   process_gimple_cond_stmt (stmt); 
           }

           /* Decide if current block is a bypass block. */ 
           ((bb_csipdfa_info)(cbb->aux))->pta_bypass_block = is_pta_bypass_block (cbb);
           ((bb_csipdfa_info)(cbb->aux))->lv_bypass_block = is_lv_bypass_block (cbb, false);
       }

       if (dump_file)// && !ipacs_time)
           gimple_dump_cfg (dump_file, dump_flags);

       /* If there was no endblock, mark it. */
       if (!endblock) 
           endblock = EXIT_BLOCK_PTR_FOR_FUNCTION (DECL_STRUCT_FUNCTION (cnode->decl))->prev_bb;

       /* Set the reverse postorder index for all the bb. */
       rp = XNEWVEC (int, total_bbs);
       pre_and_rev_post_order_compute (NULL, rp, false);

       /* Initialize the cgraph info. */
       initialize_cgraphaux_info (cnode, endblock, rp, total_bbs);
       free (rp);
   }
}

/* ----------------------------------------------------------------
   Restoring the cfg by clearing the aux field of each basic block
   and removing unnecessary (split) blocks.
   ---------------------------------------------------------------*/
static void
restore_cfg (void)
{
   for (cnode = cgraph_nodes; cnode; cnode = cnode->next) {
       if (!gimple_has_body_p (cnode->decl) || cnode->clone_of)
           continue;
       current_function_decl = cnode->decl;
       set_cfun (DECL_STRUCT_FUNCTION (cnode->decl));
       /* Free cgraph node's aux field. */
       if (cnode->aux) {
           ggc_free (cnode->aux);
           cnode->aux = NULL;
       }
       /* Free each bb's aux field. */
       FOR_EACH_BB (cbb) {
           if (cbb->aux) {
               ggc_free (cbb->aux);
               cbb->aux = NULL;
           }
       }
       /* Merge bb's if necessary. */
       cleanup_tree_cfg ();
       /* Free the dominator info. */
       free_dominance_info (CDI_DOMINATORS);
       free_dominance_info (CDI_POST_DOMINATORS);
   }
}

/*------------------------------------------------------------------------
  Append a basic block to the CALLSTRING SEQUENCE, and return the index of
  the new callstring from the callstring pool.
  -----------------------------------------------------------------------*/
static int
append_bb_to_callstring (basic_block bb, int cs, basic_block succ_bb, bool in_ipdfa)
{
   int index;
   callstring oldcs, newcs;
   if (cs == -1)
       oldcs = NULL;
   else
       oldcs = VEC_index (callstring, callstrings, cs);
   newcs = (callstring) alloc_mem (sizeof (struct callstring_def));
   newcs->bb = bb;
   newcs->next = oldcs;
   index = check_callstring_existance (newcs, succ_bb, in_ipdfa);
   if (index == -1) {
       index = VEC_length (callstring, callstrings);
       VEC_safe_push (callstring, heap, callstrings, newcs);
   }
   return index;
}

static int
callstring_length (callstring ai)
{
   int len = 0;
   for (; ai; ai=ai->next) 
       len++;
   return len;
}
  
/* Find if callstring indexed at index is a substring of cs. */
static int
callstring_is_substring (callstring *cs, int index)
{
   callstring bi = (index == -1) ? NULL : VEC_index (callstring, callstrings, index);
   int len_ai = callstring_length (*cs);
   int len_bi = callstring_length (bi);

   if (len_ai > len_bi) {
       int diff = len_ai - len_bi;
       callstring ci;
       for (ci=*cs; ci && diff>0; ci=ci->next) 
	   diff--;
       while (ci) {
	   if (!bi)
	      break;
	   if (ci->bb != bi->bb)
	       return 0;
	   ci = ci->next;
	   bi = bi->next;
      }
      if (ci || bi)
	   return 0;
      return len_bi;
   }
   return 0;
}

static csdfa_info 
get_representative_csdfa (basic_block bb, callstring *cs)
{
   csdfa_info bi, rep_csdfa = NULL; 
   int max_len = 0;

   for (bi=in_of_bb (bb); bi; bi=bi->next) {
       int len = callstring_is_substring (cs, bi->cs_index);
       if (len > max_len) {
           rep_csdfa = bi;
	   max_len = len;
       }
   }
   return rep_csdfa;
}

/* Create a new callstring map cell. */
static cs_map
create_new_csmap_cell (csdfa_info ai, bool present_in_out)
{
   cs_map temp = (cs_map) alloc_mem (sizeof (struct cs_map_def));
   temp->lhs = ai;
   temp->present_in_out = present_in_out;
   return temp;
}

static csmap_rhs
create_csmap_rhs (csdfa_info ai, bool present_in_out)
{
   csmap_rhs newrhs = (csmap_rhs) alloc_mem (sizeof (struct csmap_rhs_def));
   newrhs->rhsel = ai;
   newrhs->present_in_out = present_in_out;
   return newrhs;
}


static void
print_callstring_sequence (int cs)
{
   if (cs == -1)
       fprintf (dump_file, "null");
   else {
       callstring ci = VEC_index (callstring, callstrings, cs);
       fprintf (dump_file, "{");
       for (; ci; ci=ci->next) {
           basic_block bb = ci->bb;
           if (ci->next == NULL)
               fprintf (dump_file, "%s.%d", cgraph_node_name (cnode_of_bb (bb)), bb->index);
           else
               fprintf (dump_file, "%s.%d, ", cgraph_node_name (cnode_of_bb (bb)), bb->index);
       }
       fprintf (dump_file, "}");
   }
}

static void
add_to_lcsmap_and_out (struct cgraph_node *node, csdfa_info a, int rep_index)
{
   cs_map di;
   bool found = false;
   basic_block end_bb = end_bb_of_fn (node);
   csdfa_info bi, prev = NULL;
   csdfa_info ai = copy_csdfa_info (a);
   ai->pinfo = -2;

   /* Add ai to lcsmap. */
   for (di=lcsmap_of_cnode (node); di; di=di->next) {
       csmap_rhs temp;
       if (di->lhs->cs_index == rep_index) {
           /* Add ai to the rhs of bi. */
	   found = true;
           ai->linfo = di->lhs->linfo;
           temp = create_csmap_rhs (ai, false);
           temp->next = di->rhs;
           di->rhs = temp;
           break;
       }
       else {
           csmap_rhs ci;
           for (ci=di->rhs; ci; ci=ci->next) {
               if (ci->rhsel->cs_index == rep_index) {
		   found = true;
		   ai->linfo = di->lhs->linfo;
                   /* Add ai to the rhs of bi. */
                   temp = create_csmap_rhs (ai, false);
                   temp->next = di->rhs;
                   di->rhs = temp;
                   break;
               }
           }
       }
       if (found)
	   break;
   }
   
   gcc_assert (found);
 
   /* Add ai to the out of endblock. */
   for (bi=out_of_bb (end_bb); bi; bi=bi->next) {
       if (ai->cs_index >= bi->cs_index)
           break;
       prev = bi;
   }
   if (prev == NULL) {
       ai->next = out_of_bb (end_bb);
       ((bb_csipdfa_info)(end_bb->aux))->out_ipdfa = ai;
   }
   else {
       ai->next = prev->next;
       prev->next = ai;
   }
}

static int
find_rep_in_lcsmap (struct cgraph_node *node, int cs_index)
{
   cs_map ai;
   for (ai=lcsmap_of_cnode (node); ai; ai=ai->next) {
       if (ai->lhs->cs_index == cs_index) 
           return -1;
       else {
           csmap_rhs bi;
           for (bi=ai->rhs; bi; bi=bi->next) {
               if (bi->rhsel->cs_index == cs_index)
                   return ai->lhs->cs_index;
           }
       }
   }
   return -1;
}

static int
get_representative_linfo (basic_block bb, int repcs)
{
   csdfa_info bi;
   for (bi=in_of_bb (bb); bi; bi=bi->next) {
       if (bi->cs_index == repcs)
	   return bi->linfo;
   }
   return -1;
}
/* Modify both callstrings and liveness maps. */
static int
append_bb_to_callstring_1 (struct cgraph_node *node, basic_block bb, csdfa_info *ai, 
		           basic_block succ_bb, bool in_ipdfa)
{
   int index = -1;
   callstring oldcs, newcs;
   if ((*ai)->cs_index == -1)
       oldcs = NULL;
   else
       oldcs = VEC_index (callstring, callstrings, (*ai)->cs_index);
   newcs = (callstring) alloc_mem (sizeof (struct callstring_def));
   newcs->bb = bb;
   newcs->next = oldcs;
   index = check_callstring_existance (newcs, succ_bb, in_ipdfa);
   if (index == -1) {
       csdfa_info rep = NULL;
       int repcs = find_rep_in_lcsmap (cnode, (*ai)->cs_index);
       index = VEC_length (callstring, callstrings);
       VEC_safe_push (callstring, heap, callstrings, newcs);
       (*ai)->cs_index = index;
       /* We need to compute the linfo for index. Was this 
          callstring represented by something else? */
       if (repcs == -1) { 
           rep = get_representative_csdfa (succ_bb, &newcs);
	   if (!rep) 
	       return -1;
           add_to_lcsmap_and_out (node, *ai, rep->cs_index);
	   (*ai)->linfo = rep->linfo;
       }
       else {
           /* Make the linfo of ai same as that of repcs at called node. */
	   callstring oldrep = VEC_index (callstring, callstrings, repcs);
	   callstring newrep = (callstring) alloc_mem (sizeof (struct callstring_def));
	   newrep->bb = bb;
	   newrep->next = oldrep;
           repcs = check_callstring_existance (newrep, succ_bb, in_ipdfa);
	   if (repcs == -1)
	       return -1;
           add_to_lcsmap_and_out (node, *ai, repcs);
	   (*ai)->linfo = get_representative_linfo (succ_bb, repcs);
       }
   }
   return index;
}



/* Function to add a variable to dflist of LIVE computation. */
static void
add_to_df_list (unsigned int val, int *list)
{

    if (!is_proper_var (val))
        return;

    if (*list < 0) {
       *list = init_live_set (val);
       return;
    }

*list = add_var_to_liveset(*list,val);
}


/*------------------------------------------------------------------------
  Remove a basic block from the given callstring, and return the index of
  the resulting callstring.
  -----------------------------------------------------------------------*/
static int
strip_bb_from_callstring (int cs, basic_block bb, bool in_ipdfa)
{
   int index = -1;
   callstring oldcs = VEC_index (callstring, callstrings, cs);
   callstring newcs = oldcs->next;
   if (!newcs)
       return -1;
   else {
       index = check_callstring_existance (newcs, bb, in_ipdfa);
       return index;
   }
}

static int
get_caller_bb (basic_block *bb, int cs_index)
{
   *bb = get_bb_from_callstring (cs_index);
   return strip_bb_from_callstring (cs_index, *bb, false);
}

/* -----------------------------------------------------------------
   Function to delete all the information associated with ipacs pass,
   the data structures and the VEC information.
   ----------------------------------------------------------------*/
static void
delete_ipacs_info (void)
{
    //delete_alias_heapvars ();
    pointer_map_destroy (vi_for_tree);
    VEC_free (csvarinfo_t, heap, csvarmap);
    VEC_free (callstring, heap, callstrings);
    VEC_free (constraint_t, heap, aliases);
    VEC_free (pointsto_val, heap, mayinfo);
    VEC_free (df_list, heap, liveinfo);
    free_alloc_pool (csvarinfo_pool);
    free_alloc_pool (constraint_pool);
    obstack_free (&fake_var_decl_obstack, NULL);
}


static void
print_df_list (int info)
{
  
  bitmap_iterator bir;
  unsigned int var;
  int i ,j = 1;
  bitmap live_set;
  /*df_list ai;*/
   if (info<0) 
       fprintf (dump_file, "NULL");
  live_set = get_live_set(info);
  i = bitmap_count_bits(live_set);
  EXECUTE_IF_SET_IN_BITMAP(live_set,0,var,bir){
/*   for (ai=info; ai; ai=ai->next) {*/
       fprintf (dump_file, "  %s", cs_get_varinfo (var)->name);
       if (j<i)
	   fprintf (dump_file, ",");
	j++;
   }
}

static void
print_pointsto_info (basic_block bb)
{
   csdfa_info in, out;

   /* If its a start block, there may be more values at in than out. */
   if (start_block (bb)) {
       for (in=in_of_bb (bb); in; in=in->next) {
           for (out=out_of_bb (bb); out; out=out->next) {
               if (in->cs_index == out->cs_index)
                   break;
           }
           fprintf (dump_file, "\nCALLSTRING SEQUENCE : ");
           print_callstring_sequence (in->cs_index);
           fprintf (dump_file, "\nmayin : ");
           /*in_info = (in->pinfo < 0) ? NULL : VEC_index (pointsto_val, mayinfo, in->pinfo);*/
           print_pointsto_list (in->pinfo);
           fprintf (dump_file, "\nmayout : ");
           if (out) {
               /*out_info = (out->pinfo < 0) ? NULL : VEC_index (pointsto_val, mayinfo, out->pinfo);*/
               print_pointsto_list (out->pinfo);
           }
           else
               fprintf (dump_file, "value is mapped");
       }
   }

   /* If its an end block, there may be more values at out than in. */
   else if (end_block (bb)) {
       for (out=out_of_bb (bb); out; out=out->next) {
           for (in=in_of_bb (bb); in; in=in->next) {
               if (out->cs_index == in->cs_index)
                   break;
           }
           fprintf (dump_file, "\nCALLSTRING SEQUENCE : ");
           print_callstring_sequence (out->cs_index);
           fprintf (dump_file, "\nmayin : ");
	   if (in) {
               /*in_info = (in->pinfo < 0) ? NULL : VEC_index (pointsto_val, mayinfo, in->pinfo);*/
               print_pointsto_list (in->pinfo);
           }
           else
               fprintf (dump_file, "value is mapped");
           fprintf (dump_file, "\nmayout : ");
           /*out_info = (out->pinfo < 0) ? NULL : VEC_index (pointsto_val, mayinfo, out->pinfo);     */
	   print_pointsto_list (out->pinfo);
       }
   }
   else {
       for (in=in_of_bb (cbb); in; in=in->next) {
           for (out=out_of_bb (bb); out; out=out->next) {
               if (in->cs_index == out->cs_index)
                   break;
           }
           fprintf (dump_file, "\nCALLSTRING SEQUENCE : ");
           print_callstring_sequence (in->cs_index);
           fprintf (dump_file, "\nmayin : ");
           /*in_info = (in->pinfo < 0) ? NULL : VEC_index (pointsto_val, mayinfo, in->pinfo);*/
           print_pointsto_list (in->pinfo);
	   fprintf (dump_file, "\nmayout : ");
           if (out) {
               /*out_info = (out->pinfo < 0) ? NULL : VEC_index (pointsto_val, mayinfo, out->pinfo);*/
               print_pointsto_list (out->pinfo);
           }
           else
               fprintf (dump_file, "value is mapped");
       }
   }
   fprintf (dump_file, "\n");
}

static void
print_liveness_info (basic_block bb)
{
   csdfa_info in, out;
   /* If its a start block, there may be more values at in than out. */
   if (start_block (bb)) {
       for (in=in_of_bb (bb); in; in=in->next) {
           for (out=out_of_bb (bb); out; out=out->next) {
	       if (out->cs_index == in->cs_index) 
                   break;
           }
	   fprintf (dump_file, "\nCALLSTRING SEQUENCE : ");
           print_callstring_sequence (in->cs_index);
           fprintf (dump_file, "\nliveout : ");
           if (out) {
               /*out_info = (out->linfo < 0) ? NULL : VEC_index (df_list, liveinfo, out->linfo);*/
               print_df_list (out->linfo);
           }
 	   else 
	       fprintf (dump_file, "value is mapped"); 
           fprintf (dump_file, "\nlivein : ");
           /*in_info = (in->linfo < 0) ? NULL : VEC_index (df_list, liveinfo, in->linfo);*/
           print_df_list (in->linfo);
       }
   }      
   
   /* If its an end block, there may be more values at out than in. */
   else if (end_block (bb)) {
       for (out=out_of_bb (bb); out; out=out->next) {
           for (in=in_of_bb (bb); in; in=in->next) {
               if (out->cs_index == in->cs_index) 
                   break;
           }
           fprintf (dump_file, "\nCALLSTRING SEQUENCE : ");
           print_callstring_sequence (out->cs_index);
           fprintf (dump_file, "\nliveout : ");
           /*out_info = (out->linfo < 0) ? NULL : VEC_index (df_list, liveinfo, out->linfo);*/
           print_df_list (out->linfo);
           fprintf (dump_file, "\nlivein : ");
           if (in) {
               /*in_info = (in->linfo < 0) ? NULL : VEC_index (df_list, liveinfo, in->linfo);*/
               print_df_list (in->linfo);
           }
           else
               fprintf (dump_file, "value is mapped"); 
       }
   }
   else {
       for (out=out_of_bb (bb); out; out=out->next) {
           for (in=in_of_bb (bb); in; in=in->next) {
               if (out->cs_index == in->cs_index)
                   break;
           }
           fprintf (dump_file, "\nCALLSTRING SEQUENCE : ");
           print_callstring_sequence (out->cs_index);
           fprintf (dump_file, "\nliveout : ");
           /*out_info = (out->linfo < 0) ? NULL : VEC_index (df_list, liveinfo, out->linfo);*/
           print_df_list (out->linfo);
           fprintf (dump_file, "\nlivein : ");
           if (in) {
               /*in_info = (in->linfo < 0) ? NULL : VEC_index (df_list, liveinfo, in->linfo);*/
               print_df_list (in->linfo);
           }
           else
               fprintf (dump_file, "value is mapped");
       }
   } 
   fprintf (dump_file, "\n");
}  
    
/* Function to print the in and out pointsto information of a basic block. */
static void
print_bb_pinfo (basic_block bb)
{
   if (dump_file) {
       fprintf (dump_file, "\n\t\t\t\tpointer information for %s function, basic block no %d:\n",
           cgraph_node_name (cnode_of_bb (bb)), bb->index);
       print_pointsto_info (bb);
   }
}

static void
print_bb_linfo (basic_block bb)
{
   if (dump_file) {
       fprintf (dump_file, "\n\t\t\t\tliveness information for %s function, basic block no %d:\n",
           cgraph_node_name (cnode_of_bb (bb)), bb->index);
       print_liveness_info (bb);
   }
}

static void
print_pcsmap_at_startblock (basic_block bb)
{
   cs_map ai;
   if (!pcsmap_required (cnode_of_bb (bb)))
       fprintf (dump_file, "\nNo pointsto map required at %s node's %d block \n",
                cgraph_node_name (cnode_of_bb (bb)), bb->index);
   else {
       fprintf (dump_file, "\npointsto mapping performed at %s node's %d block:\n",
                cgraph_node_name (cnode_of_bb (bb)), bb->index);
       for (ai=pcsmap_of_cnode (cnode_of_bb (bb)); ai; ai=ai->next) {
           csmap_rhs bi = ai->rhs;
           print_callstring_sequence (ai->lhs->cs_index);
           fprintf (dump_file, " --> ");
           if (bi == NULL)
               fprintf (dump_file, "Empty");
           else {
               for (; bi; bi=bi->next) {
                   print_callstring_sequence (bi->rhsel->cs_index);
                   if (bi->next)
                       fprintf (dump_file, "  ,  ");
               }
           }
           fprintf (dump_file, "\n");
       }
   }
}

static void
print_lcsmap_at_endblock (basic_block bb)
{
   cs_map ai;
   if (!lcsmap_required (cnode_of_bb (bb)))
       fprintf (dump_file, "\nNo liveness mapping required at %s node's %d block \n",
                cgraph_node_name (cnode_of_bb (bb)), bb->index);
   else {
       fprintf (dump_file, "\nliveness mapping at %s node's %d block:\n",
                cgraph_node_name (cnode_of_bb (bb)), bb->index);
       for (ai=lcsmap_of_cnode (cnode_of_bb (bb)); ai; ai=ai->next) {
           csmap_rhs bi = ai->rhs;
           print_callstring_sequence (ai->lhs->cs_index);
           fprintf (dump_file, " --> ");
           if (bi == NULL)
               fprintf (dump_file, "Empty");
           else {
               for (; bi; bi=bi->next) {
                   print_callstring_sequence (bi->rhsel->cs_index);
                   if (bi->next)
                       fprintf (dump_file, "  ,  ");
               }
           }
           fprintf (dump_file, "\n");
       }
   }
}

static void
print_pta_worklist (void)
{
   worklist ai;
   bb_worklist list = worklist_of_cnode (cnode);
   int i;
   int num_bb = n_basic_blocks_for_function (DECL_STRUCT_FUNCTION (cnode->decl)) - NUM_FIXED_BLOCKS;
   fprintf (dump_file, "\nPrinting pointer function worklist:  ");
   for (ai=start; ai->next; ai=ai->next) 
       fprintf (dump_file, "%s,  ", cgraph_node_name (ai->called_fn));
   fprintf (dump_file, "%s ", cgraph_node_name (ai->called_fn));

   fprintf (dump_file, "\nPrinting current pointer bb worklist:  ");
   for (i=0; i<=num_bb; i++) {
       if (list[i].process_pta)
           fprintf (dump_file, "%s.%d ", cgraph_node_name (cnode), list[i].bb->index);
   }
   fprintf (dump_file, "\n");
}

static void
print_lv_worklist (void)
{
   worklist ai;
   bb_worklist list = worklist_of_cnode (cnode);
   int i;
   int num_bb = n_basic_blocks_for_function (DECL_STRUCT_FUNCTION (cnode->decl)) - NUM_FIXED_BLOCKS;
   fprintf (dump_file, "\nPrinting liveness function worklist:  ");
   for (ai=start; ai->next; ai=ai->next) 
       fprintf (dump_file, "%s,  ", cgraph_node_name (ai->called_fn));
   fprintf (dump_file, "%s ", cgraph_node_name (ai->called_fn));
   
   fprintf (dump_file, "\nPrinting current liveness bb worklist:  ");
   for (i=num_bb; i>=0; i--) {
       if (list[i].process_lv)
           fprintf (dump_file, "%s.%d ", cgraph_node_name (cnode), list[i].bb->index);
   }
   fprintf (dump_file, "\n");
}

/*--------------------------------------------------------------------------------
   Copy the in pointsto information for a basic block to the out information.
   After copying, reset the 'new_ptadf' flag for each points-to information.
   ------------------------------------------------------------------------------*/
static void
copy_in_to_out (void)
{
   csdfa_info ai, bi=NULL;
   for (ai=in_of_bb (cbb); ai; ai=ai->next) {
       if (ai->new_ptadf) {
           csdfa_info prev = NULL;
           for (bi=out_of_bb (cbb); bi; bi=bi->next) {
               if (ai->cs_index >= bi->cs_index)
                   break;
               prev = bi;
           }
           if (bi && (bi->cs_index == ai->cs_index)) {
               bi->pinfo = ai->pinfo;
               bi->new_ptadf = true;
           }
           else {
               csdfa_info temp = copy_csdfa_info (ai);
               if (prev == NULL) {
                  temp->next = out_of_bb (cbb);
                  ((bb_csipdfa_info)(cbb->aux))->out_ipdfa = temp;
               }
               else {
                   temp->next = prev->next;
                   prev->next = temp;
               }
           }
           ai->new_ptadf = false;
       }
   }
}

/* Copy lout to lin. */
static void
copy_out_to_in (void)
{
   csdfa_info ai, bi=NULL;
   for (ai=out_of_bb (cbb); ai; ai=ai->next) {
       if (ai->new_lvdf) {
           csdfa_info prev = NULL;
           for (bi=in_of_bb (cbb); bi; bi=bi->next) {
               if (ai->cs_index >= bi->cs_index)
                   break;
               prev = bi;
           }
           if (bi && (bi->cs_index == ai->cs_index)) {
               bi->linfo = ai->linfo;
               bi->new_lvdf = true;
           }
           else {
               csdfa_info temp = copy_csdfa_info (ai);
               if (prev == NULL) {
                  temp->next = in_of_bb (cbb);
                  ((bb_csipdfa_info)(cbb->aux))->in_ipdfa = temp;
               }
               else {
                   temp->next = prev->next;
                   prev->next = temp;
               }
           }
           ai->new_lvdf = false;
       }
   }
}

static void
copy_out_to_in_1 (void)
{
   csdfa_info ai, bi=NULL;
   for (ai=out_of_bb (cbb); ai; ai=ai->next) {
           csdfa_info prev = NULL;
           for (bi=in_of_bb (cbb); bi; bi=bi->next) {
               if (ai->cs_index >= bi->cs_index)
                   break;
               prev = bi;
           }
           if (bi && (bi->cs_index == ai->cs_index)) {
               bi->linfo = ai->linfo;
               bi->new_lvdf = true;
           }
           else {
               csdfa_info temp = copy_csdfa_info (ai);
               if (prev == NULL) {
                  temp->next = in_of_bb (cbb);
                  ((bb_csipdfa_info)(cbb->aux))->in_ipdfa = temp;
               }
               else {
                   temp->next = prev->next;
                   prev->next = temp;
               }
           }
           ai->new_lvdf = false;
   }
}

/* Create the RHS of a pointsto data structure. */
static bitmap
create_pointsto_val_rhs (unsigned int val)
{
   	bitmap tmp = BITMAP_ALLOC(NULL);
        bitmap_set_bit(tmp,val);
	return tmp;
}

/* Prepend undef to the RHS value if it contains only one 
   element (which is not undef). */
/*static bitmap
prepend_undef_to_rhs (bitmap a)
{
   if(bitmap_count_bits(a)==1)
   {
	bitmap_iterator bi;
	unsigned int bit;
	EXECUTE_IF_SET_IN_BITMAP(a, 0 , bit, bi){
		if(bit != undef_id)
		   bitmap_set_bit(a,undef_id);		
	}

   }
   return a;
}*/

/* Return a copy of the given RHS of pointsto information. */

/* Return a copy of normalized RHS. The RHS can be 
{undef, a, b} in which case, the returned RHS will 
be {a, b} */
static bitmap
copy_pointsto_rhs_1 (bitmap from)
{

	bitmap to = BITMAP_ALLOC(NULL);
	from = normalize_rhs_val(from);
	bitmap_copy(to,from);
	return to;

}





/* Remove unknown constraints from a and return
the resulting copy. */
static int
copy_pointsto_val_1 (int index,vec_list list)
{
  int root = -2;
  bitmap pointee, lhs_bitmap;
  bitmap_iterator bir;
  unsigned int var;

  lhs_bitmap = get_bitmap_for_lhs(index,list);

  EXECUTE_IF_SET_IN_BITMAP(lhs_bitmap,0,var,bir) {
  if(has_only_undef_pointee(index,list,var)){
           continue;
     }
        
     pointee = get_pointee_set(index,list,var);
     root = add_pointee_set(root,list,var,pointee);
 }

 return root;
  /* int size = size_of_pointsto_set(index,list);

   while(i<size)
   {
	unsigned int var = get_var_at_index_in_pointsto_set(index,list,i);
	if(has_only_undef_pointee(index,list,var)){
		i++;
		continue;
	}
	
		bitmap pointee = get_pointee_set(index,list,var);
		root = add_pointee_set(root,list,var,pointee);

	i++;
   }
return root;*/
}


/* Remove unknown constraints and normalize RHS. This function 
is invoked for phi nodes, where you need to normalize RHS 
becasue you can have cases where RHS is {undef, a, b} */
static int
copy_pointsto_val_2 (int index, vec_list list)
{
   int root = -2;
  bitmap lhs_bitmap, pointee, tmp;
  bitmap_iterator bir;
  unsigned int var;
  lhs_bitmap = get_bitmap_for_lhs(index,list);

  EXECUTE_IF_SET_IN_BITMAP(lhs_bitmap,0,var,bir) {
  if(has_only_undef_pointee(index,list,var)){
           continue;
     }

     	tmp = get_pointee_set(index,list,var);
        pointee = copy_pointsto_rhs_1 (tmp);
	root = add_pointee_set(root,list,var,pointee);
 }

 return root;

}




static int
compute_geninfo (int index, unsigned int lhs, unsigned int rhs)
{

   bitmap pointee_set = BITMAP_ALLOC(NULL);
   bitmap_set_bit(pointee_set,rhs);

   return add_pointee_set(index, tmplist, lhs, pointee_set);

} 


static bool
mark_live_at_endblock (basic_block bb, unsigned int lhs)
{

   csdfa_info ai;
   bool out_changed = false;
   int old_linfo;
   for (ai=out_of_bb (bb); ai; ai=ai->next) {
       if (ai->cs_index == cpinfo->cs_index) {
           /*df_list out = (ai->linfo < 0) ? NULL : copy_df_list (VEC_index (df_list, liveinfo, ai->linfo));*/
           old_linfo = ai->linfo;
           add_to_df_list (lhs, &(ai->linfo));
           /*ai->linfo = compute_liveness_index (out);*/
           if (ai->linfo != old_linfo) {
               modify_lcsmap_at_endblock (cnode, ai, false);
               out_changed = true;
               ai->new_lvdf = true;
           }
       }
   }
   return out_changed;
}

/* lhs is of type scalar and rhs is address. Reduce lhs and rhs 
   to structure equivalent if they have a non-zero offset. */
static bool 
pta_lhs_scalar_rhs_addr (int *gen, unsigned int lhsvar, 
        HOST_WIDE_INT lhsoffset, unsigned int rhsvar, HOST_WIDE_INT rhsoffset) 
{
   csvarinfo_t lhs_vi=NULL, rhs_vi=NULL;
   bool kill = true;

   /* Return if LHS is not a proper variable.*/
   if (!is_proper_var (lhsvar))
       return true;

   if (!is_proper_var (rhsvar))
       rhsoffset = 0;
 
   /* Reduce LHS if possible. */
   if (!unexpandable_var (lhsvar, lhsoffset)) {
       lhs_vi = cs_first_vi_for_offset (cs_get_varinfo (lhsvar), lhsoffset);
       if (lhs_vi) {
           lhsvar = lhs_vi->id;
           lhsoffset = 0;
       }
   }
   /* Reduce RHS if possible. */
   if (!unexpandable_var (rhsvar, rhsoffset)) {
       rhs_vi = cs_first_vi_for_offset (cs_get_varinfo (rhsvar), rhsoffset);
       if (rhs_vi) { 
           rhsvar = rhs_vi->id;
	   rhsoffset = 0;
       }
   }

   /* Return if LHS equals RHS. This can happen due to pointer arithmetic. */
   if (element_equal (lhsvar, rhsvar))
       return true;
   /* If lhsoffset is not 0, reset kill flag. */
   if (lhsoffset != 0)
       kill = false;
   /* If rhsoffset is not zero, generate undef rhs. */
   if (rhsoffset != 0)
       *gen = compute_geninfo (*gen, lhsvar, undef_id); 

   /* If the LHS is not defined in the current function, mark it as live
      at the endblock. */
   if (!compute_only_pinfo) {
       if (!(var_defined_in_cfun (lhsvar) || parm_decl (lhsvar) || global_var (lhsvar))) {
           basic_block endbb = end_bb_of_fn (cnode);
           bool out_changed = mark_live_at_endblock (endbb, lhsvar);
           if (out_changed) {
               set_bb_to_process_lv (cnode, endbb);
               set_in_lv_worklist (cnode);
               recompute_liveness = true;
           }
       }
   }

   *gen = compute_geninfo (*gen, lhsvar, rhsvar);
   return kill;
}





/* lhs is scalar and rhs is scalar. Deref rhs by one level and reduce it to
   the first case. */

static bool 
pta_lhs_scalar_rhs_scalar (int index, int *gen, 
          struct constraint_expr lhs, struct constraint_expr rhs)
{
   /*pointsto_val in = (index < 0) ? NULL :
                         copy_pointsto_val (VEC_index (pointsto_val, tmpinfo, index));
   pointsto_val ai;*/
   unsigned int bit_num;
   bool kill = true, rhs_found = false;

   bitmap pointee_set = BITMAP_ALLOC(NULL);
   bitmap_iterator pset;
   pointee_set = get_pointee_set(index,tmplist,rhs.var);
   if(is_x_in_pointsto_set(index,tmplist,rhs.var))
	rhs_found = true;  
  
   EXECUTE_IF_SET_IN_BITMAP(pointee_set,0,bit_num,pset) {
         kill = pta_lhs_scalar_rhs_addr (gen, lhs.var, lhs.offset, bit_num, rhs.offset);
   }

   if (!rhs_found)
       kill = pta_lhs_scalar_rhs_addr (gen, lhs.var, lhs.offset, undef_id, 0);

   return kill;
}



/* lhs is scalar and rhs is deref. Deref rhs two levels and reduce to the first
   case. After the first deref, try to reduce to an equivalent structure deref
   if the offset is not NULL. */
static bool 
pta_lhs_scalar_rhs_deref (int index, int *gen,
                      struct constraint_expr lhs, struct constraint_expr rhs)
{
   /*pointsto_val ai, ci=NULL;*/
   bool rhs_found = false, kill = true;
   unsigned int bit_num1;
/*   pointsto_val in = (index < 0) ? NULL :
                         copy_pointsto_val (VEC_index (pointsto_val, tmpinfo, index));*/

   bitmap pointee_set1 = BITMAP_ALLOC(NULL), pointee_set2 = BITMAP_ALLOC(NULL);
   bitmap_iterator bmpi1;
  
   pointee_set1 = get_indirect_pointee_set(index,tmplist,rhs.var); 
   pointee_set2 = get_pointee_set(index,tmplist,rhs.var);

/* Look into : why is_x_in_pointsto_set */
   EXECUTE_IF_SET_IN_BITMAP (pointee_set2,0,bit_num1,bmpi1)	
   {
          if(is_x_in_pointsto_set(index,tmplist,bit_num1))
                {
		        rhs_found = true;
			break;
		}
		
   }

   EXECUTE_IF_SET_IN_BITMAP (pointee_set1,0,bit_num1,bmpi1)
   {
              kill = pta_lhs_scalar_rhs_addr (gen, lhs.var, lhs.offset, bit_num1, rhs.offset);

   }
   if (!rhs_found)
       kill = pta_lhs_scalar_rhs_addr (gen, lhs.var, lhs.offset, undef_id, 0);

   return kill;
}

/* lhs is deref and rhs is addressof. Indirect lhs one level and
   reduce it to the first case. */

static bool 
pta_lhs_deref_rhs_addr (int index, int *gen, 
                         struct constraint_expr lhs, struct constraint_expr rhs)
{
   bool kill = true;
   unsigned int bit_num;
   bitmap pointee_set;
   bitmap_iterator pset;
   /*pointsto_val in = (index < 0) ? NULL :
                         copy_pointsto_val (VEC_index (pointsto_val, tmpinfo, index));*/

   
   if(!has_must_pointee(index,tmplist,lhs.var))
          kill = false;


        pointee_set = BITMAP_ALLOC(NULL);
   	pointee_set = get_pointee_set(index,tmplist,lhs.var);
   	EXECUTE_IF_SET_IN_BITMAP(pointee_set,0,bit_num,pset) {
                     kill &= pta_lhs_scalar_rhs_addr (gen, bit_num, lhs.offset, rhs.var, rhs.offset);
	}
    
   return kill;
}

/* lhs is deref and rhs is scalar. Deref lhs and rhs one level and reduce to 
   the first case. */


static bool 
pta_lhs_deref_rhs_scalar (int index, int *gen,  
                      struct constraint_expr lhs, struct constraint_expr rhs)
{
   bool kill = true, rhs_found = false;

  unsigned int bit_num, bit_num1;
  bitmap pointee_set = BITMAP_ALLOC(NULL),pointee_set1;
  bitmap_iterator pset ,pset1;
         
	

  pointee_set = get_pointee_set(index,tmplist,lhs.var);

  if(is_x_in_pointsto_set(index,tmplist,rhs.var))
	rhs_found = true;

   if(!has_must_pointee(index,tmplist,lhs.var))
   	kill = false;

   EXECUTE_IF_SET_IN_BITMAP(pointee_set,0,bit_num,pset) {
	if(rhs_found) {
		pointee_set1 = get_pointee_set(index,tmplist,rhs.var);
         
		EXECUTE_IF_SET_IN_BITMAP(pointee_set1,0,bit_num1,pset1) {
			kill &= pta_lhs_scalar_rhs_addr (gen, bit_num, lhs.offset, bit_num1, rhs.offset);		
		}
	}
	else if(kill) 
                kill &= pta_lhs_scalar_rhs_addr (gen, bit_num, lhs.offset, undef_id, 0);
   }
 
   return kill;
}


/* lhs is deref and rhs is deref. Deref lhs to one level and rhs to two
   levels and recude to first case. */

static bool 
pta_lhs_deref_rhs_deref (int index, int *gen, 
                  struct constraint_expr lhs, struct constraint_expr rhs)
{

   bool kill = true, lhs_found = false ,rhs_found = false;
   unsigned int bit_num, bit_num1, lhs_bit_num;
   bitmap_iterator pseti,pseti2;
   bitmap pointee_set , lhs_pointee_set;
   if(!has_must_pointee(index,tmplist,lhs.var))
	kill = false;
  
  if(is_x_in_pointsto_set(index,tmplist, lhs.var))
	lhs_found = true;


   if(lhs_found){
         rhs_found = false;
         pointee_set = get_pointee_set(index,tmplist,rhs.var);
         EXECUTE_IF_SET_IN_BITMAP(pointee_set,0,bit_num,pseti) {
		if(is_x_in_pointsto_set(index,tmplist,bit_num))
		{
			rhs_found = true;
			break;
		}
         }

        	pointee_set = get_indirect_pointee_set(index,tmplist,bit_num);
	        EXECUTE_IF_SET_IN_BITMAP(pointee_set,0,bit_num1,pseti) {
			lhs_pointee_set = get_pointee_set(index,tmplist,lhs.var);
         		EXECUTE_IF_SET_IN_BITMAP(lhs_pointee_set,0,lhs_bit_num,pseti2) {
                                   kill &= pta_lhs_scalar_rhs_addr (gen, lhs_bit_num, lhs.offset, bit_num1, rhs.offset);
			}
		}
    
       if (!rhs_found && kill){	
	 	lhs_pointee_set = get_pointee_set(index,tmplist,lhs.var);
		 EXECUTE_IF_SET_IN_BITMAP(lhs_pointee_set,0, lhs_bit_num, pseti2){
          			 kill &= pta_lhs_scalar_rhs_addr (gen, lhs_bit_num, lhs.offset, undef_id, 0);
		}
	}
   }
   return kill;
} 



static bitmap 
get_kill_pointsto_set(int gen_1)
{

	return get_bitmap_for_lhs(gen_1,tmplist);
	

}


static bitmap
get_gen_pointsto_set(int gen)
{
	return get_bitmap_for_lhs(gen,tmplist);	


}



	/*--------------------------------------------------------------------------------
  Function to compute the out information, given the in, GEN and KILL information.
  We use the basic data flow equation, out = (in - KILL) U GEN. The out value is
  again sorted like GEN. 
  -------------------------------------------------------------------------------*/

static int
compute_pointsto_out (int index, int gen_1, bool kill)
{
   
  bitmap kill_set, gen_set , pointee_set;
  bitmap_iterator bis;
  unsigned int kill_var,gen_var;


  kill_set = get_kill_pointsto_set(gen_1);
  gen_set =  kill_set;
  /* Perform in = (in - KILL) */
if(kill) {
      EXECUTE_IF_SET_IN_BITMAP(kill_set,0,kill_var,bis) {
               index = remove_pointee_set(index, tmplist,kill_var);
   }

 }
   /* Perform (in - KILL) U GEN */
   if (is_pointee_list_empty(index)){ 
	index = copy_pointsto_val_1(gen_1,tmplist);
   return index;
   }
   gen_set = kill_set;
   EXECUTE_IF_SET_IN_BITMAP(gen_set,0,gen_var,bis) {
		pointee_set = get_pointee_set(gen_1,tmplist,gen_var);
        	if(bitmap_single_bit_set_p(pointee_set)){
			 if(bitmap_first_set_bit (pointee_set) == undef_id)
				return index;
		}

               index = add_pointee_set(index,tmplist, gen_var, pointee_set);
   }
   return index;
}



/*--------------------------------------------------------------------------------
  Function to compute the out information, given the in, GEN and KILL information.
  We use the basic data flow equation, out = (in - KILL) U GEN. The out value is
  again sorted like GEN. This function is invoked by PHI nodes. 
  -------------------------------------------------------------------------------*/

static int
compute_pointsto_out_1 (int index, int gen_1, bool kill)
{

   bitmap_iterator bis;
   unsigned int kill_var,gen_var;
   bitmap kill_set = get_kill_pointsto_set(gen_1);
   bitmap gen_set = kill_set;

  /* Perform in = (in - KILL) */
  if(kill) {
     EXECUTE_IF_SET_IN_BITMAP(kill_set,0,kill_var,bis) {
               index = remove_pointee_set(index, tmplist,kill_var);
   }
  } 

   /* Perform (in - KILL) U GEN */
   if (is_pointee_list_empty(index)){
	return copy_pointsto_val_2(gen_1,tmplist);
   }
 

    gen_set = kill_set;
   EXECUTE_IF_SET_IN_BITMAP(gen_set,0,gen_var,bis) {



	if(has_only_undef_pointee(gen_1,tmplist,gen_var))
			return index;
              index = add_pointee_set(index,tmplist, gen_var, get_pointee_set(gen_1,tmplist,gen_var));
   }
 

   return index;

}



static int
compute_out_index (int in, int gen, bool kill)
{

   int idx = compute_pointsto_out (in, gen, kill);
   return compute_pointsto_index_from_templist (idx);

}


static int
compute_out_index_1 (int in_index, int gen_index, bool kill)
{
   if (in_index == gen_index)
       return in_index;
   else {
	int tmpin = copy_pointsto_set_to_temp(in_index);
	int tmpgen = copy_pointsto_set_to_temp(gen_index);
	int in = compute_pointsto_out (tmpin, tmpgen, kill);
        return compute_pointsto_index_from_templist (in);
    
   }
}





/*----------------------------------------------------------------------------------
  Function to compute the MAY out information for each alias. 
       Constraint            Pointsto generated
         a = &b                   (a->b)
         a = b                    (a->c) where (b->c) belongs to in
        a = *b                   (a->d) where (b->c, c->d) belongs to in
        *a = &b                   (c->b) where (a->c) belongs to in
        *a = b                    (c->d) where (a->c, b->d) belongs to in
       *a = *b                   (c->e) where (a->c, b->d, d->e) belongs to in
  ---------------------------------------------------------------------------------*/
static int
compute_stmt_out (int index, constraint_t t)
{
   struct constraint_expr rhs = t->rhs;
   struct constraint_expr lhs = t->lhs;
/*   pointsto_val gen=NULL;*/
  int gen=-2;

   bool kill = true;

   /* If the LHS type is scalar, then the rhs can be any of the three: */
   if (lhs.type == SCALAR) {
       if (rhs.type == ADDRESSOF) /* x = &y */
           pta_lhs_scalar_rhs_addr (&gen, lhs.var, lhs.offset, rhs.var, rhs.offset);
       if (rhs.type == SCALAR) /* x = y */
           pta_lhs_scalar_rhs_scalar (index, &gen, lhs, rhs);
       if (rhs.type == DEREF) /* x = *y */ 
          pta_lhs_scalar_rhs_deref (index, &gen, lhs, rhs);
   }
   /* If the LHS type is deref, then the rhs can be any of the three: */
   if (lhs.type == DEREF) {
       if (rhs.type == ADDRESSOF) /* *x = &y */
           kill = pta_lhs_deref_rhs_addr (index, &gen, lhs, rhs);
       if (rhs.type == SCALAR) /* *x = y */
           kill = pta_lhs_deref_rhs_scalar (index, &gen, lhs, rhs);
       if (rhs.type == DEREF) /* *x = *y */
           kill = pta_lhs_deref_rhs_deref (index, &gen, lhs, rhs);
   }

 index = compute_pointsto_out (index, gen, kill);
 return index;
}

/* ----------------------------------------------------------------------
   Compute the sorted confluence of the two mayin's. The confluence here 
   is union. The union is sorted, like in the case of GEN computation.
   Cases : 
	in (update) is NULL - prepend undef if required to each 
                 pointsto-val of update (in) and normalize the RHS.
        Otherwise find the confluence, normalize the RHS and then prepend
	undef if required. 
   ---------------------------------------------------------------------*/
static int
compute_in_confluence (int in_index, int update_index)
{
   #if 0
   int tmpin, tmpupdate;
   bitmap in_lhs, update_lhs, tmp_set, pointee_set;
   unsigned int invar;
   bitmap_iterator bi1;   

   if (in_index < 0) {
       if (update_index < 0)
	   return -1;
       else
	   return update_index;
   }
   if (in_index == update_index)
       return in_index;
  
   
 /*  int in = copy_pointsto_set_to_temp(in_index);*/
   tmpupdate = copy_pointsto_set_to_temp(update_index);
  tmpin = copy_pointsto_set_to_temp(in_index);


   in_lhs = get_bitmap_for_lhs(tmpin,tmplist);
   update_lhs = get_bitmap_for_lhs(tmpupdate,tmplist);
   
   EXECUTE_IF_SET_IN_BITMAP(in_lhs,0,invar,bi1) {
	if(bitmap_bit_p(update_lhs,invar))
	{
		bitmap_clear_bit(update_lhs,invar);
		pointee_set = get_pointee_set(tmpupdate,tmplist,invar);
		tmpin = add_pointee_set(tmpin,tmplist,invar,pointee_set);
	}
	else
	{
            tmp_set = get_pointee_set(tmpin,tmplist,invar);
            pointee_set  = prepend_undef_to_rhs (tmp_set);
	    tmpin = add_pointee_set(tmpin,tmplist,invar,pointee_set);

	}
   }

   EXECUTE_IF_SET_IN_BITMAP(update_lhs,0,invar,bi1) {
            tmp_set = get_pointee_set(tmpupdate,tmplist,invar);
            pointee_set  = prepend_undef_to_rhs (tmp_set);
            tmpin = add_pointee_set(tmpin,tmplist,invar,pointee_set);
   }

    return compute_pointsto_index_from_templist (tmpin);
   #endif  
  return merge_pointsto_info (in_index, update_index, maylist);

}


static void 
compute_in_confluence_1 (int *in_index, int *update_index)
{

        *in_index = merge_pointsto_info(*in_index,*update_index,tmplist);

}


static int 
compute_df_list_union (int lin, int gen_i)
{


   /*int size = get_live_set_size(gen_i);
   int i =0;
   while(i<size){*/
   bitmap live_set = get_live_set(gen_i);
   bitmap_iterator bir;
   unsigned int val;
   EXECUTE_IF_SET_IN_BITMAP(live_set,0,val,bir){
        lin = add_var_to_liveset(lin,val);
   }
	return lin;


   /*while (bi){

	lin = add_var_to_liveset(lin,bi->val);
        bi = bi->next;
   }*/
   /*while (bi) {
       if (ai == NULL)
           break;
       if (ai->val == bi->val) {
           prev_ai = ai;
           ai = ai->next;
           bi = bi->next;
           continue;
       }
       else if (ai->val > bi->val) {
           df_list tmp = create_df_constraint (bi->val);
           tmp->next = ai;
           if (prev_ai) {
               prev_ai->next = tmp;
               prev_ai = tmp;
           }
           else {
               prev_ai = in = tmp;
           }
           bi = bi->next;
           continue;
       }
       else if (ai->val < bi->val) {
           prev_ai = ai;
           ai = ai->next;
       }
    }
    if (bi) {
       df_list ci;
       gcc_assert (prev_ai);
       for (ci = bi; ci; ci=ci->next) {
           df_list tmp = create_df_constraint (ci->val);
           prev_ai->next = tmp;
           prev_ai = prev_ai->next;
       }
   }
   return compute_liveness_index(in);*/

/*   return merge_liveness_info(lin, gen_i);   */
}

static int 
compute_lout_union (int lin, int lupdate)
{
   
   /*if (lin == -2 && lupdate == -2)
       return -1;

   if (lin == -1 || lin == -2)
       return lupdate;

   if (lupdate == -1 || lin == lupdate)
       return lin;

   lin = compute_df_list_union (lin, lupdate);
   return lin;*/
	return merge_liveness_info(lin, lupdate);

}



static bool 
compute_phi_gen (int *in, constraint_t t, 
                 int *aggregate_gen_index, bool init)
{
   struct constraint_expr rhs = t->rhs;
   struct constraint_expr lhs = t->lhs;
   /*pointsto_val gen=NULL;*/
   int gen = -2;
   bool kill = true; 

   /* If the LHS type is scalar, then the rhs can be any of the three: */
   if (lhs.type == SCALAR) {
       if (rhs.type == ADDRESSOF) /* x = &y */
           pta_lhs_scalar_rhs_addr (&gen, lhs.var, lhs.offset, rhs.var, rhs.offset);
       if (rhs.type == SCALAR) /* x = y */
           pta_lhs_scalar_rhs_scalar (*in, &gen, lhs, rhs);
       if (rhs.type == DEREF) /* x = *y */
           pta_lhs_scalar_rhs_deref (*in, &gen, lhs, rhs);
   }
   /* If the LHS type is deref, then the rhs can be any of the three: */
   if (lhs.type == DEREF) {
       if (rhs.type == ADDRESSOF) /* *x = &y */
           kill = pta_lhs_deref_rhs_addr (*in, &gen, lhs, rhs);
       if (rhs.type == SCALAR) /* *x = y */
           kill = pta_lhs_deref_rhs_scalar (*in, &gen, lhs, rhs);
       if (rhs.type == DEREF) /* *x = *y */
           kill = pta_lhs_deref_rhs_deref (*in, &gen, lhs, rhs);
   }
   /* Compute aggregated mustgen and maygen. */
   if (init) 
       *aggregate_gen_index = gen;
   else  
       compute_in_confluence_1 (aggregate_gen_index, &gen);

   return kill;
}



static void
compute_stmt_out_1 (csdfa_info dfainfo, constraint_t t)
{
   struct constraint_expr rhs = t->rhs;
   struct constraint_expr lhs = t->lhs;
   /*pointsto_val in=NULL, gen=NULL;*/
   bool kill = true;
   int index=-2,gen =-2;

   /* copy out values to mayin and mustin */
   if (dfainfo->pinfo >= 0)
	index = copy_pointsto_set_to_temp(dfainfo->pinfo);
       /*in = copy_pointsto_val (VEC_index (pointsto_val, mayinfo, dfainfo->pinfo));*/

   if (lhs.type == SCALAR) {
       if (rhs.type == ADDRESSOF) /* x = &y */
           pta_lhs_scalar_rhs_addr (&gen, lhs.var, lhs.offset, rhs.var, rhs.offset);
       if (rhs.type == SCALAR) /* x = y */
           pta_lhs_scalar_rhs_scalar (index, &gen, lhs, rhs);
       if (rhs.type == DEREF) /* x = *y */
           pta_lhs_scalar_rhs_deref (index, &gen, lhs, rhs);
   }
   /* If the LHS type is deref, then the rhs can be any of the three: */
   if (lhs.type == DEREF) {
       if (rhs.type == ADDRESSOF) /* *x = &y */
           kill = pta_lhs_deref_rhs_addr (index, &gen, lhs, rhs);
       if (rhs.type == SCALAR) /* *x = y */
           kill = pta_lhs_deref_rhs_scalar (index, &gen, lhs, rhs);
       if (rhs.type == DEREF) /* *x = *y */
           kill = pta_lhs_deref_rhs_deref (index, &gen, lhs, rhs);
   }

   /* compute mayout */
   dfainfo->pinfo = compute_out_index (index, gen, kill);
    /*empty_temp_set();*/
   /*if (gen) ggc_free (gen);*/
}

static void
compute_stmt_out_2 (csdfa_info dfainfo, constraint_t t)
{
   struct constraint_expr rhs = t->rhs;
   struct constraint_expr lhs = t->lhs;
   /*pointsto_val in=NULL, gen=NULL;*/
   int index = -2, gen = -2;

   /* copy out values to mayin and mustin */
   if (dfainfo->pinfo >= 0)
        index = copy_pointsto_set_to_temp(dfainfo->pinfo);

/*   in = copy_pointsto_val (VEC_index (pointsto_val, mayinfo, dfainfo->pinfo));*/

   /* If the LHS type is scalar, then the rhs can be any of the three: */
   if (lhs.type == SCALAR) {
       if (rhs.type == ADDRESSOF) /* x = &y */
           pta_lhs_scalar_rhs_addr (&gen, lhs.var, lhs.offset, rhs.var, rhs.offset);
       if (rhs.type == SCALAR) /* x = y */
           pta_lhs_scalar_rhs_scalar (index, &gen, lhs, rhs);
       if (rhs.type == DEREF) /* x = *y */
           pta_lhs_scalar_rhs_deref (index, &gen, lhs, rhs);
   }
   /* If the LHS type is deref, then the rhs can be any of the three: */
   if (lhs.type == DEREF) {
       if (rhs.type == ADDRESSOF) /* *x = &y */
           pta_lhs_deref_rhs_addr (index, &gen, lhs, rhs);
       if (rhs.type == SCALAR) /* *x = y */
           pta_lhs_deref_rhs_scalar (index, &gen, lhs, rhs);
       if (rhs.type == DEREF) /* *x = *y */
           pta_lhs_deref_rhs_deref (index, &gen, lhs, rhs);
   }

   /* compute mayout */
   dfainfo->pinfo = compute_pointsto_index_from_templist (gen);
}


/* ---------------------------------------------------------------------------------------------
   Function to iterate over the alias list of the current basic block, and compute the pointsto
   information. If the 'all_contexts_together' flag is set, we iterate over all the
   pointsto values available at out of a bb. Else we iterate over the pointsto value pointed to
   by the cpinfo.
   ---------------------------------------------------------------------------------------------*/
static void 
find_pointsto_pair (void)
{
   csdfa_info ci;
   constraint_list bi; 
   int index=-2; 
  if (all_contexts_together) {
       for (ci=out_of_bb (cbb); ci; ci=ci->next) {
           if (ci->new_ptadf) {
               df_list ai;

		index = copy_pointsto_set_to_temp(ci->pinfo);

               cpinfo = ci;
		for (bi=get_constraint_start (cbb); bi; bi=bi->next) {

		   if (!bi->is_alias)
		       continue;
                   if (!bi->stmt_constraint_chain->next) {
			constraint_t temp = VEC_index (constraint_t, aliases,bi->stmt_constraint_chain->val);
                           index = compute_stmt_out (index, temp);
			 /*  index = compute_stmt_out_temp (ci->pinfo, temp);*/
                   }
                   /* We are dealing with constraints generated from PHI nodes. */
                   else {

			int maygen_ind = -2;
                       bool init = true, kill = true;

                      for (ai=bi->stmt_constraint_chain; ai; ai=ai->next) {

                           constraint_t temp = VEC_index (constraint_t, aliases, ai->val);
                           kill = compute_phi_gen (&index, temp, &maygen_ind, init);
                           init = false;

                       }
                       /* Compute must and may out. */
                       index = compute_pointsto_out_1 (index, maygen_ind, kill);
                       if (maygen_ind > -1) {maygen_ind = -2;}
			
                   }
               }
	
              ci->pinfo = compute_pointsto_index_from_templist (index);
	       /*empty_temp_set();*/
           }
       }
   }

   else {
       if (cpinfo->new_ptadf) {
           df_list ai;
           /*pointsto_val mayin;*/
           ci = cpinfo;
		index = copy_pointsto_set_to_temp(ci->pinfo);

           for (bi=get_constraint_start (cbb); bi; bi=bi->next) {
	       if (!bi->is_alias)
		   continue;
               if (!bi->stmt_constraint_chain->next) {
                         constraint_t temp = VEC_index (constraint_t, aliases,bi->stmt_constraint_chain->val);
                         index = compute_stmt_out (index, temp);
		}
		else {
                     /*pointsto_val maygen=NULL;*/
                        int maygen_ind = -2;
                       bool init = true, kill = true;

                      for (ai=bi->stmt_constraint_chain; ai; ai=ai->next) {
                           constraint_t temp = VEC_index (constraint_t, aliases, ai->val);
                           kill = compute_phi_gen (&index, temp, &maygen_ind, init);
                           init = false;

                       }
                       /* Compute must and may out. */
                       index = compute_pointsto_out_1 (index, maygen_ind, kill);
                       if (maygen_ind > -1) {maygen_ind = -2;};
 
                   }
           }
               ci->pinfo = compute_pointsto_index_from_templist (index);
	       /*empty_temp_set();*/
       }
   }
}


/* -------------------------------------------------------------------------
   Function to perform callstring mapping at the start of a function.
   We prefer choosing a representative which has already been processed, i.e
   whose 'new_ptadf' flag is set to false.
   ------------------------------------------------------------------------ */
static void
compute_mapped_out_at_startblock (void)
{
   csdfa_info ai, di;
   cs_map bi;
   /* copy the mapped points-to info to the outcsipdfa. */
   for (bi=pcsmap_of_cnode (cnode); bi; bi=bi->next) {
       ai = bi->lhs;
       if (ai->new_ptadf) {
           csdfa_info prev = NULL;
	   bi->present_in_out = true;

           for (di=out_of_bb (cbb); di; di=di->next) {
               if (ai->cs_index >= di->cs_index)
                   break;
               prev = di;
           }
           if (di && (di->cs_index == ai->cs_index)) {
               di->pinfo = ai->pinfo;
               di->new_ptadf = true;
           }
           else {
               csdfa_info ci = copy_csdfa_info (ai);
               if (prev == NULL) {
                   ci->next = out_of_bb (cbb);
                   ((bb_csipdfa_info)(cbb->aux))->out_ipdfa = ci;
               }
               else {
                   ci->next = prev->next;
                   prev->next = ci;
               }
           }
           if (!pcsmap_required (cnode))
               ai->new_ptadf = false;
       }
   }
   if (pcsmap_required (cnode)) {
       basic_block end_bb = end_bb_of_fn (cnode);
       if (end_block (end_bb))
           set_bb_to_process_pta (cnode, end_bb);
   }
   /* Dump the map in the dump_file */
   /*if (dump_file && !ipacs_time)
       print_pcsmap_at_startblock (cbb);*/
}

/* -------------------------------------------------------------------------
   Function to perform callstring mapping at the start of a function.
   We prefer choosing a representative which has already been processed, i.e
   whose 'new_lvdf' flag is set to false.
   ------------------------------------------------------------------------ */
static void
compute_mapped_in_at_endblock (void)
{
   csdfa_info ai, di;
   cs_map bi;

   /* copy the mapped live-to info to in. */
   for (bi=lcsmap_of_cnode (cnode); bi; bi=bi->next) {
       ai = bi->lhs;
       if (ai->new_lvdf == true) {
           //if(dump_file)// && !ipacs_time) 
		//{fprintf(dump_file,"\ncompute mapped in at endblock 1 \n");}
           csdfa_info prev = NULL;
	   bi->present_in_out = true;

           for (di=in_of_bb (cbb); di; di=di->next) {
               if (ai->cs_index >= di->cs_index)
                   break;
               prev = di;
           }
           if (di && (di->cs_index == ai->cs_index)) {
                //if(dump_file)// && !ipacs_time) 
			//{fprintf(dump_file,"\ncompute mapped in at endblock 2 \n");}
               di->linfo = ai->linfo;
               di->new_lvdf = true;
           }
           else {
               csdfa_info ci = copy_csdfa_info (ai);
               if (prev == NULL) {
                 //if(dump_file)// && !ipacs_time) 
			//{fprintf(dump_file,"\ncompute mapped in at endblock 3 \n");}
                   ci->next = in_of_bb (cbb);
                   ((bb_csipdfa_info)(cbb->aux))->in_ipdfa = ci;
               }
               else {
	           //if(dump_file)// && !ipacs_time) 
			//{fprintf(dump_file,"\ncompute mapped in at endblock 4 \n");}

                   ci->next = prev->next;
                   prev->next = ci;
               }
           }
           if (!lcsmap_required (cnode))
               ai->new_lvdf = false;
       }
   }
   if (lcsmap_required (cnode)) {
       basic_block start_bb = start_bb_of_fn (cnode);
       set_bb_to_process_lv (cnode, start_bb);
   }
   /* Dump the map in the dump_file */
   if (dump_file)//&& !ipacs_time)
       print_lcsmap_at_endblock (cbb);
}


/*static void
print_live(int index , char *s)
{

   df_list lin = (index < 0) ? NULL : VEC_index (df_list, liveinfo, index);
   if(dump_file && !ipacs_time) {
	fprintf(dump_file,"\n\n%s : ",s);
	print_df_list(lin);
   }

}*/


static int
compute_livevar_at_out (int outlive, int inlive)
{

   int lin = -1;
   bitmap in_set = get_live_set(inlive);
   bitmap out_set = get_live_set(outlive);
   bitmap_iterator bi;
   unsigned int var;
   bitmap result = BITMAP_ALLOC(NULL);
   bitmap_and (result, in_set, out_set);
   EXECUTE_IF_SET_IN_BITMAP(result,0,var,bi)
   {
	add_to_df_list(var, &lin);
   }
   
return lin;
}
	       

static int
calculate_actual_out (csdfa_info ai, csdfa_info bi)
{

   int lin = ai->linfo, pts = -2, size_lin;
   bitmap ai_lhs, pointee_set;
   bitmap_iterator bir;
   unsigned int var;

   if (bi) {
	lin = compute_livevar_at_out (ai->linfo, bi->linfo);

   }

	if(lin < 0)
		return -1;
   
    ai_lhs = get_bitmap_for_lhs(ai->pinfo,maylist);
     


    
    EXECUTE_IF_SET_IN_BITMAP(ai_lhs,0,var,bir){
	if(is_var_live(lin,var)){
            pointee_set = get_pointee_set(ai->pinfo,maylist,var);
	    pts = add_pointee_set(pts,tmplist,var,pointee_set);
	}
    }

    return compute_pointsto_index_from_templist (pts);
 
}

static int
calculate_actual_out_1 (csdfa_info ai, csdfa_info bi)
{

   bitmap ai_lhs, pointee_set;
   int pts = -2, pinfo = ai->pinfo , lin = -2;
   unsigned int var;
   bitmap_iterator bir;
   if(bi)
	lin = bi->linfo;

    if(lin < 0)
        return -1;

    ai_lhs = get_bitmap_for_lhs(pinfo,maylist);

    
    EXECUTE_IF_SET_IN_BITMAP(ai_lhs,0,var,bir){
        if(is_var_live(lin,var)){
            pointee_set = get_pointee_set(pinfo,maylist,var);
            pts = add_pointee_set(pts,tmplist,var,pointee_set);
        }
    }

    return compute_pointsto_index_from_templist (pts);

}


/* ------------------------------------------------------------------------
   Function to propagate all the pointsto information cells (whose new_ptadf
   is set) of a block to all its successors.    
   -----------------------------------------------------------------------*/
static void 
propagate_pointsto_to_all_succs (basic_block bb)
{
   edge e;
   edge_iterator ei;
   csdfa_info ai, bi;
   FOR_EACH_EDGE (e, ei, bb->succs) {
       basic_block succ_bb = e->dest;
       if (pta_bypass_block (succ_bb))
           propagate_pointsto_to_all_succs (succ_bb);
       else {
           bool set_succ_for_processing = false;
           for (ai=out_of_bb (cbb); ai; ai=ai->next) {
               if (ai->new_ptadf) {
		   int new_index;
                   csdfa_info prev = NULL;
                   for (bi=in_of_bb (succ_bb); bi; bi=bi->next) {
                       if (ai->cs_index >= bi->cs_index)
                           break;
                       prev = bi;
                   }
	        
 		   /*calculate_actual_out(cbb, bi->csindex)*/	
		   new_index = calculate_actual_out (ai, bi);
                   if (bi && (bi->cs_index == ai->cs_index)) {
                       /* calculate confluence of mustout and mayout. */
                       int old_pinfo = bi->pinfo;
                       bi->pinfo = compute_in_confluence (bi->pinfo, new_index);
                       /* If the must or may in values have changed, add the bb to worklist. */
                       if (old_pinfo != bi->pinfo) {
                           bi->new_ptadf = true;
                           set_succ_for_processing = true;
                       }
                   }
                   else {
                       csdfa_info temp = copy_csdfa_info (ai);
		       temp->pinfo = new_index;
                       if (prev == NULL) {
                           temp->next = in_of_bb (succ_bb);
                           ((bb_csipdfa_info)(succ_bb->aux))->in_ipdfa = temp;
                       }
                       else {
                           temp->next = prev->next;
                           prev->next = temp;
                       }
                       set_succ_for_processing = true;
                   }
               }
           }
           if (set_succ_for_processing) {
	       if (has_deref_stmt (succ_bb)) {
		   if (call_block (succ_bb))
		       set_bb_to_process_lv (cnode, single_succ (succ_bb));
		   else
		       set_bb_to_process_lv (cnode, succ_bb);
		   set_in_lv_worklist (cnode);
		   recompute_liveness = true; 
               }
               set_bb_to_process_pta (cnode, succ_bb);
           }
       }
   }
}

static void
propagate_liveness_to_all_preds (basic_block bb, bool recompute)
{
   edge e;
   edge_iterator ei;
   csdfa_info ai, bi;
// if(dump_file )//&& !ipacs_time) fprintf(dump_file,"\n***propagate liveness to all preds****\n");
   FOR_EACH_EDGE (e, ei, bb->preds) {
	 // if(dump_file)//&& !ipacs_time) fprintf(dump_file,"\n***propagate liveness to all preds 1****\n");
       bool pred_pta_set = false;
       basic_block pred_bb = e->src;
       if (lv_bypass_block (pred_bb))
           propagate_liveness_to_all_preds (pred_bb, recompute);
       else {
           bool set_pred_for_processing = false;
           for (ai=in_of_bb (cbb); ai; ai=ai->next) {
			  // if(dump_file) && (!ipacs_time) fprintf(dump_file,"\n***propagate liveness to all preds 2****\n");
               if (ai->new_lvdf) {
                   csdfa_info prev = NULL;
                   for (bi=out_of_bb (pred_bb); bi; bi=bi->next) {
 					  //if(dump_file) && (!ipacs_time) fprintf(dump_file,"\n***propagate liveness to all preds 3****\n");
                       if (ai->cs_index >= bi->cs_index)
                           break;
                       prev = bi;
                   }
                   if (bi && (bi->cs_index == ai->cs_index)) {
                       /* calculate of lout. */
                       int old_linfo = bi->linfo;
                       bi->linfo = compute_lout_union (bi->linfo, ai->linfo);
                       if (old_linfo != bi->linfo) {
						  // if(dump_file)// && !ipacs_time) 
							//fprintf(dump_file,"\n***propagate liveness to all preds 5****\n");
                           bi->new_lvdf = true;
                           set_pred_for_processing = true;
                       }
                       if (recompute) {
                           pred_pta_set = true;
                           bi->new_ptadf = true;
                       }

                   }
                   else {
					  // if(dump_file)// && !ipacs_time) 
						//fprintf(dump_file,"\n***propagate liveness to all preds 6****\n");
                       csdfa_info temp = copy_csdfa_info (ai);
                       if (prev == NULL) {
                           temp->next = out_of_bb (pred_bb);
                           ((bb_csipdfa_info)(pred_bb->aux))->out_ipdfa = temp;
                       }
                       else {
                           temp->next = prev->next;
                           prev->next = temp;
                       }
                       set_pred_for_processing = true;
                   }
               }
           }
           if (recompute && pred_pta_set)
               set_bb_to_process_pta (cnode, pred_bb);
           if (set_pred_for_processing)
               set_bb_to_process_lv (cnode, pred_bb);
       }
   }
 // if(dump_file)// && !ipacs_time) 
	//fprintf(dump_file,"\n***Exit propagate liveness to all pred****\n");
}

static void
regenerate_lv_at_in (basic_block bb, int rep_cs, csdfa_info ai)
{
   csdfa_info di, ei, prev_in = NULL;

   for (di=in_of_bb (bb); di; di=di->next) {
       if (rep_cs >= di->cs_index)
           break;
   }
   if (!di || di->cs_index != rep_cs)
       return;

   for (ei=in_of_bb (bb); ei; ei=ei->next) {
       if (ai->cs_index >= ei->cs_index)
           break;
       prev_in = ei;
   }
   if (ei && (ei->cs_index == ai->cs_index))
       ei->linfo = di->linfo;
   else {
       csdfa_info temp = copy_csdfa_info (ai);
       temp->pinfo = -2;
       temp->linfo = di->linfo;
       temp->new_lvdf = temp->new_ptadf = false;
       if (prev_in == NULL) {
           temp->next = in_of_bb (cbb);
           ((bb_csipdfa_info)(cbb->aux))->in_ipdfa = temp;
       }
       else {
           temp->next = prev_in->next;
           prev_in->next = temp;
       }
   }
}

static void
regenerate_pta_at_in (basic_block bb, int rep_cs, csdfa_info ai)
{
   csdfa_info di, ei, prev_in = NULL;

   for (di=in_of_bb (bb); di; di=di->next) {
       if (rep_cs >= di->cs_index)
           break;
   }
   if (!di || di->cs_index != rep_cs)
       return;

   for (ei=in_of_bb (bb); ei; ei=ei->next) {
       if (ai->cs_index >= ei->cs_index)
           break;
       prev_in = ei;
   }
   if (ei && (ei->cs_index == ai->cs_index))
       ei->pinfo = di->pinfo;
   else {
       csdfa_info temp = copy_csdfa_info (ai);
       temp->pinfo = di->pinfo;
       temp->linfo = -2;
       temp->new_ptadf = temp->new_lvdf = false;
       if (prev_in == NULL) {
           temp->next = in_of_bb (cbb);
           ((bb_csipdfa_info)(cbb->aux))->in_ipdfa = temp;
       }
       else {
           temp->next = prev_in->next;
           prev_in->next = temp;
       }
   }
}

static void
regenerate_lv_at_out (basic_block bb, int rep_cs, csdfa_info ai)
{
   csdfa_info di, ei, prev_out = NULL;

   for (di=out_of_bb (bb); di; di=di->next) {
       if (rep_cs >= di->cs_index)
           break;
   }
   if (!di || di->cs_index != rep_cs)
       return;

   for (ei=out_of_bb (bb); ei; ei=ei->next) {
       if (ai->cs_index >= ei->cs_index)
           break;
           prev_out = ei;
   }
   if (ei && (ei->cs_index == ai->cs_index))
       ei->linfo = di->linfo;
   else {
       csdfa_info temp = copy_csdfa_info (ai);
       temp->pinfo = -2;
       temp->linfo = di->linfo;
       temp->new_lvdf = temp->new_ptadf = false;
       if (prev_out == NULL) {
           temp->next = out_of_bb (cbb);
           ((bb_csipdfa_info)(cbb->aux))->out_ipdfa = temp;
       }
       else {
           temp->next = prev_out->next;
           prev_out->next = temp;
       }
   }
}

static void
regenerate_pta_at_out (basic_block bb, int rep_cs, csdfa_info ai)
{
   csdfa_info di, ei, prev_out = NULL;

   for (di=out_of_bb (bb); di; di=di->next) {
       if (rep_cs >= di->cs_index)
           break;
   }
   if (!di || di->cs_index != rep_cs)
       return;
 
   for (ei=out_of_bb (bb); ei; ei=ei->next) {
       if (ai->cs_index >= ei->cs_index)
           break;
       prev_out = ei;
   }
   if (ei && (ei->cs_index == ai->cs_index))
       ei->pinfo = di->pinfo;
   else {
       csdfa_info temp = copy_csdfa_info (ai);
       temp->pinfo = di->pinfo;
       temp->linfo = -2;
       temp->new_ptadf = temp->new_lvdf = false;
       if (prev_out == NULL) {
           temp->next = out_of_bb (cbb);
           ((bb_csipdfa_info)(cbb->aux))->out_ipdfa = temp;
       }
       else {
           temp->next = prev_out->next;
           prev_out->next = temp;
       }
   }
}

static void
regenerate_linfo_for_pcsmap_lhs (struct cgraph_node *node)
{
   cs_map ai, bi;
   int i, rep_cs = -1;
   bb_worklist bb_list = worklist_of_cnode (node);
   int num_bb = n_basic_blocks_for_function (DECL_STRUCT_FUNCTION (cnode->decl)) - NUM_FIXED_BLOCKS;
   
   for (ai=pcsmap_of_cnode (node); ai; ai=ai->next) {
       bool is_present = false;
       for (bi=lcsmap_of_cnode (node); bi; bi=bi->next) {
	   if (ai->lhs->cs_index == bi->lhs->cs_index) {
	       bi->present_in_out = true;
	       is_present = true;
	       break;
	   }
	   /* Find the liveness information corresponding to the lhs. */
	   else {
	       csmap_rhs ci;
	       for (ci=bi->rhs; ci; ci=ci->next) {
		   if (ai->lhs->cs_index == ci->rhsel->cs_index) {
		       ci->present_in_out = true;
		       rep_cs = bi->lhs->cs_index;
                       break;
                   }
	       }
           }
	   /* Regenerate the LHS at each node of the cfg with 
	      its liveness information. */
	   if (!is_present && rep_cs!=-1) {
               for (i=num_bb; i>=0; i--) {
                   basic_block bb = bb_list[i].bb;
	           /* Modify at IN of bb. */
	           if (!start_block (bb) && !lv_bypass_block (bb))
	               regenerate_lv_at_in (bb, rep_cs, ai->lhs);
                   /* Modify at OUT of bb. */
	           if (!end_block (bb) && !lv_bypass_block (bb))
	               regenerate_lv_at_out (bb, rep_cs, ai->lhs);
               }
           }
       }     
   }
}

static void
regenerate_pinfo_for_lcsmap_lhs (struct cgraph_node *node)
{
   cs_map ai, bi;
   int i, rep_cs = -1;
   bb_worklist bb_list = worklist_of_cnode (node);
   int num_bb = n_basic_blocks_for_function (DECL_STRUCT_FUNCTION (cnode->decl)) - NUM_FIXED_BLOCKS;

   /* If there is no pcsmap, return. */
   if (!pcsmap_of_cnode (node))
       return;

   for (ai=lcsmap_of_cnode (node); ai; ai=ai->next) {
     if(dump_file)// && !ipacs_time) 
		{fprintf(dump_file,"\nregenerate_pinfo_for_lcsmap_lhs 1 \n");} 
      bool is_present = false;
       for (bi=pcsmap_of_cnode (node); bi; bi=bi->next) {
           if (ai->lhs->cs_index == bi->lhs->cs_index) {
	      if(dump_file)// && !ipacs_time) 
			{fprintf(dump_file,"\nregenerate_pinfo_for_lcsmap_lhs 2 \n");}

	       bi->present_in_out = true;
               is_present = true;
               break;
           }
           /* Find the liveness information corresponding to the lhs. */
           else {
             if(dump_file)// && !ipacs_time) 
		{fprintf(dump_file,"\nregenerate_pinfo_for_lcsmap_lhs 3 \n");}

               csmap_rhs ci;
               for (ci=bi->rhs; ci; ci=ci->next) {
                   if (ai->lhs->cs_index == ci->rhsel->cs_index) {
		       ci->present_in_out = true;
		       if (ci->rhsel->new_ptadf || bi->lhs->new_ptadf)
			   is_present = true;
		       else
                           rep_cs = bi->lhs->cs_index;
                       break;
                   }
               }
           }
           /* Regenerate the LHS at each node of the cfg with 
              its liveness information. */
           if (!is_present && rep_cs!=-1) {
	       for (i=num_bb; i>=0; i--) {
	           basic_block bb = bb_list[i].bb;
	           /* Modify at OUT of bb. */
	           if (!end_block (bb) && !pta_bypass_block (bb))
                       regenerate_pta_at_out (bb, rep_cs, ai->lhs);
                   /* Modify at IN of bb. */
	           if (!start_block (bb) && !pta_bypass_block (bb)){     
			if(dump_file)// && !ipacs_time) 
				{fprintf(dump_file,"\nregenerate_pinfo_for_lcsmap_lhs 4 \n");}

                       regenerate_pta_at_in (bb, rep_cs, ai->lhs);}
               }  
           }
       }
   }
}

/* ----------------------------------
   Process the intraprocecural block.
   ---------------------------------*/
static void
perform_csipdfa_on_intrablock (void)
{
   csdfa_info ai;

   /* If it is not a start block, or a start block of main, copy in-info to out. */
   if (!start_block (cbb)) {
       if (!get_constraint_start (cbb))
           ((bb_csipdfa_info)(cbb->aux))->out_ipdfa = in_of_bb (cbb);
       else
           copy_in_to_out ();
   }
   else {
       if (cbb == main_firstbb) 
	   copy_in_to_out ();
       else {
	   regenerate_linfo_for_pcsmap_lhs (cnode_of_bb (cbb)); 
           /* Compute mapped out at start block. */
           compute_mapped_out_at_startblock ();
       }
   }

   /* Since it is an intra block, we have already discovered aliases for it. */
   if (get_constraint_start (cbb))
       find_pointsto_pair ();
   /* Now propagate changes to successors. */
   propagate_pointsto_to_all_succs (cbb);
   /* Reset the new_ptadf flag for all out csdfa_info of bb. */
   for (ai=out_of_bb (cbb); ai; ai=ai->next)
       ai->new_ptadf = false;

   if (dump_file)// && !ipacs_time)
       print_bb_pinfo (cbb);
}

/*-------------------------------------------------------------------
  Get the basic block of the cnode that needs to be processed. 
  If there is no node to be processed, or the node is a stall node,
  return NULL.
  ------------------------------------------------------------------*/
static basic_block
get_bb_for_pta_processing (bb_worklist list, bool *new_cnode)
{
   bool any_bit_set = false;
   int i;
   int num_bb = n_basic_blocks_for_function (DECL_STRUCT_FUNCTION (cnode->decl)) - NUM_FIXED_BLOCKS;
   for (i=0; i<=num_bb; i++) {
       if (list[i].process_pta == true) {
           if (pta_callee_nodes_processed ()) {
               list[i].process_pta = false;
               return list[i].bb;
           } 
           else {
               if (single_pred_p (list[i].bb) && !return_block (list[i].bb)
                   && !end_block (list[i].bb)) {
                   list[i].process_pta = false;
                   return list[i].bb;
               }
           }
           any_bit_set = true;
       }
   }
   if (!any_bit_set) {
       cnode = prev_node_from_pta_worklist ();
       *new_cnode = true;
   }
   return NULL;
}

static basic_block
get_bb_for_lv_processing (bb_worklist list, bool *new_cnode)
{
   bool callees_processed = lv_callee_nodes_processed ();
   bool any_bit_set = false;
   int i = n_basic_blocks_for_function (DECL_STRUCT_FUNCTION (cnode->decl)) - NUM_FIXED_BLOCKS;
   for (; i>=0; i--) {
       if (list[i].process_lv == true) {
           if (callees_processed) {
               list[i].process_lv = false;
               return list[i].bb;
           } 
           else {
               if (single_succ_p (list[i].bb) && !call_block (list[i].bb)
                   && !start_block (list[i].bb)) {
                   list[i].process_lv = false;
                   return list[i].bb;
               }
           } 
           any_bit_set = true;
       }
   }
   if (!any_bit_set && callees_processed) { 
       cnode = prev_node_from_lv_worklist ();
       *new_cnode = true;
   }
   return NULL;
}

static gimple 
bb_call_stmt (basic_block bb)
{
   gimple_stmt_iterator gsi;
   gimple stmt;
   for (gsi = gsi_start_bb (bb); !gsi_end_p (gsi); gsi_next (&gsi)) {
      stmt = gsi_stmt (gsi);
      if (is_gimple_call (stmt))
         return stmt;
   }
   return NULL;
}

static void
exchange_csmap_entries (cs_map *ai, csmap_rhs *bi)
{
   csdfa_info temp = (*ai)->lhs;
   bool tempbool = (*ai)->present_in_out;

   (*ai)->lhs = (*bi)->rhsel;
   (*ai)->present_in_out = (*bi)->present_in_out;

   (*bi)->rhsel = temp;
   (*bi)->present_in_out = tempbool;
}

static bool
present_in_lcsmap_lhs (struct cgraph_node *node, int cs_index)
{
   cs_map ai;
   bool is_present = false;
   for (ai=lcsmap_of_cnode (node); ai; ai=ai->next) {
       if (ai->lhs->cs_index == cs_index) {
	   is_present = true;
	   break;
       }
   }
   return is_present;
}

static void
modify_pcsmap_1 (struct cgraph_node *node, cs_map bi, csdfa_info ai, cs_map prev_lhs)
{
   cs_map ci;
   /* Can the new bi, with its rhs, be represented by another 
   csinfo in the map? */
   for (ci=pcsmap_of_cnode (node); ci; ci=ci->next) {
       if (ci->lhs->pinfo == ai->pinfo && ci != bi)
           break;
   }
   /* If not, change bi. */
   if (!ci) {
       bi->lhs->pinfo = ai->pinfo;
       bi->lhs->new_ptadf = true;
       if (bi->rhs)
           ((cgraph_info)(node->aux))->pcsmap_required = true;
   }
   /* If yes, merge the two rhs'es. */
   else {
       csmap_rhs temp;
       if (bi->rhs) {
           csmap_rhs rhs = NULL;
           /* Append RHS of bi to RHS of ci. */
           if (ci->lhs->new_ptadf == false) {
               csmap_rhs di;
               for (di=bi->rhs; di; di=di->next) {
                   di->rhsel->new_ptadf = true;
                   rhs = di;
               }
           }
           else
              for (rhs=bi->rhs; rhs->next; rhs=rhs->next);
           rhs->next = ci->rhs;
           ci->rhs = bi->rhs;
       }
       temp = create_csmap_rhs (ai, bi->present_in_out);
       temp->next = ci->rhs;
       ci->rhs = temp;
       if (ci->lhs->new_ptadf && !ci->present_in_out && bi->present_in_out)
           exchange_csmap_entries (&ci, &(ci->rhs));
       /* Remove the row of bi. */
       if (!prev_lhs)
           ((cgraph_info)(node->aux))->pcsmap = bi->next;
       else
           prev_lhs->next = bi->next;
       ggc_free (bi);
       ((cgraph_info)(node->aux))->pcsmap_required = true;
   }
}

static void
modify_pcsmap_2 (struct cgraph_node *node, cs_map bi, csdfa_info ai, cs_map prev_lhs)
{
   csmap_rhs di = bi->rhs;
 
   /* If RHS is NULL, remove this map entry. */
   if (!di) {
       if (prev_lhs)
           prev_lhs->next = bi->next;
       else
           ((cgraph_info)(node->aux))->pcsmap = bi->next;
       ggc_free (bi);
       return;
   }
   else {
       bool rep_found = false;
       /* Initially, replace lhs with the first rhs of the map. */
       bi->lhs = di->rhsel;
       bi->rhs = di->next;
       bi->present_in_out = di->present_in_out;
       ggc_free (di);
       /* If new lhs is to be processed, try finding another
          value from rhs which is already processed. */
       if (bi->lhs->new_ptadf == true) {
           /* Find a better replacement for lhs from rhs:
             an info with new_ptadf as false. */
           for (di=bi->rhs; di; di=di->next) {
               if (di->rhsel->new_ptadf == false) {
                   exchange_csmap_entries (&bi, &di);
                   break;
               }
               /* Otherwise if the RHS is already present in out, choose it as LHS. */
               if (!rep_found) {
                   if (di->present_in_out) {
                       exchange_csmap_entries (&bi, &di);
                       rep_found = true;
                   }
               }
           }
       }
       /* The new replacement of bi's lhs must be at out. */
       regenerate_pta_at_out (end_bb_of_fn (node), ai->cs_index, bi->lhs); 
   }
}

static void
modify_pcsmap_at_startblock (struct cgraph_node *node, csdfa_info ai, bool is_new)
{
   cs_map bi, prev_lhs=NULL;
   bool is_present_in_out = present_in_lcsmap_lhs (node, ai->cs_index);
 
   /* If ai is in lhs/rhs of the pcsmap, modify it. */
   if (!is_new) {
       for (bi=pcsmap_of_cnode (node); bi; bi=bi->next) {
           /* If the changed in info was lhs, we need to choose 
              a new representative from its rhs values. */
           if (bi->lhs->cs_index == ai->cs_index) {
	       if (bi->lhs->new_ptadf == true) {
		   modify_pcsmap_1 (node, bi, ai, prev_lhs);
                   return;
               }
	       else {
		   is_present_in_out |= bi->present_in_out;
		   modify_pcsmap_2 (node, bi, ai, prev_lhs);
		   break;
	       }
           }
           else {
               /* Iterate over all the RHS and see if they match ai. */
               csmap_rhs di, prev_rhs=NULL;
               for (di=bi->rhs; di; di=di->next) {
                    if (di->rhsel->cs_index == ai->cs_index) {
                       is_present_in_out |= di->present_in_out;
                       if (!prev_rhs)
                           bi->rhs = di->next;
                       else
                           prev_rhs->next = di->next;
                       ggc_free (di);
                       break;
                    }
                    prev_rhs = di;
               }
           }
	   prev_lhs = bi;
       }
   }
   /* Add the dataflow value to the callstring map. */
   for (bi=pcsmap_of_cnode (node); bi; bi=bi->next) {
       if (bi->lhs->pinfo == ai->pinfo)
           break;
   }
   if (bi == NULL) {
       cs_map temp = create_new_csmap_cell (ai, is_present_in_out);
       temp->next = pcsmap_of_cnode (node);
       ((cgraph_info)(node->aux))->pcsmap = temp;
   }
   else {
       csmap_rhs temp = create_csmap_rhs (ai, is_present_in_out);
       temp->next = bi->rhs;
       bi->rhs = temp;
       if (bi->lhs->new_ptadf && !bi->present_in_out && is_present_in_out)
           exchange_csmap_entries (&bi, &(bi->rhs));
       ((cgraph_info)(node->aux))->pcsmap_required = true;
   }
}

static void
modify_lcsmap_1 (struct cgraph_node *node, cs_map bi, csdfa_info ai, cs_map prev_lhs)
{
   cs_map ci;
   /* Can the new bi, with its rhs, be represented by another 
      csinfo in the map? */
   for (ci=lcsmap_of_cnode (node); ci; ci=ci->next) {
       if (ci->lhs->linfo == ai->linfo && ci != bi)
           break;
   }
   /* If not, change bi. */
   if (!ci) {
       bi->lhs->linfo = ai->linfo;
       bi->lhs->new_lvdf = true;
       if (bi->rhs)
           ((cgraph_info)(node->aux))->lcsmap_required = true;
   }
   /* If yes, merge the two rhs'es. */
   else {
       csmap_rhs temp;
       /* Append RHS of bi to RHS of ci. */
       if (bi->rhs) {
           csmap_rhs rhs=NULL;
           if (ci->lhs->new_lvdf == false) {
               csmap_rhs di;
               for (di=bi->rhs; di; di=di->next) {
                   di->rhsel->new_lvdf = true;
                   rhs = di;
               }
           }
           else
               for (rhs=bi->rhs; rhs->next; rhs=rhs->next);
           rhs->next = ci->rhs;
           ci->rhs = bi->rhs;
       }
       temp = create_csmap_rhs (ai, bi->present_in_out);
       temp->next = ci->rhs;
       ci->rhs = temp;
       if (ci->lhs->new_lvdf && !ci->present_in_out && bi->present_in_out)
           exchange_csmap_entries (&ci, &(ci->rhs));
       /* Remove the row of bi. */
       if (!prev_lhs)
           ((cgraph_info)(node->aux))->lcsmap = bi->next;
       else
           prev_lhs->next = bi->next;
       ggc_free (bi);
       ((cgraph_info)(node->aux))->lcsmap_required = true;
   }
}

static void
modify_lcsmap_2 (struct cgraph_node *node, cs_map bi, csdfa_info ai, cs_map prev_lhs)
{
   csmap_rhs di=bi->rhs;
   /* If RHS is NULL, remove this map entry from the map. */
   if (!di) {
       if (prev_lhs)
           prev_lhs->next = bi->next;
       else
           ((cgraph_info)(node->aux))->lcsmap = bi->next;
       ggc_free (bi);
       return;
   }
   else {
       bool rep_found = false;
       /* Initially, replace lhs with the first rhs. */
       bi->lhs = di->rhsel;
       bi->rhs = di->next;
       bi->present_in_out = di->present_in_out;
       ggc_free (di);
       /* If the new LHS is to be processed, try finding another 
          value from RHS which is already processed. */
       if (bi->lhs->new_lvdf == true) {
           /* Is lhs already present in out? */
           if (bi->present_in_out)
               rep_found = true;
           /* Find a better replacement for LHS from RHS:
              info with new_lvdf as false. */
           for (di=bi->rhs; di; di=di->next) {
               if (di->rhsel->new_lvdf == false) {
                   exchange_csmap_entries (&bi, &di);
                   break;
               }
               /* Otherwise if the rhs is already present in out, 
                  choose it as lhs. */
               if (!rep_found) {
                   if (di->present_in_out) {
                       exchange_csmap_entries (&bi, &di);
                       rep_found = true;
                   }
               }
           }
       }
        /* The new replacement of bi's lhs must be at out. */
       regenerate_lv_at_in (start_bb_of_fn (node), ai->cs_index, bi->lhs);
   }
}

static void
modify_lcsmap_at_endblock (struct cgraph_node *node, csdfa_info ai, bool is_new)
{
   cs_map bi, prev_lhs=NULL;
   bool is_present_in_out = false;
   /* If the changed in info is already in lhs/rhs of map, 
      modify it. */
   if (!is_new) {
       if(dump_file)// && !ipacs_time) 
		fprintf(dump_file,"\nmodify lcsmap at endblock 1\n");

       for (bi=lcsmap_of_cnode (node); bi; bi=bi->next) {
           /* If the changed in info was lhs, we need to 
	      perform regeneration again at startblock. */
           if (bi->lhs->cs_index == ai->cs_index) {
	       if (bi->lhs->new_lvdf == true) {
                 if(dump_file)// && !ipacs_time) 
			fprintf(dump_file,"\nmodify lcsmap at endblock 2\n");

		   modify_lcsmap_1 (node, bi, ai, prev_lhs);
		   return;
               }
               else {
                  if(dump_file)// && !ipacs_time) 
			fprintf(dump_file,"\nmodify lcsmap at endblock 3\n");

		   is_present_in_out = bi->present_in_out;
                   modify_lcsmap_2 (node, bi, ai, prev_lhs);
		   break;
               }
           }
           else {
               /* Iterate over all the RHS and see if they match ai. */
               csmap_rhs di, prev_rhs=NULL;
               for (di=bi->rhs; di; di=di->next) {
                    if (di->rhsel->cs_index == ai->cs_index) {
                   if(dump_file)// && !ipacs_time) 
			fprintf(dump_file,"\nmodify lcsmap at endblock 4\n");

                       is_present_in_out = di->present_in_out;
                       if (!prev_rhs){                  if(dump_file)// && !ipacs_time) 
								fprintf(dump_file,"\nmodify lcsmap at endblock 5\n");

                           bi->rhs = di->next;}
                       else{                  if(dump_file)// && !ipacs_time) 
							fprintf(dump_file,"\nmodify lcsmap at endblock 6\n");

                           prev_rhs->next = di->next;}
                       ggc_free (di);
                       break;
                    }
                    prev_rhs = di;
               }
           }
           prev_lhs = bi;
       }
   }
   /* Add the dataflow value to the callstring map. */
   for (bi=lcsmap_of_cnode (node); bi; bi=bi->next) {
       if (bi->lhs->linfo == ai->linfo)
           break;
   }
   if (bi == NULL) {
       cs_map temp = create_new_csmap_cell (ai, is_present_in_out);
       temp->next = lcsmap_of_cnode (node);
       ((cgraph_info)(node->aux))->lcsmap = temp;
   }
   else {
       csmap_rhs temp = create_csmap_rhs (ai, is_present_in_out);
       temp->next = bi->rhs;
       bi->rhs = temp;
       if (bi->lhs->new_lvdf && !bi->present_in_out && is_present_in_out)
	   exchange_csmap_entries (&bi, &(bi->rhs));
       ((cgraph_info)(node->aux))->lcsmap_required = true;
   }
}

/* ------------------------------------------------------------------------------
   Function to propagate a single pointsto info cell to the given basic block.
   The pointsto info to be propagated is pointed to by the cpinfo.
   -----------------------------------------------------------------------------*/
static bool
propagate_pointsto_to_calledblock (struct cgraph_node *node, basic_block bb)
{
   bool add_to_worklist = false;
   csdfa_info ai=cpinfo, bi, prev=NULL;
 
   ai->cs_index = append_bb_to_callstring_1 (node, cbb, &ai, bb, true);
   if (ai->cs_index == -1) 
       return false;

   for (bi=in_of_bb (bb); bi; bi=bi->next) {
       if (ai->cs_index >= bi->cs_index)
          break;
       prev = bi;
   }

   ai->pinfo = calculate_actual_out (ai, bi);

   if (bi && (bi->cs_index == ai->cs_index)) {
       /* calculate confluence of mustout and mayout. */
       int old_info = bi->pinfo;
       bi->pinfo = compute_in_confluence (bi->pinfo, ai->pinfo);
       if (old_info != bi->pinfo) {
           bi->new_ptadf = true;
	   add_to_worklist = true;
           modify_pcsmap_at_startblock (node, bi, false);
       }
       ggc_free (ai);
   }
   else {
       modify_pcsmap_at_startblock (node, ai, true);
       if (prev == NULL) {
           ai->next = in_of_bb (bb);
           ((bb_csipdfa_info)(bb->aux))->in_ipdfa = ai;
       }
       else {
          ai->next = prev->next;
          prev->next = ai;
       }
       add_to_worklist = true;
   }
   return add_to_worklist;
}

static int
remove_local_vars (int index)
{
   
    int in = index;
    bitmap live_set;
    bitmap_iterator bir;
    unsigned int var;

    if(index < 0)
	return -1;

   live_set = get_live_set(index);
 
   EXECUTE_IF_SET_IN_BITMAP(live_set,0,var,bir) {
        if(!global_var(var)) {
                in = remove_var_from_live_set(in,var);
        }
    }
   
   return in;
 
}

static bool
propagate_liveness_to_endblock (struct cgraph_node *called_node,
                                basic_block end_bb, csdfa_info ai, bool recompute)
{
   bool add_to_worklist = false, set_bb_for_processing_pta = false;
   csdfa_info bi, prev = NULL;
    //if(dump_file)// && !ipacs_time) 
	//fprintf(dump_file,"\npropagate_liveness_to_endblock\n");

   csdfa_info temp = copy_csdfa_info (ai);
   temp->cs_index = append_bb_to_callstring (single_pred (cbb), ai->cs_index, end_bb, false);
   temp->linfo = remove_local_vars (temp->linfo);

   for (bi=out_of_bb (end_bb); bi; bi=bi->next) {
       if (temp->cs_index >= bi->cs_index)
          break;
       prev = bi;
   }
   if (bi && (bi->cs_index == temp->cs_index)) {
       int old_info = bi->linfo;
       bi->linfo = compute_lout_union (bi->linfo, temp->linfo);
       if (old_info != bi->linfo) {
           bi->new_lvdf = true;
           add_to_worklist = true;
           modify_lcsmap_at_endblock (called_node, bi, false);
       }
       if (recompute) {
           bi->new_ptadf = true;
           set_bb_for_processing_pta = true;
       }
       ggc_free (temp);
   }
   else {
       modify_lcsmap_at_endblock (called_node, temp, true);
       if (prev == NULL) {
           temp->next = out_of_bb (end_bb);
           ((bb_csipdfa_info)(end_bb->aux))->out_ipdfa = temp;
       }
       else {
          temp->next = prev->next;
          prev->next = temp;
       }
       add_to_worklist = true;
   }
   return add_to_worklist;
}


static void
process_fptr_call_liveness (basic_block caller_bb, tree func_decl, bool recompute)
{
   csdfa_info ai, ci;
   tree decl;
   bitmap pointee_set;
   bitmap_iterator di;
   unsigned int dival;
   /* if(dump_file)// && !ipacs_time) 
	fprintf(dump_file,"\nprocess_fptr_call_liveness\n");*/

   unsigned int varid = cs_get_vi_for_tree (func_decl)->id;

   for (ai=in_of_bb (caller_bb); ai; ai=ai->next) {
       if (ai->new_ptadf) {
	    /* Find the corresponding csdfa in return block and set its new_lvdf. */ 
           for (ci=in_of_bb (cbb); ci; ci=ci->next) {
               if (ci->cs_index <= ai->cs_index) 
		   break;
           }
	   if (ci && (ci->cs_index == ai->cs_index)) {
               /*pinfo = (ai->pinfo < 0) ? NULL : VEC_index (pointsto_val, mayinfo, ai->pinfo)
               for (bi=pinfo; bi; bi=bi->next) {
                   if (bi->lhs == varid) {*/
		   if(is_x_in_pointsto_set(ai->pinfo, maylist, varid)){
		       pointee_set = get_pointee_set(ai->pinfo,maylist,varid);
                       EXECUTE_IF_SET_IN_BITMAP(pointee_set,0,dival,di){
                           decl = cs_get_varinfo (dival)->decl;
                           if (!decl)
                               continue;
	                   /* Deduce the function called. */
                           if (TREE_CODE (decl) == FUNCTION_DECL) {
                               struct function *called_fn = DECL_STRUCT_FUNCTION (decl);
                               if (called_fn) {
                                   struct cgraph_node *node = cnode_of_bb (EXIT_BLOCK_PTR_FOR_FUNCTION (called_fn)->prev_bb);
                                   basic_block end_bb = end_bb_of_fn (node);
                                   bool add_to_worklist = false;
				   ci->new_lvdf = true;
                                   add_to_worklist = propagate_liveness_to_endblock (node, end_bb, ci, recompute);
                                   if (add_to_worklist) {
                                       set_bb_to_process_lv (node, end_bb);
                                       add_to_fn_worklist (node);
                                   }
                               }
                           }
                      }     
		                     
               }
           }
       }
   }
}

/* ------------------------------------------------------------------------------------
   If the call is made through function pointer, we need to check the mayin information
   to find the functions to which the function pointer may point to, and propagate
   the pointsto information to the called function.
   -----------------------------------------------------------------------------------*/
static void
process_fptr_call_pointsto (gimple stmt, tree func_decl)
{
   csdfa_info ai;
   tree decl;
   bitmap_iterator ci;
   unsigned int cival;
   bitmap pointee_set;
   unsigned int varid = cs_get_vi_for_tree (func_decl)->id;
   for (ai=out_of_bb (cbb); ai; ai=ai->next) {
       if (ai->new_ptadf) {
           /*pointsto_val bi, pinfo;*/
	   cpinfo = copy_csdfa_info (ai);
	   pointee_set = get_pointee_set(ai->pinfo,maylist,varid);
           if(pointee_set){
		   EXECUTE_IF_SET_IN_BITMAP(pointee_set,0,cival,ci){
                       decl = cs_get_varinfo (cival)->decl;
		       if (!decl)
			   continue;
                       if (TREE_CODE (decl) == FUNCTION_DECL) {
                           struct function *called_function = DECL_STRUCT_FUNCTION (decl);
                           if (called_function) {
                               basic_block called_bb = ENTRY_BLOCK_PTR_FOR_FUNCTION (called_function)->next_bb;
                               struct cgraph_node *node = cnode_of_bb (called_bb);
                               bool add_to_worklist = false;
		               cpinfo = copy_csdfa_info (ai);
                               compute_alias_and_pinfo = true;
                               map_arguments_at_call (stmt, node->decl, false);
                               compute_alias_and_pinfo = false;
                               fptr_constraint_list = NULL;
                               add_to_worklist |= propagate_pointsto_to_calledblock (node, called_bb);
                               if (add_to_worklist) {
                                   set_bb_to_process_pta (node, called_bb);
                                   set_in_pta_worklist (node);
                               }
                           }
                       }
                   
               }
           }
       }
   }
}

/* -------------------------------------
   Function to process a call statement.
   ------------------------------------*/
static void
process_call_stmt (gimple stmt)
{
   tree decl = get_called_fn_decl (stmt);

   /* If its a simple function call. */
   if (TREE_CODE (decl) == FUNCTION_DECL) {
       basic_block called_bb;
       csdfa_info ai;
       bool add_to_worklist = false;
       struct cgraph_node *node;
       struct function *called_function = DECL_STRUCT_FUNCTION (decl);
       gcc_assert (called_function);
       called_bb = ENTRY_BLOCK_PTR_FOR_FUNCTION (called_function)->next_bb;
       node = cnode_of_bb (called_bb);
     
       for (ai=out_of_bb (cbb); ai; ai=ai->next) {
           if (ai->new_ptadf) {
	       cpinfo = copy_csdfa_info (ai);
	       find_pointsto_pair ();
               add_to_worklist |= propagate_pointsto_to_calledblock (node, called_bb);
           }
        }
        if (add_to_worklist) {
           set_bb_to_process_pta (node, called_bb);
           set_in_pta_worklist (node);
        }
   }
   /* The call is through function pointers. */
   else
       process_fptr_call_pointsto (stmt, decl);
}
 
/* ----------------------
   Process the call block.
   ---------------------*/
static void
perform_csipdfa_on_callblock (void)
{
   csdfa_info ai;
   /* If the call block is the start block of main (), or not a start
      block, copy in-info to out. */
   if (!start_block (cbb) || cbb == main_firstbb) 
       copy_in_to_out ();
   else 
       compute_mapped_out_at_startblock ();
   
   all_contexts_together = false;
   process_call_stmt (bb_call_stmt (cbb));
   all_contexts_together = true;
   
   propagate_pointsto_to_all_succs (cbb);

   /* Reset the new_ptadf flag for all out csdfa_info of bb. */
   for (ai=out_of_bb (cbb); ai; ai=ai->next)
       ai->new_ptadf = false;
    
   if (dump_file)// && !ipacs_time)
       print_bb_pinfo (cbb);
}

/* ----------------------------------------------------------------------
   Compute the sorted confluence of the two mayin's. The confluence here 
   is union. The union is sorted, like in the case of GEN computation.
   ---------------------------------------------------------------------*/

static int
compute_out_confluence (int out_index, int in_index)
{
   int tmp_out; 
   bitmap lhs_bit;
   bitmap_iterator bir;
   unsigned int lhs;

 
   if (in_index < 0 )
       return out_index;
   if (out_index < 0)
       return in_index;

   tmp_out = copy_pointsto_set_to_temp(out_index);
   lhs_bit = get_bitmap_for_lhs(in_index,maylist);

   EXECUTE_IF_SET_IN_BITMAP(lhs_bit,0,lhs,bir){
        bitmap pointee_set = get_pointee_set(in_index, maylist, lhs);
        tmp_out = add_pointee_set(tmp_out,tmplist,lhs,pointee_set);
   }
   return compute_pointsto_index_from_templist(tmp_out);
}

static void
copy_in_to_out_1 (void)
{
   csdfa_info ai, bi=NULL;
   for (ai=in_of_bb (cbb); ai; ai=ai->next) {
       if (ai->new_ptadf) {
           csdfa_info prev = NULL;
           for (bi=out_of_bb (cbb); bi; bi=bi->next) {
               if (ai->cs_index >= bi->cs_index)
                   break;
               prev = bi;
           }
           if (bi && (bi->cs_index == ai->cs_index)) {
               int old_pinfo = bi->pinfo;
               bi->pinfo = compute_out_confluence (bi->pinfo, ai->pinfo);
               if (bi->pinfo != old_pinfo)
                   bi->new_ptadf = true;
           }
           else {
               csdfa_info temp = copy_csdfa_info (ai);
               if (prev == NULL) {
                  temp->next = out_of_bb (cbb);
                  ((bb_csipdfa_info)(cbb->aux))->out_ipdfa = temp;
               }
               else {
                   temp->next = prev->next;
                   prev->next = temp;
               }
           }
           ai->new_ptadf = false;
       }
   }
}

/* -------------------------
   Process the return block.
   ------------------------*/
static void
perform_csipdfa_on_returnblock (void)
{
   csdfa_info ai;
   
   copy_in_to_out_1 ();

   /* Now propagate changes to successors. */
   propagate_pointsto_to_all_succs (cbb);

   /* Reset the new_ptadf flag for all out csdfa_info of bb. */
   for (ai=out_of_bb (cbb); ai; ai=ai->next)
       ai->new_ptadf = false;

   if (dump_file)// && !ipacs_time)
       print_bb_pinfo (cbb);
}

static void
print_pcsmap_at_endblock (basic_block bb)
{
   cs_map ai;
   fprintf (dump_file, "\nRegeneration performed at %s node's %d block, printing the map :\n",
            cgraph_node_name (cnode_of_bb (bb)), bb->index);
   for (ai=pcsmap_of_cnode (cnode_of_bb (bb)); ai; ai=ai->next) {
       csmap_rhs bi = ai->rhs;
       print_callstring_sequence (ai->lhs->cs_index);
       fprintf (dump_file, " --> ");
       if (bi == NULL)
           fprintf (dump_file, "Empty");
       else {
           for (; bi; bi=bi->next) {
               print_callstring_sequence (bi->rhsel->cs_index);
               if (bi->next)
                   fprintf (dump_file, "  ,  ");
           }
       }
       fprintf (dump_file, "\n");
   }
}

static void
print_lcsmap_at_startblock (basic_block bb)
{
   cs_map ai;
   fprintf (dump_file, "\nRegeneration performed at %s node's %d block, printing the map :\n",
            cgraph_node_name (cnode_of_bb (bb)), bb->index);
   for (ai=lcsmap_of_cnode (cnode_of_bb (bb)); ai; ai=ai->next) {
       csmap_rhs bi = ai->rhs;
       print_callstring_sequence (ai->lhs->cs_index);
       fprintf (dump_file, " --> ");
       if (bi == NULL)
           fprintf (dump_file, "Empty");
       else {
           for (; bi; bi=bi->next) {
               print_callstring_sequence (bi->rhsel->cs_index);
               if (bi->next)
                   fprintf (dump_file, "  ,  ");
           }
       }
       fprintf (dump_file, "\n");
   }
}

/*-------------------------------------------------------------------------------------
  A call statement can return a value. This mapping has to be performed (after the call
  has been made) at the return block. 
  ------------------------------------------------------------------------------------*/
static void
map_return_value (basic_block callerbb)
{
   gimple_stmt_iterator ret_gsi;
   /* Is there a pointer value returned by the return statement? */
   for (ret_gsi = gsi_start_bb (cbb); !gsi_end_p (ret_gsi); gsi_next (&ret_gsi)) {
      gimple ret_stmt = gsi_stmt (ret_gsi);
      if (gimple_code (ret_stmt) == GIMPLE_RETURN
          && gimple_return_retval (ret_stmt) != NULL_TREE
          && field_must_have_pointers (gimple_return_retval (ret_stmt))) {
           /* Map it to the receiving value of call statement. */
           gimple call_stmt = bb_call_stmt (callerbb);
           if (is_gimple_call (call_stmt)) {
               tree lhsop = gimple_call_lhs (call_stmt);
               if (lhsop) {
                   VEC(ce_s, heap) *lhsc = NULL, *rhsc = NULL;
                   cs_get_constraint_for (lhsop, &lhsc);
                   cs_get_constraint_for (gimple_return_retval (ret_stmt), &rhsc);
                   cs_process_all_all_constraints (lhsc, rhsc);
                   VEC_free (ce_s, heap, lhsc);
                   VEC_free (ce_s, heap, rhsc);
               }
               break;
           }
       }
       break;
   }
}

static int
mark_escaped_rhs (int index)
{
   bitmap_iterator bir;
   unsigned int var;
   pointsto_val root = (index < 0) ? NULL : copy_pointsto_val (VEC_index (pointsto_val, mayinfo, index));
   pointsto_val out = root;

   while (out) {
       bitmap rhs = out->rhs;
       EXECUTE_IF_SET_IN_BITMAP(rhs,0,var,bir)
       {
          if (var_defined_in_cfun (var)){
            var = escaped_id;
            bitmap_set_bit(rhs,var);
        }

       }
       out = out->next;
   }
   /* Compute the index of the actual out. */
   index = index_in_maypool(out);
   if(index < 0) {
         index = VEC_length (pointsto_val, mayinfo);
         VEC_safe_push (pointsto_val, heap, mayinfo, out);
   }

   return index;

}

/*------------------------------------------------------------------------------
  If the current basic block is the end block, we need to propagate the pointsto 
  information to the return block of the respective caller of the function to 
  which the endblock belongs. To do so, we find out the caller call block by 
  analyzing the callstrings. The pointsto information is then propagated to the 
  return block corresponding to that call block. If the in value has changed, 
  the return block is added to the worklist.
  -----------------------------------------------------------------------------*/
static void
propagate_pointsto_to_returnblock_in (basic_block retbb)
{
   csdfa_info ai, prev=NULL;

   for (ai=in_of_bb (retbb); ai; ai=ai->next) {
       if (cpinfo->cs_index >= ai->cs_index)
           break;
        prev = ai;
   }
   cpinfo->pinfo = calculate_actual_out (cpinfo, ai);
   if (ai && (ai->cs_index = cpinfo->cs_index)) {
       ai->pinfo = compute_out_index_1 (ai->pinfo, cpinfo->pinfo, true);
       ggc_free (cpinfo);
   }
   else {
       if (prev == NULL) {
           cpinfo->next = in_of_bb (retbb);
           ((bb_csipdfa_info)(retbb->aux))->in_ipdfa = cpinfo;
       }
       else {
           cpinfo->next = prev->next;
           prev->next = cpinfo;
       }
   }
}

static void
propagate_pointsto_to_returnblock_out (basic_block retbb)
{
   csdfa_info ai, prev=NULL;

   for (ai=out_of_bb (retbb); ai; ai=ai->next) {
       if (cpinfo->cs_index >= ai->cs_index)
           break;
        prev = ai;
   }
   cpinfo->pinfo = calculate_actual_out_1 (cpinfo, ai);
   if (ai && (ai->cs_index = cpinfo->cs_index)) {
       ai->pinfo = compute_in_confluence (ai->pinfo, cpinfo->pinfo);
       ggc_free (cpinfo);
   }
   else {
       if (prev == NULL) {
           cpinfo->next = out_of_bb (retbb);
           ((bb_csipdfa_info)(retbb->aux))->out_ipdfa = cpinfo;
       }
       else {
           cpinfo->next = prev->next;
           prev->next = cpinfo;
       }
   }
}

static void
propagate_pointsto_to_returnblock (void)
{
   csdfa_info ai;
   for (ai=out_of_bb (cbb); ai; ai=ai->next) {
       if (ai->new_ptadf) {
           basic_block callerbb = NULL;
           int cs_index = get_caller_bb (&callerbb, ai->cs_index);

           /* Propagate a set to in of returnblock. */
           cpinfo = copy_csdfa_info (ai);
           cpinfo->cs_index = cs_index;
           propagate_pointsto_to_returnblock_in (single_succ (callerbb));

           /* Propagate a set to out of returnblock. */
           cpinfo = copy_csdfa_info (ai);
           cpinfo->cs_index = cs_index;

           compute_only_pinfo = true;
           map_return_value (callerbb);
           compute_only_pinfo = false;

           /* If ai's linfo has changed, copy it to cpinfo. */
           cpinfo->linfo = ai->linfo;
           propagate_pointsto_to_returnblock_out (single_succ (callerbb));
           ai->new_ptadf = false;
       }
   }
}

static void
propagate_liveness_to_callblock (bool recompute)
{
   csdfa_info ai;

   for (ai=in_of_bb (cbb); ai; ai=ai->next) {
       if (ai->new_lvdf) {
           bool set_bb_for_processing = false, set_bb_for_processing_pta = false;
           basic_block callerbb = NULL;
           int cs_index = get_caller_bb (&callerbb, ai->cs_index);
           csdfa_info bi;
           
           for (bi=out_of_bb (callerbb); bi; bi=bi->next) {
               if (cs_index == bi->cs_index) {
                   int old_linfo = bi->linfo;
                   bi->linfo = compute_lout_union (bi->linfo, ai->linfo);
                   /* Check if the out value has changed. */
                   if (old_linfo != bi->linfo) {
		       set_bb_for_processing = true; 
                       bi->new_lvdf = true;
		   }
                   if (recompute) {
                       bi->new_ptadf = true;
                       set_bb_for_processing_pta = true;
                   }
		   break;
               }
           }
           ai->new_lvdf = false;
           if (set_bb_for_processing) {
               struct cgraph_node *node = cnode_of_bb (callerbb);	
               set_bb_to_process_lv (node, callerbb);
               add_to_fn_worklist_1 (node);
           }
       }
   }
}

/* -----------------------------------------------------------------------
   We had performed callstring mapping at the start block of a function. 
   Now for rest of the intraprocedural basic blocks of that function, 
   only the pointsto values associated with the representative callstrings 
   will be processed. Therefore, at the end block, we need to regenerate 
   the whole pointsto information using the pcsmap at the startblock. For 
   eg, let the callstring map be {(ci -> di, ei), (fi->gi)}. The pointsto 
   value associated with ci be val1, and with fi be val2. Thus the value 
   at in of end block will be {(ci, val1), (fi, val2). Then the final 
   regenerated value at the out of the end block will be {(ci, val1), 
   (di, val1), (ei, val1), (fi, val2), (gi, val2)}. 
   ----------------------------------------------------------------------*/
static void
perform_regeneration_at_endblock (basic_block sp_bb)
{
   csdfa_info ai, rep;
   cs_map bi;
   csmap_rhs rhs;
   for (bi=pcsmap_of_cnode (cnode); bi; bi=bi->next) {
       /* Get the representative cs from out of endbb. */
       for (rep=out_of_bb (cbb); rep; rep=rep->next) {
	   if (bi->lhs->cs_index > rep->cs_index)
	       break;
           if (bi->lhs->cs_index == rep->cs_index) {
               for (rhs=bi->rhs; rhs; rhs=rhs->next) {
	           csdfa_info regen = rhs->rhsel;
                   if (rep->new_ptadf || regen->new_ptadf) {
                       csdfa_info prev = NULL;
    
                       /* Change the pointsto info of regen at out. */
                       for (ai=out_of_bb (cbb); ai; ai=ai->next) {
	                   if (regen->cs_index >= ai->cs_index)
                               break;
		           prev = ai;
                       }
	               if (ai && (regen->cs_index == ai->cs_index)) {
                           if (ai->new_ptadf == false) {
                               if (ai->pinfo != rep->pinfo) {
                                   ai->pinfo = rep->pinfo;
                                   ai->new_ptadf = true;
                               }
                           }
                       }
	               else {
                           csdfa_info temp = copy_csdfa_info (rep);
                           temp->cs_index = regen->cs_index;
                           temp->new_ptadf = true;
                           if (prev == NULL) {
                               temp->next = out_of_bb (cbb);
                               ((bb_csipdfa_info)(cbb->aux))->out_ipdfa = temp;
                           }
                           else {
                               temp->next = prev->next;
                               prev->next = temp;
                           }
                       }
                   }
	       }
           }
       }
   }
   /* Reset the in of sp_bb. */
   for (ai=in_of_bb (sp_bb); ai; ai=ai->next) 
       ai->new_ptadf = false;

   /*if (dump_file && !ipacs_time)
       print_pcsmap_at_endblock (cbb);*/
}

static void
perform_regeneration_at_startblock (basic_block end_bb)
{
   csdfa_info rep;
   cs_map bi;
   csmap_rhs rhs;
 
   for (bi=lcsmap_of_cnode (cnode); bi; bi=bi->next) {
       /* Get the representative callstring from in. */
       for (rep=in_of_bb (cbb); rep; rep=rep->next) {
	   if (bi->lhs->cs_index > rep->cs_index) 
	       break;
	   if (rep->cs_index == bi->lhs->cs_index) {
               for (rhs=bi->rhs; rhs; rhs=rhs->next) {
	           csdfa_info regen = rhs->rhsel;
		   if (regen->new_lvdf || rep->new_lvdf) {
                       csdfa_info prev = NULL, ai;

                       /* Change the pointsto info of regen at in info. */
                       for (ai=in_of_bb (cbb); ai; ai=ai->next) {
                           if (regen->cs_index >= ai->cs_index)
                               break;
                           prev = ai;
                       }
           	       if (ai && (ai->cs_index == regen->cs_index)) {
               		   if (ai->new_lvdf == false) {
                   	       if (ai->linfo != rep->linfo) {
                       		   ai->linfo = rep->linfo;
                       	           ai->new_lvdf = true;
               		       }
           		   }
		       }
           	       else {
               	           csdfa_info temp = copy_csdfa_info (rep);
	       		   temp->pinfo = -2; 
			   temp->cs_index = regen->cs_index;
                           temp->new_lvdf = true;
	
                           if (prev == NULL) {
                               temp->next = in_of_bb (cbb);
                               ((bb_csipdfa_info)(cbb->aux))->in_ipdfa = temp;
                           }
                           else {
                               temp->next = prev->next;
                               prev->next = temp;
                           }
                       }
                   }
               }
           }
       }
   }
   /* Reset the out of end_bb. */
   for (rep=out_of_bb (end_bb); rep; rep=rep->next)
       rep->new_lvdf = false;

   /*if (dump_file && !ipacs_time)
       print_lcsmap_at_startblock (cbb);*/
}

/* ----------------------
   Process the end block.
   ---------------------*/
static void
perform_csipdfa_on_endblock (void)
{
   basic_block sp_block = start_bb_of_fn (cnode);
   
   copy_in_to_out ();

   if (sp_block != main_firstbb) {
       if (pcsmap_required (cnode)) {
           perform_regeneration_at_endblock (sp_block);
           ((cgraph_info)(cnode->aux))->pcsmap_required = false;
       }    
       /* Else just print that the regeneration is not required */ 
       else {
           if (dump_file)// && !ipacs_time)
               fprintf (dump_file, "\npointsto regeneration not required at %s node's %d block\n", 
		 cgraph_node_name (cnode), cbb->index);
       }

       all_contexts_together = false;
       propagate_pointsto_to_returnblock ();
       all_contexts_together = true;
   }

   if (dump_file)// && !ipacs_time)
       print_bb_pinfo (cbb);
}


/* -------------------------------------
   Execute the IPA PTA with callstrings. 
   ------------------------------------*/
static void
compute_pointsto (void)
{
  bool do_pointsto = true;
  
  while (do_pointsto) {
       cnode = first_node_from_pta_worklist ();
       if (!cnode) 
	   do_pointsto = false;
       /* If cnode is NULL, we are done with liveness computation. */
       while (cnode) {
           bb_worklist bb_list = worklist_of_cnode (cnode);
           bool process_bb = true;
           while (process_bb) {
	       bool new_cnode = false;
               /*if (dump_file && !ipacs_time)
                   print_pta_worklist ();*/
               cbb = get_bb_for_pta_processing (bb_list, &new_cnode);
               if (!new_cnode) {
                   if (cbb) {
		       recompute_liveness = false;
                       if (call_block (cbb)) 
                           perform_csipdfa_on_callblock ();
                       else if (return_block (cbb))
                           perform_csipdfa_on_returnblock ();
                       else if (end_block (cbb)) {
                           perform_csipdfa_on_endblock ();
                           cnode = prev_node_from_pta_worklist ();
                           process_bb = false;
                       }
                       else
                           perform_csipdfa_on_intrablock ();
		       if (recompute_liveness) {
			   compute_liveness (true);
			   break;
		       }
                   }
                   else {
                       cnode = next_node_from_pta_worklist (pta_node);
                       process_bb = false;
                   }
	       }
	       else
	           process_bb = false;
           }
       }
   }
}


/* Function to remove a variable from dflist. */
static bool
remove_from_df_list (unsigned int val, int *list)
{

     df_list bi  = (*list < 0) ? NULL : copy_df_list ( VEC_index (df_list, liveinfo, *list));
     df_list prev = NULL,ai = bi;	 
   /*   df_list ai = *list, prev=NULL;*/

   bool found = false;

   for (; ai; ai=ai->next) {
       if (ai->val == val) {
           found = true;
           break;
       }
       if (ai->val > val)
           break;
       prev = ai;
   }

    if (found) {
       if (!prev)
          bi = bi->next;
       else
          prev->next = ai->next;
       /*ggc_free (ai);*/
        *list = compute_liveness_index(bi);
	ggc_free (ai);
   }
    return found;
}


static void
compute_liveness_from_constraint (csdfa_info *ci, constraint_list bi,
                                  int *gen, int *kill, int linfo)
{
      df_list list;
      bitmap_iterator ei;
      unsigned int eival , dirval;
      bool init = true;
      bitmap pointee_set;
   for (list=bi->stmt_constraint_chain; list; list=list->next) {
       constraint_t t = VEC_index (constraint_t, aliases, list->val);
       struct constraint_expr lhs = t->lhs;
       struct constraint_expr rhs = t->rhs;
       bool lhs_live = false;
       int lhsvar = -1;
       /* Find kill for LHS for the first time if its a scalar. */

       if (init && lhs.type == SCALAR) {
           csvarinfo_t lhs_vi = NULL;
           /* Reduce LHS. */
           if (!unexpandable_var (lhs.var, lhs.offset)) {
               lhs_vi = cs_first_vi_for_offset (cs_get_varinfo (lhs.var), lhs.offset);
               /* If lhs_vi is NULL, the lhs does not give any valid information. */
               if (lhs_vi) {
                   lhsvar = lhs_vi->id;
                   add_to_df_list (lhs_vi->id, kill);
                   lhs_live |= remove_from_df_list (lhs_vi->id, gen);
               }
           }
           else {
               lhsvar = lhs.var;
               add_to_df_list (lhs.var, kill);
               lhs_live |= remove_from_df_list (lhs.var, gen);
           }
       }

       
	/* Find gen & kill for LHS for the first time if its a deref. */
       if (init && lhs.type == DEREF) { 
           bool must_lhs = false, found = false;
           if(is_x_in_pointsto_set((*ci)->pinfo , maylist, lhs.var))
                found = true;
           if(has_must_pointee((*ci)->pinfo , maylist, lhs.var))
                must_lhs = true;
           if (!found) {
               (*ci)->linfo = -1;
               *gen = *kill = -1;
           }
           else {
               if (must_lhs) {
                   pointee_set = get_pointee_set((*ci)->pinfo, maylist, lhs.var);
                   dirval = bitmap_first_set_bit(pointee_set);
                   if (!unexpandable_var (dirval, lhs.offset)) {
                       csvarinfo_t lhs_vi = NULL;
                       lhs_vi = cs_first_vi_for_offset (cs_get_varinfo (dirval), lhs.offset);
                       if (lhs_vi)
			{
                           lhsvar = lhs_vi->id;
                           add_to_df_list (lhs_vi->id, kill);
                           lhs_live |= remove_from_df_list (lhs_vi->id, gen);
			}
		   }
                  else{
                             /*lhsvar = di->rhs->val;*/
                        lhsvar = dirval;
                        add_to_df_list (dirval, kill);
                        lhs_live |= remove_from_df_list (dirval, gen);
                 }
                       /*add_to_df_list (lhsvar, kill);
                       lhs_live |= remove_from_df_list (lhsvar, gen);*/
             }

           }
           /* The LHS is by default live. */
           add_to_df_list (lhs.var, gen);
       }

       /* Generate liveness for RHS if required. */
       if (rhs.type == SCALAR && lhsvar != -1) {
           if (lhs_live == false && linfo != -1)
               lhs_live |= is_var_live (linfo, lhsvar);
           if (lhs_live)
               add_to_df_list (rhs.var, gen);
		
       }

       /* If RHS is deref, the RHS and its pointees are live. */
       if (rhs.type == DEREF && lhsvar != -1) {
           if (lhs_live == false && linfo != -1)
               lhs_live |= is_var_live (linfo, lhsvar);
           if (lhs_live) {
               /*pointsto_val di, tmp;*/
               add_to_df_list (rhs.var, gen);
               /* Add its pointees to gen. */
                       pointee_set = get_pointee_set((*ci)->pinfo, maylist, rhs.var);
                        EXECUTE_IF_SET_IN_BITMAP(pointee_set,0,eival,ei){
                           add_to_df_list (eival, gen);
                        }



           }
       }
       init = false;
   }

}


static int
compute_liveness_sub (int lin, int kill_index)
{
   /*int size = get_live_set_size(kill_index);
   int i =0;
   while(i<size){
	unsigned int val = get_var_at_index_in_liveset(kill_index,i);
	lin = remove_var_from_live_set(lin,val);
	i++;
    }*/

    bitmap live_set = get_live_set(kill_index);
    bitmap_iterator bir;
    unsigned int var;
    EXECUTE_IF_SET_IN_BITMAP(live_set,0,var,bir) {
                lin = remove_var_from_live_set(lin,var);
    }



  return lin;
}



	  
static int 
compute_live_in (int lin, int gen_i, int kill_i)
{

   lin = compute_liveness_sub(lin, kill_i);
   if (lin < 0 ) 
	return gen_i;
    else {
       lin = compute_df_list_union (lin, gen_i);
       return lin;
   }

}

/* Compute live in, given live out and the constraints. */
static void 
find_live_vars (bool recompute)
{
   csdfa_info ci;
   constraint_list bi;

   /* If recompute is set, perform copying. */
   if (recompute)
       copy_out_to_in_1 ();
 
   for (ci=in_of_bb (cbb); ci; ci=ci->next) {
       if (ci->new_lvdf || recompute) {
	    int gen_i = -1 , kill_i = -1;
           /* Iterate over all the constraints of bb and compute gen & kill. */
           for (bi=get_constraint_end (cbb); bi; bi=bi->prev) {
               if (bi->is_alias){
                    compute_liveness_from_constraint (&ci, bi, &gen_i, &kill_i, ci->linfo);		   
		}
		
               else {
                   df_list ai;
                   for (ai=bi->stmt_constraint_chain; ai; ai=ai->next){
                                add_to_df_list (ai->val, &gen_i);

		   }
               }
           }
	/*if(dump_file)// && !ipacs_time)
	{
        fprintf(dump_file,"\n\nliveness info :");
        print_df_list(ci->linfo);
        fprintf(dump_file,"\n\nliveness geninfo :");
        print_df_list(gen_i);
        fprintf(dump_file,"\n\nliveness kill info :");
        print_df_list(kill_i);
   }*/
	ci->linfo = compute_live_in (ci->linfo, gen_i, kill_i);
       }
   } 
}

static void
perform_lvipdfa_on_startblock (bool recompute)
{
   basic_block end_bb = end_bb_of_fn (cnode);
  // if(dump_file) && !ipacs_time) 
	//{fprintf(dump_file,"\nperform_lvipdfa_on_startblock\n");}
   copy_out_to_in ();
   find_live_vars (recompute);
   
   if (cbb != main_firstbb) {
       if (lcsmap_required (cnode)) {
           perform_regeneration_at_startblock (end_bb);
           ((cgraph_info)(cnode->aux))->lcsmap_required = false;
       }
       else {
           /*if (dump_file && !ipacs_time)
               fprintf (dump_file, "\nliveness regeneration not required at %s node's %d block\n",
                 cgraph_node_name (cnode), cbb->index);*/
       }

       /* Propagate changes to respective blocks. */
       propagate_liveness_to_callblock (recompute);
   }

   if (dump_file)// && !ipacs_time)
       print_bb_linfo (cbb);
}

static void
perform_lvipdfa_on_intrablock (bool recompute)
{
   csdfa_info ai;

   /* If not an endblock, copy out to in. */
   if (cbb != end_bb_of_fn (cnode)) {
	//if(dump_file)// && !ipacs_time) 
		//{fprintf(dump_file,"\nIN IF\n");}
       if (!get_constraint_end (cbb)){
           ((bb_csipdfa_info)(cbb->aux))->in_ipdfa = out_of_bb (cbb);
           //if(dump_file)// && !ipacs_time) 
		//{fprintf(dump_file,"\nIN IF 1\n");}

	}
       else {
           copy_out_to_in ();
         // if(dump_file)// && !ipacs_time) 
		//{fprintf(dump_file,"\nIN IF 2\n");}

	}
   }
   /* If an end block, computed in from mapping. */
   else {
       // if(dump_file)// && !ipacs_time) 
		//{fprintf(dump_file,"\nIN ELSE\n");}
       if (cbb == end_bb_of_fn (cnode_of_bb (main_firstbb))){
         //if(dump_file)// && !ipacs_time) 
		//{fprintf(dump_file,"\nIN ELSE 1\n");}
	   copy_out_to_in ();
	}
       else {
         //if(dump_file)// && !ipacs_time) 
		//{fprintf(dump_file,"\nIN ELSE 2\n");}

	   regenerate_pinfo_for_lcsmap_lhs (cnode_of_bb (cbb));
           compute_mapped_in_at_endblock ();
       }
   }
  
   if (get_constraint_end (cbb))
       find_live_vars (recompute);

   propagate_liveness_to_all_preds (cbb, recompute);

   for (ai=in_of_bb (cbb); ai; ai=ai->next)
       ai->new_lvdf = false;
   
   if (dump_file)// && !ipacs_time)
       print_bb_linfo (cbb);
}

static int
remove_non_local_vars (int index)
{
   
    int in = index;
    bitmap live_set;
    bitmap_iterator bir;
    unsigned int var;
 
   if(index < 0)
	return -1;

    live_set = get_live_set(index);
    EXECUTE_IF_SET_IN_BITMAP(live_set,0,var,bir) {
        if (!(var_defined_in_cfun (var) || global_var (var))) {
                in = remove_var_from_live_set(in,var);
        }
    }

   return in;

  
}

static void
compute_call_stmt_in (csdfa_info li)
{

   csdfa_info ai, prev=NULL;
   int linfo = remove_non_local_vars (li->linfo);
   //if(dump_file)// && !ipacs_time) 
	//{fprintf(dump_file,"\ncompute_call_stmt_in\n");}

   for (ai=in_of_bb (cbb); ai; ai=ai->next) {
       if (li->cs_index >= ai->cs_index)
	   break;
       prev = ai;
   }
   if (ai && (ai->cs_index == li->cs_index)) {
       int old_linfo = ai->linfo;
       ai->linfo = compute_lout_union (ai->linfo, linfo);
       if (old_linfo != ai->linfo)
           ai->new_lvdf = true;
   }
   else {
       csdfa_info temp = copy_csdfa_info (li);
       temp->linfo = linfo;
       if (prev == NULL) {
           temp->next = in_of_bb (cbb);
           ((bb_csipdfa_info)(cbb->aux))->in_ipdfa = temp;
       }
       else {
           temp->next = prev->next;
           prev->next = temp;
       }
   }
}

static int
compute_live_parms (int index)
{
   constraint_list bi;

 
   if (!get_constraint_end (cbb))
       return index;
    

   for (bi=get_constraint_end (cbb); bi; bi=bi->prev) {
       if (bi->is_alias) {
           constraint_t t = VEC_index (constraint_t, aliases, bi->stmt_constraint_chain->val);
	   /* If the RHS isn't addressof, add it to live. */
	   if (t->rhs.type != ADDRESSOF)
                add_to_df_list (t->rhs.var, &index);
       }
       else {
           df_list ai;
           for (ai=bi->stmt_constraint_chain; ai; ai=ai->next)
               add_to_df_list (ai->val, &index);
       }
   }
  return index;
}

static void
compute_call_stmt_liveness (void)
{
   csdfa_info ai;
  //if(dump_file)// && !ipacs_time) 
	//fprintf(dump_file,"\ncompute_call_stmt_liveness\n");

   for (ai=out_of_bb (cbb); ai; ai=ai->next) {
       if (ai->new_lvdf) {
         /*    if(dump_file && !ipacs_time) {fprintf(dump_file,"\ncompute_call_stmt\nBEFORE\nlinfo : "); print_df_list(ai->linfo);
                }*/

	   ai->linfo = compute_live_parms (ai->linfo);
            /* if(dump_file && !ipacs_time) {fprintf(dump_file,"\nafter compute live params\nlinfo : "); print_df_list(ai->linfo);
                }*/

	   compute_call_stmt_in (ai);
             /*if(dump_file && !ipacs_time) {fprintf(dump_file,"\nfinal\nlinfo : "); print_df_list(ai->linfo);
                }*/

	   ai->new_lvdf = false;
       }    
   }
}

static void
perform_lvipdfa_on_callblock (bool recompute)
{
   csdfa_info ai;
  /* if(dump_file)// && !ipacs_time) 
	fprintf(dump_file,"\n***perform lvipdfa on callblock***");*/

   compute_call_stmt_liveness ();    

   if (!start_block (cbb)) {
       propagate_liveness_to_all_preds (cbb,recompute);
       for (ai=in_of_bb (cbb); ai; ai=ai->next)
           ai->new_lvdf = false;
   }
   else if (cbb != main_firstbb) {
       if (lcsmap_required (cnode)) {
           perform_regeneration_at_startblock (end_bb_of_fn (cnode));
           ((cgraph_info)(cnode->aux))->lcsmap_required = false;
       }
       else {
           /*if (dump_file && !ipacs_time)
               fprintf (dump_file, "\nliveness regeneration not required at %s node's %d block\n",
                    cgraph_node_name (cnode), cbb->index);*/
       }
       /* Propagate changes to respective blocks. */
       propagate_liveness_to_callblock (recompute);
   }
   
   if (dump_file)// && !ipacs_time)
       print_bb_linfo (cbb);
}
  
static void
copy_in_at_returnblock (gimple stmt)
{
   VEC(ce_s, heap) *lhsc = NULL;
   tree lhsop = gimple_call_lhs (stmt);
   csdfa_info ai;

   //if(dump_file)// && !ipacs_time) 
	//fprintf(dump_file,"\ncopy_in_at_returnblock\n");
   if (lhsop && field_must_have_pointers (lhsop)) 
       cs_get_constraint_for (lhsop, &lhsc);

   for (ai=out_of_bb (cbb); ai; ai=ai->next) {
       if (ai->new_lvdf == true) {
           csdfa_info bi = copy_csdfa_info (ai);
	   csdfa_info ci, prev=NULL;
	   if (lhsc) {
               unsigned int i;
	       struct constraint_expr *lhs;
               if (bi->linfo >= 0) {
		   int temp = bi->linfo;
                   /*df_list temp = (copy_df_list (VEC_index (df_list, liveinfo, bi->linfo)));*/
                   FOR_EACH_VEC_ELT (ce_s, lhsc, i, lhs) {
		       if (lhs->type == SCALAR) {
                           csvarinfo_t lhs_vi = NULL;
                           /* Reduce LHS if its a scalar. */
                           if (!unexpandable_var (lhs->var, lhs->offset)) {
                               lhs_vi = cs_first_vi_for_offset (cs_get_varinfo (lhs->var), lhs->offset);
                               /* If lhs_vi is NULL, the lhs does not give any valid information. */
                               if (lhs_vi) 
                                  remove_from_df_list (lhs_vi->id, &temp);
                           }
			   else 
			       remove_from_df_list (lhs->var, &temp);
		       }
		       else
                           remove_from_df_list (lhs->var, &temp);
                       /*bi->linfo = compute_liveness_index (temp);*/
		        bi->linfo = temp;
                   }
               }
           }
                       
           for (ci=in_of_bb (cbb); ci; ci=ci->next) {
               if (bi->cs_index >= ci->cs_index)
                   break;
               prev = ci;
           }
           if (ci && (ci->cs_index == bi->cs_index)) {
	       int old_linfo = ci->linfo; 
               ci->linfo = compute_lout_union (ci->linfo, bi->linfo);
	       if (old_linfo != ci->linfo)
                   ci->new_lvdf = true;
	       ggc_free (bi);
           }
           else {
               if (prev == NULL) {
                   bi->next = in_of_bb (cbb);
                   ((bb_csipdfa_info)(cbb->aux))->in_ipdfa = bi;
               }
               else {
                   bi->next = prev->next;
                   prev->next = bi;
               }
           }
           ai->new_lvdf = false;
       }
   }
   VEC_free (ce_s, heap, lhsc);
}

static void
perform_lvipdfa_on_returnblock (bool recompute)
{
   basic_block caller_bb = single_pred (cbb);
   gimple stmt = bb_call_stmt (caller_bb);
   tree decl = get_called_fn_decl (stmt);
   bool add_to_worklist = false;
   csdfa_info ai;

   /* Kill the return assignment at in. */
   copy_in_at_returnblock (stmt);  
 
   /* Process the call. */ 
   if (TREE_CODE (decl) == FUNCTION_DECL) {
       struct function *called_function = DECL_STRUCT_FUNCTION (decl);
       struct cgraph_node *node = cnode_of_bb (EXIT_BLOCK_PTR_FOR_FUNCTION (called_function)->prev_bb);
       basic_block end_bb = end_bb_of_fn (node);
       csdfa_info ai;
       for (ai=in_of_bb (cbb); ai; ai=ai->next) {
           if (ai->new_lvdf) {
              add_to_worklist |= propagate_liveness_to_endblock (node, end_bb, ai, recompute);}
       }
       if (add_to_worklist) {
           set_bb_to_process_lv (node, end_bb);
           add_to_fn_worklist (node);
       }
   }
   else {
 
     process_fptr_call_liveness (caller_bb, decl, recompute); 

   }

   propagate_liveness_to_all_preds (cbb, recompute);

   for (ai=in_of_bb (cbb); ai; ai=ai->next)
       ai->new_lvdf = false;
 
   if (dump_file)// && !ipacs_time)
       print_bb_linfo (cbb);
}
 
static void
init_ipacs (void)
{
   if (main_firstbb) {
       cnode = cnode_of_bb (main_firstbb);
       initialize_fn_worklist ();
       ((bb_csipdfa_info)(end_bb_of_fn (cnode)->aux))->out_ipdfa = initialize_out ();
       set_bb_to_process_lv (cnode, end_bb_of_fn (cnode));
   }
}

static void
compute_liveness (bool recompute)
{
   /* Get the first node to being liveness pass. */
   cnode = first_node_from_lv_worklist ();
   
   /* If cnode is NULL, we are done with liveness computation. */
   while (cnode) {
       bb_worklist bb_list = worklist_of_cnode (cnode);
       bool process_bb = true;
       while (process_bb) {
           bool new_cnode = false;
           /*if (dump_file && !ipacs_time)
               print_lv_worklist ();*/
           cbb = get_bb_for_lv_processing (bb_list, &new_cnode);
	   if (!new_cnode) {
               if (cbb) {
                   if (call_block (cbb))
                       perform_lvipdfa_on_callblock (recompute);
                   else if (return_block (cbb))
                       perform_lvipdfa_on_returnblock (recompute);
                   else if (start_block (cbb)) {
                       perform_lvipdfa_on_startblock (recompute);
		       if (lv_callee_nodes_processed ()) {
                           cnode = prev_node_from_lv_worklist ();
                           process_bb = false;
		       }
                   }
                   else
                       perform_lvipdfa_on_intrablock (recompute);
               }
               else {
                   cnode = next_node_from_lv_worklist (lv_node);
                   process_bb = false;
	       }
           }
	   else
	       process_bb = false;
       }
   }
}



static void
print_final_info(void)
{

struct cgraph_node *node;
        basic_block bb;
        csdfa_info lin, lout;
        for (node = cgraph_nodes; node; node=node->next)
        {
                if (!gimple_has_body_p (node->decl) || node->clone_of)
                    continue;
                 push_cfun (DECL_STRUCT_FUNCTION (node->decl));
                 FOR_EACH_BB (bb)
                 {
                         /*if (dump_file && bb) {
                           fprintf(dump_file,"\n\nfinal liveness information for function %s and basic block %d is \n",cgraph_node_name(node),bb->index);
                           for(lin = in_of_bb(bb);lin;lin=lin->next)
                           {
                                fprintf(dump_file,"\nCallstring : "); print_callstring_sequence(lin->cs_index);
                                fprintf(dump_file,"\nlivein : "); print_df_list(lin->linfo);

                           }

                           for(lout = out_of_bb(bb);lout;lout=lout->next)
                           {
                                fprintf(dump_file,"\nCallstring : "); print_callstring_sequence
(lout->cs_index);
                                fprintf(dump_file,"\nliveout : "); print_df_list(lout->linfo);

                           }

                        }
			*/
		     if (dump_file && bb) {
                           fprintf(dump_file,"\n\nfinal liveness information for function %s and basic block %d is \n",cgraph_node_name(node),bb->index);
                           for(lin = in_of_bb(bb);lin;lin=lin->next)
                           {

                                for(lout = out_of_bb(bb);lout;lout=lout->next)
                                {

                                        if(lin->cs_index == lout->cs_index) {
		                                fprintf(dump_file,"\nCallstring : "); print_callstring_sequence(lin->cs_index);
                		                fprintf(dump_file,"\nlivein : "); print_df_list(lin->linfo);

                                                fprintf(dump_file,"\nCallstring : "); print_callstring_sequence(lout->cs_index);
                                                fprintf(dump_file,"\nliveout : "); print_df_list(lout->linfo);
						break;
                                        }

                                }
                          }     

                           /*fprintf (dump_file, "\n : %s %d : %d\n",cgraph_node_name(node),bb->index,count_for_each_bb(bb));*/
                     }

                }


        pop_cfun ();
       }

}
                            


/*------------------------------------------------------------
  The driver function for pointsto analysis using callstrings.
  -----------------------------------------------------------*/
static unsigned int
execute_ipacs (void)
{
   /* Preserve the context before function execution. */
   struct function *old_cfun = cfun;
   tree old_cfundecl = current_function_decl;
   //printf("start\n");
   //if (ipacs_time)
       gettimeofday (&tstart, NULL);
   init_info ();
   preprocess_cfg ();
   init_ipacs ();
   compute_liveness (false);
   compute_pointsto ();
   //if(dump_file)// && !ipacs_time) 
	//print_final_info();
   restore_cfg ();
   delete_ipacs_info ();
   if (dump_file)// && ipacs_time) 
   {
       gettimeofday (&tend, NULL);
       timersub (&tend, &tstart, &tresult);
       fprintf (dump_file, "Time taken = %ld seconds, %6ld microseconds\n", tresult.tv_sec, tresult.tv_usec);
   }
   /* Restore the context after function finishes. */
   current_function_decl = old_cfundecl;
   set_cfun (old_cfun);
	//printf("end\n");
	print_liveness();	
   return 0;
}

/* -----------------------------------------
   Return true if we should execute IPA PTA.
   ----------------------------------------*/
/*static bool
gate_ipacs (void)
{
   return (flag_ipacs && !(errorcount || sorrycount));
}*/

/* -------------------------------
   The callstrings pointsto pass. 
   ------------------------------*/
	


struct simple_ipa_opt_pass pass_ipacs =
{
  {
    SIMPLE_IPA_PASS,
    "lfcpa",                                /* name */
    NULL,	                            /* gate */
    execute_ipacs,		            /* execute */
    NULL,                                   /* sub */
    NULL,                                   /* next */
    0,                                      /* static_pass_number */
    TV_INTEGRATION,                         /* tv_id */
    0,                                      /* properties_required */
    0,                                      /* properties_provided */
    0,                                      /* properties_destroyed */
    0,                                      /* todo_flags_start */
    0                                       /* todo_flags_finish */
  }
};
struct register_pass_info pass_info = 
{
	&(pass_ipacs.pass),
	"pta",
	0,
	PASS_POS_INSERT_BEFORE
};
int plugin_init(struct plugin_name_args *plugin_info, struct plugin_gcc_version *version)
{
	register_callback(
		plugin_info->base_name,
		PLUGIN_PASS_MANAGER_SETUP,
		NULL,
		&pass_info);
	return 0;
}
