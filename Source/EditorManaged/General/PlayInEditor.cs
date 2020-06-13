//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
using System;
using System.Collections.Generic;

namespace bs.Editor
{
    /** @addtogroup Editor-General
     *  @{
     */
    public partial class PlayInEditor
    {
        private static List<UUID> lastSelectedSceneObjects = new List<UUID>();

        /// <summary>Triggered right after the play mode is exited.</summary>
        public static event Action OnStopped;

        /// <summary>
        /// Updates the play state of the game, making the game stop or start running. Note the actual state change
        /// will be delayed until the next update() call. Use the onPlay/onStopped/onPaused/onUnpaused event to get notified 
        /// when the change actually happens.
        /// </summary>
        public static PlayInEditorState State
        {
            get => Internal_getState();
            set
            {
                if (value == PlayInEditorState.Paused)
                {
                    Internal_setState(value);
                    return;
                }

                bool isPlaying = value == PlayInEditorState.Playing;

                if (!isPlaying)
                    Selection.SceneObject = null;
                else
                {
                    if (EditorSettings.GetBool(LogWindow.CLEAR_ON_PLAY_KEY, true))
                    {
                        Debug.Clear();

                        LogWindow log = EditorWindow.GetWindow<LogWindow>();
                        if (log != null)
                            log.Refresh();
                    }

                    lastSelectedSceneObjects.Clear();

                    SceneObject[] sos = Selection.SceneObjects;
                    foreach (var entry in sos)
                        lastSelectedSceneObjects.Add(entry.UUID);
                }

                Internal_setState(value);
            }
        }

        static partial void Callback_OnStopped()
        {
            // Select last selected objects before rendering play mode
            if (lastSelectedSceneObjects.Count == 0)
                Selection.SceneObject = null;
            else
            {
                List<SceneObject> selection = new List<SceneObject>();

                SceneObject root = Scene.Root;
                if (root != null)
                {
                    Stack<SceneObject> todo = new Stack<SceneObject>();
                    todo.Push(root);

                    while (todo.Count > 0)
                    {
                        SceneObject so = todo.Pop();
                        if (so.IsDestroyed)
                            continue;

                        foreach (var entry in lastSelectedSceneObjects)
                        {
                            if (so.UUID == entry)
                            {
                                selection.Add(so);
                                break;
                            }
                        }

                        int numChildren = so.GetNumChildren();
                        for (int i = 0; i < numChildren; i++)
                            todo.Push(so.GetChild(i));
                    }
                }

                Selection.SceneObjects = selection.ToArray();
            }

            OnStopped?.Invoke();
        }
    }

    /** @} */
}
