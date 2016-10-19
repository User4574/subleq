#define bool char
#define true 1
#define false 0

typedef enum {
  NOP, SUBLEQ, SUB, ADD, JMP, MOV, BEQ, NOT, HALT, DATA
} INSTRUCTION;

typedef struct {
  INSTRUCTION i;
  int a, b, c;
  int args;
  bool eof;
  bool valid;
} TOKEN;

void out(FILE* file, int a, int b, int c) {
  fwrite(&a, sizeof(int), 1, file);
  fwrite(&b, sizeof(int), 1, file);
  fwrite(&c, sizeof(int), 1, file);
  fflush(file);
}

#define PLEN 6
#define Z 3

void preamble(FILE* file) {
  // Jump over Z
  out(file, Z, Z, PLEN);
  // Z
  out(file, 0, 0, 0);
}

void postamble(FILE* file) {
  // Halt
  out(file, -1, -1, -1);
}
