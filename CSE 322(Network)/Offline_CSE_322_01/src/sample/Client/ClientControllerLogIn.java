/**
 * Sample Skeleton for 'ClientLogin.fxml' Controller Class
 */

package sample.Client;

import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.TextField;
import sample.Network.NetworkUtil;
import sample.Util.TeamList;

import java.net.URL;
import java.util.ResourceBundle;

public class ClientControllerLogIn {
    private ClientMain clientMain;

    @FXML // ResourceBundle that was given to the FXMLLoader
    private ResourceBundle resources;

    @FXML // URL location of the FXML file that was given to the FXMLLoader
    private URL location;

    @FXML // fx:id="serverAddress"
    private TextField serverAddress; // Value injected by FXMLLoader

    @FXML // fx:id="clientConnect"
    private Button clientConnect; // Value injected by FXMLLoader

    @FXML // fx:id="sid"
    private TextField sid; // Value injected by FXMLLoader

    @FXML // fx:id="port"
    private TextField port; // Value injected by FXMLLoader

    @FXML
    void connect(ActionEvent event) {
        String serverIP=serverAddress.getText();
        String serverPort=port.getText();
        String studentId = sid.getText();
        clientMain.sid=studentId;
        clientMain.serverIP=serverIP;
        Thread t=new Thread(new ClientChangeListener(serverIP,Integer.parseInt(serverPort),studentId,clientMain));
        t.start();

    }

    @FXML // This method is called by the FXMLLoader when initialization is complete
    void initialize() {
        assert serverAddress != null : "fx:id=\"serverAddress\" was not injected: check your FXML file 'ClientLogin.fxml'.";
        assert clientConnect != null : "fx:id=\"clientConnect\" was not injected: check your FXML file 'ClientLogin.fxml'.";
        assert sid != null : "fx:id=\"sid\" was not injected: check your FXML file 'ClientLogin.fxml'.";
        assert port != null : "fx:id=\"port\" was not injected: check your FXML file 'ClientLogin.fxml'.";

    }

    public ClientMain getClientMain() {
        return clientMain;
    }

    public void setClientMain(ClientMain clientMain) {
        this.clientMain = clientMain;
    }
}
