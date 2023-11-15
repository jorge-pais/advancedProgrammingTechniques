#ifndef WORLDWRAPPER_H
#define WORLDWRAPPER_H

#include <QObject>
#include "world.h"

class WorldWrapper : public QObject
{
    Q_OBJECT
public:
    explicit WorldWrapper(World& world, QObject *parent = nullptr)
        : QObject(parent), m_world(world) {}

private:
    World& m_world;
};

#endif // WORLDWRAPPER_H

