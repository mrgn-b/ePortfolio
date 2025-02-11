package application;

import javafx.scene.control.Alert;
import javafx.scene.control.TextArea;

import java.util.*;

public class PathAlert extends Alert{

	public PathAlert(Node fromNode, Node toNode, ListGraph<Node> listGraph) {
		super(AlertType.INFORMATION);
		TextArea textArea = new TextArea();
		
		List<Edge<Node>> edgeList = listGraph.getPath(fromNode, toNode);
		StringBuilder result = new StringBuilder();
		int totalTime = 0;
		for(Edge<Node> edge: edgeList) {
			totalTime += edge.getWeight();
			result.append("to ").append(edge.getDestination().getName()).append(" by ").append(edge.getName()).append(" takes ").append(edge.getWeight()).append("\n");
		}
		result.append("Total ").append(totalTime);
		
		textArea.setText(result.toString());
		textArea.setEditable(false);
		
		setTitle("Message");
		setHeaderText("The Path from " + fromNode.getName() + " to " + toNode.getName() + ":");
		getDialogPane().setContent(textArea);
	}

}
