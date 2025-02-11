module PROG2_Project_JavaFX {
	requires javafx.controls;
	requires java.desktop;
	requires javafx.swing;

	
	opens application to javafx.graphics, javafx.fxml;
}
