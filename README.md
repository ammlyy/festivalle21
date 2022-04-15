# OSC-toolkit

This repository started as a deep learning and OSC protocol-powered stage lighting control system but has since then become a more abstract oriented plugin to implement
custom audio analisys and send results via the OSC protocol. Its utility lies in the ability to make any DAW communicate with every other program accepting data via OSC protocol in a custom way. 

Check the [tutorial on how to add new strategies](https://github.com/ammlyy/festivalle21/wiki/Tutorial:-Add-a-new-strategy) to start implementing your own analisys strategies.

The project was initially developed for the "Project Course" at Politecnico of Milan in collaboration with Festivalle, a jazz festival in the Valle dei Templi in Agrigento (Sicily).

## [Documentation](https://github.com/ammlyy/festivalle21/wiki)

## About
The codebase is subdivided in:
* [Python scripts](https://github.com/ammlyy/festivalle21/tree/main/py)
* [Madmapper projects](https://github.com/ammlyy/festivalle21/tree/main/madmapper)
* [JUCE project](https://github.com/ammlyy/festivalle21/tree/main/Festivalle21)

Check out our [docs](https://github.com/ammlyy/festivalle21/wiki) or explore directly the code using the above links. 


## Setup
Install [Frugally-Deep](https://github.com/Dobiasd/frugally-deep) and point to it in the jucer exporters and project settings Header search paths.
On Windows we recommend using the [vcpkg utility](https://github.com/microsoft/vcpkg) following [these steps](https://github.com/microsoft/vcpkg#getting-started) and then in Juce headers pointing directly to vcpkg's **include** subdirectory that was created.
