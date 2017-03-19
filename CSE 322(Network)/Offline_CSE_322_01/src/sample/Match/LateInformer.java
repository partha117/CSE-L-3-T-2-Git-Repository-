package sample.Match;

import sample.Network.NetworkUtil;
import sample.Util.Constant;
import sample.Util.DataPacket;

import java.io.*;
import java.util.ArrayList;
import java.util.UUID;

/**
 * Created by ASUS on 18-Mar-17.
 */
public class LateInformer implements Runnable {
    private NetworkUtil nc;
    ScoreCard sc;
    int over;
    private ArrayList<DataPacket> pool;
    int currentOver;

    public LateInformer(NetworkUtil nc, ScoreCard sc, int over,int currentOver) {
        this.nc = nc;
        this.sc = sc;
        this.over = over;
        this.currentOver=currentOver;
    }

    @Override
    public void run() {
        for(int i=over;i<=currentOver;i++)
        {
            int []run=sc.getTeamRun().get(i);
            int temp=run[1]+run[2]+run[3]+run[4]+run[5]+run[6];
            String  matchData="over-"+i+":run-"+temp;
            String  filName=fileWriter(matchData);
            pool=new ArrayList<>();
            createPool(filName);
            for(int j=0;j<pool.size();j++)
            {
                nc.write(pool.get(j));
                System.out.println("From server Write got packet"+pool.get(j).startsFrom);
                try {
                    String  ack=(String)nc.read();
                    if(ack.equals(Constant.acknowledgement))
                    {

                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }

    private String fileWriter(String data)
    {
        String  temp= "ForLate"+UUID.randomUUID().toString()+".txt";
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
