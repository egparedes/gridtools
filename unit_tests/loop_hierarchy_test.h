#pragma once

#include <stencil-composition/loop_hierarchy.h>
#define VERBOSE 1

namespace loop_test{
using namespace gridtools;

//stub iterate_domain
struct iterate_domain_{

    template <typename Index>
    void get_index(Index idx) const {}

    template <typename Index>
    void set_index(Index idx){}

    template< ushort_t index, enumtype::execution>
    void increment(){}
};

struct functor{

    functor():m_iterations(0){}

    void operator()(){
        m_iterations ++;
    }

    uint_t m_iterations;
};

bool test(){

    typedef  array<uint_t, 3> array_t;
    iterate_domain_ it_domain;
    functor fun;

    loop_hierarchy<array_t, loop_item<1, enumtype::forward>, loop_item<5, enumtype::forward>, static_loop_item<0, enumtype::forward, 0, 10> > h(2, 5, 6, 8);
    h.apply(it_domain, fun);

    return fun.m_iterations == 4*3*11;
}
}//namespace loop_test