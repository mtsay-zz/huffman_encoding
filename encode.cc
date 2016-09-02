#include "common.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void populate_mapping_table(bool **table, node_t *tree, bool *path, int depth);

header_t *create_header(FILE *is) {
  header_t *header = (header_t *)malloc(sizeof(header_t));
  memset(header, 0, sizeof(header_t));

  int c;

  while ((c = getc(is)) != EOF) {
    header->freq[c]++;
    header->size++;
  }

  return header;
}

void populate_mapping_table(bits_t **table, node_t *tree, bits_t *path);

void populate_mapping_table_traversal(bits_t **table, node_t *new_tree, bits_t *path, bool dir) {
  bits_append(path, dir);
  populate_mapping_table(table, new_tree, path);
  bits_remove_last(path);
}

void populate_mapping_table(bits_t **table, node_t *tree, bits_t *path) {
  assert((tree->left == 0) == (tree->right == 0));

  if (tree->left == 0) {
    table[tree->value] = bits_clone(path);
  } else {
    populate_mapping_table_traversal(table, tree->left, path, 0);
    populate_mapping_table_traversal(table, tree->right, path, 1);
  }
}

bits_t **create_mapping_table(node_t *tree) {
  bits_t *bits = bits_create();
  bits_t **mapping = (bits_t **)calloc(256, sizeof(bits_t *));
  populate_mapping_table(mapping, tree, bits);
  bits_free(bits);
  return mapping;
}

void encode(header_t *header, bits_t **mapping, FILE *is, FILE *os) {
  fwrite(header, 1, sizeof(header_t), os);
  fflush(os);

  bos_t *bos = bos_create(os);

  int c;
  while ((c = getc(is)) != EOF) {
    bits_t *bits = mapping[c];
    bos_write(bos, bits->bits, bits->size);
  }

  bos_flush(bos);

  free(bos);
}

void free_mapping(bits_t **mapping) {
  for (int i = 0 ; i < 256; i++) {
    bits_free(mapping[i]);
  }

  free(mapping);
}

int main() {

  header_t *header = create_header(stdin);
  rewind(stdin);

  node_t *tree = build_encode_tree(header->freq);
  bits_t **mapping = create_mapping_table(tree);
  encode(header, mapping, stdin, stdout);

  free_mapping(mapping);
  free_tree(tree);
  free(header);

  return 0;
}
