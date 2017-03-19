package sample.Match;

import java.util.ArrayList;

/**
 * Created by partha on 09-Mar-17.
 */
public class ScoreCard {
    private String team1Name;
    private String team2Name;
    private ArrayList<int[]>teamRun;

    private int currentOver=1;
    public int currentBall=1;
    boolean currentBat=false;

    public ScoreCard(String team1,String team2) {
        team1Name=team1;
        team2Name=team2;
        teamRun=new ArrayList<>();

        for(int i=1;i<=40;i++)
        {
            teamRun.add(new int[7]);

        }
    }

    public String getTeam1Name() {
        return team1Name;
    }
    public void set(int run)
    {
        if(currentOver<=40)
        {
            teamRun.get(currentOver)[currentBall] = run;
            currentBall++;
            if (currentBall == 7) {
                currentOver++;
                currentBall = 1;
            }
        }


    }

    public void setTeam1Name(String team1Name) {
        this.team1Name = team1Name;
    }

    public String getTeam2Name() {
        return team2Name;
    }

    public void setTeam2Name(String team2Name) {
        this.team2Name = team2Name;
    }

    public ArrayList<int[]> getTeamRun() {
        return teamRun;
    }
}
