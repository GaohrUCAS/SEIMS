#PBS -N scenario_analysis
#PBS -M gaohrgao@163.com
#PBS -e /home/zhulj/SEIMS/models/dianbu/model_dianbu2_30m_longterm/NSGAII_OUTPUT/scenario_analysis.err
#PBS -j oe

#echo Running on hosts : $HOSTNAME  
#echo The date is :  
#date  
#echo The directory is : $PWD
#echo This job will run on the following nodes : $PBS_NODEFILE

hostfilepath=/home/zhulj/SEIMS/models/dianbu/dgpm_hosts_SCOOP
workers=12
script=/home/zhulj/SEIMS/seims_omp_rel_x86-201611/scenario_analysis/nsga2.py
configfile=/home/zhulj/SEIMS/models/dianbu/nsgaii_dianbu2_30m_longterm_omp_dgpm.ini

python -m scoop --hostfile $hostfilepath -n $workers $script -ini $configfile