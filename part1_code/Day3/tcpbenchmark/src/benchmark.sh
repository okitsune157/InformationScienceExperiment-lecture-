#!/usr/bin/bash
echo “input csv file name”
read NAME
ip=192.168.1.101
port=11111
tryNum=1
threadNum=100
echoBackNum=5
echo “thread” > $NAME.csv
for var in `seq 10`
do
        echo $threadNum
        sudo ./tcpbenchmark $ip $port $tryNum $threadNum $echoBackNum | tail -2 |head -1 >> $NAME.csv
        threadNum=`expr $threadNum + 100`
done
