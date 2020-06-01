# Victor 86D reader for Linux

Linux application to read USB data from Multimeter Victor Model 86D.

This project is based on https://github.com/mvneves/victor70c, adapted to work with 86D model and added some additional options.
It uses HIDapi library https://github.com/signal11/hidapi/

Tested on ARM v6x and v7x architectures, like Raspberry PI.

## Installation

Clone the repository or download it directly in a folder

```
git clone https://github.com/edmasini/Victor86D
```

From inside the repository folder, run `Make` command.
**Note:** You need build tools gcc and make installed to run this. You might need to run `sudo apt update` beforehand

```
make
```

Make sure Hidraw properly installed in this command, otherwise you have to compile it manually from the Hidapi repository here ['Hidapi'](https://github.com/signal11/hidapi/).

After build concluded, proceed to installation

```
make install
```

**Note:** if a device is already connected, you have to disconnect it and connect again.

## How to use

Execute victor86d to read the device.

```
$ victor86d
25-01-2020 10:34:12.32343 -0.03 mV
```

Add one or more options as follows:

| parameter alias | parameter name      | description                                |
| --------------- | ------------------- | ------------------------------------------ |
| -h              | --help              |                                            | Display help page |
| -a              | --value             |                                            | Print value only |
| -j              | --json              |                                            | Return JSON format object |
| -p              | --posix             |                                            | Display time as UNIX time format |
| -i [number]     | --interval=[number] | Loop Interval in seconds (>1 second)       |
| -d [number]     | --duration=[number] | Duration of capture in seconds (1> second) |

## Troubleshooting

```
Cannot find Victor86D device
```

Possible reasons:

- Device is not properly connected
- Device is not set to send RS232
- Hidraw driver is not installed and accessible
- User has no permission to access usb device

```
[Empty]
```

Possible reasons:

- Faulty usb cable
- Device is not sending RS232 data
