# space_shooter
Allegro Space Shooter game (Allegro Vivace) rewritten in object oriented C++ as a learning process.
Allegro Vivace (https://github.com/liballeg/allegro_wiki/wiki/Allegro-Vivace) is a tutorial that I found very inspired (yup, and of course it looks doable to me). So I rewrote it (because I'm studying C++), added a second player for fun.
All Image and Sounds kept as original from Allegro tutorial. But you can add your own and adjust config accordingly.

1. Requirement:
- Allegro5

2. Installation: Clone repository, including image and sounds, then do commands:
- g++ *.cpp -o space_shooter $(pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_audio-5 allegro_acodec-5 --libs --cflags)
- ./space_shooter

Or use .pro file to open Qt project and build from there. 

3. Control: 
- Player 1 use 'A', 'W', 'S', 'D' for navigation and 'F' for shooting.
- Player 2 use arrow buttons for navigation and 'M' for shooting.
- 'ESC' to quit game.
- 'P' to pause and resume game. 

4. Config:
First run of program, a config file name ssconfig.ini will be generated.
You can change the spritesheet file name, sound file names, positions and dim 
of components such as ships, aliens, explode effects...on this file. The game will
auto rescale assigned components to fit game windows, so you can choose whatever
sizes of spritesheet and components.


Master branch will play at cooperate mode, two ships one score.
