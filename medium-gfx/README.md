Missing Medium and Heavy graphic (Warrior Only)
===============================================================================

The warrior class (only warrior) can have medium/heavy armor graphics. These graphics are missing though and need to be converted from retail.

Reading Graphics Filenames:
-------------------------------------------------------------------------------

e.g. `wmbst`

w = warrior  
m = medium armor weight  
b = bow

st = stand town (idle animation when not moving in town)  
wl = walk lazy? town walk animation  
as = action stance? in dungeon, idle (not moving or attacking)  
aw = action walk? moving animation in dungeon

Step 1: Get retail graphics
--------------------------------------------------------------------------------

Extract graphics from retail DIABDAT.MPQ using http://www.zezula.net/en/mpq/download.html .

For listfile, scroll to the bottom of http://www.zezula.net/en/mpq/namebreak.html for downloads.

Step 2: Convert from CL2 to CEL
--------------------------------------------------------------------------------

We will use TDG (https://forum.xentax.com/blog/?tag=diablo). The workflow from a high level looks like this:

1. Decompose CL2 to BMP frames
2. Convert BMP frames to CEL frames
3. Reconstruct CEL from CEL frames

We can use Batch Operations (dig through the help guide).

There's one problem with this approach: if the frame width is not detected correctly then converting to CEL will probably fail. E.g. `wmbat` is 96 wide and 128 tall, but TDG thinks it's the opposite. So BMP exporting will be messed up and that will confuse the CEL converter.

This is a royal pain in the butt since `wmbat` is _128_ frames and each frame will need to be _manually corrected_.

Step 2.1: Decompose CL2 to BMP frames
--------------------------------------------------------------------------------

1. Open TDG
2. Check the _Loop_ checkbox. This is critical to do before loading a file to getting Batch Operations to work.
3. Click _Open CL2_ and open your file
4. Click _Save BMP_. This will create a bunch of BMP files in the same directory as the loaded file. E.g. if you loaded `/foo/bar/test.cl2`, then this will create `/foo/bar/test_1.bmp`, `/foo/bar/test_2.bmp`, ...

Step 2.2: Convert BMP frames into CEL frames
--------------------------------------------------------------------------------

Continuing off from the last step:

1. Uncheck the _Loop_ checkbox. This is super critical.
2. Open the first BMP generated, e.g. `/foo/bar/test_1.bmp` It should open just that file, i.e. the program should think there's only 1 frame of animation.
3. Check the _Loop_ checkbox.
4. Click _Save CEL_. This will loop through all frames and convert them to CEL. If you are asked for a filename then you did something wrong. E.g. you should now have `/foo/bar/test_1.cel`, `/foo/bar/test_2.cel`, ...

Step 2.3: Reconstruct CEL from CEL frames
--------------------------------------------------------------------------------

1. Make sure _Loop_ is checked. Check it if is unchecked. This is super critical
2. Click _Open CEL_ and select the first cel, e.g. `/foo/bar/test_1.cel`
3. Change _Groups_ to _8_.
4. Click _Save CEL_. This should combine all frames into one cel, e.g. `/foo/bar/test.cel`