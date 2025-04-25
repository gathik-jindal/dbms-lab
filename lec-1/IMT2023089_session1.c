#include <stdio.h>

struct student
{
  int rollnum;
  char name[30];
  int age;
};

int save_num_text(char *filename)
{
  // write your code
  int numbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

  // printf("Opening file\n");
  FILE *fp = fopen(filename, "w");

  if (fp == NULL)
  {
    printf("Error opening file\n");
    return 1;
  }

  // printf("Writing to file\n");
  for (int i = 0; i < 20; i++)
  {
    fprintf(fp, "%d\n", numbers[i]);
  }

  // printf("Closing file\n");
  fclose(fp);

  return 0;
}

int read_num_text(char *filename)
{
  // write your code
  FILE *fp = fopen(filename, "r");

  if (fp == NULL)
  {
    printf("Error opening file\n");
    return 1;
  }

  int num;
  while (fscanf(fp, "%d", &num) != EOF)
  {
    printf("%d\n", num);
  }

  fclose(fp);

  return 0;
}

int save_struct_text(char *filename)
{
  // write your code
  struct student s[5] = {
      {1, "A", 16},
      {2, "B", 17},
      {3, "C", 18},
      {4, "D", 19},
      {5, "E", 20}};

  // printf("Opening file\n");
  FILE *fp = fopen(filename, "w");

  if (fp == NULL)
  {
    printf("Error opening file\n");
    return 1;
  }

  // printf("Writing to file\n");
  for (int i = 0; i < 5; i++)
  {
    fprintf(fp, "%d %s %d\n", s[i].rollnum, s[i].name, s[i].age);
  }

  // printf("Closing file\n");
  fclose(fp);

  return 0;
}

int read_struct_text(char *filename)
{
  // write your code
  struct student s[5];

  // printf("Opening file\n");
  FILE *fp = fopen(filename, "r");

  if (fp == NULL)
  {
    printf("Error opening file\n");
    return 1;
  }

  char buffer[100];
  // printf("Reading file\n");
  while (fgets(buffer, 100, fp))
  {
    printf("%s", buffer);
  }

  fclose(fp);
}

int save_num_binary(char *filename)
{
  // write your code
  int numbers[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

  // printf("Opening file\n");
  FILE *fp = fopen(filename, "wb");

  if (fp == NULL)
  {
    printf("Error opening file\n");
    return 1;
  }

  // printf("Writing to file\n");
  int num = fwrite(numbers, sizeof(numbers[0]), 20, fp);

  if (num != 20)
  {
    printf("Error writing to file\n");
    return 1;
  }

  // printf("Print written successfully\n");

  fclose(fp);

  return 0;
}

int read_num_binary(char *filename)
{
  // write your code
  int numbers[20];

  // printf("Opening file\n");
  FILE *fp = fopen(filename, "rb");

  if (fp == NULL)
  {
    printf("Error opening file\n");
    return 1;
  }

  // printf("Reading file");
  int bytesRead = fread(numbers, sizeof(int), 20, fp);

  if (bytesRead != 20)
  {
    printf("Error reading file\n");
    return 1;
  }

  // printf("Printing data\n");

  for (int i = 0; i < 20; i++)
  {
    printf("%d\n", numbers[i]);
  }

  // printf("Closing file\n");
  fclose(fp);

  return 0;
}

int save_struct_binary(char *filename)
{
  // write your code
  struct student s[5] = {
      {1, 'A', 16},
      {2, 'B', 17},
      {3, 'C', 18},
      {4, 'D', 19},
      {5, 'E', 20}};

  FILE *fp = fopen(filename, "wb");

  if (fp == NULL)
  {
    printf("Error opening file\n");
    return 1;
  }

  fwrite(s, sizeof(struct student), 5, fp);

  fclose(fp);

  return 0;
}

int read_struct_binary(char *filename)
{
  // write your code
  FILE *fp = fopen(filename, "rb");

  if (fp == NULL)
  {
    printf("Error opening file\n");
    return 1;
  }

  struct student s;
  while (fread(&s, sizeof(struct student), 1, fp) > 0)
  {
    printf("%d %s %d\n", s.rollnum, s.name, s.age);
  }

  fclose(fp);

  return 0;
}
