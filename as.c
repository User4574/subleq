#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "as.h"

void gettoken(TOKEN* token, FILE* file) {
  char i[10];
  int a, b, c;
  char line[30];
  int res;

  if (fgets(line, 30, file) == NULL) {
    if (feof(file)) {
      token->eof = true;
      token->valid = true;
    } else {
      token->eof = false;
      token->valid = true;
    }
  } else {
    res = sscanf(line, "%s %x, %x, %x", i, &a, &b, &c);

    if (!strncmp(i, "nop",    3)) token->i = NOP;
    if (!strncmp(i, "subleq", 6)) token->i = SUBLEQ;
    if (!strncmp(i, "sub",    3)) token->i = SUB;
    if (!strncmp(i, "add",    3)) token->i = ADD;
    if (!strncmp(i, "jmp",    3)) token->i = JMP;
    if (!strncmp(i, "mov",    3)) token->i = MOV;
    if (!strncmp(i, "beq",    3)) token->i = BEQ;
    if (!strncmp(i, "not",    3)) token->i = NOT;
    if (!strncmp(i, "halt",   4)) token->i = HALT;
    if (!strncmp(i, "data",   4)) token->i = DATA;

    token->args = res - 1;
    token->a = a;
    token->b = b;
    token->c = c;
    token->eof = false;
    token->valid = true;
  }
}

bool handletoken(FILE* file, int* pc, TOKEN* token) {
  switch(token->i) {
    case NOP:
      if (token->args != 0) return false;
      *pc += 3; out(file, Z, Z, *pc);
      break;
    case SUBLEQ:
      switch (token->args) {
        case 2:
          *pc += 3; out(file, token->a, token->b, *pc);
          break;
        case 3:
          *pc += 3; out(file, token->a, token->b, token->c);
          break;
        default:
          return false;
      }
      break;
    case SUB:
      if (token->args != 2) return false;
      *pc += 3; out(file, token->a, token->b, *pc);
      break;
    case ADD:
      if (token->args != 2) return false;
      *pc += 3; out(file, token->a, Z, *pc);
      *pc += 3; out(file, Z, token->b, *pc);
      *pc += 3; out(file, Z, Z, *pc);
      break;
    case MOV:
      if (token->args != 2) return false;
      *pc += 3; out(file, token->b, token->b, *pc);
      *pc += 3; out(file, token->a, Z, *pc);
      *pc += 3; out(file, Z, token->b, *pc);
      *pc += 3; out(file, Z, Z, *pc);
      break;
    case BEQ:
      if (token->args != 2) return false;
      *pc += 3; out(file, token->b, Z, *pc + 3);
      *pc += 3; out(file, Z, Z, *pc + 6);
      *pc += 3; out(file, Z, Z, *pc);
      *pc += 3; out(file, Z, token->b, token->c);
      break;
    case JMP:
      if (token->args != 1) return false;
      *pc += 3; out(file, Z, Z, token->a);
      break;
    case NOT:
      if (token->args != 1) return false;
      *pc += 3;
      break;
    case HALT:
      *pc += 3; out(file, -1, -1, -1);
      break;
    case DATA:
      *pc += 3; 
      switch (token->args) {
        case 0:
          out(file, 0, 0, 0);
          break;
        case 1:
          out(file, token->a, 0, 0);
          break;
        case 2:
          out(file, token->a, token->b, 0);
          break;
        case 3:
          out(file, token->a, token->b, token->c);
      }
  }
  return true;
}

bool parse(TOKEN* token, FILE *out, FILE *in) {
  int pc = PLEN;
  int line = 0;

  while(true) {
    line++;
    gettoken(token, in);
    if (token->eof) break;
    if (!token->valid || !handletoken(out, &pc, token)) {
      fprintf(stderr, "Error parsing line %d\n", line);
      return false;
    }
  }

  return true;
}

int main(int argc, char** argv) {
  if (argc < 3)
    return 1;

  int ret;
  FILE *in = fopen(argv[1], "r"), *out = fopen(argv[2], "w");
  TOKEN* token = malloc(sizeof(token));

  preamble(out);
  ret = parse(token, out, in);
  postamble(out);

  fclose(in);
  fclose(out);
  free(token);

  return !ret;
}
