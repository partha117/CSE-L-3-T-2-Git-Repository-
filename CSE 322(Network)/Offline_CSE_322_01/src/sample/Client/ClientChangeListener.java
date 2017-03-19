package sample.Client;

import javafx.application.Platform;
import sample.Network.NetworkUtil;
import sample.Util.TeamList;

/**
 * Created by ASUS on 18-Mar-17.
 */
public class ClientChangeListener implements Runnable {
    private String serverIP;
    private  int  serverPort;
    private String SID;
    private ClientMain clientMain;

    public ClientChangeListener(String serverIP, int serverPort, String SID, ClientMain clientMain) {
        this.serverIP = serverIP;
        this.serverPort = serverPort;
        this.SID = SID;
        this.clientMain = clientMain;
    }

    @Override
    public void run() {

        NetworkUtil nc=new NetworkUtil(serverIP,serverPort);
        clientMain.mainServer=nc;

        nc.write(SID);
        try {
            TeamList teamList=(TeamList) (nc.read());
            clientMain.maxLimit=teamList.getMaxLimit();
            Platform.runLater(new Runnable() {
                @Override
                public void run() {
                    try {
                        clientMain.teamData=teamList;
                        clientMain.ChangeWindow1(teamList);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            });

        } catch (Exception e) {
            e.printStackTrace();
        }

        while(true)
        {

            try {
                TeamList teamList=(TeamList) (nc.read());
                Platform.runLater(new Runnable() {
                    @Override
                    public void run() {
                        clientMain.teamData=teamList;
                        clientMain.updateList(teamList);
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

    }
}
