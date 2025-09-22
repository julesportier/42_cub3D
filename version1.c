#include <stdio.h>
#include <ctype.h>

typedef struct {
    char *no, *so, *we, *ea; // NULL si non vus
    int  floor_rgb;          // -1 si non vu
    int  ceil_rgb;           // -1 si non vu
    // map (si tu veux stocker plus tard)
    char **map_lines; size_t map_h, map_cap;
} Config;

static void cfg_init(Config *c){
    memset(c, 0, sizeof(*c));
    c->floor_rgb = -1;
    c->ceil_rgb  = -1;
}
static void cfg_free(Config *c){
    free(c->no); free(c->so); free(c->we); free(c->ea);
    for (size_t i=0;i<c->map_h;i++) free(c->map_lines[i]);
    free(c->map_lines);
}

static char *skip_ws(char *p){ while (*p==' '||*p=='\t') p++; return p; }
static char *rtrim(char *p)
{
    size_t n = strlen(p);
    while (n && (p[n-1]==' '||p[n-1]=='\t'||p[n-1]=='\r'||p[n-1]=='\n')) p[--n]='\0';
    return p;
}

/* Duplique un chemin en autorisant les espaces et les guillemets.
   - Si le chemin commence par " ou ', on prend le contenu jusqu'au guillemet fermant.
   - Sinon, on prend le suffixe entier trimé. */
static char *dup_path_allow_spaces(char *p) {
    p = skip_ws(p);
    rtrim(p);
    if (*p == '"' || *p == '\'') {
        char quote = *p++;
        char *q = p;
        while (*q && *q != quote) q++;
        size_t len = (size_t)(q - p);
        char *out = malloc(len + 1);
        if (!out) return NULL;
        memcpy(out, p, len);
        out[len] = '\0';
        return out;
    } else {
        return strdup(p);
    }
}

/* parse entier 0..255 (strict) */
static int parse_u8_strict(const char *s, int *out) {
    char *end; long v = strtol(s, &end, 10);
    if (end == s || *end != '\0' || v < 0 || v > 255) return -1;
    *out = (int)v; return 0;
}

/* Retour: 1 = entête traitée, 0 = pas entête, -1 = erreur */
int parse_header_line_option1(const char *line_in, Config *cfg, const char **err) {
    // on fait une copie modifiable pour trim/scan
    char *line = strdup(line_in);
    if (!line) { *err="OOM"; return -1; }

    char *p = skip_ws(rtrim(line));
    if (!*p) { free(line); return 0; } // vide

    // -------- Textures NO/SO/WE/EA (chemin peut contenir des espaces) --------
    if (!strncmp(p,"NO",2) && isspace((unsigned char)p[2])) {
        if (cfg->no) { *err="Doublon NO"; free(line); return -1; }
        cfg->no = dup_path_allow_spaces(p+2);
        if (!cfg->no || !*cfg->no) { *err="NO sans chemin"; free(cfg->no); cfg->no=NULL; free(line); return -1; }
        free(line); return 1;
    }
    if (!strncmp(p,"SO",2) && isspace((unsigned char)p[2])) {
        if (cfg->so) { *err="Doublon SO"; free(line); return -1; }
        cfg->so = dup_path_allow_spaces(p+2);
        if (!cfg->so || !*cfg->so) { *err="SO sans chemin"; free(cfg->so); cfg->so=NULL; free(line); return -1; }
        free(line); return 1;
    }
    if (!strncmp(p,"WE",2) && isspace((unsigned char)p[2])) {
        if (cfg->we) { *err="Doublon WE"; free(line); return -1; }
        cfg->we = dup_path_allow_spaces(p+2);
        if (!cfg->we || !*cfg->we) { *err="WE sans chemin"; free(cfg->we); cfg->we=NULL; free(line); return -1; }
        free(line); return 1;
    }
    if (!strncmp(p,"EA",2) && isspace((unsigned char)p[2])) {
        if (cfg->ea) { *err="Doublon EA"; free(line); return -1; }
        cfg->ea = dup_path_allow_spaces(p+2);
        if (!cfg->ea || !*cfg->ea) { *err="EA sans chemin"; free(cfg->ea); cfg->ea=NULL; free(line); return -1; }
        free(line); return 1;
    }

    // -------- Couleurs F/C (split " \t," pour supprimer espaces/virgules/tabs) --------
    if ((p[0]=='F' || p[0]=='C') && isspace((unsigned char)p[1])) {
        char **tok = ft_split_multi(p, " \t,"); // ex: "F 220,100,0" -> ["F","220","100","0"]
        if (!tok || !tok[0]) { free(tok); free(line); return 0; }
        if (!tok[1] || !tok[2] || !tok[3]) { *err="Couleur incomplète"; free_tokens(tok); free(line); return -1; }
        int r,g,b;
        if (parse_u8_strict(tok[1],&r) || parse_u8_strict(tok[2],&g) || parse_u8_strict(tok[3],&b)) {
            *err="RGB invalide (0..255)"; free_tokens(tok); free(line); return -1;
        }
        int packed = (r<<16)|(g<<8)|b;
        if (p[0]=='F') {
            if (cfg->floor_rgb != -1) { *err="Doublon F"; free_tokens(tok); free(line); return -1; }
            cfg->floor_rgb = packed;
        } else {
            if (cfg->ceil_rgb != -1) { *err="Doublon C"; free_tokens(tok); free(line); return -1; }
            cfg->ceil_rgb = packed;
        }
        free_tokens(tok); free(line); return 1;
    }

    // Pas une ligne d’en-tête connue → probablement map/vides/commentaires
    free(line);
    return 0;
}

/* Vérifs finales après l’en-tête */
int header_complete(const Config *cfg, const char **err){
    if (!cfg->no || !cfg->so || !cfg->we || !cfg->ea) { *err="Texture manquante"; return -1; }
    if (cfg->floor_rgb == -1 || cfg->ceil_rgb == -1) { *err="Couleur manquante"; return -1; }
    return 0;
}

static void push_map_line(Config *c, const char *line) {
    if (c->map_h + 1 > c->map_cap) {
        size_t newcap = c->map_cap ? c->map_cap*2 : 16;
        char **tmp = realloc(c->map_lines, newcap * sizeof *tmp);
        if (!tmp) { perror("realloc"); exit(1); }
        c->map_lines = tmp; c->map_cap = newcap;
    }
    c->map_lines[c->map_h++] = strdup(line); // conserve espaces/tabs exactement
}

int main(void) {
    const char *err = NULL;
    Config cfg; cfg_init(&cfg);

    const char *headers[] = {
        "NO   ./textures/brick red.xpm",
        "SO\t\"./My Textures/south wall.xpm\"",
        "WE ./textures/west.xpm",
        "EA ./textures/east.xpm",
        "F  220,100,0",
        "C\t0,0,0",
        NULL
    };

    for (int i=0; headers[i]; i++) {
        int r = parse_header_line_option1(headers[i], &cfg, &err);
        if (r == -1) { fprintf(stderr, "Erreur: %s\n", err); cfg_free(&cfg); return 1; }
        // r==1: ok, r==0: pas header (ignorer ici)
    }
    if (header_complete(&cfg, &err)) {
        fprintf(stderr, "Erreur: %s\n", err);
        cfg_free(&cfg); return 1;
    }

    printf("NO=%s\nSO=%s\nWE=%s\nEA=%s\nF=%06X\nC=%06X\n",
        cfg.no, cfg.so, cfg.we, cfg.ea, cfg.floor_rgb, cfg.ceil_rgb);

    // ensuite, en lisant le fichier : dès que tu détectes la première ligne de map,
    // appelle push_map_line(&cfg, ligne_brute);

    cfg_free(&cfg);
    return 0;
}
