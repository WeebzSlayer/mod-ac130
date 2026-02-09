# 𝖒𝖔𝖉-𝖆𝖈130 🚀🔥

[![AzerothCore-WotLK](https://img.shields.io/badge/AzerothCore-WotLK-blue?style=for-the-badge&logo=c%2B%2B)](https://www.azerothcore.org/)
![License-AGPL](https://img.shields.io/badge/License-AGPL-red?style=for-the-badge)
![Module-Gameplay](https://img.shields.io/badge/Module-Gameplay-orange?style=for-the-badge)

A high-intensity world event module for **AzerothCore** that brings total aerial devastation to the world. Every hour, Alliance and Horde AC130 fleets spawn to "glass" specific zones, leaving nothing alive in their wake. 💀

## 📖 Description

The `mod-ac130` creates a dynamic, dangerous environment where players must keep an eye on the skies. Three Zeppelins per faction will patrol a random zone, dropping massive ordnance that kills players and NPCs alike.

## 🎯 Intelligent Targeting Hierarchy

The module follows a strategic priority system to ensure maximum impact:

*   **Metropolitan Strike (5% - 10%):** A small chance to target major capital cities (Stormwind, Orgrimmar, Ironforge, etc.). 🏰
*   **Hunter-Killer (30% - 40%):** If a city isn't targeted, the module searches for zones currently populated by active players. 👥
*   **Scorched Earth:** If no players are found, it falls back to a list of predefined common zones (The Barrens, Westfall, etc.). 🌍

## 🛠️ AC130 Mechanics

*   **Anti-Collision Logic:** Alliance and Horde fleets operate simultaneously but will never target the same zone at once. 🛰️
*   **Squadron Formation:** Spawns 3 Zeppelins (*Skybreakers* for Alliance, *Orgrim's Hammers* for Horde) flying in formation.
*   **Slow Death:** Zeppelins fly at a custom slow speed (configurable) to ensure maximum "danger time" for those on the ground. 🐢
*   **Area Denial:** Every few seconds, bombs are dropped. Anything within the 40-yard impact radius (players/NPCs) is instantly killed. 💣
*   **Warning Systems:** A zone-wide colored system message alerts all players when a fleet has arrived. 🚨

---

## ⚙️ Configuration

This module is fully configurable via `ac130.conf`. You can change settings without recompiling.

| Setting | Description | Default |
| :--- | :--- | :--- |
| **Enable** | Toggle the module on or off. | `1` |
| **Interval** | Time between bombardments (in minutes). | `60` |
| **Altitude** | How high the ships fly. | `80.0` |
| **FlySpeed** | The movement speed of the fleet. | `0.3` |
| **BombRadius** | Impact radius of the bombs. | `30.0` |
| **InstantKill** | Toggle between 1-shot kills or % based damage. | `1` |

---

## 🛠️ Installation

1.  **Download :** Place the `mod-ac130` folder into your `azerothcore-wotlk/modules/` directory.
2.  **Config :** Copy `ac130.conf.dist` to your server's `etc` folder and rename it to `ac130.conf`.
3.  **SQL :** Run `creature_template.sql` in your `acore.world` database to link the NPC AI:
4.  **Server :** Run your server and brace yourself!

---

## 🧪 Testing the Module
*  To test the event immediately without waiting an hour, open ac130.conf.
*  Set AC130.Interval = 1.
*  Restart your worldserver.
*  The fleet will arrive 60 seconds after the server starts.

---

## 📜 Credits

*   **WeebzSlayer**
*   **Rev** for the inspiration.

---

**Made for AzerothCore.** ✨
If you like this module, feel free to give it a ⭐ on GitHub!
