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
    /// Container and functionality to creating a serialized version of an object. The object must be of valid serializable
    /// type (<see cref="ManagedComponent"/>, <see cref="ManagedResource"/> or a class/struct marked with 
    /// <see cref="SerializeObject"/> attribute).
    /// </summary>
    public class SerializedObject : ScriptObject
    {
        /// <summary>
        /// Constructs a new serialized object. Only for internal runtime use.
        /// </summary>
        private SerializedObject()
        { }

        /// <summary>
        /// Serializes all data within the provided object.
        /// </summary>
        /// <param name="obj">Object to serialize.</param>
        /// <returns>Object containing serialized data.</returns>
        public static SerializedObject Create(object obj)
        {
            if (obj == null)
                return null;

            return Internal_Create(obj);
        }

        /// <summary>
        /// Deserializes data stored in this object. Components and resources cannot be deserialized.
        /// </summary>
        /// <typeparam name="T">Type to cast the object to after deserialization.</typeparam>
        /// <returns>Deserialized object if successful, null otherwise.</returns>
        public T Get<T>()
        {
            return (T) Internal_Deserialize(mCachedPtr);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern SerializedObject Internal_Create(object obj);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern object Internal_Deserialize(IntPtr instance);
    }

    /** @} */
}
