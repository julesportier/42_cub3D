#include <stdio.h>
#include <stdlib.h>

// déclare ft_split_multi, free_tokens, dump_bytes ici
// (ou #include "parsing.h" si propre)

char **ft_split_multi(const char *s, const char *delims);
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
    const char *delims = " \t\r"; // ESSAIE AUSSI " \t\r\xC2\xA0\xC2\xBB" si tu avais NBSP + »

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
