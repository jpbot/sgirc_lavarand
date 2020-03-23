This was the first concept attempt implementing the random number seed generation
as described by Landon Curt Noll's lavarand website.

`seed.sh` script uses IRIX provided utilities to make a single image capture with
the O2 cam, and convert to RAWRGB. `1n.c` is a c program to output every n-th
byte of a given file. `1n` is used to split the RAWRGB capture into 7 segments each
hashed with the *SHA1* algorithim using openssl.

The output is 7 SHA1 digests concatenated, (160 bits * 7) 1120 bits.

This implementation takes a bit over six seconds on my O2, a far cry from the approximate
9000 bits/sec of seed data according to the lavarand website.

`1n.c` is pretty slow, but using `dmconvert` to convert the sgi rgb file to raw rgb is
a big time consumer.

\
Example run with timing:
```
oxide 74# timex ./seed.sh
vidtomem: saved image to file TEMPSNAP-00000.rgb
ouput len 131658
ouput len 131657
ouput len 131657
ouput len 131657
ouput len 131657
ouput len 131657
ouput len 131657
92071488d8ac265081b1250482d20c6dd4919de6b3af36a33941bfa293579ae30e5c412809f34a46fcaae5f0215aec647a634d396765895537174ef3fa2a7f6ce90b608b571aa7f6c951248768657e43ab8d8379d87dce72703f19e1e9fcac9fcb38304a11332665e31b0899286b74af994cc4742aef0e03c5277be68966807c9e29e90aca1f3adf897919d2

real        6.20
user        1.20
sys         0.76
```

\
**hinv of test system**
```
oxide 75# hinv
CPU: MIPS R12000 Processor Chip Revision: 2.3
FPU: MIPS R12010 Floating Point Chip Revision: 0.0
1 270 MHZ IP32 Processor
Main memory size: 640 Mbytes
Secondary unified instruction/data cache size: 1 Mbyte on Processor 0
Instruction cache size: 32 Kbytes
Data cache size: 32 Kbytes
FLASH PROM version 4.18
Integral SCSI controller 0: Version ADAPTEC 7880
  Disk drive: unit 2 on SCSI controller 0
  CDROM: unit 4 on SCSI controller 0
Integral SCSI controller 1: Version ADAPTEC 7880
On-board serial ports: tty1
On-board serial ports: tty2
On-board EPP/ECP parallel port
CRM graphics installed
Integral Ethernet: ec0, version 1
Iris Audio Processor: version A3 revision 0
Video: MVP unit 0 version 1.4
AV: AV1 Card version 1, O2Cam type 1 version 0 connected.
Vice: TRE
1600SW Flat Panel adapter board and display.
```
