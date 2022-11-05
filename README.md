<img align="left" width="100" height="100" src="https://github.com/Toksisitee/PopSoundEditor/blob/master/icon.png">  

# PopSoundEditor
Open source Sound Editor tool written in C++ for Bullfrog's **Populous: The Beginning** game.

### Features
- **Open SDT banks**
	- **Sound Banks**
		- ``Soundd2.SDT`` contains the grunts of followers, ambient noises, SFX sounds, etc.
	- **Drum Banks**
		- ``PopDrum***.SDT`` files contain the rhythmic drums.
	- **Music Banks**
		- ``PopDrones22.SDT`` contains the game music.

- **Preview Sounds**
	- Double click any sound entry to listen to the contents.
	
- **Extract SDT contents**
	- Sound and drum banks contents can be exported as WAV files.
	- Music bank contents can be exported as MP2 files.

- **Create SDT banks**
	- Create new Sound and Drum SDT banks made of your own sounds.
	- **TODO**: music bank creation is only half-finished.
	
### Usage
- **Opening Banks**
	- To open a SDT bank navigate to the ``File`` menu, then use the sub-menu options to select your desired bank type. Once you've opened a bank, the sound table will be appropriately filled with the bank contents. You may double click the entries to preview a specific sound.
- **Extracting Banks**
	- As long as you have a bank opened, extracting banks is straight forward. Navigate to the ``File`` menu and select the ``Extract`` option.
- **Creating Banks**
	- Follow the first step to extract a SDT bank. Replace the extracted files with your own files while maintaining the numbering prefixes and names of the files. These files will be in a different output folder depending on the bank you've extracted (sounds/drums/music). Now go to ``File`` -> appropriate bank sub-menu -> ``Create Bank``. Note that your sound files have to be in the WAV format with the sample rate of 22.050 Hz.

### License
PopSoundEditor Copyright © 2021 Toksisitee.  
This program is distributed under the GNU GPLv3 license. See the LICENSE file for more details. 

### Building
  - [Qt 5.12.11](https://www.qt.io/download)
  - MinGW 32 bit

### Preview
<p align="center">
 <img width="800" height="600" src="https://github.com/Toksisitee/PopSoundEditor/blob/master/PopSoundEditor.PNG"></img>
</p>
