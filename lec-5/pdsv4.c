// IMT2023089

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "contact.h"
#include "pdsv4.h"

struct PDS_RepoInfo repo_handle;

int pds_create(char *repo_name)
{
    // creating string copies
    char fileDat[30], fileNDX[30];
    strcpy(repo_handle.pds_name, repo_name);
    strcpy(fileDat, repo_name);
    strcpy(fileNDX, repo_name);

    strcat(fileDat, ".dat"); // adding .dat file extension
    strcat(fileNDX, ".ndx"); // adding .ndx file extension

    // opening files
    FILE *fp = fopen(fileDat, "wb");
    FILE *ifp = fopen(fileNDX, "wb");

    // checking if file opened successfully
    if (fp == NULL || ifp == NULL)
        return PDS_FILE_ERROR;

    int zero = 0;
    // checking if written successfully
    if (fwrite(&zero, sizeof(int), 1, ifp) < 1)
        return PDS_FILE_ERROR;

    // closing files
    fclose(fp);
    fclose(ifp);

    // returning
    return PDS_SUCCESS;
}

int pds_open(char *repo_name, int rec_size)
{
    // if file is already open
    if (repo_handle.repo_status == PDS_REPO_OPEN)
        return PDS_REPO_ALREADY_OPEN;

    char repo_dat_name[30], repo_ndx_name[30];

    strcpy(repo_handle.pds_name, repo_name); // copying repo name
    strcpy(repo_dat_name, repo_name);        // copying repo name
    strcpy(repo_ndx_name, repo_name);        // copying repo name

    strcat(repo_dat_name, ".dat"); // adding .dat file extension
    strcat(repo_ndx_name, ".ndx"); // adding .ndx file extension

    // opening file
    FILE *ifp = fopen(repo_ndx_name, "rb+");
    FILE *fp = fopen(repo_dat_name, "rb+");

    // if error in opening file
    if (ifp == NULL)
        return PDS_FILE_ERROR;

    // reading the data
    repo_handle.pds_data_fp = fp;
    repo_handle.rec_size = rec_size;
    repo_handle.repo_status = PDS_REPO_OPEN;
    repo_handle.pds_ndx_fp = ifp;

    // reading index file
    if (pds_load_ndx() != PDS_SUCCESS)
        return PDS_FILE_ERROR;

    // closing file
    fclose(ifp);
    repo_handle.pds_ndx_fp = NULL;

    return PDS_SUCCESS;
}

int pds_load_ndx()
{
    // loading index
    repo_handle.ndx_root = NULL;
    fread(&repo_handle.rec_count, sizeof(int), 1, repo_handle.pds_ndx_fp);
    struct PDS_NdxInfo *temp = (struct PDS_NdxInfo *)malloc(sizeof(struct PDS_NdxInfo));

    // reading index file
    for (int i = 0; i < repo_handle.rec_count; i++)
    {
        if (fread(temp, sizeof(struct PDS_NdxInfo), 1, repo_handle.pds_ndx_fp) < 1)
            return PDS_FILE_ERROR;

        // adding the node
        int *temp1 = (int *)malloc(sizeof(int));
        *temp1 = temp->offset;
        if (bst_add_node(&repo_handle.ndx_root, temp->key, temp1) != BST_SUCCESS)
            return PDS_ADD_FAILED;
    }

    return PDS_SUCCESS;
}

int put_rec_by_key(int key, void *rec)
{
    // if repo is not opened
    if (repo_handle.repo_status != PDS_REPO_OPEN)
        return PDS_REPO_NOT_OPEN;

    // checking if key is duplicate
    if (bst_search(repo_handle.ndx_root, key) != NULL)
        return PDS_ADD_FAILED;

    // if error in seek
    if (fseek(repo_handle.pds_data_fp, 0, SEEK_END) != 0)
        return PDS_FILE_ERROR;

    // getting current location
    int loc = ftell(repo_handle.pds_data_fp);

    // adding index entry
    int *temp = (int *)malloc(sizeof(int));
    *temp = loc;
    if (bst_add_node(&repo_handle.ndx_root, key, temp) != BST_SUCCESS)
        return PDS_ADD_FAILED;

    // writing to file
    if (fwrite(&key, sizeof(int), 1, repo_handle.pds_data_fp) < 1)
        return PDS_ADD_FAILED;
    if (fwrite(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp) < 1)
        return PDS_ADD_FAILED;

    // updating count
    repo_handle.rec_count++;

    return PDS_SUCCESS;
}

int get_rec_by_key(int key, void *rec)
{
    // if repo is not opened
    if (repo_handle.repo_status != PDS_REPO_OPEN)
        return PDS_REPO_NOT_OPEN;

    // if error in seek
    if (fseek(repo_handle.pds_data_fp, 0, SEEK_SET) != 0)
        return PDS_FILE_ERROR;

    // finding offset
    struct BST_Node *temp = bst_search(repo_handle.ndx_root, key);

    // reading from the file
    if (temp != NULL)
    {
        if (fseek(repo_handle.pds_data_fp, *((int *)temp->data), SEEK_SET))
            return PDS_FILE_ERROR;

        printf("%d %d\n", *((int *)temp->data), temp->key);
        int var;
        fread(&var, sizeof(int), 1, repo_handle.pds_data_fp);
        fread(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);
        return PDS_SUCCESS;
    }
    else
        return PDS_REC_NOT_FOUND;
}

int get_rec_by_field(void *searchvalue, void *rec, int (*matcher)(void *rec, void *searchvalue), int *comp_count)
{
    // checking if repo is open
    if (repo_handle.repo_status != PDS_REPO_OPEN)
        return PDS_REPO_NOT_OPEN;

    // searching by field
    int key;
    *comp_count = 0;
    fseek(repo_handle.pds_data_fp, 0, SEEK_SET);
    while (fread(&key, sizeof(int), 1, repo_handle.pds_data_fp))
    {
        (*comp_count)++;
        fread(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);
        if (matcher(rec, searchvalue) == 0)
            return PDS_SUCCESS;
    }

    // returning if not found
    *comp_count = -1;
    return PDS_REC_NOT_FOUND;
}

int bst_pre_order_traversal(struct BST_Node *root)
{
    // Pre-order traversal
    if (root == NULL)
        return PDS_SUCCESS;
    else
    {
        // creating struct
        struct PDS_NdxInfo temp = {root->key, *((int *)root->data)};

        // writing to file
        if (fwrite(&temp, sizeof(struct PDS_NdxInfo), 1, repo_handle.pds_ndx_fp) < 1)
            return PDS_FILE_ERROR;

        // checking for errors
        if (bst_pre_order_traversal(root->left_child) != PDS_SUCCESS || bst_pre_order_traversal(root->right_child) != PDS_SUCCESS)
            return PDS_FILE_ERROR;

        return PDS_SUCCESS;
    }
}

int pds_close(char *repo_name)
{
    // if repo is not opened
    if (repo_handle.repo_status != PDS_REPO_OPEN || strcmp(repo_handle.pds_name, repo_name) != 0)
        return PDS_REPO_NOT_OPEN;

    char repo_ndx_name[30];

    // generating name
    strcpy(repo_ndx_name, repo_handle.pds_name);
    strcat(repo_ndx_name, ".ndx");

    // wrtiting to ndx file
    repo_handle.pds_ndx_fp = fopen(repo_ndx_name, "wb");
    if (repo_handle.pds_ndx_fp == NULL)
        return PDS_NDX_SAVE_FAILED;
    if (fwrite(&repo_handle.rec_count, sizeof(int), 1, repo_handle.pds_ndx_fp) < 1)
        return PDS_NDX_SAVE_FAILED;

    if (bst_pre_order_traversal(repo_handle.ndx_root) != PDS_SUCCESS)
        return PDS_NDX_SAVE_FAILED;
    
    // destroying tree
    // bst_destroy(repo_handle.ndx_root);
    bst_free(repo_handle.ndx_root);
    repo_handle.ndx_root = NULL;

    // closing the file
    fclose(repo_handle.pds_data_fp);
    fclose(repo_handle.pds_ndx_fp);

    // NULL-ing values
    repo_handle.pds_data_fp = NULL;
    repo_handle.pds_ndx_fp = NULL;
    repo_handle.repo_status = PDS_REPO_CLOSED;

    return PDS_SUCCESS;
}