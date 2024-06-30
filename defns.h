#ifndef DEFINITIONS_H
#define DEFINITIONS_H

/* File contains the enum declerations for selecting the
   operation type and status of operation of a function.
  
   Decleration of functions to print error and help message 
   and to select the operation type is also made in this file */

void print_error_usage_message( int argc, char *argv[] );

void print_help_menu( void );

/* OperationType will be used to select the operation */
typedef enum
{
	e_view,
	e_edit,
	e_help,
	e_error
} OperationType;

/* Status will be used to return if a function has 
   executed the code properly and it works		*/

typedef enum
{
	e_success,
	e_failure
} Status;

OperationType select_operation_type( char *argv[] );
#endif
