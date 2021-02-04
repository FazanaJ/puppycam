# Ultra Puppycam for SM64

Ultra Puppycam is a brand new camera mode for SM64, designed from the ground up, to improve and fix all of the existing flaws with the horribly dated camera, that comes with the original game. It is based on the original [Puppycam](https://github.com/FazanaJ/puppycam), with improvements and updates by several community members.

# Usage

You will want to use [this repo (UltraSM64 for Refresh 13) as a base](https://github.com/CrashOveride95/ultrasm64) and then apply the patch in WSL. Simply copy ultrapuppycam.patch to the enhancements folder, then in the root directory of your repo, run "tools/apply_patch.sh enhancements/ultrapuppycam.patch" To revert, type "tools/revert_patch.sh enhancements/ultrapuppycam.patch" instead. It should theoretically work with [the vanilla decompilation repo](https://github.com/n64decomp/sm64), but going forward we'll be prioritizing UltraSM64 for compatibility with the extra features it provides.

# Features:

- It features movement on both the X and Y axis, as well as accurate raycasting collision that prevents the camera from ever getting stuck, while retaining its absolute formost in staying within the level boundaries.
- It also features a whole range of different behavioural flags that dictate how the camera itself will operate, based on what you decide to set.
- It has a dynamic options menu, that is incredibly easy to add, or take away options for.
- Support for a player 2 joystick, for emulator users, to allow for analogue camera control.

# Screenshots:

![Screenshot 1](https://i.imgur.com/NJHhyyd.png)
![Screenshot 2](https://i.imgur.com/TmELc4d.png)

# Credits:

- [Fazana](https://github.com/FazanaJ) for creating the original Puppycam and the majority of this version's code
- [CuckyDev](https://github.com/cuckydev) for making the raycasting script that powers Puppycam's collision system.
- [The Clever Folks™](https://github.com/n64decomp/sm64) who made the decompilation project happen to begin with
- [CrashOveride](https://github.com/CrashOveride95) for creating the decomp fork that this is developed for
### Ultra Puppycam Contributors:
- [Axollyon](https://github.com/Axollyon)
- [Fazana](https://github.com/FazanaJ)
- [someone2639](https://github.com/farisawan-2000)
- [Yoshi Milkman](https://github.com/YoshiMilkman)

SM64 Decomp Discord: https://discord.gg/DuYH3Fh
