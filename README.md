# My Max Objects

This repository contains custom Max objects developed for specific audio and music processing needs.

## Objects

### temperaments
A Max object for microtonal frequency calculations. Supports various temperaments (19-TET, 31-TET, 41-TET, 53-TET) and provides frequency output based on key IDs.

Features:
- Multiple temperament support (19, 31, 41, 53)
- Real-time frequency calculation
- Integration with jweb interface
- Polyphonic support

### jweb_processor
A helper object that processes messages from jweb objects to work with the temperaments object.

Features:
- Converts jweb click events to key ID + velocity pairs
- Handles both press and release events
- Clean message formatting for temperaments object

## Building

### Requirements
- CMake 3.19 or higher
- Max SDK
- C compiler (Visual Studio 2019 or higher on Windows, Xcode on macOS)

### Build Steps
1. Clone this repository
2. Set up the Max SDK
3. Configure with CMake:
   ```bash
   cmake -B build -S .
   ```
4. Build:
   ```bash
   cmake --build build --config Release
   ```
5. Copy the resulting .mxe64 files to your Max externals folder

## Installation
Copy the compiled objects (.mxe64 on Windows, .mxo on Mac) to:
- Windows: `C:\Program Files\Cycling '74\Max 8\resources\packages\max-mxj\java-classes\lib\`
- Mac: `/Applications/Max.app/Contents/Resources/C74/packages/max-mxj/java-classes/lib/`

## Usage
See the help files (temperaments.maxhelp and jweb_processor.maxhelp) for detailed usage instructions.

## License
MIT License

## Author
Totopo27