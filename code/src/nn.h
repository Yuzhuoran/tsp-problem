/*
 * nn.h
 *
 *  Created on: Dec 3, 2016
 *      Author: Lenovo
 */

#ifndef NN_H_
#define NN_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <sys/time.h>
#include "list.h"



struct city {
    struct dl_list node;

    int id;
    double x;
    double y;
};

struct edge {
    struct dl_list node;

    struct city *from;
    struct city *to;
    int distance;
};

static void *malloc0(size_t sz)
{
    void *p = malloc(sz);
    assert(p != NULL);
    memset(p, 0, sz);

    return p;
}

static int calc_distance(const struct city *from, const struct city *to)
{
    return (int)sqrt(pow(from->x - to->x, 2) + pow(from->y - to->y, 2));
}

static int neighbor_tsp(struct dl_list *city_list, struct dl_list *edge_list)
{
    int total_distance = 0;

    dl_list_init(edge_list);

    struct city *from = dl_list_first(city_list, struct city, node);
    if (NULL == from) {
        return total_distance;
    }

    dl_list_del(&from->node);

    while (!dl_list_empty(city_list)) {
        int distance = -1;
        struct city *to = NULL;
        struct city *city;
        dl_list_for_each(city, city_list, struct city, node) {
            int tmp = calc_distance(from, city);
            if (distance < 0 || tmp < distance) {
                distance = tmp;
                to = city;
            }
        }

        dl_list_del(&to->node);

        struct edge *edge = (struct edge *)malloc0(sizeof(struct edge));
        edge->from = from;
        edge->to = to;
        edge->distance = distance;
        dl_list_add_tail(edge_list, &edge->node);

        from = to;

        total_distance += distance;
    }

    struct edge *first = dl_list_first(edge_list, struct edge, node);
    struct edge *last  = dl_list_last(edge_list, struct edge, node);
    if (first && first != last) {
        struct edge *edge = (struct edge *)malloc0(sizeof(struct edge));
        edge->from = last->to;
        edge->to = first->from;
        edge->distance = calc_distance(edge->from, edge->to);
        dl_list_add_tail(edge_list, &edge->node);

        total_distance += edge->distance;
    }

    return total_distance;
}

static void read_file(const char *filename, struct dl_list *city_list)
{
    FILE *file = fopen(filename, "r");
    /*if (NULL == file) {
        fprintf(stderr, "failed to open file: %s", filename);
        exit(-1);
    }*/

    dl_list_init(city_list);

    int i;
    char line[1024];
    /* skip 5 line */
    for (i = 0; i < 5; i++) {
    	//assert(NULL!= fgets(line, sizeof(line), file));
    	fgets(line, sizeof(line), file);
    }

    while (fgets(line, sizeof(line), file)) {
        if (0 == strncmp(line, "EOF", strlen("EOF"))) {
            break;
        }

        struct city *city = (struct city *)malloc0(sizeof(struct city));
        assert(3 == sscanf(line, "%d %lf %lf", &city->id, &city->x, &city->y));
        dl_list_add_tail(city_list, &city->node);
    }

    fclose(file);

    return ;
}

static void output_path(const char *filename, int total_distance, const struct dl_list *edge_list)
{
    FILE *file = fopen(filename, "w+");
    /*if (NULL == file) {
        fprintf(stderr, "failed to open file: %s", filename);
        exit(-1);
    }
*/
    fprintf(file, "%d\n", total_distance);

    struct edge *edge;
    dl_list_for_each(edge, edge_list, struct edge, node) {
        fprintf(file, "%d %d %d\n", edge->from->id, edge->to->id, edge->distance);
    }

    fclose(file);
}

static void output_time(const char *filename, double total_time, int total_distance)
{
    FILE *file = fopen(filename, "w+");
	/*
    if (NULL == file) {
		fprintf(stderr, "failed to open file: %s", filename);
		exit(-1);
	}
*/
	fprintf(file, "%lf,%d\n", total_time, total_distance);

    fclose(file);
}

static double current_time(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + (double)tv.tv_usec / 1000000;
}

void algorithm_nn(const char *city_file, const char *algo, int cutoff_time)
{
    char input_city[32], output_sol[32], output_trace[32];

    sprintf(input_city, "data//%s.tsp", city_file);
    sprintf(output_sol, "output//%s_%s_%d.sol", city_file, algo, cutoff_time);
    sprintf(output_trace, "output//%s_%s_%d.trace", city_file, algo, cutoff_time);

    struct dl_list city_list;
    struct dl_list edge_list;

    read_file(input_city, &city_list);

    double start_time = current_time();
    int total_distance = neighbor_tsp(&city_list, &edge_list);
    double end_time = current_time();


    output_path(output_sol, total_distance, &edge_list);
	output_time(output_trace, end_time - start_time, total_distance);

    /* TODO: clean */

    return;
}

int nn(char *data,char *algo,int cut ,int s)
{
    /*if (argc != 5) {
        fprintf(stderr, "USAGEG: %s <city> <algo> <cutoff> <seed>\n", argv[0]);
        return -1;
    }
*/
    algorithm_nn(data, algo, cut);

    return 0;
}
#ifdef __cplusplus
}
#endif


#endif /* NN_H_ */
