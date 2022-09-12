/*******************************************************************************
 *
 *   OCO Source Materials
 *   5737-A56
 *   © Copyright IBM Corp. 2017
 *
 *   The source code for this program is not published or other-wise divested
 *   of its trade secrets, irrespective of what has been deposited with the
 *   U.S. Copyright Office.
 ******************************************************************************/

#ifndef OMP_WRAPPER_H_
#define OMP_WRAPPER_H_

#ifndef DISABLE_OPENMP
#include <omp.h>
#else
class omp_lock_t
{
public:
  omp_lock_t() {}
};

static void omp_init_lock(omp_lock_t* lock) {}

static void omp_destroy_lock(omp_lock_t* lock) {}

static void omp_set_lock(omp_lock_t* lock) {}

static void omp_unset_lock(omp_lock_t* lock) {}

static int omp_get_max_threads() { return 1; }

static bool omp_in_parallel() { return false; }
#endif

#endif /* OMP_WRAPPER_H_ */