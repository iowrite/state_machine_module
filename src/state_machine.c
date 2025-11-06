#include <stddef.h>
#include "state_machine.h"

static struct transition *getTransition( struct stateMachine *stateMachine, struct state *state);

void state_machine_init( struct stateMachine *fsm, struct state *initialState)
{
   if ( !fsm )
      return;

   fsm->currentState = initialState;
   fsm->previousState = NULL;

   // do initial state's entry action
   struct state *s = fsm->currentState;
   do{
      s->entryAction( s->data);
      fsm->currentState = s;
      s = s->entryState;
   }while(s);

}

int state_machine_run( struct stateMachine *fsm)
{
   if ( !fsm)
      return stateM_errArg;

   if ( !fsm->currentState )
   {
      return stateM_errorStateReached;
   }

   if ( !fsm->currentState->numTransitions )
      return stateM_noStateChange;

   struct state *s = fsm->currentState;
   do                                                    // walk through parent states to run all runActions
   {
      if(s->runAction)
      {
         s->runAction(s->data);
         s->isChecked = false;
      }
      s = s->parentState;
   }while(s);

   struct state *nextState = fsm->currentState;
   do {
      struct transition *transition = getTransition( fsm, nextState);

      /* If there were no transitions for the given event for the current
       * state, check if there are any transitions for any of the parent
       * states (if any): */
      if ( !transition )
      {
         continue;
      }

      /* A transition must have a next state defined. If the user has not
       * defined the next state, go to error state: */
      if ( !transition->nextState )
      {
         return stateM_errorStateReached;
      }

      nextState = transition->nextState;

      /* If the new state is a parent state, enter its entry state (if it has
       * one). Step down through the whole family tree until a state without
       * an entry state is found: */
      while ( nextState->entryState )
         nextState = nextState->entryState;

      /* Run exit action only if the current state is left (only if it does
       * not return to itself): */
      if ( nextState != fsm->currentState && fsm->currentState->exitAction )
         fsm->currentState->exitAction( fsm->currentState->data);

      /* Run transition action (if any): */
      if ( transition->action )
         transition->action( fsm->currentState->data, nextState->
               data );

      /* Call the new state's entry action if it has any (only if state does
       * not return to itself): */
      if ( nextState != fsm->currentState && nextState->entryAction )
         nextState->entryAction( nextState->data);

      fsm->previousState = fsm->currentState;
      fsm->currentState = nextState;                                             // update current state
      
      /* If the state returned to itself: */
      if ( fsm->currentState == fsm->previousState )
         return stateM_stateLoopSelf;

      /* If the new state is a final state, notify user that the state
       * machine has stopped: */
      if ( !fsm->currentState->numTransitions )
         return stateM_finalStateReached;

      return stateM_stateChanged;                                                // every call of run processes only check on state transitions
   } while ( nextState->isChecked == false );

   return stateM_noStateChange;
}




static struct transition *getTransition( struct stateMachine *fsm, struct state *state)
{
   (void)fsm;
   struct transition * retTransition = NULL;
   // find the deepest unchecked parent state
   while(state->parentState)                    // break, if it havent parent state
   {
      if(state->parentState->isChecked)         // break, if it's parent state is already checked
      {
         break;
      }
      state = state->parentState;
   }

   for (int i = 0; i < state->numTransitions; ++i )
   {
      struct transition *t = &state->transitions[ i ];
      if ( !t->guard )
      {
         retTransition = t;
         break;
      } 
      else if ( t->guard( t->condition) ){
         retTransition = t;
         break;
      }
   }
   state->isChecked = true;
   

   /* No transitions found for given event for given state: */
   return retTransition;
}

