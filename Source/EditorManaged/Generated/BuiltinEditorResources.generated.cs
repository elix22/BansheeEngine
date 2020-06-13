//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//************** Copyright (c) 2016-2019 Marko Pintera (marko.pintera@gmail.com). All rights reserved. *******************//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using bs;

namespace bs.Editor
{
	/** @addtogroup Utility-Editor
	 *  @{
	 */

	/// <summary>Contains a set of built-in resources used by the editor.</summary>
	[ShowInInspector]
	public partial class EditorBuiltin : ScriptObject
	{
		private EditorBuiltin(bool __dummy0) { }
		protected EditorBuiltin() { }

		/// <summary>Returns the default editor GUI skin.</summary>
		[NativeWrapper]
		public static GUISkin GUISkin
		{
			get { return Internal_getSkin(); }
		}

		/// <summary>Returns the default font used by the editor.</summary>
		[NativeWrapper]
		public static Font DefaultFont
		{
			get { return Internal_getDefaultFont(); }
		}

		/// <summary>Returns the default antialiased font used by the editor.</summary>
		[NativeWrapper]
		public static Font DefaultAAFont
		{
			get { return Internal_getDefaultAAFont(); }
		}

		/// <summary>Returns text contained in the default &quot;empty&quot; shader.</summary>
		[NativeWrapper]
		public static string EmptyShaderCode
		{
			get { return Internal_getEmptyShaderCode(); }
		}

		/// <summary>Returns text contained in the default &quot;empty&quot; C# script.</summary>
		[NativeWrapper]
		public static string EmptyCSScriptCode
		{
			get { return Internal_getEmptyCSScriptCode(); }
		}

		/// <summary>
		/// Retrieves an icon that represents a specific resource type that may be displayed when viewing the project  library.
		/// </summary>
		public static SpriteTexture GetProjectLibraryIcon(ProjectLibraryIcon icon, int size)
		{
			return Internal_getProjectLibraryIcon(icon, size);
		}

		/// <summary>Retrieves an icon that may be displayed on the main window&apos;s toolbar.</summary>
		public static SpriteTexture GetToolbarIcon(ToolbarIcon icon)
		{
			return Internal_getToolbarIcon(icon);
		}

		/// <summary>Retrieves an icon that may be displayed on the scene window.</summary>
		public static GUIContentImages GetSceneWindowIcon(SceneWindowIcon icon)
		{
			GUIContentImages temp;
			Internal_getSceneWindowIcon(icon, out temp);
			return temp;
		}

		/// <summary>Retrieves an icon that may be displayed in the 3D scene view.</summary>
		public static SpriteTexture GetSceneViewIcon(SceneViewIcon icon)
		{
			return Internal_getSceneViewIcon(icon);
		}

		/// <summary>Retrieves an icon that may be displayed on the library window.</summary>
		public static SpriteTexture GetLibraryWindowIcon(LibraryWindowIcon icon)
		{
			return Internal_getLibraryWindowIcon(icon);
		}

		/// <summary>Retrieves an icon that may be displayed on the inspector window.</summary>
		public static SpriteTexture GetInspectorWindowIcon(InspectorWindowIcon icon)
		{
			return Internal_getInspectorWindowIcon(icon);
		}

		/// <summary>Retrieves an icon that may be displayed on the animation editor window.</summary>
		public static GUIContentImages GetAnimationWindowIcon(AnimationWindowIcon icon)
		{
			GUIContentImages temp;
			Internal_getAnimationWindowIcon(icon, out temp);
			return temp;
		}

		/// <summary>Retrieves an icon that represents a specific generic editor icon.</summary>
		public static SpriteTexture GetEditorIcon(EditorIcon icon)
		{
			return Internal_getEditorIcon(icon);
		}

		/// <summary>Retrieves an icon that represents a specific generic editor icon used for toggleable elements.</summary>
		public static GUIContentImages GetEditorToggleIcon(EditorToggleIcon icon)
		{
			GUIContentImages temp;
			Internal_getEditorToggleIcon(icon, out temp);
			return temp;
		}

		/// <summary>Retrieves an icon that represents a specific log message type.</summary>
		public static SpriteTexture GetLogMessageIcon(LogMessageIcon icon, int size, bool dark)
		{
			return Internal_getLogMessageIcon(icon, size, dark);
		}

		/// <summary>Retrieves a builtin editor sprite texture.</summary>
		public static SpriteTexture GetSprite(EditorSprites sprite)
		{
			return Internal_getSprite(sprite);
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern GUISkin Internal_getSkin();
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern Font Internal_getDefaultFont();
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern Font Internal_getDefaultAAFont();
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern SpriteTexture Internal_getProjectLibraryIcon(ProjectLibraryIcon icon, int size);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern SpriteTexture Internal_getToolbarIcon(ToolbarIcon icon);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_getSceneWindowIcon(SceneWindowIcon icon, out GUIContentImages __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern SpriteTexture Internal_getSceneViewIcon(SceneViewIcon icon);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern SpriteTexture Internal_getLibraryWindowIcon(LibraryWindowIcon icon);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern SpriteTexture Internal_getInspectorWindowIcon(InspectorWindowIcon icon);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_getAnimationWindowIcon(AnimationWindowIcon icon, out GUIContentImages __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern SpriteTexture Internal_getEditorIcon(EditorIcon icon);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_getEditorToggleIcon(EditorToggleIcon icon, out GUIContentImages __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern SpriteTexture Internal_getLogMessageIcon(LogMessageIcon icon, int size, bool dark);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern SpriteTexture Internal_getSprite(EditorSprites sprite);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern string Internal_getEmptyShaderCode();
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern string Internal_getEmptyCSScriptCode();
	}

	/** @} */
}
