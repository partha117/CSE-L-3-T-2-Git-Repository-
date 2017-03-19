package sample.Server;

import javafx.util.Pair;
import sample.Network.NetworkUtil;
import sample.Util.Constant;
import sample.Util.TeamList;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;

/**
 * Created by partha on 16-Mar-17.
 */
public class ServerReadThread implements  Runnable {
    private  ServerMain serverMain;

    public ServerReadThread(ServerMain serverMain) {
        this.serverMain = serverMain;
    }

    @Override
    public void run() {


        ServerSocket serverSocket = null;
        try {
            serverSocket = new ServerSocket(Constant.serverPort);
        } catch (IOException e) {
            e.printStackTrace();
        }

        while(true)
        {
            try {
                Socket clientSocket = serverSocket.accept();
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        NetworkUtil nc=new NetworkUtil(clientSocket);
                        try {
                            String  id=(String )nc.read();
                            if(serverMain.getAllowedStudent().contains(id))
                            {
                                serverMain.getAllClient().add(nc);
                                /*if (!serverMain.getClientMap().containsKey(clientSocket.getInetAddress())) {
                                    serverMain.getClientMap().put(clientSocket.getInetAddress().toString(), new ArrayList<>());
                                }*/

                                TeamList team = new TeamList(serverMain.getCurrentMatches(), Constant.maxLimit, serverMain.getPortList());
                                nc.write(team);
                            }
                        } catch (Exception e) {
                            e.printStackTrace();
                        }

                    }
                }).start();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }


    }
}
