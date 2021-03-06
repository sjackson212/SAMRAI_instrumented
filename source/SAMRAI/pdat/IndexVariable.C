/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright
 * information, see COPYRIGHT and COPYING.LESSER.
 *
 * Copyright:     (c) 1997-2015 Lawrence Livermore National Security, LLC
 * Description:   IndexVariable implementation
 *
 ************************************************************************/

#ifndef included_pdat_IndexVariable_C
#define included_pdat_IndexVariable_C

#include "SAMRAI/pdat/IndexVariable.h"
#include "SAMRAI/pdat/IndexDataFactory.h"

#include "SAMRAI/tbox/Utilities.h"

#include "boost/make_shared.hpp"

namespace SAMRAI {
namespace pdat {

/*
 *************************************************************************
 *
 * Constructor and destructor for irregular index variable objects
 *
 *************************************************************************
 */

template<class TYPE, class BOX_GEOMETRY>
IndexVariable<TYPE, BOX_GEOMETRY>::IndexVariable(
   const tbox::Dimension& dim,
   const std::string& name):
   // default zero ghost cells
   hier::Variable(
      name,
      boost::make_shared<IndexDataFactory<TYPE, BOX_GEOMETRY> >(
         hier::IntVector::getZero(dim)))
{
}

template<class TYPE, class BOX_GEOMETRY>
IndexVariable<TYPE, BOX_GEOMETRY>::~IndexVariable()
{
}

}
}
#endif
