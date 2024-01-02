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

    /// TESTING REGEX FOR LOADING LEVELS!
    std::string data = 
        "level:\n"
        "roomA path/to/levelImgA 10 5 0\n"
        "roomB path/to/levelImgB 8 3 0.8\n"
        "roomC path/to/levelImgC 8 3 0.3\n"
        "doors:\n"
        "roomA roomB 3 3 10 10\n"
        "roomB roomC 5 6 10 10";

    std::regex levelRegex("^(.*)\\s+(.*)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+(?:\\.\\d+)?)$");
    std::regex doorRegex("^(.*)\\s+(.*)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)$");

    std::istringstream iss(data);
    std::string line;

    while (std::getline(iss, line)) {
        std::smatch matches;
        if (std::regex_match(line, matches, levelRegex)) {
            std::cout << "Level - Name: " << matches[1] << ", Path: " << matches[2] 
                      << ", Enemies: " << matches[3] << ", Healthpacks: " << matches[4] 
                      << ", Ratio: " << matches[5] << std::endl;
        } else if (std::regex_match(line, matches, doorRegex)) {
            std::cout << "Door - Source: " << matches[1] << ", Destination: " << matches[2] 
                      << ", PosX_src: " << matches[3] << ", PosY_src: " << matches[4]
                      << ", PosX_dst: " << matches[5] << ", PosY_dst: " << matches[6] << std::endl;
        }
    }
}

std::string Settings::getTileSet(){ return tileSet; }
std::string Settings::getLevel(){ return level; }
std::string Settings::getOverlay(){ return overlay; }