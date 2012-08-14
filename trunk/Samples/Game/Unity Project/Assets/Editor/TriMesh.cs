using UnityEngine;
using System.Collections.Generic;

/**
 * Ported and adapted from "TriMesh_algo.h" and "TriMesh.h".
 * 
 * Description: Various mesh-munging algorithms using triangle meshes.
 * 
 * By Szymon Rusinkiewicz (Princeton University)
 * 
 */
public class TriMesh
{
	private class CompareArrayElements : IComparer<int>
	{
		private List<int> array;
		
		public CompareArrayElements (List<int> array)
		{
			this.array = array;
		}
		
		public int Compare (int x, int y)
		{
			return array [x].CompareTo (array [y]);
		}
	}
	
	public struct Face
	{
		public static Face EMPTY = new Face (-1, -1, -1);
		public int[] vertex;
		
		public Face (int v0, int v1, int v2)
		{ 
			vertex = new int[3];
			
			vertex [0] = v0;
			vertex [1] = v1;
			vertex [2] = v2; 
		}
		
		public Face (int[] vertices)
		{
			vertex = new int[3];
			
			vertex [0] = vertices [0];
			vertex [1] = vertices [1];
			vertex [2] = vertices [2]; 
		}
	};
	
	public List<Vector3> vertices;
	public List<Face> faces = new List<Face> ();
	public Dictionary<int, List<int> > adjacencyMap = new Dictionary<int, List<int> > ();
	public List<int> indexes;
	
	public TriMesh (Mesh mesh)
	{
		vertices = new List<Vector3> (mesh.vertices);
		indexes = new List<int> (mesh.triangles);
	}
	
	private void BuildFromIndexes ()
	{
		if (indexes.Count == 0 || faces.Count != 0) {
			return;
		}
	
		Debug.Log ("Building from indexes... ");
	
		faces = new List<Face> (indexes.Count / 3);
	
		for (int i = 0; i < indexes.Count; i += 3) {
			faces.Add (new Face (indexes [i],
                                 indexes [i + 1],
                                 indexes [i + 2]));
		}
		
		Debug.Log ("Done.\n  " + faces.Count + " triangles.\n");
	}
	
	public void BuildFaces ()
	{
		if (faces.Count != 0) {
			return;
		}
		
		if (indexes.Count != 0) {
			BuildFromIndexes ();
		}
	}
	
	public void BuildAdjacencyMap ()
	{
		if (adjacencyMap.Count != 0) {
			return;
		}

		BuildFaces ();
		
		if (faces.Count == 0) {
			return;
		}

		Debug.Log ("Finding vertex to triangle maps... ");
		
		for (int faceIndex = 0; faceIndex < faces.Count; faceIndex++) {
			for (int j = 0; j < 3; j++) {
				int vertexIndex = faces [faceIndex].vertex [j];
				
				if (!adjacencyMap .ContainsKey (vertexIndex)) {
					adjacencyMap [vertexIndex] = new List<int> ();
				}
				
				adjacencyMap [vertexIndex].Add (faceIndex);
			}
		}

		Debug.Log ("Done.\n");
	}
	
	private static void FindConnected (TriMesh triMesh, Dictionary<int, int> componentByFace, Dictionary<int, int> componentSizeByIndex, int faceIndex, int componentIndex, bool connectedVertices)
	{
		Stack<int> stack = new Stack<int> ();
		stack.Push (faceIndex);
		while (stack.Count > 0) {
			int currentFace = stack.Pop ();
			for (int i = 0; i < 3; i++) {
				int vertexIndex = triMesh.faces [currentFace].vertex [i];
				for (int adjacencyIndex = 0, adjacentFace = triMesh.adjacencyMap[vertexIndex][0]; 
	     			adjacencyIndex < triMesh.adjacencyMap[vertexIndex].Count; 
	     			adjacencyIndex++) {
					adjacentFace = triMesh.adjacencyMap [vertexIndex] [adjacencyIndex];

					if (componentByFace.ContainsKey (adjacentFace) || 
						!IsConnected (triMesh, adjacentFace, currentFace, connectedVertices)) {
						continue;
					}
					
					componentByFace [adjacentFace] = componentIndex;
					componentSizeByIndex [componentIndex]++;
					
					stack.Push (adjacentFace);
				}
			}
		}
	}
	
	// Are two faces connected along an edge (or vertex)?
	private static bool IsConnected (TriMesh mesh, int face1, int face2, bool connectedVertices)
	{
		int f10 = mesh.faces [face1].vertex [0], f11 = mesh.faces [face1].vertex [1], f12 = mesh.faces [face1].vertex [2];
		int f20 = mesh.faces [face2].vertex [0], f21 = mesh.faces [face2].vertex [1], f22 = mesh.faces [face2].vertex [2];
	
		if (connectedVertices) {
			return f10 == f20 || f10 == f21 || f10 == f22 ||
			       f11 == f20 || f11 == f21 || f11 == f22 ||
			       f12 == f20 || f12 == f21 || f12 == f22;
		} else {
			return (f10 == f20 && (f11 == f22 || f12 == f21)) ||
			       (f10 == f21 && (f11 == f20 || f12 == f22)) ||
			       (f10 == f22 && (f11 == f21 || f12 == f20)) ||
			       (f11 == f20 && f12 == f22) ||
			       (f11 == f21 && f12 == f20) ||
			       (f11 == f22 && f12 == f21);
		}
	}
	
	private static List<Face> GetComponentFaces (TriMesh triMesh, Dictionary<int, int> componentByFace, int componentIndex)
	{
		List<Face> faces = new List<Face> ();
		Dictionary<int, int>.Enumerator e = componentByFace.GetEnumerator ();
		while (e.MoveNext()) {
			if (e.Current.Value == componentIndex) {
				faces.Add (triMesh.faces [e.Current.Key]);
			}
		}
		return faces;
	}
	
	/**
	 * Find the connected components of TriMesh. 
	 * Considers components to be connected if they touch at a vertex (if "connectedVertices" == true), else they need to touch at an edge.
	 */
	public static List<Vector3[]> FindComponents (Mesh mesh, bool connectedVertices = false)
	{
		List<Vector3[] > connectedComponents = new List<Vector3[] > ();
		TriMesh triMesh = new TriMesh (mesh);
		
		if (triMesh.vertices.Count == 0) {
			return connectedComponents;
		}
		
		triMesh.BuildFaces ();
		
		if (triMesh.faces.Count == 0) {
			return connectedComponents;
		}
		
		triMesh.BuildAdjacencyMap ();

		int numberOfFaces = triMesh.faces.Count;
		
		Dictionary<int, int> componentByFace = new Dictionary<int, int> ();
		Dictionary<int, int> componentFacesCountByIndex = new Dictionary<int, int> ();
		int componentsCount = 0;

		int faceIndex;
		for (faceIndex = 0; faceIndex < numberOfFaces; faceIndex++) {
			if (componentByFace.ContainsKey (faceIndex)) {
				continue;
			}
			
			int componentIndex = componentsCount++;
			componentByFace [faceIndex] = componentIndex;
			componentFacesCountByIndex [componentIndex] = 1;
			FindConnected (triMesh, componentByFace, componentFacesCountByIndex, faceIndex, componentIndex, connectedVertices);
		}
		
		for (int componentIndex = 0; componentIndex < componentsCount; componentIndex++) {
			Vector3[] connectedComponent = new Vector3[componentFacesCountByIndex [componentIndex] * 3];
			List<Face> faces = GetComponentFaces (triMesh, componentByFace, componentIndex);
			int i = 0;
			foreach (Face face in faces) {
				connectedComponent [i++] = mesh.vertices [mesh.triangles [face.vertex [0]]];
				connectedComponent [i++] = mesh.vertices [mesh.triangles [face.vertex [1]]];
				connectedComponent [i++] = mesh.vertices [mesh.triangles [face.vertex [2]]];
			}
			connectedComponents.Add (connectedComponent);
		}
		
		return connectedComponents;
	}
	
}
