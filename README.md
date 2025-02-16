# 5G Layer 2 Protocol Stack Implementation

This project implements components of the Layer 2 protocol stack for 5G New Radio (5G NR) from the user equipment (UE) perspective, including both uplink and downlink implementations.

## Overview
This implementation focuses on the User Plane (U-Plane) of the 5G NR protocol stack, providing:
- End-to-end data flow simulation
- Protocol layer encapsulation and decapsulation
- Data integrity verification
- Performance measurements

## Components

1. IP Packet Generator
2. PDCP (Packet Data Convergence Protocol)
3. RLC (Radio Link Control)
4. MAC (Medium Access Control)
5. Transport Block Loopback

## Project Structure
```
5g_l2_implementation/
├── include/
│   ├── ip_packet_generator.h
│   ├── pdcp.h
│   ├── rlc.h
│   ├── mac.h
│   └── profiling.h
├── src/
│   ├── pdcp.c
│   ├── rlc.c
│   ├── mac.c
│   └── profiling.c
├── tests/
│   ├── test_protocol_stack.c
│   ├── test_full_stack.c
│   └── test_performance.c
└── docs/
    ├── user_manual.md
    └── technical_report.md
```

## Building and Running

### Prerequisites

- MinGW GCC compiler
- Visual Studio Code with C/C++ extension
- Windows operating system
- Git (for version control)
- 4GB RAM minimum
- 100MB free disk space

### Environment Setup
1. Install MinGW and add to PATH:
   ```
   C:\MinGW\bin
   ```
2. Verify installation:
   ```bash
   gcc --version
   ```
3. Configure VS Code:
   - Install C/C++ extension
   - Set up compiler path in c_cpp_properties.json
   - Configure tasks.json for building

### Build Instructions

1. Clone the repository
```bash
git clone https://github.com/yourusername/5g-l2-implementation.git
cd 5g-l2-implementation
```
2. Open the project in VS Code
3. Build the desired test:
   - Protocol Stack Test: `Ctrl+Shift+P` -> "Tasks: Run Build Task" -> "build_protocol_stack"
   - Full Stack Test: Select "build_full_stack"
   - Performance Test: Select "build_performance_test"

### Running Tests

1. Protocol Stack Test:
```bash
./test_protocol_stack.exe
```
2. Full Stack Test:
```bash
./test_full_stack.exe
```
3. Performance Test:
```bash
./test_performance.exe
```

## Test Descriptions

1. **Protocol Stack Test**: Basic test of individual protocol layers
2. **Full Stack Test**: End-to-end test with IP packet generation
3. **Performance Test**: Measures throughput and processing times

## Features
- IP packet generation with customizable parameters
- PDCP layer with encryption and integrity protection
- RLC layer with segmentation and reassembly
- MAC layer with transport block handling
- Performance profiling and statistics
- Configurable test parameters
- Detailed performance metrics
- Memory leak prevention
- Error handling and reporting

## Configuration
Key configuration parameters can be modified in:
- `include/pdcp.h`: PDCP header structure
- `include/rlc.h`: RLC modes and segment sizes
- `include/mac.h`: MAC header format
- `tests/test_performance.c`: Test parameters

### Default Settings
- RLC PDU size: 50 bytes
- PDCP encryption: XOR-based (test implementation)
- MAC logical channel ID: 1
- Test iterations: 100 per payload size

## Contributing

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

### Coding Standards
- Use clear variable and function names
- Add comments for complex logic
- Include unit tests for new features
- Follow existing code style

## License
- MIT License
- Copyright (c) 2024
- [Full license text available in LICENSE file]

## Authors
- [Your Name]
- [Your Email]
- [Your Institution]

## Acknowledgments
- 3GPP specifications
- Open source community
- [Your Institution Name]

## Version History
- v1.0.0: Initial release
- v1.1.0: Added performance profiling
- v1.2.0: Enhanced error handling

## Support
For support, please:
1. Check existing documentation
2. Review closed issues
3. Open a new issue with detailed description