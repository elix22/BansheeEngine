﻿//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using System;
using System.Runtime.CompilerServices;
using bs;

namespace bs.Editor
{
    /** @addtogroup Utility-Editor
     *  @{
     */

    /// <summary>
    /// Performs various prefab specific operations.
    /// </summary>
    public static class PrefabUtility
    {
        /// <summary>
        /// Breaks the link between a prefab instance and its prefab. Object will retain all current values but will
        /// no longer be influenced by modifications to its parent prefab.
        /// </summary>
        /// <param name="obj">Prefab instance whose link to break.</param>
        public static void BreakPrefab(SceneObject obj)
        {
            if (obj == null)
                return;

            IntPtr objPtr = obj.GetCachedPtr();
            Internal_BreakPrefab(objPtr);
        }

        /// <summary>
        /// Updates the contents of the prefab with the contents of the provided prefab instance. If the provided object
        /// is not a prefab instance nothing happens.
        /// </summary>
        /// <param name="obj">Prefab instance whose prefab to update.</param>
        /// <param name="refreshScene">If true, all prefab instances in the current scene will be updated so they consistent
        ///                            with the newly saved data.</param>
        public static void ApplyPrefab(SceneObject obj, bool refreshScene = true)
        {
            if (obj == null)
                return;

            SceneObject prefabInstanceRoot = GetPrefabParent(obj);
            if (prefabInstanceRoot == null)
                return;

            if (refreshScene)
            {
                SceneObject root = Scene.Root;
                if (root != null)
                    Internal_RecordPrefabDiff(root.GetCachedPtr());
            }

            UUID prefabUUID = GetPrefabUUID(prefabInstanceRoot);
            string prefabPath = ProjectLibrary.GetPath(prefabUUID);
            Prefab prefab = ProjectLibrary.Load<Prefab>(prefabPath);
            if (prefab != null)
            {
                IntPtr soPtr = prefabInstanceRoot.GetCachedPtr();
                IntPtr prefabPtr = prefab.GetCachedPtr();

                Internal_ApplyPrefab(soPtr, prefabPtr);
                ProjectLibrary.Save(prefab);
            }

            if (refreshScene)
            {
                SceneObject root = Scene.Root;
                if (root != null)
                    Internal_UpdateFromPrefab(root.GetCachedPtr());
            }
        }

        /// <summary>
        /// Remove any instance specific changes to the object or its hierarchy from the provided prefab instance and 
        /// restore it to the exact copy of the linked prefab.
        /// </summary>
        /// <param name="obj">Prefab instance to revert to original state.</param>
        public static void RevertPrefab(SceneObject obj)
        {
            if (obj == null)
                return;

            IntPtr objPtr = obj.GetCachedPtr();
            Internal_RevertPrefab(objPtr);
        }

        /// <summary>
        /// Updates all of the objects belonging to the same prefab instance as the provided object (if any). The update
        /// will apply any changes from the linked prefab to the hierarchy(if any).
        /// </summary>
        /// <param name="obj"></param>
        public static void UpdateFromPrefab(SceneObject obj)
        {
            if (obj == null)
                return;

            IntPtr objPtr = obj.GetCachedPtr();
            Internal_UpdateFromPrefab(objPtr);
        }

        /// <summary>
        /// Checks if a scene object has a prefab link. Scene objects with a prefab link will be automatically updated
        /// when their prefab changes in order to reflect its changes.
        /// </summary>
        /// <param name="obj">Scene object to check if it has a prefab link.</param>
        /// <returns>True if the object is a prefab instance (has a prefab link), false otherwise.</returns>
        public static bool IsPrefabInstance(SceneObject obj)
        {
            if (obj == null)
                return false;

            IntPtr objPtr = obj.GetCachedPtr();
            return Internal_HasPrefabLink(objPtr);
        }

        /// <summary>
        /// Returns the root object of the prefab instance that this object belongs to, if any. 
        /// </summary>
        /// <param name="obj">Scene object to retrieve the prefab parent for.</param>
        /// <returns>Prefab parent of the provided object, or null if the object is not part of a prefab instance.</returns>
        public static SceneObject GetPrefabParent(SceneObject obj)
        {
            if (obj == null)
                return null;

            IntPtr objPtr = obj.GetCachedPtr();
            return Internal_GetPrefabParent(objPtr);
        }

        /// <summary>
        /// Returns the UUID of the prefab attached to the provided scene object. Only works on root prefab objects.
        /// </summary>
        /// <param name="obj">Scene object to retrieve the prefab UUID for.</param>
        /// <returns>Prefab UUID if the object is part of a prefab, null otherwise. </returns>
        public static UUID GetPrefabUUID(SceneObject obj)
        {
            if (obj == null)
                return UUID.Empty;

            IntPtr objPtr = obj.GetCachedPtr();

            UUID uuid;
            Internal_GetPrefabUUID(objPtr, out uuid);

            return uuid;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_BreakPrefab(IntPtr soPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_ApplyPrefab(IntPtr soPtr, IntPtr prefabPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_RecordPrefabDiff(IntPtr soPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_RevertPrefab(IntPtr soPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool Internal_HasPrefabLink(IntPtr soPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_UpdateFromPrefab(IntPtr soPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern SceneObject Internal_GetPrefabParent(IntPtr soPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_GetPrefabUUID(IntPtr soPtr, out UUID uuid);
    }

    /** @} */
}
