#
# lib
#
cd lib
  mkdir build
  cd build
  qmake ../w.pro "CONFIG+=release"
  make
  cd ..
cd ..

# 
# app
#
cd app/NetBlock
  mkdir build
  cd build
  qmake ../NetBlock.pro
  make
  cd ..
cd ..
cd ..

#
# setup
#
cd setup
mkdir linux
cd linux
#
# NetBlock
cp ../../bin/NetBlock .; strip NetBlock
cp ../../bin/manuf_rep.txt .
cp ../../bin/netblock.db .

#
# compress
#
tar czf ../netblock-linux-$(sed 's/"//g' ../../version.txt).7z *
cd ..
cd ..

