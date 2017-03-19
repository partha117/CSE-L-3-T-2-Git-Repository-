package sample.Match;

import sample.Network.NetworkUtil;
import sample.Util.ClientData;
import sample.Util.Constant;
import sample.Util.DataPacket;

import java.io.*;
import java.util.ArrayList;
import java.util.UUID;

/**
 * Created by ASUS on 17-Mar-17.
 */
public class MatchWriteThread implements  Runnable {

    ArrayList<ClientData> client;
    String  matchData;
    private ArrayList<DataPacket> pool;
    private  MatchReadThread matchReadThread;
    private  int over;

    public MatchWriteThread(ArrayList<ClientData> client, String matchData, MatchReadThread matchReadThread,int over) {
        this.client = client;
        this.matchData = matchData;
        this.matchReadThread=matchReadThread;
        pool=new ArrayList<>();
        this.over=over;
    }

    @Override
    public void run() {

        String  filName=fileWriter(matchData);
        createPool(filName);

        for(int i=0;i<client.size();i++)
        {

            for(int j=0;j<pool.size();j++)
            {
                client.get(i).getNetworkUtil().write(pool.get(j));
                System.out.println("From server Write got packet"+pool.get(j).startsFrom);
                try {
                    String  ack=(String)client.get(i).getNetworkUtil().read();
                    if(ack.equals(Constant.acknowledgement))
                    {
                        client.get(i).seen(over);
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }

        }




    }

    private String fileWriter(String data)
    {
        String  temp=UUID.randomUUID().toString()+".txt";
        BufferedWriter writer = null;
        try {
            writer = new BufferedWriter( new FileWriter(temp));
            writer.write(data);
            writer.close();
            return temp;
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }
    /*private int size(String file)
    {
        File fp=new File(file);
        int num=(int)Math.ceil(file.length()/ Constant.packetSize);
        return num;
    }*/
    private void createPool(String  fileName)
    {
        File fp=new File(fileName);
        try {
            FileInputStream fis=new FileInputStream(fp);

        int totalDP=(int)Math.ceil(((double) fp.length())/ ((double) Constant.packetSize));
        int size=(int)fp.length();
        int left=size;
        int startsFrom=0;

        for(int i=0;i<totalDP;i++)
        {
            if(left<Constant.packetSize)
            {
                byte[] barray=new byte[left];
                fis.read(barray);
                DataPacket dp = new DataPacket(fileName, startsFrom,left,barray);
                left=0;
                startsFrom=size;
                dp.last=true;
                pool.add(dp);
            }
            else
            {
                byte[] barray=new byte[Constant.packetSize];
                fis.read(barray);
                DataPacket dp = new DataPacket(fileName, startsFrom,Constant.packetSize,barray);
                left=left-Constant.packetSize;
                startsFrom=startsFrom+Constant.packetSize;
                pool.add(dp);
            }
        }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}
