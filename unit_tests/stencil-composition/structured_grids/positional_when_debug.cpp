/*
  GridTools Libraries

  Copyright (c) 2016, GridTools Consortium
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

  1. Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

  3. Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  For information: http://eth-cscs.github.io/gridtools/
*/
/*
 * test_computation.cpp
 *
 *  Created on: Mar 9, 2015
 *      Author: carlosos
 */

#define BOOST_NO_CXX11_RVALUE_REFERENCES

#ifdef NDEBUG
#undef NDEBUG
#define __WAS_DEBUG
#endif

#include <gridtools.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/fusion/include/make_vector.hpp>

#include "gtest/gtest.h"

#include <stencil-composition/stencil-composition.hpp>
#include "stencil-composition/backend.hpp"
#include "stencil-composition/make_computation.hpp"
#include "stencil-composition/make_stencils.hpp"


namespace positional_when_debug_test{

    typedef gridtools::interval<gridtools::level<0,-1>, gridtools::level<1,-1> > x_interval;
    typedef gridtools::interval<gridtools::level<0,-2>, gridtools::level<1,1> > axis_t;
    typedef gridtools::grid<axis_t> grid_t;


    struct test_functor {
        typedef gridtools::accessor< 0, gridtools::enumtype::inout > in;
        typedef boost::mpl::vector1<in> arg_list;

        template <typename Evaluation>
        GT_FUNCTION
        static void Do(Evaluation const & eval, x_interval) {
            eval.i();
            eval.j();
            eval.k();
        }
    };
}

TEST(test_make_computation, positional_when_debug) {

    using namespace gridtools;
    using namespace gridtools::enumtype;
#ifdef __CUDACC__
#define BACKEND backend< Cuda, GRIDBACKEND, Block >
#else
#define BACKEND backend< Host, GRIDBACKEND, Block >
#endif

    typedef layout_map<2,1,0> layout_t;
    typedef BACKEND::storage_type<int, BACKEND::storage_info<0,layout_t> >::type storage_type;
    BACKEND::storage_info<0,layout_t> sinfo(3,3,3);
    storage_type a_storage(sinfo, 0, "test");

    typedef arg<0, storage_type> p_in;
    typedef boost::mpl::vector<p_in> accessor_list_t;

    /* canot use the assignment since with a single placeholder the wrong constructor is picked.
       This is a TODO in aggregator_type.hpp */
    aggregator_type< accessor_list_t > dm(boost::fusion::make_vector(&a_storage));
#ifdef CXX11_ENABLED
    auto
#else
#ifdef __CUDACC__
    computation *
#else
    boost::shared_ptr< gridtools::computation >
#endif
#endif
        test_computation = make_computation< BACKEND >(dm,
            positional_when_debug_test::grid_t({0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}),
            make_multistage // mss_descriptor
            (execute< forward >(), make_stage< positional_when_debug_test::test_functor >(p_in())));

    EXPECT_TRUE(true);
}


#ifdef __WAS_DEBUG
#undef __WAS_DEBUG
#define NDEBUG
#endif