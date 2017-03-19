/**
 * Sample Skeleton for 'server1.fxml' Controller Class
 */

package sample.Server;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.TextField;
import javafx.util.Pair;
import sample.Match.MatchReadThread;
import sample.Match.MatchThread;
import sample.Network.NetworkUtil;
import sample.Util.Constant;
import sample.Util.TeamList;

import java.net.URL;
import java.util.ArrayList;
import java.util.ResourceBundle;

public class ServerControllerMatchStart {
    private  ServerMain main;
    private int port=5000;
    private  int count=1;

    @FXML // ResourceBundle that was given to the FXMLLoader
    private ResourceBundle resources;

    @FXML // URL location of the FXML file that was given to the FXMLLoader
    private URL location;

    @FXML // fx:id="team1"
    private TextField team1; // Value injected by FXMLLoader

    @FXML // fx:id="team2"
    private TextField team2; // Value injected by FXMLLoader

    @FXML // fx:id="start"
    private Button start; // Value injected by FXMLLoader

    @FXML
    void onStart(ActionEvent event) {
        main.getCurrentMatches().add(new Pair<String ,String >(team1.getText(),team2.getText()));
        main.getPortList().add(port);
        System.out.println(main.getCurrentMatches().get(0)+"  "+main.getPortList().get(0));
        MatchReadThread matchReadThread=new MatchReadThread(port,team1.getText(),team2.getText(),main);
        Thread t =new Thread(matchReadThread);
        t.start();
        Thread t1=new Thread(new MatchThread(team1.getText(),team2.getText(),main,matchReadThread));
        t1.start();
        team1.setText("");
        team2.setText("");
        count++;
        port++;
        new Thread(new Runnable() {
            @Override
            public void run() {
                TeamList team = new TeamList(main.getCurrentMatches(), Constant.maxLimit, main.getPortList());
                ArrayList<NetworkUtil> temp=main.getAllClient();
                for(int i=0;i<temp.size();i++)
                {
                    temp.get(i).write(team);
                }

            }
        }).start();

    }

    @FXML // This method is called by the FXMLLoader when initialization is complete
    void initialize() {
        assert team1 != null : "fx:id=\"team1\" was not injected: check your FXML file 'server1.fxml'.";
        assert team2 != null : "fx:id=\"team2\" was not injected: check your FXML file 'server1.fxml'.";
        assert start != null : "fx:id=\"start\" was not injected: check your FXML file 'server1.fxml'.";

    }

    public void setMain(ServerMain main) {
        this.main = main;
    }
}
