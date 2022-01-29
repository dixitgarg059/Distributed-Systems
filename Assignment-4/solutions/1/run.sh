#!/bin/bash
#SBATCH -A ds-m21
#SBATCH -NI
#SBATCH -n 5
#SBATCH --mem-per-cpu=1024
# load the module
module load hdfs
# run streaming job, use absolute paths for all except -input and -output. For them use paths from HDFs which you can find using commands likehdfs fs -ls /ds-m21
hadoop jar /usr/local/apps/hadoop-3.3.0/share/hadoop/tools/lib/hadoop-streaming-3.3.0.jar -file /home/ds-m21-user9/HW-4/1/mapper.py -mapper /home/ds-m21-user9/HW-4/1/mapper.py -file /home/ds-m21-user9/HW-4/1/reducer.py -reducer /home/ds-m21-user9/HW-4/1/reducer.py -input /ds-m21/team_9/inputs/1/input.txt -output /ds-m21/team_9/outputs/1/test_out
