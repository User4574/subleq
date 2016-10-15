#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {
  if (argc < 2)
    return 1;

  FILE* file = fopen(argv[1], "r");
  fseek(file, 0, SEEK_END);
  int flen = ftell(file) / 4;
  fseek(file, 0, SEEK_SET);

  int* mem = calloc(flen, sizeof(int));
  fread(mem, sizeof(int), flen, file);
  fclose(file);

  int a, b, c;
  int pc = 0;

  while (pc >= 0) {
    a = mem[pc];
    b = mem[pc + 1];
    c = mem[pc + 2];
    if (a < 0 || b < 0)
      pc = -1;
    else {
      mem[b] = mem[b] - mem[a];
      if (mem[b] > 0)
        pc += 3;
      else
        pc = c;
    }
  }

  file = fopen(argv[1], "w");
  fwrite(mem, sizeof(int), flen, file);
  fflush(file);
  fclose(file);
  free(mem);

  return 0;
}
