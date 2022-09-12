#### HELayers iOS Tutorial

This includes the intro level tutorials for learning how to compute on data Fully Homormorphically.  The code is all written in C++, but it is shown in a Swift Xcode project.

The libraries are included as binaries, their headers are also included, and the helayers library was compiled twice to run on X86_64 devices (Simulator) as well as arm64 devices.  I am unsure if that means it will work on apple's M1 chip as I have not tested it there.  It also has not been tested that it runs on an iPhone either.  If someone tries it and it does not please open an issue.  The examples included use Microsoft's SEAL library for the underlying FHE library.  So that binary is also included.

#### TODOs

• The third tutorial does not currently run because it is expected to write to a directory like a c++ application.  It needs to be adapted for iOS.

• All of the code is c++ but would like to be able integrate more into the swift language.  Not sure the best way to do this at the moment.

• Port more of the tutorials over to the iOS project.

This is the next step of the evolution of the [IBM FHE Toolkit](https://github.com/IBM/fhe-toolkit-ios).  

