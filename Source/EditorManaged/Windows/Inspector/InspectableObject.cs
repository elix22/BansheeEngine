//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using System.Collections.Generic;
using System;
using System.Reflection;
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspector
     *  @{
     */

    /// <summary>
    /// Displays GUI for a serializable property containing a generic object. Inspectable object fields are displayed
    /// in separate rows.
    /// </summary>
    public class InspectableObject : InspectableField
    {
        private const int IndentAmount = 5;

        private object propertyValue;
        private InspectorFieldDrawer drawer;
        private InspectableFieldStyleInfo style;

        private GUILayoutY guiLayout;
        private GUILayoutX guiChildLayout;
        private GUILayoutX guiTitleLayout;
        private GUILayoutX guiInternalTitleLayout;
        private GUIButton guiCreateBtn;
        private ContextMenu createContextMenu;
        private bool isExpanded;
        private bool forceUpdate = true;
        private bool isInline;
        private State state;
        private Type[] instantiableTypes;

        /// <summary>
        /// Creates a new inspectable object GUI for the specified property.
        /// </summary>
        /// <param name="context">Context shared by all inspectable fields created by the same parent.</param>
        /// <param name="title">Name of the property, or some other value to set as the title.</param>
        /// <param name="path">Full path to this property (includes name of this property and all parent properties).</param>
        /// <param name="depth">Determines how deep within the inspector nesting hierarchy is this field. Some fields may
        ///                     contain other fields, in which case you should increase this value by one.</param>
        /// <param name="layout">Parent layout that all the field elements will be added to.</param>
        /// <param name="property">Serializable property referencing the object whose contents to display.</param>
        /// <param name="style">Information that can be used for customizing field rendering and behaviour.</param>
        public InspectableObject(InspectableContext context, string title, string path, int depth, InspectableFieldLayout layout, 
            SerializableProperty property, InspectableFieldStyleInfo style)
            : base(context, title, path, SerializableProperty.FieldType.Object, depth, layout, property)
        {
            this.style = style;
            isExpanded = context.Persistent.GetBool(path + "_Expanded");
            isInline = style != null && style.StyleFlags.HasFlag(InspectableFieldStyleFlags.Inline);

            // Builds a context menu that lets the user create objects to assign to this field.
            bool hasCreateButton = !property.IsValueType && !isInline;
            if (hasCreateButton)
            {
                instantiableTypes = GetInstantiableTypes(property.InternalType);
                if (instantiableTypes.Length > 1)
                {
                    createContextMenu = new ContextMenu();

                    Array.Sort(instantiableTypes, (x, y) => string.Compare(x.Name, y.Name, StringComparison.Ordinal));

                    bool showNamespace = false;
                    string ns = instantiableTypes[0].Namespace;
                    for (int i = 1; i < instantiableTypes.Length; i++)
                    {
                        if (instantiableTypes[i].Namespace != ns)
                        {
                            showNamespace = true;
                            break;
                        }
                    }

                    foreach (var type in instantiableTypes)
                    {
                        string prefix = "";
                        if (showNamespace)
                            prefix = type.Namespace + ".";

                        createContextMenu.AddItem(prefix + type.Name,
                            () =>
                            {
                                StartUndo();
                                property.SetValue(Activator.CreateInstance(type));
                                EndUndo();
                            });
                    }
                }
            }
        }

        /// <inheritdoc/>
        public override GUILayoutX GetTitleLayout()
        {
            return guiTitleLayout;
        }

        /// <inheritdoc/>
        public override InspectableState Refresh(int layoutIndex, bool force = false)
        {
            // Check if modified internally and rebuild if needed
            object newPropertyValue = property.GetValue<object>();
            if (forceUpdate)
            {
                propertyValue = newPropertyValue;
                BuildGUI(layoutIndex);
                
                forceUpdate = false;
            }
            else if (propertyValue == null && newPropertyValue != null)
            {
                propertyValue = newPropertyValue;
                BuildGUI(layoutIndex);
                
            }
            else if (newPropertyValue == null && propertyValue != null)
            {
                propertyValue = null;
                BuildGUI(layoutIndex);
            }

            if(drawer != null)
                return drawer.Refresh(force);

            return InspectableState.NotModified;
        }

        /// <inheritdoc />
        public override InspectableField FindPath(string path)
        {
            if(drawer != null)
                return FindPath(path, depth, drawer.Fields);

            return null;
        }

        /// <inheritdoc />
        protected override void SetActive(bool active)
        {
            if (drawer != null)
            {
                foreach (var entry in drawer.Fields)
                    entry.Active = active;
            }

            base.SetActive(active);
        }

        /// <summary>
        /// Rebuilds the GUI object header if needed. 
        /// </summary>
        /// <param name="layoutIndex">Index at which to insert the GUI elements.</param>
        protected void BuildGUI(int layoutIndex)
        {
            Action BuildEmptyGUI = () =>
            {
                if (isInline)
                    return;

                guiInternalTitleLayout = guiTitleLayout.InsertLayoutX(0);

                guiInternalTitleLayout.AddElement(new GUILabel(title));
                guiInternalTitleLayout.AddElement(new GUILabel("Empty", GUIOption.FixedWidth(100)));

                if (!property.IsValueType)
                {
                    GUIContent createIcon = new GUIContent(EditorBuiltin.GetInspectorWindowIcon(InspectorWindowIcon.Create),
                        new LocEdString("Create"));
                    guiCreateBtn = new GUIButton(createIcon, GUIOption.FixedWidth(30));
                    guiCreateBtn.OnClick += OnCreateButtonClicked;
                    guiInternalTitleLayout.AddElement(guiCreateBtn);
                }
            };

            Action BuildFilledGUI = () =>
            {
                if (!isInline)
                {
                    guiInternalTitleLayout = guiTitleLayout.InsertLayoutX(0);

                    GUIToggle guiFoldout = new GUIToggle(title, EditorStyles.Foldout);
                    guiFoldout.Value = isExpanded;
                    guiFoldout.AcceptsKeyFocus = false;
                    guiFoldout.OnToggled += OnFoldoutToggled;
                    guiInternalTitleLayout.AddElement(guiFoldout);

                    if (!property.IsValueType && (style == null || !style.StyleFlags.HasFlag(InspectableFieldStyleFlags.NotNull)))
                    {
                        GUIContent clearIcon = new GUIContent(
                            EditorBuiltin.GetInspectorWindowIcon(InspectorWindowIcon.Clear),
                            new LocEdString("Clear"));
                        GUIButton clearBtn = new GUIButton(clearIcon, GUIOption.FixedWidth(20));
                        clearBtn.OnClick += OnClearButtonClicked;
                        guiInternalTitleLayout.AddElement(clearBtn);
                    }
                }

                if (isExpanded || isInline)
                {
                    SerializableObject serializableObject = property.GetObject();
                    SerializableField[] fields = serializableObject.Fields;

                    if (fields.Length > 0)
                    {
                        GUILayoutY guiContentLayout;
                        if (!isInline)
                        {
                            guiChildLayout = guiLayout.AddLayoutX();
                            guiChildLayout.AddSpace(IndentAmount);

                            GUIPanel guiContentPanel = guiChildLayout.AddPanel();
                            GUILayoutX guiIndentLayoutX = guiContentPanel.AddLayoutX();
                            guiIndentLayoutX.AddSpace(IndentAmount);
                            GUILayoutY guiIndentLayoutY = guiIndentLayoutX.AddLayoutY();
                            guiIndentLayoutY.AddSpace(IndentAmount);
                            guiContentLayout = guiIndentLayoutY.AddLayoutY();
                            guiIndentLayoutY.AddSpace(IndentAmount);
                            guiIndentLayoutX.AddSpace(IndentAmount);
                            guiChildLayout.AddSpace(IndentAmount);

                            short backgroundDepth = (short) (Inspector.START_BACKGROUND_DEPTH - depth - 1);
                            string bgPanelStyle = depth % 2 == 0
                                ? EditorStylesInternal.InspectorContentBgAlternate
                                : EditorStylesInternal.InspectorContentBg;
                            GUIPanel backgroundPanel = guiContentPanel.AddPanel(backgroundDepth);
                            GUITexture inspectorContentBg = new GUITexture(null, bgPanelStyle);
                            backgroundPanel.AddElement(inspectorContentBg);
                        }
                        else
                            guiContentLayout = guiLayout;

                        drawer = new InspectorFieldDrawer(context, guiContentLayout, path, depth + 1);
                        drawer.AddDefault(serializableObject);

                        if (!active)
                        {
                            foreach (var field in drawer.Fields)
                                field.Active = false;
                        }
                    }
                }
                else
                    guiChildLayout = null;
            };

            if (state == State.None)
            {
                if (propertyValue != null)
                {
                    BuildFilledGUI();
                    state = State.Filled;
                }
                else
                {
                    BuildEmptyGUI();

                    state = State.Empty;
                }
            }
            else if (state == State.Empty)
            {
                if (propertyValue != null)
                {
                    guiInternalTitleLayout?.Destroy();
                    guiInternalTitleLayout = null;
                    guiCreateBtn = null;

                    BuildFilledGUI();
                    state = State.Filled;
                }
            }
            else if (state == State.Filled)
            {
                drawer?.Clear();

                guiInternalTitleLayout?.Destroy();
                guiInternalTitleLayout = null;
                guiCreateBtn = null;

                if (guiChildLayout != null)
                {
                    guiChildLayout.Destroy();
                    guiChildLayout = null;
                }

                if (propertyValue == null)
                {
                    BuildEmptyGUI();
                    state = State.Empty;
                }
                else
                {
                    BuildFilledGUI();
                }
            }
        }

        /// <inheritdoc/>
        protected internal override void Initialize(int index)
        {
            guiLayout = layout.AddLayoutY(index);

            if(!isInline)
                guiTitleLayout = guiLayout.AddLayoutX();

            propertyValue = property.GetValue<object>();
            BuildGUI(index);
        }

        /// <summary>
        /// Triggered when the user clicks on the expand/collapse toggle in the title bar.
        /// </summary>
        /// <param name="expanded">Determines whether the contents were expanded or collapsed.</param>
        private void OnFoldoutToggled(bool expanded)
        {
            context.Persistent.SetBool(path + "_Expanded", expanded);

            isExpanded = expanded;
            forceUpdate = true;
        }

        /// <summary>
        /// Triggered when the user clicks on the create button on the title bar. Creates a brand new object with default
        /// values in the place of the current array.
        /// </summary>
        private void OnCreateButtonClicked()
        {
            if (createContextMenu == null)
            {
                if (instantiableTypes.Length > 0)
                {
                    StartUndo();
                    property.SetValue(Activator.CreateInstance(instantiableTypes[0]));
                    EndUndo();
                }
            }
            else
            {
                Rect2I bounds = GUIUtility.CalculateBounds(guiCreateBtn, guiInternalTitleLayout);
                Vector2I position = new Vector2I(bounds.x + bounds.width / 2, bounds.y + bounds.height / 2);

                createContextMenu.Open(position, guiInternalTitleLayout);
            }
        }

        /// <summary>
        /// Triggered when the user clicks on the clear button on the title bar. Deletes the current object and sets
        /// the reference to the object in the parent object to null. This is only relevant for objects of reference types.
        /// </summary>
        private void OnClearButtonClicked()
        {
            StartUndo();
            property.SetValue<object>(null);
            EndUndo();
        }

        /// <summary>
        /// Possible states object GUI can be in.
        /// </summary>
        private enum State
        {
            None,
            Empty,
            Filled
        }

        /// <summary>
        /// Returns a list of all types that can be created using the parameterless constructor and assigned to an object of
        /// type <paramref name="type"/>.
        /// </summary>
        /// <param name="type">Type to which the instantiable types need to be assignable to.</param>
        /// <returns>List of types that can be instantiated and assigned type <paramref name="type"/></returns>
        private static Type[] GetInstantiableTypes(Type type)
        {
            // Note: This could be cached
            List<Type> output = new List<Type>();

            Assembly[] assemblies = AppDomain.CurrentDomain.GetAssemblies();
            foreach (var assembly in assemblies)
            {
                Type[] assemblyTypes = assembly.GetExportedTypes();
                foreach (var assemblyType in assemblyTypes)
                {
                    if (assemblyType.IsAbstract)
                        continue;

                    if (!type.IsAssignableFrom(assemblyType))
                        continue;

                    var ctor = assemblyType.GetConstructor(Type.EmptyTypes);
                    if (ctor == null)
                        continue;

                    output.Add(assemblyType);
                }
            }

            return output.ToArray();
        }
    }

    /** @} */
}
