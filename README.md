Name: Jack Penn\
Section: 3504C\
UFL email: j.penn@ufl.edu\
System: Windows\
Compiler: g++\
SFML version: 2.6.2\
IDE: CLion\
Other notes:
Inside the submission zip, I have included two .png files referenced by this program. These should be moved into the ./files/images folder when testing the program.\
In several places, I have added functionality to the base Minesweeper requirements. These have been described below.
- ### Welcome Window
  - Enhanced name input UX
    - Blinking text cursor
    - Delete key functionality
    - Arrow key navigation
    - Mouse click to set cursor position in input box
    - If the user tries to submit an empty name, the window shakes.
  - Animation for removing a character
    - Deleting, backspacing, or overflowing a character will animate the character bouncing and exploding.
    - During the explosion, the window has a shake animation
- ### Game Window
  - Enhanced game logic
    - In addition to clicking to reveal a tile, a user can drag their cursor to reveal multiple tiles quickly.
    - The game will move a mine if it is clicked on the player's first tile reveal.
    - The timer waits to start until the first tile is pressed.
    - The game logic for placing the mines is optimized for O(n).
      - This means that the game should have no problem generating a board that has number_of_tiles -1 mines.
  - Animations
    - A Tile popping animations plays whenever a tile is revealed.
    - When a bomb is pressed, all bombs will reveal in an animation and create window-shaking explosions radiating outwards.
  - ### Miscellaneous
    - The Welcome Window, Game Window, and Leaderboard windows all have a unique window icon.
  - ### Credit
    - The explosion animation spritesheet was adapted from: https://www.pngegg.com/en/png-xiyem
    - The Crown Image used for the Leaderboard window icon was adapted from: https://www.vecteezy.com/vector-art/25560519-crown-pixel-8-bit-isolated-on-white-background