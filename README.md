# space_shooter
Allegro Space Shooter game (Allegro Vivace) rewritten in object oriented C++ as a learning process.
Allegro Vivace (https://github.com/liballeg/allegro_wiki/wiki/Allegro-Vivace) is a tutorial that I found very inspired. So I rewrote it (because I'm studying C++), added a second player for fun.
All Image and Sounds kept as original from Allegro tutorial. Game setting is a little bit hard (or I'm too bad at playing)

1. Requirement:
- Allegro5

2. Installation: Clone repository, including image and sounds, then do commands:
- g++ *.cpp -o space_shooter $(pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_audio-5 allegro_acodec-5 --libs --cflags)
- ./space_shooter


3. Control: 
- Player 1 use 'A', 'W', 'S', 'D' for navigation and 'SPACE' for shooting.
- Player 2 use arrow buttons for navigation and 'M' for shooting.
- 'ESC' to quit game.
