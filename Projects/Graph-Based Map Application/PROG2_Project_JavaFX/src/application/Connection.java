package application;

import javafx.scene.shape.Line;

public class Connection extends Line{
	private String name;
	private String from;
	private String to;
	private int time;

	public Connection(double fromX, double fromY, double toX, double toY, String from, String to, String name, int time) {
		super(fromX, fromY, toX, toY);
		this.from = from;
		this.to = to;
		this.name = name;
		this.time = time;
		
		setStrokeWidth(2);
		setDisable(true);
	}
	
	public String getFrom() {
		return from;
	}
	
	public String getTo() {
		return to;
	}
	
	public int getTime() {
		return time;
	}
	
	public String getName() {
		return name;
	}
}
