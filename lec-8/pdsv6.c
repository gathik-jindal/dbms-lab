// IMT2023089

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pdsv7.h"

struct PDS_DBInfo repo_handle;

int pds_create_db(char *dbname)
{
    // creating dir
    int ret_value = mkdir(dbname, 0777);
    if (ret_value == -1)
        return PDS_FILE_ERROR;

    int cstatus = chdir(dbname);
    if (cstatus == -1)
        return PDS_FILE_ERROR;

    // creating file name
    char loc[500];
    sprintf(loc, "%s.info", dbname);
    FILE *fh = fopen(loc, "wb");
    // writing database name
    fwrite(dbname, sizeof(char), 30, fh);

    fclose(fh);

    // come to home directory
    cstatus = chdir("..");
    if (cstatus == -1)
        return PDS_FILE_ERROR;

    return PDS_SUCCESS;
}

int pds_open_db(char *dbname)
{
    if (repo_handle.repo_status == PDS_DB_OPEN)
        return PDS_DB_ALREADY_OPEN;

    // changing dir
    int cstatus = chdir(dbname);
    if (cstatus == -1)
        return PDS_FILE_ERROR;

    // opening file
    char loc[500];
    sprintf(loc, "%s.info", dbname);
    FILE *fh = fopen(loc, "rb+");
    repo_handle.repo_status = PDS_DB_OPEN;
    repo_handle.num_tables = 0;
    strcpy(repo_handle.pds_repo_name, dbname);

    // reading info
    char db_name[500];
    if (fh == NULL || fread(db_name, sizeof(char), 30, fh) < 30)
    {
        printf("%s\n", db_name);
        return PDS_FILE_ERROR;
    }

    // comparing info
    if (strcmp(db_name, dbname) != 0)
        return PDS_FILE_ERROR;

    fclose(fh);

    return PDS_SUCCESS;
}

struct PDS_TableInfo *helper_get_table_info(char *table_name)
{
    // searching for table
    for (int i = 0; i < MAX_TABLES; i++)
        if (strcmp(repo_handle.tables[i].table_name, table_name) == 0)
            return &repo_handle.tables[i];

    return NULL;
}

int pds_create_table(char *table_name)
{
    if (repo_handle.repo_status != PDS_DB_OPEN)
        return PDS_DB_NOT_OPEN;

    // creating file pointers and file names
    FILE *fp_index, *fp_data;
    char data_file_name[100], index_file_name[100];
    sprintf(data_file_name, "%s.dat", table_name);
    sprintf(index_file_name, "%s.ndx", table_name);

    // creating files
    fp_index = fopen(index_file_name, "wb");
    fp_data = fopen(data_file_name, "wb");

    if (fp_index == NULL || fp_data == NULL)
        return PDS_FILE_ERROR;

    // writing zero
    int zero = 0;
    if (fwrite(&zero, sizeof(int), 1, fp_index) < 1)
        return PDS_FILE_ERROR;

    fclose(fp_data);
    fclose(fp_index);

    return PDS_SUCCESS;
}

int pds_open_table(char *table_name, int rec_size)
{
    if (repo_handle.repo_status != PDS_DB_OPEN)
        return PDS_DB_NOT_OPEN;

    struct PDS_TableInfo *tb_handle;
    if ((tb_handle = helper_get_table_info(table_name)) != NULL && tb_handle->table_status == PDS_TABLE_OPEN)
        return PDS_FILE_ERROR;

    // creating file pointers and file names
    FILE *fp_index, *fp_data;
    char data_file_name[100], index_file_name[100];
    sprintf(data_file_name, "%s.dat", table_name);
    sprintf(index_file_name, "%s.ndx", table_name);

    // opening tables
    fp_data = fopen(data_file_name, "rb+");
    fp_index = fopen(index_file_name, "rb+");

    if (fp_data == NULL || fp_index == NULL)
        return PDS_FILE_ERROR;

    // loading tableInfo appropriately
    if (tb_handle == NULL)
        tb_handle = &repo_handle.tables[repo_handle.num_tables++];

    tb_handle->data_fp = fp_data;
    tb_handle->ndx_fp = fp_index;
    tb_handle->ndx_root = NULL;
    strcpy(tb_handle->table_name, table_name);
    tb_handle->rec_size = rec_size;
    tb_handle->table_status = PDS_TABLE_OPEN;

    // loading index file
    if (pds_load_ndx(tb_handle) == PDS_FILE_ERROR)
        return PDS_FILE_ERROR;

    // fclose(fp_index);
    // tb_handle->ndx_fp = NULL;

    return PDS_SUCCESS;
}

int pds_load_ndx(struct PDS_TableInfo *tablePtr)
{
    // loading index
    tablePtr->ndx_root = NULL;
    fread(&tablePtr->rec_count, sizeof(int), 1, tablePtr->ndx_fp);

    // reading index file
    for (int i = 0; i < tablePtr->rec_count; i++)
    {
        struct PDS_NdxInfo *temp = (struct PDS_NdxInfo *)malloc(sizeof(struct PDS_NdxInfo));
        if (fread(temp, sizeof(struct PDS_NdxInfo), 1, tablePtr->ndx_fp) < 1)
        {
            printf("%d %d\n", i, tablePtr->rec_count);
            return PDS_FILE_ERROR;
        }

        // adding the node
        if (bst_add_node(&tablePtr->ndx_root, temp->key, temp) != BST_SUCCESS)
            return PDS_ADD_FAILED;
    }

    return PDS_SUCCESS;
}

int put_rec_by_key(char *table_name, int key, void *rec)
{
    // if repo is not opened
    if (repo_handle.repo_status != PDS_DB_OPEN)
        return PDS_DB_NOT_OPEN;

    struct PDS_TableInfo *tb_handle = helper_get_table_info(table_name);
    if (tb_handle == NULL)
        return PDS_FILE_ERROR;

    // if repo is not opened
    if (tb_handle->table_status != PDS_TABLE_OPEN)
        return PDS_TABLE_NOT_OPEN;

    // checking if key is duplicate
    if (bst_search(tb_handle->ndx_root, key) != NULL)
        return PDS_ADD_FAILED;

    // if error in seek
    if (fseek(tb_handle->data_fp, 0, SEEK_END) != 0)
        return PDS_FILE_ERROR;

    // getting current location
    int loc = ftell(tb_handle->data_fp);

    // adding index entry
    struct PDS_NdxInfo *temp = (struct PDS_NdxInfo *)malloc(sizeof(struct PDS_NdxInfo));
    temp->offset = loc;
    temp->key = key;
    temp->is_deleted = REC_NOT_DELETED;
    if (bst_add_node(&tb_handle->ndx_root, key, temp) != BST_SUCCESS)
        return PDS_ADD_FAILED;

    // writing to file
    if (fwrite(&key, sizeof(int), 1, tb_handle->data_fp) != 1)
        return PDS_ADD_FAILED;
    if (fwrite(rec, tb_handle->rec_size, 1, tb_handle->data_fp) != 1)
        return PDS_ADD_FAILED;

    // updating count
    tb_handle->rec_count++;

    return PDS_SUCCESS;
}

int get_rec_by_key(char *table_name, int key, void *rec)
{
    // if repo is not opened
    if (repo_handle.repo_status != PDS_DB_OPEN)
        return PDS_DB_NOT_OPEN;

    struct PDS_TableInfo *tb_handle = helper_get_table_info(table_name);
    if (tb_handle == NULL)
        return PDS_FILE_ERROR;

    // if repo is not opened
    if (tb_handle->table_status != PDS_TABLE_OPEN)
        return PDS_TABLE_NOT_OPEN;

    // if error in seek
    if (fseek(tb_handle->data_fp, 0, SEEK_SET) != 0)
        return PDS_FILE_ERROR;

    // finding offset
    struct BST_Node *temp = bst_search(tb_handle->ndx_root, key);

    // reading from the file
    if (temp != NULL && ((struct PDS_NdxInfo *)temp->data)->is_deleted == REC_NOT_DELETED)
    {
        if (fseek(tb_handle->data_fp, ((struct PDS_NdxInfo *)temp->data)->offset, SEEK_SET))
            return PDS_FILE_ERROR;

        int var;
        fread(&var, sizeof(int), 1, tb_handle->data_fp);
        fread(rec, tb_handle->rec_size, 1, tb_handle->data_fp);
        return PDS_SUCCESS;
    }
    else
        return PDS_REC_NOT_FOUND;
}

int get_rec_by_field(char *table_name, void *searchvalue, void *rec, int (*matcher)(void *rec, void *searchvalue), int *comp_count)
{
    // if repo is not opened
    if (repo_handle.repo_status != PDS_DB_OPEN)
        return PDS_DB_NOT_OPEN;

    struct PDS_TableInfo *tb_handle = helper_get_table_info(table_name);
    if (tb_handle == NULL)
        return PDS_FILE_ERROR;

    // checking if repo is open
    if (tb_handle->table_status != PDS_TABLE_OPEN)
        return PDS_TABLE_NOT_OPEN;

    // searching by field
    int key;
    *comp_count = 0;
    fseek(tb_handle->data_fp, 0, SEEK_SET);
    while (fread(&key, sizeof(int), 1, tb_handle->data_fp) > 0)
    {
        (*comp_count)++;
        fread(rec, tb_handle->rec_size, 1, tb_handle->data_fp);
        if (matcher(rec, searchvalue) == 0)
        {
            // checking if record is deleted
            if (((struct PDS_NdxInfo *)bst_search(tb_handle->ndx_root, key)->data)->is_deleted != REC_NOT_DELETED)
                return PDS_REC_NOT_FOUND;

            return PDS_SUCCESS;
        }
    }

    // returning if not found
    return PDS_REC_NOT_FOUND;
}

int delete_rec_by_key(char *table_name, int key)
{
    // if repo is not opened
    if (repo_handle.repo_status != PDS_DB_OPEN)
        return PDS_DB_NOT_OPEN;

    struct PDS_TableInfo *tb_handle = helper_get_table_info(table_name);
    if (tb_handle == NULL)
        return PDS_FILE_ERROR;

    // checking if repo is open
    if (tb_handle->table_status != PDS_TABLE_OPEN)
        return PDS_TABLE_NOT_OPEN;

    // fetching index to delete
    struct PDS_NdxInfo *ndx_to_delete = bst_search(tb_handle->ndx_root, key)->data;

    // checking if index is already deleted
    if (ndx_to_delete == NULL || ndx_to_delete->is_deleted != REC_NOT_DELETED)
        return PDS_REC_NOT_FOUND;

    // setting it to deleted
    ndx_to_delete->is_deleted = REC_DELETED;

    return PDS_SUCCESS;
}

int bst_pre_order_traversal(struct BST_Node *root, FILE *fp)
{
    // Pre-order traversal
    if (root == NULL)
        return PDS_SUCCESS;
    else
    {
        // writing to file
        if (fwrite(root->data, sizeof(struct PDS_NdxInfo), 1, fp) < 1)
            return PDS_FILE_ERROR;

        // checking for errors
        if (bst_pre_order_traversal(root->left_child, fp) != PDS_SUCCESS || bst_pre_order_traversal(root->right_child, fp) != PDS_SUCCESS)
            return PDS_FILE_ERROR;

        return PDS_SUCCESS;
    }
}

int pds_close_table(char *table_name)
{
    // if repo is not opened
    if (repo_handle.repo_status != PDS_DB_OPEN)
        return PDS_DB_NOT_OPEN;

    struct PDS_TableInfo *tb_handle = helper_get_table_info(table_name);
    if (tb_handle == NULL)
        return PDS_FILE_ERROR;

    // if repo is not opened
    if (tb_handle->table_status != PDS_TABLE_OPEN)
        return PDS_TABLE_NOT_OPEN;

    char repo_ndx_name[30];

    // generating name
    strcpy(repo_ndx_name, tb_handle->table_name);
    strcat(repo_ndx_name, ".ndx");

    // wrtiting to ndx file
    tb_handle->ndx_fp = fopen(repo_ndx_name, "wb");
    if (tb_handle->ndx_fp == NULL)
        return PDS_NDX_SAVE_FAILED;
    if (fwrite(&tb_handle->rec_count, sizeof(int), 1, tb_handle->ndx_fp) < 1)
        return PDS_NDX_SAVE_FAILED;

    if (bst_pre_order_traversal(tb_handle->ndx_root, tb_handle->ndx_fp) != PDS_SUCCESS)
        return PDS_NDX_SAVE_FAILED;

    // destroying tree
    bst_destroy(tb_handle->ndx_root);
    tb_handle->ndx_root = NULL;

    // closing the file
    fclose(tb_handle->data_fp);
    fclose(tb_handle->ndx_fp);

    // NULL-ing values
    tb_handle->data_fp = NULL;
    tb_handle->ndx_fp = NULL;
    tb_handle->table_status = PDS_TABLE_CLOSED;

    return PDS_SUCCESS;
}

int pds_close_db(char *dbname)
{
    // if repo is not opened
    if (repo_handle.repo_status != PDS_DB_OPEN)
        return PDS_DB_NOT_OPEN;

    for (int i = 0; i < MAX_TABLES; i++)
    {
        if (repo_handle.tables[i].table_status == PDS_TABLE_OPEN)
            pds_close_table(repo_handle.tables[i].table_name);
    }

    repo_handle.repo_status = PDS_DB_CLOSED;

    int cstatus = chdir("..");
    if (cstatus == -1)
        return PDS_FILE_ERROR;

    return PDS_SUCCESS;
}