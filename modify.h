#ifndef MODIFY_H
#define MODIFY_H

#include "modify.h"

/* File contains necessary structre defenition and fuction
   declerations to modify the tag data of the mp3 file */

typedef struct _Modify
{
	/*To store file names and file pointers */
	char *file_name;
	FILE *file_ptr;
	FILE *temporary;
	int offset;

	/* Stores the MP3 file tag version */
	char id3_tag[4];
	unsigned char id3_version;

	/* Tag details from user */
	char *user_tag_data;
	char user_tag_size;
	
	char tag_identifier[5];
	char tag_identifier2[5];
	char *attribute_name;

	/* Tag data from file */
	int tag_size_file;
	char tag_id_file[5];

	/* To print data after modification */
	char *tag;
	char tag_size;
} Modify;

/* Function prototypes for modifying tag data */

/* To check if the file passed by user is .mp3 or not and to open files */
Status check_and_open_file( char *argv[], Modify *modtag);

/* Function to select appropriate tag identifier */
Status select_tag_identifier( char *argv[], Modify *modtag);

/* Function to call sub functions and modify tag data */
Status modify_tag_data( Modify *modtag);

/* Function to check for ID3 tag version */
Status validate_id3_tag( Modify *modtag );

/* Function to search for the tag data */
Status find_tag( Modify *modtag );

/* Function to copy data to temporary file */
Status copy_data_to_temp_file( Modify *modtag );

/* Function to modify the tag data */
Status edit_tag( Modify *modtag );

/* Function to copy data back to original file */
Status restore_data_to_file( Modify *modtag );

/* Function to print data after modifying */
Status print_updated_tag_data( Modify *modtag );

/* Function to close all opened files and to remove temporary file */
Status close_and_delete( Modify *modtag );

#endif
