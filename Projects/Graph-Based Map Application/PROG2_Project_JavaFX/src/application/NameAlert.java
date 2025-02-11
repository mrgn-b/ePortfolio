package application;

import javafx.geometry.Pos;
import javafx.scene.control.Alert;
import javafx.scene.layout.GridPane;
import javafx.geometry.Insets;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;

public class NameAlert extends Alert{
	private TextField nameField = new TextField();
	
	public NameAlert() {
		super(AlertType.CONFIRMATION);
		GridPane grid = new GridPane();
		
		grid.setAlignment(Pos.BASELINE_CENTER);
		grid.setPadding(new Insets(10));
		grid.setHgap(5);
		grid.setVgap(10);
		
		grid.addRow(0, new Label("Name of place:"), nameField);
		setHeaderText(null);
		getDialogPane().setContent(grid);
	}
	
	public String getName() {
		return nameField.getText();
	}

}
