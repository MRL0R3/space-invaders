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


## 🏆 Leaderboard

- Scores and player names are saved to leaderboard.txt in the game directory.
- Only the highest score per player is kept.
- View the leaderboard anytime from the main menu.

---

## ❓ FAQ
Q: The console window is too small / not centered!
A: The game auto-resizes the console, but you can maximize it manually if needed.

Q: My score isn’t saving!
A: Make sure you have write permission in the game folder. Check for leaderboard.txt.

Q: The graphics look weird!
A: Make sure you run it in Windows’ default console. Custom or non-standard terminals may not render correctly.

Q: Can I add more levels or enemies?
A: Absolutely! The code is organized for easy extension. Add more enemy types, adjust patterns, or create new modes.

---

🙏 Credits
@MRL0R3 , @mutecopies (main developers)

Classic arcade inspiration: Tomohiro Nishikado’s Space Invaders (1978)

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
