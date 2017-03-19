package sample.Client;

import javafx.application.Application;
import javafx.collections.ObservableList;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.CheckBox;
import javafx.scene.control.ToggleGroup;
import javafx.scene.input.MouseEvent;
import javafx.stage.Stage;
import javafx.util.Pair;
import sample.Network.NetworkUtil;
import sample.Util.Constant;
import sample.Util.TeamList;

import java.util.ArrayList;
import java.util.HashMap;

/**
 * Created by partha on 16-Mar-17.
 */
public class ClientMain extends Application {

    private ToggleGroup group = new ToggleGroup();
    private Stage stage;
    public ClientControllerLogIn clientControllerLogIn;
    public ClientControllerChoiceList clientControllerChoiceList;
    public NetworkUtil mainServer;
    public HashMap<String, NetworkUtil> currentStreaming;
    public ArrayList< Pair<String ,String >>selectedList;
    public  int maxLimit=0;
    public  TeamList teamData;
    public  String serverIP;
    public String sid;

    @Override
    public void start(Stage primaryStage) throws Exception {

        FXMLLoader loader = new FXMLLoader();
        loader.setLocation(getClass().getResource("ClientLogIn.fxml"));
        Parent root = loader.load();

        clientControllerLogIn = loader.getController();
        clientControllerLogIn.setClientMain(this);
        stage = primaryStage;
        primaryStage.setTitle("Client");
        primaryStage.setScene(new Scene(root, 551, 315));
        primaryStage.show();
        currentStreaming = new HashMap<>();
    }

    public void ChangeWindow1(TeamList team) throws Exception {
        FXMLLoader loader = new FXMLLoader();
        loader.setLocation(getClass().getResource("ClientChoiceList.fxml"));
        Parent root = loader.load();
        stage.setTitle("ChoiceList");
        stage.setScene(new Scene(root, 857, 600));
        stage.show();
        selectedList=new ArrayList<>();
        maxLimit=team.getMaxLimit();
        teamData=team;
         clientControllerChoiceList = loader.getController();
        clientControllerChoiceList.label.setText("Maximum Streaming limit :" + maxLimit);
        clientControllerChoiceList.setClientMain(this);
        for (int i = 0; i < team.getArrayList().size(); i++) {
            CheckBox cb = new CheckBox(team.getArrayList().get(i).getKey() + "-" + team.getArrayList().get(i).getValue());
            //cb.setMinWidth(40);
            clientControllerChoiceList.box.getChildren().add(cb);
            cb.addEventHandler(MouseEvent.MOUSE_CLICKED,
                    (event -> checker(cb.getUserData())));
            cb.setUserData(team.getArrayList().get(i).getKey() + "-" + team.getArrayList().get(i).getValue());
        }
    }
    private void checker(Object ob)
    {
        String st=(String )ob;
        String [] temp=st.split("-");
        for(int i=0;i<selectedList.size();i++)
        {
            if((selectedList.get(i).getKey().compareTo(temp[0])==0)&&(selectedList.get(i).getValue().compareTo(temp[1])==0))
            {
                selectedList.remove(i);
                return;
            }

        }
        selectedList.add(new Pair<>(temp[0],temp[1]));

    }
    public  void updateList(TeamList teamData)
    {
        ObservableList<Node> temp=clientControllerChoiceList.box.getChildren();
        boolean exist;
        for(int i=0;i<teamData.getArrayList().size();i++)
        {
            exist=false;
            String name=teamData.getArrayList().get(i).getKey()+"-"+teamData.getArrayList().get(i).getValue();
            for(int j=0;j<temp.size();j++)
            {
                if(((String)temp.get(j).getUserData()).compareTo(name)==0)
                {
                    exist=true;
                }
            }
            if(!exist)
            {
                CheckBox cb = new CheckBox(name);
                //cb.setMinWidth(40);
                clientControllerChoiceList.box.getChildren().add(cb);
                cb.addEventHandler(MouseEvent.MOUSE_CLICKED,
                        (event -> checker(cb.getUserData())));
                cb.setUserData(name);
            }

        }
    }
}