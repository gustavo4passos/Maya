#include "../include/LevelLoader.h"

#include <algorithm>

#include "../include/Button.h"
#include "../include/Door.h"
#include "../include/ErrorHandler.h"
#include "../include/Golem.h"
#include "../include/Layer.h"
#include "../include/Level.h"
#include "../include/MovingPlatform.h"
#include "../include/PlatformSwitch.h"
#include "../include/Region.h"
#include "../include/ResourceManager.h"
#include "../include/TeleportZone.h"
#include "../include/PushableObject.h"

Level* LevelLoader::ParseLevel(const std::string& filename){
    // create the XML document
	TiXmlDocument xmlDoc;

	// load the XML document
	if (!xmlDoc.LoadFile(filename)) {
        LOG_ERROR("Unable to open level file \"" + filename + "\" - " + std::string(xmlDoc.ErrorDesc()));
		return NULL;
	}

	// get the root element
	TiXmlElement* pRoot = xmlDoc.RootElement();

	std::vector<int> tileLayer; // ints vector that will contain the tile values

	Level* level;
	Layer* layer = NULL;
	Tileset* tileset = NULL;

	int width, height, tileWidth, tileHeight;
	pRoot->Attribute("width", &width);
	pRoot->Attribute("height", &height);
	pRoot->Attribute("tilewidth", &tileWidth);
	pRoot->Attribute("tileheight", &tileHeight);

    // Looping the xml file to find the tileset
	TiXmlElement* e = NULL;

	for(e = pRoot->FirstChildElement(); e != NULL; e = e->NextSiblingElement()) {
		if(e->Value() == std::string("tileset")){
        	if(!(tileset = ParseTileset(e))){
				LOG_ERROR("Tileset object is NULL");
				return NULL;
			}
			break;
        }
	}

    if(e == NULL){
        LOG_ERROR("Cannot find tileset node on xml file");
        return NULL;
	}

	level = new Level(tileset, width, height, tileWidth, tileHeight, filename);

    for(e = pRoot->FirstChildElement(); e!= NULL; e = e->NextSiblingElement()){
        if(e->Value() == std::string("layer")){
            layer = ParseLayer(e, level, tileset);

			if(layer == NULL){
			  	LOG_ERROR("Unable to parse layer " + std::string(e->Attribute("name")));
				delete level;
				return NULL;
			}

			level->AddBackgroundLayer(layer);
        }
    }

	for(e=pRoot->FirstChildElement(); e!=NULL; e = e->NextSiblingElement()){
		if(e->Value() == std::string("objectgroup")){
			ParseObjectGroup(e, level);
		}
	}

    return level;
}

Tileset* LevelLoader::ParseTileset(TiXmlElement* node){
    TiXmlElement* imagenode = node->FirstChildElement();
	// Checks if image node is absent
	if(imagenode == NULL || imagenode->Value() != std::string("image")){
	  	LOG_ERROR("Tileset node missing image field.");
		return NULL;
	}

    std::string source;
    std::string name;

    int width, height;
	int tileWidth, tileHeight;
    int margin, spacing;
	int nColumns, nRows;

    name = std::string(node->Attribute("name"));
    node->Attribute("tilewidth", &tileWidth);
    node->Attribute("tileheight", &tileHeight);
    node->Attribute("columns", &nColumns);

	// Checks if margin and spacing fields are present
	const char* marginFieldCheck;
	const char* spacingFieldCheck;
    marginFieldCheck = node->Attribute("margin", &margin);
    spacingFieldCheck = node->Attribute("spacing", &spacing);

	// If margin and/or spacing fields are absent, set them to 0
	if(marginFieldCheck == NULL) { margin = 0; }
	if(spacingFieldCheck == NULL) { spacing = 0; }

    source = std::string(imagenode->Attribute("source"));
    imagenode->Attribute("width", &width);
    imagenode->Attribute("height", &height);
    nRows = (height - 2*margin + spacing) / (tileHeight + spacing);

	// Tries to load tileset texture to video memory
	if(!ResourceManager::LoadTexture(source, name)) {
		LOG_ERROR("Unable to load tileset texture. Filename: " + source);
		return NULL;
	}

    return new Tileset(source, name, width, height, tileWidth, tileHeight, margin, spacing, nColumns, nRows);

}

void LevelLoader::ParseObjectGroup(TiXmlElement* objectsNode, Level* level){
	float x, y;
    std::string test;

	if(objectsNode->Attribute("name") == nullptr) {
		LOG_WARNING("Objects layer missing name field. Ignoring.");
		return;
	}

    test = std::string(objectsNode->Attribute("name"));
	if(test == std::string("CollisionLayer")){
		for(TiXmlElement* e = objectsNode->FirstChildElement(); e!=NULL; e = e->NextSiblingElement()){

			if(e->Value() == std::string("object")){
				if(CollisionRect* rct = ParseRect(e)){
					level->AddCollisionRect(rct);
				} else {
					LOG_ERROR("Unable to parse collisionRect");
				}
			}
		}
	}
	else if(test == std::string("GameObjects")){
		for(TiXmlElement* e = objectsNode->FirstChildElement(); e!=NULL; e = e->NextSiblingElement()){
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
						if(initiallyOpen == NULL){
							LOG_WARNING("Door's inittialyOpen is missing, door not loaded");
							continue;
						}
						initiallyOpen->QueryBoolAttribute("value", &initialState);
						TiXmlElement* switchesRequiredProperty = GetProperty(propertiesNode, "switchesRequired");
						if(switchesRequiredProperty == NULL){
							LOG_WARNING("Door's switchesRequired's is missing, door not loaded");
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
					TiXmlElement* propertiesNode = e->FirstChildElement();
					if(propertiesNode == NULL) {
						LOG_WARNING("Button missing properties field. Ignoring...");
						continue;
					}
					if(std::string(propertiesNode->Value()) != std::string("properties"))
						continue;
					else{
						TiXmlElement* activatesSwitchNode = GetProperty(propertiesNode, "activatesSwitch");
						if(activatesSwitchNode == NULL){
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
						if(activatesSwitchNode == NULL){
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
						if(destinationXProperty == NULL){
							LOG_WARNING("MovingPlatform destination x is missing, MovingPlatform not loaded");
							continue;
						}

						destinationXProperty->QueryFloatAttribute("value", &destinationX);


						TiXmlElement* destinationYProperty = GetProperty(propertiesNode, "destinationY");
						if(destinationYProperty == NULL) {
							LOG_WARNING("MovingPlatform destination y is missing. Moving platform not loaded");
							continue;
						}
						destinationYProperty->QueryFloatAttribute("value", &destinationY);

						TiXmlElement* requiresSwitchProperty = GetProperty(propertiesNode, "requiresSwitch");
						if(requiresSwitchProperty != NULL && (requiresSwitchProperty->Attribute("value") != NULL)) {
							requiresSwitch = std::string(requiresSwitchProperty->Attribute("value"));
 						}

						level->AddGameObject(new MovingPlatform(Vector2D(x, y), Vector2D(destinationX, destinationY), true, requiresSwitch));
					}
				}
				else if(temp == std::string("pushableObject")) {
					std::cout << "MINHA " << std::endl;
					e->QueryFloatAttribute("x", &x);
					e->QueryFloatAttribute("y", &y);
					level->AddGameObject(new PushableObject(x, y));
				}
			}
		}
	}
	else if(test == std::string("Enemies")){
        int width, height;
        std::string enemyType;
        std::string enemySwitch;
        TiXmlElement* enemyTypeNode;
        TiXmlElement* enemySwitchNode;
        for(TiXmlElement* e = objectsNode->FirstChildElement(); e!=NULL; e = e->NextSiblingElement()){
            if(e->Value() == std::string("object")){
                e->QueryFloatAttribute("x", &x);
                e->QueryFloatAttribute("y", &y);
                e->Attribute("width", &width);
                e->Attribute("height", &height);

                TiXmlElement* propertiesNode = e->FirstChildElement();
                if(std::string(propertiesNode->Value()) != std::string("properties"))
                    continue;
                else{
                    enemyTypeNode = GetProperty(propertiesNode, "type");

                    if(enemyTypeNode != NULL){
                        enemyType = std::string(enemyTypeNode->Attribute("value"));

                    } else {
                        LOG_WARNING("Enemy's type is missing, enemy not loaded");
                        continue;
                    }

                    enemySwitchNode = GetProperty(propertiesNode, "switch");

                    if(enemySwitchNode != NULL){
                        enemySwitch = std::string(enemySwitchNode->Attribute("value"));

                    } else {
                        level->AddEnemy(new Golem(x, y));
                        continue;
                    }
                }

            	level->AddEnemy(new Golem(x, y, enemySwitch));
            }
        }
    }
	else if(test == std::string("Zones")){
		int width, height;
		std::string destinationLevel;
		std::string zoneType;
		float destinationX;
		float destinationY;
		TiXmlElement* zoneNode;
		for(TiXmlElement* e = objectsNode->FirstChildElement(); e!=NULL; e = e->NextSiblingElement()){
            if(e->Value() == std::string("object")){
				e->QueryFloatAttribute("x", &x);
                e->QueryFloatAttribute("y", &y);
                e->Attribute("width", &width);
                e->Attribute("height", &height);

				if(e->Attribute("type") == NULL) {
					LOG_WARNING("Zone's type is missing. Ignoring...");
					return;
				}

				TiXmlElement* propertiesNode = e->FirstChildElement();
				if(std::string(propertiesNode->Value()) != std::string("properties"))
                    continue;
                else{
                    zoneNode = GetProperty(propertiesNode, "destinationLevel");

                    if(zoneNode != NULL){
                        destinationLevel = std::string(zoneNode->Attribute("value"));
                    } else {
                        LOG_WARNING("Zone's destination is missing, zone not loaded");
                        continue;
                    }

					zoneNode = GetProperty(propertiesNode, "destinationX");

					if(zoneNode != NULL){
                       zoneNode->QueryFloatAttribute("value", &destinationX);
                    } else {
                        LOG_WARNING("Zone's destinationX is missing, zone not loaded");
                        continue;
                    }

					zoneNode = GetProperty(propertiesNode, "destinationY");

					if(zoneNode != NULL){
                       zoneNode->QueryFloatAttribute("value", &destinationY);
                    } else {
                        LOG_WARNING("Zone's destinationY is missing, zone not loaded");
                        continue;
                    }

					level->AddGameObject(new TeleportZone(Rect(x, y, width, height),
						destinationLevel, Vector2D(destinationX, destinationY)));
                }
			}
		}
	}
	else {
		LOG_WARNING("Unknown layer type. Name: " + test + " ignoring.");
		return;
	}
}

CollisionRect* LevelLoader::ParseRect(TiXmlElement* objectNode){
	std::string id;
	int x, y;
	int width, height;

	const char* aux = objectNode->Attribute("id");
	if(aux == NULL){
		LOG_ERROR("Id field missing in object from objectgroup");
	} else {
		id = std::string(objectNode->Attribute("id"));
	}
	if(!objectNode->Attribute("x", &x)){
		LOG_ERROR("X field missing in object from objectgroup. Id: " + id);
		return NULL;
	}
	if(!objectNode->Attribute("y", &y)){
		LOG_ERROR("Y field missing in object from objectgroup. Id: " + id);
		return NULL;
	}
	if(!objectNode->Attribute("width", &width)){
		LOG_ERROR("Width field missing in object from objectgroup. Id: " + id);
		return NULL;
	}
	if(!objectNode->Attribute("height", &height)){
		LOG_ERROR("Height field missing in object from objectgroup. Id: " + id);
		return NULL;
	}
	if(objectNode->FirstChildElement() == NULL){
		return new CollisionRect(Rect(x, y, width, height), CollisionBehavior::BLOCK);
	}
	else{
		TiXmlElement* rectProperties = objectNode->FirstChildElement();
		TiXmlElement* e = NULL;
		for(e = rectProperties->FirstChildElement(); e!= NULL; e = e->NextSiblingElement()){
			if(std::string(e->Value()) == std::string("property")){
				if(std::string(e->Attribute("name")) == std::string("collisionbehavior")){
					std::string collision = e->Attribute("value");
					if(collision == "oneway"){
						return new CollisionRect(Rect(x, y, width, height), CollisionBehavior::ONE_WAY);
					}
					else if(collision == "block"){
						return new CollisionRect(Rect(x, y, width, height), CollisionBehavior::BLOCK);
					}
					else{
						LOG_WARNING("Invalid CollisionBehavior for CollisionRect in CollisionLayer. Seting Behavior as BLOCK");
						return new CollisionRect(Rect(x, y, width, height), CollisionBehavior::BLOCK);
					}
				}
			}
		}

		LOG_WARNING("CollisionRect in CollisionLayer missing property field. Setting it as BLOCK.");
		return new CollisionRect(Rect(x, y, width, height), CollisionBehavior::BLOCK);
	}
}

Layer* LevelLoader::ParseLayer(TiXmlElement* layerNode, Level* level, Tileset* tileset){
    if(layerNode == NULL){
        LOG_ERROR("layerNode is NULL");
        DEBUG_BREAK();
		return NULL;
    }

	// Query name, width and height info
	std::string name;
	std::vector<int> layerData;
	int width, height;
	// Append the level filename to the layer name, to avoid duplicate
	// entries in the mesh map in case two layers from different levels
	// have the same name and needs to be in memory simultaneously.
	name = level->filename() + std::string(layerNode->Attribute("name"));
	layerNode->Attribute("width", &width);
	layerNode->Attribute("height", &height);

	// Find layer properties node
	TiXmlElement* propertiesNode = NULL;
	for(propertiesNode = layerNode->FirstChildElement(); propertiesNode != NULL; propertiesNode = propertiesNode->NextSiblingElement()){
	  	if(propertiesNode->Value() == std::string("properties")){
		  	break;
		}
	}

	// Read layer properties data
	double zDistance = 1; // Sets zDistance to the default value, in case it isn't specified
	if(propertiesNode != NULL){
		TiXmlElement* propertyElement = NULL;
		for(propertyElement = propertiesNode->FirstChildElement(); propertyElement != NULL; propertyElement = propertyElement->NextSiblingElement()){
			// Convert to lower case, to make the property name case insensitive
			std::string loweredCasePropertyName = propertyElement->Attribute("name");
			std::transform(loweredCasePropertyName.begin(), loweredCasePropertyName.end(), loweredCasePropertyName.begin(), ::tolower);

			  if(loweredCasePropertyName == std::string("zdistance")){
				propertyElement->Attribute("value", &zDistance);
			}
			else {
				LOG_ERROR("Unknown property in " + name + ": " + propertyElement->Attribute("name"));
			}
		}
	}

	// Load layers mesh data to the GPU
	TiXmlElement* dataNode = NULL;
	for(dataNode = layerNode->FirstChildElement(); dataNode != NULL; dataNode = dataNode->NextSiblingElement()) {
		if(dataNode->Value() == std::string("data")){ // find the dataNode and call ParseLayerData
			layerData = ParseLayerData(dataNode);
			LoadLayerMesh(layerData, level, tileset, name);
		}
	}

	return new Layer(name, width, height, tileset, zDistance);
}

std::vector<int> LevelLoader::ParseLayerData(TiXmlElement* dataNode){
    std::string t;
    std::vector<int> layerdata;

	//finding the text in xml and attributing it to t
	if (dataNode != NULL) {
		for (TiXmlNode* e = dataNode->FirstChild(); e != NULL; e = e->NextSibling()) {
			TiXmlText* text = e->ToText();
			t = text->Value();
		}
	} else {
        LOG_ERROR("dataNode is NULL");
        DEBUG_BREAK();
    }

	//
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
	if(propertiesNode==NULL){
		LOG_ERROR("PropertiesNode is NULL");
		return NULL;
	} else {
		for(TiXmlElement* e = propertiesNode->FirstChildElement(); e!=NULL; e = e->NextSiblingElement()){
			if(std::string(e->Attribute("name")) == propertyName){
				return e;
			}
		}
	}
	return NULL; //Property not found.
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

	TiXmlElement* e = NULL;

	Region* region = new Region();

	for(e = pRoot->FirstChildElement(); e!= NULL; e = e->NextSiblingElement()){
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
	TiXmlElement* e = NULL;

	for(e = ResourcesNode->FirstChildElement(); e != NULL; e = e->NextSiblingElement()){
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
	if(LevelDataNode->FirstChildElement() != NULL){
		TiXmlElement* currentLevel = NULL;
		for(currentLevel = LevelDataNode->FirstChildElement(); currentLevel != NULL; currentLevel = currentLevel->NextSiblingElement()){
			if(currentLevel->Attribute("filename") == NULL){
				LOG_WARNING("LevelDataNode is missing filename");
				continue;
			}
			std::string filename = std::string(currentLevel->Attribute("filename"));

			if(currentLevel->Attribute("id") == NULL){
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
	if(SoundEffectsNode->FirstChildElement() != NULL){
		for(TiXmlElement* currentSoundEffect = SoundEffectsNode->FirstChildElement(); currentSoundEffect != NULL; currentSoundEffect = currentSoundEffect->NextSiblingElement()){
			if(currentSoundEffect->Attribute("filename") == NULL){
				LOG_WARNING("SoundEffectsNode is missing filename");
				continue;
			}
			std::string filename = std::string(currentSoundEffect->Attribute("filename"));
			if(currentSoundEffect->Attribute("id") == NULL){
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
	if(SongsNode->FirstChildElement() != NULL){
		for(TiXmlElement* currentSong = SongsNode->FirstChildElement(); currentSong != NULL; currentSong = currentSong->NextSiblingElement()){
			if(currentSong->Attribute("filename") == NULL){
				LOG_WARNING("SongsNode is missing filename");
				continue;
			}
			std::string filename = std::string(currentSong->Attribute("filename"));
			if(currentSong->Attribute("id") == NULL){
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
	if(SpritesNode->FirstChildElement() != NULL){
		TiXmlElement* currentSprite = NULL;
		for(currentSprite = SpritesNode->FirstChildElement(); currentSprite != NULL; currentSprite = currentSprite->NextSiblingElement()){
			if(currentSprite->Attribute("filename") == NULL){
				LOG_WARNING("SpritesNode is missing filename");
				continue;
			}
			std::string filename = std::string(currentSprite->Attribute("filename"));
			if(currentSprite->Attribute("id") == NULL){
				LOG_WARNING("SpritesNode is missing id");
				continue;
			}
			std::string id = std::string(currentSprite->Attribute("id"));

			ResourceManager::LoadTexture(filename, id);
			region->AddResource(ResourceType::TEXTURE, id);
		}
	}
}
