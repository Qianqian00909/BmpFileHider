# BMP File Hider

## Introduction

BmpFileHider is a simple tool that allows you to embed and extract hidden information in a BMP (specially BMP3) file.

By modifying the least significant bits of the red,green,and blue channels of each pixel in a BMP file,information is subtly hidden without being noticeable.

And the project provide you with two executable file.One is called *Encoder*,the other is called *Decoder*.

Hope you enjoy it!

## Installation
## Prerequisites
Ensure you have `CMake` installed on your system.
## Steps
1. Clone the repository
```bash
git clone https://github.com/yourusername/BMPFileHider.git
cd BMPFileHider
```
2. build the project:
```bash
mkdir build && cd build
cmake ..
cmake --build .
```
3. The executables will be located in the ./build/src directory:
```bash
cd ..
./build/src/Encoder
./build/src/Decoder
```
## Usage
### Encoder
Fisrt you run *Encoder*.It will ask you for the path
```bash
Please enter the load path:
Please enter the save path:
```
After you provide the right path,it will ask you whether you want to type the information directly with your keyboard,or use a .txt file as the origin of the information you want to hide.
```bash
successfully load the bmp image!
It's1728*1876
There are 1215638 letters that can be hidden
1.txt message.   2.input message directly.  1 or 2 ?
```
Enter 1 or 2 to choose.And if you choose 1,you need to give the path of your .txt file.
### Decoder
Decoder is much more simple.You simply enter your path and it will decode for you.
```bash
Please enter the load path:
```
