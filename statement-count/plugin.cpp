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
#include <map>
#include <set>

/*-----------------------------------------------------------------------------
 *  Each plugin MUST define this global int to assert compatibility with GPL; 
 *  else the compiler throws a fatal error 
 *-----------------------------------------------------------------------------*/
int plugin_is_GPL_compatible;
std::map<const char*, int> stmts;
static unsigned int inter_gimple_manipulation (void);
static int process_function(void);
/*-----------------------------------------------------------------------------
 *  Structure of the pass we want to insert, identical to a regular ipa pass
 *-----------------------------------------------------------------------------*/
struct simple_ipa_opt_pass pass_plugin = {
{
    SIMPLE_IPA_PASS,
    "total_stmt_count",               /*  name */
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
	fprintf(dump_file, "Function\t Statement Count (-1 ~ inf)\n");
	for (std::map<const char*, int>::const_iterator itr = stmts.begin(); itr != stmts.end(); itr++)
	{
		func_pointer = itr->first;
		count        = itr->second;
		total = count < 0 || total < 0 ? -1 : total + count;
		fprintf(dump_file, "%s\t\t%d\n", func_pointer, count);
	}
	fprintf(dump_file, "Total:\t\t%d\n\n\n", total);
	/** TODO: Stop GCC from printing every GIMPLE statement after our output.
	 */
	return 0;
}		

static int process_function (void)
{
	int count = 0;
	basic_block bb;
	gimple_stmt_iterator si;
	const char * func_pointer = IDENTIFIER_POINTER(DECL_NAME(cfun->decl));
	static std::set<const char*> call_stack;
	
	/** 
	 * Firstly, we can check if the function has already been processed.
	 * If so, we can skip the calculation. Note that, for an entry to
	 * exist in stmts, the calculation must have terminated once, which
	 * implies this function doesn't have recursion, or, if it did recurse,
	 * has a count of -1.
	 *
	 * Then, we check to see if the call_stack contains this function.
	 * If so, then there is (direct or indirect) recursion happening.
	 * In which case, we return -1 (~ infinity) immediately.
	 */
	if (stmts.count(func_pointer) != 0)
		return count = stmts[func_pointer];

	if (!call_stack.insert(func_pointer).second)
		return fprintf(stderr, "Recursive call to %s detected.\n", func_pointer), -1;

	FOR_ALL_BB (bb) 
	{
		for (si = gsi_start_bb (bb); !gsi_end_p (si); gsi_next(&si))
		{
			count++;
			gimple stmt = gsi_stmt (si);
			
			/*fprintf(dump_file, "%d\t", count);
			print_gimple_stmt(dump_file, stmt, 0, TDF_SLIM);*/
			/**
			 * Check for gimple call statements, see if the FUNCTION_DECL
			 * of the called function is available (it may not be for certain
			 * types of functions) and recursively call process_function()
			 * for available functions.
			 */
			if ( (void_type_node != gimple_expr_type(stmt)) 
					&& (CALL_EXPR == gimple_expr_code(stmt)) )
			{
				tree fn (gimple_call_fndecl(stmt));
				if (!fn)
					fprintf(stderr, "NULL.\n");
				else if (gimple_has_body_p (fn))
				{
					int sub_count;

					push_cfun(DECL_STRUCT_FUNCTION (fn));
					sub_count = process_function();
					pop_cfun();

					if (sub_count == -1)			// Recursion?
					{								// Recursion!
						count = -1;
						break;
					}
					else 
						count += sub_count - 1;		// count has already been incremented.
				}
			}
		}
		if (count == -1)
			break;
	}

	call_stack.erase(func_pointer);
	stmts.insert(std::make_pair(func_pointer, count));
	return count;
}
