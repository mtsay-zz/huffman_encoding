#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>

typedef struct header {
  int freq[256];
  int size;
} header_t;

typedef struct node {
  struct node *left;
  struct node *right;
  int value;
} node_t;

typedef struct pair {
  int priority;
  node_t *value;
} pair_t;

typedef struct priority_queue {
  pair *items;
  int size;
  int capacity;
} priority_queue_t;

typedef struct bit_vector {
  bool *bits;
  int size;
  int capacity;
} bits_t;

typedef struct bit_input_stream {
  FILE *base;
  int buf;     // bits to be read in the buffer
  int num_bits; // number of bits to be read in the buffer
} bis_t;

typedef struct bit_output_stream {
  FILE *base;
  int buf;     // bits to be written in the buffer
  int num_bits; // number of bits in the buffer
} bos_t;

node_t *node_create(int value);
node_t *node_create(int value, node_t *left, node_t *right);
void free_tree(node_t *tree);

priority_queue_t *pq_create();
void pq_free(priority_queue_t *queue);
void pq_enqueue(priority_queue_t *queue, int priority, node_t *value);
pair_t pq_dequeue(priority_queue_t *queue);

bits_t *bits_create();
bits_t *bits_clone(bits_t *base);
void bits_free(bits_t *bits);
void bits_append(bits_t *bits, bool bit);
void bits_remove_last(bits_t *bits);

bis_t *bis_create(FILE *base);
int bis_read(bis_t *bis);

bos_t *bos_create(FILE *base);
void bos_write(bos_t *bos, bool *bits, int num);
void bos_flush(bos_t *bos);

node_t *build_encode_tree(int *freq);

#endif
