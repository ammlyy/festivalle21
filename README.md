# Festivalle21

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

The interface shows the color circle with a point corresponding to the color that will be sent through OSC. Below it is possible to change the IP Adress and the Port on which messages are sent. To ensure a communication between the plugin and the reciever make sure the IP Adress and Port corresponds.
