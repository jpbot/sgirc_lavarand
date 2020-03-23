#!/bin/sh

ONEN=/temp/1n
DIGEST="openssl sha1"
PWDSAVE=`pwd`

cd /tmp

vidtomem -f TEMPSNAP
dmconvert -f rawrgb TEMPSNAP-00000.rgb seed.src
rm TEMPSNAP-00000.rgb

SEED1=`$ONEN seed.src 1 7 | $DIGEST`
SEED2=`$ONEN seed.src 2 7 | $DIGEST`
SEED3=`$ONEN seed.src 3 7 | $DIGEST`
SEED4=`$ONEN seed.src 4 7 | $DIGEST`
SEED5=`$ONEN seed.src 5 7 | $DIGEST`
SEED6=`$ONEN seed.src 6 7 | $DIGEST`
SEED7=`$ONEN seed.src 7 7 | $DIGEST`

SEED=$SEED1$SEED2$SEED3$SEED4$SEED5$SEED6$SEED7

echo $SEED

cd $PWDSAVE
