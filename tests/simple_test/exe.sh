export EXTRAE_HOME=/home/bsc/bsc/tools/extrae-install
export EXTRAE_CONFIG_FILE=./extrae.xml
export LD_PRELOAD=${EXTRAE_HOME}/lib/libseqtrace.so:/home/bsc/bsc/tools/OmpSs_replay/install/lib/libmpissTrace.so
export EXECUTABLE=executable.exe # This is taken by extrae.xml

./executable.exe