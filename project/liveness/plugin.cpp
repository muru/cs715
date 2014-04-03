/*-----------------------------------------------------------------------------
 *  "gcc-plugin.h" must be the FIRST file to be included 
 *-----------------------------------------------------------------------------*/
#include "gcc-plugin.h"

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
//#include "tm_p.h"
#include "diagnostic.h"
#include "tree-flow.h"
#include "tree-pass.h"
#include "toplev.h"
#include "gimple-pretty-print.h"
#include "cgraph.h"
#include "coretypes.h"
#include "tree.h"
#include "diagnostic.h"
#include "timevar.h"
#include "alloc-pool.h"
#include "params.h"
#include "ggc.h"
#include "vec.h"
//#include "df.h"
#include "bitmap.h"
#include "tree-ssa-operands.h"
#include "tree-flow-inline.h"

/*-----------------------------------------------------------------------------
 *  Each plugin MUST define this global int to assert compatibility with GPL; 
 *  else the compiler throws a fatal error 
 *-----------------------------------------------------------------------------*/
int plugin_is_GPL_compatible;
static unsigned int print_liveness(void);
static void process_function (void);
/*-----------------------------------------------------------------------------
 *  Structure of the pass we want to insert, identical to a regular ipa pass
 *-----------------------------------------------------------------------------*/
struct simple_ipa_opt_pass pass_plugin = {
{
    SIMPLE_IPA_PASS,
    "liveness",               /*  name */
    NULL,                            /*  gate */
    print_liveness,	  /* execute */
    NULL,                         /*  sub */
    NULL,                         /*  next */
    0,                            /*  static pass number */
    TV_INTEGRATION,               /*  tv_id */
    0,                            /*  properties required */
    0,                            /*  properties provided */
    0,                            /*  properties destroyed */
    0,                            /*  todo_flags start */
    0                             /*  todo_flags end */
  }
};



/*-----------------------------------------------------------------------------
 *  This structure provides the information about inserting the pass in the
 *  pass manager. 
 *-----------------------------------------------------------------------------*/
struct register_pass_info pass_info = {
  &(pass_plugin.pass),            /* Address of new pass, here, the 'struct
                                     opt_pass' field of 'simple_ipa_opt_pass'
                                     defined above */
  "pta",               /* Name of the reference pass for hooking up
                                     the new pass.  */
  0,                              /* Insert the pass at the specified instance
                                     number of the reference pass. Do it for
                                     every instance if it is 0. */
  PASS_POS_INSERT_AFTER           /* how to insert the new pass: before,
                                     after, or replace. Here we are inserting
                                     a pass names 'plug' after the pass named
                                     'pta' */
};




/*-----------------------------------------------------------------------------
 *  plugin_init is the first function to be called after the plugin is loaded
 *-----------------------------------------------------------------------------*/
int plugin_init(struct plugin_name_args *plugin_info,struct plugin_gcc_version *version)
{

  /*-----------------------------------------------------------------------------
   * Plugins are activiated using this callback 
   *-----------------------------------------------------------------------------*/
   register_callback (
      plugin_info->base_name,     /* char *name: Plugin name, could be any
                                     name. plugin_info->base_name gives this
                                     filename */
      PLUGIN_PASS_MANAGER_SETUP,  /* int event: The event code. Here, setting
                                     up a new pass */
      NULL,                       /* The function that handles event */
      &pass_info);                /* plugin specific data */

  return 0;
}

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
 fprintf(dump_file,"num_ssa_name=%d\n",num_ssa_names);

 fprintf (dump_file, "\n\npoints-to information\n\n");
 int i;
   for (i = 1; i < num_ssa_names; i++)
     {
       tree ptr = ssa_name (i);
       struct ptr_info_def *pi;
	//fprintf(dump_file,"num_ssa_name=%d\n",num_ssa_name); 
       if (ptr == NULL_TREE
           || SSA_NAME_IN_FREE_LIST (ptr))
         continue;
 
       pi = SSA_NAME_PTR_INFO (ptr);
       if (pi)
         dump_points_to_info_for (dump_file, ptr);
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
			}}}
 return;
}

