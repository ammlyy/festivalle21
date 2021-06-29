# Festivalle21

This repository contains all the code we used to develop our stage lighting control system.
The project was developed for the "Project Course" at Politecnico of Milan in collaboration with Festivalle, a jazz festival in the Valle dei Templi in Agrigento (Sicily).
We aimed at automating the process of controlling the color of a light system by mean of a neural network able to predict valence and arousal, two values used in the Russell's model of affect to describe emotions. 
The network is built inside a VST built in JUCE, that is fed with raw audio. It segments the stream in windows of 500ms, performs the prediction and outputs two values that are mapped to a RGB color and finally sent through OSC protocol to Madmapper. Madmapper then maps the color messages to lights through the DMX protocol.
## [Documentation](https://github.com/ammlyy/festivalle21/wiki)

## About
This is the codebase for our project on stage lighting control based on MER (Music emotion recognition), subdivided in:
* [Python scripts](https://github.com/ammlyy/festivalle21/tree/main/py)
* [Madmapper projects](https://github.com/ammlyy/festivalle21/tree/main/madmapper)
* [JUCE project](https://github.com/ammlyy/festivalle21/tree/main/Festivalle21)

Check out our [docs](https://github.com/ammlyy/festivalle21/wiki) or explore directly the code using the above links. 


## Setup
Install [Frugally-Deep](https://github.com/Dobiasd/frugally-deep) and point to it in the jucer exporters and project settings Header search paths.
On Windows we recommend using the [vcpkg utility](https://github.com/microsoft/vcpkg) following [these steps](https://github.com/microsoft/vcpkg#getting-started) and then in Juce headers pointing directly to vcpkg's **include** subdirectory that was created.


## Plugin interface
<p>
  <img src="https://user-images.githubusercontent.com/37587013/123405938-a7b8e100-d5aa-11eb-849f-e83f9a69b0f4.png" width="50%"//>
</p>

The interface shows the color circle with a point corresponding to the color that will be sent through OSC. Below it is possible to change the IP Adress and the Port on which messages are sent. To ensure a communication between the plugin and the receiver make sure the IP Adress and Port corresponds.
