#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX 35000

int main()
{
   int vetor[MAX];
   int aux[MAX];
   int i, j;
   float a = 2, b = 3;

   srand(time(NULL));

   for (i=0; i < MAX; i++)
     {
           vetor[i] = (rand() % 5);
           //printf ("%d" , vetor[i]);
           //printf("%s\n", "\n");
     }

    for (i=0; i < MAX; i++){
     switch (vetor[i]) {
       case 0:
          vetor[i] = a + b;
          //printf ("%d ", vetor[i]);
          //printf ("%s", "\n");
          break;
       case 1:
          vetor[i] = a - b;
          //printf ("%d ", vetor[i]);
          //printf ("%s", "\n");
          break;
       case 2:
          vetor[i] = a * b;
          //printf ("%d ", vetor[i]);
          //printf ("%s", "\n");
          break;
       case 3:
          vetor[i] = a / b;
          //printf ("%d ", vetor[i]);
          //printf ("%s", "\n");
          break;
       case 4:
          for(j=0; j < vetor[i]; j++){
            aux[j] = vetor[i];
            //printf ("%d ", aux[j]);
            //printf ("%s", "\n");
          }
          break;
       default:
          //printf("%s\n", "ACAbou");
          break;
     }
}
   return 0;
}
