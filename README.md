# BackBeat
Currently a music application engine for Windows 10/11 written in C++.
Currently implements basic polyphonic synth following the MIDI Manufacturers Association (MMA) DLS Level 1 Specification and the ability to play WAV files in one current format.
Has an example _Sampler_ application for testing.
## Goals
The goal is to create an open source, simple digital audio workstation (DAW) with the ability to easily customize sounds and interface
### Upcoming features:
- Modular Matrix for interactive GUI implementation to customize sounds
- Create basic filters like Low Frequency Oscillators (LFOs)
### Planned features:
- Allow for reading MIDI input devices from USB and customizing and programming their sounds
- Create audio tracks for editing and recording with GUI contols
- Allow for sampling, editing, and programming existing audio 
- Other GUI controls for changing module cores and/or synth voices, and other features
- Writing, saving, and exporting music files
- Support for other sample rates, file formats, mono, etc.
- Other Synth modes including monophonic, legatto, gliding, etc.
- Linux support and other random features as necessary or personally desired
## Sources
#### _Designing Software Synthesizer Plugins in C++ with Audio DSP_ by Will C. Pirkle
- link:
  	* https://www.willpirkle.com/synthlab/docs/html/index.html
 - personal uses:
 	* Applies architecture for the Synth Audio Engine used in this project
	* For certain formulas in regards to sound and wave functions
 	* For understanding Synth architecture and other digital audio principles and concepts	 	 	
#### The Cherno Game engine series
- link: 
	* https://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT
	* https://github.com/TheCherno/Hazel
- personal uses:
	* To start/lead the project until implementation of OpenGL (first 18 commits) using videoes 1-14 near verbatim.
	* For guidance and reference when lost.
#### MIDI Association Documents
- DLS Level 1 Specification
  	* https://www.midi.org/specifications-old/item/dls-level-1-specification
