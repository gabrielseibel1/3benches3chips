#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#ifndef FLT_MAX
#define FLT_MAX 3.40282347e+38
#endif

#define N_OBJ 100000
#define N_ATTR 5
#define K 2
#define N_THREADS 1

/**
 * multi-dimensional spatial Euclid distance square
 */
float euclid_dist_2(float *pt1,
                    float *pt2,
                    int numdims) ;

int main(int argc, char *argv[]) {
  float points[N_OBJ][N_ATTR], delta;
  int i, j, k, n = 0, index, loop = 0;
  float threshold = 0.001;

  /* perform regular Kmeans */
  /* allocate space for returning variable clusters[] */
  float clusters[K][N_ATTR];

  int new_centers_len[K];         /* [K]: no. of points in each cluster */
  float new_centers[K][N_ATTR];/* [K][N_ATTR] */

  int partial_new_centers_len[N_THREADS][K];
  float partial_new_centers[N_THREADS][K][N_ATTR];

  int membership[N_OBJ];

  srand(7);

  /* allocate space for points[] and read points of all objects */
  for (i = 0; i < N_OBJ; ++i) {
    for (j = 0; j < N_ATTR; ++j) {
      points[i][j] = rand();
    }
  }

  /* randomly pick cluster centers */
  for (i = 0; i < K; i++) {
    //n = (int)rand() % N_OBJ;
    for (j = 0; j < N_ATTR; j++)
      clusters[i][j] = points[n][j];
    n++;
  }

  for (i = 0; i < N_OBJ; i++)
    membership[i] = -1;

  do {
    delta = 0.0;
    int tid = 0;
    for (i = 0; i < N_OBJ; i++) {
      /* find the index of nestest cluster centers */
      index = find_nearest_point(points[i],
                                 clusters,
                                 K);
      /* if membership changes, increase delta by 1 */
      if (membership[i] != index) delta += 1.0;

      /* assign the membership to object i */
      membership[i] = index;

      /* update new cluster centers : sum of all objects located within */
      partial_new_centers_len[tid][index]++;
      for (j = 0; j < N_ATTR; j++)
        partial_new_centers[tid][index][j] += points[i][j];
    }

    /* let the main thread perform the array reduction */
    for (i = 0; i < K; i++) {
      for (j = 0; j < N_THREADS; j++) {
        new_centers_len[i] += partial_new_centers_len[j][i];
        partial_new_centers_len[j][i] = 0.0;
        for (k = 0; k < N_ATTR; k++) {
          new_centers[i][k] += partial_new_centers[j][i][k];
          partial_new_centers[j][i][k] = 0.0;
        }
      }
    }

    /* replace old cluster centers with new_centers */
    for (i = 0; i < K; i++) {
      for (j = 0; j < N_ATTR; j++) {
        if (new_centers_len[i] > 0)
          clusters[i][j] = new_centers[i][j] / new_centers_len[i];
        new_centers[i][j] = 0.0;   /* set back to 0 */
      }
      new_centers_len[i] = 0;   /* set back to 0 */
    }

  } while (delta > threshold && loop++ < 500);


  printf("Cluster Centers Output\n");
  printf("The first number is cluster number and the following data is attribute value\n");
  printf("=============================================================================\n\n");

  for (i = 0; i < K; i++) {
    printf("%d: ", i);
    for (j = 0; j < N_ATTR; j++)
      printf("%.2f ", clusters[i][j]);
    printf("\n\n");
  }


  printf("KMEANS: %d objects - %d points - %d (k) clusters\n", N_OBJ, N_ATTR, K);

  return (0);
}

int find_nearest_point(float *pt,          /* [N_ATTR] */
                       float pts[K][N_ATTR],         /* [npts][N_ATTR] */
                       int npts
) {
  int index, i;
  float min_dist = FLT_MAX;

/* find the cluster center id with min distance to pt */
  for (i = 0; i < npts; i++) {
    float dist;
    dist = euclid_dist_2(pt, pts[i], N_ATTR);  /* no need square root */
    if (dist < min_dist) {
      min_dist = dist;
      index = i;
    }
  }
  return (index);
}

/**
 * multi-dimensional spatial Euclid distance square
 */
__inline
float euclid_dist_2(float *pt1,
                    float *pt2,
                    int numdims) {
  int i;
  float ans = 0.0;

  for (i = 0; i < numdims; i++)
    ans += (pt1[i] - pt2[i]) * (pt1[i] - pt2[i]);

  return (ans);
}
