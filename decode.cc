#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "common.h"

void decode(header_t *header, node_t *tree, FILE *is, FILE *os) {
  bis_t *bis = bis_create(is);

  int written = 0, c;
  node_t *curr = tree;

  while ((c = bis_read(bis)) != -1 
      && written < header->size) {
    curr = c == 1 ? curr->right : curr->left;

    if (curr->left == 0 && curr->right == 0) {
      fputc(curr->value, os);
      written++;
      curr = tree;
    }
  }

  fflush(os);
  free(bis);
}

int main() {
  header_t *header = (header_t *)malloc(sizeof(header_t));
  fread(header, 1, sizeof(header_t), stdin);

  node_t *tree = build_encode_tree(header->freq);

  decode(header, tree, stdin, stdout);

  free_tree(tree);

  return 0;
}
