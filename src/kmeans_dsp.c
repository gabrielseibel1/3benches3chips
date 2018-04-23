#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef FLT_MAX
#define FLT_MAX 3.40282347e+38
#endif

int cluster(int, int, float **, int, float, float ***);

float **kmeans_clustering(float **, int, int, int, float, int *);

float euclid_dist_2(float *, float *, int);

int find_nearest_point(float *, int, float **, int);

int num_omp_threads = 1;

int main() {

  int argc, argv0, argv1, argv2;
  argc = 4;
  argv0 = 400;
  argv1 = 5;
  argv2 = 2;

  int numClusters;
  float *buf;
  float **attributes;
  float **cluster_centres = NULL;
  int i;

  int numAttributes;
  int numObjects;
  float threshold = 0.001;

  if (argc != 4) {
    fprintf(stderr, "Expected 3 parameters: n_objs n_features k_clusters\n");
    exit(EXIT_FAILURE);
  }

  numObjects = argv0;
  numAttributes = argv1;
  numClusters = argv2;

  /* allocate space for attributes[] and read attributes of all objects */
  buf = (float *) malloc(numObjects * numAttributes * sizeof(float));
  attributes = (float **) malloc(numObjects * sizeof(float *));
  attributes[0] = (float *) malloc(numObjects * numAttributes * sizeof(float));
  for (i = 1; i < numObjects; i++)
    attributes[i] = attributes[i - 1] + numAttributes;

  for (i = 0; i < numObjects * numAttributes; ++i) 
    buf[i] = rand();
  
  memcpy(attributes[0], buf, numObjects * numAttributes * sizeof(float));

  cluster_centres = NULL;
  cluster(numObjects,
          numAttributes,
          attributes,           /* [numObjects][numAttributes] */
          numClusters,
          threshold,
          &cluster_centres
  );


  printf("KMEANS: %d objects - %d attributes - %d (k) clusters\n", numObjects, numAttributes, numClusters);

  /*printf("Cluster Centers Output\n");
  printf("The first number is cluster number and the following data is atrribute value\n");
  printf("=============================================================================\n\n");

  for (i = 0; i < numClusters; i++) {
    printf("%d: ", i);
    for (j = 0; j < numAttributes; j++)
      printf("%.2f ", cluster_centres[i][j]);
    printf("\n\n");
  }*/

  free(attributes);
  free(cluster_centres[0]);
  free(cluster_centres);
  free(buf);
  return (0);
}

int cluster(int numObjects,      /* number of input objects */
            int numAttributes,   /* size of attribute of each object */
            float **attributes,      /* [numObjects][numAttributes] */
            int nclusters,
            float threshold,       /* in:   */
            float ***cluster_centres /* out: [best_nclusters][numAttributes] */

) {
  int *membership;
  float **tmp_cluster_centres;

  membership = (int *) malloc(numObjects * sizeof(int));

  srand(7);
  /* perform regular Kmeans */
  tmp_cluster_centres = kmeans_clustering(attributes,
                                          numAttributes,
                                          numObjects,
                                          nclusters,
                                          threshold,
                                          membership);

  if (*cluster_centres) {
    free((*cluster_centres)[0]);
    free(*cluster_centres);
  }
  *cluster_centres = tmp_cluster_centres;


  free(membership);

  return 0;
}

int find_nearest_point(float *pt,          /* [nfeatures] */
                       int nfeatures,
                       float **pts,         /* [npts][nfeatures] */
                       int npts) {
  int index, i;
  float min_dist = FLT_MAX;

  /* find the cluster center id with min distance to pt */
  for (i = 0; i < npts; i++) {
    float dist;
    dist = euclid_dist_2(pt, pts[i], nfeatures);  /* no need square root */
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

float **kmeans_clustering(float **feature,    /* in: [npoints][nfeatures] */
                          int nfeatures,
                          int npoints,
                          int nclusters,
                          float threshold,
                          int *membership) /* out: [npoints] */
{

  int i, j, k, n = 0, index, loop = 0;
  int *new_centers_len;      /* [nclusters]: no. of points in each cluster */
  float **new_centers;        /* [nclusters][nfeatures] */
  float **clusters;          /* out: [nclusters][nfeatures] */
  float delta;

  int nthreads;
  int **partial_new_centers_len;
  float ***partial_new_centers;

  nthreads = num_omp_threads;

  /* allocate space for returning variable clusters[] */
  clusters = (float **) malloc(nclusters * sizeof(float *));
  clusters[0] = (float *) malloc(nclusters * nfeatures * sizeof(float));
  for (i = 1; i < nclusters; i++)
    clusters[i] = clusters[i - 1] + nfeatures;

  /* randomly pick cluster centers */
  for (i = 0; i < nclusters; i++) {
    //n = (int)rand() % npoints;
    for (j = 0; j < nfeatures; j++)
      clusters[i][j] = feature[n][j];
    n++;
  }

  for (i = 0; i < npoints; i++)
    membership[i] = -1;

  /* need to initialize new_centers_len and new_centers[0] to all 0 */
  new_centers_len = (int *) calloc(nclusters, sizeof(int));

  new_centers = (float **) malloc(nclusters * sizeof(float *));
  new_centers[0] = (float *) calloc(nclusters * nfeatures, sizeof(float));
  for (i = 1; i < nclusters; i++)
    new_centers[i] = new_centers[i - 1] + nfeatures;


  partial_new_centers_len = (int **) malloc(nthreads * sizeof(int *));
  partial_new_centers_len[0] = (int *) calloc(nthreads * nclusters, sizeof(int));
  for (i = 1; i < nthreads; i++)
    partial_new_centers_len[i] = partial_new_centers_len[i - 1] + nclusters;

  partial_new_centers = (float ***) malloc(nthreads * sizeof(float **));
  partial_new_centers[0] = (float **) malloc(nthreads * nclusters * sizeof(float *));
  for (i = 1; i < nthreads; i++)
    partial_new_centers[i] = partial_new_centers[i - 1] + nclusters;

  for (i = 0; i < nthreads; i++) {
    for (j = 0; j < nclusters; j++)
      partial_new_centers[i][j] = (float *) calloc(nfeatures, sizeof(float));
  }
  //printf("num of threads = %d\n", num_omp_threads);
  do {
    delta = 0.0;
    int tid = 0;
    for (i = 0; i < npoints; i++) {
      /* find the index of nestest cluster centers */
      index = find_nearest_point(feature[i],
                                 nfeatures,
                                 clusters,
                                 nclusters);
      /* if membership changes, increase delta by 1 */
      if (membership[i] != index) delta += 1.0;

      /* assign the membership to object i */
      membership[i] = index;

      /* update new cluster centers : sum of all objects located
       within */
      partial_new_centers_len[tid][index]++;
      for (j = 0; j < nfeatures; j++)
        partial_new_centers[tid][index][j] += feature[i][j];
    }

    /* let the main thread perform the array reduction */
    for (i = 0; i < nclusters; i++) {
      for (j = 0; j < nthreads; j++) {
        new_centers_len[i] += partial_new_centers_len[j][i];
        partial_new_centers_len[j][i] = 0.0;
        for (k = 0; k < nfeatures; k++) {
          new_centers[i][k] += partial_new_centers[j][i][k];
          partial_new_centers[j][i][k] = 0.0;
        }
      }
    }

    /* replace old cluster centers with new_centers */
    for (i = 0; i < nclusters; i++) {
      for (j = 0; j < nfeatures; j++) {
        if (new_centers_len[i] > 0)
          clusters[i][j] = new_centers[i][j] / new_centers_len[i];
        new_centers[i][j] = 0.0;   /* set back to 0 */
      }
      new_centers_len[i] = 0;   /* set back to 0 */
    }

  } while (delta > threshold && loop++ < 500);


  free(new_centers[0]);
  free(new_centers);
  free(new_centers_len);

  return clusters;
}