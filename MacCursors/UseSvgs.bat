@echo off

set INKSCAPE="Inkscape/bin/inkscape.exe"
set SIZE1=32
set SIZE2=64
set SIZE3=128
rem scrap SIZE3 DUE TO Size being weird

for %%F in (*.svg) do (
    echo Processing: %%F
    %INKSCAPE% "%%F" --export-area-drawing --export-width=%SIZE1% --export-filename="%%~nF.png"
	echo %SIZE1% done!
    %INKSCAPE% "%%F" --export-area-drawing --export-width=%SIZE2% --export-filename="%%~nF_%SIZE2%.png"
	echo %SIZE2% done!
   rem %INKSCAPE% "%%F" --export-area-drawing --export-width=%SIZE3% --export-filename="%%~nF_%SIZE3%.png"
    rem echo %SIZE3% done!
)

echo Done!
