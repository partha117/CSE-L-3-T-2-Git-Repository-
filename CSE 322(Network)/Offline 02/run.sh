./setup
echo "Links are established"
g++ -std=c++11 -w -o router router.cpp
echo "Suceesfull compilation"
gnome-terminal -e "./router 192.168.10.1 topo.txt" &
gnome-terminal -e "./router 192.168.10.2 topo.txt" &
gnome-terminal -e "./router 192.168.10.3 topo.txt" &
gnome-terminal -e "./router 192.168.10.4 topo.txt" 
echo "All the routers are up"
echo "Driver is running"
python driver.py topo.txt
