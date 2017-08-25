# bflyttool

A tool for modify bflyt file.

## History

- v1.0.0 @ 2017.08.25 - First release

## Platforms

- Windows
- Linux
- macOS

## Building

### Dependencies

- cmake

### Compiling

- make 64-bit version
~~~
mkdir project
cd project
cmake ..
make
~~~

- make 32-bit version
~~~
mkdir project
cd project
cmake -DBUILD64=OFF ..
make
~~~

### Installing

~~~
make install
~~~

## Usage

~~~
bflyttool [option...] [option]...
~~~

## Options

See `bflyttool --help` messages.
