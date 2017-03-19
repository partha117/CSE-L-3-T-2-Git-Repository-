/**
 * Sample Skeleton for 'ServerConfiguration.fxml' Controller Class
 */

package sample.Server;

import java.net.URL;
import java.util.ArrayList;
import java.util.ResourceBundle;

import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;
import sample.Util.Constant;

public class ServerConfigurationController {
    private ServerMain main;

    @FXML // ResourceBundle that was given to the FXMLLoader
    private ResourceBundle resources;

    @FXML // URL location of the FXML file that was given to the FXMLLoader
    private URL location;

    @FXML // fx:id="allowedList"
    private TextArea allowedList; // Value injected by FXMLLoader

    @FXML // fx:id="sLimit"
    private TextField sLimit; // Value injected by FXMLLoader

    @FXML // fx:id="done"
    private Button done; // Value injected by FXMLLoader

    @FXML
    void Complete(ActionEvent event) {

        Constant.maxLimit=Integer.parseInt(sLimit.getText());
        main.setAllowedStudent(parser(allowedList.getText()));
        Platform.runLater(new Runnable() {
            @Override
            public void run() {
                main.ChangeWindow();
            }
        });


    }

    @FXML // This method is called by the FXMLLoader when initialization is complete
    void initialize() {
        assert allowedList != null : "fx:id=\"allowedList\" was not injected: check your FXML file 'ServerConfiguration.fxml'.";
        assert sLimit != null : "fx:id=\"sLimit\" was not injected: check your FXML file 'ServerConfiguration.fxml'.";
        assert done != null : "fx:id=\"done\" was not injected: check your FXML file 'ServerConfiguration.fxml'.";

    }

    public void setMain(ServerMain main) {
        this.main = main;
    }
    public ArrayList<String> parser(String  list)
    {
        ArrayList<String > studentList=new ArrayList<>();
        if(list.contains("-"))
        {

            String temp[]=list.split("-");
            int num1=Integer.parseInt(temp[0]);
            int num2=Integer.parseInt(temp[1]);
            for(int i=num1;i<=num2;i++)
            {
                studentList.add(String.valueOf(i));
            }

        }
        else
        {
            String temp[] = list.split(",");
            for (int i = 0; i < temp.length; i++) {
                studentList.add(temp[i]);
            }
        }
        return studentList;

    }
}
