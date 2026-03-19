// no-comment.c
// Řešení příkladu IJC-DU1, příklad b), 9.3.2026
// Autor: Antonín Hrnčíř, FIT
// Přeloženo: gcc 15.2.1
/* Program pro odstraňení komentářů ze zdrojového kódu v jazyce C
    s využitím stavového automatu
    - OMEZENÍ: Není zaručena správnost pro chybný zdrojový kód */

#define _POSIX_C_SOURCE 2 //pro fileno()
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> //pro fstat()
#include <stdbool.h>
#include "error.h"

//stavy automatu
typedef enum {
    CODE, //cast kodu
    STR_LIT, //string literal
    CHAR_LIT, //char literal
    SLASH_COMM, //tento typ komentare
    STAR_COMM, /*tento typ komentare*/
} State;

//specialni pravidla pro dany charakter v retezci
typedef enum {
    NOTHING, //bez omezeni
    SLASH_READ, //charakter '/' precten
    STAR_READ, //charakter '*' precten
    MISSING_SLASH, //charakter '/' precten, ale nezapsan
} Special;

//kontrola, jestli stdout neni presmerovany na zdrojovy soubor
static inline void assert_different_from_stdout(FILE *file)
{
    struct stat out_stat;
    struct stat file_stat;

    int out_descriptor = fileno(stdout);
    int file_descriptor = fileno(file);

    fstat(out_descriptor, &out_stat);
    fstat(file_descriptor, &file_stat);

    if (out_stat.st_dev == file_stat.st_dev &&
        out_stat.st_ino == file_stat.st_ino)
        error_exit("Undefined behavior - stdout redirected to source file");
}

int main(int argc, char **argv)
{
    FILE *file;

    //bez argumentu -> stdin
    if (argc < 2)
        file = stdin;
    else //prvni argument = soubor ke cteni
    {
        if (argc > 2)
            warning("Given %d arguments, expected 1 argument", argc - 1);

        file = fopen(argv[1], "r");
        
        if (!file)
            error_exit("Could not open %s", argv[1]);

        assert_different_from_stdout(file);
    }

    int ch;
    State state = CODE;
    bool escaped = false;
    Special sp_eff = NOTHING;
    while ((ch = fgetc(file)) != EOF)
    {
        switch(state)
        {
            case CODE:
                if (sp_eff == SLASH_READ)
                {
                    if (ch == '/') //case "//" -> komentar
                    {
                        state = SLASH_COMM;
                        sp_eff = NOTHING;
                        continue;
                    }
                    else if (ch == '*') //case "/*" -> komentar
                    {
                        state = STAR_COMM;
                        sp_eff = NOTHING;
                        continue;
                    }
                    else //case "/x" -> nejedna se o komentar
                        sp_eff = MISSING_SLASH;
                }
                if (ch == '/')
                {
                    sp_eff = SLASH_READ;
                    continue;
                }
                else if (ch == '"')
                    state = STR_LIT;
                else if (ch == '\'')
                    state = CHAR_LIT;
                break;
            
            case STR_LIT:
                if (!escaped && ch == '"')
                    state = CODE;
                break;

            case CHAR_LIT:
                if (!escaped && ch == '\'')
                    state = CODE;
                break;

            case SLASH_COMM:
                if (!escaped && ch == '\n') //nonescaped '\n' -> konec komentare
                {
                    state = CODE;
                    break;
                }
                else goto ESCAPE_CHECK; //nunost checkovat '\'

            case STAR_COMM:
                if (sp_eff == STAR_READ && ch == '/') //case "*/" -> konec komentare
                {
                    state = CODE;
                    sp_eff = NOTHING;
                }
                else if (ch == '*')
                    sp_eff = STAR_READ;
                continue;
                    
        }

        //pripadne doplneni zamlceneho charakteru '/'
        if (sp_eff == MISSING_SLASH)
        {
            putchar('/');
            sp_eff = NOTHING;
        }

        //jestlize se kod dostal sem, napsat ch (neni komentar)
        putchar(ch);

    ESCAPE_CHECK:
        //check, jestli ma byt nasledujici char escaped
        if (!escaped && ch == '\\')
            escaped = true;
        else 
            escaped = false;
    }

    return 0;
}
