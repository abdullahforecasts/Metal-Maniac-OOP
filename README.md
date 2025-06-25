# Metal Maniac OOP Edition 🎮🔥

![Metal Maniac OOP](https://img.shields.io/badge/Metal_Maniac_OOP-C%2B%2B-orange.svg)

## Link

[🔥 Check Out the Action (Video of GamePlay) ](https://drive.google.com/file/d/1b-njKuVIypZHW8acFLcyZTIY4hfsjiH0/view?usp=sharing)

## Table of Contents

* [Description](#description)
* [New Features](#new-features)
* [Features](#features)
* [Installation](#installation)
* [File Structure](#file-structure)
* [Controls](#controls)
* [Shop System](#shop-system)
* [Game Mechanics](#game-mechanics)
* [Persistence](#persistence)
* [Development](#development)

## Description

**Metal Maniac OOP Edition** is an enhanced object-oriented version of the original side-scrolling shooter, featuring:

* Robust OOP architecture with proper class hierarchy
* Integrated shop system 
* Persistent game state saving
* New pause functionality
* Customizable player experience through unlockables

## New Features

### Shop System (Menu Class)

* Earn Valor currency (1 per enemy kill)
* Unlockable skins and themes
* Option to disable ads
* Persistent unlocks through game sessions
* Shop appears at game start

### OOP Enhancements

* Proper class hierarchy and encapsulation
* Exception handling system
* Improved code organization
* Base GameObject class for inheritance

## Features

### Core Gameplay

* Run, jump, and shoot mechanics
* Directional aiming (left/right)
* Animated sprite sheets
* 5-life health system
* Pause functionality (Z key)

### Enemy Systems

* Ground enemies with pursuit AI
* Jet enemies with bombing mechanics
* Increasing difficulty each level
* Varied enemy behaviors through inheritance

## Installation

1. Ensure all PNG and MP3 files are in the same directory as the `.sln` file.
2. Clone the repository:

```bash
git clone https://github.com/abdullahforecasts/Metal-Maniac-OOP.git
```

3. Install dependencies:

   * Raylib (v4.0+ recommended)
   * C++17 compatible compiler

4. Compile and run:

```bash
cd Metal-Maniac-OOP
g++ bscs24009_main.cpp -o game -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
./game
```

## File Structure

All game assets (PNGs and MP3s) must be in the same directory as the solution file. No separate assets folder is needed.

### Core class files:

```
bscs24009_Background.h/cpp    # Background management
bscs24009_Bullet.h/cpp        # Bullet physics and rendering
bscs24009_Constants.h         # Game constants
bscs24009_Enemy.h/cpp         # Base enemy class
bscs24009_Game.h/cpp          # Main game loop
bscs24009_GameObject.h/cpp    # Base game object class
bscs24009_Jet.h/cpp           # Jet enemy subclass
bscs24009_main.cpp            # Entry point
bscs24009_Player.h/cpp        # Player character
bscs24009_GameExceptions.h    # Custom exceptions
```

## Controls

| Key   | Action                |
| ----- | --------------------- |
| ← →   | Move left/right       |
| Space | Jump                  |
| F     | Fire weapon           |
| Z     | Pause game            |
| ESC   | Quit game             |
| R     | Restart after death   |
| N     | Advance to next level |

## Shop System

### Currency (Valor)

* Earn 1 Valor per enemy kill
* Persistent across game sessions via `GameState.txt`

### Unlockables

| Item        | Cost (Valor) | Description          |
| ----------- | ------------ | -------------------- |
| Skin 2      | 1000         | Alternate character  |
| Skin 3      | 1500         | Premium character    |
| Theme 2     | 500          | Alternate background |
| Disable Ads | 2999         | Remove ad displays   |

> First skin and theme are available by default

## Game Mechanics

### Progression System

* Defeat all enemies to advance
* Each level increases enemy count
* Jets appear after first level
* Shop accessible at game start

### Combat

* Enemies respawn when hit
* Precise bullet collision
* Health bar visual feedback
* Score tracking

## Persistence

Game saves all state to `GameState.txt`, including:

* Current Valor balance
* Unlocked skins/themes
* Ads enabled/disabled status
* Selected preferences
* Game progress

## Development

### Built With:

* C++17 (OOP approach)
* Raylib 4.0


---

🛠️ **This project is open for additions, improvements, and collaborations. Feel free to fork, extend, and contribute!** 🚀
