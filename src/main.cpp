#include <iostream>
#include <string>

#include <stdio.h>


struct Word
{
    char* word;
    uint16_t offset;
};

void init_word      (Word* w);
void update_word    (Word* w, const char* const wrd, uint16_t offset);
void display_word   (FILE* out, Word w);
void free_word      (Word w);


struct Word_List
{
    struct Word* words;     // Word[]
    uint16_t size;          // #elements - allocated
    uint16_t count;         // #elements - current
};

void init_list          (Word_List* word_list, uint16_t size);
void add_word           (Word_List* word_list, Word word);
Word get_word_at_index  (Word_List word_list, uint16_t index);
void free_list          (Word_List word_list);
void display_words      (FILE* out, Word_List word_list);


//==========
// utils
//==========

uint32_t strlength (const char* const word)
{
    const char* ptr = word;

    while (*ptr)
        ptr++;

    return ptr-word;
}

void strcopy (char* dest, const char* src)
{
    int i = 0;

    // src = "abc"

    while (*(src+i))
    {
        *(dest+i) = *(src+i);
        i++;
    }

    *(dest+i) = '\0';
}

//==========
// Word
//==========

void init_word (Word* w)
{
    w->word     = NULL;
    w->offset   = 0;
}

void update_word(   Word* w
                    ,const char* const wrd
                    ,uint16_t offset
                    )
{
    if (w->word)
    {
        free (w->word);
    }

    w->word = (char*) malloc(strlength(wrd)+1);

    strcopy (w->word,wrd);

    w->offset = offset;
}

// output the word to the given file
void display_word ( FILE* out
                    ,Word w
                    )
{
    fprintf (out, "%-20s @ Offset: %02x\n", w.word, w.offset);
}


void free_word (Word w)
{
    free (w.word);
}


//==========
// Word_List
//==========

void init_list (Word_List* word_list, uint16_t size)
{
    word_list->words    = (Word*) malloc (size*sizeof(Word));
    word_list->size     = size;
    word_list->count    = 0;
}

// append the given Word
void add_word (Word_List* word_list, Word word)
{
    if (word_list->size == word_list->count)
    {
        uint16_t new_size = (word_list->size) * 2;

        word_list->words    = (Word*) realloc(word_list->words, new_size * sizeof(Word));
        word_list->size     = new_size;
    }

    word_list->words[word_list->count] = word;
    word_list->count++;
}

Word get_word_at_index (Word_List word_list, uint16_t index)
{
    if (index >= 0 && index < word_list.count)
    {
        // copy of word will be returned

        Word word;
        
        init_word(&word);
        update_word(&word, word_list.words[index].word, word_list.words[index].offset);

        return word;
    }
    else
    {
        Word invalid_word;
        init_word(&invalid_word);
        update_word(&invalid_word, "Word Not Foun", 0);

        return invalid_word;
    }
}

// loop over the words in the list, free each word
// then free the word array itself
void free_list (Word_List word_list)
{
    for (int i=0; i < word_list.count; i++)
    {
        free_word (word_list.words[i]);        
    }

    free (word_list.words);
}

// loop over each word in the list and display each word
void display_words (FILE* out, Word_List word_list)
{
    for (int i=0; i < word_list.count; i++)
    {
        display_word(out, word_list.words[i]);
    }
}



int main (int argc, const char* argv[])
{
    if (argc != 2)
    {
        printf ("usage: p1 <file>\n");
        return 1;
    }

    printf ("file=|%s|\n",argv[1]);

    FILE* binary_file = fopen(argv[1],"rb");

    if (binary_file == NULL)
    {
        printf ("error opening file |%s|", argv[1]);
        return 1;
    }

    // if here, successfully opened file

    // read N bytes - it wasn't exactly clear to me where the # of bytes would come from

    size_t N = 1024;   // just choosing some random number

    uint8_t* arr = (uint8_t*) malloc (N);


    size_t num_bytes = fread (arr, N, sizeof(uint8_t), binary_file);

    if (num_bytes != N)
    {
        printf ("unexpected number of bytes read. got %zu. expecting %zu\n", num_bytes, N);
        return 1;
    }

    // at this point, we have a uint8_t array

    uint16_t* num_words = (uint16_t*)arr+0;

    uint16_t* offset    = (uint16_t*)arr+2;


    for (int i=0; i < *num_words; i++)
    {
        uint8_t num_bytes_in_rec = arr[*offset];    // 1 byte 

        offset = (uint16_t*) arr+(*offset)+1;       // 2 bytes

        uint8_t num_bytes_in_word = num_bytes_in_rec - 3;
    }
    
    return 0;
}

