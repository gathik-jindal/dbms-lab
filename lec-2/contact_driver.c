
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "pds.h"
#include "contact.h"

// You can use the following macro to report test result
// Do NOT put a semicolon after calling the macro (see examples below)

#define TREPORT(tid,result) { printf("Test-case-id %s:%s",tid,result); fflush(stdout); }

int main()
{
	char *repo_name = "contactdemo";
	int status, rec_size;
	struct Contact testContact;
	char *test_case_id;

	strcpy(testContact.contact_name, "dummy name");
	strcpy(testContact.phone, "dummy number");

	rec_size = sizeof(struct Contact);

	test_case_id = "01";
	status = pds_open( repo_name, rec_size );
	if( status == PDS_SUCCESS ){
		TREPORT(test_case_id,"SUCCESS")
	}
	else{
		TREPORT(test_case_id,"FAIL")
		exit(status);
	}
	// exit(1);
	
	test_case_id = "02";
	testContact.contact_id = 10000;
	status = put_rec_by_key( testContact.contact_id, &testContact );
	if( status == PDS_SUCCESS )
		TREPORT(test_case_id,"SUCCESS")
	else
		TREPORT(test_case_id,"FAIL")

	test_case_id = "03";
	testContact.contact_id = -1;
	status = get_rec_by_key( 10000, &testContact );
	if( status == PDS_SUCCESS )
		TREPORT(test_case_id,"SUCCESS")
	else
		TREPORT(test_case_id,"FAIL")

	test_case_id = "04";
	if( testContact.contact_id == 10000 && 
		strcmp(testContact.contact_name,"dummy name") == 0 &&
		strcmp(testContact.phone,"dummy number") == 0 )

		TREPORT(test_case_id,"SUCCESS")
	else
		TREPORT(test_case_id,"FAIL")

	test_case_id = "05";
	status = pds_close();
	if( status == PDS_SUCCESS ){
		TREPORT(test_case_id,"SUCCESS")
	}
	else{
		TREPORT(test_case_id,"FAIL")
		exit(status);
	}

	test_case_id = "06";
	status = pds_open( repo_name, rec_size );
	if( status == PDS_SUCCESS ){
		TREPORT(test_case_id,"SUCCESS")
	}
	else{
		TREPORT(test_case_id,"FAIL")
		exit(status);
	}

	test_case_id = "07";
	testContact.contact_id = -1;
	status = get_rec_by_key( 10001, &testContact );
	if( status == PDS_REC_NOT_FOUND )
		TREPORT(test_case_id,"SUCCESS")
	else
		TREPORT(test_case_id,"FAIL")

	test_case_id = "08";
	testContact.contact_id = -1;
	status = get_rec_by_key( 10000, &testContact );
	if( status == PDS_SUCCESS )
		TREPORT(test_case_id,"SUCCESS")
	else
		TREPORT(test_case_id,"FAIL")

	test_case_id = "09";
	if( testContact.contact_id == 10000 && 
		strcmp(testContact.contact_name,"dummy name") == 0 &&
		strcmp(testContact.phone,"dummy number") == 0 )

		TREPORT(test_case_id,"SUCCESS")
	else
		TREPORT(test_case_id,"FAIL")

        test_case_id = "10";
	status = pds_close();
	if( status == PDS_SUCCESS ){
		TREPORT(test_case_id,"SUCCESS")
	}
	else{
		TREPORT(test_case_id,"FAIL")
		exit(status);
	}
  return 0;
}


