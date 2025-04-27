# ![BackBeat](BackBeat/assets/logos/BackbeatLogo_NoFilter_32x32.png?raw=true "BackBeat")  BackBeat

Music application engine for Windows 10/11 written in C++.
Currently implements a basic Digital Audio Workstation (DAW) with a polyphonic synth, a sampler with custom sample splicing,
recording audio devices like mics using Windows API, and WAV file playback as shown in the example _Exampler_ application

## Goal
The goal is to create an open source, simple DAW with the ability to easily customize sounds and interface
### Current features:
- Synthesizer with 4 wave oscillators, ladder low pass filter with resonance and bass boost, high pass filter, low frequency oscillator, amplitude envelope generator, filter envelope generator, white noise generator, and digitally controlled amplifier with panning and volume control
- Sample splicer with gain control, pitch shifter, time stretcher, low pass filter, and high pass fitler to fully cut and customize samples from any WAV file
- Sampler with 16 individual pads to play custom sample files with the ability to loop, pan, and control the volume of each individual pad/sample
- Importing of WAV files of all bit depths and 48k sample rate for playback, recording, and sampling
- MIDI input device support for both Synthesizer and Sampler
- Recording audio from devices such as mics and instruments through Windows API and recording BackBeat objects like Sampler and Synth
- Audio Visualization to show users the music their making and how their inputs change the sound
- GUI using ImGui to fully control, add, and delete BackBeat objects during runtime
- XML serialization to save and load any work done as projects
### Upcoming features:
- Upsampler and downsampler to play and splice samples from WAV files of different sample rates
- Reverb
### Planned features:
- Add more GUI controls like copy-paste, grabbing, selecting, deleting, etc. to BackBeat::ImGuiTimeline
- WaveTable oscillators
- More instruments like drumsets and other synths
- Modular Matrix for interactive GUI implementation to customize synth sounds
- Other Synth modes including monophonic, legatto, gliding, etc.
- Add support to play MP3 files
- 2D Renderer for custom sprites and animations for audio visualization
- Linux support

## How To Use
Note: Developed soley on Windows with Visual Studios 2022, other IDEs untested and may not work. Other audio setups also untested and may not work.
Set `Exampler` as Startup project to run and make sure to clone recursively. If not, use `git submodule update --init` to clone submodules.
1. Clone repository with
	- `git clone --recursive https://github.com/nicholasp13/BackBeat.git`
 	- Using Visual Studios 2022 `Clone a repository` with `https://github.com/nicholasp13/BackBeat.git`
2. Run [GenerateProject.bat](https://github.com/nicholasp13/BackBeat/blob/main/GenerateProject.bat)
3. Run `BackBeat.sln` in Visual Studios 2022.
4. Run _Exampler_ as startup project

## How To Run Unit Tests on Visual Studios 2022
1. `Tools` -> `NuGet Package Manager` -> `Manage NuGet Packages for Solution...`
2. Browse for _GoogleTest_ and install the static version of Microsoft GoogleTest into _UnitTests_ project
3. Run tests through Visual Studios `Test Explorer`
  
## Sources
#### _Designing Software Synthesizer Plugins in C++ with Audio DSP_ by Will C. Pirkle
- link to SynthLab SDK:
  	* https://www.willpirkle.com/synthlab/docs/html/index.html
 - personal uses:
 	* Applies architecture for the Synth Audio Engine used in this project
	* For certain formulas in regards to sound and wave functions
 	* For understanding Synth architecture and other digital audio principles and concepts	 	 	
#### The Cherno Game engine series
- links: 
	* https://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT
	* https://github.com/TheCherno/Hazel
- personal uses:
	* To start the project for implementation of OpenGL, ImGui, logging, event handling, etc. 
#### MIDI Association Documents
- DLS Level 1 & 2 Specification
  	* link: https://midi.org/dls
- personal uses:
  	* To ensure BackBeat follows MIDI industry standards
#### _The Art of VA Filter Design_ by Vadim Zavalishin
- link:
  	* https://www.native-instruments.com/fileadmin/ni_media/downloads/pdf/VAFilterDesign_2.1.0.pdf
- personal uses:
  	* For understanding virtual filter design
  	* For the design in section 3.10 for both the low pass and high pass filters currently implemented in synth
