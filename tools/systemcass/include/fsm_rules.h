/*------------------------------------------------------------\
|                                                             |
| Tool    :                  systemcass                       |
|                                                             |
| File    :                 fsm_rules.h                       |
|                                                             |
| Author  :                 Buchmann Richard                  |
|                                                             |
| Date    :                   10_05_2005                      |
|                                                             |
\------------------------------------------------------------*/
#ifndef __FSM_RULES_H__
#define __FSM_RULES_H__

#ifdef CHECK_FSM_RULES

namespace sc_core {

typedef enum { TRANSITION, GEN_MOORE, GEN_MEALY, STIMULI, ELABORATION} casc_fsm_step_t;
extern casc_fsm_step_t casc_fsm_step;

inline const char *get_step_name   () 
{ const char *s[] = {"TRANSITION", "MOORE GENERATION", "MEALY GENERATION", "SIMULATION LOOP", "ELABORATION"}; return s[casc_fsm_step]; }

} // end of sc_core namespace

#endif

#endif

