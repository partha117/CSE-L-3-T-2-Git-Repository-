package sample.Client;

import sample.Network.NetworkUtil;
import sample.Util.Constant;
import sample.Util.DataPacket;

import javax.xml.crypto.Data;
import java.io.FileOutputStream;

/**
 * Created by partha on 16-Mar-17.
 */
public class ClientReadThread implements  Runnable {
    private NetworkUtil nc;
    private  String  team1;
    private String  team2;

    public ClientReadThread(NetworkUtil nc, String team1, String team2) {
        this.nc = nc;
        this.team1 = team1;
        this.team2 = team2;
    }

    @Override
    public void run() {


        while(true)
        {
            try {

                DataPacket dp=(DataPacket)nc.read();
                FileOutputStream fos=new FileOutputStream("Client("+team1+"-"+team2+")"+dp.fileName);
                while (true)
                {

                    if(dp.isLast())
                    {
                        fos.write(dp.data);
                        System.out.println("From Client Read got packet"+dp.startsFrom);
                        fos.close();
                        nc.write(Constant.acknowledgement);
                        break;
                    }
                    else
                    {
                        System.out.println("From Client Read got packet"+dp.startsFrom);
                        fos.write(dp.data);
                        dp=(DataPacket)nc.read();
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

    }
}
