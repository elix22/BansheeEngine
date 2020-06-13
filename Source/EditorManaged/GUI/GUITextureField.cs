//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using System;
using System.Runtime.CompilerServices;
using bs;

namespace bs.Editor
{
    /** @addtogroup GUI-Editor 
     *  @{
     */

    /// <summary>
    /// Editor GUI element that displays a reference to a <see cref="Texture"/> or <see cref="SpriteTexture"/> and an
    /// optional label. Textures can be dragged and dropped onto the field to update the reference. This is similar
    /// to <see cref="GUIResourceField"/> but the will display the texture contents and not only the name.
    /// </summary>
    public sealed class GUITextureField : GUIElement
    {
        public delegate void OnChangedDelegate(RRef<Resource> newValue);

        /// <summary>
        /// Triggered when the value in the field changes.
        /// </summary>
        public event OnChangedDelegate OnChanged;

        /// <summary>
        /// Resource referenced by the field, which could be either a normal or a sprite texture. This will load the
        /// resource if it is not already loaded. Use <see cref="ValueRef"/> if you don't require a loaded resource.
        /// </summary>
        public Resource Value
        {
            get
            {
                Resource value;
                Internal_GetValue(mCachedPtr, out value);
                return value;
            }

            set { Internal_SetValue(mCachedPtr, value); }
        }

        /// <summary>
        /// Handle to the resource referenced by the field, which could be either a normal or a sprite texture.
        /// </summary>
        public RRef<Resource> ValueRef
        {
            get
            {
                RRef<Resource> value;
                Internal_GetValueRef(mCachedPtr, out value);
                return value;
            }

            set { Internal_SetValueRef(mCachedPtr, value); }
        }

        /// <summary>
        /// <see cref="Texture"/> referenced by the field. This will load the resource if it is not already loaded. Use
        /// <see cref="TextureRef"/> if you don't require a loaded resource. Returns null if the field contains a
        /// <see cref="SpriteTexture"/> instead.
        /// </summary>
        public Texture Texture
        {
            get
            {
                Texture value;
                Internal_GetTexture(mCachedPtr, out value);
                return value;
            }

            set { Internal_SetTexture(mCachedPtr, value); }
        }

        /// <summary>
        /// Handle to the <see cref="Texture"/> referenced by the field. Returns null if the field contains a
        /// <see cref="SpriteTexture"/> instead.
        /// </summary>
        public RRef<Texture> TextureRef
        {
            get
            {
                RRef<Texture> value;
                Internal_GetTextureRef(mCachedPtr, out value);
                return value;
            }

            set { Internal_SetTextureRef(mCachedPtr, value); }
        }

        /// <summary>
        /// <see cref="SpriteTexture"/> referenced by the field. This will load the resource if it is not already loaded.
        /// Use <see cref="SpriteTextureRef"/> if you don't require a loaded resource. Returns null if the field contains a
        /// <see cref="Texture"/> instead.
        /// </summary>
        public SpriteTexture SpriteTexture
        {
            get
            {
                SpriteTexture value;
                Internal_GetSpriteTexture(mCachedPtr, out value);
                return value;
            }

            set { Internal_SetSpriteTexture(mCachedPtr, value); }
        }

        /// <summary>
        /// Handle to the <see cref="SpriteTexture"/> referenced by the field. Returns null if the field contains a
        /// <see cref="Texture"/> instead.
        /// </summary>
        public RRef<SpriteTexture> SpriteTextureRef
        {
            get
            {
                RRef<SpriteTexture> value;
                Internal_GetSpriteTextureRef(mCachedPtr, out value);
                return value;
            }

            set { Internal_SetSpriteTextureRef(mCachedPtr, value); }
        }

        /// <summary>
        /// Creates a new texture field element with a label. The field will accept textures but not sprite textures.
        /// </summary>
        /// <param name="title">Content to display on the label.</param>
        /// <param name="titleWidth">Width of the title label in pixels.</param>
        /// <param name="style">Optional style to use for the element. Style controls the look of the element, as well as 
        ///                     default layout options. Style will be retrieved from the active GUISkin. If not specified 
        ///                     default element style is used.</param>
        /// <param name="options">Options that allow you to control how is the element  positioned and sized. This will 
        ///                       override any similar options set by style.</param>
        public GUITextureField(GUIContent title, int titleWidth = 100, string style = "", params GUIOption[] options)
        {
            Internal_CreateInstance(this, GUITextureFieldType.Texture, ref title, titleWidth, style, options, true);
        }

        /// <summary>
        /// Creates a new texture field element without a label. The field will accept textures but not sprite textures.
        /// </summary>
        /// <param name="style">Optional style to use for the element. Style controls the look of the element, as well as 
        ///                     default layout options. Style will be retrieved from the active GUISkin. If not specified 
        ///                     default element style is used.</param>
        /// <param name="options">Options that allow you to control how is the element  positioned and sized. This will 
        ///                       override any similar options set by style.</param>
        public GUITextureField(string style = "", params GUIOption[] options)
        {
            GUIContent emptyContent = new GUIContent();
            Internal_CreateInstance(this, GUITextureFieldType.Texture, ref emptyContent, 0, style, options, false);
        }

        /// <summary>
        /// Creates a new texture field element with a label.
        /// </summary>
        /// <param name="type">Determines the type of textures should the field accept.</param>
        /// <param name="title">Content to display on the label.</param>
        /// <param name="titleWidth">Width of the title label in pixels.</param>
        /// <param name="style">Optional style to use for the element. Style controls the look of the element, as well as 
        ///                     default layout options. Style will be retrieved from the active GUISkin. If not specified 
        ///                     default element style is used.</param>
        /// <param name="options">Options that allow you to control how is the element  positioned and sized. This will 
        ///                       override any similar options set by style.</param>
        public GUITextureField(GUITextureFieldType type, GUIContent title, 
            int titleWidth = 100, string style = "", params GUIOption[] options)
        {
            Internal_CreateInstance(this, type, ref title, titleWidth, style, options, true);
        }

        /// <summary>
        /// Creates a new texture field element without a label.
        /// </summary>
        /// <param name="type">Determines the type of textures should the field accept.</param>
        /// <param name="style">Optional style to use for the element. Style controls the look of the element, as well as 
        ///                     default layout options. Style will be retrieved from the active GUISkin. If not specified 
        ///                     default element style is used.</param>
        /// <param name="options">Options that allow you to control how is the element  positioned and sized. This will 
        ///                       override any similar options set by style.</param>
        public GUITextureField(GUITextureFieldType type, string style = "", params GUIOption[] options)
        {
            GUIContent emptyContent = new GUIContent();
            Internal_CreateInstance(this, type, ref emptyContent, 0, style, options, false);
        }

        /// <summary>
        /// Colors the element with a specific tint.
        /// </summary>
        /// <param name="color">Tint to apply to the element.</param>
        public void SetTint(Color color)
        {
            Internal_SetTint(mCachedPtr, ref color);
        }

        /// <summary>
        /// Triggered by the runtime when the value of the field changes.
        /// </summary>
        /// <param name="newValue">New resource referenced by the field.</param>
        private void Internal_DoOnChanged(RRef<Resource> newValue)
        {
            if (OnChanged != null)
                OnChanged(newValue);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_CreateInstance(GUITextureField instance, GUITextureFieldType type,
            ref GUIContent title, int titleWidth, string style, GUIOption[] options, bool withTitle);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_GetSpriteTexture(IntPtr nativeInstance, out SpriteTexture value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_SetSpriteTexture(IntPtr nativeInstance, SpriteTexture value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_GetSpriteTextureRef(IntPtr nativeInstance, out RRef<SpriteTexture> value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_SetSpriteTextureRef(IntPtr nativeInstance, RRef<SpriteTexture> value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_GetTexture(IntPtr nativeInstance, out Texture value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_SetTexture(IntPtr nativeInstance, Texture value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_GetTextureRef(IntPtr nativeInstance, out RRef<Texture> value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_SetTextureRef(IntPtr nativeInstance, RRef<Texture> value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_GetValue(IntPtr nativeInstance, out Resource value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_SetValue(IntPtr nativeInstance, Resource value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_GetValueRef(IntPtr nativeInstance, out RRef<Resource> value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_SetValueRef(IntPtr nativeInstance, RRef<Resource> value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_SetTint(IntPtr nativeInstance, ref Color color);
    }

    /** @} */
}
