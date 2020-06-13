﻿//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using bs;

namespace bs.Editor
{
    /** @addtogroup Utility-Editor
     *  @{
     */

    /// <summary>
    /// Provides functionality to undo or redo recently performed operations in the editor. All commands executed from this
    /// class are undoable/redoable.
    /// 
    /// The class provides static methods that access the global undo/redo stack, but can also be instantiated to provide
    /// local undo/redo stacks.
    /// </summary>
    public class UndoRedo : ScriptObject
    {
        private static UndoRedo global;

        /// <summary>
        /// Constructor for internal runtime use.
        /// </summary>
        /// <param name="dummy">Dummy parameter to distinguish from public constructor.</param>
        private UndoRedo(bool dummy)
        { }

        /// <summary>
        /// Creates a new undo/redo stack.
        /// </summary>
        public UndoRedo()
        {
            Internal_CreateInstance(this);
        }

        /// <summary>
        /// Returns the global undo/redo stack.
        /// </summary>
        public static UndoRedo Global
        {
            get { return global; }
        }

        /// <summary>
        /// Returns the unique identifier of the command currently at the top of the undo stack.
        /// </summary>
        public int TopCommandId
        {
            get { return Internal_GetTopCommandId(mCachedPtr); }
        }

        /// <summary>
        /// Executes the last command on the undo stack, undoing its operations.
        /// </summary>
        public void Undo()
        {
            Internal_Undo(mCachedPtr);
        }

        /// <summary>
        /// Executes the last command on the redo stack (last command we called undo on), re-applying its operation.
        /// </summary>
        public void Redo()
        {
            Internal_Redo(mCachedPtr);
        }

        /// <summary>
        /// Registers a new undo command.
        /// </summary>
        /// <param name="command">Command to register</param>
        public void RegisterCommand(UndoableCommand command)
        {
            if (command == null)
                return;

            Internal_RegisterCommand(mCachedPtr, command.GetCachedPtr());
        }

        /// <summary>
        /// Creates a new undo/redo group. All new commands will be registered to this group. You may remove the group and 
        /// all of its commands by calling <see cref="PopGroup"/>.
        /// </summary>
        /// <param name="name">Unique name of the group.</param>
        public void PushGroup(string name)
        {
            Internal_PushGroup(mCachedPtr, name);
        }

        /// <summary>
        /// Removes all the command registered to the current undo/redo group.
        /// </summary>
        /// <param name="name">Unique name of the group.</param>
        public void PopGroup(string name)
        {
            Internal_PopGroup(mCachedPtr, name);
        }

        /// <summary>
        /// Removes a command with the specified identifier from undo/redo stack without executing it.
        /// </summary>
        /// <param name="id">Identifier of the command as returned by <see cref="TopCommandId"/></param>
        public void PopCommand(int id)
        {
            Internal_PopCommand(mCachedPtr, id);
        }

        /// <summary>
        /// Clears all undo/redo commands from the stack.
        /// </summary>
        public void Clear()
        {
            Internal_Clear(mCachedPtr);
        }

        /// <summary>
        /// Creates new scene object(s) by cloning existing objects. Undo operation recorded in global undo/redo stack.
        /// </summary>
        /// <param name="so">Scene object(s) to clone.</param>
        /// <param name="description">Optional description of what exactly the command does.</param>
        /// <returns>Cloned scene objects.</returns>
        public static SceneObject[] CloneSO(SceneObject[] so, string description = "")
        {
            if (so != null)
            {
                List<IntPtr> soPtrs = new List<IntPtr>();
                for (int i = 0; i < so.Length; i++)
                {
                    if(so[i] != null)
                        soPtrs.Add(so[i].GetCachedPtr());
                }

                return Internal_CloneSOMulti(soPtrs.ToArray(), description);
            }

            return new SceneObject[0];
        }

        /// <summary>
        /// Creates new a scene object by cloning an existing object. Undo operation recorded in global undo/redo stack.
        /// </summary>
        /// <param name="so">Scene object to clone.</param>
        /// <param name="description">Optional description of what exactly the command does.</param>
        /// <returns>Cloned scene object.</returns>
        public static SceneObject CloneSO(SceneObject so, string description = "")
        {
            if (so != null)
                return Internal_CloneSO(so.GetCachedPtr(), description);

            return null;
        }

        /// <summary>
        /// Instantiates scene object(s) from a prefab. Undo operation recorded in global undo/redo stack.
        /// </summary>
        /// <param name="prefab">Prefab to instantiate.</param>
        /// <param name="description">Optional description of what exactly the command does.</param>
        /// <returns>Instantiated scene object.</returns>
        public static SceneObject Instantiate(Prefab prefab, string description = "")
        {
            if (prefab != null)
                return Internal_Instantiate(prefab.GetCachedPtr(), description);

            return null;
        }

        /// <summary>
        /// Creates a new scene object. Undo operation recorded in global undo/redo stack.
        /// </summary>
        /// <param name="name">Name of the scene object.</param>
        /// <param name="description">Optional description of what exactly the command does.</param>
        /// <returns>Newly created scene object.</returns>
        public static SceneObject CreateSO(string name, string description = "")
        {
            return Internal_CreateSO(name, description);
        }

        /// <summary>
        /// Creates a new scene object with a set of initial components. Undo operation recorded in global undo/redo stack.
        /// </summary>
        /// <param name="name">Name of the scene object.</param>
        /// <param name="description">Optional description of what exactly the command does.</param>
        /// <param name="componentTypes">Optional set of components that will be added to the scene object.</param>
        /// <returns>Newly created scene object.</returns>
        public static SceneObject CreateSO(string name, string description = "", params Type[] componentTypes)
        {
            return Internal_CreateSO2(name, componentTypes, description);
        }

        /// <summary>
        /// Deletes a scene object. Undo operation recorded in global undo/redo stack.
        /// </summary>
        /// <param name="so">Scene object to delete.</param>
        /// <param name="description">Optional description of what exactly the command does.</param>
        public static void DeleteSO(SceneObject so, string description = "")
        {
            if (so != null)
                Internal_DeleteSO(so.GetCachedPtr(), description);
        }

        /// <summary>
        /// Changes the parent of the scene object. Undo operation recorded in global undo/redo stack.
        /// </summary>
        /// <param name="so">Scene object to change the parent of.</param>
        /// <param name="parent">New parent.</param>
        /// <param name="description">Optional description of what exactly the command does.</param>
        public static void ReparentSO(SceneObject so, SceneObject parent, string description = "")
        {
            if (so != null)
            {
                IntPtr parentPtr = IntPtr.Zero;
                if (parent != null)
                    parentPtr = parent.GetCachedPtr();

                Internal_ReparentSO(so.GetCachedPtr(), parentPtr, description);
            }
        }

        /// <summary>
        /// Changes the parent of a set of scene objects. Undo operation recorded in global undo/redo stack.
        /// </summary>
        /// <param name="so">Scene objects to change the parent of.</param>
        /// <param name="parent">New parent.</param>
        /// <param name="description">Optional description of what exactly the command does.</param>
        public static void ReparentSO(SceneObject[] so, SceneObject parent, string description = "")
        {
            if (so != null)
            {
                List<IntPtr> soPtrs = new List<IntPtr>();
                for (int i = 0; i < so.Length; i++)
                {
                    if (so[i] != null)
                        soPtrs.Add(so[i].GetCachedPtr());
                }

                if (soPtrs.Count > 0)
                {
                    IntPtr parentPtr = IntPtr.Zero;
                    if (parent != null)
                        parentPtr = parent.GetCachedPtr();

                    Internal_ReparentSOMulti(soPtrs.ToArray(), parentPtr, description);
                }
            }
        }

        /// <summary>
        /// Breaks the prefab link on the provided scene object and makes the operation undo-able. Undo operation recorded 
        /// in global undo/redo stack.
        /// See <see cref="PrefabUtility.BreakPrefab"/>.
        /// </summary>
        /// <param name="so">Scene object whose prefab link to break.</param>
        /// <param name="description">Optional description of what exactly the command does.</param>
        public static void BreakPrefab(SceneObject so, string description = "")
        {
            if (so != null)
                Internal_BreakPrefab(so.GetCachedPtr(), description);
        }

        /// <summary>
        /// Used by the runtime to set the global undo/redo stack.
        /// </summary>
        /// <param name="global">Instance of the global undo/redo stack.</param>
        private static void Internal_SetGlobal(UndoRedo global)
        {
            // We can't set this directly through the field because there is an issue with Mono and static fields
            UndoRedo.global = global;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Internal_CreateInstance(UndoRedo instance);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Internal_Undo(IntPtr thisPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Internal_Redo(IntPtr thisPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Internal_RegisterCommand(IntPtr thisPtr, IntPtr commandPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Internal_PushGroup(IntPtr thisPtr, string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Internal_PopGroup(IntPtr thisPtr, string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Internal_Clear(IntPtr thisPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Internal_PopCommand(IntPtr thisPtr, int id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern int Internal_GetTopCommandId(IntPtr thisPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern SceneObject Internal_CloneSO(IntPtr soPtr, string description);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern SceneObject[] Internal_CloneSOMulti(IntPtr[] soPtr, string description);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern SceneObject Internal_Instantiate(IntPtr prefabPtr, string description);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern SceneObject Internal_CreateSO(string name, string description);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern SceneObject Internal_CreateSO2(string name, Type[] componentTypes, string description);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Internal_DeleteSO(IntPtr soPtr, string description);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Internal_ReparentSO(IntPtr soPtr, IntPtr parentSOPtr, string description);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Internal_ReparentSOMulti(IntPtr[] soPtr, IntPtr parentSOPtr, string description);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Internal_BreakPrefab(IntPtr soPtr, string description);
    }

    /** @} */
}
