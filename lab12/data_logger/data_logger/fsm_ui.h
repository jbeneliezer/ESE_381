/*
 * fsm_ui.h
 *
 * Created: 4/29/2021 5:43:27 PM
 *  Author: Judah Ben-Eliezer 
 */ 


#ifndef FSM_UI_H_
#define FSM_UI_H_

extern int dig;

typedef enum {s, h, m, e, digit, enter, eol} key;
typedef enum {idle, set, hours, minutes, seconds} state;
	
typedef void (*task_fn_ptr) ();

typedef struct
{
	key keyval;
	state next_state;
	task_fn_ptr tf_ptr;
} transition;

char* bcd2int(uint8_t);
uint8_t int2bcd(int);
void set_fn();
void hours_fn();
void minutes_fn();
void seconds_fn();
void digit_fn();
void enter_hours_fn();
void enter_minutes_fn();
void enter_seconds_fn();
void error_fn();

state fsm_ui(state ps, key keyval);

#endif /* FSM_UI_H_ */