# bflyttool

A tool for modify bflyt file.

## History

- v1.1.0 @ 2018.01.04 - A new beginning
- v1.1.1 @ 2018.07.07 - Add force mode
- v1.1.2 @ 2018.07.17 - Check text string offset
- v1.1.3 @ 2018.08.23 - Check text string bytes

### v1.0

- v1.0.0 @ 2017.08.25 - First release
- v1.0.1 @ 2017.08.27 - Support change textBox size

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
