rem
rem lib
rem
cd lib
  mkdir build
  cd build
  qmake ..\w.pro "CONFIG+=release"
  mingw32-make -j4
  cd ..
cd ..

rem
rem app
rem
cd app\NetBlock
  mkdir build
  cd build
  qmake ../netblock.pro
  mingw32-make -j4
  cd ..
cd ..
cd ..

rem
rem setup
rem
cd setup
mkdir win
cd win
rem
rem NetBlock
copy ..\..\bin\NetBlock.exe . & strip NetBlock.exe
copy ..\..\bin\manuf_rep.txt .
copy ..\..\bin\netblock.db .

rem
rem deploy
rem
windeployqt NetBlock.exe

rem
rem compress
rem
set /p VERSION=<..\..\version.txt
"C:\Program Files\Bandizip\Bandizip.exe" a ..\netblock-windows-%VERSION%.7z *
cd ..
cd ..