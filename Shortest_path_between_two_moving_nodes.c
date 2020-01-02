/**
 * @file        : prob1.c
 * @brief       : This file contains the program to navigate the
 *                two Pragyan rovers to meet each other in common
 *                place with possible shorest path
 * @author      : Sathishkumar Padmanabhan (sathishkumar.p@vvdntech.in)
 */


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_STR   100
#define P_ASCII         80
#define D_ASCII         82
#define O_ASCII         79
#define SPACE_ASCII     32
#define SUCCESS         0
#define FAILURE         1

#define BIT(a)          (1 << a)
typedef enum status {
    DEST_PRES = BIT(0),
    PRAG_PRES = BIT(1),
} status_t;

typedef struct {
    uint8_t row;
    uint8_t col;
} location_t;

typedef struct direction {
    location_t current;
    uint8_t is_mid;
    struct direction *next_l;
    struct direction *next_r;
    struct direction *next_t;
    struct direction *next_b;
    struct direction *prev;
    struct direction *nxt_seq;
} direction_t;

uint8_t **terr;
uint8_t *p_is_traced;
uint8_t *d_is_traced;

direction_t *p_head_dir;
direction_t *d_head_dir;

void reset_data(int size)
{
    direction_t *node;
    int idx;

    if (p_is_traced != NULL) {
        free(p_is_traced);
    }

    if (d_is_traced != NULL) {
        free(d_is_traced);
    }

    node = p_head_dir;
    while (node != NULL) {
        p_head_dir = node->nxt_seq;
        free(node);
        node = p_head_dir;
    }

    node = d_head_dir;
    while (node != NULL) {
        d_head_dir = node->nxt_seq;
        free(node);
        node = d_head_dir;
    }

    if (terr != NULL) {
        for (idx = 0; idx < size; idx++) {
            if (terr[idx] != NULL) {
                free(terr[idx]);
            }
        }
        free(terr);
    }
}

int alloc_mem_for_terrain(uint8_t size)
{
    uint8_t idx = 0;

    terr = (uint8_t **)malloc(sizeof(uint8_t *) * size);
    if (NULL == terr) {
        printf("Failed to allocate memory\n");
        return FAILURE;
    }

    for (idx = 0; idx < size; idx++) {
        terr[idx] = (uint8_t *)malloc(sizeof(uint8_t) * size);
        if (NULL == terr[idx]) {
            printf("Failed to allocate memory\n");
            return FAILURE;
        }
    }

    p_is_traced = (uint8_t *)malloc(sizeof(uint8_t) * size * size);
    if (p_is_traced == NULL) {
        printf("Failed to allocate memory\n");
        return FAILURE;
    }

    d_is_traced = (uint8_t *)malloc(sizeof(uint8_t) * size * size);
    if (d_is_traced == NULL) {
        printf("Failed to allocate memory\n");
        return FAILURE;
    }
    return SUCCESS;
}

static inline int validate_input(char input, uint8_t *flags)
{
    int ret = FAILURE;
    if ((input == P_ASCII) || (input == D_ASCII)
            || (input == O_ASCII) || (input == SPACE_ASCII)) {
        ret = SUCCESS;
    }

    if (input == D_ASCII) {
        if (*flags & DEST_PRES) {
            ret = FAILURE;
        } else {
            *flags |= DEST_PRES;
        }
    } else if (input == P_ASCII) {
        if (*flags & PRAG_PRES) {
            ret = FAILURE;
        } else {
            *flags |= PRAG_PRES;
        }
    }
    return ret;
}

direction_t *get_direction_ptr(uint8_t row, uint8_t col, uint8_t matrx_sz, uint8_t val_to_update)
{
    int idx = 0;
    direction_t *curr;
    uint8_t is_mid = 0;

    idx = ((matrx_sz * row) + col);
    if (terr[row][col] == O_ASCII) {
        return NULL;
    }

    if (val_to_update == P_ASCII) {
        if (p_is_traced[idx] == 1) {
            return NULL;
        }
        p_is_traced[idx] = 1;
        if (terr[row][col] == D_ASCII) {
            is_mid = 1;
        }
    } else {
        if (d_is_traced[idx] == 1) {
            return NULL;
        }
        d_is_traced[idx] = 1;
        if (terr[row][col] == P_ASCII) {
            is_mid = 1;
        }
    }

    curr = (direction_t *)malloc(sizeof(direction_t));
    if (curr == NULL) {
        printf("Failed to allocate memory for direction\n");
        return NULL;
    }

    memset(curr, 0, sizeof(direction_t));
    curr->current.row = row;
    curr->current.col = col;
    curr->is_mid = is_mid;
    terr[row][col] = val_to_update;

    return curr;
}

direction_t *check_bottom(uint8_t row, uint8_t col, uint8_t matrx_sz, uint8_t val_to_update)
{
    if (row == (matrx_sz - 1)) {
        return NULL;
    }

    row = row + 1;
    return get_direction_ptr(row, col, matrx_sz, val_to_update);
}

direction_t *check_top(uint8_t row, uint8_t col, uint8_t matrx_sz, uint8_t val_to_update)
{
    if (row == 0) {
        return NULL;
    }

    row = row - 1;
    return get_direction_ptr(row, col, matrx_sz, val_to_update);
}

direction_t *check_right(uint8_t row, uint8_t col, uint8_t matrx_sz, uint8_t val_to_update)
{
    if (col == (matrx_sz - 1)) {
        return NULL;
    }

    col = col + 1;
    return get_direction_ptr(row, col, matrx_sz, val_to_update);
}

direction_t *check_left(uint8_t row, uint8_t col, uint8_t matrx_sz, uint8_t val_to_update)
{
    if (col == 0) {
        return NULL;
    }

    col = col - 1;
    return get_direction_ptr(row, col, matrx_sz, val_to_update);
}

int main()
{
    char row_input[MAX_INPUT_STR] = {0};
    uint8_t matrx_sz = 0, new_matrx = 0, row_cnt = 0;
    uint8_t flags = 0, is_first_ref = 1;
    uint8_t row = 0, col = 0;
    location_t src = {0}, dest = {0};
    direction_t *node, *p_curr_seq;
    direction_t *p_curr_ref, *nxt_ref = NULL;
    direction_t *d_curr_ref, *d_curr_seq = NULL;
    location_t *path;
    location_t mid_path = {0};

    printf("\nEnter the terrain Map. Given below input details\n");
    printf("P: Pragyan Rover-1\nR: Pragyan Rover-2\nO: Obstacles\n");
    printf("Space: Free path\nPress enter key twice to end you input\n");

    /* Fetch input from the user */
    while (1) {
        scanf("%[^\n]%*c", row_input);
        if (row_input[0] == '\0') {
            goto err;
        }

        if (!matrx_sz) {
            matrx_sz = strlen(row_input);
            if (alloc_mem_for_terrain(matrx_sz)) {
                printf("Terminating the execution since memory allocation failed\n");
                goto err;
            }
        }

        new_matrx = strlen(row_input);
        if (new_matrx != matrx_sz) {
            printf("Terminating the execution. Invalid terrain size\n");
            goto err;
        }

        for (col = 0; col < matrx_sz; col++) {
            if (validate_input(row_input[col], &flags)) {
                printf("Terminating the execution. Invalid input\n");
                goto err;
            }
            terr[row_cnt][col] = row_input[col];
            if (row_input[col] == D_ASCII) {
                dest.row = row_cnt;
                dest.col = col;
            } else if (row_input[col] == P_ASCII) {
                src.row = row_cnt;
                src.col = col;
                row = ((matrx_sz * row_cnt) + col);
                p_is_traced[row] = 1;
            }
        }
        row_cnt++;

        if (row_cnt >= matrx_sz) {
            break;
        }
        memset(row_input, 0, sizeof(char) * MAX_INPUT_STR);
    }

    if (!((flags & DEST_PRES) && (flags & PRAG_PRES))) {
        printf("Pragyan Rover or its destination not found in map %d\n",flags);
        goto err;
    }

    printf("********************\tTERRAIN MAP\t********************\n");
    for (row = 0; row < matrx_sz; row++) {
        printf("|");
        for (col = 0; col < matrx_sz; col++) {
            printf("%c\t|",terr[row][col]);
        }
        printf("\n");
    }

    /* Two kind of list has been. One is in tree format(next_l,
     * next_r, next_t, next_b, prev) and another one is single
     * linked list(nxt_seq). Tree format list is used for path
     * selection and single linked list is used in path discovery     
     * and freeing nodes.
     */

    /* Allocate head node */
    p_head_dir = (direction_t *)malloc(sizeof(direction_t));
    if (p_head_dir == NULL) {
        printf("Failed to allocate memory\n");
        goto err;
    }
    memset(p_head_dir, 0, sizeof(direction_t));
    p_head_dir->current.row = src.row;
    p_head_dir->current.col = src.col;
    p_curr_ref = p_head_dir;
    p_curr_seq = p_head_dir;

    d_head_dir = (direction_t *)malloc(sizeof(direction_t));
    if (d_head_dir == NULL) {
        printf("Failed to allocate memory\n");
        goto err;
    }
    memset(d_head_dir, 0, sizeof(direction_t));
    d_head_dir->current.row = dest.row;
    d_head_dir->current.col = dest.col;
    d_curr_ref = d_head_dir;
    d_curr_seq = d_head_dir;
    while (1) {
        do {
            node = check_left(p_curr_ref->current.row, p_curr_ref->current.col, matrx_sz, P_ASCII);
            if (node != NULL) {
                p_curr_ref->next_l = node;
                node->prev = p_curr_ref;
                if (is_first_ref) {
                    is_first_ref = 0;
                    nxt_ref = node;
                }
                p_curr_seq->nxt_seq = node;
                p_curr_seq = node;
                if (node->is_mid) {
                    mid_path.row = node->current.row;
                    mid_path.col = node->current.col;
                    goto path_found;
                }
            }

            node = check_right(p_curr_ref->current.row, p_curr_ref->current.col, matrx_sz, P_ASCII);
            if (node != NULL) {
                p_curr_ref->next_r = node;
                node->prev = p_curr_ref;
                if (is_first_ref) {
                    is_first_ref = 0;
                    nxt_ref = node;
                }
                p_curr_seq->nxt_seq = node;
                p_curr_seq = node;
                if (node->is_mid) {
                    mid_path.row = node->current.row;
                    mid_path.col = node->current.col;
                    goto path_found;
                }
            }

            node = check_top(p_curr_ref->current.row, p_curr_ref->current.col, matrx_sz, P_ASCII);
            if (node != NULL) {
                p_curr_ref->next_t = node;
                node->prev = p_curr_ref;
                if (is_first_ref) {
                    is_first_ref = 0;
                    nxt_ref = node;
                }
                p_curr_seq->nxt_seq = node;
                p_curr_seq = node;
                if (node->is_mid) {
                    mid_path.row = node->current.row;
                    mid_path.col = node->current.col;
                    goto path_found;
                }
            }

            node = check_bottom(p_curr_ref->current.row, p_curr_ref->current.col, matrx_sz, P_ASCII);
            if (node != NULL) {
                p_curr_ref->next_b = node;
                node->prev = p_curr_ref;
                if (is_first_ref) {
                    is_first_ref = 0;
                    nxt_ref = node;
                }
                p_curr_seq->nxt_seq = node;
                p_curr_seq = node;
                if (node->is_mid) {
                    mid_path.row = node->current.row;
                    mid_path.col = node->current.col;
                    goto path_found;
                }
            }

            p_curr_ref = p_curr_ref->nxt_seq;
        } while((p_curr_ref != nxt_ref) && (p_curr_ref != NULL));

        if (p_curr_ref == NULL) {
            goto path_not_found;
        }

        is_first_ref = 1;
        nxt_ref = NULL;

        do {
            node = check_left(d_curr_ref->current.row, d_curr_ref->current.col, matrx_sz, D_ASCII);
            if (node != NULL) {
                d_curr_ref->next_l = node;
                node->prev = d_curr_ref;
                if (is_first_ref) {
                    is_first_ref = 0;
                    nxt_ref = node;
                }
                d_curr_seq->nxt_seq = node;
                d_curr_seq = node;
                if (node->is_mid) {
                    mid_path.row = node->current.row;
                    mid_path.col = node->current.col;
                    goto path_found;
                }
            }

            node = check_right(d_curr_ref->current.row, d_curr_ref->current.col, matrx_sz, D_ASCII);
            if (node != NULL) {
                d_curr_ref->next_r = node;
                node->prev = d_curr_ref;
                if (is_first_ref) {
                    is_first_ref = 0;
                    nxt_ref = node;
                }
                d_curr_seq->nxt_seq = node;
                d_curr_seq = node;
                if (node->is_mid) {
                    mid_path.row = node->current.row;
                    mid_path.col = node->current.col;
                    goto path_found;
                }
            }

            node = check_top(d_curr_ref->current.row, d_curr_ref->current.col, matrx_sz, D_ASCII);
            if (node != NULL) {
                d_curr_ref->next_t = node;
                node->prev = d_curr_ref;
                if (is_first_ref) {
                    is_first_ref = 0;
                    nxt_ref = node;
                }
                d_curr_seq->nxt_seq = node;
                d_curr_seq = node;
                if (node->is_mid) {
                    mid_path.row = node->current.row;
                    mid_path.col = node->current.col;
                    goto path_found;
                }
            }

            node = check_bottom(d_curr_ref->current.row, d_curr_ref->current.col, matrx_sz, D_ASCII);
            if (node != NULL) {
                d_curr_ref->next_b = node;
                node->prev = d_curr_ref;
                if (is_first_ref) {
                    is_first_ref = 0;
                    nxt_ref = node;
                }
                d_curr_seq->nxt_seq = node;
                d_curr_seq = node;
                if (node->is_mid) {
                    mid_path.row = node->current.row;
                    mid_path.col = node->current.col;
                    goto path_found;
                }
            }

            d_curr_ref = d_curr_ref->nxt_seq;
        } while((d_curr_ref != nxt_ref) && (d_curr_ref != NULL));

        if (d_curr_ref == NULL) {
            goto path_not_found;
        }

        is_first_ref = 1;
        nxt_ref = NULL;
    }
path_found:
    printf("Path found\n");
    printf("P\n");
    path = (location_t *)malloc(sizeof(location_t) * matrx_sz * matrx_sz);
    if (path == NULL) {
        printf("Failed to allocate memory\n");
        goto err;
    }
    node = p_head_dir;
    col = 1;
    
nxt_path_selection:
    memset(path, 0, sizeof(location_t) * matrx_sz * matrx_sz);
    while (node != NULL) {
        if ((node->current.row == mid_path.row) &&
                (node->current.col == mid_path.col))
        {
            break;
        }
        node = node->nxt_seq;
    }

    if (node == NULL) {
        printf("Could not trace the path\n");
        goto err;
    }

    row = 0;
    while (node != NULL) {
        path[row].row = node->current.row;
        path[row].col = node->current.col;
        row++;
        node = node->prev;
    }

    do {
        row--;
        printf("(%d,%d)\n",path[row].row, path[row].col);
    } while(row);

    if (col) {
        col = 0;
        node = d_head_dir;
        printf("R\n");
        goto nxt_path_selection;
    }

    free(path);
    reset_data(matrx_sz);
    return SUCCESS;

path_not_found:
    printf("Could not find the path\n");

err:
    reset_data(matrx_sz);
    return FAILURE;
}
