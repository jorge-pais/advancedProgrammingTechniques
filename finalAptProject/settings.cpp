#include "settings.h"

// QT Logging
#include "qloggingcategory.h"
QLoggingCategory settingsCat("settings");

Settings::Settings(){
    levels.push_back(std::make_pair(":/images/resources/world_images/worldmap.png", ""));
    levels.push_back(std::make_pair(":/images/resources/world_images/worldmap2.png", ""));
    levels.push_back(std::make_pair(":/images/resources/world_images/worldmap3.png", ""));
    levels.push_back(std::make_pair(":/images/resources/world_images/worldmap5.png", ""));
}

void Settings::setGView(std::shared_ptr<GraphicalView> gfxView){ this->gView = gfxView; }

/// @brief 
/// @param tileFile 
/// @note Tileset file syntax: <low1> <high1> <file1> \newline
void Settings::loadTileSet(std::string tileFile){
    std::ifstream file(tileFile); // open the file

    if(!file){ // check
        qCCritical(settingsCat) << "unable to open tile set file!";
        return;
    }

    gView->clearTileSet();

    std::regex pattern(R"(^(\d+(?:\.\d+)?)\s+(\d+(?:\.\d+)?)\s+(.+)$)");
    std::smatch matches;
   
    std::string line;
    while(std::getline(file, line)){
        //std::cout << line << std::endl;
        if(std::regex_match(line, matches, pattern)){
            float low = std::stof(matches[1].str());
            float high = std::stof(matches[2].str());
            std::string path = matches[3].str();

            gView->addTileSet(low, high, QPixmap(QString::fromStdString(path)));
            //std::cout << "Float 1: " << low << ", Float 2: " << high << ", Path: " << path << std::endl;
        }
    }
    
    this->tileSet = tileFile;
}

std::string Settings::getTileSet(){ return tileSet; }