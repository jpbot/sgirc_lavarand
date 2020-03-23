# Mk1 improved implementation #

`seed.sh` script combines the C program `cap_stdout.c` and `sha1`.
This sha1 implementation has a handy feature for making multiple digests
of a file based on n-th byte, eliminating the 1n program used in the concept.

`cap_stdout.c` uses the IRIX video library to capture an image from the
O2 cam and outputs the RAWRGB pixel data to *STDOUT*.

`cap_seed.c` similar to cap_stdout.c above, but creates a file `out.seed`.
Mostly existed for testing.

`sha1` is an IRIX binary created from the https://github.com/jpbot/md5_sha repository made from
source available from Landon Curt Noll's website http://www.isthe.com/chongo/bio.html#software

Again, the output is 7 SHA1 digests concatenated, (160 bits * 7) 1120 bits.

This implementation takes about 1 second on my O2, much improved over the concept implementation.

\
Example run with timing:
```
oxide 112# timex ./seed.sh
2cc506706a8d9dd641cb96b25d860ffb8b4649f843b569bbd8a60af4ade8c79e94f16e7ecafe57f915df324d652d079c4f273cb6c09773c01f1bd9b0585b4fc0a42803c6e256021180e87f3579aa1f1a191723e72fd0fe75bd16af22f19610e4909e4e6b7a0eef6552a3369994a9715dfbc9242421b28906176834ef4a3a9fd95559f1b7774e2ddc34537606

real        1.08
user        0.30
sys         0.20
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
