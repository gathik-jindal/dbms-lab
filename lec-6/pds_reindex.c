// IMT2023089

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pdsv5.h"
#include "contact.h"

int main(int argc, char *argv[])
{
    // initializing variables
    FILE *ndxFP, *dataFP;
    char repo_name[50], data_name[50], ndx_name[50];

    // checking if input is given
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s repo_name\n", argv[0]);
        exit(1);
    }

    // initializing names
    strcpy(repo_name, argv[1]);
    strcpy(data_name, argv[1]);
    strcat(data_name, ".dat");
    strcpy(ndx_name, argv[1]);
    strcat(ndx_name, ".ndx");

    // getting record size
    int rec_size = sizeof(struct Contact);

    // opening files
    ndxFP = fopen(ndx_name, "wb");
    dataFP = fopen(data_name, "rb");

    // initialzing variables
    struct BST_Node *root = NULL;
    int rec_count = 0;
    int key;

    // skipping sizeof(int) to write rec_count later
    int zero = 0;
    fwrite(&zero, sizeof(int), 1, ndxFP);

    // reading data file
    struct PDS_NdxInfo var;
    while (fread(&key, sizeof(int), 1, dataFP) >= 1)
    {
        // creating element
        var.key = key;
        var.is_deleted = REC_NOT_DELETED;
        var.offset = ftell(dataFP) - sizeof(int);
        // skipping record
        fseek(dataFP, rec_size, SEEK_CUR);
        // writing record
        fwrite(&var, sizeof(struct PDS_NdxInfo), 1, ndxFP);

        rec_count++;
    }

    // writing to file
    fseek(ndxFP, 0, SEEK_SET);
    fwrite(&rec_count, sizeof(int), 1, ndxFP);
    fclose(ndxFP);
    fclose(dataFP);
}