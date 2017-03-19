/**
 * Sample Skeleton for 'ClientChoiceList.fxml' Controller Class
 */

package sample.Client;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.VBox;
import javafx.util.Pair;
import sample.Network.NetworkUtil;
import sample.Util.TeamList;

import java.net.URL;
import java.util.ArrayList;
import java.util.ResourceBundle;

public class ClientControllerChoiceList {
    private ClientMain clientMain;

    @FXML // ResourceBundle that was given to the FXMLLoader
    private ResourceBundle resources;

    @FXML // URL location of the FXML file that was given to the FXMLLoader
    private URL location;

    @FXML // fx:id="pane"
    private AnchorPane pane; // Value injected by FXMLLoader

    @FXML // fx:id="label"
    public Label label; // Value injected by FXMLLoader

    @FXML // fx:id="box"
    public VBox box; // Value injected by FXMLLoader

    @FXML // fx:id="ok"
    private Button ok; // Value injected by FXMLLoader

    @FXML
    void selected(ActionEvent event) {
        ArrayList list=clientMain.selectedList;
        if(clientMain.maxLimit>=list.size())
        {
            new Thread(new Runnable() {
                @Override
                public void run() {
                    for(int i=0;i<clientMain.selectedList.size();i++)
                    {
                        clientMain.maxLimit--;

                        NetworkUtil nc=new NetworkUtil(clientMain.serverIP,find(clientMain.teamData,clientMain.selectedList.get(i)));
                        nc.write(clientMain.sid);
                        clientMain.currentStreaming.put(clientMain.selectedList.get(i).getKey()+"-"+clientMain.selectedList.get(i).getValue(),nc);
                        Thread t= new Thread(new ClientReadThread(nc,clientMain.selectedList.get(i).getKey(),clientMain.selectedList.get(i).getValue()));
                        t.start();

                    }

                }
            }).start();
        }

    }

    @FXML // This method is called by the FXMLLoader when initialization is complete
    void initialize() {
        assert pane != null : "fx:id=\"pane\" was not injected: check your FXML file 'ClientChoiceList.fxml'.";
        assert label != null : "fx:id=\"label\" was not injected: check your FXML file 'ClientChoiceList.fxml'.";
        assert box != null : "fx:id=\"box\" was not injected: check your FXML file 'ClientChoiceList.fxml'.";
        assert ok != null : "fx:id=\"ok\" was not injected: check your FXML file 'ClientChoiceList.fxml'.";

    }

    public ClientMain getClientMain() {
        return clientMain;
    }

    public void setClientMain(ClientMain clientMain) {
        this.clientMain = clientMain;
    }
    public int find(TeamList teamData, Pair<String ,String > match)
    {
        ArrayList<Pair<String,String >> team=teamData.getArrayList();
        for(int i=0;i<team.size();i++)
        {
            if(((team.get(i).getKey().equals(match.getKey()))&&(team.get(i).getValue().equals(match.getValue())))
                ||((team.get(i).getKey().equals(match.getValue()))&&(team.get(i).getValue().equals(match.getKey()))))
            {
                System.out.println("From Client: port "+teamData.getPortList().get(i));
                return teamData.getPortList().get(i);
            }
        }
        return -1;
    }
}
