# !/bin/bash

#SBATCH -A ds-m21
#SBATCH -NI
#SBATCH -n 5
#SBATCH --mem-per-cpu=1024
# load the module



input_path=/ds-m21/team_9/inputs/3/input.txt
output_dir=/ds-m21/team_9/outputs/3/test_out

module load hdfs 
if test -f "$input_path";then

hadoop fs -rm $input_path

fi


filename=/home/ds-m21-user9/HW-4/3/input.txt
kNumIterations=$(head -n 1 $filename | awk '{print $1;}')
let "kNumIterations+=10"
echo $kNumIterations

tail -n +2 /home/ds-m21-user9/HW-4/3/input.txt > /home/ds-m21-user9/HW-4/3/tmp.txt && mv /home/ds-m21-user9/HW-4/3/tmp.txt /home/ds-m21-user9/HW-4/3/input.txt
hadoop fs -copyFromLocal /home/ds-m21-user9/HW-4/3/input.txt $input_path
hdfs dfs -test -d $output_dir
if [ $? == 0 ]; then
    hadoop fs -rm -r $output_dir
fi
for ((i=1;i<=kNumIterations;i++))
do
# run streaming job, use absolute paths for all except -input and -output. For them use paths from HDFs which you can find using commands likehdfs fs -ls /ds-m21
    hadoop jar /usr/local/apps/hadoop-3.3.0/share/hadoop/tools/lib/hadoop-streaming-3.3.0.jar -file /home/ds-m21-user9/HW-4/3/mapper.py -mapper /home/ds-m21-user9/HW-4/3/mapper.py -file /home/ds-m21-user9/HW-4/3/reducer.py -reducer /home/ds-m21-user9/HW-4/3/reducer.py -input $input_path -output $output_dir

    hadoop fs -rm $input_path
    hadoop fs -mv $"$output_dir/part-00000" $input_path
    hadoop fs -rm -r $output_dir
done

hadoop jar /usr/local/apps/hadoop-3.3.0/share/hadoop/tools/lib/hadoop-streaming-3.3.0.jar -file /home/ds-m21-user9/HW-4/3/mapper2.py -mapper /home/ds-m21-user9/HW-4/3/mapper2.py -file /home/ds-m21-user9/HW-4/3/reducer2.py -reducer /home/ds-m21-user9/HW-4/3/reducer2.py -input $input_path -output $output_dir

if test -f /home/ds-m21-user9/HW-4/3/output.txt;then
    rm /home/ds-m21-user9/HW-4/3/output.txt
fi
hadoop fs -copyToLocal "$output_dir/part-00000" /home/ds-m21-user9/HW-4/3/output.txt
no_of_components=$(< /home/ds-m21-user9/HW-4/3/output.txt wc -l)
sed -i $"1i$no_of_components" /home/ds-m21-user9/HW-4/3/output.txt
echo "output written to /home/ds-m21-user9/HW-4/3/output.txt"

