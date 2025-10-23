#ifndef TYPES_H
#define TYPES_H

typedef struct
{
    int from;
    int to;
} t_link;

typedef struct
{
    t_link *links;   // dynamically allocated array of t_link
    int log_size;    // number of valid links
    int capacity;    // optional: total allocated size
} t_link_array;

#endif
