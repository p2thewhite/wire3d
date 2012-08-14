using UnityEditor;
using UnityEngine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;

public class Unity3DExporter : EditorWindow
{
	private static bool mIsWindowOpen;
	private bool mExportStateMaterial;
	private bool mIgnoreUnderscore;
	private bool mExportXmlOnly;
	private bool mPreventRepeatedNodeNames;
	private string m2ndTextureName;
	private string mPath;
	private List<string> mMeshAssetsProcessed;
	private List<string> mTextureAssetsProcessed;
	private Dictionary<string, int> mNodeNameCounter;

	[MenuItem("Wire3D/Exporter")]
	protected static void CreateWizard ()
	{
		if (!mIsWindowOpen) {
			mIsWindowOpen = true;
			Unity3DExporter editor = GetWindow ((typeof(Unity3DExporter))) as Unity3DExporter;
			if (editor) {
				editor.autoRepaintOnSceneChange = true;
			}
		}
	}

	private static List<Transform> GetRootTransforms ()
	{
		List<Transform> allTransforms = new List<Transform> (FindObjectsOfType (typeof(Transform)) as Transform[]);
		List<Transform> rootTransforms = allTransforms.Where (t => t.parent == null).ToList ();
		return (from s in rootTransforms orderby s.name select s).ToList ();
	}
	
	private static void GenerateAxisAlignedBoxFromVertices (Vector3[] vertices, out Vector3 aabbMin, out Vector3 aabbMax)
	{
		aabbMin = new Vector3 (vertices [0].x, vertices [0].y, vertices [0].z);
		aabbMax = new Vector3 (aabbMin.x, aabbMin.y, aabbMin.z);
		
		foreach (Vector3 vertex in vertices) {
			if (vertex.x < aabbMin.x) {
				aabbMin.x = vertex.x;
			}
			if (vertex.x > aabbMax.x) {
				aabbMax.x = vertex.x;
			} 
			if (vertex.y < aabbMin.y) {
				aabbMin.y = vertex.y;
			}
			if (vertex.y > aabbMax.y) {
				aabbMax.y = vertex.y;
			}
			if (vertex.z < aabbMin.z) {
				aabbMin.z = vertex.z;
			}
			if (vertex.z > aabbMax.z) {
				aabbMax.z = vertex.z;
			}
		}
	}
	
	private static void GenerateBoxColliderTraverse (Transform transform)
	{
		if (IsLeaf (transform)) {
			GenerateBoxCollider (transform);
		} else {
			if (transform.GetChildCount () > 0) {
				for (int i = 0; i < transform.GetChildCount(); i++) {
					GenerateBoxColliderTraverse (transform.GetChild (i));
				}
			}
		}
	}

	private static void GenerateBoxCollider (Transform transform)
	{
		MeshFilter meshFilter = transform.gameObject.GetComponent<MeshFilter> ();
		Mesh mesh = meshFilter.sharedMesh;
		List<Vector3[] > meshComponents = TriMesh.FindComponents (mesh, true);
		
		Debug.Log ("meshComponents: " + meshComponents.Count);
		
		/*foreach (Vector3[] meshComponent in meshComponents) {
			Vector3 aabbMin;
			Vector3 aabbMax;
			GenerateAxisAlignedBoxFromVertices (meshComponent, out aabbMin, out aabbMax);
			
			GameObject dummyGameObject = new GameObject ();
			dummyGameObject.transform.position = gameObject.transform.position;
			BoxCollider boxCollider = dummyGameObject.AddComponent<BoxCollider> ();
			boxCollider.center = Vector3.zero;
			boxCollider.size = new Vector3 (
				(aabbMax.x - aabbMin.x), 
				(aabbMax.y - aabbMin.y), 
				(aabbMax.z - aabbMin.z)
				);
		}*/
	}
	
	[MenuItem("Wire3D/Generate Box Colliders")]
	protected static void GenerateBoxColliders ()
	{
		foreach (Transform transform in GetRootTransforms ()) {
			GenerateBoxColliderTraverse (transform);
		}
	}

	protected void OnDestroy ()
	{
		mIsWindowOpen = false;
	}

	protected virtual void OnGUI ()
	{
		EditorGUILayout.BeginHorizontal ();
		EditorGUIUtility.LookLikeControls ();
		mPath = EditorGUILayout.TextField ("Save to directory:", mPath ?? string.Empty);
		EditorGUILayout.EndHorizontal ();

		bool dirExists = Directory.Exists (mPath);

		if (!dirExists && !string.IsNullOrEmpty (mPath)) {
			GUILayout.Label ("Specified path '" + mPath + "' does not exist.");
		}
		
		// ---
		GUILayout.Space (10);

		mExportStateMaterial = GUILayout.Toggle (mExportStateMaterial, "Try to export Material State (i.e. Main Color)");
		mIgnoreUnderscore = GUILayout.Toggle (mIgnoreUnderscore, "Ignore GameObjects starting with '_'");
		mExportXmlOnly = GUILayout.Toggle (mExportXmlOnly, "Export scene XML only");
		mPreventRepeatedNodeNames = GUILayout.Toggle (mPreventRepeatedNodeNames, "Prevent repeated node names");
		
		// ---
		GUILayout.Space (10);
		
		GUILayout.Label ("Use property names:");
		m2ndTextureName = EditorGUILayout.TextField ("- 2nd Texture ", m2ndTextureName ?? string.Empty);
		
		// ---
		GUILayout.Space (20);
		
		if (dirExists && !string.IsNullOrEmpty (mPath)) {
			if (GUILayout.Button ("Export")) {
				if (mPath [mPath.Length - 1] != '/' && mPath [mPath.Length - 1] != '\\') {
					mPath = mPath + '/';
				}

				mMeshAssetsProcessed = new List<string> ();
				mTextureAssetsProcessed = new List<string> ();
				Export ();
			}
		}
		
		if (mPreventRepeatedNodeNames) {
			mNodeNameCounter = new Dictionary<string, int> ();
		}
	}
	
	private string GetNodeName (GameObject gameObject)
	{
		string nodeName = gameObject.name;
		
		if (!mPreventRepeatedNodeNames)
			return nodeName;
				
		int nameUsageCount;
		if (mNodeNameCounter.ContainsKey (nodeName)) {
			nameUsageCount = mNodeNameCounter [nodeName];
		} else {
			nameUsageCount = 0;
		}
		
		mNodeNameCounter [nodeName] = ++nameUsageCount;
		
		if (nameUsageCount > 1) {
			return nodeName + nameUsageCount;
		} else {
			return nodeName;
		}
	}

	private void Export ()
	{
		string[] unityScenePath = EditorApplication.currentScene.Split (char.Parse ("/"));
		string unitySceneName = unityScenePath [unityScenePath.Length - 1];
		if (string.IsNullOrEmpty (unitySceneName)) {
			Debug.Log ("Scene is empty, nothing to do.");
		} else {
			if (unitySceneName.Substring (unitySceneName.Length - 6, 6).Equals (".unity")) {
				unitySceneName = unitySceneName.Remove (unitySceneName.Length - 6);
			}
		}
        
		StreamWriter outFile = new StreamWriter (mPath + unitySceneName + ".xml");
		try {
			GameObject root = new GameObject ("Root");
			outFile.WriteLine ("<Node Name=\"" + GetNodeName (root) + "\" " + GetTransformAsString (root.transform) + ">");
			string indent = "  ";
	
			foreach (Transform transform in GetRootTransforms ()) {
				WriteTraverse (transform, outFile, indent);
			}
	
			DestroyImmediate (root);
			
			outFile.WriteLine ("</Node>");
		} finally {
			outFile.Close ();
		}
	}

	private static bool IsLeaf (Transform transform)
	{
		GameObject gameObject = transform.gameObject;
		MeshFilter meshFilter = gameObject.GetComponent<MeshFilter> ();
		MeshRenderer meshRenderer = gameObject.GetComponent<MeshRenderer> ();
		
		return (meshFilter != null) && (meshRenderer != null && meshRenderer.enabled) && transform.GetChildCount () == 0;
	}

	private void WriteTraverse (Transform transform, StreamWriter outFile, string indent)
	{
		if (mIgnoreUnderscore && transform.gameObject.name.StartsWith ("_")) {
			return;
		}
      	
		if (IsLeaf (transform)) {
			WriteLeaf (transform, outFile, indent);
		} else {
			WriteNonVisibleNode (transform, outFile, indent);
		}
	}

	private string GetTransformAsString (Transform transform)
	{
		Vector3 position = transform.localPosition;
		Quaternion rotation = transform.localRotation;
		Vector3 scale = transform.localScale;
		
		return "Pos=\"" + (-position.x) + ", " + position.y + ", " + position.z + "\" " +
               "Rot=\"" + rotation.w + ", " + rotation.x + ", " + (-rotation.y) + ", " + (-rotation.z) + "\" " +
               "Scale=\"" + scale.x + ", " + scale.y + ", " + scale.z + "\"";
	}

	private void WriteLight (GameObject gameObject, StreamWriter outFile, string indent)
	{
		Light light = gameObject.GetComponent<Light> ();
		
		if (light == null) {
			return;
		}

		Color ambient = RenderSettings.ambientLight;
		Color color = light.color * light.intensity; 
		
		outFile.WriteLine (indent + "  " + "<Light Type=\"" + light.type +
			"\" Ambient=\"" + ambient.r + ", " + ambient.g + ", " + ambient.b +
			"\" Color=\"" + color.r + ", " + color.g + ", " + color.b + "\" />");
	}
	
	private void WriteCollider (GameObject gameObject, StreamWriter outFile, string indent)
	{
		Collider collider = gameObject.GetComponent<Collider> ();
		
		if (collider == null) {
			return;
		}
		
		string shape = GetColliderShapeName (collider);
		
		// Collider element start.
		outFile.Write (indent + "  " + "<Collider Shape=\"" + shape + "\" ");
		
		if (collider is BoxCollider) {
			WriteBoxColliderAttributes (collider as BoxCollider, outFile, indent);
		} else {
			Debug.Log ("Collider shape not supported yet: '" + shape + "'.");
		}
		
		// Collider element end.
		outFile.Write (" />\n");
	}
	
	private string GetColliderShapeName (Collider collider)
	{
		string typeName = collider.GetType ().Name;
		return typeName.Replace ("Collider", "");
	}
	
	private void WriteBoxColliderAttributes (BoxCollider boxCollider, StreamWriter outFile, string indent)
	{
		bool isTrigger = boxCollider.isTrigger;
		Vector3 center = boxCollider.center;
		Vector3 size = boxCollider.size;
		
		outFile.Write ("IsTrigger=\"" + isTrigger + "\" Center=\"" + center.x + ", " + center.y + ", " + center.z +
			"\" Size=\"" + size.x + ", " + size.y + ", " + size.z + "\"");
	}
    
	private void WriteCamera (GameObject gameObject, StreamWriter outFile, string indent)
	{
		Camera camera = gameObject.GetComponent<Camera> ();
		
		if (camera == null) {
			return;
		}

		float fieldOfView = camera.fieldOfView;
		if (camera.orthographic) {
			fieldOfView = 0;
		}

		outFile.WriteLine (indent + "  " + "<Camera Fov=\"" + fieldOfView + "\" Near=\"" +
            camera.nearClipPlane + "\" Far=\"" + camera.farClipPlane + "\" />");
	}

	private void WriteLeaf (Transform transform, StreamWriter outFile, string indent, bool writeComponents = true)
	{
		MeshFilter meshFilter = transform.gameObject.GetComponent<MeshFilter> ();
		MeshRenderer meshRenderer = transform.gameObject.GetComponent<MeshRenderer> ();
		
		if ((meshRenderer == null) || (meshFilter == null)) {
			return;
		}

		string isStatic = " Static=\"" + (transform.gameObject.isStatic ? "1" : "0") + "\"";
		outFile.WriteLine (indent + "<Leaf Name=\"" + GetNodeName (transform.gameObject) + "\" " + GetTransformAsString (transform) + isStatic + ">");

		if (writeComponents) {
			WriteCamera (transform.gameObject, outFile, indent);
			WriteLight (transform.gameObject, outFile, indent);
			WriteCollider (transform.gameObject, outFile, indent);
		}
		
		WriteMesh (meshFilter.sharedMesh, outFile, indent + "  ", meshRenderer.lightmapTilingOffset);
		WriteMaterial (meshRenderer, outFile, indent + "  ");
		
		outFile.WriteLine (indent + "</Leaf>");       
	}
	
	private void WriteNonVisibleNode (Transform transform, StreamWriter outFile, string indent)
	{
		GameObject gameObject = transform.gameObject;
		MeshFilter meshFilter = gameObject.GetComponent<MeshFilter> ();
		MeshRenderer meshRenderer = gameObject.GetComponent<MeshRenderer> ();
		
		outFile.WriteLine (indent + "<Node Name=\"" + GetNodeName (transform.gameObject) + "\" " + GetTransformAsString (transform) + ">");
    	
		WriteCamera (transform.gameObject, outFile, indent);
		WriteLight (transform.gameObject, outFile, indent);
		WriteCollider (transform.gameObject, outFile, indent);
    	
		if (transform.GetChildCount () > 0) {
			if ((meshFilter != null) && (meshRenderer != null)) {
				WriteLeaf (transform, outFile, indent + "  ", false);
			}
    	
			for (int i = 0; i < transform.GetChildCount(); i++) {
				WriteTraverse (transform.GetChild (i), outFile, indent + "  ");
			}
		}
    	
		outFile.WriteLine (indent + "</Node>");
	}

	private void WriteMaterial (MeshRenderer meshRenderer, StreamWriter outFile, string indent)
	{
		Material material = meshRenderer.sharedMaterial;
		if (material == null) {
			return;
		}

		string lightMapIndex = string.Empty;
		if (meshRenderer.lightmapIndex != -1) {
			lightMapIndex = "_" + meshRenderer.lightmapIndex;
		}

		outFile.WriteLine (indent + "<Material Name=\"" + material.name + lightMapIndex + "\">");      

		Texture2D texture = material.mainTexture as Texture2D;
		WriteTexture (texture, outFile, indent);

		if (!string.IsNullOrEmpty (m2ndTextureName) && material.HasProperty (m2ndTextureName)) {
			Texture2D _2ndTexture = material.GetTexture (m2ndTextureName) as Texture2D;
			WriteTexture (_2ndTexture, outFile, indent);
		}

		if (meshRenderer.lightmapIndex != -1 && meshRenderer.lightmapIndex != 254) {
			Texture2D lightmap = LightmapSettings.lightmaps [meshRenderer.lightmapIndex].lightmapFar;
			WriteTexture (lightmap, outFile, indent, true);               
		}

		WriteMaterialState (material, outFile, indent);

		outFile.WriteLine (indent + "</Material>");
	}

	private void WriteMaterialState (Material materialState, StreamWriter outFile, string indent)
	{
		if (mExportStateMaterial == false) {
			return;
		}

		if (materialState.HasProperty ("_Color")) {
			Color color = materialState.GetColor ("_Color");
			
			outFile.WriteLine (indent + "  " + "<MaterialState Ambient=\"" +
                color.r + ", " + color.g + ", " + color.b + ", " + color.a + "\" />");
		}
	}

	private void WriteTexture (Texture2D texture, StreamWriter outFile, string indent, bool isLightmap = false)
	{
		if (texture == null) {
			return;
		}

		string texName = texture.name + "_" + texture.GetInstanceID ().ToString ("X8") + ".png";
		outFile.WriteLine (indent + "  <Texture Name=\"" + texName + 
            "\" FilterMode=\"" + texture.filterMode + "\" AnisoLevel=\"" + texture.anisoLevel +
            "\" WrapMode=\"" + texture.wrapMode + "\" Mipmaps=\"" + texture.mipmapCount + "\" />");
		
		if (mExportXmlOnly) {
			return;
		}
		
		string assetPath = AssetDatabase.GetAssetPath (texture);
		TextureImporter textureImporter = AssetImporter.GetAtPath (assetPath) as TextureImporter;
		if (textureImporter == null) {
			Debug.Log ("Error getting TextureImporter for '" + texture.name + "'");
			return;
		}

		bool wasReadable = textureImporter.isReadable;
		TextureImporterFormat wasFormat = textureImporter.textureFormat;

		bool needsReimport = false;

		if (wasReadable == false) {
			needsReimport = true;
			textureImporter.isReadable = true;
		}

		if (texture.format == TextureFormat.DXT1) {
			needsReimport = true;
			textureImporter.textureFormat = TextureImporterFormat.RGB24;
		}

		if (texture.format == TextureFormat.DXT5 || texture.format == TextureFormat.RGBA32) {
			needsReimport = true;
			textureImporter.textureFormat = TextureImporterFormat.ARGB32;
		}

		bool alreadyProcessed = false;
		if (!mTextureAssetsProcessed.Contains (assetPath)) {
			mTextureAssetsProcessed.Add (assetPath);

			if (needsReimport) {
				AssetDatabase.ImportAsset (assetPath);
			}
		} else {
			alreadyProcessed = true;
		}

		if (!alreadyProcessed) {
			if (!isLightmap) {
				Byte[] bytes = texture.EncodeToPNG ();
				File.WriteAllBytes (mPath + "/" + texName, bytes);
			} else {                  
				if (texture.format == TextureFormat.ARGB32) {
					Color32[] texSrc = texture.GetPixels32 ();

					Texture2D texRGB = new Texture2D (texture.width, texture.height, TextureFormat.RGB24, false);
					Color32[] texDst = texRGB.GetPixels32 ();

					for (int k = 0; k < texSrc.Length; k++) {
						float[] c = new float[4];
						c [0] = texSrc [k].a / 255.0f;
						c [1] = (texSrc [k].r / 255.0f) * 8.0f * c [0];
						c [2] = (texSrc [k].g / 255.0f) * 8.0f * c [0];
						c [3] = (texSrc [k].b / 255.0f) * 8.0f * c [0];

						c [1] = c [1] > 1.0f ? 1.0f : c [1];
						c [2] = c [2] > 1.0f ? 1.0f : c [2];
						c [3] = c [3] > 1.0f ? 1.0f : c [3];

						texDst [k].a = 255;// (byte)(c[0] * 255.0f);
						texDst [k].r = (byte)(c [1] * 255.0f);
						texDst [k].g = (byte)(c [2] * 255.0f);
						texDst [k].b = (byte)(c [3] * 255.0f);
					}

					texRGB.SetPixels32 (texDst);
					texRGB.Apply ();

					Byte[] bytes = texRGB.EncodeToPNG ();
					File.WriteAllBytes (mPath + "/" + texName, bytes);
				} else {
					Debug.Log ("Lightmap '" + texture.name + "'could not be read as ARGB32.");
				}
			}
		}

		if (needsReimport) {
			textureImporter.isReadable = wasReadable;
			textureImporter.textureFormat = wasFormat;
			if (!alreadyProcessed) {
				AssetDatabase.ImportAsset (assetPath);
			}
		}
	}

	private void WriteMesh (Mesh mesh, StreamWriter outFile, string indent, Vector4 lightmapTilingOffset)
	{
		if (mesh == null) {
			return;
		}

		string lightmapPostfix = "_" + lightmapTilingOffset.x + "_" + lightmapTilingOffset.y +
            "_" + lightmapTilingOffset.z + "_" + lightmapTilingOffset.w;

		string prefix = mesh.name + "_" + mesh.GetInstanceID ().ToString ("X8");
		string meshName = prefix + lightmapPostfix;
		outFile.WriteLine (indent + "<Mesh Name=\"" + meshName + "\">");

		bool alreadyProcessed = true;
		if (!mMeshAssetsProcessed.Contains (meshName)) {
			mMeshAssetsProcessed.Add (meshName);
			alreadyProcessed = false;
		}

		char le = BitConverter.IsLittleEndian ? 'y' : 'n';

		string vtxName = prefix + ".vtx";
		outFile.WriteLine (indent + "  <Vertices Name=\"" + vtxName + "\" LittleEndian=\"" + le + "\" />");
		if (!alreadyProcessed) {
			SaveVector3s (mesh.vertices, vtxName);
		}

		string idxName = prefix + ".idx";
		outFile.WriteLine (indent + "  <Indices Name=\"" + idxName + "\" LittleEndian=\"" + le + "\" />");
		if (!alreadyProcessed) {
			SaveIndices (mesh.triangles, idxName);
		}

		if (mesh.normals.Length > 0) {
			string nmName = prefix + ".nm";
			outFile.WriteLine (indent + "  <Normals Name=\"" + nmName + "\" LittleEndian=\"" + le + "\" />");
			if (!alreadyProcessed) {
				SaveVector3s (mesh.normals, nmName);
			}
		}

		if (mesh.colors.Length > 0) {
			string colName = prefix + ".col";
			outFile.WriteLine (indent + "  <Colors Name=\"" + colName + "\" LittleEndian=\"" + le + "\" />");
			if (!alreadyProcessed) {
				SaveColors (mesh.colors, colName);
			}
		}

		if (mesh.uv.Length > 0) {
			string uv0Name = prefix + ".uv0";
			outFile.WriteLine (indent + "  <Uv0 Name=\"" + uv0Name + "\" LittleEndian=\"" + le + "\" />");
			if (!alreadyProcessed) {
				SaveVector2s (mesh.uv, uv0Name, new Vector4 (1, 1, 0, 0));
			}
		}

		if (mesh.uv2.Length > 0) {
			string uv1Name = prefix + lightmapPostfix + ".uv1";
			outFile.WriteLine (indent + "  <Uv1 Name=\"" + uv1Name + "\" LittleEndian=\"" + le + "\" />");
			if (!alreadyProcessed) {
				SaveVector2s (mesh.uv2, uv1Name, lightmapTilingOffset);
			}
		}
        
		outFile.WriteLine (indent + "</Mesh>");
	}

	private void SaveVector3s (Vector3[] vectors, string name)
	{
		if (mExportXmlOnly) {
			return;
		}
		
		FileStream fileStream = new FileStream (mPath + "/" + name, FileMode.Create);
		BinaryWriter binaryWriter = new BinaryWriter (fileStream);

		for (int i = 0; i < vectors.Length; i++) {
			binaryWriter.Write (-vectors [i].x);
			binaryWriter.Write (vectors [i].y);
			binaryWriter.Write (vectors [i].z);
		}

		binaryWriter.Close ();
		fileStream.Close ();
	}

	private void SaveVector2s (Vector2[] vectors, string name, Vector4 lightmapTilingOffset)
	{
		if (mExportXmlOnly) {
			return;
		}

		FileStream fileStream = new FileStream (mPath + "/" + name, FileMode.Create);
		BinaryWriter binaryWriter = new BinaryWriter (fileStream);

		Vector2 scale = new Vector2 (lightmapTilingOffset.x, lightmapTilingOffset.y);
		Vector2 offset = new Vector2 (lightmapTilingOffset.z, lightmapTilingOffset.w);

		for (int i = 0; i < vectors.Length; i++) {
			Vector2 uv = offset + new Vector2 (scale.x * vectors [i].x, scale.y * vectors [i].y);
			binaryWriter.Write (uv.x);
			binaryWriter.Write (1.0f - uv.y); // OpenGL vs DirectX convention
		}

		binaryWriter.Close ();
		fileStream.Close ();
	}

	private void SaveColors (Color[] colors, string name)
	{
		if (mExportXmlOnly) {
			return;
		}

		FileStream fileStream = new FileStream (mPath + "/" + name, FileMode.Create);
		BinaryWriter binaryWriter = new BinaryWriter (fileStream);

		for (int i = 0; i < colors.Length; i++) {
			binaryWriter.Write (colors [i].r);
			binaryWriter.Write (colors [i].g);
			binaryWriter.Write (colors [i].b);
			binaryWriter.Write (colors [i].a);
		}

		binaryWriter.Close ();
		fileStream.Close ();       
	}

	private void SaveIndices (int[] indices, string name)
	{
		if (mExportXmlOnly) {
			return;
		}

		FileStream fileStream = new FileStream (mPath + "/" + name, FileMode.Create);
		BinaryWriter binaryWriter = new BinaryWriter (fileStream);

		for (int i = 0; i < indices.Length; i++) {
			binaryWriter.Write (indices [i]);
		}

		binaryWriter.Close ();
		fileStream.Close ();
	}   
}
