#include "../include/TMXHelper.h"

#include <tinyxml2.h>
#include "../include/Logger.h"

void TMXHelper::GetAllChildElements(
    tinyxml2::XMLElement* element, 
    std::vector<tinyxml2::XMLElement*>* elements)
{
    if(!CheckPointerValidity(element, elements)) return; 

    for(tinyxml2::XMLElement* e = element->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
    {
        elements->push_back(e);
    }
}

 tinyxml2::XMLElement* TMXHelper::GetChildElementByName(
        tinyxml2::XMLElement* element,
        const std::string& name)
{
    if(!CheckPointerValidity(element)) return nullptr;

    for(auto e = element->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
    {
        if(std::string(e->Name()) == name) return e;
    }

    return nullptr;
}

void TMXHelper::GetChildElementsByName(
        tinyxml2::XMLElement* element, 
        const std::string& name, 
        std::vector<tinyxml2::XMLElement*>* elements)
{
    if(!CheckPointerValidity(element, elements)) return; 

    for(auto e = element->FirstChildElement(); 
        e != nullptr; 
        e = e->NextSiblingElement())
    {
        if(std::string(e->Name()) == name) elements->push_back(e);
    }
}


template<>
bool TMXHelper::GetProperty(
    tinyxml2::XMLElement* element, 
    const std::string& propertyName,
    std::string* value)
{
    tinyxml2::XMLElement* propertyElement = GetPropertyElementByName(element, propertyName);
    if(propertyElement == nullptr) return false;
    std::string result = propertyElement->Attribute("value");

    *value = result;

    return true;
}

template<>
bool TMXHelper::SetProperty(
    tinyxml2::XMLElement* element,
    const std::string& propertyName,
    const std::string& value)
{
    if(!CheckPointerValidity(element)) return false;

    tinyxml2::XMLElement* propertyElement = GetPropertyElementByName(element, propertyName);
    if(propertyElement == nullptr) return false;
    propertyElement->SetAttribute("value", value.c_str());
    return true;
}

tinyxml2::XMLElement* TMXHelper::GetChildElementByAttribute(
        tinyxml2::XMLElement* element,
        const std::string& attribute,
        const std::string& equalTo
    )
{
    if(!CheckPointerValidity(element)) return nullptr;

    for(auto e = element->FirstChildElement(); 
        e != nullptr; e =
        e->NextSiblingElement())
    {
        if(std::string(e->Attribute(attribute.c_str())) == equalTo) return e;
    }

    return nullptr;
}

tinyxml2::XMLElement* TMXHelper::GetPropertyElementByName(
        tinyxml2::XMLElement* element,
        const std::string& propertyName)
{
    if(!CheckPointerValidity(element)) return nullptr;

    tinyxml2::XMLElement* propertiesRoot = GetChildElementByName(element, "properties");
    if(propertiesRoot == nullptr) 
    {
        // LOG_ERROR("Unable to get property \"" + propertyName + "\". Element has no properties." );
        return nullptr;
    }

    tinyxml2::XMLElement* propertyElement = GetChildElementByAttribute(propertiesRoot, "name", propertyName);
    if(propertyElement == nullptr)
    {
        // LOG_ERROR("Property " + propertyName + " has not been found in element.");
        return nullptr;
    }

    if(propertyElement->Attribute("value") == nullptr)
    {
        LOG_ERROR("Property " + propertyName + " is missing value.");
        return nullptr;
    }

    return propertyElement;
}

const std::string TMXHelper::GetElementName(tinyxml2::XMLElement* element)
{
    if(!CheckPointerValidity(element)) return std::string();

    return std::string(element->Name());
}
// TODO(Gustavo): Should this be here or in LevelFile?
// int TMXHelper::GetObjectID(tinyxml2::XMLElement* objectElement)
// {
//     if(CheckPointerValidity(objectElement)) return -1;

//     int id;
//     if(objectElement->Attribute("id") == nullptr) return -1;
//     objectElement->QueryIntAttribute("id", &id);
//     return id;
// }


bool TMXHelper::CheckPointerValidity(tinyxml2::XMLElement* element)
{
    if(element == nullptr) 
    {
        LOG_ERROR("Unable to access element. Element is nullptr.");
        return false;
    }

    return true;
}

bool TMXHelper::CheckPointerValidity(tinyxml2::XMLElement* element, 
        std::vector<tinyxml2::XMLElement*>* elements)
{
    if(!CheckPointerValidity(element)) return false;

    if(elements == nullptr)
    {
        LOG_ERROR("Unable to get child nodes. elements vector is nullptr.")
        return false;
    }

    return true;
}