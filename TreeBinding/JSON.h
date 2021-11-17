/*! 
 *  \file JSON.h
 *  \brief Macro definitions for JSON implementation
 */

#ifndef _JSON_H_
#define _JSON_H_

#include <boost/property_tree/json_parser.hpp>
#include "TreeBinding/TreeBinding.h"

namespace JSON
{

/*!
 *  \copydoc TreeBinding::NodesNum
 */
typedef TreeBinding::NodesNum ItemsNum;

/*!
 * \brief   JSON child declaration
 * \warning Each macro call should be placed in different lines
 * \param[in] ... 1. Child name. 
 * \param[in] ... 2. Child's data type
 * \param[in] ... 3. Field are optional/mandatory (mandatory by default).
 */
#define JSON_CHILD(...) TREE_NODE(__VA_ARGS__)

/*!
 * \brief   JSON array declaration
 * \warning Each macro call should be placed in different lines
 * \param   ... 1. Array name. 
 * \param   ... 2. Array's elements data type
 *              3. Required number of array elements (TreeBinding::NodesNum::MORE_THAN_ONE by default(if this parameter not passed)). 
 */
#define JSON_ARRAY(...) TREE_NODE_ARRAY(__VA_ARGS__)

/*!
 * \brief   JSON element declaration
 * \param   dataType Elements's data type
 */
#define JSON_ELEMENT(dataType) TREE_TREE(dataType)

} /* namespace JSON */

#endif /* _JSON_H_ */
