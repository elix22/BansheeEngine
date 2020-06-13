//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using System;
using System.Collections.Generic;
using System.Text;
using bs;

namespace bs.Editor
{
    /** @addtogroup AnimationEditor
     *  @{
     */


    /// <summary>
    /// A set of animation curves for a field of a certain type.
    /// </summary>
    internal struct FieldAnimCurves
    {
        public SerializableProperty.FieldType type;
        public EdCurveDrawInfo[] curveInfos;
        public bool isPropertyCurve;
    }

    /// <summary>
    /// Stores tangent modes for an 3D vector animation curve (one mode for each keyframe).
    /// </summary>
    [SerializeObject]
    internal class EditorVector3CurveTangents
    {
        public string name;
        public TangentMode[] tangentsX;
        public TangentMode[] tangentsY;
        public TangentMode[] tangentsZ;
    }

    /// <summary>
    /// Stores tangent modes for an float animation curve (one mode for each keyframe).
    /// </summary>
    [SerializeObject]
    internal class EditorFloatCurveTangents
    {
        public string name;
        public TangentMode[] tangents;
    }

    /// <summary>
    /// Stores tangent information for all curves in an animation clip.
    /// </summary>
    [SerializeObject]
    internal class EditorAnimClipTangents
    {
        public EditorVector3CurveTangents[] positionCurves;
        public EditorVector3CurveTangents[] rotationCurves;
        public EditorVector3CurveTangents[] scaleCurves;
        public EditorFloatCurveTangents[] floatCurves;
    }

    /// <summary>
    /// Contains a version of <see cref="NamedVector3Curve"/> that can be serialized from the editor.
    /// </summary>
    [SerializeObject]
    internal struct EditorNamedVector3Curve
    {
        /// <summary>Name of the curve.</summary>
        public string name;

        /// <summary>Flags that describe the animation curve.</summary>
        public AnimationCurveFlags flags;

        /// <summary> Curve keyframes. </summary>
        public KeyFrameVec3[] keyFrames;
    }

    /// <summary>
    /// Contains editor-only data for a specific <see cref="AnimationClip"/>.
    /// </summary>
    [SerializeObject]
    internal class EditorAnimClipData
    {
        public EditorNamedVector3Curve[] eulerCurves;
        public EditorAnimClipTangents tangents;
    }

    /// <summary>
    /// Stores animation clip data for clips that are currently being edited.
    /// </summary>
    internal class EditorAnimClipInfo
    {
        public AnimationClip clip;
        public bool isImported;
        public int sampleRate;
        public Dictionary<string, FieldAnimCurves> curves = new Dictionary<string, FieldAnimCurves>();
        public AnimationEvent[] events = new AnimationEvent[0];

        /// <summary>
        /// Loads curve and event information from the provided clip, and creates a new instance of this object containing
        /// the required data for editing the source clip in the animation editor.
        /// </summary>
        /// <param name="clip">Clip to load.</param>
        /// <returns>Editor specific editable information about an animation clip.</returns>
        public static EditorAnimClipInfo Create(AnimationClip clip)
        {
            EditorAnimClipInfo clipInfo = new EditorAnimClipInfo();
            clipInfo.clip = clip;
            clipInfo.isImported = IsClipImported(clip);
            clipInfo.sampleRate = (int)clip.SampleRate;

            AnimationCurves clipCurves = clip.Curves;
            EditorAnimClipData editorClipData = null;

            EditorAnimClipData lGetAnimClipData(ResourceMeta meta)
            {
                object editorData = meta.EditorData;
                EditorAnimClipData output = editorData as EditorAnimClipData;

                if (output == null)
                {
                    // Old editor data stores tangents only
                    if (editorData is EditorAnimClipTangents tangents)
                    {
                        output = new EditorAnimClipData();
                        output.tangents = tangents;
                    }
                }

                return output;
            }

            string resourcePath = ProjectLibrary.GetPath(clip);
            if (!string.IsNullOrEmpty(resourcePath))
            {
                LibraryEntry entry = ProjectLibrary.GetEntry(resourcePath);
                string clipName = PathEx.GetTail(resourcePath);

                if (entry != null && entry.Type == LibraryEntryType.File)
                {
                    FileEntry fileEntry = (FileEntry)entry;
                    ResourceMeta[] metas = fileEntry.ResourceMetas;

                    if (clipInfo.isImported)
                    {
                        for (int i = 0; i < metas.Length; i++)
                        {
                            if (clipName == metas[i].SubresourceName)
                            {
                                editorClipData = lGetAnimClipData(metas[i]);
                                break;
                            }
                        }
                    }
                    else
                    {
                        if (metas.Length > 0)
                            editorClipData = lGetAnimClipData(metas[0]);
                    }
                }
            }

            if (editorClipData == null)
            {
                editorClipData = new EditorAnimClipData();
                editorClipData.tangents = new EditorAnimClipTangents();
            }

            int globalCurveIdx = 0;
            void lLoadVector3Curve(NamedVector3Curve[] curves, EditorVector3CurveTangents[] tangents, string subPath)
            {
                foreach (var curveEntry in curves)
                {
                    TangentMode[] tangentsX = null;
                    TangentMode[] tangentsY = null;
                    TangentMode[] tangentsZ = null;

                    if (tangents != null)
                    {
                        foreach (var tangentEntry in tangents)
                        {
                            if (tangentEntry.name == curveEntry.name)
                            {
                                tangentsX = tangentEntry.tangentsX;
                                tangentsY = tangentEntry.tangentsY;
                                tangentsZ = tangentEntry.tangentsZ;
                                break;
                            }
                        }
                    }

                    // Convert compound curve to three per-component curves
                    AnimationCurve[] componentCurves = AnimationUtility.SplitCurve3D(curveEntry.curve);

                    FieldAnimCurves fieldCurves = new FieldAnimCurves();
                    fieldCurves.type = SerializableProperty.FieldType.Vector3;
                    fieldCurves.curveInfos = new EdCurveDrawInfo[3];
                    fieldCurves.isPropertyCurve = !clipInfo.isImported;

                    fieldCurves.curveInfos[0] = new EdCurveDrawInfo();
                    fieldCurves.curveInfos[0].curve = new EdAnimationCurve(componentCurves[0], tangentsX);
                    fieldCurves.curveInfos[0].color = GetUniqueColor(globalCurveIdx++);

                    fieldCurves.curveInfos[1] = new EdCurveDrawInfo();
                    fieldCurves.curveInfos[1].curve = new EdAnimationCurve(componentCurves[1], tangentsY);
                    fieldCurves.curveInfos[1].color = GetUniqueColor(globalCurveIdx++);

                    fieldCurves.curveInfos[2] = new EdCurveDrawInfo();
                    fieldCurves.curveInfos[2].curve = new EdAnimationCurve(componentCurves[2], tangentsZ);
                    fieldCurves.curveInfos[2].color = GetUniqueColor(globalCurveIdx++);

                    string curvePath = curveEntry.name.TrimEnd('/') + subPath;
                    clipInfo.curves[curvePath] = fieldCurves;
                }
            };

            NamedQuaternionCurve[] rotationCurves = clipCurves.Rotation;
            NamedVector3Curve[] eulerRotationCurves = new NamedVector3Curve[rotationCurves.Length];
            if (editorClipData.eulerCurves == null || editorClipData.eulerCurves.Length != rotationCurves.Length)
            {
                // Convert rotation from quaternion to euler if we don't have original euler animation data stored. 
                for (int i = 0; i < rotationCurves.Length; i++)
                {
                    NamedQuaternionCurve quatCurve = rotationCurves[i];
                    Vector3Curve eulerCurve = AnimationUtility.QuaternionToEulerCurve(quatCurve.curve);

                    eulerRotationCurves[i] = new NamedVector3Curve(quatCurve.name, quatCurve.flags, eulerCurve);
                }
            }
            else
            {
                for (int i = 0; i < editorClipData.eulerCurves.Length; i++)
                {
                    EditorNamedVector3Curve edCurve = editorClipData.eulerCurves[i];

                    eulerRotationCurves[i] = new NamedVector3Curve(
                        edCurve.name, edCurve.flags, new Vector3Curve(edCurve.keyFrames));
                }
            }

            lLoadVector3Curve(clipCurves.Position, editorClipData.tangents.positionCurves, "/Position");
            lLoadVector3Curve(eulerRotationCurves, editorClipData.tangents.rotationCurves, "/Rotation");
            lLoadVector3Curve(clipCurves.Scale, editorClipData.tangents.scaleCurves, "/Scale");

            // Find which individual float curves belong to the same field
            Dictionary<string, Tuple<int, int, bool>[]> floatCurveMapping = new Dictionary<string, Tuple<int, int, bool>[]>();
            {
                int curveIdx = 0;
                foreach (var curveEntry in clipCurves.Generic)
                {
                    string path = curveEntry.name;
                    string pathNoSuffix = null;

                    string pathSuffix;
                    if (path.Length >= 2)
                    {
                        pathSuffix = path.Substring(path.Length - 2, 2);
                        pathNoSuffix = path.Substring(0, path.Length - 2);
                    }
                    else
                        pathSuffix = "";

                    int tangentIdx = -1;
                    int currentTangentIdx = 0;
                    foreach (var tangentEntry in editorClipData.tangents.floatCurves)
                    {
                        if (tangentEntry.name == curveEntry.name)
                        {
                            tangentIdx = currentTangentIdx;
                            break;
                        }

                        currentTangentIdx++;
                    }

                    Animation.PropertySuffixInfo suffixInfo;
                    if (Animation.PropertySuffixInfos.TryGetValue(pathSuffix, out suffixInfo))
                    {
                        Tuple<int, int, bool>[] curveInfo;
                        if (!floatCurveMapping.TryGetValue(pathNoSuffix, out curveInfo))
                            curveInfo = new Tuple<int, int, bool>[4];

                        curveInfo[suffixInfo.elementIdx] = Tuple.Create(curveIdx, tangentIdx, suffixInfo.isVector);
                        floatCurveMapping[pathNoSuffix] = curveInfo;
                    }
                    else
                    {
                        Tuple<int, int, bool>[] curveInfo = new Tuple<int, int, bool>[4];
                        curveInfo[0] = Tuple.Create(curveIdx, tangentIdx, suffixInfo.isVector);

                        floatCurveMapping[path] = curveInfo;
                    }

                    curveIdx++;
                }
            }

            foreach (var KVP in floatCurveMapping)
            {
                int numCurves = 0;
                for (int i = 0; i < 4; i++)
                {
                    if (KVP.Value[i] == null)
                        continue;

                    numCurves++;
                }

                if (numCurves == 0)
                    continue; // Invalid curve

                FieldAnimCurves fieldCurves = new FieldAnimCurves();

                // Deduce type (note that all single value types are assumed to be float even if their source type is int or bool)
                if (numCurves == 1)
                    fieldCurves.type = SerializableProperty.FieldType.Float;
                else if (numCurves == 2)
                    fieldCurves.type = SerializableProperty.FieldType.Vector2;
                else if (numCurves == 3)
                    fieldCurves.type = SerializableProperty.FieldType.Vector3;
                else // 4 curves
                {
                    bool isVector = KVP.Value[0].Item3;
                    if (isVector)
                        fieldCurves.type = SerializableProperty.FieldType.Vector4;
                    else
                        fieldCurves.type = SerializableProperty.FieldType.Color;
                }

                bool isMorphCurve = false;
                string curvePath = KVP.Key;

                fieldCurves.curveInfos = new EdCurveDrawInfo[numCurves];
                for (int i = 0; i < numCurves; i++)
                {
                    int curveIdx = KVP.Value[i].Item1;
                    int tangentIdx = KVP.Value[i].Item2;

                    TangentMode[] tangents = null;
                    if (tangentIdx != -1)
                        tangents = editorClipData.tangents.floatCurves[tangentIdx].tangents;

                    fieldCurves.curveInfos[i] = new EdCurveDrawInfo();
                    fieldCurves.curveInfos[i].curve = new EdAnimationCurve(clipCurves.Generic[curveIdx].curve, tangents);
                    fieldCurves.curveInfos[i].color = GetUniqueColor(globalCurveIdx++);

                    if (clipCurves.Generic[curveIdx].flags.HasFlag(AnimationCurveFlags.MorphFrame))
                    {
                        curvePath = "MorphShapes/Frames/" + KVP.Key;
                        isMorphCurve = true;
                    }
                    else if (clipCurves.Generic[curveIdx].flags.HasFlag(AnimationCurveFlags.MorphWeight))
                    {
                        curvePath = "MorphShapes/Weight/" + KVP.Key;
                        isMorphCurve = true;
                    }
                }

                fieldCurves.isPropertyCurve = !clipInfo.isImported && !isMorphCurve;

                clipInfo.curves[curvePath] = fieldCurves;
            }

            // Add events
            clipInfo.events = clip.Events;
            return clipInfo;
        }

        /// <summary>
        /// Checks is the specified animation clip is imported from an external file, or created within the editor.
        /// </summary>
        /// <param name="clip">Clip to check.</param>
        /// <returns>True if the clip is imported from an external file (e.g. FBX file), or false if the clip is a native
        ///          resource created within the editor.</returns>
        public static bool IsClipImported(AnimationClip clip)
        {
            string resourcePath = ProjectLibrary.GetPath(clip);
            return ProjectLibrary.IsSubresource(resourcePath);
        }

        /// <summary>
        /// Checks does a curve with the specified path represent a curve affecting a morph shape.
        /// </summary>
        /// <param name="path">Path of the curve to check.</param>
        /// <returns>True if morph shape frame or weight animation, false otherwise.</returns>
        public static bool IsMorphShapeCurve(string path)
        {
            if (string.IsNullOrEmpty(path))
                return false;

            string trimmedPath = path.Trim('/');
            string[] entries = trimmedPath.Split('/');

            if (entries.Length < 3)
                return false;

            if (entries[entries.Length - 3] != "MorphShapes")
                return false;

            return entries[entries.Length - 2] == "Weight" || entries[entries.Length - 2] == "Frames";
        }

        /// <summary>
        /// Applies any changes made to the animation curves or events to the actual animation clip. Only works for
        /// non-imported animation clips.
        /// </summary>
        /// <param name="editorData">Additional animation clip data for use in editor.</param>
        public void Apply(out EditorAnimClipData editorData)
        {
            if (isImported || clip == null)
            {
                editorData = null;
                return;
            }

            List<NamedVector3Curve> positionCurves = new List<NamedVector3Curve>();
            List<NamedQuaternionCurve> rotationCurves = new List<NamedQuaternionCurve>();
            List<NamedVector3Curve> scaleCurves = new List<NamedVector3Curve>();
            List<NamedFloatCurve> floatCurves = new List<NamedFloatCurve>();

            List<EditorVector3CurveTangents> positionTangents = new List<EditorVector3CurveTangents>();
            List<EditorVector3CurveTangents> rotationTangents = new List<EditorVector3CurveTangents>();
            List<EditorVector3CurveTangents> scaleTangents = new List<EditorVector3CurveTangents>();
            List<EditorFloatCurveTangents> floatTangents = new List<EditorFloatCurveTangents>();
            List<EditorNamedVector3Curve> eulerRotationCurves = new List<EditorNamedVector3Curve>();

            foreach (var kvp in curves)
            {
                string[] pathEntries = kvp.Key.Split('/');
                if (pathEntries.Length == 0)
                    continue;

                string lastEntry = pathEntries[pathEntries.Length - 1];

                if (lastEntry == "Position" || lastEntry == "Rotation" || lastEntry == "Scale")
                {
                    StringBuilder sb = new StringBuilder();
                    for (int i = 0; i < pathEntries.Length - 2; i++)
                        sb.Append(pathEntries[i] + "/");

                    if (pathEntries.Length > 1)
                        sb.Append(pathEntries[pathEntries.Length - 2]);

                    string curvePath = sb.ToString();

                    NamedVector3Curve curve = new NamedVector3Curve();
                    curve.name = curvePath;
                    curve.curve = AnimationUtility.CombineCurve3D(new[]
                    {
                        new AnimationCurve(kvp.Value.curveInfos[0].curve.KeyFrames),
                        new AnimationCurve(kvp.Value.curveInfos[1].curve.KeyFrames),
                        new AnimationCurve(kvp.Value.curveInfos[2].curve.KeyFrames)
                    });

                    EditorVector3CurveTangents curveTangents = new EditorVector3CurveTangents();
                    curveTangents.name = curvePath;
                    curveTangents.tangentsX = kvp.Value.curveInfos[0].curve.TangentModes;
                    curveTangents.tangentsY = kvp.Value.curveInfos[1].curve.TangentModes;
                    curveTangents.tangentsZ = kvp.Value.curveInfos[2].curve.TangentModes;

                    if (lastEntry == "Position")
                    {
                        positionCurves.Add(curve);
                        positionTangents.Add(curveTangents);
                    }
                    else if (lastEntry == "Rotation")
                    {
                        NamedQuaternionCurve quatCurve = new NamedQuaternionCurve();
                        quatCurve.name = curve.name;
                        quatCurve.curve = AnimationUtility.EulerToQuaternionCurve(curve.curve);

                        EditorNamedVector3Curve edEulerCurve = new EditorNamedVector3Curve();
                        edEulerCurve.name = curve.name;
                        edEulerCurve.keyFrames = curve.curve.KeyFrames;

                        rotationCurves.Add(quatCurve);
                        eulerRotationCurves.Add(edEulerCurve);
                        rotationTangents.Add(curveTangents);
                    }
                    else if (lastEntry == "Scale")
                    {
                        scaleCurves.Add(curve);
                        scaleTangents.Add(curveTangents);
                    }
                }
                else
                {
                    Action<int, string, string, AnimationCurveFlags> addCurve = (idx, path, subPath, flags) =>
                    {
                        string fullPath = path + subPath;

                        NamedFloatCurve curve = new NamedFloatCurve(fullPath,
                            new AnimationCurve(kvp.Value.curveInfos[idx].curve.KeyFrames));
                        curve.flags = flags;

                        EditorFloatCurveTangents curveTangents = new EditorFloatCurveTangents();
                        curveTangents.name = fullPath;
                        curveTangents.tangents = kvp.Value.curveInfos[idx].curve.TangentModes;

                        floatCurves.Add(curve);
                        floatTangents.Add(curveTangents);
                    };

                    switch (kvp.Value.type)
                    {
                        case SerializableProperty.FieldType.Vector2:
                            addCurve(0, kvp.Key, ".x", 0);
                            addCurve(1, kvp.Key, ".y", 0);
                            break;
                        case SerializableProperty.FieldType.Vector3:
                            addCurve(0, kvp.Key, ".x", 0);
                            addCurve(1, kvp.Key, ".y", 0);
                            addCurve(2, kvp.Key, ".z", 0);
                            break;
                        case SerializableProperty.FieldType.Vector4:
                            addCurve(0, kvp.Key, ".x", 0);
                            addCurve(1, kvp.Key, ".y", 0);
                            addCurve(2, kvp.Key, ".z", 0);
                            addCurve(3, kvp.Key, ".w", 0);
                            break;
                        case SerializableProperty.FieldType.Color:
                            addCurve(0, kvp.Key, ".r", 0);
                            addCurve(1, kvp.Key, ".g", 0);
                            addCurve(2, kvp.Key, ".b", 0);
                            addCurve(3, kvp.Key, ".a", 0);
                            break;
                        case SerializableProperty.FieldType.Bool:
                        case SerializableProperty.FieldType.Int:
                        case SerializableProperty.FieldType.Float:
                        {
                            AnimationCurveFlags flags = 0;
                            string path = kvp.Key;

                            if (IsMorphShapeCurve(kvp.Key))
                            {
                                string trimmedPath = path.Trim('/');
                                string[] entries = trimmedPath.Split('/');

                                bool isWeight = entries[entries.Length - 2] == "Weight";

                                if (isWeight)
                                    flags = AnimationCurveFlags.MorphWeight;
                                else
                                    flags = AnimationCurveFlags.MorphFrame;

                                path = entries[entries.Length - 1];
                            }

                            addCurve(0, path, "", flags);
                        }
                            break;
                    }
                }
            }

            AnimationCurves newClipCurves = new AnimationCurves();
            newClipCurves.Position = positionCurves.ToArray();
            newClipCurves.Rotation = rotationCurves.ToArray();
            newClipCurves.Scale = scaleCurves.ToArray();
            newClipCurves.Generic = floatCurves.ToArray();

            clip.Curves = newClipCurves;
            clip.Events = events;
            clip.SampleRate = sampleRate;

            editorData = new EditorAnimClipData();
            editorData.tangents = new EditorAnimClipTangents();
            editorData.tangents.positionCurves = positionTangents.ToArray();
            editorData.tangents.rotationCurves = rotationTangents.ToArray();
            editorData.tangents.scaleCurves = scaleTangents.ToArray();
            editorData.tangents.floatCurves = floatTangents.ToArray();
            editorData.eulerCurves = eulerRotationCurves.ToArray();
        }

        /// <summary>
        /// Saves the animation curves and events stored in this object, into the associated animation clip resource.
        /// Relevant animation clip resource must already be created and exist in the project library.
        /// </summary>
        public void SaveToClip()
        {
            if (!isImported)
            {
                EditorAnimClipData editorAnimClipData;
                Apply(out editorAnimClipData);

                string resourcePath = ProjectLibrary.GetPath(clip);
                ProjectLibrary.Save(clip);

                ProjectLibrary.SetEditorData(resourcePath, editorAnimClipData);
            }
            else
            {
                string resourcePath = ProjectLibrary.GetPath(clip);
                LibraryEntry entry = ProjectLibrary.GetEntry(resourcePath);

                if (entry != null && entry.Type == LibraryEntryType.File)
                {
                    FileEntry fileEntry = (FileEntry)entry;
                    MeshImportOptions meshImportOptions = (MeshImportOptions)fileEntry.Options;

                    string clipName = PathEx.GetTail(resourcePath);

                    List<ImportedAnimationEvents> newEvents = new List<ImportedAnimationEvents>();
                    newEvents.AddRange(meshImportOptions.AnimationEvents);

                    bool isExisting = false;
                    for (int i = 0; i < newEvents.Count; i++)
                    {
                        if (newEvents[i].Name == clipName)
                        {
                            newEvents[i].Events = events;
                            isExisting = true;
                            break;
                        }
                    }

                    if (!isExisting)
                    {
                        ImportedAnimationEvents newEntry = new ImportedAnimationEvents();
                        newEntry.Name = clipName;
                        newEntry.Events = events;

                        newEvents.Add(newEntry);
                    }

                    meshImportOptions.AnimationEvents = newEvents.ToArray();

                    ProjectLibrary.Reimport(resourcePath, meshImportOptions, true);
                }
            }
        }

        /// <summary>
        /// Generates a unique color based on the provided index.
        /// </summary>
        /// <param name="idx">Index to use for generating a color. Should be less than 30 in order to guarantee reasonably
        /// different colors.</param>
        /// <returns>Unique color.</returns>
        public static Color GetUniqueColor(int idx)
        {
            const int COLOR_SPACING = 359 / 15;

            float hue = ((idx * COLOR_SPACING) % 359) / 359.0f;
            return Color.HSV2RGB(new Color(hue, 175.0f / 255.0f, 175.0f / 255.0f));
        }

    }

    /** @} */
}
          