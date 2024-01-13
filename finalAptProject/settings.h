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
    Settings();
    void setGView(std::shared_ptr<GraphicalView> gfxView);

    void loadTileSet(std::string tileFile);

    std::string getTileSet();

    std::vector<std::pair<std::string, std::string>> levels;    
private:
    std::string tileSet;

    std::shared_ptr<GraphicalView> gView;
};

#endif // SETTINGS_H
