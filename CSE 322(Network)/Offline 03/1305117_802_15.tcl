# ============================================================================
# Network set up
# ============================================================================
set val(chan)           Channel/WirelessChannel    ;#Channel Type
set val(prop)           Propagation/TwoRayGround   ;# radio-propagation model
set val(netif)          Phy/WirelessPhy/802_15_4            ;# network interface type
set val(mac)            Mac/802_15_4                 ;# MAC type
set val(ifq)            CMUPriQueue             ;# interface queue type Queue/DropTail/PriQueue CMUPriQueue
set val(ll)             LL                         ;# link layer type
set val(ant)            Antenna/OmniAntenna        ;# antenna model
set val(ifqlen)         50                        ;# max packet in ifq                        ;# number of mobilenodes
set val(rp)             DSDV               ;# routing protocol
set val(x)              600
set val(y)              600
set val(row)             0
set val(col)             0
set val(nn)              [lindex $argv 0]
set val(flow)            [lindex $argv 1]
set grid                 1
set val(ttime)           100
set val(extraTime)       10
set val(velocity)        [lindex $argv 2]
set val(pps)             [lindex $argv 3]
set val(packetSize)      128
set val(rate)             0
# ==========================================================================
# Mac settings
# =========================================================================
Mac/802_15_4 set dataRate_ 11Mb
Mac/802_15_4 set syncFlag_ 1
Mac/802_15_4 set dutyCycle_ cbr_interval
# ===========================================================================
# Energy parameter
# ===========================================================================
set val(energymodel_11)    EnergyModel     ;
set val(initialenergy_11)  1000            ;# Initial energy in Joules

set val(idlepower_11) 869.4e-3      ;#LEAP (802.11g) 
set val(rxpower_11) 1560.6e-3     ;#LEAP (802.11g)
set val(txpower_11) 1679.4e-3     ;#LEAP (802.11g)
set val(sleeppower_11) 37.8e-3      ;#LEAP (802.11g)
set val(transitionpower_11) 176.695e-3    ;#LEAP (802.11g)  
set val(transitiontime_11) 2.36     ;#LEAP (802.11g)
# =============================================================================
# Calculating row column
# =============================================================================
      for {set i 2} {$i < [expr $val(nn)]} {incr i} {
                    if [expr $val(nn)%5==0] {
                      set val(row) 5
                      set val(col) [expr $val(nn)/5]
                      break
                    }
                    set temp  [expr $val(nn) % $i]          
                    if [ expr $temp == 0] {
                    set val(row) $i
                    set val(col) [expr $val(nn)/$i]
                  }  
            }
# =============================================================================
#  Global Variables
# =============================================================================
#remove-all-packet-headers  
#add-packet-header IP Message  
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
# ===============================================================
# Node set up
# ===============================================================
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
                -channel $chan_1_  \
                -energyModel $val(energymodel_11) \
                -idlePower $val(idlepower_11) \
                -rxPower $val(rxpower_11) \
                -txPower $val(txpower_11) \
                -sleepPower $val(sleeppower_11) \
                -transitionPower $val(transitionpower_11) \
                -transitionTime $val(transitiontime_11) \
                -initialEnergy $val(initialenergy_11)

      for {set i 0} {$i < [expr $val(nn)]} {incr i} {
                  set node_($i) [$ns_ node]
                   $node_($i) random-motion 0
                   $node_($i) start
                  #puts "creating node $i"            ;
               # set mac_($i) [$node_($i) getMac 0]
                # $mac_($i) set RTSThreshold_ 3000       
            }


# =================================================================
# Calculating position of node
# =================================================================
set x_start [expr $val(x)/($val(col)*2)];
set y_start [expr $val(y)/($val(row)*2)];
for {set i 0} {$i < $val(row) } { incr i } {
#in same column
    for {set j 0} {$j < $val(col) } { incr j } {
    set m [expr $i*$val(col)+$j];

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
  
    }
    
};







for {set i 0} {$i < $val(nn)} {incr i} {
  $ns_ at 0.0 "$node_($i) label AP$i"
  #puts "Labeling node $i"            ;
}

for {set i 0} {$i < $val(nn) } { incr i } {
  set code [expr $i % 4]
  if [expr $code == 0] {
    $ns_ at 0.0 "$node_($i) add-mark AP$i orange circle"
  } elseif [expr $code == 1] {
         $ns_ at 0.0 "$node_($i) add-mark AP$i red circle"
    } elseif [expr $code == 2] {
         $ns_ at 0.0 "$node_($i) add-mark AP$i yellow circle"
    } else {
         $ns_ at 0.0 "$node_($i) add-mark AP$i blue circle"
    }
}

$ns_ at 0.0 "$node_([expr $val(nn)-1]) label Sink"

set val(rate) [expr int(ceil($val(packetSize)*double($val(pps))/1000))]
Application/Traffic/CBR set packetSize_ $val(packetSize)
Application/Traffic/CBR set rate_ $val(rate)Kb
# ============================================================
# Info printing
# ============================================================
puts "Nodes             : $val(nn)"
puts "Flow              : $val(flow)"
puts "Packet per second : $val(pps)"
puts "Speed             : $val(velocity)"
# ========================================================
# ========================================================
# Number of sink calculating
# ========================================================
set nSink [ expr int(ceil(double($val(flow))/[expr $val(nn)-1]))]
# ====================================================================
# Creating data node
# ====================================================================
set j 0
for {set i 0} {$i < [expr $val(nn) -$nSink]} {incr i} {
        set udp1($i) [new Agent/UDP]
        $ns_ attach-agent $node_($i) $udp1($i)
        set cbr1($i) [new Application/Traffic/CBR]
        $cbr1($i) attach-agent $udp1($i)
        #puts "creating agent for node $i"            ;
}
# ========================================================================
# Creating Null node
# =======================================================================
for {set i [expr $val(nn) -$nSink]} {$i < [expr $val(nn)]} {incr i} {

        set null($j) [new Agent/Null]
        $ns_ attach-agent $node_($i) $null($j)
        $ns_ at 0.0 "$node_([expr $i]) label Sink$j"
        incr j           ;
}



# ====================================================================
# Connecting data node with null node
# ====================================================================
set lnode [expr $val(nn)-$nSink]
set prev -1
set nodeStart 0

for {set i 0} {$i < [expr $val(flow)]} {incr i} {
    
    set con [expr int(ceil([expr $i/$lnode]))]
    if [expr $prev != $con] {
      set prev $con
      set nodeStart 0
    }
    $ns_ connect $udp1($nodeStart) $null($con)
    #puts "connecting $nodeStart with $con"
    incr nodeStart ;
}
# =====================================================================
# Setting node size
# =======================================================================






for {set i 0} {$i < $val(nn)  } { incr i} {
  $ns_ initial_node_pos $node_($i) 100
}
# ===============================================================
# Setting Random movement
# ================================================================
for {set i 5} {$i < [expr $val(ttime)-10]} {} {
        set temp $val(nn)
        $ns_ at  $i "random_movement $temp $val(x) $val(y) $val(velocity)"
        set i [expr $i+30]
      
  } 

proc random_movement { length x y v} {
        global ns_ tracefd node_
        for {set i 0} {$i < [expr $length]} {incr i} {

          set x_pos [expr int($x*rand())] ;#random settings
          set y_pos [expr int($y*rand())] ;#random settings
          if [expr $x_pos == $x] {
             set x_pos [expr $x_pos-10];
          }
          if [expr $y_pos == $y] {
             set y_pos [expr $y_pos-10];
          }
          if [expr $x_pos == 0] {
             set x_pos [expr $x_pos+10];
          }
          if [expr $y_pos == 0] {
             set y_pos [expr $y_pos+10];
          }

          $node_($i) setdest $x_pos $y_pos $v
      
  }
    
    
}




for {set i 0} {$i < [expr $val(nn)-$nSink]} {incr i} {
    #set time [expr $i % 4]
    $ns_ at 5.0 "$cbr1($i) start"
}



for {set i 0} {$i < [expr $val(nn)-$nSink] } {incr i} {

  $ns_ at [expr $val(ttime)] "$cbr1($i) stop"
}


$ns_ at [expr $val(ttime)] "puts \"Simulation end\""
$ns_ at [expr $val(ttime)+$val(extraTime)+$val(extraTime)] "puts \"NS EXITING...\" ; $ns_ halt"
$ns_ at [expr $val(ttime)+$val(extraTime)] "stop"








proc stop {} {
    global ns_ tracefd
    $ns_ flush-trace
    close $tracefd
    #exec nam 1305117_802_15.nam &
    exit 0
}

puts "Starting Simulation..."
$ns_ run

