package application;

import javafx.geometry.Pos;
import javafx.scene.control.Alert;
import javafx.scene.layout.GridPane;
import javafx.geometry.Insets;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;

public class ConnectionAlert extends Alert{
	
private TextField nameField = new TextField();
private TextField timeField = new TextField();
	
	public ConnectionAlert(String fromName, String toName) {
		super(AlertType.CONFIRMATION);
		GridPane grid = new GridPane();
		
		grid.setAlignment(Pos.BASELINE_CENTER);
		grid.setPadding(new Insets(10));
		grid.setHgap(5);
		grid.setVgap(10);
		
		grid.addRow(0, new Label("Name:"), nameField);
		grid.addRow(1, new Label("Time:"), timeField);
		setTitle("Connection");
		setHeaderText("Connection from " + fromName + " to " + toName);
		getDialogPane().setContent(grid);
	}
	
	public String getName() {
		return nameField.getText();
	}
	
	public int getTime() {
		return Integer.parseInt(timeField.getText());
	}
	
	public void setNameField(String name) {
		nameField.setText(name);
		nameField.setEditable(false);
	}
	
	public void setTimeField(String time) {
		timeField.setText(time);
		timeField.setEditable(false);
	}

}
