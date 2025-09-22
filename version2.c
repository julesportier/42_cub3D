#include <ctype.h>
#include <string.h>

typedef enum {
    ID_NONE = 0,   // ligne vide ou seulement espaces
    ID_NO, ID_SO, ID_WE, ID_EA,
    ID_F, ID_C,
    ID_UNKNOWN     // pas un identifiant reconnu (probablement ligne de map)
} Id;

static char *skip_ws(char *p) {
    while (*p == ' ' || *p == '\t') p++;
    return p;
}

/* Détecte l'identifiant au début de la ligne (après espaces).
   - Retourne l'Id (NO/SO/WE/EA/F/C/NONE/UNKNOWN)
   - Écrit dans *after un pointeur sur le caractère juste après l'identifiant.
     (typiquement le 1er espace/tab qui suit, ou directement les données)
*/
static Id get_identifier(char *line, char **after)
{
    char *p = skip_ws(line);
    if (*p == '\0') { if (after) *after = p; return ID_NONE; }

    // 2 lettres : NO / SO / WE / EA  (doivent être suivies d'un espace/tab)
    if (p[0]=='N' && p[1]=='O' && isspace((unsigned char)p[2])) { if (after) *after = p+2; return ID_NO; }
    if (p[0]=='S' && p[1]=='O' && isspace((unsigned char)p[2])) { if (after) *after = p+2; return ID_SO; }
    if (p[0]=='W' && p[1]=='E' && isspace((unsigned char)p[2])) { if (after) *after = p+2; return ID_WE; }
    if (p[0]=='E' && p[1]=='A' && isspace((unsigned char)p[2])) { if (after) *after = p+2; return ID_EA; }

    // 1 lettre : F / C (doivent être suivies d'un espace/tab)
    if (p[0]=='F' && isspace((unsigned char)p[1])) { if (after) *after = p+1; return ID_F; }
    if (p[0]=='C' && isspace((unsigned char)p[1])) { if (after) *after = p+1; return ID_C; }

    // Rien de reconnu : peut être une ligne de map (ou commentaire)
    if (after) *after = p;
    return ID_UNKNOWN;
}

int parse_header_line_option1(const char *line_in, Config *cfg, const char **err)
{
    char *line = strdup(line_in);
    if (!line) { *err = "OOM"; return -1; }

    char *after = NULL;
    Id id = get_identifier(line, &after);

    if (id == ID_NONE) { free(line); return 0; }      // ligne vide
    if (id == ID_UNKNOWN) { free(line); return 0; }   // probablement map/autre

    if (id == ID_NO || id == ID_SO || id == ID_WE || id == ID_EA) {
        char *path = dup_path_allow_spaces(after);
        if (!path || !*path) { free(path); free(line); *err = "Chemin texture manquant"; return -1; }
        if (id == ID_NO) { if (cfg->no) { *err="Doublon NO"; free(path); free(line); return -1; } cfg->no = path; }
        if (id == ID_SO) { if (cfg->so) { *err="Doublon SO"; free(path); free(line); return -1; } cfg->so = path; }
        if (id == ID_WE) { if (cfg->we) { *err="Doublon WE"; free(path); free(line); return -1; } cfg->we = path; }
        if (id == ID_EA) { if (cfg->ea) { *err="Doublon EA"; free(path); free(line); return -1; } cfg->ea = path; }
        free(line); return 1;
    }

    if (id == ID_F || id == ID_C) {
        char **tok = ft_split_multi(after, " \t,"); // " 220,100,0" -> ["220","100","0"]
        if (!tok || !tok[0] || !tok[1] || !tok[2]) { free_tokens(tok); free(line); *err="Couleur incomplète"; return -1; }
        int r,g,b;
        if (parse_u8_strict(tok[0],&r) || parse_u8_strict(tok[1],&g) || parse_u8_strict(tok[2],&b)) {
            *err = "RGB invalide (0..255)"; free_tokens(tok); free(line); return -1;
        }
        int packed = (r<<16)|(g<<8)|b;
        if (id == ID_F) {
            if (cfg->floor_rgb != -1) { *err="Doublon F"; free_tokens(tok); free(line); return -1; }
            cfg->floor_rgb = packed;
        } else {
            if (cfg->ceil_rgb != -1) { *err="Doublon C"; free_tokens(tok); free(line); return -1; }
            cfg->ceil_rgb = packed;
        }
        free_tokens(tok);
        free(line);
        return 1;
    }

    free(line);
    return 0;
}

// Suppose: char *get_next_line(int fd);  // renvoie une ligne allouée (ou NULL)
#include <fcntl.h>
#include <unistd.h>

int parse_headers_from_fd(int fd, Config *cfg, const char **err)
{
    char *line;
    while ((line = get_next_line(fd)) != NULL) {
        int r = parse_header_line_option1(line, cfg, err);
        free(line);
        if (r == -1) return -1;   // erreur
        if (r == 0) {
            // pas une ligne d'en-tête (non vide) → probablement la map
            // Ici, soit tu gardes cette ligne (il faut la stocker AVANT de free),
            // soit tu décides d'ignorer les vides exclusivement.
            break;
        }
        // r == 1 → header consommé, on continue
    }
    if (header_complete(cfg, err) != 0) return -1;
    return 0;
}
