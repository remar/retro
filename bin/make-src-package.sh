#!/bin/sh

. $(dirname $0)/version

echo ------------------------------------------------------------
echo Making source distribution of Retrobattle, version $RETROVERSION
echo ------------------------------------------------------------

RETRO=retrobattle-src-$RETROVERSION

# Clean source directories
(cd src/ && make clean)
cd ..
(cd remar2d/src && make clean)

if [ -d $RETRO ]
then
    rm -rf $RETRO
fi

mkdir -p $RETRO/src

# Copy sources
cp -r remar2d/src/* retro/src/* $RETRO/src/.

# Record revisions
cd remar2d
REMAR2D_REVISION=`git log | head -n 1 | cut -b 8-`
cd ../retro
RETRO_REVISION=`git log | head -n 1 | cut -b 8-`
cd ..
echo remar2d: $REMAR2D_REVISION > $RETRO/src/REVISIONS
echo retro:   $RETRO_REVISION >> $RETRO/src/REVISIONS


# Generate version file for source distribution
echo "                       Version $RETROVERSION - source distribution" > retro/README-files/srcversion

# Generate README file
(cd retro/README-files/ && m4 README.srcdist) > $RETRO/README

# Remove generated version file
rm retro/README-files/srcversion


# Use correct Makefile in source package
mv $RETRO/src/Makefile.srcdist $RETRO/src/Makefile

# Copy files
cp -r retro/COPYING retro/data retro/doc/manual.txt $RETRO/.

# Create archive files
tar cjvf $RETRO.tar.bz2 $RETRO
zip -r $RETRO.zip $RETRO
rm -rf $RETRO
mkdir -p releases
mv $RETRO.zip $RETRO.tar.bz2 releases
