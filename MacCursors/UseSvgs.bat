@echo off

set INKSCAPE="Inkscape/bin/inkscape.exe"
set SIZE1=32
set SIZE2=64
set SIZE3=128

for %%F in (*.svg) do (
    echo Processing: %%F
    %INKSCAPE% "%%F" --export-area-drawing --export-width=%SIZE1% --export-filename="%%~nF.png"
	echo %SIZE1% done!
    %INKSCAPE% "%%F" --export-area-drawing --export-width=%SIZE2% --export-filename="%%~nF@%SIZE2%.png"
	echo %SIZE2% done!
    %INKSCAPE% "%%F" --export-area-drawing --export-width=%SIZE3% --export-filename="%%~nF@%SIZE3%.png"
	echo %SIZE3% done!
)

echo Done!
