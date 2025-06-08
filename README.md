# 👾 Space Invaders - Console Edition

<p align="center">
  <img src="https://img.shields.io/badge/Platform-Windows%20Console-blue?style=flat-square" />
  <img src="https://img.shields.io/badge/Language-C++-informational?style=flat-square" />
  <img src="https://img.shields.io/badge/Status-Playable-brightgreen?style=flat-square" />
</p>

> 🎮 A modern, feature-rich Space Invaders remake for Windows console (C++).  
> Defend the Earth from waves of alien invaders, survive as long as you can, and climb the leaderboard!

---

## 📋 Table of Contents

- [Features](#-features)
- [Screenshots](#-screenshots)
- [How to Play](#-how-to-play)
- [Installation & Setup](#-installation--setup)
- [Leaderboard](#-leaderboard)
- [FAQ](#-faq)
- [Credits](#-credits)
- [License](#-license)

---

## ✨ Features

- **Classic Space Invaders gameplay** with modern upgrades
- Multiple difficulty levels (Normal/Hard)
- ASCII art, colored UI, and smooth animation
- **Barriers** to shield your spaceship
- Enemy and spaceship bullets, enemy AI, random events
- Persistent leaderboard (scores saved to `leaderboard.txt`)
- Pause menu, How-to-Play screen, and cool Game Over animation
- Highly optimized for Windows console (uses WinAPI)
- Custom player names & easy to extend

---


## 🎮 How to Play

- **A / Left Arrow:** Move spaceship left  
- **D / Right Arrow:** Move spaceship right  
- **Spacebar:** Shoot  
- **ESC:** Pause the game  
- **Enter:** Select menu item  
- Destroy all enemies to win. Don’t let your lives reach zero!
- In Hard mode, you only have one life. Good luck!

### Points

| Enemy Type | Score |
|------------|-------|
| Third Row  | 10    |
| Second Row | 20    |
| First Row  | 30    |
| Spaceship  | 75-200 (random) |

---

## 🛠 Installation & Setup

### Prerequisites

- Windows OS
- [g++ (MinGW)](https://www.mingw-w64.org/) or **Visual Studio** or any C++ compiler supporting Windows headers

### Build & Run

1. **Clone the repository:**
   ```bash
   git clone https://github.com/your-username/space-invaders-cpp.git
   cd space-invaders-cpp
