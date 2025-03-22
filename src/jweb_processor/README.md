# jweb_processor

A Max/MSP external object that processes messages from the `jweb` object, specifically designed to work with the `temperaments` object for microtonal synthesis.

## Features

- Filters unwanted messages from `jweb` object
- Processes key press and release events
- Converts messages to proper velocity values (127 for press, 0 for release)
- Supports both HTML and DAW input modes
- Provides detailed debug output for troubleshooting

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

1. Copy the compiled external (`jweb_processor.mxe64` for Windows or `jweb_processor.mxo` for macOS) to your Max externals folder
2. Restart Max

## Usage

1. Create a `jweb_processor` object in your Max patch
2. Connect the output of your `jweb` object to the left inlet of `jweb_processor`
3. Connect the output of `jweb_processor` to the input of the `temperaments` object

### Inlets

- Left inlet: Messages from `jweb` object
- Right inlet: DAW velocity input (optional)

### Outlets

- Output: Processed messages (list: id velocity)

### Messages

- `1`: Set to HTML velocity mode (default)
- `2`: Set to DAW velocity mode

## Debug Output

The object provides detailed debug output in the Max console, showing:
- Message ID
- Number of arguments
- Type of the third argument
- Value of the third argument
- Final calculated velocity

## License

MIT License

## Author

Totopo27