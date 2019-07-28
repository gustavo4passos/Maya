#include "../include/LevelLoader.h"

#include <algorithm>

#include "../include/Button.h"
#include "../include/Door.h"
#include "../include/Logger.h"
#include "../include/Golem.h"
#include "../include/Layer.h"
#include "../include/Level.h"
#include "../include/LevelFile.h"
#include "../include/MovingPlatform.h"
#include "../include/PlatformSwitch.h"
#include "../include/PushableObject.h"
#include "../include/Region.h"
#include "../include/RepeatingLayer.h"
#include "../include/ResourceManager.h"
#include "../include/TeleportZone.h"

Level* LevelLoader::ParseLevel(const std::string& filename){
    // create the XML document
	TiXmlDocument xmlDoc;

	// load the XML document
	if (!xmlDoc.LoadFile(filename)) {
        LOG_ERROR("Unable to open level file \"" + filename + "\" - " + std::string(xmlDoc.ErrorDesc()));
		return nullptr;
	}

	// get the root element
	TiXmlElement* pRoot = xmlDoc.RootElement();

	LevelFile* levelFile = nullptr;
	levelFile = new LevelFile(filename);
	if(levelFile == nullptr) 
	{
		LOG_ERROR("Unable to instatiate levelFile. Filename: " + filename);
		return nullptr;
	}

	if(!levelFile->OpenFile()) return nullptr;
	
	std::vector<int> tileLayer; // Will contain the tile values

	Level* level = nullptr;
	Layer* layer = nullptr;
	Tileset* tileset = nullptr;

	int width, height, tileWidth, tileHeight;

	if(levelFile->rootElement()->QueryIntAttribute("width", &width) != tinyxml2::XML_SUCCESS)
	{
		LOG_ERROR("Unable to read level width in file \"" + filename + "\".");
		return nullptr;
	}
	if(levelFile->rootElement()->QueryIntAttribute("height", &height) != tinyxml2::XML_SUCCESS)
	{
		LOG_ERROR("Unable to read level height in file \"" + filename + "\".");
		return nullptr;
	}
	if(levelFile->rootElement()->QueryIntAttribute("tilewidth", &tileWidth) != tinyxml2::XML_SUCCESS)
	{
		LOG_ERROR("Unable to read level tile width in file \"" + filename + "\".");
		return nullptr;
	}
	if(levelFile->rootElement()->QueryIntAttribute("tileheight", &tileHeight) != tinyxml2::XML_SUCCESS)
	{
		LOG_ERROR("Unable to read level tile height in file \"" + filename + "\".");
		return nullptr;
	}

	tileset = ParseTileset(levelFile->tilesetElement());
	if(tileset == nullptr)
	{
		LOG_ERROR("Unable to load tileset in level file \"" + filename + "\".");
		return nullptr;
	}

	level = new Level(tileset, width, height, tileWidth, tileHeight, filename);

    for(auto layerElement = levelFile->layerElements().begin();
        layerElement != levelFile->layerElements().end();
        layerElement++)
	{
        if(TMXHelper::GetElementName(*layerElement) == "layer")
        {
		    layer = ParseLayer(*layerElement, level, tileset);
        }
        else if(TMXHelper::GetElementName(*layerElement) == "imagelayer")
        {
            layer = ParseRepeatingLayer(*layerElement, level);
        }
        else
        {
            LOG_WARNING("Unknown layer type in level \"" + level->filename() << "\". Ignoring...");
            continue;
        }

		if(layer == nullptr){
			  	LOG_ERROR("Unable to parse layer " + std::string((*layerElement)->Attribute("name")));
				delete level;
				return nullptr;
		}

		if(!layer->IsForeground())
		{
			level->AddBackgroundLayer(layer);
		}
		else
		{
			level->AddForegroundLayer(layer);
		}
	}

	for(const auto& enemyElement : levelFile->enemiesElements())
	{
		ParseEnemy(enemyElement, level);
	}

	for(const auto& collisionRectElement : levelFile->collisionRectsElements())
	{
		ParseCollisionRect(collisionRectElement, level);
	}

	for(const auto& zoneElement : levelFile->zonesElements())
	{
		ParseZone(zoneElement, level);
	}

	TiXmlElement* e = nullptr;
    
	for(e = pRoot->FirstChildElement(); e!=nullptr; e = e->NextSiblingElement()){
		if(e->Value() == std::string("objectgroup")){
			ParseObjectGroup(e, level);
		}
	}

    return level;
}

Tileset* LevelLoader::ParseTileset(const tinyxml2::XMLElement* tilesetElement){
	using namespace tinyxml2;
	const XMLElement* imageElement= tilesetElement->FirstChildElement();
	// Checks if image node is absent
	if(imageElement == nullptr || imageElement->Value() != std::string("image")){
	  	LOG_ERROR("Tileset element is missing image field.");
		return nullptr;
	}

    std::string source;
    std::string name;

    int width, height;
	int tileWidth, tileHeight;
    int margin, spacing;
	int nColumns, nRows;

    name = std::string(tilesetElement->Attribute("name"));
    if(tilesetElement->QueryIntAttribute("tilewidth", &tileWidth) != XML_SUCCESS)
	{
		LOG_ERROR("Tileset element missing tile width field.");
		return nullptr;		
	}
    if(tilesetElement->QueryIntAttribute("tileheight", &tileHeight) != XML_SUCCESS)
	{
		LOG_ERROR("Tileset element missing tile height field.");
		return nullptr;		
	}
    if(tilesetElement->QueryIntAttribute("columns", &nColumns) != XML_SUCCESS)
	{
		LOG_ERROR("Tileset element missing tile columns field.");
		return nullptr;		
	}

	// Checks if margin and spacing fields are present
	tinyxml2::XMLError marginFieldCheck;
	tinyxml2::XMLError spacingFieldCheck;
    marginFieldCheck = tilesetElement->QueryIntAttribute("margin", &margin);
    spacingFieldCheck = tilesetElement->QueryIntAttribute("spacing", &spacing);

	// If margin and/or spacing fields are absent, set them to 0
	if(marginFieldCheck != tinyxml2::XML_SUCCESS) { margin = 0; }
	if(spacingFieldCheck != tinyxml2::XML_SUCCESS) { spacing = 0; }

	if(imageElement->Attribute("source") == 0)
	{
		LOG_ERROR("Tileset image source is missing.");
		return nullptr;
	}
    source = std::string(imageElement->Attribute("source"));

    if(imageElement->QueryIntAttribute("width", &width) != XML_SUCCESS)
	{
		LOG_ERROR("Tileset image missing width.");
		return nullptr;
	}
    if(imageElement->QueryIntAttribute("height", &height) != XML_SUCCESS)
	{
		LOG_ERROR("Tileset image missing height.");
		return nullptr;
	}
    nRows = (height - 2 * margin + spacing) / (tileHeight + spacing);

	// Tries to load tileset texture to video memory
	if(!ResourceManager::LoadTexture(source, name)) {
		LOG_ERROR("Unable to load tileset texture. Filename: " + source);
		return nullptr;
	}

    return new Tileset(source, name, width, height, tileWidth, tileHeight, margin, spacing, nColumns, nRows);

}

void LevelLoader::ParseObjectGroup(TiXmlElement* objectsNode, Level* level){
	int id;
	float x, y;
    std::string test;

	if(objectsNode->Attribute("name") == nullptr) {
		LOG_WARNING("Objects layer missing name field. Ignoring.");
		return;
	}

    test = std::string(objectsNode->Attribute("name"));

	if(test == std::string("GameObjects")){
		for(TiXmlElement* e = objectsNode->FirstChildElement(); e!=nullptr; e = e->NextSiblingElement()){
			if(e->Value() == std::string("object")){
				std::string temp;

				try {
					temp = std::string(e->Attribute("type"));
				} catch(std::logic_error e) {
					LOG_WARNING("Unable to load GameObject. Object missing type field: " + std::string(e.what()));
					return;
				}

				if(temp == std::string("door")){
					std::string switchesRequired;
					bool initialState;

					e->QueryFloatAttribute("x", &x);
					e->QueryFloatAttribute("y", &y);
					TiXmlElement* propertiesNode = e->FirstChildElement();
					if(std::string(propertiesNode->Value()) != std::string("properties"))
						continue;
					else{
						TiXmlElement* initiallyOpen = GetProperty(propertiesNode, "initiallyOpen");
						if(initiallyOpen == nullptr){
							LOG_WARNING("Door's inittialyOpen property is missing. Door not loaded");
							continue;
						}
						initiallyOpen->QueryBoolAttribute("value", &initialState);
						TiXmlElement* switchesRequiredProperty = GetProperty(propertiesNode, "switchesRequired");
						if(switchesRequiredProperty == nullptr){
							LOG_WARNING("Door's switchesRequired property is missing, Door not loaded");
							continue;
						}
						switchesRequired = std::string(switchesRequiredProperty->Attribute("value"));
						level->AddGameObject(new Door(CollisionRect(Rect(x, y, 32, 32)), 32, 32, switchesRequired, initialState));
					}

				}
				else if(temp == std::string("button")){
					std::string activatesSwitch;

					e->QueryFloatAttribute("x", &x);
					e->QueryFloatAttribute("y", &y);
					e->QueryIntAttribute("id", &id);

					TiXmlElement* propertiesNode = e->FirstChildElement();
					if(propertiesNode == nullptr) {
						LOG_WARNING("Button missing properties field. Ignoring...");
						continue;
					}
					if(std::string(propertiesNode->Value()) != std::string("properties"))
						continue;
					else{
						TiXmlElement* activatesSwitchNode = GetProperty(propertiesNode, "activatesSwitch");
						if(activatesSwitchNode == nullptr){
							LOG_WARNING("Button's activatesSwitch is missing, button not loaded");
							continue;
						}

						activatesSwitch = std::string(activatesSwitchNode->Attribute("value"));
						level->AddGameObject(new Button(x, y, activatesSwitch, false));
					}
				}
				else if(temp == std::string("platformSwitch")){
					std::string activatesSwitch;

					e->QueryFloatAttribute("x", &x);
					e->QueryFloatAttribute("y", &y);
					TiXmlElement* propertiesNode = e->FirstChildElement();

					if(std::string(propertiesNode->Value()) != std::string("properties"))
						continue;
					else{
						TiXmlElement* activatesSwitchNode = GetProperty(propertiesNode, "activatesSwitch");
						if(activatesSwitchNode == nullptr){
							LOG_WARNING("Button's activeatesSwitch is missing, button not loaded");
							continue;
						}

						activatesSwitch = std::string(activatesSwitchNode->Attribute("value"));
						level->AddGameObject(new PlatformSwitch(x, y, activatesSwitch));
					}
				}
				else if(temp == std::string("movingPlatform")){
					float destinationX, destinationY;
					std::string requiresSwitch = "";

					e->QueryFloatAttribute("x", &x);
					e->QueryFloatAttribute("y", &y);
					TiXmlElement* propertiesNode = e->FirstChildElement();
					if(std::string(propertiesNode->Value()) != std::string("properties"))
						continue;
					else{

						TiXmlElement* destinationXProperty = GetProperty(propertiesNode, "destinationX");
						if(destinationXProperty == nullptr){
							LOG_WARNING("MovingPlatform destination x is missing, MovingPlatform not loaded");
							continue;
						}

						destinationXProperty->QueryFloatAttribute("value", &destinationX);


						TiXmlElement* destinationYProperty = GetProperty(propertiesNode, "destinationY");
						if(destinationYProperty == nullptr) {
							LOG_WARNING("MovingPlatform destination y is missing. Moving platform not loaded");
							continue;
						}
						destinationYProperty->QueryFloatAttribute("value", &destinationY);

						TiXmlElement* requiresSwitchProperty = GetProperty(propertiesNode, "requiresSwitch");
						if(requiresSwitchProperty != nullptr && (requiresSwitchProperty->Attribute("value") != nullptr)) {
							requiresSwitch = std::string(requiresSwitchProperty->Attribute("value"));
 						}

						level->AddGameObject(new MovingPlatform(Vector2D(x, y), Vector2D(destinationX, destinationY), true, requiresSwitch));
					}
				}
				else if(temp == std::string("pushableObject")) {
					e->QueryFloatAttribute("x", &x);
					e->QueryFloatAttribute("y", &y);
					level->AddGameObject(new PushableObject(x, y));
				}
			}
		}
	}
	else 
	{
		LOG_WARNING("Unknown layer type. Name: " + test + " ignoring.");
		return;
	}
}

CollisionRect* LevelLoader::ParseRect(tinyxml2::XMLElement* collisionRectElement)
{
	using namespace tinyxml2;

	int id;
	int x, y;
	int width, height;

	XMLError idRetrievalStatus = collisionRectElement->QueryIntAttribute("id", &id);
	if(idRetrievalStatus != XMLError::XML_SUCCESS)
	{
		LOG_WARNING("Unable to retrieve ID from collision rect element.");
		return nullptr;
	}

	if(collisionRectElement->QueryIntAttribute("x", &x) != XMLError::XML_SUCCESS){
		LOG_ERROR("X field missing in object from objectgroup. Id: " + std::to_string(id));
		return nullptr;
	}

	if(collisionRectElement->QueryIntAttribute("y", &y) != XMLError::XML_SUCCESS){
		LOG_ERROR("Y field missing in object from objectgroup. Id: " + id);
		return nullptr;
	}
	if(collisionRectElement->QueryIntAttribute("width", &width) != XMLError::XML_SUCCESS){
		LOG_ERROR("Width field missing in object from objectgroup. Id: " + id);
		return nullptr;
	}
	if(collisionRectElement->QueryIntAttribute("height", &height) != XMLError::XML_SUCCESS){
		LOG_ERROR("Height field missing in object from objectgroup. Id: " + id);
		return nullptr;
	}

	std::string collisionBehavior = "";
	if(!TMXHelper::GetProperty(collisionRectElement, "collisionbehavior", &collisionBehavior))
	{
		return new CollisionRect(Rect(x, y, width, height), CollisionBehavior::BLOCK);
	}
	else
	{
			if(collisionBehavior == "oneway")
			{
				return new CollisionRect(Rect(x, y, width, height), CollisionBehavior::ONE_WAY);
			}
			else if(collisionBehavior == "block")
			{
				return new CollisionRect(Rect(x, y, width, height), CollisionBehavior::BLOCK);
			}
			else
			{
				LOG_WARNING("Invalid CollisionBehavior for CollisionRect in CollisionLayer. Seting Behavior as BLOCK");
				return new CollisionRect(Rect(x, y, width, height), CollisionBehavior::BLOCK);
			}
	}

	LOG_WARNING("CollisionRect in CollisionLayer missing property field. Setting it as BLOCK.");
	return new CollisionRect(Rect(x, y, width, height), CollisionBehavior::BLOCK);
}	

Layer* LevelLoader::ParseLayer(tinyxml2::XMLElement* layerElement, Level* level, Tileset* tileset){
    if(layerElement == nullptr){
        LOG_ERROR("layerElement is nullptr.");
        DEBUG_BREAK();
		return nullptr;
    }

	// Query name, width and height info
	std::string name;
	std::vector<int> layerData;
	int width, height;
	bool isForeground = false;
	
	// Append the level filename to the layer name, to avoid duplicate
	// entries in the mesh map in case two layers from different levels
	// have the same name and needs to be in memory simultaneously.
	name = level->filename() + std::string(layerElement->Attribute("name"));

	if(layerElement->QueryIntAttribute("width", &width) != tinyxml2::XML_SUCCESS)
	{
		LOG_ERROR("Layer missing width attribute. Layer \"" + 
			std::string(layerElement->Attribute("name")) + "\" in level \"" + 
			level->filename() + "\".");
		return nullptr;
	}
	if(layerElement->QueryIntAttribute("height", &height) != tinyxml2::XML_SUCCESS)
	{
		LOG_ERROR("Layer missing height attribute. Layer \"" + 
			std::string(layerElement->Attribute("name")) + "\" in level \"" + 
			level->filename() + "\".");
		return nullptr;
	}

	// Read layer properties data
	double zDistance = 1; // Sets zDistance to the default value, in case it isn't specified
	TMXHelper::GetProperty(layerElement, "zdistance", &zDistance);
	TMXHelper::GetProperty(layerElement, "foreground", &isForeground);

	tinyxml2::XMLElement* dataElement = TMXHelper::GetChildElementByName(layerElement, "data");
	if(dataElement == nullptr)
	{
		LOG_ERROR("Unable to load layer: layer missing data element. Layer \"" + 
			name + "\" in level \"" + level->filename());
			return nullptr;
	}

	layerData = ParseLayerData(dataElement);
	LoadLayerMesh(layerData, level, tileset, name);

	return new Layer(name, width, height, tileset->name(), zDistance, isForeground);
}

Layer* LevelLoader::ParseRepeatingLayer(tinyxml2::XMLElement* repeatingLayerNode, Level* level) {
	if(repeatingLayerNode == nullptr) {
		LOG_WARNING("Unable to parse repeating layer. Layer node is null");
		return nullptr;
	}

    tinyxml2::XMLElement* imageElement = TMXHelper::GetChildElementByName(repeatingLayerNode, "image");
    if(imageElement == nullptr) 
    {
        LOG_WARNING("Unable to find image node in repeating layer. Layer won't be loaded.");
		return nullptr;
    }

	// Query layer info
	std::string name;
	float offsetX, offsetY;
	float zDistance = 1.f;
	int width, height;

	if(repeatingLayerNode->Attribute("name") == 0) {
		LOG_WARNING("Unable to read repeating layer name in level: " + level->filename() + ". Layer won't be loaded.");
		return nullptr;
	}
	else {
		name = std::string(repeatingLayerNode->Attribute("name"));
	}
	if(repeatingLayerNode->QueryFloatAttribute("offsetx", &offsetX) != tinyxml2::XML_SUCCESS) {
		LOG_WARNING("Unable to read offsetx from repeating layer. Layer name: " + name + ". Layer won't be loaded.");
		return nullptr;
	}
	if(repeatingLayerNode->QueryFloatAttribute("offsety", &offsetY) != tinyxml2::XML_SUCCESS) {
		LOG_WARNING("Unable to read offsety from repeating layer. Layer name: " + name + ". Layer won't be loaded.");
		return nullptr;
	}
	if(imageElement->QueryIntAttribute("width", &width) != tinyxml2::XML_SUCCESS) {
		LOG_WARNING("Unable to read width from repeating layer. Layer name: " + name + ". Layer won't be loaded.");
		return nullptr;
	}
	if(imageElement->QueryIntAttribute("height", &height) != tinyxml2::XML_SUCCESS) {
		LOG_WARNING("Unable to read height from repeating layer. Layer name: " + name + ". Layer won't be loaded.");
		return nullptr;
	}

    if(!TMXHelper::GetProperty(repeatingLayerNode, "zdistance", &zDistance))
    {
        LOG_WARNING("Unable to read zdistance from repeating layer. Layer name \"" + name + 
            "\" in level \"" + level->filename() + "\". Setting it to default (1.0).");
    }
	return new RepeatingLayer(name, width, height, Vector2D(offsetX, offsetY), zDistance);
}

std::vector<int> LevelLoader::ParseLayerData(tinyxml2::XMLElement* dataElement){
    std::string t;
    std::vector<int> layerdata;

	// Finding the text in xml and attributing it to t
	if (dataElement != nullptr) {
		for (auto e = dataElement->FirstChild(); e != nullptr; e = e->NextSibling()) {
			tinyxml2::XMLText* text = e->ToText();
			t = text->Value();
		}
	} else {
        LOG_ERROR("dataElement is nullptr");
        DEBUG_BREAK();
    }

	for(unsigned int i=0; i<t.size(); i++){
		std::string aux;

		if(t[i] != ',' || t[i] != ' '){
			while(t[i] != ',' && i < t.size()){
				if(t[i] == ' '){
					i++;
					continue;
				}
				aux += t[i];
				i += 1;
			}
			try {
				layerdata.push_back(std::stoi(aux));
			}
			catch(const std::out_of_range& e) {
				LOG_ERROR("Unable to parse tile: id is out of range for an int. Exception thrown: " + std::string(e.what()));
			}
		}
	}

	return layerdata;
}

bool LevelLoader::ParseEnemy(tinyxml2::XMLElement* enemyElement, Level* level)
{
	int width, height;
	std::string enemyType;
	std::string enemySwitch;

	if(enemyElement->Value() == std::string("object"))
	{
		float x, y;

		enemyElement->QueryFloatAttribute("x", &x);
		enemyElement->QueryFloatAttribute("y", &y);
		enemyElement->QueryIntAttribute("width", &width);
		enemyElement->QueryIntAttribute("height", &height);

		// Tries to retrieve enemy type
		if(!TMXHelper::GetProperty(enemyElement, "type", &enemyType))
		{
			LOG_WARNING("Enemy in level: " + level->filename() + " is missing type. Ignoring.");
			return false;
		}

		// Tries to retrieve switch from enemy (optional)
		if(TMXHelper::GetProperty(enemyElement, "switch", &enemySwitch))
		{
			level->AddEnemy(new Golem(x, y, enemySwitch));
		}
		else 
		{
			level->AddEnemy(new Golem(x, y));
		}

		return true;
	}
	else
	{
		LOG_WARNING("Unable to parse enemy: Enemy element is not an object. File: " + level->filename());
		return false;
	}
}

bool LevelLoader::ParseCollisionRect(tinyxml2::XMLElement* collisionRectElement, Level* level)
{
	if(collisionRectElement->Value() == std::string("object"))
	{
		if(CollisionRect* rct = ParseRect(collisionRectElement))
		{
			level->AddCollisionRect(rct);
			return true;
		} 
		else return false;
	}
	else
	{
		LOG_WARNING("Unable to load collision rect in level: " + level->filename() + 
			"collisionRectElement is not an object");
			return false;
	}
}

bool LevelLoader::ParseZone(tinyxml2::XMLElement* zoneElement, Level* level)
{
	using namespace tinyxml2;

	float x, y;
	int width, height;

	if(zoneElement->Value() != std::string("object"))
	{
		LOG_WARNING("Unable to parse zone element in level: " + level->filename() +
			". Zone is not an object");
		return false;
	}
	
	zoneElement->QueryFloatAttribute("x", &x);
	zoneElement->QueryFloatAttribute("y", &y);
	zoneElement->QueryIntAttribute("width", &width);
	zoneElement->QueryIntAttribute("height", &height);

	// Checks if zone type is absent
	if(zoneElement->Attribute("type") == nullptr)
	{
		LOG_WARNING("Zone element in level: " + level->filename() + 
			" is missing type. Zone will be ignored.");
		return false;
	}

	std::string zoneType = zoneElement->Attribute("type");

	if(zoneType == std::string("teleportZone"))
	{
		std::string destinationLevel;
		float destinationX;
		float destinationY;

		if(!TMXHelper::GetProperty(zoneElement, "destinationLevel", &destinationLevel))
		{
			LOG_WARNING("Unable to read destination level from teleport zone in level: " + 
				level->filename() + ". Zone will be ignored.");
			return false;
		}

		if(!TMXHelper::GetProperty(zoneElement, "destinationX", &destinationX))
		{
			LOG_WARNING("Unable to read destinationX from teleport zone in level: " +
				level->filename() + ". Zone will be ignored.");
			return false;
 		}

		if(!TMXHelper::GetProperty(zoneElement, "destinationY", &destinationY))
		{
			LOG_WARNING("Unable to read destinationY from teleport zone in level: " +
				level->filename() + ". Zone will be ignored.");
			return false;
		}

		level->AddGameObject(new TeleportZone(Rect(x, y, width, height),
			destinationLevel, Vector2D(destinationX, destinationY)));

		return true;
	}
	else
	{
		LOG_WARNING("Unknown type of zone in level: " + level->filename() + 
			". Zone type: " + zoneType + ". Zone will be ignored.");
		return false;
	}
}

void LevelLoader::LoadLayerMesh(std::vector<int>& layerData, Level* level, Tileset* tileset, const std::string& name){
	int levelWidth;
	float tileWidth, tileHeight;
	int nColumns;
	int margin, spacing;
	levelWidth = level->width();
	tileWidth = (float)level->tileWidth();
	tileHeight = (float)level->tileHeight();
	nColumns = tileset->nColumns();
	margin = tileset->margin();
	spacing = tileset->spacing();
	std::vector<float> meshData;

	for(unsigned int i = 0; i < layerData.size(); i++){
		if(layerData[i] == 0) continue;

		int tileColumn, tileRow;
		int texColumn, texRow;

		tileColumn = i % levelWidth;
		tileRow = i / levelWidth;
		texColumn = (layerData[i] - 1) % nColumns;
		texRow = (layerData[i] - 1) / nColumns;

		struct Vertex { float x; float y; };
		struct UVcoordinate { float x; float y; };

		Vertex v1 = { tileColumn * tileWidth, tileRow * tileHeight };
		Vertex v2 = { tileColumn * tileWidth + tileWidth, tileRow * tileHeight };
		Vertex v3 = { tileColumn * tileWidth + tileWidth, tileRow * tileHeight + tileHeight };
		Vertex v4 = { tileColumn * tileWidth, tileRow * tileHeight + tileHeight };

		UVcoordinate tex1 = { texColumn * tileWidth + margin + spacing * texColumn, texRow * tileHeight + margin + spacing * texRow };
		UVcoordinate tex2 = { texColumn * tileWidth + tileWidth + spacing * texColumn, texRow * tileHeight + margin + spacing * texRow };
		UVcoordinate tex3 = { texColumn * tileWidth + tileWidth + spacing * texColumn, texRow * tileHeight + tileHeight + spacing * texRow};
		UVcoordinate tex4 = { texColumn * tileWidth + margin + spacing * texColumn, texRow * tileHeight + tileHeight + spacing * texRow };

		// NOTE(Gustavo): Should the UV coordinates be calculated on the CPU or GPU?
		// (Check mesh_shader.frag mat2 transform)

		// Stores vertices and tex coordinates in ram memory
		// Upper triangle
		meshData.push_back(v1.x); // Top left vertex
		meshData.push_back(v1.y);
		meshData.push_back(tex1.x); // Top left tex coordinate
		meshData.push_back(tex1.y);

		meshData.push_back(v2.x); // Top right vertex
		meshData.push_back(v2.y);
		meshData.push_back(tex2.x); // Top right vertex coordinate
		meshData.push_back(tex2.y);

		meshData.push_back(v3.x); // Bottom right vertex
		meshData.push_back(v3.y);
		meshData.push_back(tex3.x); // Bottom right tex coordinate
		meshData.push_back(tex3.y);

		// Lower triangle
		meshData.push_back(v1.x); // Top left vertex
		meshData.push_back(v1.y);
		meshData.push_back(tex1.x); // Top left tex coordinate
		meshData.push_back(tex1.y);

		meshData.push_back(v3.x); // Bottom right vertex
		meshData.push_back(v3.y);
		meshData.push_back(tex3.x); // Bottom right tex coordinate
		meshData.push_back(tex3.y);

		meshData.push_back(v4.x); // Bottom left vertex
		meshData.push_back(v4.y);
		meshData.push_back(tex4.x); // Bottom left tex coordinate
		meshData.push_back(tex4.y);
	}

	ResourceManager::LoadMesh((const void*)(&meshData[0]), meshData.size() * sizeof(float), meshData.size() / 4, name);
}

TiXmlElement* LevelLoader::GetProperty(TiXmlElement* propertiesNode, std::string propertyName){
	if(propertiesNode==nullptr){
		LOG_ERROR("PropertiesNode is nullptr");
		return nullptr;
	} else {
		for(TiXmlElement* e = propertiesNode->FirstChildElement(); e!=nullptr; e = e->NextSiblingElement()){
			if(std::string(e->Attribute("name")) == propertyName){
				return e;
			}
		}
	}
	return nullptr; //Property not found.
}

Region* LevelLoader::ParseRegion(const std::string& filename){
	// create the XML document
	TiXmlDocument xmlDoc;

	// load the XML document
	if (!xmlDoc.LoadFile(filename)) {
        LOG_ERROR("Unable to open level file \"" + filename + "\" - " + std::string(xmlDoc.ErrorDesc()));

	}

	// get the root element
	TiXmlElement* pRoot = xmlDoc.RootElement();

	TiXmlElement* e = nullptr;

	Region* region = new Region(filename);

	for(e = pRoot->FirstChildElement(); e!= nullptr; e = e->NextSiblingElement()){
		if(std::string(e->Value()) == std::string("resources")){
			ParseRegionResources(e, region);
		}
		if(std::string(e->Value()) == std::string("leveldata")){
			ParseRegionLevelData(e, region);
		}
		if(std::string(e->Value()) == std::string("startinglevel")){
			std::string currentLevel;
			currentLevel = std::string(e->Attribute("id"));
			region->ChangeCurrentLevel(currentLevel);
		}
	}

	return region;
}

void LevelLoader::ParseRegionResources(TiXmlElement* ResourcesNode, Region* region){
	TiXmlElement* e = nullptr;

	for(e = ResourcesNode->FirstChildElement(); e != nullptr; e = e->NextSiblingElement()){
		if(std::string(e->Value()) == std::string("audio")){
			TiXmlElement* soundEffects = e->FirstChildElement(); //SoundEfects Node
			TiXmlElement* songs = soundEffects->NextSiblingElement(); //Songs Node

			ParseSoundEffects(soundEffects, region);
			ParseSongs(songs, region);

		} else if(std::string(e->Value()) == std::string("sprites")){
			ParseSprites(e, region);
		}
	}
}

void LevelLoader::ParseRegionLevelData(TiXmlElement* LevelDataNode, Region* region){
	if(LevelDataNode->FirstChildElement() != nullptr){
		TiXmlElement* currentLevel = nullptr;
		for(currentLevel = LevelDataNode->FirstChildElement(); currentLevel != nullptr; currentLevel = currentLevel->NextSiblingElement()){
			if(currentLevel->Attribute("filename") == nullptr){
				LOG_WARNING("LevelDataNode is missing filename");
				continue;
			}
			std::string filename = std::string(currentLevel->Attribute("filename"));

			if(currentLevel->Attribute("id") == nullptr){
				LOG_WARNING("LevelDataNode is missing id");
				continue;
			}
			std::string id = std::string(currentLevel->Attribute("id"));

			Level* level = ParseLevel(filename);
			region->AddLevel(level, id);
		}
	}
}

void LevelLoader::ParseSoundEffects(TiXmlElement* SoundEffectsNode, Region* region){
	if(SoundEffectsNode->FirstChildElement() != nullptr){
		for(TiXmlElement* currentSoundEffect = SoundEffectsNode->FirstChildElement(); currentSoundEffect != nullptr; currentSoundEffect = currentSoundEffect->NextSiblingElement()){
			if(currentSoundEffect->Attribute("filename") == nullptr){
				LOG_WARNING("SoundEffectsNode is missing filename");
				continue;
			}
			std::string filename = std::string(currentSoundEffect->Attribute("filename"));
			if(currentSoundEffect->Attribute("id") == nullptr){
				LOG_WARNING("SoundEffectsNode is missing id");
				continue;
			}
			std::string id = std::string(currentSoundEffect->Attribute("id"));
			ResourceManager::LoadSoundEffect(filename, id);
			region->AddResource(ResourceType::SOUND_EFFECT, id);
		}
	}
}

void LevelLoader::ParseSongs(TiXmlElement* SongsNode, Region* region){
	if(SongsNode->FirstChildElement() != nullptr){
		for(TiXmlElement* currentSong = SongsNode->FirstChildElement(); currentSong != nullptr; currentSong = currentSong->NextSiblingElement()){
			if(currentSong->Attribute("filename") == nullptr){
				LOG_WARNING("SongsNode is missing filename");
				continue;
			}
			std::string filename = std::string(currentSong->Attribute("filename"));
			if(currentSong->Attribute("id") == nullptr){
				LOG_WARNING("SongsNode is missing id");
				continue;
			}
			std::string id = std::string(currentSong->Attribute("id"));

			ResourceManager::LoadMusic(filename, id);
			region->AddResource(ResourceType::SONG, id);
		}
	}
}

void LevelLoader::ParseSprites(TiXmlElement* SpritesNode, Region* region){
	if(SpritesNode->FirstChildElement() != nullptr){
		TiXmlElement* currentSprite = nullptr;
		for(currentSprite = SpritesNode->FirstChildElement(); currentSprite != nullptr; currentSprite = currentSprite->NextSiblingElement()){
			if(currentSprite->Attribute("filename") == nullptr){
				LOG_WARNING("SpritesNode is missing filename");
				continue;
			}
			std::string filename = std::string(currentSprite->Attribute("filename"));
			if(currentSprite->Attribute("id") == nullptr){
				LOG_WARNING("SpritesNode is missing id");
				continue;
			}
			std::string id = std::string(currentSprite->Attribute("id"));

			ResourceManager::LoadTexture(filename, id);
			region->AddResource(ResourceType::TEXTURE, id);
		}
	}
}