/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 sc_interface.h                    |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   09_07_2004                      |
|                                                             |
\------------------------------------------------------------*/
#ifndef __SC_INTERFACE_H__
#define __SC_INTERFACE_H__

#include"sc_fwd.h"
#include"internal_ext.h"

namespace sc_core {

// ----------------------------------------------------------------------------
//  CLASS : sc_interface
//
//  
// ----------------------------------------------------------------------------
class sc_interface
{ 
	///////////
	// Internal
	private:  tab_t              *pointer; // pointeur dans la table de signaux 
                                         // NULL pour les sc_out/sc_inout
  public:   inline tab_t       *get_pointer () const   { return pointer; }
  public:   inline void         set_pointer (tab_t *i) { pointer = i; }
	public:   size_t              data_size_in_bytes () const; /* nb of bytes */
	protected:void                init        (size_t) const;
	///////////
					 
	//////
	// LRM
public:
					 /*
  virtual void register_port (sc_port_base & port_, const char *if_typename_);
	*/
  /*virtual */const sc_event & default_event () const;
  /*virtual */~ sc_interface ();
protected:
  // constructor 
    sc_interface ();
private:
  // disabled 
    sc_interface (const sc_interface &);
    sc_interface & operator = (const sc_interface &);
  //////
};

} // end of sc_core namespace

#endif /* __SC_INTERFACE_H__ */
