#ifndef LEMIN_H
#define LEMIN_H

#include "../libft/libft.h"
#include <stdio.h>
#include <stdint.h>

typedef struct s_room
{
    char *name;
    int x;
    int y;

    struct s_room **links;
} t_room;

typedef struct s_lemin
{

} t_lemin;

#endif
