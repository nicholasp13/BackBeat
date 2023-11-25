# ![BackBeat](BackBeat/assets/logos/BackbeatLogo_NoFilter_32x32.png?raw=true "BackBeat")  BackBeat

Currently a music application engine for Windows 10/11 written in C++.
Currently implements basic polyphonic synth following the MIDI Manufacturers Association (MMA) DLS Level 1 Specification and the ability to play WAV files in one current format.
Has an example _Sampler_ application for testing.

## How To Use
Developed soley on Windows with Visual Studios 2022, other IDEs untested and may not work. Set `Sampler` as Startup project to run and make sure to clone recursively. If not, use `git submodule update --init` to clone submodules.
1. Clone repository with
	- `git clone --recursive https://github.com/nicholasp13/BackBeat.git`
 	- Using Visual Studios 2022 `Clone a repository` with `https://github.com/nicholasp13/BackBeat.git`
2. Run [GenerateProject.bat](https://github.com/nicholasp13/BackBeat/blob/main/GenerateProject.bat)
3. Run `BackBeat.sln` in Visual Studios.

## Goals
The goal is to create an open source, simple digital audio workstation (DAW) with the ability to easily customize sounds and interface
### Upcoming features:
- Allow for reading MIDI input devices from USB as well as customizing and programming their sounds
- Add support to play MP3 files and files with different sample rates, different bit depths, in mono, etc. 
### Planned features:
- Modular Matrix for interactive GUI implementation to customize sounds
- Create audio tracks for editing and recording with GUI contols
- Allow for sampling, editing, and programming existing audio 
- Other GUI controls for changing module cores and/or synth voices, and other features
- Writing, saving, and exporting music files
- Other Synth modes including monophonic, legatto, gliding, etc.
- Linux support and other random features as necessary or personally desired
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
	* To start/lead the project until implementation of OpenGL (first 18 commits) using videoes 1-14 near verbatim.
	* For guidance and reference when lost.
#### MIDI Association Documents
- DLS Level 1 Specification
  	* link: https://www.midi.org/specifications-old/item/dls-level-1-specification
- DLS Level 2 Specification
  	* link: https://www.midi.org/specifications-old/item/dls-level-2-specification
- personal uses:
  	* To ensure BackBeat follows MIDI industry standards
#### _The Art of VA Filter Design_ by Vadim Zavalishin
- link:
  	* https://www.native-instruments.com/fileadmin/ni_media/downloads/pdf/VAFilterDesign_2.1.0.pdf
- personal uses:
  	* For understanding virtual filter design
  	* For the design in section 3.10 for both the low pass and high pass filters currently implemented in synth
