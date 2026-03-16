# CSniff

A simple TCP port scanner written in C. (now with multithreading!)

## Building

```bash
make
```

The program will be created at `./build/port_sniffer`.

You can create an alias in your shell for easier access.

## Usage

```bash
./build/port_sniffer -t <target_ip> -p <port_or_range> [options]
```

Scan a single port:
```bash
./build/port_sniffer -t 192.168.1.1 -p 80
```

Scan a range:
```bash
./build/port_sniffer -t 192.168.1.1 -p 1-1024
```

Show only open ports:
```bash
./build/port_sniffer -t 192.168.1.1 -p 1-1024 -o
```

## Options

- `-t <ip>` : Target IPv4 address
- `-p <port>` : Port or range (e.g. `80` or `1-1024`)
- `-o` : Show open ports only
- `-s` : SYN scan mode (not yet implemented, falls back to TCP)
- `-n` : Legacy scan mode (single-threaded, no port cap)
- `-m` : Scan local machine (not yet implemented)
- `-h` : Help

## Notes

- Multithreaded by default, one thread per port
- Ranges over 4500 ports automatically fall back to legacy (single-threaded) mode
- Ports are corrected automatically if out of range (1-65535)
- Only one scan type flag can be set at a time, conflicts fall back to TCP

## Planned

- Run with no args and get an interactive prompt to fill in options
- Thread pool to replace one-thread-per-port and remove the port cap
- SYN scan using raw sockets
- UDP scan
- Local machine / default gateway scan with `-m`
- Actual help output for `-h`
