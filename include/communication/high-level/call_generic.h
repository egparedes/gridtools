#if !BOOST_PP_IS_ITERATING

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>

#define BOOST_PP_ITERATION_PARAMS_1 (3, (1, GCL_MAX_FIELDS, <call_generic.h>))
#include BOOST_PP_ITERATE()

#else

#define noi BOOST_PP_ITERATION()

#define _PACK_F_NAME(x) m_pack ## x ##_generic_nv
#define PACK_F_NAME(x) _PACK_F_NAME(x)

#define _PACK_FILE_NAME(x) invoke_kernels_ ## x ## _PP.h
#define PACK_FILE_NAME(x) _PACK_FILE_NAME(x)


#define _print_FIELDS(z, m, s) (*filep) << "fieldx " << field ## m  << "\n" << sizeof(typename FOTF_T ## m::value_type) << std::endl;
#define print_FIELDS(m) BOOST_PP_REPEAT(m, _print_FIELDS, nil)


template < BOOST_PP_ENUM_PARAMS(noi, typename FOTF_T) >
void PACK_F_NAME(KERNEL_TYPE)( BOOST_PP_ENUM_BINARY_PARAMS(noi, FOTF_T, const &field) , 
                               void** d_msgbufTab, 
                               const int * d_msgsize)
{
  //print_FIELDS(noi);

#define QUOTE(x) #x
#define _QUOTE(x) QUOTE(x)
#include _QUOTE(PACK_FILE_NAME(KERNEL_TYPE))
#undef QUOTE
#undef _QUOTE
} 

#define _UNPACK_F_NAME(x) m_unpack ## x ##_generic_nv
#define UNPACK_F_NAME(x) _UNPACK_F_NAME(x)

#define _UNPACK_FILE_NAME(x) invoke_kernels_U_ ## x ## _PP.h
#define UNPACK_FILE_NAME(x)  _UNPACK_FILE_NAME(x)

#define MSTR(x) #x

template < BOOST_PP_ENUM_PARAMS(noi, typename FOTF_T) >
void UNPACK_F_NAME(KERNEL_TYPE)( BOOST_PP_ENUM_BINARY_PARAMS(noi, FOTF_T, const &field) , 
                                 void** d_msgbufTab_r, 
                                 int * d_msgsize_r)
{

#define QUOTE(x) #x
#define _QUOTE(x) QUOTE(x)
#include _QUOTE(UNPACK_FILE_NAME(KERNEL_TYPE))
#undef QUOTE
#undef _QUOTE

} 

#undef PACK_F_NAME
#undef _PACK_F_NAME
#undef PACK_FILE_NAME
#undef _PACK_FILE_NAME
#undef QUOTE_PACK_FILE_NAME
#undef UNPACK_F_NAME
#undef _UNPACK_F_NAME
#undef UNPACK_FILE_NAME
#undef _UNPACK_FILE_NAME
#undef QUOTE_UNPACK_FILE_NAME
#undef print_FIELDS
#undef _print_FIELDS
#undef noi

#endif
