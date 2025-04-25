// IMT2023089

#include "pds.h"
#include <string.h>
#include <stdio.h>

struct PDS_RepoInfo repo_handle;

int pds_open(char *repo_name, int rec_size)
{
    strcpy(repo_handle.pds_name, repo_name); // copying repo name
    strcat(repo_handle.pds_name, ".dat");    // adding .dat file extension

    // if file is already open
    if (repo_handle.repo_status == PDS_REPO_OPEN)
        return PDS_REPO_ALREADY_OPEN;

    // opening file
    FILE *fp = fopen(repo_handle.pds_name, "ab+");

    // if error in opening file
    if (fp == NULL)
        return PDS_FILE_ERROR;

    repo_handle.pds_data_fp = fp;
    repo_handle.rec_size = rec_size;
    repo_handle.repo_status = PDS_REPO_OPEN;

    return PDS_SUCCESS;
}

int put_rec_by_key(int key, void *rec)
{
    // if repo is not opened
    if (repo_handle.repo_status == PDS_REPO_CLOSED)
        return PDS_REPO_ALREADY_CLOSED;

    // if error in seek
    if (fseek(repo_handle.pds_data_fp, 0, SEEK_END) != 0)
        return PDS_FILE_ERROR;

    // writing to file
    if (fwrite(&key, sizeof(int), 1, repo_handle.pds_data_fp) < 1)
        return PDS_ADD_FAILED;
    if (fwrite(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp) < 1)
        return PDS_ADD_FAILED;

    return PDS_SUCCESS;
}

int get_rec_by_key(int key, void *rec)
{
    // if repo is not opened
    if (repo_handle.repo_status == PDS_REPO_CLOSED)
        return PDS_REPO_ALREADY_CLOSED;

    // if error in seek
    if (fseek(repo_handle.pds_data_fp, 0, SEEK_SET) != 0)
        return PDS_FILE_ERROR;

    // finding file
    int keyread, flag = 0;
    while (fread(&keyread, sizeof(int), 1, repo_handle.pds_data_fp) == 1)
    {
        if (keyread == key)
        {
            // Record found
            flag = 1;
            break;
        }
        // Skip the rest of the record
        fseek(repo_handle.pds_data_fp, repo_handle.rec_size, SEEK_CUR);
    }

    if (flag)
    {
        fread(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);
        return PDS_SUCCESS;
    }
    else
        return PDS_REC_NOT_FOUND;
}

int pds_close()
{
    // if repo is not opened
    if (repo_handle.repo_status == PDS_REPO_CLOSED)
        return PDS_REPO_ALREADY_CLOSED;

    fclose(repo_handle.pds_data_fp);
    repo_handle.pds_data_fp = NULL;
    repo_handle.repo_status = PDS_REPO_CLOSED;

    return PDS_SUCCESS;
}