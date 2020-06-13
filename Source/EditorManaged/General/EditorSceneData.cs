//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using System.Collections.Generic;

namespace bs.Editor
{
    /** @addtogroup Editor-General 
     *  @{
     */

    /// <summary>
    /// Contains per-scene data relevant to the editor.
    /// </summary>
    [SerializeObject]
    internal class EditorSceneData
    {
        /// <summary>
        /// Creates new editor scene data.
        /// </summary>
        /// <param name="root">Root object of the scene to create editor scene data for.</param>
        /// <returns>New editor scene data object, initialized with the hierarchy of the provided scene.</returns>
        public static EditorSceneData FromScene(SceneObject root)
        {
            EditorSceneData output = new EditorSceneData();
            output.Root = EditorSceneObject.FromSceneObject(root);

            return output;
        }

        /// <summary>
        /// Updates the editor scene object hierarchy from the current scene hierarchy.
        /// </summary>
        /// <param name="root">Root of the hierarchy to update from.</param>
        public void UpdateFromScene(SceneObject root)
        {
            Dictionary<UUID, EditorSceneObject> lookup = GetLookup();
            Root = EditorSceneObject.FromSceneObject(root);

            void UpdateFromOldData(EditorSceneObject so)
            {
                if (lookup.TryGetValue(so.UUID, out EditorSceneObject data))
                    so.CopyData(data);

                foreach(var entry in so.Children)
                    UpdateFromOldData(entry);
            }

            UpdateFromOldData(Root);
        }

        /// <summary>
        /// Builds a lookup table of scene object UUID -> editor scene object data, for all scene objects.
        /// </summary>
        /// <returns>Lookup table.</returns>
        public Dictionary<UUID, EditorSceneObject> GetLookup()
        {
            Dictionary<UUID, EditorSceneObject> lookup = new Dictionary<UUID, EditorSceneObject>();

            void AddToLookup(EditorSceneObject so)
            {
                lookup[so.UUID] = so;

                foreach(var entry in so.Children)
                    AddToLookup(entry);
            }

            if(Root != null)
                AddToLookup(Root);

            return lookup;
        }

        /// <summary>
        /// Root object of the scene hierarchy.
        /// </summary>
        public EditorSceneObject Root;
    }

    /// <summary>
    /// Contains editor-relevant data about a scene object.
    /// </summary>
    [SerializeObject]
    internal class EditorSceneObject
    {
        /// <summary>
        /// Unique identifier of the scene object.
        /// </summary>
        public UUID UUID;

        /// <summary>
        /// True if the object is expanded in the hierarchy view.
        /// </summary>
        public bool IsExpanded;

        /// <summary>
        /// Child scene objects, if any.
        /// </summary>
        public EditorSceneObject[] Children;

        /// <summary>
        /// Initializes the object from a corresponding scene object.
        /// </summary>
        /// <param name="so">Scene object that this object contains additional data for.</param>
        /// <returns>New editor scene object.</returns>
        public static EditorSceneObject FromSceneObject(SceneObject so)
        {
            EditorSceneObject output = new EditorSceneObject();
            output.UUID = so.UUID;

            int numChildren = so.GetNumChildren();
            output.Children = new EditorSceneObject[numChildren];

            for (int i = 0; i < numChildren; i++)
                output.Children[i] = FromSceneObject(so.GetChild(i));

            return output;
        }

        /// <summary>
        /// Copies the stored data from one object instance to another. Does not copy object UUID or child list.
        /// </summary>
        /// <param name="other">Object from which to copy the data.</param>
        public void CopyData(EditorSceneObject other)
        {
            IsExpanded = other.IsExpanded;
        }
    }

    /** @} */
}
