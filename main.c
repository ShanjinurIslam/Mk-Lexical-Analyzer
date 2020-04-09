#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define VECTOR_INIT_CAPACITY 4

char letters[53];
char characters[63];
char digits[10];

//implementation of vector for dynamic string array

typedef struct vector
{
    void **items;
    int capacity;
    int total;
} vector;

void vector_init(vector *v)
{
    v->capacity = VECTOR_INIT_CAPACITY;
    v->total = 0;
    v->items = malloc(sizeof(void *) * v->capacity);
}

int vector_total(vector *v)
{
    return v->total;
}

void vector_resize(vector *v, int capacity)
{
    void **items = realloc(v->items, sizeof(void *) * capacity);
    if (items)
    {
        v->items = items;
        v->capacity = capacity;
    }
}

void vector_add(vector *v, void *item)
{
    if (v->capacity == v->total)
        vector_resize(v, v->capacity * 2);
    v->items[v->total++] = item;
}

void vector_set(vector *v, int index, void *item)
{
    if (index >= 0 && index < v->total)
        v->items[index] = item;
}

void *vector_get(vector *v, int index)
{
    if (index >= 0 && index < v->total)
        return v->items[index];
    return NULL;
}

void vector_delete(vector *v, int index)
{
    if (index < 0 || index >= v->total)
        return;

    v->items[index] = NULL;

    for (int i = index; i < v->total - 1; i++)
    {
        v->items[i] = v->items[i + 1];
        v->items[i + 1] = NULL;
    }

    v->total--;

    if (v->total > 0 && v->total == v->capacity / 4)
        vector_resize(v, v->capacity / 2);
}

void vector_free(vector *v)
{
    free(v->items);
}

// end of vector implementation

void generate_letters()
{
    int i = 0;
    for (i = 0; i < 26; i++)
    {
        letters[i] = 'a' + i;
    }

    for (i = 26; i < 2 * 26; i++)
    {
        letters[i] = 'A' + i;
    }

    letters[2 * 26] = '_';
}

void generate_digits()
{
    int i = 0;
    for (i = 0; i < 10; i++)
    {
        digits[i] = '0' + i;
    }
}

void generate_characters()
{
    int i = 0;
    for (i = 0; i < 53; i++)
    {
        characters[i] = letters[i];
    }

    for (i = 0; i < 10; i++)
    {
        characters[53 + i] = digits[i];
    }
}

int in_letters(char a)
{
    int ret = 0;
    int i = 0;
    for (i = 0; i < 53; i++)
    {
        if (a == letters[i])
        {
            ret = 1;
            break;
        }
    }
    return ret;
}

int in_chars(char a)
{
    int ret = 0;
    int i = 0;
    for (i = 0; i < 63; i++)
    {
        if (a == characters[i])
        {
            ret = 1;
            break;
        }
    }
    return ret;
}

int in_digits(char a)
{
    int ret = 0;
    int i = 0;
    for (i = 0; i < 10; i++)
    {
        if (a == digits[i])
        {
            ret = 1;
            break;
        }
    }
    return ret;
}

char *get_identifier(char *arr)
{
    int len = strlen(arr);
    char *out = (char *)malloc(len * sizeof(char));
    int i = 0;
    int j = 0;
    for (i = 0; i < len; i++)
    {
        if (in_chars(arr[i]) && arr[i]!='[' && arr[i]!=']' && arr[i]!=':' && arr[i]!=',')
        {
            out[j++] = arr[i];
        }
        else{
            break ;
        }
    }

    out[j] = '\0';
    return out;
}

char *get_ints(char *arr)
{
    int len = strlen(arr);
    char *out = (char *)malloc(len * sizeof(char));
    int i = 0;
    int j = 0;
    for (i = 0; i < len; i++)
    {
        if (in_digits(arr[i]))
        {
            out[j++] = arr[i];
        }
        else{
            break ;
        }
    }
    return out;
}

void get_tokens(char *line)
{
    vector v;
    vector_init(&v);
    int len = strlen(line);
    int i = 0 ;
    while(i<len)
    {
        char *let = "let";
        char substring[500];
        int j = 0;
    
        for (j = 0; j < i + 3; j++)
        {
            substring[j] = line[i + j];
        }

        substring[j] = '\0';
        if (!strcmp(let, substring))
        {
            vector_add(&v, "TOK_LET");
            i = i + 3;
            continue;
        }
        else if (line[i] == '[')
        {
            vector_add(&v, "TOK_SQ_BKT_L");
            i += 1;
        }
        else if (line[i] == ']')
        {
            vector_add(&v, "TOK_SQ_BKT_R");
            i += 1;
        }
        else if (line[i] == ':')
        {
            vector_add(&v, "TOK_COLON");
            i += 1;
        }
        else if (line[i] == ';')
        {
            vector_add(&v, "TOK_SEMICOLON");
            i += 1;
        }
        else if (line[i] == ',')
        {
            vector_add(&v, "TOK_COMMA");
            i += 1;
        }

        else if (line[i] == '=')
        {
            vector_add(&v, "TOK_EQ");
            i += 1;
        }

        else if (in_letters(line[i]))
        {
            
            int sub_len = len - i + 10;
            int j;
            char *arr = (char *)malloc(sub_len * sizeof(char));
            for (j = 0; j < len - i + 1; j++)
            {
                arr[j] = line[i + j];
            }
            arr[j] = '\0' ;
            char *name = get_identifier(arr);
            char *tok_name = (char*)malloc((strlen(name) + 20)*sizeof(char));
            strcpy(tok_name, "TOK_IDENT(\"");
            strcat(tok_name, name);
            strcat(tok_name, "\")");
            
            vector_add(&v, tok_name);
            
            i += strlen(name);
        }

        else if (in_digits(line[i]))
        {
            int sub_len = len - i + 10;
            int j;
            char *arr = (char *)malloc(sub_len * sizeof(char));
            for (j = 0; j < len - i + 1; j++)
            {
                arr[j] = line[i + j];
            }
            arr[j] = '\0' ;
            char *name = get_ints(arr);
            char *tok_name = (char*)malloc((strlen(name) + 20)*sizeof(char));
            strcpy(tok_name, "TOK_INT_LIT(\"");
            strcat(tok_name, name);
            strcat(tok_name, "\")");
            
            vector_add(&v, tok_name);
            i += strlen(name) ;
        }

        else if (line[i] == ' ' || line[i] == '\n')
        {
            i += 1;
        }

        else
        {
            //vector_add(&v, "TOK_INVALID");
            i += 1;
        }
    }
    
    printf("[\n");
    for (i = 0; i < vector_total(&v); i++)
        printf("%s , \n", (char *)vector_get(&v, i));
    printf("]\n");
    

    vector_free(&v);
}

int main()
{
    generate_letters();
    generate_digits();
    generate_characters();
     FILE * fp;
    char * line = NULL;
    size_t len = 0;
    size_t read;

    fp = fopen("input.mk", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        get_tokens(line);
        printf("\n");
    }

    fclose(fp);
    if (line)
        free(line);
    return 0 ;
}