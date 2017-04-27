settings=$1
fileName=1305117.out
throughput=0.0
delay=0.0
delivery_ratio=0.0
drop_ratio=0.0
total_energy=0.0





throughput=1
delay=2
delivery_ratio=3
drop_ratio=4
total_energy=5
retValue=""
INITIAL="0 0"
file_parser()
{
	line=1
	while read val
	do
		if [ $line == $1 ] 
			then
			var=$2" "$val
			retValue=$var
			return ;
		else
			line=$(($line + 1))
		fi	
	done<$fileName
}
Variable_node()
{
	dataTH=""
	dataDelay=""
	dataDelivery_ratio=""
	dataDrop_ratio=""
	dataEnergy=""
for((node=20 ;node<=100; node+=20))
do
	if [ $settings == "802_11" ]
			then
				ns 1305117_802_11.tcl $node 10 1 100
				awk -f 1305117.awk 1305117_802_11.tr > $fileName
	elif [ $settings == "802_15" ]
			then
				ns 1305117_802_15.tcl $node 10 5 100
				awk -f 1305117.awk 1305117_802_15.tr > $fileName
	fi
	file_parser $throughput $node
	dataTH="$dataTH
$retValue"
	file_parser $delay $node
	dataDelay="$dataDelay
$retValue"
	file_parser $delivery_ratio $node
	dataDelivery_ratio="$dataDrop_ratio
$retValue"
	file_parser $drop_ratio $node
	dataDrop_ratio="$dataDrop_ratio
$retValue"
	file_parser $total_energy $node
	dataEnergy="$dataEnergy
$retValue"

done
echo "$INITIAL$dataTH">$1Throughput.out
echo "$INITIAL$dataDelay">$1Delay.out
echo "$INITIAL$dataDelivery_ratio">$1Delivery.out
echo "$INITIAL$dataDrop_ratio">$1Drop.out
echo "$INITIAL$dataEnergy">$1Energy.out
xgraph $1Throughput.out -geometry 800x800 &
xgraph $1Delay.out -geometry 800x800 &
xgraph $1Delivery.out -geometry 800x800 &
xgraph $1Drop.out -geometry 800x800 &
xgraph $1Energy.out -geometry 800x800 &
exit 0
}
Variable_flow()
{
	dataTH=""
	dataDelay=""
	dataDelivery_ratio=""
	dataDrop_ratio=""
	dataEnergy=""
for((flow=10 ;flow<=50; flow+=10))
do
	if [ $settings == "802_11" ]
			then
				ns 1305117_802_11.tcl 20 $flow 1 100
				awk -f 1305117.awk 1305117_802_11.tr > $fileName
	elif [ $settings == "802_15" ]
			then
				ns 1305117_802_15.tcl 20 $flow 5 100
				awk -f 1305117.awk 1305117_802_15.tr > $fileName
	fi
	file_parser $throughput $flow
	dataTH="$dataTH
$retValue"
	file_parser $delay $flow
	dataDelay="$dataDelay
$retValue"
	file_parser $delivery_ratio $flow
	dataDelivery_ratio="$dataDrop_ratio
$retValue"
	file_parser $drop_ratio $flow
	dataDrop_ratio="$dataDrop_ratio
$retValue"
	file_parser $total_energy $flow
	dataEnergy="$dataEnergy
$retValue"

done
echo "$INITIAL$dataTH">$1Throughput.out
echo "$INITIAL$dataDelay">$1Delay.out
echo "$INITIAL$dataDelivery_ratio">$1Delivery.out
echo "$INITIAL$dataDrop_ratio">$1Drop.out
echo "$INITIAL$dataEnergy">$1Energy.out
xgraph $1Throughput.out -geometry 800x800 &
xgraph $1Delay.out -geometry 800x800 &
xgraph $1Delivery.out -geometry 800x800 &
xgraph $1Drop.out -geometry 800x800 &
xgraph $1Energy.out -geometry 800x800 &
exit 0
}
Variable_pps()
{
	dataTH=""
	dataDelay=""
	dataDelivery_ratio=""
	dataDrop_ratio=""
	dataEnergy=""
for((pps=100 ;pps<=500; pps+=100))
do
	if [ $settings == "802_11" ]
			then
				ns 1305117_802_11.tcl 20 10 1 $pps
				awk -f 1305117.awk 1305117_802_11.tr > $fileName
	elif [ $settings == "802_15" ]
			then
				ns 1305117_802_15.tcl 20 10 5 $pps
				awk -f 1305117.awk 1305117_802_15.tr > $fileName
	fi
	file_parser $throughput $pps
	dataTH="$dataTH
$retValue"
	file_parser $delay $pps
	dataDelay="$dataDelay
$retValue"
	file_parser $delivery_ratio $pps
	dataDelivery_ratio="$dataDrop_ratio
$retValue"
	file_parser $drop_ratio $pps
	dataDrop_ratio="$dataDrop_ratio
$retValue"
	file_parser $total_energy $pps
	dataEnergy="$dataEnergy
$retValue"

done
echo "$INITIAL$dataTH">$1Throughput.out
echo "$INITIAL$dataDelay">$1Delay.out
echo "$INITIAL$dataDelivery_ratio">$1Delivery.out
echo "$INITIAL$dataDrop_ratio">$1Drop.out
echo "$INITIAL$dataEnergy">$1Energy.out
xgraph $1Throughput.out -geometry 800x800 &
xgraph $1Delay.out -geometry 800x800 &
xgraph $1Delivery.out -geometry 800x800 &
xgraph $1Drop.out -geometry 800x800 &
xgraph $1Energy.out -geometry 800x800 &
exit 0
}
Variable_other()
{
	dataTH=""
	dataDelay=""
	dataDelivery_ratio=""
	dataDrop_ratio=""
	dataEnergy=""
	if [ $settings == "802_11" ]
			then
				for((range=1 ;range<=5; range+=1))
				do
					ns 1305117_802_11.tcl 20 10 $range 100
					awk -f 1305117.awk 1305117_802_11.tr > $fileName
					file_parser $throughput range
					dataTH="$dataTH
$retValue"
					file_parser $delay $range
					dataDelay="$dataDelay
$retValue"
					file_parser $delivery_ratio $range
					dataDelivery_ratio="$dataDrop_ratio
$retValue"
					file_parser $drop_ratio $range
					dataDrop_ratio="$dataDrop_ratio
$retValue"
					file_parser $total_energy $range
					dataEnergy="$dataEnergy
$retValue"

				done
				
	elif [ $settings == "802_15" ]
			then
				for((speed=10 ;speed<=50; speed+=10))
				do
					ns 1305117_802_15.tcl 20 10 $speed 100
					awk -f 1305117.awk 1305117_802_15.tr > $fileName
					file_parser $throughput $speed
					dataTH="$dataTH
$retValue"
					file_parser $delay $speed
					dataDelay="$dataDelay
$retValue"
					file_parser $delivery_ratio $speed
					dataDelivery_ratio="$dataDrop_ratio
$retValue"
					file_parser $drop_ratio $speed
					dataDrop_ratio="$dataDrop_ratio
$retValue"
					file_parser $total_energy $speed
					dataEnergy="$dataEnergy
$retValue"

				done
	fi

echo "$INITIAL$dataTH">$1Throughput.out
echo "$INITIAL$dataDelay">$1Delay.out
echo "$INITIAL$dataDelivery_ratio">$1Delivery.out
echo "$INITIAL$dataDrop_ratio">$1Drop.out
echo "$INITIAL$dataEnergy">$1Energy.out
xgraph $1Throughput.out -geometry 800x800 &
xgraph $1Delay.out -geometry 800x800 &
xgraph $1Delivery.out -geometry 800x800 &
xgraph $1Drop.out -geometry 800x800 &
xgraph $1Energy.out -geometry 800x800 &
exit 0
}
Variable_$2 vf