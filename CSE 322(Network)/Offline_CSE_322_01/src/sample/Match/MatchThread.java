package sample.Match;

import sample.Server.ServerMain;

import java.util.Random;

/**
 * Created by partha on 09-Mar-17.
 */
public class MatchThread implements Runnable {
    private String team1;
    private String team2;
    private ServerMain main;
    private  MatchReadThread matchReadThread;
    private int team1Run=0;
    private int team1Wicket=0;
    private int team2Run=0;
    private int team2Wicket=0;

    public MatchThread(String team1, String team2, ServerMain main, MatchReadThread matchReadThread) {
        this.team1 = team1;
        this.team2 = team2;
        this.main = main;
        this.matchReadThread = matchReadThread;
    }

    @Override
    public void run() {
        int run=0;
        int runInOver=0;
        int wicketInover=0;
        int runInInningns=0;
        int wicketInInnigns=0;
        //Random random=Math.random();
        ScoreCard scoreCard=new ScoreCard(team1,team2);
        matchReadThread.sc=scoreCard;


        for(int innigns=1;innigns<=2;innigns++)
        {
            for(int over=1;over<=20;over++)
            {

                if(wicketInInnigns<10) {
                    for (int ball = 1; ball <= 6; ball++) {
                        if (wicketInInnigns < 10) {
                            try {
                                Thread.sleep(3000);
                            } catch (InterruptedException e) {
                                e.printStackTrace();
                            }
                            run = (int) (Math.round(Math.random())%8);
                            if (run == 7) {
                                wicketInover++;
                                wicketInInnigns++;
                            }
                            runInOver += run;
                            runInInningns += run;
                            scoreCard.set(run);
                            System.out.println("Match Thread: "+team1+"-"+team2+": over-"+over+"Ball-"+ball+"run-"+run);

                        } else {
                            String data = "Over-" + (over) + ":Run-" + runInOver + ":Wicket-" + wicketInover;
                            Thread t = new Thread(new MatchWriteThread(matchReadThread.getAllClient(), data,matchReadThread,over));
                            t.start();
                            runInOver = 0;
                            wicketInover = 0;
                            break;
                        }

                    }
                }
                else
                {
                    break;
                }

                matchReadThread.over=over;
                //System.out.println("Mtach Thread :" + team1 + "-" + team2 + " over: " + over + " ball " + ball + " run " + run);
                String data = "Over-" + (over) + ":Run-" + runInOver + ":Wicket-" + wicketInover;
                Thread t = new Thread(new MatchWriteThread(matchReadThread.getAllClient(), data,matchReadThread,over));
                t.start();
                runInOver = 0;
                wicketInover = 0;
            }

            team1Run=runInInningns;
            team1Wicket=wicketInInnigns;
            runInInningns=wicketInInnigns=0;
        }
        team2Run=runInInningns;
        team2Wicket=wicketInInnigns;
        if(team1Run>team2Run)
        {
            String data = team1+"wins";
            Thread t = new Thread(new MatchWriteThread(matchReadThread.getAllClient(), data,matchReadThread,40));
            t.start();
        }




    }
}
