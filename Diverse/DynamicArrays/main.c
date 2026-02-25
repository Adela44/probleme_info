#include <stdio.h>
#include <stdlib.h>

typedef struct {
    float *items;
    size_t capacity;
    size_t count;
}Numbers;

#define da_append(num, i)\
  do {\
      if (num.count >= num.capacity) {\
          if (num.capacity == 0)  num.capacity = 256;\
          else num.capacity *= 2;\
          num.items = realloc(num.items, sizeof(*num.items) * num.capacity);\
      }\
      num.items[num.count++] = i;\
  }while (0)

int main(void) {
    Numbers num = {0};
    for (int i = 0; i < 10; i ++) {
             da_append(num,i); // + you need to check realloc
    }
    for (size_t i = 0; i < num.count; i ++) {
        printf("%f\n", num.items[i]);
    }
    free(num.items);
    return 0;
}