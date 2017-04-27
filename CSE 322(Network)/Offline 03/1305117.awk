BEGIN {
	max_node = 2000;
	nSentPackets = 0.0 ;		
	nReceivedPackets = 0.0 ;
	rTotalDelay = 0.0 ;
	max_pckt = 10000;

	header = 20;	

	idHighestPacket = 0;
	idLowestPacket = 100000;
	rStartTime = 10000.0;
	rEndTime = 0.0;
	nReceivedBytes = 0;
	rEnergyEfficeincy = 0;

	nDropPackets = 0.0;

	total_energy_consumption = 0;

	temp = 0;
	
	for (i=0; i<max_node; i++) {
		energy_consumption[i] = 0;		
	}

	total_retransmit = 0;
	for (i=0; i<max_pckt; i++) {
		retransmit[i] = 0;		
	}

}
{
	#  ==========================================================
	# Initialization which column  represents which metric
	# ===========================================================
	strEvent = $1 ;		                       # receive /send	
	rTime = $2 ;                               # time
	node = $3 ;                                # src node
	strAgt = $4 ;		                       # mac	
	idPacket = $6 ;                            # packet no
	strType = $7 ;		                       # str type ack,message	
	nBytes = $8;                               # bytes in packet
	energy = $13;		                       # energy string
	total_energy = $14;                        # total energy
	idle_energy_consumption = $16;	           # idle energy
	sleep_energy_consumption = $18;            # sleep energy
	transmit_energy_consumption = $20;         # transmit energy
	receive_energy_consumption = $22;          # received energy
	num_retransmit = $30;                      # retransmit
	
	sub(/^_*/, "", node);
	sub(/_*$/, "", node);
	# =======================================================
	# Energy
	#========================================================
	if (energy == "[energy") {
		energy_consumption[node] = (idle_energy_consumption + sleep_energy_consumption + transmit_energy_consumption + receive_energy_consumption);
	}

	if( trmp>=0 && temp <=25 && energy == "[energy" && strEvent == "D") {
		temp+=1;
	}
	# =======================================================
	# High low packet
	#========================================================

#	if ( strAgt == "AGT"   && strType == "cbr") {
		if (idPacket > idHighestPacket) idHighestPacket = idPacket;
		if (idPacket < idLowestPacket) idLowestPacket = idPacket;
#		}

		
	# =======================================================
	# Start time
	#========================================================	

	if(rTime<rStartTime) {
			rStartTime=rTime;
		}

	# =======================================================
	# End time
	#========================================================	
	if(rTime>rEndTime) {
		rEndTime=rTime;
		}



	# =======================================================
	# Sent packets 
	#========================================================
#	if ( strEvent == "s" &&   strType == "cbr" ) {	
	if ( strEvent == "s"  ) {
			nSentPackets += 1 ;	
			rSentTime[ idPacket ] = rTime ;
		}



	# =======================================================
	# Packet delay calculation
	#========================================================
#		if ( strEvent == "r" ) {
		if ( strEvent == "r" && idPacket >= idLowestPacket) {
			nReceivedPackets += 1 ;	
			if(  nBytes>0)	{
					nReceivedBytes += (nBytes-header);
			}
			
#			printf("bytes %d\n",nBytes);
			rReceivedTime[ idPacket ] = rTime ;
			rDelay[idPacket] = rReceivedTime[ idPacket] - rSentTime[ idPacket ];
			rTotalDelay += rDelay[idPacket]; 

		}
	# =======================================================
	# Drop packet calculation
	#========================================================
	if( strEvent == "D"   &&   strType == "cbr" )
	{
		nDropPackets += 1;
	}


	# =======================================================
	# Retransmission of packet
	#========================================================
	if( strType == "tcp" )
	{
		retransmit[idPacket] = num_retransmit;		
	}

	
}
END {

	# =======================================================
	# Ultimate Receive time (total work time)
	#========================================================
	rTime = rEndTime - rStartTime ;


	# =======================================================
	# Throughput calculation
	#========================================================
	if(rTime!=0)
	{
		rThroughput = nReceivedBytes*8 / rTime;
	}
	

	

	# =======================================================
	# Packet delivery ratio
	#========================================================
	if(nSentPackets!=0)
	{
		rPacketDeliveryRatio = (nReceivedPackets / nSentPackets) * 100 ;
	}
	


	# =======================================================
	# Packet drop ratio
	#========================================================

	if(nSentPackets!=0)
	{
		rPacketDropRatio = (nDropPackets / nSentPackets) * 100;
	}



	# =======================================================
	# Total energy consumption
	#========================================================
	for(i=0; i<max_node;i++) {
		total_energy_consumption += energy_consumption[i];
	}


	# =======================================================
	# Average delay and energy per packet calculation
	#========================================================
	if ( nReceivedPackets != 0 ) {
		rAverageDelay = rTotalDelay / nReceivedPackets ;
		avg_energy_per_packet = total_energy_consumption / nReceivedPackets ;
	}



	# ==================================================================
	# Average energy per byte,per bit and energy efficeincy Calculation
	#===================================================================
	if ( nReceivedBytes != 0 ) {
		avg_energy_per_byte = total_energy_consumption / nReceivedBytes ;
		avg_energy_per_bit = avg_energy_per_byte / 8;
		rEnergyEfficeincy = total_energy_consumption / (nReceivedBytes*8);
	}


	# =======================================================
	# Total retransmission
	#================================printf("Packet drop ratio : %f\n",rPacketDropRatio);========================
	for (i=0; i<max_pckt; i++) {
		total_retransmit += retransmit[i] ;		
	}
	#printf("Total drop : %d\n",nDropPackets);
	#printf("Total sent : %d\n",nSentPackets);
	#printf("Total received : %d\n",nReceivedPackets);
	printf("%lf\n",rThroughput);
	printf("%d\n",rTotalDelay);
	printf("%f\n",rPacketDeliveryRatio);
	printf("%f\n",rPacketDropRatio);
	printf("%f\n",total_energy_consumption);
	#printf("Average delay : %f\n",rAverageDelay);
	#printf("Average energy per byte : %f\n",avg_energy_per_byte);
	#printf("Average energy per bit : %f\n",avg_energy_per_bit);
	#printf("energy Efficiency : %f\n",rEnergyEfficeincy);
	#printf("Total retransmit : %d\n",total_retransmit);



#	printf("3 %lf\n",rThroughput);
	
}