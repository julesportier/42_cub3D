#include "parsing.h"

int main(int ac, char **av)
{
    t_parsed parsed;
    t_perr   err;

    if (ac != 2) {
        print_perr(PERR_ARGC, NULL);
        return (EXIT_FAILURE);
    }

    err = parsing_load(av[1], &parsed);
    if (err != PERR_OK) {
        print_perr(err, av[1]);
        return (EXIT_FAILURE);
    }

    // ---- Affichage minimal de la struct combinée ----
    printf("== CONFIG ==\n");
    printf(" NO: %s\n", parsed.config.no ? parsed.config.no : "(null)");
    printf(" SO: %s\n", parsed.config.so ? parsed.config.so : "(null)");
    printf(" WE: %s\n", parsed.config.we ? parsed.config.we : "(null)");
    printf(" EA: %s\n", parsed.config.ea ? parsed.config.ea : "(null)");
    printf(" F : %s (%d,%d,%d)\n",
           parsed.config.floor_rgb.is_set ? "set" : "unset",
           parsed.config.floor_rgb.r, parsed.config.floor_rgb.g, parsed.config.floor_rgb.b);
    printf(" C : %s (%d,%d,%d)\n",
           parsed.config.ceil_rgb.is_set ? "set" : "unset",
           parsed.config.ceil_rgb.r, parsed.config.ceil_rgb.g, parsed.config.ceil_rgb.b);

    printf("\n== MAP ==\n");
    printf(" rows=%d cols=%d | player=(r=%d,c=%d,dir=%c)\n",
           parsed.map.rows, parsed.map.columns,
           parsed.map.player.row, parsed.map.player.column, parsed.map.player.dir);

    // (Optionnel) affichage de la grille paddée — pratique pour vérifier visuellement
    for (int r = 0; r < parsed.map.rows; ++r) {
        for (int c = 0; c < parsed.map.columns; ++c) {
            char ch = parsed.map.grid[r][c];
            putchar(ch == ' ' ? '.' : ch); // on remplace ' ' par '.' pour voir le padding
        }
        putchar('\n');
    }

    // Libération
    parsing_free(&parsed);
    return (EXIT_SUCCESS);
}
