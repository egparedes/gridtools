#include <iostream>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/range_c.hpp>
#include "Level.h"
#include "Interval.h"
#include "FunctorDoMethods.h"

// test functor 1
struct Functor0 
{
    template <typename TArguments>
    static void Do(TArguments& args, Interval<Level<3,-1>, Level<3,-1> >) 
    {
        std::cout << "Functor0:Do(Interval<Level<3,-1>, Level<3,-1> >) called" << std::endl;
    }
};

// test functor 1
struct Functor1 
{
    template <typename TArguments>
    static void Do(TArguments& args, Interval<Level<0,1>, Level<2,-1> >) 
    {
        std::cout << "Functor1:Do(Interval<Level<0,1>, Level<2,-1> >) called" << std::endl;
    }
};

// test functor 2
struct Functor2 
{
    template <typename TArguments>
    static void Do(TArguments& args, Interval<Level<0,1>, Level<1,-1> >) 
    {
        std::cout << "Functor2:Do(Interval<Level<0,1>, Level<1,-1> >) called" << std::endl;
    }

    template <typename TArguments>
    static void Do(TArguments& args, Interval<Level<1,1>, Level<3,-1> >) 
    {
        std::cout << "Functor2:Do(Interval<Level<1,1>, Level<3,-1> >) called" << std::endl;
    }
};

// illegal functor
struct IllegalFunctor 
{
    template <typename TArguments>
    static void Do(TArguments& args, Interval<Level<1,1>, Level<2,-1> >) {}
    template <typename TArguments>
    static void Do(TArguments& args, Interval<Level<1,1>, Level<3,-2> >) {}
    template <typename TArguments>
    static void Do(TArguments& args, Interval<Level<3,-1>, Level<3,-1> >) {}
};

// functor printing level and index
struct PrintLevel
{
    template<typename T> 
    void operator()(T) 
    { 
        typedef typename index_to_level<T>::type Level;
        typedef typename level_to_index<Level>::type Index;

        std::cout 
            << "Index: " << Index::value << "\t"
            << "Level(" << Level::Splitter::value << ", " << Level::Offset::value << ")" << std::endl;
    }
};

// helper executing a functor via boost for each
template<typename TFunctor>
struct RunnerFunctor
{
    template<typename T> 
    void operator()(T) 
    { 
        // define the do method interval type
        typedef typename make_interval<
            typename boost::mpl::first<T>::type,
            typename boost::mpl::second<T>::type
        >::type Interval;

        int argument = 0;
        TFunctor::Do(argument, Interval()); 
    }
};

// test method computing functor do methods
int main(int argc, char *argv[])
{
    std::cout 
        << "Functor Do Methods" << std::endl
        << "==================" << std::endl;

    // test the level to index conversions be enumerating all levels in an range
    // (for test purposes convert the range into levels and back into an index)
    std::cout << "Verify the level index computation:" << std::endl;
    boost::mpl::for_each<
        boost::mpl::range_c<int, 0, 20>
    >(PrintLevel());
    std::cout << "Done!" << std::endl; 

    // check has_do_simple on a few examples
    BOOST_STATIC_ASSERT((has_do_simple<IllegalFunctor, Interval<Level<1,1>, Level<2,-1> > >::value));
    BOOST_STATIC_ASSERT((has_do_simple<IllegalFunctor, Interval<Level<1,1>, Level<3,-2> > >::value));
    BOOST_STATIC_ASSERT((!has_do_simple<IllegalFunctor, Interval<Level<0,1>, Level<3,-2> > >::value));

    // define the axis search interval
    typedef Interval<Level<0,-3>, Level<3,3> > AxisInterval;
   
    // run all methods of functor 0
    std::cout << "Print Functor0 Do methods:" << std::endl;
    boost::mpl::for_each<
        compute_functor_do_methods<
            Functor0, 
            AxisInterval
        >::type
    >(RunnerFunctor<Functor0>());
    std::cout << "Done!" << std::endl; 

    // run all methods of functor 1
    std::cout << "Print Functor1 Do methods:" << std::endl;
    boost::mpl::for_each<
        compute_functor_do_methods<
            Functor1, 
            AxisInterval
        >::type
    >(RunnerFunctor<Functor1>());
    std::cout << "Done!" << std::endl; 

    // run all methods of functor 2
    std::cout << "Print Functor2 Do methods:" << std::endl;
    boost::mpl::for_each<
        compute_functor_do_methods<
            Functor2, 
            AxisInterval
        >::type
    >(RunnerFunctor<Functor2>());
    std::cout << "Done!" << std::endl; 

    // test illegal functor
    //typedef compute_functor_do_methods<IllegalFunctor, AxisInterval>::type TestIllegalFunctor;

    return 0;
}