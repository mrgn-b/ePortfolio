package application;

import javafx.application.Application;
import javafx.geometry.Pos;
import javafx.stage.Stage;
import javafx.scene.layout.BorderPane;
import javafx.scene.Cursor;
import javafx.scene.Scene;
import javafx.scene.control.Menu;
import javafx.scene.control.MenuItem;
import javafx.scene.control.MenuBar;
import javafx.scene.layout.FlowPane;
import javafx.scene.control.Button;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.Pane;
import javax.imageio.ImageIO;
import javafx.scene.control.Alert;
import javafx.scene.control.Alert.AlertType;
import javafx.scene.control.ButtonType;
import javafx.scene.image.WritableImage;
import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.Optional;
import javafx.embed.swing.SwingFXUtils;
import javafx.stage.WindowEvent;

//add unsaved changes
//add exit
//set id to things

public class PathFinder extends Application{
	private VBox verticalBox;
	private Stage primaryStage;
	private Pane outputArea = new Pane();
	private Scene scene;
	private FlowPane flowPane = new FlowPane();
	private Button btnFindPath = new Button();
	private Button btnShowConnection = new Button();
	private Button btnNewPlace = new Button();
	private Button btnNewConnection = new Button();
	private Button btnChangeConnection = new Button();
	private ListGraph<Node> listGraph = new ListGraph<>();
	private Node selectedNode1;
	private Node selectedNode2;
//	private ArrayList<Node> nodes = new ArrayList<>();
	private ArrayList<Connection> connections = new ArrayList<>();
	private BorderPane root;
	private FlowPane pane;
	private MenuBar menu;
	private boolean unsavedChanges = false;
//	private ImageView newMapImageView;
	
	public static void main(String[] args) {
		launch(args);
	}

	@Override
	public void start(Stage primaryStage) {
		this.primaryStage = primaryStage;
		root = new BorderPane();
		scene = new Scene(root, 550, 75);
		primaryStage.setTitle("PathFinder");
		
		pane = new FlowPane();
	    pane.setAlignment(Pos.BASELINE_CENTER);
		menu = makeMenu();
	    
	    makeButtons(pane);
	    
		verticalBox = new VBox(10);
		verticalBox.setAlignment(Pos.BASELINE_CENTER);
		verticalBox.getChildren().addAll(menu, pane);
	    pane.setHgap(8);
	    pane.setVgap(8);
	    root.setCenter(verticalBox);
	    
	    flowPane.setAlignment(Pos.BASELINE_CENTER);
		flowPane.getChildren().add(outputArea);
		verticalBox.getChildren().add(flowPane);
	    outputArea.setId("outputArea");
	    
	    primaryStage.setOnCloseRequest(new ExitHandler());
	    	
		primaryStage.setScene(scene);
		primaryStage.show();
	}
	
	private MenuBar makeMenu() {
		Menu menuFile = new Menu("File");
		menuFile.setId("menuFile");
		MenuItem menuNewMap = new MenuItem("New Map");
		menuNewMap.setId("menuNewMap");
		MenuItem menuOpenFile = new MenuItem("Open");
		menuOpenFile.setId("menuOpenFile");
		MenuItem menuSaveFile = new MenuItem("Save");
		menuSaveFile.setId("menuSaveFile");
		MenuItem menuSaveImage = new MenuItem("Save Image");
		menuSaveImage.setId("menuSaveImage");
		MenuItem menuExit = new MenuItem("Exit");
		menuExit.setId("menuExit");
		menuFile.getItems().addAll(menuNewMap, menuOpenFile, menuSaveFile, menuSaveImage, menuExit);
		MenuBar menu = new MenuBar(menuFile);
		menu.setId("menu");
		
		menuNewMap.setOnAction(new EventHandler<ActionEvent>() {
	    	public void handle(ActionEvent event) {
	    		clearMap();
	    		createMap("europa.gif");
	    	}		
	    });
		
		menuOpenFile.setOnAction(new EventHandler<ActionEvent>() {
			public void handle(ActionEvent event) {
				if (unsavedChanges) {
					openFileConfirmation();
				} else {
					clearMap();
					openFile();	
				}
			}
		});
		
		menuSaveFile.setOnAction(new EventHandler<ActionEvent>() {
			public void handle(ActionEvent event) {
				saveFile();
			}
		});
		
		menuSaveImage.setOnAction(new EventHandler<ActionEvent>() {
			public void handle(ActionEvent event) {
				saveImage();
			}
		});
		
		menuExit.setOnAction(new EventHandler<ActionEvent>() {
			public void handle(ActionEvent event) {
				primaryStage.fireEvent(new WindowEvent(primaryStage, WindowEvent.WINDOW_CLOSE_REQUEST));
			}
		});
	    	
		
		return menu;
	}
	
	private void makeButtons(FlowPane pane) {
	    btnFindPath.setText("Find Path");
	    btnFindPath.setId("btnFindPath");
	    btnShowConnection.setText("Show Connection");
	    btnShowConnection.setId("btnShowConnection");
	    btnNewPlace.setText("New Place");
	    btnNewPlace.setId("btnNewPlace");
	    btnNewConnection.setText("New Connection");
	    btnNewConnection.setId("btnNewConnection");
	    btnChangeConnection.setText("Change Connection");
	    btnChangeConnection.setId("btnChangeConnection");
	    pane.getChildren().addAll(btnFindPath, btnShowConnection, btnNewPlace, btnNewConnection, btnChangeConnection);
	    
	    btnFindPath.setOnAction(new EventHandler<ActionEvent>() {
	    	public void handle(ActionEvent event) {
	    		findPath();
	    	}
	    });
	    
	    btnShowConnection.setOnAction(new EventHandler<ActionEvent>() {
	    	public void handle(ActionEvent event) {
	    		showConnection();
	    	}
	    });
	    
	    btnNewPlace.setOnAction(new EventHandler<ActionEvent>() {
	    	public void handle(ActionEvent event) {
	    		newPlace();
	    	}
	    });
	    
	    btnNewConnection.setOnAction(new EventHandler<ActionEvent>() {
	    	public void handle(ActionEvent event) {
	    		newConnection();
	    	}
	    });
	    
	    btnChangeConnection.setOnAction(new EventHandler<ActionEvent>() {
	    	public void handle(ActionEvent event) {
	    		changeConnection();
	    	}
	    });
	}
	
	private void openFileConfirmation() {
		Alert alert = new Alert(AlertType.CONFIRMATION);
		alert.setTitle("Warning!");
		alert.setHeaderText(null);
		alert.setContentText("Unsaved changes, open anyway?");
		Optional<ButtonType> result = alert.showAndWait();
		if(result.isPresent() && result.get() == ButtonType.OK) {
			clearMap();
			openFile();
		}
	}

	private void openFile() {
		try {
			FileReader reader = new FileReader("europa.graph");
			BufferedReader in = new BufferedReader(reader);
			
			String line;
			int lineNr = 0;
			
			while ((line = in.readLine()) != null) {
				lineNr++;
				if(lineNr == 1) {
					createMap(line);
				} else if (lineNr == 2) {
					if (line.isEmpty()) {
						in.close();
						reader.close();
						return;
					}
					String[] nodeInfoArr = line.split(";");
					addNodes(nodeInfoArr);
				} else {
					String[] connectionInfoArr = line.split(";");
					addConnection(connectionInfoArr);
				}
			}
			in.close();
			reader.close();
		} catch (FileNotFoundException e) {
			Alert fileNotFound = new Alert(AlertType.ERROR);
			fileNotFound.setTitle("ERROR");
			fileNotFound.setHeaderText(null);
			fileNotFound.setContentText("Could not find file!");
			fileNotFound.showAndWait();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	private void createMap(String fileName) {
			try {
				Image newMapImage = new Image(fileName);
				ImageView newMapImageView = new ImageView(newMapImage);
				newMapImageView.setFitHeight(newMapImage.getHeight());
				newMapImageView.setFitWidth(newMapImage.getWidth());
//				outputArea = new Pane();
				outputArea.setPrefSize(newMapImageView.getFitWidth(), newMapImageView.getFitHeight());
				outputArea.getChildren().add(newMapImageView);
//				flowPane.setAlignment(Pos.BASELINE_CENTER);
//				flowPane.getChildren().add(outputArea);
//				verticalBox.getChildren().add(flowPane);
				primaryStage.setWidth(newMapImage.getWidth());
				primaryStage.setHeight(newMapImage.getHeight() + pane.getHeight() + menu.getHeight() + 50);
				unsavedChanges = true;
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

	private void saveFile() {
		if (outputArea.getHeight() == 0) {
			return;
		}
		try {
			File file = new File("europa.graph");
			FileWriter fw = new FileWriter(file);
			BufferedWriter bw = new BufferedWriter(fw);
			String fileContent = generateFileContent();
			bw.write(fileContent);
			unsavedChanges = false;
			
			bw.close();
			fw.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	private void saveImage() {
		if (outputArea.getHeight() == 0) {
			return;
		}
		try {
			WritableImage image = root.snapshot(null, null);
			BufferedImage bufferedImage = SwingFXUtils.fromFXImage(image, null);
			ImageIO.write(bufferedImage, "png", new File("capture.png"));
		} catch (IOException e) {
			Alert alert = new Alert(Alert.AlertType.ERROR, "IO-error" + e.getMessage());
			alert.showAndWait();
		}
	}

	private void findPath() {
		if (outputArea.getHeight() == 0) {
			return;
		}
		if (selectedNode1 == null || selectedNode2 == null) {
			showNodeError();
			return;
		}
		if(!listGraph.pathExists(selectedNode1, selectedNode2)) {
			Alert alert = new Alert(AlertType.ERROR);
			alert.setTitle("ERROR");
			alert.setHeaderText(null);
			alert.setContentText("No path between places!");
			alert.showAndWait();
			return;
		}
		PathAlert pathAlert = new PathAlert(selectedNode1, selectedNode2, listGraph);
		pathAlert.showAndWait();
	}

	private void showConnection() {
		if(outputArea.getHeight() == 0) {
			return;
		}
		if(selectedNode1 == null || selectedNode2 == null) {
			showNodeError();
			return;
		}
		if(listGraph.getEdgeBetween(selectedNode1, selectedNode2) == null) {
			showConnectionError();
			return;
		}
		ConnectionAlert connectionAlert = new ConnectionAlert(selectedNode1.getName(), selectedNode2.getName());
		Edge<Node> connectionShown = listGraph.getEdgeBetween(selectedNode1, selectedNode2);
		connectionAlert.setNameField(connectionShown.getName());
		connectionAlert.setTimeField("" + connectionShown.getWeight());
		connectionAlert.showAndWait();
	}

	private void newPlace() {
		if(outputArea.getHeight() == 0) {
			return;
		}
		scene.setCursor(Cursor.CROSSHAIR);
		btnNewPlace.setDisable(true);
		outputArea.setOnMouseClicked(new ClickHandler());
	}
	
	private void newConnection() {
		if (outputArea.getHeight() == 0) {
			return;
		}
		if (selectedNode1 == null || selectedNode2 == null) {
			showNodeError();
			return;
		}
		if (listGraph.getEdgeBetween(selectedNode1, selectedNode2) != null) {
			Alert alert = new Alert(AlertType.ERROR);
			alert.setTitle("ERROR");
			alert.setHeaderText(null);
			alert.setContentText("Connection already made!");
			alert.showAndWait();
			return;
		}
		try {
		ConnectionAlert connectionAlert = new ConnectionAlert(selectedNode1.getName(), selectedNode2.getName());
		Optional<ButtonType> result = connectionAlert.showAndWait();
		if (result.isPresent() && result.get() == ButtonType.CANCEL) {
			return;
		}
		if(connectionAlert.getName().isBlank()) {
			showNameError();
			return;
		}
		if(result.isPresent() && result.get() == ButtonType.OK) {
			Connection connection = new Connection(selectedNode1.getCenterX(), selectedNode1.getCenterY(), selectedNode2.getCenterX(), selectedNode2.getCenterY(), selectedNode1.getName(), selectedNode2.getName(),connectionAlert.getName(), connectionAlert.getTime());
			outputArea.getChildren().add(connection);
			listGraph.connect(selectedNode1, selectedNode2, connectionAlert.getName(), connectionAlert.getTime());
			connections.add(connection);
			unsavedChanges = true;
		}
		} catch (NumberFormatException e) {
			showTimeError();
			return;
		}
	}
	
	private void changeConnection() {
		if(outputArea.getHeight() == 0) {
			return;
		}
		if (selectedNode1 == null || selectedNode2 == null) {
			showNodeError();
			return;
		}
		if (listGraph.getEdgeBetween(selectedNode1, selectedNode2) == null) {
			showConnectionError();
			return;
		}
		try {
			ConnectionAlert connectionAlert = new ConnectionAlert(selectedNode1.getName(), selectedNode2.getName());
			connectionAlert.setNameField(listGraph.getEdgeBetween(selectedNode1, selectedNode2).getName());
			Optional<ButtonType> result = connectionAlert.showAndWait();
			if(result.isPresent() && result.get() == ButtonType.OK) {
				listGraph.getEdgeBetween(selectedNode1, selectedNode2).setWeight(connectionAlert.getTime());
				listGraph.getEdgeBetween(selectedNode2, selectedNode1).setWeight(connectionAlert.getTime());
				unsavedChanges = true;
			}
		} catch(NumberFormatException e) {
			showTimeError();
			return;
		}
	}
	
	private String generateFileContent() {
		ArrayList<Node> nodes = getNodes();
		StringBuilder sb = new StringBuilder();
		sb.append("europa.gif").append("\n");
		
		for (int i = 0; i < nodes.size(); i++) {
			Node n = nodes.get(i);
			if (i == nodes.size() - 1) {
				sb.append(n.getName() + ";").append(n.getCenterX() + ";").append(n.getCenterY());
			} else {
				sb.append(n.getName() + ";").append(n.getCenterX() + ";").append(n.getCenterY() + ";");
			}
		}
		sb.append("\n");
		
		for (int i = 0; i < connections.size(); i++) {
			Connection c = connections.get(i);
			if (i == connections.size() - 1) {
				sb.append(c.getFrom() + ";").append(c.getTo() + ";").append(c.getName() + ";").append(c.getTime()).append("\n");
				sb.append(c.getTo() + ";").append(c.getFrom() + ";").append(c.getName() + ";").append(c.getTime());
			} else {
				sb.append(c.getFrom() + ";").append(c.getTo() + ";").append(c.getName() + ";").append(c.getTime()).append("\n");
				sb.append(c.getTo() + ";").append(c.getFrom() + ";").append(c.getName() + ";").append(c.getTime()).append("\n");
			}
		}
		return sb.toString();
	}
	
	private void clearMap() {
		if (outputArea.getHeight() != 0) {
			listGraph = new ListGraph<Node>();
//			nodes.clear();
			connections.clear();
			selectedNode1 = null;
			selectedNode2 = null;
			outputArea.getChildren().clear();
//			outputArea.getChildren().add(newMapImageView);
//			flowPane.getChildren().clear();
//			verticalBox.getChildren().remove(flowPane);
			unsavedChanges = false;	
		}
	}

	private Node getNode(String nodeName) {
		ArrayList<Node> nodes = getNodes();
		for(Node n: nodes) {
			if(n.getName().equals(nodeName)) {
				return n;
			}
		}
		return null;
	}
	
	private void addConnection(String[] arr) {
		LinkedList<String> connectionInfoList = new LinkedList<>();
		connectionInfoList.addAll(Arrays.asList(arr));
		
		String from = connectionInfoList.pollFirst();
		String to = connectionInfoList.pollFirst();
		String name = connectionInfoList.pollFirst();
		int time = Integer.parseInt(connectionInfoList.pollFirst());
		
		try {
			listGraph.connect(getNode(from), getNode(to), name, time);
		} catch(IllegalStateException e) {
			return;
		}
		Connection c = new Connection(getNode(from).getCenterX(), getNode(from).getCenterY(), getNode(to).getCenterX(), getNode(to).getCenterY(),from, to, name, time);
//		listGraph.connect(getNode(from), getNode(to), name, time);
		connections.add(c);
		outputArea.getChildren().add(c);
	}
	
	private void addNodes(String[] arr) {
		LinkedList<String> nodeInfoList= new LinkedList<>();
		nodeInfoList.addAll(Arrays.asList(arr));
		
		while(!nodeInfoList.isEmpty()) {
			String name = nodeInfoList.pollFirst();
			double x = Double.parseDouble(nodeInfoList.pollFirst());
			double y = Double.parseDouble(nodeInfoList.pollFirst());
			Node n = new Node(x, y, name);
			n.setId(name);
			listGraph.add(n);
//			nodes.add(n);
			outputArea.getChildren().add(n);
			n.setOnMouseClicked(new NodeHandler());
		}
	}
	
	private void showNodeError() {
		Alert nodeAlert = new Alert(AlertType.ERROR);
		nodeAlert.setTitle("ERROR");
		nodeAlert.setHeaderText(null);
		nodeAlert.setContentText("Two places must be selected!");
		nodeAlert.showAndWait();
	}

	private void showNameError() {
		Alert alert = new Alert(AlertType.ERROR);
		alert.setTitle("ERROR");
		alert.setHeaderText(null);
		alert.setContentText("Name can't be blank!");
		alert.showAndWait();
	}

	private void showTimeError() {
		Alert alert = new Alert(AlertType.ERROR);
		alert.setTitle("ERROR");
		alert.setHeaderText(null);
		alert.setContentText("Time has to be an integer!");
		alert.showAndWait();
	}

	private void showConnectionError() {
		Alert alert = new Alert(AlertType.ERROR);
		alert.setTitle("ERROR");
		alert.setHeaderText(null);
		alert.setContentText("No connection between places!");
		alert.showAndWait();
	}
	
	private ArrayList<Node> getNodes(){
		ArrayList<Node> nodes = new ArrayList<>();
		for (Node n: listGraph.getNodes()) {
			nodes.add(n);
		}
		return nodes;
	}

	class ClickHandler implements EventHandler<MouseEvent>{
		@Override public void handle(MouseEvent event) {
			ArrayList<Node> nodes = getNodes();
			NameAlert nameAlert = new NameAlert();
			Optional<ButtonType> result = nameAlert.showAndWait();
			if(result.isPresent() && result.get() == ButtonType.OK) {
				if(nameAlert.getName().isBlank()) {
					showNameError();
					return;
				}
				for (Node n: nodes) {
					if (n.getName().equals(nameAlert.getName())) {
						Alert alert = new Alert(AlertType.ERROR);
						alert.setTitle("ERROR");
						alert.setHeaderText(null);
						alert.setContentText("Place with the same name already exists!");
						alert.showAndWait();
						return;
					}
				}
				double x = event.getX();
				double y = event.getY();
				Node node = new Node(x, y, nameAlert.getName());
				node.setId(node.getName());
				nodes.add(node);
				node.setOnMouseClicked(new NodeHandler());
				outputArea.getChildren().add(node);
				listGraph.add(node);
				unsavedChanges = true;
				btnNewPlace.setDisable(false);
				scene.setCursor(Cursor.DEFAULT);
				outputArea.setOnMouseClicked(null);
			} else {
				btnNewPlace.setDisable(false);
				scene.setCursor(Cursor.DEFAULT);
				outputArea.setOnMouseClicked(null);
			}
			
		}
	}

	class NodeHandler implements EventHandler<MouseEvent>{
		@Override public void handle(MouseEvent event) {
			Node n = (Node)event.getSource();
			if (!n.isSelected()) {
				if (selectedNode1 == null) {
					n.select(true);
					selectedNode1 = n;
					n.setFill(Color.RED);
				} else if (selectedNode2 == null) {
					n.select(true);
					selectedNode2 = n;
					n.setFill(Color.RED);
				} else {
					return;
				}
			} else {
				n.select(false);
				n.setFill(Color.BLUE);
				if (selectedNode1.equals(n)) {
					selectedNode1 = selectedNode2;
					selectedNode2 = null;
				} else {
					selectedNode2 = null;
				}
			}
		}
	}
	class ExitHandler implements EventHandler<WindowEvent>{
		@Override public void handle(WindowEvent event) {
			if (unsavedChanges) {
				Alert alert = new Alert(AlertType.CONFIRMATION);
				alert.setTitle("Warning!");
				alert.setHeaderText(null);
				alert.setContentText("Unsaved changes, exit anyway?");
				Optional<ButtonType> result = alert.showAndWait();
				if(result.isPresent() && result.get() == ButtonType.CANCEL) {
					event.consume();
				}
			}
		}
	}
}