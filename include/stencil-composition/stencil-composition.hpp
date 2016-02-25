#pragma once

#include "intermediate_metafunctions.hpp"
#include "stencil-composition/esf.hpp"
#include "stencil-composition/make_esf.hpp"
#include "stencil-composition/make_stencils.hpp"
#include "stencil-composition/make_computation.hpp"
#include "reductions/make_reduction.hpp"
#include "stencil-composition/axis.hpp"

#ifndef STRUCTURED_GRIDS
    #include "stencil-composition/icosahedral_grids/grid.hpp"
#endif
