#pragma once

namespace gridtools {

    namespace iterate_domain_aux {
        struct assign_iterators {
            int i, j, k;

            GT_FUNCTION
            assign_iterators(int i, int j, int k)
                : i(i)
                , j(j)
                , k(k)
            {}

            template <typename ZipElem>
            GT_FUNCTION
            void operator()(ZipElem const& ze) const {
                // boost::fusion::at<0>(ze) = &( (*(boost::fusion::at<boost::mpl::int_<1> >(ze)))(i,j,k) );
            }
        };

        struct increment {
            template <typename Iterator>
            GT_FUNCTION
            void operator()(Iterator & it) const {
                printf("outout INCREMENT %X\n", it);
                ++it;
            }
        };

    } // namespace iterate_domain_aux 

    template <typename LocalDomain>
    struct iterate_domain {
        typedef typename LocalDomain::local_iterators_type local_iterators_type;

        size_t stride;
        LocalDomain const& local_domain;
        mutable local_iterators_type local_iterators;

        iterate_domain(LocalDomain const& local_domain, int i, int j, int k)
            : stride(1)
            , local_domain(local_domain)
        {
            typedef boost::fusion::vector<local_iterators_type&, typename LocalDomain::local_args_type const&> to_zip;
            typedef boost::fusion::zip_view<to_zip> zipping;
            to_zip z(local_iterators, local_domain.local_args);
            boost::fusion::for_each(zipping(z), iterate_domain_aux::assign_iterators(i,j,k));
        }

        void increment() const {
            boost::fusion::for_each(local_iterators, iterate_domain_aux::increment());
        }

        template <typename T>
        GT_FUNCTION
        void info(T const &x) const {
            std::cout << "Well... to late for that\n" << std::endl;
            local_domain.info(x);
        }

        template <typename T>
        GT_FUNCTION
        typename boost::mpl::at<typename LocalDomain::esf_args, typename T::index_type>::type::value_type&  
        operator()(T const& t) const {
            return local_domain(t);
        }

        template <typename T>
        GT_FUNCTION
        typename boost::mpl::at<typename LocalDomain::esf_args, typename T::index>::type::value_type const& 
        operator()(T const& t, int i, int j, int k) const {
            return local_domain(t, i,j,k);
        }

    };

} // namespace gridtools