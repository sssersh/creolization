/*!
 * \brief Test XML parsing
 */


// Test framework
#include "gtest/gtest.h"

// Tested file
#include "creolization.h"

#include <map>
#include <boost/property_tree/xml_parser.hpp>

/*!
 * \brief Enum type
 */
enum class EnumType
{
    ENUM1,
    ENUM2,
    ENUM3
};

namespace creolization
{

/*!
 * \brief \copydoc data_binding::Translator::fromString(std::string const &str,EnumType *value)
 */
template<>
EnumType Translator::fromString<EnumType>(std::string const &str)
{
    static const std::map<std::string, EnumType> stringToEnum =
    {
        {"ENUM1", EnumType::ENUM1},
        {"ENUM2", EnumType::ENUM2},
        {"ENUM3", EnumType::ENUM3}
    };

    return stringToEnum.at(str);
}

/*!
 * \brief \copydoc std::string data_binding::Translator::toString(const EnumType* const value)
 */
template<>
std::string Translator::toString(const EnumType& value)
{
    static const std::map<EnumType, std::string> enumToString =
    {
        {EnumType::ENUM1, "ENUM1"},
        {EnumType::ENUM2, "ENUM2"},
        {EnumType::ENUM3, "ENUM3"}
    };

    return enumToString.at(value);
}

} // namespace data_binding

/*!
 * \brief Most nested element
 */
XML_ELEMENT(MostNestedXmlElement, "MostNestedElement")
{
  XML_ATTR("StringAttrName" , std::string                           ) strAttr ; /*!< String attribute  */
  XML_ATTR("IntegerAttrName", int                                   ) intAttr ; /*!< Integer attribute */
  XML_ATTR("EnumAttrName"   , EnumType,  XML::ItemNum::NOT_SPECIFIED) enumAttr; /*!< Enum attribute (optional) */
};

/*!
 * \brief Intermediate element
 */
XML_ELEMENT(NestedXmlElement, "NestedElement")
{
    XML_ATTR("IntegerAttrName", int)     intAttr; /*!< Integer attribute */
    XML_CHILD_ELEMENTS(MostNestedXmlElement, 2) childs; /*!< Child element (must contain 2 childs elements) */
};

/*!
 * \brief Root element
 */
XML_ELEMENT(RootXmlElement, "RootElement")
{
    XML_ATTR("StringAttrName", std::string) strAttr; /*!< String attribute */
    XML_CHILD_ELEMENTS(NestedXmlElement, XML::ItemNum::MORE_THAN_0) childs; /*!< Child element (must contain more than 0 elements) */
};

static const std::string CORRECT_XML_DATA =
R"(
<RootElement StringAttrName="StringValue">
    <NestedElement IntegerAttrName="22">
        <MostNestedElement StringAttrName="StringValue1" IntegerAttrName="1"  EnumAttrName="ENUM2"/>
        <MostNestedElement StringAttrName="StringValue5" IntegerAttrName="11"/>
    </NestedElement>
</RootElement>
)";

TEST(common, test_xml) {
    RootXmlElement rootXmlElement;

    boost::property_tree::ptree propertyTree;
    std::istringstream str(CORRECT_XML_DATA);

    boost::property_tree::read_xml(str, propertyTree);

    rootXmlElement.parsePtree(propertyTree);

    ASSERT_EQ(*rootXmlElement.strAttr, "StringValue");
    ASSERT_EQ(rootXmlElement.childs->size(), 1ul );

    auto& nested = rootXmlElement.childs->at(0);
    ASSERT_EQ(*nested.intAttr, 22);
    ASSERT_EQ(nested.childs->size(), 2ul);

    auto& mostNested0 = nested.childs->at(0);
    auto& mostNested1 = nested.childs->at(1);
    ASSERT_EQ(*mostNested0.strAttr , "StringValue1");
    ASSERT_EQ(*mostNested0.intAttr , 1);
    ASSERT_EQ(*mostNested0.enumAttr, EnumType::ENUM2);

    ASSERT_EQ(*mostNested1.strAttr , "StringValue5");
    ASSERT_EQ(*mostNested1.intAttr , 11);
    ASSERT_EQ( mostNested1.enumAttr.validity, false);
}
