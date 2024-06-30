#ifndef VIEW_H
#define VIEW_H

#include "defns.h"

/*File contains the structre and function declerations for viewing tag data of file */

typedef struct _Viewtags
{
	/* Stores file details */
	char *file_name;
	FILE *file_ptr;
	
	/* Stores ID3 Tag and vesion */
	char id3_tag[4];
	unsigned char id3_version;

	/* Stores the tags */
	int tag_type;
	char tag_flag[5];
	int tag_size;
	char tag_data[30];

	int print_flag;
} ViewTags;

/* Function prototypes for viewing tags */

/* Calls the fucntion and prints the tags */
Status view_file_tag_data( ViewTags *viewtags );

/*Check if passed file is .mp3 file */
Status check_file_type( char *argv[], ViewTags *viewtags );

/* Function to open the files to read the tag */
Status open_mp3_file( ViewTags *viewtags );

/* Function to check if tags are of ID3 Tag */
Status check_id3_tag( ViewTags *viewtags );

/* Function to check the version of ID3 Tag data */
Status check_id3_version( ViewTags *viewtags );

/* Function to skip the header data */
Status skip_mp3_header_data( ViewTags *viewtags );

/* Function to read tag */
Status read_tags( ViewTags *viewtags );

/* Function to read tag flag */
Status read_tag_flag( ViewTags *viewtags );

/* Function to read tag size */
Status read_tag_size( ViewTags *viewtags );

/* Function to read tag data */
Status read_tag_data( ViewTags *viewtags );

/* Function to print tag data */
Status print_tag_data( ViewTags *viewtags );

#endif
