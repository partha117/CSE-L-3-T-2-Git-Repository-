package sample.Server;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;
import javafx.util.Pair;
import sample.Network.NetworkUtil;

import java.io.IOException;
import java.net.InetAddress;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.StringJoiner;

/**
 * Created by partha on 16-Mar-17.
 */
public class ServerMain extends Application {
    private Stage stage;
    //private HashMap <String,NetworkUtil>clientinfo;//<match,nc>
    private  ArrayList<Pair<String ,String >>CurrentMatches;//
    //private Hashtable<String ,ArrayList<Pair<String ,Integer>>> clientMap;//<ip,<<matchname,lastview>>>
    private ArrayList<Integer> portList;
    private ArrayList<String> allowedStudent;
    private  HashMap<String ,String > studentIPMapping;
    private ArrayList<NetworkUtil>allClient;

    @Override
    public void start(Stage primaryStage) throws Exception {
            studentIPMapping=new HashMap<>();
            allClient=new ArrayList<>();
            stage=primaryStage;
            FXMLLoader loader = new FXMLLoader();
            loader.setLocation(getClass().getResource("ServerConfiguration.fxml"));
            Parent root=loader.load();
            ServerConfigurationController serverConfigurationController=loader.getController();
            serverConfigurationController.setMain(this);
            primaryStage.setTitle("Configuration");
            primaryStage.setScene(new Scene(root,600,400));
            primaryStage.show();




    }
    public  void ChangeWindow()
    {
        CurrentMatches = new ArrayList<>();
        //clientMap = new Hashtable<>();
        portList = new ArrayList<>();

        FXMLLoader loader = new FXMLLoader();
        loader.setLocation(getClass().getResource("ServerMatchStart.fxml"));

        Parent root = null;
        try {
            root = loader.load();
            ServerControllerMatchStart serverControllerMatchStart = loader.getController();
            serverControllerMatchStart.setMain(this);

            stage.setTitle("Server");
            stage.setScene(new Scene(root, 726, 456));
            stage.show();
            Thread t = new Thread(new ServerReadThread(this));
            t.start();
        } catch (IOException e) {
            e.printStackTrace();
        }


    }

    public static void main(String[] args) {
        launch(args);
    }

    /*public HashMap<String, NetworkUtil> getClientinfo() {
        return clientinfo;
    }*/

    public ArrayList<Pair<String, String>> getCurrentMatches() {
        return CurrentMatches;
    }

   /* public Hashtable<String, ArrayList<Pair<String, Integer>>> getClientMap() {
        return clientMap;
    }*/

    public ArrayList<Integer> getPortList() {
        return portList;
    }

    public void setAllowedStudent(ArrayList<String> allowedStudent) {
        this.allowedStudent = allowedStudent;
    }

    public HashMap<String, String> getStudentIPMapping() {
        return studentIPMapping;
    }

    public ArrayList<String> getAllowedStudent() {
        return allowedStudent;
    }

    public ArrayList<NetworkUtil> getAllClient() {
        return allClient;
    }

    public void setAllClient(ArrayList<NetworkUtil> allClient) {
        this.allClient = allClient;
    }
}
