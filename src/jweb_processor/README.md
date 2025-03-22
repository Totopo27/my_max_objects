# jweb_processor

A Max external object that processes messages from the `jweb` object to work seamlessly with the `temperaments` object.

## Features

- Filters out unwanted messages (title, url, etc.)
- Converts jweb key events to proper velocity values:
  - "id do" → "id 127" (note on)
  - "id 0" → "id 0" (note off)
- Supports two velocity sources:
  - HTML input (default)
  - DAW input

## Inlets

1. Left inlet: Messages from jweb (list)
2. Right inlet: DAW velocity input (list)

## Outlet

- Outputs processed messages as "id velocity" pairs

## Messages

- `int 1`: Use HTML velocity (default)
- `int 2`: Use DAW velocity
- Lists in format: `id velocity`

## Usage Example

```
[jweb] -> [jweb_processor] -> [temperaments]
```

## Building

1. Clone the Max SDK
2. Copy the `jweb_processor` folder to `max-sdk-base/source/`
3. Run CMake:
   ```bash
   cd max-sdk-base/build
   cmake ..
   cmake --build . --config Release
   ```
4. Copy the resulting `jweb_processor.mxe64` to your Max externals folder