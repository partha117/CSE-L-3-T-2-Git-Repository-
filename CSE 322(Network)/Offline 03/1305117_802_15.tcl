set val(chan)           Channel/WirelessChannel    ;#Channel Type
set val(prop)           Propagation/TwoRayGround   ;# radio-propagation model
set val(netif)          Phy/WirelessPhy/802_15_4            ;# network interface type
set val(mac)            Mac/802_15_4                 ;# MAC type
set val(ifq)            Queue/DropTail/PriQueue             ;# interface queue type Queue/DropTail/PriQueue CMUPriQueue
set val(ll)             LL                         ;# link layer type
set val(ant)            Antenna/OmniAntenna        ;# antenna model
set val(ifqlen)         50                        ;# max packet in ifq                        ;# number of mobilenodes
set val(rp)             DSDV               ;# routing protocol
set val(x)              600
set val(y)              600
set val(row)             3
set val(col)             3
set val(nn)             [expr $val(row)*$val(col)]
set grid                 1
set val(ttime)           100
set val(extraTime)       10
Mac/802_15_4 set dataRate_ 11Mb

#  Global Variables
set ns_         [new Simulator]
set tracefd     [open 1305117_802_15.tr w]
$ns_ trace-all $tracefd

set namtrace [open 1305117_802_15.nam w]
$ns_ namtrace-all-wireless $namtrace $val(x) $val(y)

# set up topography object
set topo       [new Topography]
$topo load_flatgrid $val(x) $val(y)

# Create God
create-god $val(nn)

# Create channel
set chan_1_ [new $val(chan)]

$ns_ node-config -adhocRouting $val(rp) \
                -llType $val(ll) \
                -macType $val(mac) \
                -ifqType $val(ifq) \
                -ifqLen $val(ifqlen) \
                -antType $val(ant) \
                -propType $val(prop) \
                -phyType $val(netif) \
                -topoInstance $topo \
                -agentTrace ON \
                -routerTrace OFF \
                -macTrace ON \
                -movementTrace ON \
                -channel $chan_1_

      for {set i 0} {$i < [expr $val(nn)]} {incr i} {
                  set node_($i) [$ns_ node]
                  $node_($i) random-motion 0  
                  puts "creating node $i"            ;
               # set mac_($i) [$node_($i) getMac 0]
                # $mac_($i) set RTSThreshold_ 3000       
            }

#if [expr $grid==0] {
#   for {set i 0} {$i < $val(nn) } { incr i } {
#   set xx [expr rand()*$val(x)]
#   $node_($i) set X_ $xx
#     $node_($i) set Y_ $yy
# }
#} else {
# for {set i 0} {$i < $val(row) } { incr i } {
#
#   for {set j 0} {$j < $val(column) } { incr j } {
#
#   }
# }
#}


set x_start [expr $val(x)/($val(col)*2)];
set y_start [expr $val(y)/($val(row)*2)];
for {set i 0} {$i < $val(row) } { incr i } {
#in same column
    for {set j 0} {$j < $val(col) } { incr j } {
#in same row
puts "it is i $i it is j $j"
  set m [expr $i*$val(col)+$j];
# $node_($m) set X_ [expr $i*240];
# $node_($m) set Y_ [expr $k*240+20.0];
#CHNG
  if {$grid == 1} {
    set x_pos [expr $x_start+$j*($val(x)/$val(col))];#grid settings
    set y_pos [expr $y_start+$i*($val(y)/$val(row))];#grid settings
  } else {
    set x_pos [expr int($val(x)*rand())] ;#random settings
    set y_pos [expr int($val(y)*rand())] ;#random settings
  }
  $node_($m) set X_ $x_pos;
  $node_($m) set Y_ $y_pos;
  $node_($m) set Z_ 0.0
  puts "now m $m
  "
# puts "$m"
  puts  "$m x: [$node_($m) set X_] y: [$node_($m) set Y_] \n"
    }
    
};







for {set i 0} {$i < $val(nn)} {incr i} {
  $ns_ at 0.0 "$node_($i) label AP$i"
  puts "Labeling node $i"            ;
}

for {set i 0} {$i < $val(nn) } { incr i } {
  set code [expr $i % 4]
  if [expr $code == 0] {
    $ns_ at 0.0 "$node_($i) add-mark AP$i green circle"
  } elseif [expr $code == 1] {
         $ns_ at 0.0 "$node_($i) add-mark AP$i red circle"
    } elseif [expr $code == 2] {
         $ns_ at 0.0 "$node_($i) add-mark AP$i yellow circle"
    } else {
         $ns_ at 0.0 "$node_($i) add-mark AP$i blue circle"
    }
}

$ns_ at 0.0 "$node_([expr $val(nn)-1]) label Sink"


#set AP_ADDR1 [$mac_(0) id]
#$mac_(0) ap $AP_ADDR1
#set AP_ADDR2 [$mac_([expr $val(nn) - 1]) id]
#$mac_([expr $val(nn) - 1]) ap $AP_ADDR2


#$mac_(1) ScanType ACTIVE

#for {set i 3} {$i < [expr $val(nn) - 1]} {incr i} {
#        $mac_($i) ScanType PASSIVE      ;
#}


#$ns_ at 1.0 "$mac_(2) ScanType ACTIVE"
Application/Traffic/CBR set packetSize_ 1023
Application/Traffic/CBR set rate_ 256Kb

       
for {set i 0} {$i < [expr $val(nn) -1]} {incr i} {
        set udp1($i) [new Agent/UDP]

        $ns_ attach-agent $node_($i) $udp1($i)
        set cbr1($i) [new Application/Traffic/CBR]
        $cbr1($i) attach-agent $udp1($i)
        puts "creating agent for node $i"            ;
}



set null0 [new Agent/Null]
$ns_ attach-agent $node_([expr $val(nn) -1]) $null0
for {set i 0} {$i < [expr $val(nn) -1]} {incr i} {
    $ns_ connect $udp1($i) $null0
        puts "Add sink for node $i"            ;
}






for {set i 0} {$i < $val(nn)  } { incr i} {
  $ns_ initial_node_pos $node_($i) 100
}







for {set i 0} {$i < [expr $val(nn) -1]} {incr i} {
    #set time [expr $i % 4]
    $ns_ at 5.0 "$cbr1($i) start"
}

for {set i 30} {$i < [expr $val(ttime)-10]} {} {
        set temp $val(nn)
#         $ns_ at 50.000000000000 "$node_([expr $val(nn)-1]) setdest 50 25 3.371785899154"
        $ns_ at  $i "random_movement $temp $val(x) $val(y)"
        set i [expr $i+20]
#        puts "$i random calling"
      
  } 


for {set i 0} {$i < [expr $val(nn)] } {incr i} {

  $ns_ at [expr $val(ttime)+$val(extraTime)] "$node_($i) reset"
}

$ns_ at [expr $val(ttime)+$val(extraTime)] "stop"
$ns_ at [expr $val(ttime)+$val(extraTime)] "puts \"NS EXITING...\" ; $ns_ halt"



proc random_movement { length x y} {
        global ns_ tracefd node_
        for {set i 0} {$i < [expr $length]} {incr i} {
          set x_pos [expr int($x*rand())] ;#random settings
          set y_pos [expr int($y*rand())] ;#random settings
          $node_($i) setdest $x_pos $y_pos 5.00
 #         puts "executing"
      
  }
    
    
}




proc stop {} {
    global ns_ tracefd
    $ns_ flush-trace
    close $tracefd
    exec nam 1305117_802_15.nam 
    exit 0
}

puts "Starting Simulation..."
$ns_ run

