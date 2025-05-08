# wingman

## Introduction

This mod introduces some fixes and changes to the game Rogue Squadron on PC. It addresses bugs that occur when running the game at higher framerates (>30fps), as well as the infamous camera bug. This bug causes the camera to fall behind until the player's craft eventually vanishes into the distance.
The cause of why this may or may not happen appears to lie in uneven framepacing, but the fix here is to simply skip some code that used to mess up the calculations.

Here is the list of fixes/changes:
- Fixes camera bug
- Fixes probe droids bouncing too fast at higher fps
- Fixes fighter craft swaying too much at higher fps
- Fixes smoke (from shooting the ground) expanding too much at higher fps
- Changes level of detail to always show objects at full detail from any distance

## How to Use

Note: wingman is only compatible with the GOG version of the game.
Simply drop rsLauncher.exe and wingman.dll into the game's ROGUE folder. Use rsLauncher to launch the game.
