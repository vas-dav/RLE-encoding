# RLE-encoding
#### RLE-encoding/decoding for files in C 
My version of file encoding/decoding as an assignment for Metropolia UAS in C-programming course . 
The task:
>Implement RLE-encoding/decoding of file. Write log of compression results (name, original size, CRC, compressed size, name of compressed file).
>The encoding is based on encoding the multiple occurrences of the same byte with an encoding that saves number of repeating bytes instead of individual bytes. The encoding works as follows:
0x80 is a special character that indicates encoded sequence. It is immediately followed by the byte to repeat and number of repetitions. For example, a sequence of seven ‘T’s would be encoded as: 0x80 0x07 0x54. From this we can see that encoding sequences of less than three same characters increases the result file size instead of decreasing size.
Occurrence of 0x80 is always encoded regardless of the length. There is one exception in the encoding, when number of repetitions is set to zero it indicates a 0x80. A single 0x80 would be encoded as 0x80 0x00, two 0x80s would be encoded as 0x80 0x02 0x80 and three 0x80s as 0x80 0x03 0x80 and so on.
The encoded file must contain a small header that carries the CRC of the original file so that decompressed data can verified.

The project contains two files for the program to run: 

- main.c
> for functions, main program
- rle_vasily.h
> for declarations and macros

(C) Vasily Davydov 2021
