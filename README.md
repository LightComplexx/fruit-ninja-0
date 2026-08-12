# fruit-ninja-0
Adds an additional Bonus Stage to the DragonFly engine game 'Fruit Ninja' with inspiration from the Tekken series.
## Bonus Stage
* If the player reaches a score of 1000 or above in the normal mode, the player will enter a bonus stage where they can control an actual Fruit Ninja to obtain extra points by slicing more fruits.
* Fruits will fly at the player from the right side, and the player must slice using a certain input technique taken from the Tekken series: the Wind Fruit Slice!
* This technique is performed with a Forward -> Neutral -> Down -> Down-Forward -> Slice input on the keyboard.
* If the player presses the last 2 inputs (Down-Forward & Slice) on the exact same frame, the Fruit Ninja performs a faster, stronger Electric Wind Fruit Slice! This will trigger blue effects and an "Electric!" text popup above the Fruit Ninja.
* There is an input display on the right side that will show which buttons the player inputs and for how many frames they held it.

<b>Controls (Normal):</b>
<ul>
    <li>Uses mouse pointer and Left Click to Slice</li>
</ul>

<b>Controls (Bonus Stage):</b>
<ul>
    <li>Up == Press 'W'</li>
    <li>Left == Press 'A'</li>
    <li>Down == Press 'S'</li>
    <li>Right == Press 'D'</li>
    <li>Slice == Press 'I'</li>
</ul>

<b>Extra Additions:</b>
- New character, text, and effect sprites in /sprites
- New header/cpp files for bonus stage

--------------------------------------------------------------------
# Credits
Mark Claypool (WPI)  
2016-2025

Saucer Shoot game from Dragonfly tutorial.

Tutorial available online:

<http://dragonfly.wpi.edu/tutorial/index.html>

And in book:

    Mark Claypool. Dragonfly - Program a Game Engine from Scratch,
    Interactive Media and Game Development, Worcester Polytechnic
    Institute, 2014. Online at: <http://dragonfly.wpi.edu/book/>

--------------------------------------------------------------------

To build:

0) Setup development environment for Windows, Linux or MacOS.  See <http://dragonfly.wpi.edu/engine/index.html#setup> for details.

1) Build, as appropriate for the platform (e.g., "make" on Linux or Mac, F7 on Windows from Visual Studio).

2) Run game, as appropriate for the platform (e.g., "./game" on Linux or Mac, F5 on Windows from Visual Studio)

--------------------------------------------------------------------

Directories:

<ul>
    <li>sounds/ contains the sound files used in Saucer shoot</li>
    <li>sprites/ contains the sprite files used in Saucer shoot</li>
    <li>vs-2022/ contains MS Visual Studio solution files (see README.md)
</ul>

--------------------------------------------------------------------

Happy shooting!

-- Mark
claypool@cs.wpi.edu
