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
#include "libiberty/hashtab.h"
#include <map>
#include <set>

/*-----------------------------------------------------------------------------
 *  Each plugin MUST define this global int to assert compatibility with GPL; 
 *  else the compiler throws a fatal error 
 *-----------------------------------------------------------------------------*/
int plugin_is_GPL_compatible;
static unsigned int inter_gimple_manipulation (void);
static int process_function(void);
static int old_n = 0;
static int n = 0;
static std::map<tree, const char *> syms;
typedef struct var_id_def
{
	long func_decl_uid;
	long var_decl_uid;
	long var_version;
	long bb_num;
} var_id;

/*-----------------------------------------------------------------------------
 *  Structure of the pass we want to insert, identical to a regular ipa pass
 *-----------------------------------------------------------------------------*/
struct simple_ipa_opt_pass pass_plugin = {
{
    SIMPLE_IPA_PASS,
    "ssa_names",               /*  name */
    NULL,                            /*  gate */
    inter_gimple_manipulation,	  /* execute */
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
  "pta",                          /* Name of the reference pass for hooking up
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

static unsigned int inter_gimple_manipulation (void)
{
	int count, total = 0;
	const char * func_pointer;
	struct cgraph_node *node;

	for (node = cgraph_nodes; node; node = node->next)
	{
		//if (!gimple_has_body_p (node->decl) || node->clone_of)
		//	continue;
		//fprintf(stderr, "%s\n", IDENTIFIER_POINTER(DECL_NAME(node->decl)));
		if (gimple_has_body_p(node->decl))
		{
			push_cfun (DECL_STRUCT_FUNCTION (node->decl));
			process_function();
			pop_cfun();
		}
	}
	/*
	fprintf(dump_file, "Function\t Statement Count (-1 ~ inf)\n");
	for (std::map<const char*, int>::const_iterator itr = stmts.begin(); itr != stmts.end(); itr++)
	{
		func_pointer = itr->first;
		count        = itr->second;
		total = count < 0 || total < 0 ? -1 : total + count;
		fprintf(dump_file, "%s\t\t%d\n", func_pointer, count);
	}
	fprintf(dump_file, "Total:\t\t%d\n\n\n", total);
	*/
	/** TODO: Stop GCC from printing every GIMPLE statement after our output.
	 */
	return 0;
}		

static int process_function (void)
{
	n += num_ssa_names;
	tree name;

	fprintf (stderr, "Num ssa_names: %d\n", n);
	for (int i = 0; i < num_ssa_names; i++)
	{
		name = ssa_name (i);
		if (!name)
		{
			fprintf (dump_file, "NULL ssa_name.\n");
			continue;
		}
		print_generic_expr (dump_file, name, TDF_SLIM);
		tree sym = SSA_NAME_VAR (name);
		if (!sym)
			fprintf (dump_file, " has no symbol.\n");
		else 
		{
			if (DECL_NAME (sym))
			{
				syms.insert (std::make_pair (name, IDENTIFIER_POINTER (DECL_NAME (sym))));
				fprintf (stderr, "%s\n", IDENTIFIER_POINTER (DECL_NAME (sym)));
			}
			else
				fprintf (stderr, "NULL DECL_NAME: %d\n", DECL_UID (sym));
			fprintf (dump_file, " is SSA_NAME for: ");
			print_generic_expr (dump_file, sym, TDF_SLIM);
			fprintf (dump_file, " with version: %d", SSA_NAME_VERSION (name));
			fprintf (dump_file, " with UID: %d, sym UID: %d", DECL_UID (name), DECL_UID (sym));
			fprintf (dump_file, "\n");
		}
	}
}
