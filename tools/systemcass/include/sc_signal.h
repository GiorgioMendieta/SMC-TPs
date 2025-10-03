/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                   sc_signal.h                     |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                           Taktak Sami                       |
|                                                             |
| Date    :                   09_07_2004                      |
|                                                             |
\------------------------------------------------------------*/
#ifndef __SC_SIGNAL_H__
#define __SC_SIGNAL_H__

// Define registers writing method
#include<iostream>
#include"sc_fwd.h"
#include"sc_nbdefs.h"
//#include"sc_event_finder.h"
//#include"sc_event.h"
#include"sc_time.h" // SC_ZERO_TIME
#include"sc_object.h"
#include"sc_interface.h"
#include"internal_ext.h"
#include "fsm_rules.h"

namespace sc_core {

//
#if ((__GNUC__ < 3) || (__GNUC_MINOR__ < 4))
#define INLINE __attribute__((always_inline))
#else
/* gcc3.4 doesn't support */ 
#define INLINE
#endif

#define READ_SIGNAL(value_type_,pointer_) \
	((value_type_&) (*((value_type_*) (pointer_))))

	///////////////////// DEPRECATED
// C ANSI-only since it is needed to link with extern "C"
// this declaration is not in casc.h since the CHECK_FSM_RULES macro
// is not defined.

extern void bind (sc_port_base&,sc_port_base&);
extern void bind (sc_port_base&,sc_signal_base&);
extern void bind (sc_signal_base &x);
extern void bind (sc_port_base   &x);
typedef tab_t base_type;
struct pending_write {
	base_type   *pointer;
	base_type    value;
	//pending_write (base_type *const pointer_, const base_type value_)
	//{	pointer = pointer_; value = value_; }
	friend std::ostream& operator << (std::ostream &o, const pending_write &p)
	{ return o << "(pointer = " << p.pointer << "; value = " << p.value << ")\n"; }
};

// Check pending_writing to register
extern void pending_writing2register_clear  ();
extern void pending_writing2register_record_and_check (const tab_t *);

// Pending write to register (simple stack)
typedef pending_write *pending_write_vector_t;
extern pending_write_vector_t pending_write_vector;
extern "C" unsigned int pending_write_vector_nb;
extern unsigned int pending_write_vector_capacity;


template <typename T>
inline void post_write (base_type *const pointer_,
                        const T          value_) /*INLINE*/;
template <typename T>
inline void post_multiwrite (base_type *const pointer_,
                             const T          value_)
{
	size_t size = (sizeof (T)-1) / sizeof (base_type);
	size_t i = 0;
	const base_type *pvalue = (const base_type*)(void*)(&value_);
	do {
#if 0
    cout << "post_multiwrite 0x" << hex << pvalue[i] << " @" << (pointer_ + i) << "\n";
#endif
		post_write (pointer_ + i, pvalue[i]);
	} while (i++ < size);
}
template <typename T>
inline void post_write (base_type *const pointer_, 
                        const T          value_)
{
	if (sizeof (T) > sizeof (base_type)) {
#if 0
    cout << "sizeof (T) = " << sizeof (T) << " (base_type = " << sizeof
(base_type) << "\n";
#endif
		post_multiwrite (pointer_,value_);
	} else {
#if defined(DEBUG)
	if (pending_write_vector_nb >= pending_write_vector_capacity) {
	//if (pending_write_vector_nb >= pending_write_vector_capacity * sizeof(pending_write)) {
		std::cerr << "Error : The array for posted writing on register is too small.\n";
		std::cerr << "Up to 1 writing per register is allowed during a cycle.\n";
		std::cerr << "Please check the hardware description.\n";
		exit (-1);
	}
#endif // DEBUG
  pending_write_vector[pending_write_vector_nb].pointer = pointer_;
//	pending_write_vector[pending_write_vector_nb++].value = *(reinterpret_cast<const base_type*const>(&value_)); => bug !
  pending_write_vector[pending_write_vector_nb++].value = value_; // => bug avec blues !

	// -> fix to use user-defined struct in sc_signal/sc_in/sc_out/sc_inout
	// pending_write_vector[pending_write_vector_nb++].value = *((base_type*)&value_); => bug !
#if 0
	std::cerr << "posted write : ptr = " << pointer_ << ", val = " << value_ << "\n";
#endif
#if 0 
	// introduce bug on using trace functions
	if (value_ == READ_SIGNAL(T,pointer_))
		return;
#endif
	};
}

inline bool is_posted_write ()
{
		return pending_write_vector_nb > 0;
}

extern "C" void update (void);

// ----------------------------------------------------------------------------
//  CLASS : sc_signal_base
//
//  The sc_signal_base<T> primitive channel class.
// ----------------------------------------------------------------------------

class sc_signal_base : public sc_object, public sc_interface
{
	//////
	// Internal
  friend class sc_clock;
  friend class sc_port_base;
  void init ();
	//////				 
  

public: 
  // LRM (?)
  //virtual const sc_event /*&*/ default_event () const;
  static const char* const kind_string;
  //virtual const char *kind () const;

  //
public:
  sc_signal_base();
  sc_signal_base(const char* name_);
  sc_signal_base(const char* name_, void*);
	~sc_signal_base();
};

template <typename T>
class sc_signal : public sc_signal_base
{
private:
	T val;
  typedef T  data_type;
  typedef sc_signal < T >  this_type;
	///////////
	// Internal
	public: void init ();
	///////////
//  virtual void update ();
  void check_writer ();
public:
  // constructors, destructor 
  sc_signal () 
	{ init (); }
  explicit sc_signal (const char *name_): sc_signal_base(name_)
	{ init (); }
  /*virtual */~ sc_signal () 
  {}
  // methods 
  /*
  virtual void register_port (sc_port_base &, const char *)
  {}
  virtual const sc_event & default_event () const 
  {}
  virtual const sc_event & value_changed_event () const 
  {}
  */
  /*virtual*/ inline const data_type & read () const INLINE;
/*
  virtual const T & get_data_ref () const
  {}
  virtual bool event () const
  {}
  */
  /*virtual*/ inline void write (const data_type &) /*INLINE*/;
  inline operator const data_type & () const
  { return this->read(); }
  inline this_type& operator = (const data_type & a)
  { sc_signal<T>::write (a); return *this; }
  inline this_type& operator = (const sc_signal < T > &a)
  { sc_signal<T>::write (a.read()); return *this; }
  inline this_type& operator += (const data_type & a)
  { sc_signal<T>::write (read() + a); return *this; }
  inline this_type& operator += (const sc_signal < T > &a)
  { sc_signal<T>::write (read()+a.read()); return *this; }
  const data_type & get_new_value () const;
//  void trace (sc_trace_file * tf) const;
  /*
	virtual void print (std::ostream &o) const
  { o << *this; }
  virtual void dump (std::ostream &o) const
  { o << *this; }
	*/
private:
  // disabled 
  sc_signal (const sc_signal < T > &);

};

template <typename T>
void
sc_signal<T>::init()
{
	set_pointer ((tab_t*)(void*)&val);
  set_kind    (kind_string);
	sc_interface::init (sizeof (data_type)); 
  val = 0; /* The simulator initializes the signal/register to 0.    */
           /* However, hardware initialization still has to be done. */
           /* This kind of initialization is for trace diffing.      */
}
// read the value
template <typename T>
/*virtual*/ 
inline 
const T & 
sc_signal<T>::read() const
{
#ifdef DUMP_READ
  std::cerr << "read " << READ_SIGNAL(const T, get_pointer())
	 	<< " on signal " << name () << "\n";
#endif
#ifdef CHECK_FSM_RULES
	// we can read value from sc_signal type (used like a register) at any time
#endif	
    return READ_SIGNAL(const T, get_pointer());
}

// write the new value
template <typename T>
inline
void
sc_signal<T>::write( const data_type& value_ )
{
#ifdef CHECK_FSM_RULES
	if ((casc_fsm_step != TRANSITION) 
			&& ( casc_fsm_step != STIMULI)) {
		std::cerr << "FSM rules error : trying to write on signal " 
			  << name () 
			  << " from " << get_step_name () << " function.\n";
		exit (-1);
	}		
#endif
#ifdef DEBUG
  if (get_pointer() == NULL)
  {
    std::cerr << "Error : Unable to write into '" << name () << "'.";
    exit (24032005);
  }
#endif
#ifdef CHECK_MULTIWRITING2REGISTER
  pending_writing2register_record_and_check (get_pointer ());
#endif
#ifdef DUMP_WRITE
  if (sc_signal<T>::read() == value_)
    return;
  std::cerr << "write (posted) " << value_ 
		<< " on sc_signal (writing into register) '" << name () << "'\n";
#endif
  post_write (/*(tab_t*)&val*/ get_pointer(), value_);
}

#undef INLINE

#undef READ_SIGNAL

} // end of namespace sc_core

#endif /* __SC_SIGNAL_H__ */

