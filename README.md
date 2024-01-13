# Final Project - Advanced Programming Techniques

## Basics

This section explains how to play the game.

This is you:

![tux](./finalAptProject/resources/entities/tux.png)

You can move around using the ```WASD``` keys.

Go over an enemy to attack.

![enemy](./finalAptProject/resources/entities/snowball-2.png)

Killing an enemy squishes it.

![dead enemy](./finalAptProject/resources/entities/squished-left.png)

Go over an fish platter to heal.

![health pack](./finalAptProject/resources/entities/platter.png)

Go over a door to go to another level. Go over it gain to go back!

![door](./finalAptProject/resources/entities/door.png)

## Enemy types

The pink enemy applies poison which damages both you and it.

![pink enemy](./finalAptProject/resources/entities/smartball-2.png)

The eye-patched enemy is a necromancer and teleports to the corpse of fellow dead enemies and resurrects itself there.

![red enemy](./finalAptProject/resources/entities/captain_left-2.png)

## Views

You can switch tabs (by clicking them or using ```Ctrl``` + ```tab```) to switch views. Type help on the text user interface to see list of available commands.

### Graphical View

#### Overlay

You can set overlays for each level within the settings window. This can then be toggled from the toolbar. Do make sure to load a new game first!

#### Tileset

A universal tileset can also be set through the settings window. Simply create a file with the following syntax:
```
<low1> <high1> <file1>
<low2> <high2> <file2>
...
```

And point to it on the settings window. It can then be toggled from the toolbar in the GUI.

#### Zoom

Use ```Ctrl``` + ```+``` to zoom in on the GUI and ```Ctrl``` + ```-``` to zoom out.