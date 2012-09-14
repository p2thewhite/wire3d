using UnityEditor;
using UnityEngine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;

public class Unity3DExporter : EditorWindow
{
	private static bool mIsWindowOpen;
	private bool mExportXmlOnly;
	private bool mIgnoreUnderscore;
	private bool mShowAdvancedSettings;
	private bool mExportStateMaterial = true;
	private bool mDontGenerateMipmapsForLightmaps = true;
	private bool mDiscardTexturesOnBind = true;
    private bool mWriteDataAsBigEndian = true;
	private string m2ndTextureName;
	private string mPath = "../Data/Scene";

    private List<string> mMeshAssetsProcessed;
	private Dictionary<string, string> mMeshAssetNameToMeshName;
	private Dictionary<string, int> mMeshNameToCount;

    private List<string> mTextureAssetsProcessed;
	private Dictionary<string, string> mTextureAssetNameToTextureName;
	private Dictionary<string, int> mTextureNameToCount;

    private List<string> mMaterialAssetsProcessed;
    private Dictionary<string, string> mMaterialAssetNameToMaterialName;
    private Dictionary<string, int> mMaterialNameToCount;
   
    private float mSkyboxScale = 120.0f;

	private struct Statistics
	{
		public int LightmapsCount;
		public int LightmapsTotalSizeOnDisk;
		public int LightmapsTotalSizeInRam;
		public int TexturesCount;
		public int TexturesTotalSizeOnDisk;
		public int TexturesTotalSizeInRam;

		public void Print ()
		{
			Debug.Log ("---> Export Statistics <---\n");
			Debug.Log ("Textures: " + TexturesCount + " (Lightmaps: " + LightmapsCount + ")\n");
			Debug.Log ("Textures total size on disk: " + (TexturesTotalSizeOnDisk / (1024.0f * 1024.0f)).ToString ("F2") + " MB (Lightmaps: " + (LightmapsTotalSizeOnDisk / (1024.0f * 1024.0f)).ToString ("F2") + " MB)\n");
			Debug.Log ("Textures (including mipmaps) total size in RAM: " + (TexturesTotalSizeInRam / (1024.0f * 1024.0f)).ToString ("F2") + " MB (Lightmaps: " + (LightmapsTotalSizeInRam / (1024.0f * 1024.0f)).ToString ("F2") + " MB)\n");
		}
	}

	private Statistics mStatistics;

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
	
	[MenuItem("Wire3D/Generate Box Colliders")]
	protected static void GenerateBoxColliders ()
	{
		foreach (Transform transform in GetRootTransforms ()) {
			GenerateBoxColliderTraverse (transform);
		}
	}

	private static List<Transform> GetRootTransforms ()
	{
		List<Transform> allTransforms = new List<Transform> (FindObjectsOfType (typeof(Transform)) as Transform[]);
		List<Transform> rootTransforms = allTransforms.Where (t => t.parent == null).ToList ();
		return (from s in rootTransforms orderby s.name select s).ToList ();
	}
	
	private static bool IsLeaf (Transform transform)
	{
		GameObject gameObject = transform.gameObject;
		MeshFilter meshFilter = gameObject.GetComponent<MeshFilter> ();
		MeshRenderer meshRenderer = gameObject.GetComponent<MeshRenderer> ();
		
		return (meshFilter != null && meshFilter.sharedMesh != null) &&
            (meshRenderer != null && meshRenderer.enabled);
	}
	
	private static void GenerateAABBFromVertices (Vector3[] vertices, out Vector3 aabbMin, out Vector3 aabbMax)
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
		if (IsLeaf (transform))
        {
			GenerateBoxCollider (transform);
		}

		if (transform.GetChildCount () > 0)
        {
			for (int i = 0; i < transform.GetChildCount(); i++)
            {
				GenerateBoxColliderTraverse (transform.GetChild (i));
			}
		}
	}

	private static void GenerateBoxCollider (Transform transform)
	{
		MeshFilter meshFilter = transform.GetComponent<MeshFilter> ();
		Mesh mesh = meshFilter.sharedMesh;
		
		Vector3 aabbMin;
		Vector3 aabbMax;
		GenerateAABBFromVertices (mesh.vertices, out aabbMin, out aabbMax);
			
		GameObject colliderGameObject = new GameObject ("Collider for " + transform.gameObject.name);
		
		Vector3 aabbSize = new Vector3 (aabbMax.x - aabbMin.x, aabbMax.y - aabbMin.y, aabbMax.z - aabbMin.z);
		Vector3 aabbCenter = aabbMin + (aabbSize * 0.5f);
		
		colliderGameObject.transform.position = aabbCenter;
		BoxCollider boxCollider = colliderGameObject.AddComponent<BoxCollider> ();
		boxCollider.size = aabbSize;
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

		mExportXmlOnly = GUILayout.Toggle (mExportXmlOnly, new GUIContent ("Export scene XML file only", "Textures, meshes, etc. will not be exported."));
		mIgnoreUnderscore = GUILayout.Toggle (mIgnoreUnderscore, "Ignore GameObjects starting with '_'");

		if (RenderSettings.skybox != null) {
			mSkyboxScale = EditorGUILayout.FloatField ("Skybox Size", mSkyboxScale);
		}
		
		// ---
		GUILayout.Space (10);

		mShowAdvancedSettings = EditorGUILayout.Foldout (mShowAdvancedSettings, "Advanced Settings");
		if (mShowAdvancedSettings) {
			mExportStateMaterial = GUILayout.Toggle (mExportStateMaterial, new GUIContent (
                "Try to export StateMaterial from Shader",
                "The 'Main Color' (if available) of a shader will be exported as the StateMaterial's ambient color."));
			mDontGenerateMipmapsForLightmaps = GUILayout.Toggle (mDontGenerateMipmapsForLightmaps,
                "Do not generate Mipmaps for Lightmaps");
			mDiscardTexturesOnBind = GUILayout.Toggle (mDiscardTexturesOnBind, new GUIContent (
                "Flag textures as 'Discard on bind'",
                "Textures that are not discarded at bind time can be bound multiple times, but at the cost of double ram usage per texture."));
            mWriteDataAsBigEndian = GUILayout.Toggle(mWriteDataAsBigEndian, new GUIContent(
                "Write data as big endian",
                "PC uses little endian. Wii uses big endian. Byte order will be rearranged by the Importer if necessary. Thus favoring a particular order gives a slight speedup when importing data on that particular platform."));
			GUILayout.Label ("Use property names:");
			m2ndTextureName = EditorGUILayout.TextField ("- 2nd Texture ", m2ndTextureName ?? string.Empty);
		}
		
		// ---
		GUILayout.Space (20);
		
		if (dirExists && !string.IsNullOrEmpty (mPath)) {
			if (GUILayout.Button ("Export")) {
				if (mPath [mPath.Length - 1] != '/' && mPath [mPath.Length - 1] != '\\') {
					mPath = mPath + '/';
				}

				mMeshAssetsProcessed = new List<string> ();
				mMeshAssetNameToMeshName = new Dictionary<string, string> ();
				mMeshNameToCount = new Dictionary<string, int> ();

				mTextureAssetsProcessed = new List<string> ();
				mTextureAssetNameToTextureName = new Dictionary<string, string> ();
				mTextureNameToCount = new Dictionary<string, int> ();

                mMaterialAssetsProcessed = new List<string>();
                mMaterialAssetNameToMaterialName = new Dictionary<string, string>();
                mMaterialNameToCount = new Dictionary<string, int>();

				mStatistics = new Statistics ();
				Export ();
				if (!mExportXmlOnly) {
					mStatistics.Print ();
				}
			}
		}	
	}

	private static string GetUniqueName (string originalName, Dictionary<string, int> context)
	{
		int nameUsageCount;
		if (context.ContainsKey (originalName)) {
			nameUsageCount = context [originalName];
		} else {
			nameUsageCount = 0;
		}

		context [originalName] = ++nameUsageCount;

		if (nameUsageCount > 1) {
            string newName = originalName + "_" + nameUsageCount + "_(renamed)";
            Debug.LogWarning("Renaming '" + originalName + "' to '" + newName + "'");
			return newName;
		} else {
			return originalName;
		}
	}

	private void WriteLeaf (Transform transform, StreamWriter outFile, string indent, bool writeComponents = true)
	{
		MeshFilter meshFilter = transform.gameObject.GetComponent<MeshFilter> ();
		MeshRenderer meshRenderer = transform.gameObject.GetComponent<MeshRenderer> ();
		
		if ((meshRenderer == null) || (meshFilter == null)) {
			return;
		}

        string isStatic = transform.gameObject.isStatic ? " Static=\"1\"" : "";

        int submeshCount = meshFilter.sharedMesh.subMeshCount;
        bool exportSubmeshes = submeshCount > 1 ? true : false;       
        string xmlNodeName = exportSubmeshes ? "Node" : "Leaf";
		outFile.WriteLine (indent + "<" + xmlNodeName + " Name=\"" + transform.gameObject.name + "\" " + GetTransformAsString (transform) + isStatic + ">");

		if (writeComponents) {
			WriteCamera (transform.gameObject, outFile, indent);
			WriteLight (transform.gameObject, outFile, indent);
			WriteCollider (transform.gameObject, outFile, indent);
		}

        if (exportSubmeshes)
        {
            WriteSubLeafs(transform.gameObject, outFile, indent + "  ");
        }
        else
        {
            WriteMesh(meshFilter.sharedMesh, meshRenderer.lightmapTilingOffset, outFile, indent + "  ");
            WriteMaterial(meshRenderer, meshRenderer.sharedMaterial, outFile, indent + "  ");
        }

        outFile.WriteLine(indent + "</" + xmlNodeName + ">");       
	}

    private void WriteSubLeafs(GameObject gameObject, StreamWriter outFile, string indent)
    {
        MeshFilter meshFilter = gameObject.GetComponent<MeshFilter>();
        MeshRenderer meshRenderer = gameObject.GetComponent<MeshRenderer>();

        if ((meshRenderer == null) || (meshFilter == null))
        {
            return;
        }

        Mesh mesh = meshFilter.sharedMesh;
        int submeshCount = mesh.subMeshCount;
        for (int i = 0; i < submeshCount; i++)
        {
            string isStatic = gameObject.isStatic ? " Static=\"1\"" : "";
            outFile.WriteLine(indent + "<Leaf Name=\"" + gameObject.name + " (submesh_" + i + ")\"" + isStatic +
                " SubMeshIndex=\"" + i + "\">");
            WriteMesh(mesh, meshRenderer.lightmapTilingOffset, outFile, indent + "  ");

            WriteMaterial(meshRenderer, meshRenderer.sharedMaterials[i], outFile, indent + "  ");
            outFile.WriteLine(indent + "</Leaf>");
        }
    }

	private void WriteNode (Transform transform, StreamWriter outFile, string indent)
	{
		outFile.WriteLine (indent + "<Node Name=\"" + transform.gameObject.name + "\" " + GetTransformAsString (transform) + ">");
    	
		WriteCamera (transform.gameObject, outFile, indent);
		WriteLight (transform.gameObject, outFile, indent);
		WriteCollider (transform.gameObject, outFile, indent);
    	
		if (transform.GetChildCount () > 0)
        {
			if (IsLeaf(transform))
            {
				WriteLeaf (transform, outFile, indent + "  ", false);
			}
    	
			for (int i = 0; i < transform.GetChildCount(); i++) {
				Traverse (transform.GetChild (i), outFile, indent + "  ");
			}
		}
    	
		outFile.WriteLine (indent + "</Node>");
	}

	private void WriteSkybox (StreamWriter outFile, string indent)
	{
		Material skyboxMaterial = RenderSettings.skybox;
		if (skyboxMaterial == null) {
			return;
		}

		Shader skyBoxShader = skyboxMaterial.shader;
		if (skyBoxShader == null) {
			return;
		}

		if (!skyBoxShader.name.Equals ("RenderFX/Skybox")) {
			return;
		}

		Texture2D posZTexture2D = GetTextureFromMaterial (skyboxMaterial, "_FrontTex");
		Texture2D negZTexture2D = GetTextureFromMaterial (skyboxMaterial, "_BackTex");
		Texture2D posXTexture2D = GetTextureFromMaterial (skyboxMaterial, "_LeftTex");
		Texture2D negXTexture2D = GetTextureFromMaterial (skyboxMaterial, "_RightTex");
		Texture2D posYTexture2D = GetTextureFromMaterial (skyboxMaterial, "_UpTex");
		Texture2D negYTexture2D = GetTextureFromMaterial (skyboxMaterial, "_DownTex");

		outFile.WriteLine (indent + "<Skybox Scale=\"" + mSkyboxScale + "\">");

		outFile.WriteLine (indent + "  " + "<PosZ>");
		WriteTexture (posZTexture2D, outFile, indent + "  ");
		outFile.WriteLine (indent + "  " + "</PosZ>");

		outFile.WriteLine (indent + "  " + "<NegZ>");
		WriteTexture (negZTexture2D, outFile, indent + "  ");
		outFile.WriteLine (indent + "  " + "</NegZ>");

		outFile.WriteLine (indent + "  " + "<PosX>");
		WriteTexture (negXTexture2D, outFile, indent + "  ");
		outFile.WriteLine (indent + "  " + "</PosX>");

		outFile.WriteLine (indent + "  " + "<NegX>");
		WriteTexture (posXTexture2D, outFile, indent + "  ");
		outFile.WriteLine (indent + "  " + "</NegX>");

		outFile.WriteLine (indent + "  " + "<PosY>");
		WriteTexture (posYTexture2D, outFile, indent + "  ");
		outFile.WriteLine (indent + "  " + "</PosY>");

		outFile.WriteLine (indent + "  " + "<NegY>");
		WriteTexture (negYTexture2D, outFile, indent + "  ");
		outFile.WriteLine (indent + "  " + "</NegY>");

        outFile.WriteLine(indent + "<FogState Enabled=\"0\" />");

		outFile.WriteLine (indent + "</Skybox>");
	}
    
	static private Texture2D GetTextureFromMaterial (Material material, string propertyName)
	{
		Texture2D texture = null;

		if (material.HasProperty (propertyName)) {
			texture = material.GetTexture (propertyName) as Texture2D;
		}

		return texture;
	}

	private void Traverse (Transform transform, StreamWriter outFile, string indent)
	{
		if (mIgnoreUnderscore && transform.gameObject.name.StartsWith ("_"))
        {
			return;
		}
      	
		if (IsLeaf (transform) && transform.GetChildCount() == 0)
        {
			WriteLeaf (transform, outFile, indent);
		}
        else
        {
			WriteNode (transform, outFile, indent);
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
            WriteAssets(outFile);

            GameObject root = new GameObject ("Root");
			outFile.WriteLine ("<Node Name=\"" + root.name + "\" " + GetTransformAsString (root.transform) + ">");
			DestroyImmediate (root);
			string indent = "  ";

            WriteStateFog (outFile, indent);

			foreach (Transform transform in GetRootTransforms ()) {
				Traverse (transform, outFile, indent);
			}
	
			WriteSkybox (outFile, indent);

			outFile.WriteLine ("</Node>");
		} finally {
			outFile.Close ();
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

    private void WriteAssets(StreamWriter outFile)
    {
        outFile.WriteLine("<Assets>");

        string indent = "    ";
        outFile.WriteLine("  <Meshes>");
        WriteAssets(outFile, indent, true, false);
        outFile.WriteLine("  </Meshes>");

        outFile.WriteLine("  <Materials>");
        WriteAssets(outFile, indent, false, true);
        outFile.WriteLine("  </Materials>");

        outFile.WriteLine("</Assets>");
    }

    private void WriteAssets(StreamWriter outFile, string indent, bool writeMeshes, bool writeMaterials)
    {
        Stack<Transform> stack = new Stack<Transform>();
        foreach (Transform transform in GetRootTransforms())
        {
            stack.Push(transform);
        }

        while (stack.Count > 0)
        {
            Transform t = stack.Pop();
            for (int i = t.GetChildCount()-1; i >= 0; i--)
            {
                stack.Push(t.GetChild(i));
            }

            if (IsLeaf(t))
            {
                MeshFilter meshFilter = t.gameObject.GetComponent<MeshFilter>();
                MeshRenderer meshRenderer = t.gameObject.GetComponent<MeshRenderer>();
                if (meshFilter == null || meshRenderer == null)
                {
                    continue;
                }

                if (writeMeshes)
                {
                    string meshAssetName = GetMeshAssetName(meshFilter.sharedMesh, meshRenderer.lightmapTilingOffset);
                    if (!mMeshAssetsProcessed.Contains(meshAssetName))
                    {
                        WriteMesh(meshFilter.sharedMesh, meshRenderer.lightmapTilingOffset, outFile, indent);
                    }
                }

                if (writeMaterials)
                {
                    foreach (Material material in meshRenderer.sharedMaterials)
                    {
                        string materialAssetName = GetMaterialAssetName(material, meshRenderer);
                        if (!mMaterialAssetsProcessed.Contains(materialAssetName))
                        {
                            WriteMaterial(meshRenderer, material, outFile, indent);
                        }
                    }
                }
            }
        }
    }

    private void WriteStateFog (StreamWriter outFile, string indent)
    {
        string enabled = RenderSettings.fog ? "1" : "0";
        Color color = RenderSettings.fogColor;
        string mode = "LINEAR";
        if (RenderSettings.fogMode == FogMode.Exponential)
        {
            mode = "EXP";
        }
        else if (RenderSettings.fogMode == FogMode.ExponentialSquared)
        {
            mode = "EXPSQR";
        }

        outFile.WriteLine(indent + "<FogState Enabled=\"" + enabled + "\" Color=\"" + color.r + ", " + color.g + ", " + color.b + "\" " +
            "Start=\"" + RenderSettings.fogStartDistance + "\" End=\"" + RenderSettings.fogEndDistance + "\"" + " Mode=\"" + mode + "\" />");
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

    private string GetMaterialName(Material material, MeshRenderer meshRenderer)
    {
        string lightMapIndex = string.Empty;
        if (meshRenderer.lightmapIndex != -1)
        {
            lightMapIndex = "_" + meshRenderer.lightmapIndex;
        }

        return material.name + lightMapIndex;
    }

    private string GetMaterialAssetName(Material material, MeshRenderer meshRenderer)
    {
        return GetMaterialName(material, meshRenderer) + "_" + material.GetInstanceID().ToString("X8");
    }

    private void WriteMaterial (MeshRenderer meshRenderer, Material material, StreamWriter outFile, string indent)
	{
		if (material == null) {
			return;
		}

        string materialName = GetMaterialName(material, meshRenderer);
        string materialAssetName = GetMaterialAssetName(material, meshRenderer);
        bool alreadyProcessed = true;
        if (!mMaterialAssetsProcessed.Contains(materialAssetName))
        {
            mMaterialAssetsProcessed.Add(materialAssetName);
            mMaterialAssetNameToMaterialName.Add(materialAssetName, GetUniqueName(materialName, mMaterialNameToCount));
            alreadyProcessed = false;
        }

        materialName = mMaterialAssetNameToMaterialName[materialAssetName];

        if (alreadyProcessed)
        {
            outFile.WriteLine(indent + "<Material Name=\"" + materialName + "\" />");
            return;
        }
        else
        {
            outFile.WriteLine(indent + "<Material Name=\"" + materialName + "\">");
        }

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

		string texName = texture.name + "_" + texture.GetInstanceID ().ToString ("X8");
		string assetPath = AssetDatabase.GetAssetPath (texture);

		bool alreadyProcessed = true;
		if (!mTextureAssetsProcessed.Contains (assetPath)) {
			mTextureAssetsProcessed.Add (assetPath);
			mTextureAssetNameToTextureName.Add (texName, GetUniqueName (texture.name, mTextureNameToCount));
			alreadyProcessed = false;
		}

		texName = mTextureAssetNameToTextureName [texName] + ".png";

		int mipmapCount = (mDontGenerateMipmapsForLightmaps && isLightmap) ? 1 : texture.mipmapCount;

		string textureXmlNode = indent + "  <Texture Name=\"" + texName +
            "\" FilterMode=\"" + texture.filterMode + "\" AnisoLevel=\"" + texture.anisoLevel +
            "\" WrapMode=\"" + texture.wrapMode + "\" Mipmaps=\"" + mipmapCount + "\" ";

		if (mDiscardTexturesOnBind) {
			textureXmlNode += "Usage=\"STATIC_DISCARD_ON_BIND\" ";
		}

		textureXmlNode += "/>";

		outFile.WriteLine (textureXmlNode);
		
		if (mExportXmlOnly) {
			return;
		}

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

		if (!alreadyProcessed) {
			if (needsReimport) {
				AssetDatabase.ImportAsset (assetPath);
			}
		}

		if (!alreadyProcessed) {
			int bpp = textureImporter.DoesSourceTextureHaveAlpha () ? 4 : 3;
			bpp = isLightmap ? 3 : bpp;

			mStatistics.TexturesTotalSizeInRam += GetSizeFromLevelOfMipmaps (mipmapCount, texture) * bpp;
			mStatistics.TexturesCount++;

			if (!isLightmap) {
				Byte[] bytes = texture.EncodeToPNG ();
				File.WriteAllBytes (mPath + "/" + texName, bytes);
				mStatistics.TexturesTotalSizeOnDisk += bytes.Length;
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
					mStatistics.LightmapsCount++;
					mStatistics.LightmapsTotalSizeOnDisk += bytes.Length;
					mStatistics.TexturesTotalSizeOnDisk += bytes.Length;
					mStatistics.LightmapsTotalSizeInRam += GetSizeFromLevelOfMipmaps (mipmapCount, texture) * bpp;
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

    private string GetMeshAssetName(Mesh mesh, Vector4 lightmapTilingOffset)
    {
        string lightmapPostfix = "_" + lightmapTilingOffset.x + "_" + lightmapTilingOffset.y + "_" +
            lightmapTilingOffset.z + "_" + lightmapTilingOffset.w;

        return mesh.name + lightmapPostfix + "_" + mesh.GetInstanceID().ToString("X8");
    }

	private void WriteMesh (Mesh mesh, Vector4 lightmapTilingOffset, StreamWriter outFile, string indent)
	{
		if (mesh == null) {
			return;
		}

        string meshName = mesh.name;
        string meshAssetName = GetMeshAssetName(mesh, lightmapTilingOffset);
		bool alreadyProcessed = true;
        if (!mMeshAssetsProcessed.Contains(meshAssetName))
        {
            mMeshAssetsProcessed.Add(meshAssetName);
            mMeshAssetNameToMeshName.Add(meshAssetName, GetUniqueName(meshName, mMeshNameToCount));
			alreadyProcessed = false;
		}

        meshName = mMeshAssetNameToMeshName[meshAssetName];

        if (alreadyProcessed)
        {
            outFile.WriteLine(indent + "<Mesh Name=\"" + meshName + "\" />");
            return;
        }

        outFile.WriteLine(indent + "<Mesh Name=\"" + meshName + "\">");

        if (mesh.subMeshCount > 1)
        {
            outFile.WriteLine (indent + "  <SubMeshes>");

            int startIndex = 0;
            
            for (int i = 0; i < mesh.subMeshCount; i++)
            {
                int indexCount = mesh.GetTriangles(i).Length;
                outFile.WriteLine (indent + "    " + "<SubMesh Index=\"" + i + "\" StartIndex=\"" + startIndex +
                    "\" IndexCount=\"" + indexCount + "\" />");
                startIndex += indexCount;
            }

            outFile.WriteLine (indent + "  </SubMeshes>");
        }

		string le = mWriteDataAsBigEndian ? string.Empty : " LittleEndian=\"1\"";

        string vtxName = meshName + ".vtx";
		outFile.WriteLine (indent + "  <Vertices Name=\"" + vtxName + "\"" + le + " />");
		if (!alreadyProcessed) {
			SaveVector3s (mesh.vertices, vtxName);
		}

        bool is16Bit = true;
        int[] triangles = mesh.triangles;
        foreach (int i in triangles)
        {
            if (i > 65535)
            {
                is16Bit = false;
                break;
            }
        }

        if (!is16Bit)
        {
            Debug.LogError("Mesh '" + mesh.name + "' uses 32bit indices.");
        }

        string is16BitString = is16Bit ? " 16bit=\"1\"" : string.Empty;
        string idxName = meshName + ".idx";
        outFile.WriteLine(indent + "  <Indices Name=\"" + idxName + "\"" + le + is16BitString + " />");
		if (!alreadyProcessed) {
            SaveIndices(mesh.triangles, idxName, is16Bit);
		}

		if (mesh.normals.Length > 0) {
            string nmName = meshName + ".nm";
			outFile.WriteLine (indent + "  <Normals Name=\"" + nmName + "\"" + le + " />");
			if (!alreadyProcessed) {
				SaveVector3s (mesh.normals, nmName);
			}
		}

		if (mesh.colors.Length > 0) {
            string colName = meshName + ".col";
			outFile.WriteLine (indent + "  <Colors Name=\"" + colName + "\"" + le + " />");
			if (!alreadyProcessed) {
				SaveColors (mesh.colors, colName);
			}
		}

		if (mesh.uv.Length > 0) {
            string uv0Name = meshName + ".uv0";
			outFile.WriteLine (indent + "  <Uv0 Name=\"" + uv0Name + "\"" + le + " />");
			if (!alreadyProcessed) {
				SaveVector2s (mesh.uv, uv0Name, new Vector4 (1, 1, 0, 0));
			}
		}

		if (mesh.uv2.Length > 0) {
            string uv1Name = meshName + ".uv1";
			outFile.WriteLine (indent + "  <Uv1 Name=\"" + uv1Name + "\"" + le + " />");
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

		for (int i = 0; i < vectors.Length; i++)
        {
            Vector3 vec = vectors[i];
            vec.x = -vec.x;

            WriteFloat(vec.x, binaryWriter);
            WriteFloat(vec.y, binaryWriter);
            WriteFloat(vec.z, binaryWriter);
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

		for (int i = 0; i < vectors.Length; i++)
        {
			Vector2 uv = offset + new Vector2 (scale.x * vectors [i].x, scale.y * vectors [i].y);
            uv.y = 1.0f - uv.y; // OpenGL vs DirectX convention

            WriteFloat(uv.x, binaryWriter);
            WriteFloat(uv.y, binaryWriter);
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

		for (int i = 0; i < colors.Length; i++)
        {
            WriteFloat(colors[i].r, binaryWriter);
            WriteFloat(colors[i].g, binaryWriter);
            WriteFloat(colors[i].b, binaryWriter);
            WriteFloat(colors[i].a, binaryWriter);
		}

		binaryWriter.Close ();
		fileStream.Close ();       
	}

    private void WriteFloat(float f, BinaryWriter writer)
    {
        if ((BitConverter.IsLittleEndian && mWriteDataAsBigEndian) ||
            (!BitConverter.IsLittleEndian && !mWriteDataAsBigEndian))
        {
            byte[] x = BitConverter.GetBytes(f);
            Array.Reverse(x);
            writer.Write(x);
        }
        else
        {
            writer.Write(f);
        }
    }

    private void SaveIndices(int[] indices, string name, bool is16Bit)
	{
		if (mExportXmlOnly) {
			return;
		}

		FileStream fileStream = new FileStream (mPath + "/" + name, FileMode.Create);
		BinaryWriter binaryWriter = new BinaryWriter (fileStream);

        if (is16Bit)
        {
            for (int i = 0; i < indices.Length; i++)
            {
                ushort s = (ushort)indices[i];
                if ((BitConverter.IsLittleEndian && mWriteDataAsBigEndian) ||
                    (!BitConverter.IsLittleEndian && !mWriteDataAsBigEndian))
                {
                    byte[] x = BitConverter.GetBytes(s);
                    Array.Reverse(x);
                    binaryWriter.Write(x);
                }
                else
                {
                    binaryWriter.Write(s);
                }
            }
        }
        else
        {
            for (int i = 0; i < indices.Length; i++)
            {
                if ((BitConverter.IsLittleEndian && mWriteDataAsBigEndian) ||
                    (!BitConverter.IsLittleEndian && !mWriteDataAsBigEndian))
                {
                    byte[] x = BitConverter.GetBytes(indices[i]);
                    Array.Reverse(x);
                    binaryWriter.Write(x);
                }
                else
                {
                    binaryWriter.Write(indices[i]);
                }
            }
        }

		binaryWriter.Close ();
		fileStream.Close ();
	}

	private static int GetSizeFromLevelOfMipmaps (int mipmapCount, Texture2D texture)
	{
		if (mipmapCount == 1) {
			return texture.width * texture.height;
		}

		int size = 0;
		for (int i = 0; i < mipmapCount; i++) {
			int n = 1 << i;
			size += (n * n);
		}

		return size;
	}
}