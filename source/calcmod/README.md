# calcmod - Modular Arithmetic External Object

A Max/MSP external object that performs modular arithmetic operations, useful for musical temperament calculations.

## Description

`calcmod` performs addition and subtraction operations with modular arithmetic. It's particularly useful for musical applications where operations need to be performed within a specific temperament (e.g., 12-tone equal temperament).

## Features

- Addition and subtraction operations
- Modular arithmetic with configurable temperament
- Proper handling of negative numbers
- Hot inlets for immediate calculation
- Detailed debugging output

## Inlets (from left to right)

1. First sum input (int)
2. Second sum input (int)
3. First subtraction input (int)
4. Second subtraction input (int)
5. Temperament value for modulo (int, default: 12)

## Outlet

- Single outlet that outputs the result of the most recently triggered operation (sum or subtraction) after applying the modulo operation

## Usage Examples

### Basic Addition with Modulo
```
[5] [3]   [temperament: 12]
 |   |          |
[calcmod]
 |
[number]
Result: (5 + 3) mod 12 = 8
```

### Basic Subtraction with Modulo
```
      [8] [2]   [temperament: 12]
       |   |          |
    [calcmod]
       |
    [number]
Result: (8 - 2) mod 12 = 6
```

### Different Temperament Example
```
[5] [3]   [temperament: 7]
 |   |          |
[calcmod]
 |
[number]
Result: (5 + 3) mod 7 = 1
```

## Building

1. Make sure you have the Max SDK installed
2. Update the `C74_MAX_SDK_DIR` variable in CMakeLists.txt if necessary
3. Build using CMake:
   ```
   mkdir build
   cd build
   cmake ..
   cmake --build . --config Release
   ```

## Installation

1. Copy the compiled external (`calcmod.mxe64` for Windows) to your Max externals folder
2. Restart Max
3. Create a new object with `[calcmod]`

## Notes

- All inlets are "hot", meaning they trigger calculation immediately when receiving a value
- The temperament value must be positive and non-zero (defaults to 12 if invalid)
- Negative results are properly wrapped to positive values within the temperament range
- The object provides detailed debugging output in the Max console