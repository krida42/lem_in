#ifndef VISU_H
#define VISU_H

// Forward declarations pour éviter les conflits d'includes
typedef struct s_lemin t_lemin;

// Configuration de la visualisation
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
#define MAX_FPS 60

// Fonction principale pour lancer la visualisation
int run_visualization(t_lemin* lemin);

#endif
