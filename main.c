#include<stdio.h>
#include<string.h>
#include "defns.h"
#include "view.h"
#include "modify.h"

/*	######################### Documentation #########################
Author : Pranav S
Project Name : MP3 Tag Reader and Editor for ID3 V2.3
Description : MP3 files contains metadata that can give information about 
			  the music contained in the file, which includes information such as
			  title, album, artists, genre , etc. This data is stored in the header
			  data of MP3 files.

			  This code can be used to view and edit the tag data of a MP3 file.
 */

int main( int argc, char *argv[])
{
	ViewTags viewtags;
	Modify modtag;
	int opr = select_operation_type( argv );
	if( opr == e_view )
	{
		/* Operation to view the tags of the file */
		if( argc >= 3 )
		{
			if( check_file_type( argv, &viewtags ) == e_success )
			{
				if( view_file_tag_data( &viewtags ) == e_failure )
				{
					printf("ERROR: Unable to fetch tag details\n");
				}
			}
			else
			{
				printf("ERROR: Input file not .mp3\n");
			}
		}
		else
		{
			printf("ERROR: Insufficient Number Of Command Line Arguments For Viewing\n");
		}
	}
	else if( opr == e_edit )
	{
		/* Operation to edit the tag of the file */
		if( argc >= 5 )
		{
			printf("-------------------------SELECTED EDIT DETAILS------------------------\n\n\n");
			printf("-------------------------SELECTED EDIT OPTION-------------------------\n\n");
			if( check_and_open_file( argv, &modtag ) == e_success)
			{
				//printf("Opened the files\n");
				if( select_tag_identifier( argv, &modtag) == e_success )
				{
					//printf("Selected tag identifier\n");
					if( modify_tag_data( &modtag ) == e_success )
					{
						printf("-----------%s CHANGED SUCESSFULLY-------------\n\n", modtag.attribute_name);
						//printf("INFO: Tag Data Modified Sucessfully\n");
					}
					else
					{
						printf("ERROR: Tag data not modified\n");
					}
				}
				else
				{
					printf("ERROR: Invalid Option\n");
				}
			}
		}
		else
		{
			printf("ERROR: Insufficient Number Of Command Line Arguments For Editing\n");
		}
	}
	else if( opr == e_help )
	{
		/* Operation to print the help menu */
		print_help_menu();
	}
	else
	{
		/* Operation to print error and usage menu */
		print_error_usage_message( argc, argv);
	}

	return 0;
}

OperationType select_operation_type( char *argv[] )
{
	/* Function returns enum based on operation selected by the user */
	if( argv[1] == NULL )
	{
		return e_error;
	}
	else if( strcmp( argv[1], "-v") == 0 )
	{
		return e_view;
	}
	else if ( strcmp ( argv[1], "-e") == 0 )
	{
		return e_edit;
	}
	else if ( strcmp ( argv[1], "--help") == 0 )
	{
		return e_help;
	}
}

void print_error_usage_message( int argc, char *argv[] )
{
	/* Function to print usage in case of improper input by user */
	printf("--------------------------------------------------------------------------------\n\n");
	printf("ERROR: ");
	for( int i = 0 ; i < argc ; i++ )
	{
		printf("%s " , argv[i] );
	}
	printf(": INVALID ARGUMENTS\n");
	printf("USAGE :\n");
	printf("To view, please pass : ./a.out -v mp3filename.mp3\n");
	printf("To edit, please pass : ./a.out -e -t/-a/-A/-m/-y/-c change_text mp3filename.mp3\n");
	printf("To get help, please pass : ./a.out --help\n");
	printf("--------------------------------------------------------------------------------\n\n");
}

void print_help_menu( void )
{
	/* Function displays the help menu */
	printf("------------------------------HELP MENU-----------------------------\n\n");
	printf("1. -v -> to view mp3 file contents\n");
	printf("2. -e -> to edit mp3 file contents\n");
	printf("         2.1. -t -> to edit song title\n");
	printf("         2.2. -a -> to edit song artist name\n");
	printf("         2.3. -A -> to edit album name\n");
	printf("         2.4. -y -> to edit year\n");
	printf("         2.5. -m -> to edit comment\n");
	printf("         2.6. -g -> to edit genre\n\n");
	printf("--------------------------------------------------------------------\n\n");
}
