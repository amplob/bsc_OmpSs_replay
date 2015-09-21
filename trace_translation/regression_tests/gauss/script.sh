prv2dim EXTRAE_Paraver_trace.prv dim-initial.dim
../../translate_to_mpismpss_trace.py -s dim-initial.dim -d trf-final.trf
trf2trf trf-final.trf dim-final.dim
dimemas_simulation_ncores.py dim-final.dim 4
cp simulation_results/prv_4cores.prv .
