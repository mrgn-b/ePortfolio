package application;


public class Edge<T> {
	
	private T destination;
	private int weight;
	private String name;
	
	public Edge(T destination, String name, int weight) {
		this.destination = destination;
		this.name = name;
		setWeight(weight);
	}
	
	public T getDestination() {
		return destination;
	}
	
	public int getWeight() {
		return weight;
	}
	
	public void setWeight(int weight) {
		if (weight < 0 ) {
			throw new IllegalArgumentException();
		} else {
			this.weight = weight;
		}
	}
	
	public String getName() {
		return name;
	}
	
	@Override
	public String toString() {
//		return "Destination: " + destination + ", Weight: " + weight + ", Name: " + name;
		return "till " + destination + " med " + name + " tar " + weight;
	}

}
