package sample.Util;

import javafx.util.Pair;

import java.io.Serializable;
import java.util.ArrayList;

/**
 * Created by partha on 11-Mar-17.
 */
public class TeamList implements Serializable{
    private String[] list;
    private int maxLimit=0;
    private  int []portList;

    public TeamList(ArrayList<Pair<String ,String >> team,int limit,ArrayList<Integer>port) {
        list=new String[team.size()];
        portList=new int[port.size()];
        for(int i=0;i<team.size();i++)
        {
            list[i]=team.get(i).getKey()+" -VS- "+team.get(i).getValue();
            portList[i]=port.get(i);
        }
        maxLimit=limit;
    }

    public TeamList(ArrayList<Pair<String ,String >> team) {
        list=new String[team.size()];
        for(int i=0;i<team.size();i++)
        {
            list[i]=team.get(i).getKey()+" -VS- "+team.get(i).getValue();
        }
        maxLimit=0;
    }

    public  ArrayList<Pair<String ,String >> getArrayList()
    {
        if(list!=null)
        {
            ArrayList<Pair<String,String >> arrayList=new ArrayList<>();
            for(int i=0;i<list.length;i++)
            {
                String temp[]=list[i].split("-VS-");
                arrayList.add(new Pair<>(temp[0],temp[1]) );
            }
            return arrayList;
        }
        return null;
    }
    public  ArrayList<Integer> getPortList()
    {
        if(portList!=null)
        {
            ArrayList<Integer> arrayList=new ArrayList<>();
            for(int i=0;i<portList.length;i++)
            {

                arrayList.add(portList[i]);
            }
            return arrayList;
        }
        return null;
    }

    public int getMaxLimit() {
        return maxLimit;
    }
}
