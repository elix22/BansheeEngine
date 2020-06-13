//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
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
    /// Container and functionality to creating and storing differences between two managed objects of the same type.
    /// </summary>
    public class SerializedDiff : ScriptObject
    {
        /// <summary>
        /// Returns true if the diff doesn't contain any changes between the two objects.
        /// </summary>
        public bool IsEmpty => Internal_IsEmpty(mCachedPtr);

        /// <summary>
        /// Creates a difference of properties that are different in <paramref name="newObj"/> compared to 
        /// <paramref name="oldObj"/>. The difference can later be applied to <paramref name="oldObj"/> to restore it to
        /// the same state as <paramref name="newObj"/> .
        /// </summary>
        /// <param name="oldObj">Object to compare from.</param>
        /// <param name="newObj">Object to compare to.</param>
        /// <returns>A set of differences between the two objects.</returns>
        public static SerializedDiff Create(object oldObj, object newObj)
        {
            if (oldObj == null || newObj == null)
                return null;

            SerializedObject serializedOldObj = SerializedObject.Create(oldObj);
            SerializedObject serializedNewObj = SerializedObject.Create(newObj);
            return Create(serializedOldObj, serializedNewObj);
        }

        /// <summary>
        /// Creates a difference of properties that are different in <paramref name="newObj"/> compared to 
        /// <paramref name="oldObj"/>. The difference can later be applied to <paramref name="oldObj"/> to restore it to
        /// the same state as <paramref name="newObj"/> .
        /// </summary>
        /// <param name="oldObj">Object to compare from.</param>
        /// <param name="newObj">Object to compare to.</param>
        /// <returns>A set of differences between the two objects.</returns>
        public static SerializedDiff Create(SerializedObject oldObj, object newObj)
        {
            if (newObj == null)
                return null;

            SerializedObject serializedNewObj = SerializedObject.Create(newObj);
            return Create(oldObj, serializedNewObj);
        }

        /// <summary>
        /// Creates a difference of properties that are different in <paramref name="newObj"/> compared to 
        /// <paramref name="oldObj"/>. The difference can later be applied to <paramref name="oldObj"/> to restore it to
        /// the same state as <paramref name="newObj"/> .
        /// </summary>
        /// <param name="oldObj">Object to compare from.</param>
        /// <param name="newObj">Object to compare to.</param>
        /// <returns>A set of differences between the two objects.</returns>
        public static SerializedDiff Create(object oldObj, SerializedObject newObj)
        {
            if (oldObj == null)
                return null;

            SerializedObject serializedOldObj = SerializedObject.Create(oldObj);
            return Create(serializedOldObj, newObj);
        }

        /// <summary>
        /// Creates a difference of properties that are different in <paramref name="newObj"/> compared to 
        /// <paramref name="oldObj"/>. The difference can later be applied to <paramref name="oldObj"/> to restore it to
        /// the same state as <paramref name="newObj"/> .
        /// </summary>
        /// <param name="oldObj">Object to compare from.</param>
        /// <param name="newObj">Object to compare to.</param>
        /// <returns>A set of differences between the two objects.</returns>
        public static SerializedDiff Create(SerializedObject oldObj, SerializedObject newObj)
        {
            if (oldObj == null || newObj == null)
                return null;

            IntPtr oldObjPtr = oldObj.GetCachedPtr();
            IntPtr newObjPtr = newObj.GetCachedPtr();
            return Internal_CreateDiff(oldObjPtr, newObjPtr);
        }

        /// <summary>
        /// Applies difference stored in this object to the provided object. The type of the object must be the same as the
        /// type of objects the difference was generated from.
        /// </summary>
        /// <param name="obj">Object to apply the difference to.</param>
        public void Apply(object obj)
        {
            if (obj == null)
                return;

            Internal_ApplyDiff(mCachedPtr, obj);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern SerializedDiff Internal_CreateDiff(IntPtr oldObj, IntPtr newObj);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_ApplyDiff(IntPtr thisPtr, object obj);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool Internal_IsEmpty(IntPtr thisPtr);
    }

    /** @} */
}
