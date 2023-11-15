#ifndef MAP_H
#define MAP_H

#define MAP_SIZE 20

class Map
{
public:
    Map();
    void generateRandomMap();
    int mapArray[MAP_SIZE][MAP_SIZE];
};

#endif // MAP_H
