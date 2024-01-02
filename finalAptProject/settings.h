#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <memory>
#include <fstream>
#include <iostream>
#include <regex>

#include "graphicalview.h"

class GraphicalView;

class Settings
{
public:
    Settings(std::shared_ptr<GraphicalView> gfxView);
    void loadTileSet(std::string tileFile);
    void setOverlay(std::string overlayFile);
    void setLevelFile(std::string levelFile);

    std::string getTileSet();
    std::string getLevel();
    std::string getOverlay();

private:
    std::string tileSet;
    std::string level;
    std::string overlay;
    
    std::shared_ptr<GraphicalView> gView;
};

#endif // SETTINGS_H
