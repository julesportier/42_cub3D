#include "parsing.h"

static bool check_extension(const char *path)
{
    size_t n;

    if (path == NULL)
        return (false);
    n = strlen(path);
    if (n <= 4)                      // refuse ".cub"
        return (false);
    if (path[n - 5] == '/')          // refuse "/.cub", "./.cub", "dir/.cub"
        return (false);
    if (strcmp(path + (n - 4), ".cub") == 0)
        return (true);
    return (false);
}

t_perr validate_params(int argc, char **argv, const char **out_path)
{
    const char *path;
    int         fd;
    char        octet_test;
    ssize_t     r;

    if (argc != 2)
        return (PERR_ARGC);
    path = argv[1];
    if (path == NULL || *path == '\0')
        return (PERR_EMPTY);
    if (!check_extension(path))
        return (PERR_EXT);
    fd = open(path, O_RDONLY);
    if (fd < 0)
        return (PERR_OPEN);
    r = read(fd, &octet_test, 1);
    if (r < 0)
    {
        if (errno == EISDIR)
        {
            close(fd);
            return (PERR_DIR);
        }
        close(fd);
        return (PERR_OPEN);  // autre erreur ??
    }
    close(fd);
    if (out_path)
        *out_path = path;
    return (PERR_OK);
}

void print_perr(t_perr err, const char *str) //attention fprintf n'est pas autorisé !
{
    const char *path_for_msg;

    path_for_msg = str;
    if (path_for_msg == NULL)
        path_for_msg = "";
    if (err == PERR_OK)
		return;
    if (err == PERR_ARGC)
    {
        fprintf(stderr, "Usage: ./cub3D <map.cub>\n");
        return;
    }
    if (err == PERR_EMPTY)
    {
        fprintf(stderr, "Error: chemin de fichier vide.\n");
        return;
    }
    if (err == PERR_EXT)
    {
        fprintf(stderr, "Error: extension invalide pour '%s' (attendu: .cub, sensible à la casse).\n", path_for_msg);
        return;
    }
    if (err == PERR_DIR)
    {
        fprintf(stderr, "Error: '%s' est un dossier.\n", path_for_msg);
        return;
    }
    if (err == PERR_OPEN)
    {
        fprintf(stderr, "Error: impossible d'ouvrir '%s' (%s).\n", path_for_msg, strerror(errno));
        return;
    }
    fprintf(stderr, "Error: échec inconnu.\n");
}

/* 
int main(int argc, char **argv)
{
    const char *path = NULL;
    t_perr err = validate_params(argc, argv, &path);

    if (err != PERR_OK) {
        print_perr(err, (argc > 1 ? argv[1] : NULL));
        return 1;
    }
    write(1, "params OK: ", 15);
    write(1, path, strlen(path));
    write(1, "\n", 1);
    return 0;
}*/


