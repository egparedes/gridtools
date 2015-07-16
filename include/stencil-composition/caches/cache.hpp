/**
   @file
   @brief File containing the definition of caches. They are the API exposed to the user to describe
   parameters that will be cached in a on-chip memory.
*/

#pragma once

#include "cache_definitions.hpp"
#include "../accessor.hpp"

namespace gridtools{

/**
 * @struct cache
 * main API class to define caches for particular parameters
 * The cache type is described with a template parameter to the class
 * Caching assumes a parallelization model where all the processing all elements in the vertical dimension are
 * private to each parallel thread,
 * while the processing of grid points in the horizontal plane is executed by different parallel threads.
 * Those caches that cover data in the horizontal (IJ and IJK) are accessed by parallel core units, and
 * therefore require synchronization capabilities (for example shared memory in the GPU), like IJ or IJK caches.
 * On the contrary caches in the K dimension are only accessed by one thread, and therefore resources can be allocated
 * in on-chip without synchronization capabilities (for example registers in GPU)
 * @tparam  cacheType type of cache
 * @tparam Arg argument with parameter being cached
 * @tparam CacheIOPolicy IO policy for cache
 */
template<CacheType cacheType, typename Arg, CacheIOPolicy cacheIOPolicy>
struct cache
{
    GRIDTOOLS_STATIC_ASSERT((is_arg<Arg>::value), "argument passed to ij cache is not of the right arg<> type");
};

}