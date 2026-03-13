# CSniff

A simple TCP port scanner written in C.

## Building

```bash
make
```

The program will be created at `./build/port_sniffer`.

## Usage

```bash
./build/port_sniffer -t <target_ip> -p <port_or_range>
```

You can create an alias in your shell for easier access.

### Examples

Scan a single port:
```bash
./build/port_sniffer -t 192.168.1.1 -p 80
```

Scan a port range:
```bash
./build/port_sniffer -t 192.168.1.1 -p 1-1024
```

## Options

- `-t <ip>`: Target IP address (required)
- `-p <port>`: Port number or range in format `start-end`
- `-h`: Help message
- `-m`: Scan the local machine (m = "my machine")

## Notes

- Ports must be in the range 1-65535
- Target must be a IPv4 address
