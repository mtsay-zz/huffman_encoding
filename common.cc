#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

#define QUEUE_CAPACITY_HACK 260
#define BIT_VECTOR_CAPACITY_HACK 260

node_t *node_create(int value) {
  return node_create(value, 0, 0);
}

node_t *node_create(int value, node_t *left, node_t *right) {
  node_t *node = (node_t *)malloc(sizeof(node_t));
  node->value = value;
  node->left = left;
  node->right = right;
  return node;
}

void free_tree(node_t *tree) {
  if (tree == 0) { return; }
  free_tree(tree->left);
  free_tree(tree->right);
  free(tree);
}

priority_queue_t *pq_create() {
  priority_queue_t *queue = (priority_queue_t *)malloc(sizeof(priority_queue_t));
  queue->size = 0;
  queue->capacity = QUEUE_CAPACITY_HACK;
  queue->items = (pair_t *)calloc(queue->capacity, sizeof(pair_t));
  return queue;
}

void pq_free(priority_queue_t *queue) {
  free(queue->items);
  queue->items = 0;
  free(queue);
}

void pq_enqueue(priority_queue_t *queue, int priority, node_t *value) {
  assert(queue->size < queue->capacity);

  queue->items[queue->size].priority = priority;
  queue->items[queue->size].value = value;
  queue->size++;
}

pair_t pq_dequeue(priority_queue_t *queue) {
  assert(queue->size > 0);

  int min_index = 0;
  int min_value = queue->size > 0 ? queue->items[0].priority : -1;

  for (int i = 1; i < queue->size; i++) {
    if (queue->items[i].priority < min_value) {
      min_index = i;
      min_value = queue->items[i].priority;
    }
  }

  pair_t output = queue->items[min_index];

  queue->items[min_index] = queue->items[queue->size - 1];
  queue->size--;

  return output;
}

bits_t *bits_create(int size, int capacity, bool *base) {
  bits_t *bits = (bits_t *)malloc(sizeof(bits_t));
  bits->size = size;
  bits->capacity = capacity;
  bits->bits = (bool *)calloc(capacity, sizeof(bool));
  
  memcpy(bits->bits, base, bits->size * sizeof(bool));
  
  return bits;
}

bits_t *bits_create() {
  return bits_create(0, BIT_VECTOR_CAPACITY_HACK, 0);
}

bits_t *bits_clone(bits_t *base) {
  return bits_create(base->size, base->size, base->bits);
}

void bits_append(bits_t *bits, bool bit) {
  if (bits->size >= bits->capacity) {
    bool *new_bits = (bool *)calloc(bits->capacity * 2, sizeof(bool));
    memcpy(new_bits, bits->bits, bits->size * sizeof(bool));
    free(bits->bits);
    bits->bits = new_bits;
    bits->capacity *= 2;
  }

  bits->bits[bits->size] = bit;
  bits->size++;
}

void bits_remove_last(bits_t *bits) {
  assert(bits->size > 0);
  bits->size--;
}

void bits_free(bits_t *bits) {
  free(bits->bits);
  bits->bits = 0;
  free(bits);
}

bis_t *bis_create(FILE *base) {
  bis_t *bis = (bis_t *)malloc(sizeof(bis_t));
  bis->base = base;
  bis->buf = 0;
  bis->num_bits = 0;
  return bis;
}

// 1 for true, 0 for false, or EOF for EOF
int bis_read(bis_t *bis) {
  if (bis->num_bits <= 0) {
    int c = getc(bis->base);
    if (c == EOF) { return -1; }

    bis->buf = c & 0xff;
    bis->num_bits = 8;
  }

  int bit = bis->buf >> 7;

  bis->buf = (bis->buf << 1) & 0xff;
  bis->num_bits--;
  return bit;
}

bos_t *bos_create(FILE *base) {
  bos_t *bos = (bos_t *)malloc(sizeof(bos_t));
  bos->base = base;
  bos->buf = 0;
  bos->num_bits = 0;
  return bos;
}

void bos_write(bos_t *bos, bool *bits, int num) {
  assert(bos->num_bits < 8);

  while(num > 0) {
    bos->buf = (bos->buf << 1) | *bits;
    bos->num_bits++;

    if (bos->num_bits >= 8) {
      bos_flush(bos);
    }

    bits++;
    num--;
  }
}

void bos_flush(bos_t *bos) {
  fputc(bos->buf << (8 - bos->num_bits), bos->base);
  bos->buf = 0;
  bos->num_bits = 0;
}

node_t *build_encode_tree(int *freq) {
  priority_queue_t *pq = pq_create();

  for (int i = 0; i < 256; i++) {
    pq_enqueue(pq, freq[i], node_create(i));
    pq->items[pq->size - 1].priority = freq[i];
  }

  while(pq->size > 1) {
    pair left = pq_dequeue(pq);
    pair right = pq_dequeue(pq);

    node_t *parent = node_create(0, left.value, right.value);
    pq_enqueue(pq, left.priority + right.priority, parent);
  }

  node_t *tree = pq_dequeue(pq).value;
  pq_free(pq);
  return tree;
}
