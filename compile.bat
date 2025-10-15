@echo off

if "%1" == "record" (
    @echo on
    g++ time_utils\nano_current.cpp record\record_value.cpp record.cpp -o recording
    @echo off
    goto exit
)

if "%1" == "read" (
    @echo on
    g++ read_file.cpp -o read
    @echo off
    goto exit
)

echo "The option %1 is not reconized!"

:exit