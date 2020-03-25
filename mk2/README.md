# Mk2 integrate SHA1 in data capture program #

`cap_sha1.c` uses the IRIX video library to capture an image from the
O2 cam and outputs 7 concatenated SHA1 digests from the RAWRGB pixel data to *STDOUT*.

`cap_seed.c` was built during testing to output a file `out.seed`. Just and updated
version of the Mk1 file that evolved into cap_sha1.c, change from Mk1 is stripping
alpha byte from the ARGB data provided by the video library to just RGB in a memory
buffer.

`sha1` directory is sha1 implementation from the https://github.com/jpbot/md5_sha
repository made from source available from Landon Curt Noll's website
http://www.isthe.com/chongo/bio.html#software

`Makefile` makefile to build cap_sha from cap_sha1.c

## Building ##
Makefile isn't perfect at this time, the .o files for the sha1 directory need to be
separately built. Easiest build:
```cd sha1
make
cd ..
make
```

_note:_ Currently a number of warnings because cap_sha1.c is built with the `sha1drvr.o`
which expects a couple functions from `sha1dual.o`, additionally, `shadrvr.o` is unchanged
and includes a duplicate main function.


\
Example run with timing (10 frames):
```
oxide 130# timex ./cap_sha1
e84c022896e3a33e85a3f2d3cd5df0eee808d3f7e2170d219b2c8bdb85944e0b43126e11b9320b53eb87e5e2351aa35710e1265fd22d46bbae05229a274f44cac638f8e14d30502f907ada62b0a9d22fb88037554585a10d159176f33f1e50760ea811983edd6fe41fe691e022476f0a2fec118371f843deba3080f0651e713f1b39aeeb87333a666f1f7807
662ccce7ac62862f175a5cb366e67525d5d65daa46fa4c2db5a6dad5357261fe6e11335de1b529455890895ee978103a0ea86cecb8fdbc548048c9d93143e677fc25e9473d2928e6834d9d15af6eee5455e71a582c7c6a45e2f737215ca187ea115711eb08f27390f5e3517c524137e472acd2469c6cfbabe215278b24050ec823b3e16b976b958067cb5db3
9feed34f3a80e2db518dd37c2bcb15998e0db9421b036fee0e866af074482f8054eceecb577d1cebdcee2839aaca302c55e7aa1334e3beb4b6a6b0112f06fc3ef2f2dfa8ecdca681746b992ed16d8e313e21b62344935fa3545e07969dca6b77457e863325975a28429f04291cf8373e7b13ae71825ea8cf4ad2c0b8af0fcfc5e9502784f8e91586b903cb1d
976124d6e5ae0bd576a49e01ed39f751d7517a21e32035edc3859d51d71e4282af836c95fb3a03af583abf3ad4073b3bf817184af342ecf4161e509790642ef106db252373705c80f36c0e81e0f0f41bf855768416329a550c83c3fdd5eea72725aa694261708b521198012140ff87a01766ef630e8c04328586e8507f87b4a669c0c18eaadbbabceb18e59d
bb2d5654d0e74bd8fb80aec68d368664553ac9c567c88f73ac996b4e8ca93bbfb10fecad1b4a435085a439d0ffb13618fe5fd9c85ffbca2166498b2a0504f807ea609e09621aecbb9fffeae666d6763e47e0860486ac8b67db8fd9eec957edb6f90e9e7d90e17e134cd5ccc32d407b2bbd017348496d5e664a048b98ab9524f5bc9c06e244125f6ca5e3ce53
b5af0a19d7f2291395630db4b552c8f52299c78fcdc508bdc7f65c01f3f6ae206a40354aa5ebbcefd5451ddd8c718f544fbf58764a159dd7769e2490b239fcf7e16932a5ea53e08548a5bce2c733b90cef14b28b4d5353e22a4b12ab7ce54e5d49287171bf964156100bf89bc56b4aa9cfa471766fcb5d35d35c6f028489cf95e7ae193df30f27205a83e7c1
df1be73b02fc99b6f24b4b5abf912cd1de6f53bd86dad67c457bab6879b85c428a6ebef97b0d532e991d8ba21673bbcc83a4f69b1a4c6344f745e8181a924caaea3c0614ca75df228e0ba5b84175c56b0e1213384ff5414ca4ad698ed883f0c9dd8c598ad10f01aa6ac9664f638b965527a7613802918474660632d7fea2e92ce100c73dc2d956ac7f312b95
df60b139692a6963a5699d758dd9898b2e3ebc6949cc9ad2b911f961945d36c0b1472d6151234aab446ecda4e5222a5725850a8eb7ba967067235e9d851d1481b0c58282855f7287d786064b877d78f7cd95999ade26ff8da7c135a1c2dadd6e9843a902a7de23f875671be103124ce5ba2e04b6415ca5544973f1ad8b42d498eab6dc8a1275f921fc621f1c
091fdf3beeceb0b7d60f12519b21498fe956cbaf06d0d164f9485dfb35fcb85eac6a823ea0db59963ab9d71f6027d3ff762ca94876b244031554f3f1e0f1fda4a88bef8dccbf5850619d58408b6bc6cac9fbf4eb83ebac8b2b4a13d243cef8d5158ff9096f80549f5dd1e3d0ebbbd12366eda1d2617aa17ff9404f54dff651df406fd369973862481f0acd2a
2e9470729e8e7288d5c7d51c0ef3bc5a5dd52f0be3b5dabe89c9dfcaf69f0b7e73feb2970ceccaebd4e26292ae9ee21bc8de337ec8bff72edc232be0c6386f973c76b3e2867d4c06c307e00526ed1fcc7e58f9f499a53e21135b94ee15dae5fbe3931c0ab8dc030c078142b83a9b2cdcd132b8e3e1ff38b6385b095799271a8e86460c998a1c9f9540e210fa

real        2.29
user        1.22
sys         0.26
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
