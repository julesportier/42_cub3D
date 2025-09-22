#include <stdio.h>
#include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <errno.h>
# include <stdio.h>
# include <stdbool.h>
# include <ctype.h>
#include <string.h>

char **free_splits(char **arr, size_t pos)
{
    if (!arr) return NULL;
    for (size_t j = 0; j < pos; j++) free(arr[j]);
    free(arr);
    return NULL;
}

static bool is_delim(char c, const char *str)
{
    size_t i = 0;
    if (!str || !*str) return false;
    if (c == '\0')      return false;
    while (str[i])
    {
        if (c == str[i]) return true;
        i++;
    }
    return false;
}

static size_t count_splits(const char *str, const char *delim)
{
    size_t i = 0, count = 0;
    bool in = false;

    if (!str || !*str) return 0;
    while (str[i])
    {
        if (!in && !is_delim(str[i], delim)) { in = true; count++; }
        else if (in && is_delim(str[i], delim)) { in = false; }
        i++;
    }
    return count;
}

static size_t get_split_len(const char *str, const char *delim, size_t start)
{
    size_t i = 0;
    while (str[start + i] && !is_delim(str[start + i], delim)) i++;
    return i;
}

static char *make_split(const char *str, size_t start, size_t len)
{
    char *out = malloc(len + 1);
    if (!out) return NULL;
    if (len) memcpy(out, str + start, len);   // safe si len == 0
    out[len] = '\0';
    return out;
}

char **ft_split_multi(const char *str, const char *delims)
{
    if (!str || !delims || !*delims) return NULL;

    size_t nb_tokens = count_splits(str, delims);
    char **arr = malloc((nb_tokens + 1) * sizeof *arr);
    if (!arr) return NULL;

    size_t i = 0, pos = 0;
    while (pos < nb_tokens)
    {
        while (str[i] && is_delim(str[i], delims)) i++;

        size_t len = get_split_len(str, delims, i);
        char *word = make_split(str, i, len);
        if (!word) return free_splits(arr, pos);

        arr[pos++] = word;
        i += len;

        while (str[i] && is_delim(str[i], delims)) i++;
    }
    arr[pos] = NULL;
    return arr;
}


void free_tokens(char **tok) {
    if (!tok) return;
    for (size_t i=0; tok[i]; i++) free(tok[i]);
    free(tok);
}
static void dump_bytes(const char *tag, const char *s) {
    fprintf(stderr, "DBG %s: ", tag);
    if (!s) { fprintf(stderr, "(null)\n"); return; }
    fprintf(stderr, "\"");
    for (const unsigned char *p=(const unsigned char*)s; *p; p++)
        fputc((*p>=32 && *p<=126)?*p:'.', stderr);
    fprintf(stderr, "\"  | bytes:");
    for (const unsigned char *p=(const unsigned char*)s; *p; p++)
        fprintf(stderr," %02X", *p);
    fprintf(stderr,"\n");
}

int main(void) {
    const char *lines[] = {
        "EA",
        "EA     ",
        "   EA",
        "EA ./path",
        "EA\t./path",
        "C",
        "C 14 , 14,14",
        "C14,14,14",
        NULL
    };
    const char *delims = " \t\r";

    for (int i=0; lines[i]; i++) {
        fprintf(stderr, "\n=== case %d ===\n", i);
        dump_bytes("line", lines[i]);
        dump_bytes("delims", delims);
        char **tok = ft_split_multi(lines[i], delims);
        if (!tok) { fprintf(stderr, "split returned NULL\n"); continue; }
        for (int j=0; tok[j]; j++) {
            dump_bytes("tok", tok[j]);
        }
        free_tokens(tok);
    }
    return 0;
}
