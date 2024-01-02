#include "settings.h"

// QT Logging
#include "qloggingcategory.h"
QLoggingCategory settingsCat("settings");

Settings::Settings(std::shared_ptr<GraphicalView> gfxView) : gView(gfxView) {
    // perhaps have a default tileset in the resources and load it here!

}

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

void Settings::setOverlay(std::string overlayFile){
    QPixmap image(QString::fromStdString(overlayFile));

    if(image.isNull()) {
        qCDebug(settingsCat) << "image is null somehow?";
        return;
    }

    gView->setOverlay(image);

    this->overlay = overlayFile;
}

void Settings::setLevelFile(std::string levelFile){
    this->level = levelFile;
}

std::string Settings::getTileSet(){ return tileSet; }
std::string Settings::getLevel(){ return level; }
std::string Settings::getOverlay(){ return overlay; }