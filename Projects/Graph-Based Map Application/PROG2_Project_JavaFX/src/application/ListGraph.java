package application;

import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.NoSuchElementException;
import java.util.Set;

public class ListGraph<T> implements Graph<T>{
	
	private Map<T, Set<Edge<T>>> graph = new HashMap<T, Set<Edge<T>>>();
	
	public void add(T node) {
		if (graph.get(node) != null) {
			return;
		} else {
			graph.put(node, new HashSet<Edge<T>>());
		}
	}

	public void remove(T node) {
		getEdgesFrom(node);
		graph.remove(node);
		for (Set<Edge<T>> edgeSet: graph.values()) {
			edgeSet.removeIf(e -> e.getDestination() == node);
			
		}
	}
	
	public void connect(T from, T to, String name, int weight) {
		getEdgesFrom(from);
		getEdgesFrom(to);
		Edge<T> fromEdge = new Edge<T>(to, name, weight);
		Edge<T> toEdge = new Edge<T>(from, name, weight);
		if (getEdgeBetween(from, to) != null) {
			throw new IllegalStateException("Error: Connection between nodes already made!");
		}
		Set<Edge<T>> fromEdges = graph.get(from);
		Set<Edge<T>> toEdges = graph.get(to);
		fromEdges.add(fromEdge);
		toEdges.add(toEdge);
	}
	
	public void disconnect(T from, T to) {
		getEdgesFrom(from);
		getEdgesFrom(to);
		if (getEdgeBetween(from, to) == null) {
			throw new IllegalStateException("Error: No connection between nodes!");
		}
		Set<Edge<T>> fromEdges = graph.get(from);
		Set<Edge<T>> toEdges = graph.get(to);
		fromEdges.removeIf(e -> e.getDestination() == to);
		toEdges.removeIf(e -> e.getDestination() == from);
	}
	
	public void setConnectionWeight(T from, T to, int weight) {
		getEdgesFrom(from);
		getEdgesFrom(to);
		if (getEdgeBetween(from, to) == null) {
			throw new IllegalStateException("Error: No connection between nodes!");
		}
		getEdgeBetween(from, to).setWeight(weight);
		getEdgeBetween(to, from).setWeight(weight);
	} 
	
	public Set<T> getNodes(){
		Set<T> nodeSet = new HashSet<T>();
		nodeSet = graph.keySet();
		return nodeSet;
	}
	
	public Set<Edge<T>> getEdgesFrom(T node){
		Set<Edge<T>> nodesEdges = new HashSet<>();
		if (!getNodes().contains(node)) {
			throw new NoSuchElementException("Error: Node not in graph!");
		}
		nodesEdges = graph.get(node);
		return nodesEdges;
	}
	
	public Edge<T> getEdgeBetween(T from, T to){
		Set<Edge<T>> fromEdges = getEdgesFrom(from);
		getEdgesFrom(to);
		for (Edge<T> edge: fromEdges) {
			if (edge.getDestination().equals(to)) {
				return edge;
			}
		}
		return null;
	}
	
	public String toString() {
		StringBuilder sb = new StringBuilder();
		for (T node: graph.keySet()) {
			sb.append(node).append(":").append(graph.get(node)).append("\n");
		}
		return sb.toString();
	}
	
	public boolean pathExists(T from, T to) {
		if (!getNodes().contains(from) || !getNodes().contains(to)) {
			return false;
		}
		Set<T> visited = new HashSet<>();
		dfs(from, to, visited);
		return visited.contains(to);
	}
	
	private void dfs(T current, T target, Set<T> visited) {
		visited.add(current);
		if (current.equals(target)) {
//			System.out.print("path found!");
			return;
		}
		for (Edge<T> edge: getEdgesFrom(current)) {
			if (!visited.contains(edge.getDestination())) {
				dfs(edge.getDestination(), target, visited);
			}
		}
	}
	
	public List<Edge<T>> getPath(T from, T to) {
		Map<T, T> connections = new HashMap<>();
		LinkedList<T> queue = new LinkedList<>();
		connections.put(from, null);
		queue.add(from);
		while(!queue.isEmpty()) {
			T next = queue.pollFirst();
			for (Edge<T> edge: getEdgesFrom(next)) {
				T destination = edge.getDestination();
				if (!connections.containsKey(destination)) {
					connections.put(destination, next);
					queue.add(destination);
				}
			}
		}
		if (!connections.containsKey(to)) {
			return null;
		}
		return collectPath(from, to, connections);
	}
	
	private List<Edge<T>> collectPath(T from, T to, Map<T, T> connections){
		LinkedList<Edge<T>> path = new LinkedList<>();
		T current = to;
		while (!current.equals(from)) {
			T next = connections.get(current);
			Edge<T> edge = getEdgeBetween(next, current);
			path.addFirst(edge);
			current = next;
		}
		return path;
	}

}
