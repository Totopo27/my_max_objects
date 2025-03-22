# Max/MSP Objects Collection

This repository contains a collection of Max/MSP external objects for microtonal synthesis and MIDI processing.

## Objects

### temperaments

A microtonal synthesis object that supports multiple equal temperaments:

- 19-EDO (19 Equal Divisions of the Octave)
- 31-EDO
- 41-EDO
- 53-EDO

Features:
- Real-time frequency calculation
- Velocity-sensitive output
- Configurable base frequency
- Support for multiple temperament systems

### jweb_processor

A helper object that processes messages from the `jweb` object before they reach the `temperaments` object.

Features:
- Filters unwanted messages from `jweb`
- Processes key press/release events
- Converts messages to proper velocity values:
  - Press: velocity = 127
  - Release: velocity = 0
- Supports both HTML and DAW inputs
- Provides detailed debug output in Max Console
- Seamless integration with `temperaments` object

## Building

### Requirements

- CMake 3.19 or higher
- Max SDK
- C compiler (Visual Studio 2019 or later for Windows, Xcode for macOS)

### Build Steps

1. Clone this repository
2. Set up the Max SDK path in CMake
3. Run CMake to generate build files
4. Build the project

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## Installation

1. Copy the compiled externals to your Max externals folder:
   - Windows: `*.mxe64` files
   - macOS: `*.mxo` files
2. Restart Max

## Usage

Each object has its own README with detailed usage instructions:
- [temperaments documentation](src/temperaments/README.md)
- [jweb_processor documentation](src/jweb_processor/README.md)

## License

MIT License

## Author

Totopo27