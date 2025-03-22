# My Max Objects

A collection of Max/MSP external objects for microtonal synthesis and web interaction.

## Objects

### temperaments

A microtonal synthesis object that calculates frequencies based on different equal temperaments.

Features:
- Supports multiple temperaments (19-EDO, 31-EDO, 41-EDO, 53-EDO)
- Real-time frequency calculation
- Velocity-sensitive output
- Base frequency configuration

### jweb_processor

A helper object that processes messages from the `jweb` object to work with the `temperaments` object.

Features:
- Filters unwanted messages
- Converts jweb key events to proper velocity values
- Supports both HTML and DAW velocity inputs
- Seamless integration with `temperaments`

## Building

1. Clone the Max SDK
2. Copy the source folders to `max-sdk-base/source/`
3. Run CMake:
   ```bash
   cd max-sdk-base/build
   cmake ..
   cmake --build . --config Release
   ```
4. Copy the resulting `.mxe64` files to your Max externals folder:
   - `temperaments.mxe64`
   - `jweb_processor.mxe64`

## Installation

Copy the compiled externals to:
```
C:\Program Files\Cycling '74\Max 8\resources\packages\max-mxj\java-classes\lib\
```

## Usage

See the help files in Max for detailed usage instructions:
- `temperaments.maxhelp`
- `jweb_processor.maxhelp`

## License

MIT License

## Author

Totopo27