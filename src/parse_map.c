/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vakozhev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 14:08:16 by vakozhev          #+#    #+#             */
/*   Updated: 2025/09/24 14:09:32 by vakozhev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct s_map {
    int rows, cols;
    char *grid;          // rows * cols, row-major
    int spawn_r, spawn_c;
    char spawn_dir;      // 'N','S','E','W' (angle calculé ailleurs)
} t_map;

typedef struct s_mapbuild {
    char *buf;     size_t len, cap;     // gros buffer concaténé (+ '\n')
    int  *offs;    int rows, rows_cap;  // offset début de ligne
    int  *lens;                         // longueur de ligne (sans '\n')
    int maxw;                          // largeur max
    int spawn_r, spawn_c; char spawn_dir; int spawn_count;
    bool started, ended;
} t_mapbuild;
// utils_chars.c
#include "cub3d_map.h"

/* is_map_char: vrai si c est autorisé dans la carte.
   Tabs non autorisés ici (on les convertit avant). */
static inline bool is_map_char(int c) {
    return (c=='0'||c=='1'||c==' '||c=='N'||c=='S'||c=='E'||c=='W');
}

/* tabs_to_spaces: remplace in-place \t par ' ' (pour éviter l’ambiguïté). */
void tabs_to_spaces(char *s) {
    if (!s) return;
    for (; *s; ++s) if (*s=='\t') *s=' ';
}

/* line_len_no_nl: renvoie la longueur sans '\n' et '\r' finaux. */
int line_len_no_nl(const char *s) {
    int n=0; while (s[n]) n++;
    while (n>0 && (s[n-1]=='\n' || s[n-1]=='\r')) n--;
    return n;
}

/* is_map_line: vrai si la ligne (sans \n/\r) ne contient QUE des chars carte. */
bool is_map_line(const char *s) {
    int n = line_len_no_nl(s);
    for (int i=0;i<n;i++) {
        if (s[i]=='\t') return true;     // accepté, sera converti
        if (!is_map_char((unsigned char)s[i])) return false;
    }
    return n>0; // ligne vide ne déclenche pas la map
}
// main_utils_test.c
// But: tester tabs_to_spaces, line_len_no_nl, is_map_line de utils_chars.c
// Usage: ./a.out [fichier]  (sinon lit sur stdin)

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

// Protos des fonctions à tester (ou inclus via ton cub3d_map.h)
int  line_len_no_nl(const char *s);
void tabs_to_spaces(char *s);
bool is_map_line(const char *s);

int main(int argc, char **argv) {
    FILE *f = (argc > 1) ? fopen(argv[1], "r") : stdin;
    if (!f) { perror("open"); return 1; }

    char buf[4096], tmp[4096];
    int line_no = 1;
    while (fgets(buf, sizeof buf, f)) {
        bool before = is_map_line(buf);
        strncpy(tmp, buf, sizeof tmp); tmp[sizeof tmp - 1] = '\0';
        tabs_to_spaces(tmp);
        bool after = is_map_line(tmp);
        int Lraw = line_len_no_nl(buf);
        int L    = line_len_no_nl(tmp);

        printf("L%03d len=%d map(before)=%d map(after)=%d -> \"", line_no++, Lraw, before, after);
        for (int i = 0; i < L; i++) putchar(tmp[i] == ' ' ? '·' : tmp[i]);
        puts("\"");
    }
    if (f != stdin) fclose(f);
    return 0;
}
