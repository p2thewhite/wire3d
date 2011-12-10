using UnityEditor;
using UnityEngine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;

public class Exporter : EditorWindow
{
    private static bool mIsWindowOpen;

    private string mPath;
    private List<string> mMeshAssetsProcessed;
    private List<string> mTextureAssetsProcessed;

    [MenuItem("Wire3D/Exporter")]
    protected static void CreateWizard()
    {
        if (!mIsWindowOpen)
        {
            mIsWindowOpen = true;
            Exporter editor = GetWindow((typeof(Exporter))) as Exporter;
            if (editor)
            {
                editor.autoRepaintOnSceneChange = true;
            }
        }
    }

    protected void OnDestroy()
    {
        mIsWindowOpen = false;
    }

    protected virtual void OnGUI()
    {
        EditorGUILayout.BeginHorizontal();
        EditorGUIUtility.LookLikeControls();

        GUILayout.Label("Save to:");
        mPath = EditorGUILayout.TextField("", mPath ?? string.Empty);

        EditorGUILayout.EndHorizontal();

        bool dirExists = Directory.Exists(mPath);

        if (!dirExists && !string.IsNullOrEmpty(mPath))
        {
            GUILayout.Label("Specified path '" + mPath + "' does not exist.");
        }

        if (dirExists && !string.IsNullOrEmpty(mPath))
        {
            if (GUILayout.Button("Export"))
            {
                if (mPath[mPath.Length - 1] != '/' && mPath[mPath.Length - 1] != '\\')
                {
                    mPath = mPath + '/';
                }

                mMeshAssetsProcessed = new List<string>();
                mTextureAssetsProcessed = new List<string>();
                Export();
            }
        }
    }

    private void Export()
    {
		string[] unityScenePath = EditorApplication.currentScene.Split(char.Parse("/"));
		string unitySceneName = unityScenePath[unityScenePath.Length -1];
		if (string.IsNullOrEmpty(unitySceneName))
		{
			Debug.Log("Scene is empty, nothing to do.");
		}
        else
        {
            if (unitySceneName.Substring(unitySceneName.Length - 6, 6).Equals(".unity"))
            {
                unitySceneName = unitySceneName.Remove(unitySceneName.Length - 6);
            }
        }
        
        StreamWriter outfile = new StreamWriter(mPath + unitySceneName + ".xml");
	
        List<Transform> allTrafos = new List<Transform>(FindObjectsOfType(typeof (Transform)) as Transform[]);

        List<Transform> rootTrafos = allTrafos.Where(t => t.parent == null).ToList();
        rootTrafos = (from s in rootTrafos orderby s.name select s).ToList();

        GameObject root = new GameObject("Root");
        outfile.WriteLine("<Node Name=\"" + root.name + "\" " + GetTransform(root.transform) + ">");
        string indent = "  ";

        foreach (Transform t in rootTrafos)
        {
            Traverse(t, outfile, indent);
        }

        DestroyImmediate(root);
        outfile.WriteLine("</Node>");
        outfile.Close();
    }

    private void Traverse(Transform t, StreamWriter outfile, string indent)
    {
        GameObject go = t.gameObject;
        MeshFilter mf = go.GetComponent<MeshFilter>();
        MeshRenderer mr = go.GetComponent<MeshRenderer>();

        if ((mf != null) && (mr != null) && t.GetChildCount() == 0)
        {
            WriteLeaf(t, outfile, indent);
            return;
        }

        outfile.WriteLine(indent + "<Node Name=\"" + t.gameObject.name + "_" +
            t.gameObject.GetInstanceID().ToString("X8") + "\" " + GetTransform(t) + ">");

        if (t.GetChildCount() > 0)
        {
            if ((mf != null) && (mr != null))
            {
                WriteLeaf(t, outfile, indent);
                return;
            }

            for (int i = 0; i < t.GetChildCount(); i++)
            {
                Traverse(t.GetChild(i), outfile, indent + "  ");
            }
        }

        outfile.WriteLine(indent + "</Node>");
    }

    private string GetTransform(Transform t)
    {
        Vector3 pos = t.localPosition;
        Quaternion rot = t.localRotation;
        Vector3 scale = t.localScale;
        return "Pos=\"" + pos.x + ", " + pos.y + ", " + pos.z + "\" " +
            "Rot=\"" + rot.w + ", " + rot.x + ", " + (-rot.y) + ", " + (-rot.z) + "\" " +
            "Scale=\"" + scale.x + ", " + scale.y + ", " + scale.z + "\"";
    }

    private void WriteLeaf(Transform t, StreamWriter outfile, string indent)
    {
        MeshFilter mf = t.gameObject.GetComponent<MeshFilter>();
        MeshRenderer mr = t.gameObject.GetComponent<MeshRenderer>();
        if ((mr == null) || (mf == null))
        {
            return;
        }

        string isStatic = " Static=\"" + (t.gameObject.isStatic ? "1" : "0") + "\"";
        outfile.WriteLine(indent + "<Leaf Name=\"" + t.gameObject.name + "_" +
            t.gameObject.GetInstanceID().ToString("X8") + "\" " + GetTransform(t) + isStatic + ">");
        WriteMesh(mf.sharedMesh, outfile, indent + "  ", mr.lightmapTilingOffset);
        WriteMaterial(mr, outfile, indent + "  ");
        outfile.WriteLine(indent + "</Leaf>");       
    }

    private void WriteMaterial(MeshRenderer mr, StreamWriter outfile, string indent)
    {
        Material m = mr.sharedMaterial;
        if (m == null)
        {
            return;
        }

        string lightMapIndex = string.Empty;
        if (mr.lightmapIndex != -1)
        {
            lightMapIndex = "_" + mr.lightmapIndex;
        }

        outfile.WriteLine(indent + "<Material Name=\"" + m.name + lightMapIndex + "\">");      

        Texture2D tex = m.mainTexture as Texture2D;
        WriteTexture(tex, outfile, indent);

        if (mr.lightmapIndex != -1 && mr.lightmapIndex != 254)
        {
            Texture2D lightmap = LightmapSettings.lightmaps[mr.lightmapIndex].lightmapFar;
            WriteTexture(lightmap, outfile, indent, true);               
        }

        outfile.WriteLine(indent + "</Material>");
    }

    private void WriteTexture(Texture2D tex, StreamWriter outfile, string indent, bool isLightmap = false)
    {
        if (tex)
        {
            string assetPath = AssetDatabase.GetAssetPath(tex);
            TextureImporter ti = AssetImporter.GetAtPath(assetPath) as TextureImporter;
            if (ti == null)
            {
                Debug.Log("Error getting TextureImporter for '" + tex.name + "'");
                return;
            }

            bool wasReadable = ti.isReadable;
            TextureImporterFormat wasFormat = ti.textureFormat;

            bool needsReimport = false;

            if (wasReadable == false)
            {
                needsReimport = true;
                ti.isReadable = true;
            }

            if (tex.format == TextureFormat.DXT1)
            {
                needsReimport = true;
                ti.textureFormat = TextureImporterFormat.RGB24;
            }

            if (tex.format == TextureFormat.DXT5)
            {
                needsReimport = true;
                ti.textureFormat = TextureImporterFormat.ARGB32;
            }

            bool alreadyProcessed = false;
            if (!mTextureAssetsProcessed.Contains(assetPath))
            {
                mTextureAssetsProcessed.Add(assetPath);

                if (needsReimport)
                {
                    AssetDatabase.ImportAsset(assetPath);
                }
            }
            else
            {
                alreadyProcessed = true;
            }

            string texName = tex.name + "_" + tex.GetInstanceID().ToString("X8") + ".png";
            outfile.WriteLine(indent + "  <Texture Name=\"" + texName + 
                "\" FilterMode=\"" + tex.filterMode + "\" AnisoLevel=\"" + tex.anisoLevel +
                "\" WrapMode=\"" + tex.wrapMode + "\" Mipmaps=\"" + tex.mipmapCount + "\" />");

            if (!alreadyProcessed)
            {
                if (!isLightmap)
                {
                    Byte[] bytes = tex.EncodeToPNG();
                    File.WriteAllBytes(mPath + "/" + texName, bytes);
                }
                else
                {                  
                    if (tex.format == TextureFormat.ARGB32)
                    {
                        Color32[] texSrc = tex.GetPixels32();

                        Texture2D texRGB = new Texture2D(tex.width, tex.height, TextureFormat.RGB24, false);
                        Color32[] texDst = texRGB.GetPixels32();

                        for (int k = 0; k < texSrc.Length; k++)
                        {
                            float[] c = new float[4];
                            c[0] = texSrc[k].a / 255.0f;
                            c[1] = (texSrc[k].r / 255.0f) * 8.0f * c[0];
                            c[2] = (texSrc[k].g / 255.0f) * 8.0f * c[0];
                            c[3] = (texSrc[k].b / 255.0f) * 8.0f * c[0];

                            c[1] = c[1] > 1.0f ? 1.0f : c[1];
                            c[2] = c[2] > 1.0f ? 1.0f : c[2];
                            c[3] = c[3] > 1.0f ? 1.0f : c[3];

                            texDst[k].a = 255;// (byte)(c[0] * 255.0f);
                            texDst[k].r = (byte)(c[1] * 255.0f);
                            texDst[k].g = (byte)(c[2] * 255.0f);
                            texDst[k].b = (byte)(c[3] * 255.0f);
                        }

                        texRGB.SetPixels32(texDst);
                        texRGB.Apply();

                        Byte[] bytes = texRGB.EncodeToPNG();
                        File.WriteAllBytes(mPath + "/" + texName, bytes);
                    }
                    else
                    {
                        Debug.Log("Lightmap '" + tex.name + "'could not be read as ARGB32.");
                    }
                }
            }

            if (needsReimport)
            {
                ti.isReadable = wasReadable;
                ti.textureFormat = wasFormat;
                if (!alreadyProcessed)
                {
                    AssetDatabase.ImportAsset(assetPath);
                }
            }
        }
    }

    private void WriteMesh(Mesh m, StreamWriter outfile, string indent, Vector4 lightmapTilingOffset)
    {
        if (m == null)
        {
            return;
        }

        string lightmapPostfix = "_" + lightmapTilingOffset.x + "_" + lightmapTilingOffset.y +
            "_" + lightmapTilingOffset.z + "_" + lightmapTilingOffset.w;

        string meshName = m.name + "_" + m.GetInstanceID().ToString("X8") + lightmapPostfix;
        outfile.WriteLine(indent + "<Mesh Name=\"" + meshName + "\">");

        bool alreadyProcessed = true;
        if (!mMeshAssetsProcessed.Contains(meshName))
        {
            mMeshAssetsProcessed.Add(meshName);
            alreadyProcessed = false;
        }

        char le = BitConverter.IsLittleEndian ? 'y' : 'n';

        string vtxName = m.name + "_" + m.GetInstanceID().ToString("X8") + ".vtx";
        outfile.WriteLine(indent + "  <Vertices Name=\"" + vtxName + "\" LittleEndian=\"" + le + "\" />");
        if (!alreadyProcessed)
        {
            SaveVector3s(m.vertices, vtxName);
        }
      
        string idxName = m.name + "_" + m.GetInstanceID().ToString("X8") + ".idx";
        outfile.WriteLine(indent + "  <Indices Name=\"" + idxName + "\" LittleEndian=\"" + le + "\" />");
        if (!alreadyProcessed)
        {
            SaveIndices(m.triangles, idxName);
        }

        if (m.normals.Length > 0)
        {
            string nmName = m.name + "_" + m.GetInstanceID().ToString("X8") + ".nm";
            outfile.WriteLine(indent + "  <Normals Name=\"" + nmName + "\" LittleEndian=\"" + le + "\" />");
            if (!alreadyProcessed)
            {
                SaveVector3s(m.normals, nmName);
            }
        }

        if (m.colors.Length > 0)
        {
            string colName = m.name + "_" + m.GetInstanceID().ToString("X8") + ".col";
            outfile.WriteLine(indent + "  <Colors Name=\"" + colName + "\" LittleEndian=\"" + le + "\" />");
            if (!alreadyProcessed)
            {
                SaveColors(m.colors, colName);
            }
        }

        if (m.uv.Length > 0)
        {
            string uv0Name = m.name + "_" + m.GetInstanceID().ToString("X8") + ".uv0";
            outfile.WriteLine(indent + "  <Uv0 Name=\"" + uv0Name + "\" LittleEndian=\"" + le + "\" />");
            if (!alreadyProcessed)
            {
                SaveVector2s(m.uv, uv0Name, new Vector4(1,1,0,0));
            }
        }

        if (m.uv2.Length > 0)
        {
            string uv1Name = m.name + "_" + m.GetInstanceID().ToString("X8") + lightmapPostfix + ".uv1";
            outfile.WriteLine(indent + "  <Uv1 Name=\"" + uv1Name + "\" LittleEndian=\"" + le + "\" />");
            if (!alreadyProcessed)
            {
                SaveVector2s(m.uv2, uv1Name, lightmapTilingOffset);
            }
        }
        
        outfile.WriteLine(indent + "</Mesh>");
    }

    private void SaveVector3s(Vector3[] vectors, string name)
    {
        FileStream fs = new FileStream(mPath + "/" + name, FileMode.Create);
        BinaryWriter w = new BinaryWriter(fs);

        for (int i = 0; i < vectors.Length; i++)
        {
            w.Write(-vectors[i].x);
            w.Write(vectors[i].y);
            w.Write(vectors[i].z);
        }

        w.Close();
        fs.Close();
    }

    private void SaveVector2s(Vector2[] vectors, string name, Vector4 lightmapTilingOffset)
    {
        FileStream fs = new FileStream(mPath + "/" + name, FileMode.Create);
        BinaryWriter w = new BinaryWriter(fs);

        Vector2 scale = new Vector2(lightmapTilingOffset.x, lightmapTilingOffset.y);
        Vector2 offset = new Vector2(lightmapTilingOffset.z, lightmapTilingOffset.w);

        for (int i = 0; i < vectors.Length; i++)
        {
            Vector2 uv = offset + new Vector2(scale.x * vectors[i].x, scale.y * vectors[i].y);
            w.Write(uv.x);
            w.Write(1.0f - uv.y); // OpenGL vs DirectX convention
        }

        w.Close();
        fs.Close();
    }

    private void SaveColors(Color[] colors, string name)
    {
        FileStream fs = new FileStream(mPath + "/" + name, FileMode.Create);
        BinaryWriter w = new BinaryWriter(fs);

        for (int i = 0; i < colors.Length; i++)
        {
            w.Write(colors[i].r);
            w.Write(colors[i].g);
            w.Write(colors[i].b);
            w.Write(colors[i].a);
        }

        w.Close();
        fs.Close();       
    }

    private void SaveIndices(int[] indices, string name)
    {
        FileStream fs = new FileStream(mPath + "/" + name, FileMode.Create);
        BinaryWriter w = new BinaryWriter(fs);

        for (int i = 0; i < indices.Length; i++)
        {
            w.Write(indices[i]);
        }

        w.Close();
        fs.Close();
    }   
}
