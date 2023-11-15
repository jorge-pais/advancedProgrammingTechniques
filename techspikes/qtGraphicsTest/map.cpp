#include "map.h"
#include <QObject>
#include <ctime>

Map::Map()
{
    srand(time(0));
}

void Map::generateRandomMap(){

    for(int i = 0; i < MAP_SIZE; i++){
        for(int j = 0; j < MAP_SIZE; j++){
            mapArray[i][j] = rand() % 20;
        }
    }

}
