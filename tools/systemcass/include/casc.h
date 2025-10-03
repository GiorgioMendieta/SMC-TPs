/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 casc.h                            |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   09_07_2004                      |
|                                                             |
\------------------------------------------------------------*/
#ifndef __CASC_H__
#define __CASC_H__

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif

#include <stdint.h>

EXTERN char unstable;
EXTERN int32_t pending_write_vector_nb;

namespace sc_core {

struct sc_module;

inline void transition        (void);
EXTERN void update            (void);
inline void moore_generation  (void);
EXTERN void mealy_generation  (void);
EXTERN bool casc_check_version(const char*);
}

#ifdef SCHEDULING_BY_CASC
#include <systemcass_version_ext.h>
#include <fsm_rules.h>

namespace sc_core {

EXTERN void initialize        ()
{
  casc_check_version (SYSTEMC_VERSION);
}

EXTERN void simulate_1_cycle (void) 
{
#ifdef CHECK_FSM_RULES
	casc_fsm_step = TRANSITION;
#endif
  transition ();
  update     ();
#ifdef CHECK_FSM_RULES
	casc_fsm_step = GEN_MOORE;
#endif
  moore_generation ();
#ifdef CHECK_FSM_RULES
	casc_fsm_step = GEN_MEALY;
#endif
  mealy_generation (); 
#ifdef CHECK_FSM_RULES
	casc_fsm_step = STIMULI;
#endif
}
} // end of sc_core namespace

#endif


#endif

