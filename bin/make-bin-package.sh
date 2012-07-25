#!/bin/sh

. $(dirname $0)/version

echo ------------------------------------------------------------
echo Making binary distribution of Retrobattle, version $RETROVERSION
echo ------------------------------------------------------------

RETRO=retrobattle-linux-x86-$RETROVERSION

# Compile
cd ..
(cd remar2d/src && make)
(cd retro/src && make)

if [ -d $RETRO ]
then
    rm -rf $RETRO
fi

mkdir $RETRO

# Record revisions
cd remar2d
REMAR2D_REVISION=`git log | head -n 1 | cut -b 8-`
cd ../retro
RETRO_REVISION=`git log | head -n 1 | cut -b 8-`
cd ..
echo "   remar2d: $REMAR2D_REVISION" > retro/README-files/revisions
echo "   retro:   $RETRO_REVISION" >> retro/README-files/revisions

# Generate version file for source distribution
echo "                       Version $RETROVERSION - binary distribution" > retro/README-files/binversion

# Generate README file
(cd retro/README-files/ && m4 README.bindist) > $RETRO/README

# Remove generated files
rm retro/README-files/binversion
rm retro/README-files/revisions

# Copy files
cp -r retro/retrobattle retro/COPYING retro/data $RETRO/.

# Create archive files
tar cjvf $RETRO.tar.bz2 $RETRO
zip -r $RETRO.zip $RETRO
rm -rf $RETRO
mkdir -p releases
mv $RETRO.zip $RETRO.tar.bz2 releases
