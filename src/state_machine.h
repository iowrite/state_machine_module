#include <stdbool.h>
#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H



struct state;

struct transition
{
   void *condition;
   bool ( *guard )( void *condition);
   void ( *action )( void *currentStateData,void *newStateData );
   struct state *nextState;
};


 
struct state
{

   struct state *parentState;

   struct state *entryState;

   struct transition *transitions;

   int numTransitions;

   bool isChecked;

   void *data;

   void ( *entryAction )( void *stateData);
   void ( *runAction )( void *stateData);
   void ( *exitAction )( void *stateData);
};



struct stateMachine
{

   struct state *currentState;

   struct state *previousState;

};



void stateM_init( struct stateMachine *stateMachine, struct state *initialState);



enum stateM_handleEventRetVals
{

   stateM_errArg = -2,

   stateM_errorStateReached,

   stateM_stateChanged,

   stateM_stateLoopSelf,

   stateM_noStateChange,

   stateM_finalStateReached,
};





int state_machine_run( struct stateMachine *stateMachine);




#endif





