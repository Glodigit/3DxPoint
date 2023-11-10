# 3DxPoint
Named from the merge of "3DxWare" and "trackpoint", this is a DLL tested on 3DxWare 10.6.4 to be able to use a spacemouse as a 2D mouse, smooth scroller and macro ring. I wrote it for my SpaceExplorer, which you can read about [here](https://hackaday.io/project/192735-airberries-and-spaceexplorer/log/224884-a-introducing-3dxpoint-the-3d-trackpoint).

## Usage
Move the spacemouse in X/Y to move the cursor in X/Y. 

Twist the spacemouse to smooth scroll.

Lift and lower the spacemouse to speed up or slow down the speed of the cursor and scrolling.

### ButtonRing
To use the ButtonRing, tilt the spacemouse in one of 8 directions a specific amount. The tilt required is split into "Cardinal Direction", "Outer Cardinal Direction" and "Cardinal Direction Edge", allowing for a total of 24 virtual buttons. A virtual button is selected when the tilt amount stops increasing, and unselected when the tilt is almost 0.

For the ```PC::Typical``` version of 3DxPoint.dll, included in ```[downloaded repo]\DLL```, the ButtonRing is set up as follows:
- Left Mouse: West
- Right Mouse: Outer West
- Middle Mouse: East
- Mirror (switch between left / right hands): Outer East
- Legacy scroll up 1 / 2 / 5 times: North / Outer North / North Edge respectively
- Legacy scroll down 1 / 2 / 5 times: South / Outer South / South Edge respectively
- Snip + Sketch (Snipping Tool): North-west
- Browser - Previous Tab: South-west
- Browser - Next Tab: North-east
- Browser - Close Tab: Outer South-east
- Browser - Back: Outer South-west
- Browser - Forward: Outer North-east

If you want to customise the virtual buttons, you'll have to compile from source. In there, you can set up to 3 different PCs. In ```settings.h```, change ```PC_Select``` to either ```Computer1```, ```Computer2``` or ```Computer3```. Then go to ```SelectButtonOnRing()``` in ```3DxPoint.cpp``` and follow the ```LogMessage((wchar_t*)L"ButtonRing: [Cardinal Direction]: [Up/Down]\n");``` lines to understand where to put the actions. You may want to skim through [this project log](https://hackaday.io/project/192735-airberries-and-spaceexplorer/log/225018-c-3dxpoint-sounds-and-shortcuts) to see how I went about implementing keyboard shortcuts.

## Setup
Download the repository.

Stop the 3DxWare driver if currently running.

Navigate to ```%appdata%``` and then to ```\3Dconnexion\3DxWare\Cfg```.

Paste in ```Desktop.xml``` and ```Global.xml``` from ```[downloaded repo]\Example XML```. In ```Global.xml```, make sure you know which button ```Driver_Desktop_ToggleGrab``` is set to. On my SpaceExplorer, it's the "FIT" button. You may need to start 3DxWare, click on some program, change the desired button in the 3DxWare settings and then see what's generated in ```[program name].xml```. You're looking for 

```
<ButtonBank>
        <Name>STR_DEFAULT_BUTTONBANK</Name>
        <ID>Default</ID>
        <Button>
            <Input>
                <ActionID>[name of the button you need]</ActionID>
            </Input>
```

If you want sounds when a virtual button on the ButtonRing has been pressed, navigate to ```C:\ProgramData\3Dconnexion```, make a folder called ```3DxPoint``` and paste in ```Pizzicato [D/E/F].wav``` from ```[downloaded repo]\Sound Effects```. Additionally, if you want a sound when the ButtonRing has been depressed, paste in ```Pizzicato C.wav```.

Navigate to ```C:\Program Files\3Dconnexion\3DxWare\3DxWinCore64\Win64\DLLs``` and paste in ```3DxPoint.dll```, either from ```[downloaded repo]\DLL``` or, if you've compiled from Visual Studio (2022), ```[downloaded repo]\x64\Debug```.

Start 3DxWare.

## Other notes / trivia
- A small distance from the center (that I'm currently calling the Absolute Zone) will move the cursor the same amount as the spacemouse (but won't move the cursor back to the start like the physical spacemouse). After this zone, it'll move like a Trackpoint.
- If the Outer Edge of the ButtonRing isn't activating, it may be because the speed of the axis (or all axes) set in 3DxPoint is set under 1.0.
- Run 3DxWare in administrator mode if you want to be able to move / click on "elevated programs", such as Task Manager.
- If the program in focus stops responding, the cursor doesn't seem to move. 
- If this is the first time you've ever needed to compile a DLL, see [these prerequisites](https://learn.microsoft.com/en-us/cpp/build/walkthrough-creating-and-using-a-dynamic-link-library-cpp?view=msvc-170#prerequisites).
- Visual Studio 2022 decided to make the main branch name "z" when I initialised the repo. Not sure why, but it's nice and short so I kept it. This is my first non-forked repo so there's much to learn.