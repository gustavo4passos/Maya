#ifndef TMXHELPER_H
#define THXHELPER_H

#include <string>
#include <tinyxml2.h>
#include <typeinfo>
#include <vector>
#include "Logger.h"

class TMXHelper 
{
public:
    // Stores all child elements inside element inside the elements vector
    static void GetAllChildElements(
        tinyxml2::XMLElement* element, 
        std::vector<tinyxml2::XMLElement*>* elements
    );

    // Returns first child element with the specified name, or
    // nullptr if no child with such name can be found
    static tinyxml2::XMLElement* GetChildElementByName(
        tinyxml2::XMLElement* element,
        const std::string& name
    );

    // Stores all child elements with a certain name inside the elements vector
    static void GetChildElementsByName(
        tinyxml2::XMLElement* element, 
        const std::string& name, 
        std::vector<tinyxml2::XMLElement*>* elements
    );

    // Stores the value of propertyName property in value
    // Returns if the property is succesfully found and has the correct type
    // If the property can't be found, or if it has a different type, than T
    // value is kept unchanged and false is returned.
    template<typename T>
    static bool GetProperty(
        tinyxml2::XMLElement* element, 
        const std::string& propertyName, 
        T value
    );

    // Returns the first child element where the attribute
    // has a certain value.
    // Returns nullptr if no element satisfies the condition
    static tinyxml2::XMLElement* GetChildElementByAttribute(
        tinyxml2::XMLElement* element,
        const std::string& attribute,
        const std::string& equalTo
    );

    // TODO(Gustavo): Should this be here or in LevelFile?
    // static int GetObjectID(tinyxml2::XMLElement* objectElement);


private:
    static bool CheckPointerValidity(tinyxml2::XMLElement* element);
    static bool CheckPointerValidity(
        tinyxml2::XMLElement* element, 
        std::vector<tinyxml2::XMLElement*>* elements
    );

    static tinyxml2::XMLElement* GetPropertyElementByName(
        tinyxml2::XMLElement* element,
        const std::string& propertyName
    );

};

template<typename T>
bool TMXHelper::GetProperty(
        tinyxml2::XMLElement* element, 
        const std::string& propertyName, 
        T value)
{

    tinyxml2::XMLElement* propertyElement = GetPropertyElementByName(element, propertyName);
    if(propertyElement == nullptr) return false;

    tinyxml2::XMLError error = (tinyxml2::XMLError)propertyElement->QueryAttribute("value", value);
    if(error == tinyxml2::XML_WRONG_ATTRIBUTE_TYPE) return false;
    return true;
}

template<>
bool TMXHelper::GetProperty(
    tinyxml2::XMLElement* element,
    const std::string& propertyName,
    std::string* value);

#endif