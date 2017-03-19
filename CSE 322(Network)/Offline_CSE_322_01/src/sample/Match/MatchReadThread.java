package sample.Match;

import sample.Network.NetworkUtil;
import sample.Server.ServerMain;
import sample.Util.ClientData;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.HashMap;

/**
 * Created by ASUS on 17-Mar-17.
 */
public class MatchReadThread implements  Runnable {
    private  int port;
    private String team1;
    private String  team2;
    private ServerMain serverMain;
    private ArrayList<ClientData> allClient;
    private HashMap<String ,Integer> viewMap;
    public  ScoreCard sc;
    public int over=0;

    public MatchReadThread(int port, String team1, String team2, ServerMain serverMain) {
        this.port = port;
        this.team1 = team1;
        this.team2 = team2;
        this.serverMain = serverMain;
        allClient=new ArrayList<>();
        viewMap=new HashMap<>();
    }

    @Override
    public void run() {

        ServerSocket serverSocket = null;
        try {
            serverSocket = new ServerSocket(port);
        } catch (IOException e) {
            e.printStackTrace();
        }
        while(true)
        {

            try {

                Socket clientSocket = serverSocket.accept();
                ServerSocket finalServerSocket = serverSocket;
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        String sid=null;
                        NetworkUtil nc=new NetworkUtil(clientSocket);
                        try {
                            sid=(String)nc.read();
                            if(!viewMap.containsKey(sid))
                            {
                                viewMap.put(sid,0);
                                allClient.add(new ClientData(nc,sid,0));
                                if(over>0)
                                {

                                    new Thread(new LateInformer(nc,sc,0,over)).start();
                                }

                            }
                            else
                            {

                                new Thread(new LateInformer(nc,sc,lastView(sid)+1,over)).start();
                                for(int i=0;i<allClient.size();i++)
                                {
                                    if(allClient.get(i).getSid().compareTo(sid)==0)
                                    {
                                        allClient.remove(i);

                                    }
                                }
                                allClient.add(new ClientData(nc,sid,lastView(sid)));

                            }
                        } catch (Exception e) {
                            e.printStackTrace();
                        }

                        /*if(serverMain.getClientMap().get(finalServerSocket.getInetAddress().toString())==null)
                        {
                            serverMain.getClientMap().put(finalServerSocket.getInetAddress().toString(),new ArrayList<Pair<String, Integer>>());
                        }
                        serverMain.getClientMap().get(finalServerSocket.getInetAddress().toString()).add(new Pair<>(team1+"-"+team2,0));*/
                    }
                }).start();
                /*Thread t=new Thread(new MatchThread(team1,team2,serverMain,this));
                t.start();*/

            } catch (IOException e) {
                e.printStackTrace();
            }
        }


    }
    public int lastView(String  id)
    {

        for(int i=0;i<allClient.size();i++)
        {
            if(allClient.get(i).getSid().compareTo(id)==0)
            {
                return allClient.get(i).getLast();
            }
        }
        return 0;
    }

    public ArrayList<ClientData> getAllClient() {
        return allClient;
    }
}
