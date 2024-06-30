#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "defns.h"
#include "modify.h"

/* The file contains the necessary function defintions
   to modify the tag data of the MP3 file input by user */

Status check_and_open_file( char *argv[], Modify *modtag )
{
	/* Function checks if the file passed by the user is a .mp3
	   file and opens it. It also creates and opens the 
	   temporary file to store the data to modify correctly */

	//To check if the file passed is .mp3
	if( strstr( argv[4], ".mp3") != NULL )
	{
		modtag->file_name = argv[4];
	}
	else
	{
		printf("ERROR: File passed should be a .mp3 file\n");
		return e_failure;
	}

	//Open the mp3 file
	modtag->file_ptr = fopen( modtag->file_name, "r+");
	//Validate if opened correctly
	if( modtag->file_ptr == NULL )
	{
		printf("ERROR: Unable to open %s file\n", modtag->file_name);
		return e_failure;
	}

	//Create and open the temporary file and validate
	modtag->temporary = fopen( "temp.mp3", "w+");

	if( modtag->temporary == NULL )
	{
		printf("ERROR: Unable to open temporary file\n");
		return e_failure;
	}

	return e_success;
}

Status select_tag_identifier( char *argv[], Modify *modtag )
{
	/* Function selects appropriate tag identifier to be edited
	   based on user inputand also stores the data to modify with */

	if( strcmp( argv[2], "-t") == 0 )
	{
		//printf("Selectd Title\n");
		strcpy( modtag->tag_identifier, "TIT1" );
		strcpy( modtag->tag_identifier2, "TIT2" );
		modtag->attribute_name = "TITLE";
	}
	else if( strcmp( argv[2], "-a") == 0 )
	{
		//printf("Selectd Artist\n");
		strcpy( modtag->tag_identifier, "TPE1" );
		strcpy( modtag->tag_identifier2, "TPE2" );
		modtag->attribute_name = "ARTIST";
	}
	else if( strcmp( argv[2], "-A") == 0 )
	{
		//printf("Selectd Album\n");
		strcpy( modtag->tag_identifier, "TALB" );
		modtag->attribute_name = "ALBUM";
	}
	else if( strcmp( argv[2], "-y") == 0 )
	{
		//printf("Selectd Year\n");
		strcpy( modtag->tag_identifier, "TYER" );
		strcpy( modtag->tag_identifier2, "TDRC" );
		modtag->attribute_name = "YEAR";
	}
	else if( strcmp( argv[2], "-m") == 0 )
	{
		//printf("Selectd Comments\n");
		strcpy( modtag->tag_identifier, "COMM" );
		modtag->attribute_name = "COMMENT";
	}
	else if( strcmp( argv[2], "-g") == 0 )
	{
		//printf("Selectd Genre\n");
		strcpy( modtag->tag_identifier, "TCON" );
		modtag->attribute_name = "GENRE";
	}
	else
	{
		printf("ERROR: Invalid option\n");
		return e_failure;
	}

	//Store new tag data to structre variable
	modtag->user_tag_data =  argv[3];
	return e_success;
}

Status modify_tag_data( Modify *modtag )
{
	/* Function executes the flow of the program by calling other fuctions */

	if( validate_id3_tag( modtag ) == e_success )
	{
		//printf("Validated ID3 Tag\n");
		if( find_tag( modtag ) == e_success )
		{
			//printf("Found tag\n");
			if( copy_data_to_temp_file( modtag ) == e_success )
			{
				//printf("Copied data to temp file\n");
				if( edit_tag( modtag ) == e_success )
				{
					//printf("Edited tag data successfully\n");
					if( restore_data_to_file( modtag ) == e_success )
					{
						//printf("Restored data to source file succesfully\n");
						if( print_updated_tag_data( modtag ) == e_success )
						{
							//printf("Printed tag data\n");
							if( close_and_delete( modtag ) == e_failure )
							{
								//printf("ERROR: Unable to close the files\n");
								return e_failure;
							}
							else
							{
								//printf("Files closed and deleted\n");
							}
						}
						else
						{
							//printf("ERROR: Unable to fetch data\n");
							return e_failure;
						}
					}
					else
					{
						//printf("ERROR: Unable to restore the data\n");
						return e_failure;
					}
				}
				else
				{
					//printf("ERROR: Unable to modify tag data\n");
					return e_failure;
				}
			}
			else
			{
				//printf("ERROR: Unable to find ID3 Tag\n");
				return e_failure;
			}
		}
		else
		{
			//printf("ERROR: Unable to find Tag identifier\n");
			return e_failure;
		}
	}
	else
	{
		//printf("ERROR: Unable to find ID3 Tag\n");
		return e_failure;
	}

	return e_success;
}

Status validate_id3_tag( Modify *modtag )
{
	/* Function checks if the .mp3 file has metadata stored
	   in it as ID3 v2.3 version */

	//To read and check for ID3 tag
	if( fread( modtag->id3_tag, 1, 3, modtag->file_ptr) != 3 )
	{
		printf("ERROR: Unable to check for ID3 Tag\n");
		return e_failure;
	}

	if( strcmp( modtag->id3_tag, "ID3") != 0 )
	{
		printf("ERROR: File does not have ID3 Tag\n");
	}

	//To read and check for ID3 v2.3
	if( fread( &modtag->id3_version, 1, 1, modtag->file_ptr) != 1 )
	{
		printf("ERROR: Unable to fetch ID3 version\n");
		return e_failure;
	}

	if( modtag->id3_version != 0x03 )
	{
		printf("ERROR: File is not of ID3 v2.3 version\n");
		return e_failure;
	}

	return e_success;
}

Status find_tag( Modify *modtag )
{
	/* Function searches the file for the required tag identifier */

	fseek( modtag->file_ptr, 10, SEEK_SET);
	if( ftell( modtag->file_ptr) != 10 )
	{
		printf("ERROR: Unable to move through data\n");
		return e_failure;
	}

	while( 1 )
	{
		//Reads the tag identifier from file
		if( fread( modtag->tag_id_file, 1, 4, modtag->file_ptr) != 4 )
		{
			return e_failure;
		}
		//Checking if it is the required tag identifier
		if( strcmp( modtag->tag_identifier, modtag->tag_id_file) == 0 )
		{
		  	/* printf("Tag Modifier : %s\n", modtag->tag_id_file);
			printf("Tag Identifiers : %s and %s\n", modtag->tag_identifier, modtag->tag_identifier2); */
			break;
		}
		else if( strcmp( modtag->tag_identifier2, modtag->tag_id_file) == 0 )
		{
		  	/*	printf("Tag Modifier : %s\n", modtag->tag_id_file);
			printf("Tag Identifiers : %s and %s\n", modtag->tag_identifier, modtag->tag_identifier2); */
			break;
		}
		else
		{
			//If not found, skip the current tag and procced to next
			fseek( modtag->file_ptr, 3, SEEK_CUR);
			if( fread( &modtag->tag_size_file, 1, 1, modtag->file_ptr) != 1 )
			{
				return e_failure;
			}
			fseek( modtag->file_ptr, (modtag->tag_size_file + 2), SEEK_CUR);
		}
	}

	return e_success;
}

Status copy_data_to_temp_file( Modify *modtag )
{
	/* Function obtains the size of the tag data and moves to the
	   end of the tag data and copies the rest of the data to the
	   temporary file */


	modtag->offset = ftell( modtag->file_ptr ); //Stores the current postion to return to

	fseek( modtag->file_ptr, 3, SEEK_CUR ); //Obtains the size of the tag data
	fread( &modtag->tag_size_file, 1, 1, modtag->file_ptr );

	//printf("Size of tag : %d\n", modtag->tag_size_file);

	fseek( modtag->file_ptr, (modtag->tag_size_file + 2 ), SEEK_CUR );

	//printf("Offset at end of tag data : %d\n", (int)ftell(modtag->file_ptr));

	//After moving to the end of the tag, copy the rest of the data to the temp file
	/* Use a loop to copy character by character till file pointer encounters EOF */
	char ch;
	while( fread( &ch, 1, 1, modtag->file_ptr) != 0 )
	{
		if( fwrite( &ch, 1, 1, modtag->temporary) != 1 )
		{
			return e_failure;
		}
	}

	//After copying data, reset file pointer of the mp3 file back to tag location
	fseek( modtag->file_ptr, modtag->offset, SEEK_SET );

	return e_success;
}

Status edit_tag( Modify *modtag )
{
	/* Function modifies the tag data in the file */

	//Obtains the size of the new tag and write it to the file
	modtag->user_tag_size = (char)(strlen( modtag->user_tag_data ) + 1);
	//printf("Size : %d\n", (int)sze);
	fseek( modtag->file_ptr, 3, SEEK_CUR);
	if( fwrite( &modtag->user_tag_size, 1, 1, modtag->file_ptr ) != 1 )
	{
		return e_failure;
	}

	//Skip the 2 byte of tag flag
	fseek( modtag->file_ptr, 2, SEEK_CUR);

	//Write the new tag data to the mp3 file 
	if( fwrite( modtag->user_tag_data, modtag->user_tag_size, 1, modtag->file_ptr ) != 1 )
	{
		return e_failure;
	}

/*	char herja[modtag->user_tag_size];
	fseek( modtag->file_ptr, -(modtag->user_tag_size), SEEK_CUR);
	fread( herja, 1, modtag->user_tag_size, modtag->file_ptr);
	printf("New tag data : %s\n", herja); */

	return e_success;
}

Status restore_data_to_file( Modify *modtag )
{
	/* Function copies the data from orginal file and 
	   attaches it to the end of the updated tag */

	rewind( modtag->temporary ); //Rewinds the file pointer of temporary

	//The data is then copied using a loop
	//The loop will run until the temporary file pointer reaches EOF
	char ch;
	while( fread( &ch, 1, 1, modtag->temporary) != 0 )
	{
		if( fwrite( &ch, 1, 1, modtag->file_ptr) != 1 )
		{
			return e_failure;
		}
	}

	//Reset the mp3 file pointer back to tag location
	fseek( modtag->file_ptr, modtag->offset, SEEK_SET );

	return e_success;
}

Status print_updated_tag_data( Modify *modtag )
{
	/* Function fetches the updated tag from file and prints */

	//Obtain the size from the file
	fseek( modtag->file_ptr, 3, SEEK_CUR ); //Obtains the size of the tag data
	fread( &modtag->tag_size, 1, 1, modtag->file_ptr );
	fseek( modtag->file_ptr, 2, SEEK_CUR ); //Skips the tag flag

	//Allocate memory for storing the new tag and validate
	modtag->tag = calloc( (int)modtag->tag_size, 1 );

	if( modtag->tag == NULL )
	{
		printf("ERROR: Memory not allocated for buffer\n");
	}

	//Read the tag data from the file
	if( fread( modtag->tag, 1, modtag->tag_size, modtag->file_ptr ) != modtag->tag_size )
	{
		printf("ERROR: Unable to read data\n");
		return e_failure;
	}

	//Print the data
	printf("---------------CHANGE THE %s---------------\n\n", modtag->attribute_name);
	printf("%s          :          ", modtag->attribute_name);
	for( int i = 0 ; i < modtag->tag_size ; i++ )
	{
		printf("%c", modtag->tag[i]);
	}
	printf("\n\n");

	//Deallocate memory after printing
	free( modtag->tag);

//		return e_failure;
	return e_success;
}

Status close_and_delete( Modify *modtag )
{
	/* Function is used to close all the files and remove the temporary file */

	fclose( modtag->file_ptr ); //Closes the source file

	fclose( modtag->temporary ); //Closes the temporary file

	if( remove("temp.mp3") != 0 ) //Removes the temporary file
	{
		return e_failure;
	}

	return e_success;
}
