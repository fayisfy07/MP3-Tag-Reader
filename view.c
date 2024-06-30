#include<stdio.h>
#include<string.h>
#include "defns.h"
#include "view.h"

/* File contains function definitions for viewing tags*/

Status check_file_type( char *argv[], ViewTags *viewtags )
{
	/* Function checks if the file input by the user is a .mp3 file or not */

	if( strstr( argv[2], ".mp3" ) != NULL )
	{
		viewtags->file_name = argv[2];
	}
	else
	{
		return e_failure;
	}
	return e_success;
}

Status open_mp3_file( ViewTags *viewtags )
{
	/* Function opens the file input by the use */

	viewtags->file_ptr = fopen( viewtags->file_name, "r" );

	if( viewtags->file_ptr == NULL )
	{
		printf("ERROR: Unable to open %s file\n", viewtags->file_name);
		return e_failure;
	}
	return e_success;
}

Status check_id3_tag( ViewTags *viewtags)
{
	/* Function checks for the presence of ID3 tag witin the MP3 file */

	if( fread(viewtags->id3_tag, 1, 3, viewtags->file_ptr) != 3 )
	{
		return e_failure;
	}

	if( strcmp(viewtags->id3_tag, "ID3") != 0 )
	{
		return e_failure;
	}
	return e_success;
}

Status check_id3_version( ViewTags *viewtags )
{
	/* Function checks for the version of ID3 V2 and if it is not
	   ID3 V2.3 It returns an error */

	if( fread(&viewtags->id3_version, 1, 1, viewtags->file_ptr) != 1 )
	{
		return e_failure;
	}

	if( viewtags->id3_version != 0x03 )
	{
		return e_failure;
	}

	return e_success;
}

Status skip_mp3_header_data( ViewTags *viewtags )
{
	/* Function rewinds to the beginning of the MP3 file and skips the
	   10 byte of header data */
	rewind( viewtags->file_ptr );
	fseek( viewtags->file_ptr, 10, SEEK_SET );
	if( ftell( viewtags->file_ptr) != 10 )
	{
		return e_failure;
	}
	return e_success;
}

Status read_tags( ViewTags *viewtags )
{
	/* This function calls other functions to read tag type, size and data
	   and then print the tag data */

	viewtags->print_flag=0;
	for( int i = 0 ; viewtags->print_flag < 6 ; i++ )
	{
		if( read_tag_flag( viewtags ) == e_success )
		{
			if( read_tag_size( viewtags ) == e_success )
			{
				if( read_tag_data( viewtags ) == e_success )
				{
					if( print_tag_data( viewtags ) == e_failure )
					{
						return e_failure;
					}
				}
				else
				{
					printf("ERROR: Unable to print tag data\n");
					return e_failure;
				}
			}
			else
			{
				printf("ERROR : Unable to fetch tag size\n");
				return e_failure;
			}
		}
		else
		{
			printf("ERROR : Unable to fetch tag flag\n");
			return e_failure;
		}
	}
	return e_success;
}

Status read_tag_flag( ViewTags *viewtags )
{
	/* Function reads the type of tag data */
	if( fread( viewtags->tag_flag, 1, 4, viewtags->file_ptr) != 4 )
	{
		return e_failure;
	}
//		printf("Tag flag : %s\n", viewtags->tag_flag);
	if( strcmp( viewtags->tag_flag, "TALB" ) == 0 )
	{
		viewtags->tag_type = 0;
	}
	else if( strcmp( viewtags->tag_flag, "TIT2" ) == 0 )
	{
		viewtags->tag_type = 1;
	}
	else if( strcmp( viewtags->tag_flag, "TPE2" ) == 0 )
	{
		viewtags->tag_type = 2;
	}
	else if( strcmp( viewtags->tag_flag, "TCON" ) == 0 )
	{
		viewtags->tag_type = 3;
	}
	else if( strcmp( viewtags->tag_flag, "COMM" ) == 0 )
	{
		viewtags->tag_type = 4;
	}
	else if( strcmp( viewtags->tag_flag, "TDRC" ) == 0 )
	{
		viewtags->tag_type = 5;
	}
	else if( strcmp( viewtags->tag_flag, "TPE1" ) == 0 )
	{
		viewtags->tag_type = 2;
	}
	else if( strcmp( viewtags->tag_flag, "TYER" ) == 0 )
	{
		viewtags->tag_type = 5;
	}
	else
	{
		viewtags->tag_type = 6;
	}

	//	printf("Tag flag set : %d\n", viewtags->tag_type);	

	return e_success;
}

Status read_tag_size( ViewTags *viewtags )
{
	/* Function reads the tag size from the file */

	fseek(viewtags->file_ptr, 3, SEEK_CUR);
	if( fread( &viewtags->tag_size, 1, 1, viewtags->file_ptr) != 1 )
	{
		return e_failure;
	}
	//	printf("Tag Size : %d\n", viewtags->tag_size);

	return e_success;
}

Status read_tag_data( ViewTags *viewtags )
{
	/* Function reads the tag data after skipping the tag flag */

	fseek( viewtags->file_ptr, 2, SEEK_CUR); //To skip tag flag
	if( fread( viewtags->tag_data, 1, viewtags->tag_size, viewtags->file_ptr) != viewtags->tag_size )
	{
		return e_failure;
	}

	return e_success;
}

Status print_tag_data( ViewTags *viewtags )
{
	/* Function prints the tag data obtained from the file */

	switch( viewtags->tag_type )
	{
		case 0:
			printf("ALBUM         :");
			break;
		case 1:
			printf("TITLE         :");
			break;
		case 2:
			printf("ARTIST        :");
			break;
		case 3:
			printf("GENRE         :");
			break;
		case 4:
			printf("COMMENT       :");
			break;
		case 5:
			printf("YEAR          :");
			break;
		default:
	}
	if( viewtags->tag_type < 6 && viewtags->print_flag <= 6)
	{
		printf("          ");
		for( int i = 0 ; i < viewtags->tag_size ; i++ )
		{
			printf("%c", viewtags->tag_data[i]);
		}
		printf("\n");
		viewtags->print_flag++;
	}
	return e_success;
}

Status view_file_tag_data( ViewTags *viewtags )
{
	/* Funtion prints the tag details obtained from the file to the output */

	printf("----------------------------SELECTED VIEW DETAILS----------------------------\n\n\n");
	printf("-------------------------------------------------------------------------\n");
	printf("                   MP3 TAG READER AND EDITOR FOR ID3V2                   \n");
	printf("-------------------------------------------------------------------------\n");

	if( open_mp3_file( viewtags ) == e_success )
	{
		if( check_id3_tag( viewtags ) == e_success )
		{
			if( check_id3_version( viewtags ) == e_success )
			{
				if( skip_mp3_header_data( viewtags ) == e_success )
				{
					if( read_tags( viewtags ) == e_success )
					{
						printf("-------------------------------------------------------------------------\n\n");
						printf("----------------------DETAILS DISPLAYED SUCESSFULLY----------------------\n\n");
						fclose( viewtags->file_ptr);
						return e_success;
					}
					else
					{
						printf("ERROR: Unable to read tag data\n");
						return e_failure;
					}
				}
				else
				{
					printf("ERROR: Unable to skip MP3 file header data\n");
					return e_failure;
				}
			}
			else
			{
				printf("ERROR: ID3 version is not V2.3\n");
				return e_failure;
			}
		}
		else
		{
			printf("ERROR: ID3 Tag signature not found\n");
			return e_failure;
		}
	}
	else
	{
		printf("ERROR: Unable to open file\n");
		return e_failure;
	}
}
