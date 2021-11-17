/*!
 *  \file  StringContainer.h
 *  \brief StringContainer class and macroses definitions
 */

#ifndef _STRINGCONTAINER_H_
#define _STRINGCONTAINER_H_

#include "TreeBinding/Details/Core/MacroUtils.h"

/*!
 * \brief     Build string container name
 * \details   Concatenate token "__StringContainer__" and suffix
 * \param[in] uniqSuffix Unique suffix
 */
#define TREE_BINDING_DETAILS_STRING_CONTAINER_NAME(uniqSuffix) \
    TREE_BINDING_DETAILS_CONCAT(__StringContainer__, uniqSuffix)

/*!
 * \brief     Unique string container
 * \details   template can't has string literal arguments, it's passed by it wrapper
 * \note      Not used template for string container and different specialization for it,
 *            because it's impossible to specialize container inside struct/class definition.
 * \param[in] str String, which will hold in container
 * \param[in] uniqSuffix Unique suffix
 */
#define TREE_BINDING_DETAILS_STRING_CONTAINER(str, uniqSuffix)  \
  struct TREE_BINDING_DETAILS_STRING_CONTAINER_NAME(uniqSuffix) \
  {                                                             \
    static const char* const getName()                          \
    {                                                           \
      return str;                                               \
    }                                                           \
  }

#endif /* _STRINGCONTAINER_H_ */
