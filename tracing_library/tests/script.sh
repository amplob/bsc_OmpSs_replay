if [ $# -ne 1 ]; then
    echo Example of usage:
    echo ./script.sh gauss_seidel
else
    cd $1
    prv2dim EXTRAE_Paraver_trace.prv dim-initial.dim || echo -e "\n\n\nstopped at step 1\n\n\n"
    /home/bsc/bsc/tools/OmpSs_replay/trace_translation/translate_to_mpismpss_trace.py -s dim-initial.dim -d trf-final.trf ||
    echo -e "\n\n\nstopped at step 2\n\n\n"
    trf2trf trf-final.trf dim-final.dim || echo -e "\n\n\nstopped at step 3\n\n\n"
    dimemas_simulation_ncores.py dim-final.dim 4 || echo -e "\n\n\nstopped at step 4\n\n\n"
    cp simulation_results/prv_4cores.prv . || echo -e "\n\n\nstopped at step 5\n\n\n"
    cd ..
fi
