/*! 
 *  \file TreeBinding.h
 *  \brief Interface file for boost tree and classes/structures binding
 */

#ifndef _TREE_BINDING_H_
#define _TREE_BINDING_H_

#include <string>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/serialization/access.hpp>
#include "TreeBindingDetails.h"
#include "ArchiveSerializer.h"

namespace TreeBinding
{

/*! 
 * \brief Declaration of function, which translate string value to target type
 */
struct Translator
{
  static bool isNumber(const std::string& s);

  /*! 
   *  \brief  Translate string value to target type
   *  \note   Not used direct get<T2> from boost tree, because it's necessary for forward instance of boost translator_between
   *  \tparam Target type
   *  \param  str   String representation of value
   *  \param  value Pointer to target value
   */
  template<typename T>
  static void fromString(std::string const &str, T* const value) throw(std::runtime_error);

  template<typename T>
  static std::string toString(const T* const value);

};

#define TREE_BINDING_TRANSLATOR_TO_STRING_STUB(type)   \
  template<>                                           \
  std::string Translator::toString(const type * const) \
  {                                                    \
    throw std::runtime_error                           \
      (TREE_BINDING_CONCAT("Conversion to string not implementeted for", #type)); \
  }

#define TREE_BINDING_TABLE_TRANSLATORS_DECLARATION(type, table)         \
template<>                                                              \
void Translator::fromString(std::string const &str,                     \
                            type* const value) throw(std::out_of_range) \
{                                                                       \
  *value = table.left.find(str)->second;                                \
}                                                                       \
template<>                                                              \
std::string Translator::toString(const type* const value)               \
{                                                                       \
  return table.right.find(*value)->second;                              \
}

/*!
 *  \brief   Type for store integer fields of Object
 */
typedef long Integer;

// Declared in private, because is necessary for forward declaration
struct NodesNum;

/*! 
 * \brief   Declaration of field which binds with tree node
 * \details When passed 2 arguments, expanded to call of TREE_BINDING_DETAILS_NODE_3 macro.
 *          When passed 3 arguments, expanded to call of TREE_BINDING_DETAILS_NODE_4 macro.
 * \warning Each macro call should be placed in different lines
 * \param   ... 1. Node name.
 *              2. Node's data type
 *              3. Node are optional/mandatory
 */
#define TREE_NODE(...) TREE_BINDING_DETAILS_NODE_COMMON(__VA_ARGS__)

typedef struct WrongChildsNumException : public std::runtime_error
{
  WrongChildsNumException(std::string const &objectName, NodesNum const requiredNum, int const actuallyNum);
} WrongChildsNumException;



typedef class BasicTree
{

protected:

  BasicTree() = delete;
  BasicTree(const char* const _name);
  Details::BasicNodeData& operator[](size_t const index) const;

  size_t            nodesNum; /*!< Number of fields in current tree */
  const char* const name;     /*!< Name of tree                     */

  struct NodeIterator;

  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
public:
 
  NodeIterator begin() const;
  NodeIterator end()   const;

  void parsePtree(boost::property_tree::ptree &tree, bool isRoot = true);
  void parseTable(std::vector<std::vector<std::wstring>> &table,
    std::function<size_t(std::string &const)> const &nameToIndex,
    std::pair<size_t, size_t> const &rows);

  void writePtree(boost::property_tree::ptree &tree, const bool isRoot = true) const;

  bool operator== (BasicTree const &rhs) const;
  virtual BasicTree& operator= (BasicTree const &rhs);
  bool isLeafsValid() const;
  void reset();
  const char* getKeyNodeName() const;

  bool isValid() const
  {
    return true;
  }

//  template<typename, typename> friend class Tree;
} BasicTree;


template<class Archive>
void BasicTree::serialize(Archive & ar, const unsigned int version)
{
  for (auto nodeIt = this->begin(); nodeIt != this->end(); ++nodeIt)
  {
    ar & (*nodeIt);
  }
}



struct BasicTree::NodeIterator
{
  NodeIterator() = default;
  NodeIterator(const NodeIterator&) = default;
  NodeIterator& operator=(const NodeIterator&) = default;

  bool operator!=(const NodeIterator&) const;
  NodeIterator& operator+(int const index);
  NodeIterator& operator++();
  Details::BasicNodeData& operator*() const;
  Details::BasicNodeData* operator->() const;

  Details::BasicNodeData* ptr;
};

template<typename NameContainer, typename T>
struct Tree : public BasicTree
{
  Tree();
  typedef NameContainer NameContainer_;

//private:
//  friend class boost::serialization::access;
};

/*!
 * \brief  Define structure of tree
 * \tparam name Name of tree (in file)
 * \tparam type Name of this type (in code)
 */
#define TREE_TREE(name, type)                  \
  TREE_BINDING_DETAILS_STRING_CONTAINER(name); \
  struct type : public TreeBinding::Tree < TREE_BINDING_DETAILS_STRING_CONTAINER_NAME, type >

} /* namespace TreeBinding */

// Function definitions file
#include "TreeBindingDefs.h"

#endif /* _TREE_BINDING_H_ */
