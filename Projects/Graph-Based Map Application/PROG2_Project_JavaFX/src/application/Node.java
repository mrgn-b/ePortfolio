package application;

import javafx.scene.shape.Circle;
import javafx.scene.paint.Color;


public class Node extends Circle{
	private String name;
	private boolean selected = false;
	
	public Node (double x, double y, String name) {
		super(x, y, 10, Color.BLUE);
		this.name = name;
	}
	
	public String getName() {
		return name;
	}
	
	public void select(boolean selected) {
		this.selected = selected;
	}
	
	public boolean isSelected() {
		return selected;
	}

}
