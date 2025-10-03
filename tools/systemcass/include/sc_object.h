/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 sc_object.h                       |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   09_07_2004                      |
|                                                             |
\------------------------------------------------------------*/
#ifndef __SC_OBJECT_H__
#define __SC_OBJECT_H__

#include <iostream>
#include <vector>

// ----------------------------------------------------------------------------
//  CLASS : sc_object
//
// ----------------------------------------------------------------------------

namespace sc_core {

class sc_object
{
	///////////
	// Internal
	protected: void             init ();
	protected: void             set_kind (const char *);
	///////////		 
public:
  static const char* const kind_string;
  // LRM
  const char *name     () const;
  const char *basename () const;
  void        rename   (const char*) const;

	friend std::ostream& operator << (std::ostream& , const sc_object&);
  /*
  void print () const;
  virtual void print (std::ostream & os) const;
  void dump () const;
  virtual void dump (std::ostream & os) const;
  virtual void trace (sc_trace_file *) const;
  */
  /*virtual*/ const char *kind () const;
  /*
  sc_simcontext *simcontext () const;
  bool add_attribute (sc_attr_base &);
  sc_attr_base *get_attribute (const sc_string &);
  const sc_attr_base *get_attribute (const sc_string &) const;
  sc_attr_base *remove_attribute (const sc_string &);
  void remove_all_attributes ();
  int num_attributes () const;
    sc_attr_cltn & attr_cltn ();
  const sc_attr_cltn & attr_cltn () const;
  */
  /* virtual */ const std::vector<sc_object*>& get_child_objects() const;
  sc_object*               get_parent_object       () const;
protected:
    sc_object ();
    sc_object (const char *);
    /*virtual */~ sc_object ();
		
};

const std::vector<sc_object*>& sc_get_top_level_objects();
const sc_object*               sc_find_object          (const char*);

} // end of sc_core namespace

using sc_core::sc_get_top_level_objects;
using sc_core::sc_find_object;

#endif

