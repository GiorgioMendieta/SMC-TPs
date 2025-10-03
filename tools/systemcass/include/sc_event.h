/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 sc_event.h                        |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   09_07_2004                      |
|                                                             |
\------------------------------------------------------------*/
#ifndef __SC_EVENT_H__
#define __SC_EVENT_H__

#include"sc_fwd.h"

namespace sc_core {

class sc_event {
public : typedef enum { POS, NEG, VAL } flag_t;
private:
	const sc_interface   &interface;
	flag_t flag;
public:
	bool pos () const;
  sc_event (const sc_interface &, flag_t);
  ~sc_event ();

  // methods
  const sc_interface&  get_interface() const { return interface;}; // CASC specific
  flag_t              get_flag () const { return flag;}; // CASC specific
  
  // operators

  friend std::ostream& operator << (std::ostream &, const sc_event &);
	
  sc_event ();
	/*
	// LRM
  sc_event ();
private:
  // disabled
  sc_event (const sc_event &);
  sc_event& operator = (const sc_event&);  
	*/
};

} // end of sc_core namespace

#endif /* __SC_EVENT_H__ */
