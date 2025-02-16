# 5G Layer 2 Protocol Stack - User Manual

## Table of Contents
1. [Introduction](#introduction)
2. [Installation](#installation)
3. [Configuration](#configuration)
4. [Usage](#usage)
5. [Testing](#testing)
6. [Troubleshooting](#troubleshooting)

## Introduction

### Overview
This 5G Layer 2 protocol stack implementation simulates the data processing of a 5G NR User Equipment (UE). It includes:
- IP packet generation
- PDCP layer processing
- RLC segmentation/reassembly
- MAC layer handling
- Transport block simulation

### System Requirements
- Hardware:
  - x86_64 processor
  - 4GB RAM minimum
  - 100MB free storage
- Software:
  - Windows 10 or later
  - MinGW GCC compiler
  - Visual Studio Code
  - Git (optional)

## Installation

### Step 1: Install MinGW
1. Download MinGW installer from [mingw-w64.org](https://mingw-w64.org/)
2. Run the installer
3. Add to PATH:
   ```
   Control Panel → System → Advanced → Environment Variables
   Add C:\MinGW\bin to Path
   ```
4. Verify installation:
   ```bash
   gcc --version
   ```

### Step 2: Install Visual Studio Code
1. Download from [code.visualstudio.com](https://code.visualstudio.com)
2. Install C/C++ extension
3. Configure compiler path:
   ```json
   {
       "configurations": [
           {
               "name": "Win32",
               "includePath": ["${workspaceFolder}/**"],
               "compilerPath": "C:/MinGW/bin/gcc.exe"
           }
       ]
   }
   ```

### Step 3: Get the Code
```bash
git clone https://github.com/yourusername/5g-l2-implementation.git
cd 5g-l2-implementation
```

## Configuration

### IP Packet Generator
Configurable parameters in `ip_packet_generator.h`:
```c
uint16_t payload_size;    // Default: 100 bytes
const char* src_ip;       // Default: "192.168.1.100"
const char* dst_ip;       // Default: "192.168.1.200"
uint8_t protocol;         // Default: 6 (TCP)
```

### PDCP Configuration
Settings in `pdcp.h`:
```c
// Header structure
struct pdcp_header {
    uint32_t sequence_number;
    uint8_t data_type;
    uint8_t reserved;
};
```

### RLC Configuration
Options in `rlc.h`:
```c
enum rlc_mode {
    RLC_TM,  // Transparent Mode
    RLC_UM,  // Unacknowledged Mode
    RLC_AM   // Acknowledged Mode
};

// Segment size configuration
uint32_t max_rlc_pdu_size = 50;  // Configurable
```

### MAC Configuration
Parameters in `mac.h`:
```c
struct mac_header {
    uint8_t lcid;          // Logical Channel ID
    uint16_t length;       // Payload length
    uint8_t flags;         // Control flags
};
```

## Usage

### Basic Operation
1. Start VS Code
2. Open the project folder
3. Build the desired test:
   ```bash
   # Protocol Stack Test
   Ctrl+Shift+P → "Tasks: Run Build Task" → "build_protocol_stack"
   
   # Full Stack Test
   Ctrl+Shift+P → "Tasks: Run Build Task" → "build_full_stack"
   
   # Performance Test
   Ctrl+Shift+P → "Tasks: Run Build Task" → "build_performance_test"
   ```

### Running Tests
```bash
# Basic protocol test
./test_protocol_stack.exe

# Full stack test with IP packets
./test_full_stack.exe

# Performance measurements
./test_performance.exe
```

### Understanding Output
Example output format:
```
=== Processing IP Packet ===
Version: 4
Header Length: 20 bytes
Total Length: 120 bytes
...

=== Protocol Stack Performance Statistics ===
Total processing time: 0.123456 seconds
Throughput: 10.5 MB/s
...
```

## Testing

### Available Tests
1. **Protocol Stack Test**
   - Tests individual layer functionality
   - Verifies data integrity
   - Checks header processing

2. **Full Stack Test**
   - End-to-end testing
   - IP packet generation
   - Complete protocol stack flow

3. **Performance Test**
   - Throughput measurements
   - Processing time analysis
   - Memory usage monitoring

### Custom Testing
Create your own test by modifying parameters:
```c
// In test files
uint16_t payload_sizes[] = {100, 500, 1000, 1500};
int iterations = 100;
```

## Troubleshooting

### Common Issues

1. **Compiler Not Found**
   - Check MinGW installation
   - Verify PATH environment variable
   - Restart VS Code

2. **Build Errors**
   ```
   Solution: Check include paths in c_cpp_properties.json
   ```

3. **Memory Leaks**
   - Use memory monitoring tools
   - Check cleanup in test files
   - Verify free() calls

4. **Performance Issues**
   - Reduce test iterations
   - Check system resources
   - Monitor CPU usage

### Error Messages
| Error | Solution |
|-------|----------|
| "gcc.exe not found" | Check PATH variable |
| "Include file not found" | Verify project structure |
| "Memory allocation failed" | Check available RAM |

### Getting Help
1. Check documentation
2. Review source code comments
3. Submit issues on GitHub
4. Contact development team

## Appendix

### Command Reference
```bash
# Build commands
gcc -o test_protocol_stack.exe ...
gcc -o test_full_stack.exe ...
gcc -o test_performance.exe ...

# Run commands
./test_protocol_stack.exe
./test_full_stack.exe
./test_performance.exe
```

### File Structure
```
5g_l2_implementation/
├── include/          # Header files
├── src/             # Source files
├── tests/           # Test files
└── docs/            # Documentation
``` 