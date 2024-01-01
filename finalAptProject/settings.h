#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <memory>

#include "graphicalview.h"

class GraphicalView;

class Settings
{
public:
    Settings();
    void loadTileSet(std::string tileFile);

private:
    std::string tileSet;
    std::string level;
    
    std::shared_ptr<GraphicalView> gView;
};

#endif // SETTINGS_H
