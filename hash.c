#define _POSIX_C_SOURCE 200809L

#include "trove.h"

//  --------------------------------------------------------------------

//  FUNCTION hash_string() ACCEPTS A STRING PARAMETER,
//  AND RETURNS AN UNSIGNED 32-BIT INTEGER AS ITS RESULT

//  'CREATE' A NEW, EMPTY LIST - JUST A NULL POINTER
LIST *list_new(void)
{
    return NULL;
}

//  DETERMINE IF A REQUIRED ITEM (A STRING) IS STORED IN A GIVEN LIST
bool list_find(LIST *list, char *wanted)
{
    while (list != NULL)
    {
        if (strcmp(list->string, wanted) == 0)
        {
            return true;
        }
        list = list->next;
    }
    return false;
}

//  ALLOCATE SPACE FOR A NEW LIST ITEM, TESTING THAT ALLOCATION SUCCEEDS
LIST *list_new_item(char *newstring)
{
    LIST *new = calloc(1, sizeof(LIST));
    if (new == NULL)
    {
        perror("Error allocation memory");
    }
    new->string = strdup(newstring);

    // printf("%s:%s\n", new->string, new->word);
    if (new->string == NULL )
    {
        perror("Error allocation memory");
    }
    new->next = NULL;
    return new;
}

//  ADD A NEW (STRING) ITEM TO AN EXISTING LIST
LIST *list_add(LIST *list, char *newstring)
{
    if (list_find(list, newstring))
    { // only add each item once
        return list;
    }
    else
    { // add new item to head of list
        LIST *new = list_new_item(newstring);
        new->next = list;
        return new;
    }
}

//  PRINT EACH ITEM (A STRING) IN A GIVEN LIST TO stdout
void list_print(LIST *list)
{
    if (list != NULL)
    {
        while (list != NULL)
        {
            printf("%s", list->string);
            if (list->next != NULL)
            {
                printf(" -> ");
            }
            list = list->next;
        }
        printf("\n");
    }
}

uint32_t hash_string(char *string)
{
    uint32_t hash = 0;

    while (*string != '\0')
    {
        hash = hash * 33 + *string;
        ++string;
    }
    return hash;
}

//  ALLOCATE AND INITIALISE SPACE FOR A NEW HASHTABLE (AN ARRAY OF LISTS)
HASHTABLE *hashtable_new(void)
{
    HASHTABLE *new = calloc(HASHTABLE_SIZE, sizeof(LIST *));

    if (new == NULL)
    {
        perror("Couldnt Allocate Space for the Hashmap");
        exit(EXIT_FAILURE);
    }
    return new;
}

//  ADD A NEW STRING TO A GIVEN HASHTABLE
void hashtable_add(HASHTABLE *hashtable, char *string)
{
    uint32_t h = hash_string(string) % HASHTABLE_SIZE;
    // printf("%s:%s\n", string);
    hashtable[h] = list_add(hashtable[h], string);
}

//  DETERMINE IF A REQUIRED STRING ALREADY EXISTS IN A GIVEN HASHTABLE
bool hashtable_find(HASHTABLE *hashtable, char *string)
{
    uint32_t h = hash_string(string) % HASHTABLE_SIZE;

    return list_find(hashtable[h], string);
}

void hashtable_print(HASHTABLE *h)
{
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        printf("--%i--\n", i);
        list_print(h[i]);
    }
}
