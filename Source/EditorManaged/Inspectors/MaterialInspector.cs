//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//

using System;
using System.Collections.Generic;
using bs;

namespace bs.Editor
{
    /** @addtogroup Inspectors
     *  @{
     */

    /// <summary>
    /// Renders an inspector for the <see cref="Material"/> resource.
    /// </summary>
    [CustomInspector(typeof (Material))]
    internal class MaterialInspector : Inspector
    {
        private MaterialParamGUI[] guiParams;
        private MaterialVariationGUI guiVariations;
        private GUIResourceField shaderField;
        private GUIEnumField builtinShaderField;
        private Material material;

        /// <inheritdoc/>
        protected internal override void Initialize()
        {
            LoadResource();

            material = InspectedObject as Material;
            if (material == null)
                return;

            Shader activeShader = material.Shader.Value;
            BuiltinShader builtinType = ShaderToBuiltin(activeShader);

            builtinShaderField = new GUIEnumField(typeof(BuiltinShader), new LocEdString("Shader"));
            builtinShaderField.Value = (ulong) builtinType;
            builtinShaderField.OnSelectionChanged += OnBuiltinShaderFieldChanged;

            shaderField = new GUIResourceField(typeof(Shader), new LocEdString("Shader file"));
            shaderField.ValueRef = material.Shader;
            shaderField.OnChanged += (x) =>
            {
                Shader shader = Resources.Load<Shader>(x.UUID);

                material.Shader = shader;
                EditorApplication.SetDirty(material);
                RebuildParamGUI(material);
            };

            bool isCustom = builtinType == BuiltinShader.Custom;
            shaderField.Active = isCustom;

            RebuildParamGUI(material);
        }

        /// <inheritdoc/>
        protected internal override InspectableState Refresh(bool force = false)
        {
            if (material == null)
                return InspectableState.NotModified;

            if (material.Shader != shaderField.ValueRef)
            {
                shaderField.ValueRef = material.Shader;
                RebuildParamGUI(material);
            }

            if (guiParams != null)
            {
                foreach (var param in guiParams)
                    param.Refresh(material);
            }

            return InspectableState.NotModified;
        }

        private void OnBuiltinShaderFieldChanged(UInt64 value)
        {
            Shader activeShader = material.Shader.Value;
            BuiltinShader builtinType = ShaderToBuiltin(activeShader);
            BuiltinShader newBuiltinType = (BuiltinShader)value;

            if (builtinType == newBuiltinType)
                return;

            material.Shader = Builtin.GetShader(newBuiltinType);
            EditorApplication.SetDirty(material);
            RebuildParamGUI(material);

            bool newIsCustom = newBuiltinType == BuiltinShader.Custom;
            shaderField.Active = newIsCustom;
        }

        /// <summary>
        /// Recreates GUI elements for all material parameters.
        /// </summary>
        /// <param name="material">Material to create parameters for</param>
        private void RebuildParamGUI(Material material)
        {
            if (guiParams != null)
            {
                foreach (var param in guiParams)
                    param.Destroy();

                guiParams = null;
            }

            guiVariations = null;

            Layout.Clear();
            Layout.AddElement(builtinShaderField);
            Layout.AddElement(shaderField);

            if (material != null && material.Shader != null)
            {
                guiVariations = new MaterialVariationGUI(material, Layout);
                guiParams = CreateMaterialGUI(material, "", null, Layout);
            }
        }

        /// <summary>
        /// Converts a shader resource into a builtin shader.
        /// </summary>
        /// <param name="shader">Shader resource to convert.</param>
        /// <returns>Type of builtin shader, if any.</returns>
        private BuiltinShader ShaderToBuiltin(Shader shader)
        {
            // Note: Might be nice to have a better way to detect the builtin shader perhaps (store it in Material?)
            //   - Or I could just compare UUID's here to avoid loading the shaders
            var enumValues = Enum.GetValues(typeof(BuiltinShader));
            for (int i = 1; i < enumValues.Length; i++)
            {
                Shader builtinShader = Builtin.GetShader((BuiltinShader) i);
                if (builtinShader == shader)
                    return (BuiltinShader) i;
            }
            
            return BuiltinShader.Custom;
        }

        /// <summary>
        /// Creates a set of objects in which each object represents a GUI for a material parameter.
        /// </summary>
        /// <param name="mat">Material for whose parameters to create GUI for.</param>
        /// <param name="path">Path to the material field in the parent object.</param>
        /// <param name="component">Optional component the material is part of (if any).</param>
        /// <param name="layout">Layout to add the parameter GUI elements to.</param>
        /// <returns>A material parameter GUI object for each supported material parameter.</returns>
        internal static MaterialParamGUI[] CreateMaterialGUI(Material mat, string path, Component component, 
            GUILayout layout)
        {
            Shader shader = mat.Shader.Value;
            if (shader == null)
                return new MaterialParamGUI[0];

            List<MaterialParamGUI> guiParams = new List<MaterialParamGUI>();
            ShaderParameter[] shaderParams = shader.Parameters;

            foreach (var param in shaderParams)
            {
                if (param.flags.HasFlag(ShaderParameterFlag.Internal) || param.flags.HasFlag(ShaderParameterFlag.HideInInspector))
                    continue;

                switch (param.type)
                {
                    case ShaderParameterType.Float:
                        layout.AddSpace(5);
                        guiParams.Add(new MaterialParamFloatGUI(param, path, component, mat, layout));
                        break;
                    case ShaderParameterType.Vector2:
                        layout.AddSpace(5);
                        guiParams.Add(new MaterialParamVec2GUI(param, path, component, mat, layout));
                        break;
                    case ShaderParameterType.Vector3:
                        layout.AddSpace(5);
                        guiParams.Add(new MaterialParamVec3GUI(param, path, component, mat, layout));
                        break;
                    case ShaderParameterType.Vector4:
                        layout.AddSpace(5);
                        guiParams.Add(new MaterialParamVec4GUI(param, path, component, mat, layout));
                        break;
                    case ShaderParameterType.Matrix3:
                        layout.AddSpace(5);
                        guiParams.Add(new MaterialParamMat3GUI(param, path, component, mat, layout));
                        break;
                    case ShaderParameterType.Matrix4:
                        layout.AddSpace(5);
                        guiParams.Add(new MaterialParamMat4GUI(param, path, component, mat, layout));
                        break;
                    case ShaderParameterType.Color:
                        bool hdr = param.flags.HasFlag(ShaderParameterFlag.HDR);
                        layout.AddSpace(5);
                        guiParams.Add(new MaterialParamColorGUI(param, path, hdr, component, mat, layout));
                        break;
                    case ShaderParameterType.Texture2D:
                    case ShaderParameterType.Texture3D:
                    case ShaderParameterType.TextureCube:
                        layout.AddSpace(5);
                        guiParams.Add(new MaterialParamTextureGUI(param, path, component, mat, layout));
                        break;
                }
            }

            return guiParams.ToArray();
        }
    }

    /// <summary>
    /// Draws GUI that allows the user to change the active shader variation for a material.
    /// </summary>
    internal class MaterialVariationGUI
    {
        private Material material;
        private ShaderVariation variation;

        /// <summary>
        /// Creates necessary GUI elements for selecting a material variation.
        /// </summary>
        /// <param name="material">Material for which to provide variation selection.</param>
        /// <param name="layout">GUI layout to which to append GUI elements.</param>
        internal MaterialVariationGUI(Material material, GUILayout layout)
        {
            this.material = material;
            variation = material.Variation;

            Shader shader = material.Shader.Value;
            if (shader == null)
                return;

            ShaderVariationParamInfo[] variationParams = shader.VariationParams;
            foreach (var param in variationParams)
            {
                if (param.isInternal)
                    continue;

                LocString[] names = new LocString[param.values.Length];
                int[] values = new int[names.Length];
                for (int i = 0; i < names.Length; i++)
                {
                    names[i] = new LocEdString(param.values[i].name);
                    values[i] = param.values[i].value;
                }

                GUIListBoxField listBox = new GUIListBoxField(names, new LocEdString(param.name));
                listBox.OnSelectionChanged += idx =>
                {
                    int value = values[idx];
                    variation.SetInt(param.identifier, value);

                    material.Variation = variation;
                };

                int curValue = variation.GetInt(param.identifier);
                for (int j = 0; j < values.Length; j++)
                {
                    if (curValue == values[j])
                    {
                        listBox.Index = j;
                        break;
                    }
                }

                layout.AddElement(listBox);
            }
        }
    }

    /// <summary>
    /// Contains GUI element(s) for a single parameter in a <see cref="Material"/>.
    /// </summary>
    internal abstract class MaterialParamGUI
    {
        protected ShaderParameter shaderParam;
        protected string path;
        protected Component component;

        /// <summary>
        /// Underlying shader parameter the field is representing.
        /// </summary>
        internal ShaderParameter Param { get => shaderParam; }

        /// <summary>
        /// Creates a new material parameter GUI.
        /// </summary>
        /// <param name="shaderParam">Shader parameter to create the GUI for.</param>
        /// <param name="path">Path to the material field in the parent object.</param>
        /// <param name="component">Optional component the material is part of (if any).</param>
        protected MaterialParamGUI(ShaderParameter shaderParam, string path, Component component)
        {
            this.shaderParam = shaderParam;
            this.path = path;
            this.component = component;
        }

        /// <summary>
        /// Checks if the data stored in GUI and in the material matches, and updates the GUI if it doesn't.
        /// </summary>
        /// <param name="material">Material whose data to check.</param>
        internal abstract void Refresh(Material material);

        /// <summary> 
        /// Destroys the internal GUI elements.
        /// </summary>
        internal abstract void Destroy();

        /// <summary>
        /// Moves keyboard focus to this field.
        /// </summary>
        /// <param name="subFieldName">
        /// Name of the sub-field to focus on. Only relevant if the field represents multiple GUI input elements.
        /// </param>
        public virtual void SetHasFocus(string subFieldName = null) { }

        /// <summary>
        /// Zero parameter wrapper for <see cref="StartUndo(string)"/>
        /// </summary>
        protected void StartUndo()
        {
            StartUndo(null);
        }

        /// <summary>
        /// Notifies the system to start recording a new undo command. Any changes to the field after this is called
        /// will be recorded in the command. User must call <see cref="EndUndo"/> after field is done being changed.
        /// </summary>
        /// <param name="subPath">Additional path to append to the end of the current field path.</param>
        protected void StartUndo(string subPath)
        {
            if (component != null)
            {
                string fullPath = path.TrimEnd('/');
                fullPath += "/" + shaderParam.name;

                if (!string.IsNullOrEmpty(subPath))
                    fullPath += '/' + subPath.TrimStart('/');

                GameObjectUndo.RecordComponent(component, fullPath);
            }
        }

        /// <summary>
        /// Finishes recording an undo command started via <see cref="StartUndo(string)"/>. If any changes are detected on
        /// the field an undo command is recorded onto the undo-redo stack, otherwise nothing is done.
        /// </summary>
        protected void EndUndo()
        {
            GameObjectUndo.ResolveDiffs();
        }
    }

    /// <summary>
    /// Contains GUI element(s) for a single floating point parameter in a <see cref="Material"/>.
    /// </summary>
    internal class MaterialParamFloatGUI : MaterialParamGUI
    {
        private GUILayout fieldLayout;
        private GUIFloatField guiConstant;
        private GUICurvesField guiCurves;

        /// <summary>
        /// Creates a new material parameter GUI.
        /// </summary>
        /// <param name="shaderParam">Shader parameter to create the GUI for. Must be of floating point type.</param>
        /// <param name="path">Path to the material field in the parent object.</param>
        /// <param name="component">Optional component the material is part of (if any).</param>
        /// <param name="material">Material the parameter is a part of.</param>
        /// <param name="layout">Layout to append the GUI elements to.</param>
        internal MaterialParamFloatGUI(ShaderParameter shaderParam, string path, Component component, 
            Material material, GUILayout layout)
            : base(shaderParam, path, component)
        {
            LocString title = new LocEdString(shaderParam.name);

            var guiToggle = new GUIToggle(new GUIContent(
                EditorBuiltin.GetEditorToggleIcon(EditorToggleIcon.AnimateProperty), new LocString("Animate")));
            guiConstant = new GUIFloatField(title);
            guiCurves = new GUICurvesField(title);

            bool isAnimated = material.IsAnimated(shaderParam.name);
            guiConstant.Active = !isAnimated;
            guiCurves.Active = isAnimated;

            fieldLayout = layout.AddLayoutX();
            fieldLayout.AddElement(guiConstant);
            fieldLayout.AddElement(guiCurves);
            fieldLayout.AddSpace(10);
            fieldLayout.AddElement(guiToggle);

            guiConstant.OnChanged += (x) =>
            {
                material.SetFloat(shaderParam.name, x);
                EditorApplication.SetDirty(material);
            };

            guiConstant.OnFocusGained += () => StartUndo("constant");
            guiConstant.OnFocusLost += EndUndo;
            guiConstant.OnConfirmed += () =>
            {
                EndUndo();
                StartUndo("constant");
            };

            guiCurves.OnChanged += x =>
            {
                StartUndo("curve");

                material.SetFloatCurve(shaderParam.name, x);
                EditorApplication.SetDirty(material);

                EndUndo();
            };

            guiToggle.OnToggled += x =>
            {
                guiConstant.Active = !x;
                guiCurves.Active = x;
            };
        }

        /// <inheritdoc/>
        internal override void Refresh(Material material)
        {
            bool isAnimated = material.IsAnimated(shaderParam.name);
            if (isAnimated)
                guiCurves.SetCurve(material.GetFloatCurve(shaderParam.name));
            else
                guiConstant.Value = material.GetFloat(shaderParam.name);
        }

        /// <inheritdoc/>
        internal override void Destroy()
        {
            fieldLayout.Destroy();
        }

        /// <inheritdoc />
        public override void SetHasFocus(string subFieldName = null)
        {
            if (subFieldName == "constant")
                guiConstant.Focus = true;
        }
    }

    /// <summary>
    /// Contains GUI element(s) for a single 2D vector parameter in a <see cref="Material"/>.
    /// </summary>
    internal class MaterialParamVec2GUI : MaterialParamGUI
    {
        private GUIVector2Field guiElem;

        /// <summary>
        /// Creates a new material parameter GUI.
        /// </summary>
        /// <param name="shaderParam">Shader parameter to create the GUI for. Must be of 2D vector type.</param>
        /// <param name="path">Path to the material field in the parent object.</param>
        /// <param name="component">Optional component the material is part of (if any).</param>
        /// <param name="material">Material the parameter is a part of.</param>
        /// <param name="layout">Layout to append the GUI elements to.</param>
        internal MaterialParamVec2GUI(ShaderParameter shaderParam, string path, Component component, Material material, 
            GUILayout layout)
            : base(shaderParam, path, component)
        {
            LocString title = new LocEdString(shaderParam.name);
            guiElem = new GUIVector2Field(title);
            guiElem.OnValueChanged += (x) =>
            {
                material.SetVector2(shaderParam.name, x);
                EditorApplication.SetDirty(material);
            };

            guiElem.OnComponentFocusChanged += (focus, comp) =>
            {
                if(focus)
                    StartUndo(comp.ToString());
                else
                    EndUndo();
            };

            guiElem.OnConfirm += comp =>
            {
                EndUndo();
                StartUndo(comp.ToString());
            };

            layout.AddElement(guiElem);
        }

        /// <inheritdoc/>
        internal override void Refresh(Material material)
        {
            guiElem.Value = material.GetVector2(shaderParam.name);
        }

        /// <inheritdoc/>
        internal override void Destroy()
        {
            guiElem.Destroy();
        }

        /// <inheritdoc />
        public override void SetHasFocus(string subFieldName = null)
        {
            if (subFieldName == "x")
                guiElem.SetInputFocus(VectorComponent.X, true);
            else if(subFieldName == "y")
                guiElem.SetInputFocus(VectorComponent.Y, true);
        }
    }

    /// <summary>
    /// Contains GUI element(s) for a single 3D vector parameter in a <see cref="Material"/>.
    /// </summary>
    internal class MaterialParamVec3GUI : MaterialParamGUI
    {
        private GUIVector3Field guiElem;

        /// <summary>
        /// Creates a new material parameter GUI.
        /// </summary>
        /// <param name="shaderParam">Shader parameter to create the GUI for. Must be of 3D vector type.</param>
        /// <param name="path">Path to the material field in the parent object.</param>
        /// <param name="component">Optional component the material is part of (if any).</param>
        /// <param name="material">Material the parameter is a part of.</param>
        /// <param name="layout">Layout to append the GUI elements to.</param>
        /// <param name="loadResources">
        /// If true, any assigned texture resources will be loaded in memory. If false the resources will be just referenced
        /// without loading.
        /// </param>
        internal MaterialParamVec3GUI(ShaderParameter shaderParam, string path, Component component, Material material, 
            GUILayout layout)
            : base(shaderParam, path, component)
        {
            LocString title = new LocEdString(shaderParam.name);
            guiElem = new GUIVector3Field(title);
            guiElem.OnValueChanged += (x) =>
            {
                material.SetVector3(shaderParam.name, x);
                EditorApplication.SetDirty(material);
            };

            guiElem.OnComponentFocusChanged += (focus, comp) =>
            {
                if(focus)
                    StartUndo(comp.ToString());
                else
                    EndUndo();
            };

            guiElem.OnConfirm += comp =>
            {
                EndUndo();
                StartUndo(comp.ToString());
            };

            layout.AddElement(guiElem);
        }

        /// <inheritdoc/>
        internal override void Refresh(Material material)
        {
            guiElem.Value = material.GetVector3(shaderParam.name);
        }

        /// <inheritdoc/>
        internal override void Destroy()
        {
            guiElem.Destroy();
        }

        /// <inheritdoc />
        public override void SetHasFocus(string subFieldName = null)
        {
            if (subFieldName == "x")
                guiElem.SetInputFocus(VectorComponent.X, true);
            else if(subFieldName == "y")
                guiElem.SetInputFocus(VectorComponent.Y, true);
            else if(subFieldName == "z")
                guiElem.SetInputFocus(VectorComponent.Z, true);
        }
    }

    /// <summary>
    /// Contains GUI element(s) for a single 4D vector parameter in a <see cref="Material"/>.
    /// </summary>
    internal class MaterialParamVec4GUI : MaterialParamGUI
    {
        private GUIVector4Field guiElem;

        /// <summary>
        /// Creates a new material parameter GUI.
        /// </summary>
        /// <param name="shaderParam">Shader parameter to create the GUI for. Must be of 4D vector type.</param>
        /// <param name="path">Path to the material field in the parent object.</param>
        /// <param name="component">Optional component the material is part of (if any).</param>
        /// <param name="material">Material the parameter is a part of.</param>
        /// <param name="layout">Layout to append the GUI elements to.</param>
        internal MaterialParamVec4GUI(ShaderParameter shaderParam, string path, Component component, Material material, 
            GUILayout layout)
            : base(shaderParam, path, component)
        {
            LocString title = new LocEdString(shaderParam.name);
            guiElem = new GUIVector4Field(title);
            guiElem.OnValueChanged += (x) =>
            {
                material.SetVector4(shaderParam.name, x);
                EditorApplication.SetDirty(material);
            };

            guiElem.OnComponentFocusChanged += (focus, comp) =>
            {
                if(focus)
                    StartUndo(comp.ToString());
                else
                    EndUndo();
            };

            guiElem.OnConfirm += comp =>
            {
                EndUndo();
                StartUndo(comp.ToString());
            };

            layout.AddElement(guiElem);
        }

        /// <inheritdoc/>
        internal override void Refresh(Material material)
        {
            guiElem.Value = material.GetVector4(shaderParam.name);
        }

        /// <inheritdoc/>
        internal override void Destroy()
        {
            guiElem.Destroy();
        }

        /// <inheritdoc />
        public override void SetHasFocus(string subFieldName = null)
        {
            if (subFieldName == "x")
                guiElem.SetInputFocus(VectorComponent.X, true);
            else if(subFieldName == "y")
                guiElem.SetInputFocus(VectorComponent.Y, true);
            else if(subFieldName == "z")
                guiElem.SetInputFocus(VectorComponent.Z, true);
            else if(subFieldName == "w")
                guiElem.SetInputFocus(VectorComponent.W, true);
        }
    }

    /// <summary>
    /// Contains GUI element(s) for a single 3x3 matrix parameter in a <see cref="Material"/>.
    /// </summary>
    internal class MaterialParamMat3GUI : MaterialParamGUI
    {
        private const int MAT_SIZE = 3;

        private GUILayout mainLayout;
        private GUIFloatField[] guiMatFields = new GUIFloatField[MAT_SIZE * MAT_SIZE];

        /// <summary>
        /// Creates a new material parameter GUI.
        /// </summary>
        /// <param name="shaderParam">Shader parameter to create the GUI for. Must be of 3x3 matrix type.</param>
        /// <param name="path">Path to the material field in the parent object.</param>
        /// <param name="component">Optional component the material is part of (if any).</param>
        /// <param name="material">Material the parameter is a part of.</param>
        /// <param name="layout">Layout to append the GUI elements to.</param>
        internal MaterialParamMat3GUI(ShaderParameter shaderParam, string path, Component component, Material material, 
            GUILayout layout)
            : base(shaderParam, path, component)
        {
            LocString title = new LocEdString(shaderParam.name);
            GUILabel guiTitle = new GUILabel(title, GUIOption.FixedWidth(100));

            mainLayout = layout.AddLayoutY();
            GUILayoutX titleLayout = mainLayout.AddLayoutX();
            titleLayout.AddElement(guiTitle);
            titleLayout.AddFlexibleSpace();

            GUILayoutY contentLayout = mainLayout.AddLayoutY();

            GUILayoutX[] rows = new GUILayoutX[MAT_SIZE];
            for (int i = 0; i < rows.Length; i++)
                rows[i] = contentLayout.AddLayoutX();

            for (int row = 0; row < MAT_SIZE; row++)
            {
                for (int col = 0; col < MAT_SIZE; col++)
                {
                    int index = row * MAT_SIZE + col;
                    guiMatFields[index] = new GUIFloatField(row + "," + col, 20, "", GUIOption.FixedWidth(80));

                    GUIFloatField field = guiMatFields[index];
                    rows[row].AddElement(field);
                    rows[row].AddSpace(5);

                    int hoistedRow = row;
                    int hoistedCol = col;
                    field.OnChanged += (x) =>
                    {
                        Matrix3 value = material.GetMatrix3(shaderParam.name);
                        value[hoistedRow, hoistedCol] = x;
                        material.SetMatrix3(shaderParam.name, value);
                        EditorApplication.SetDirty(material);
                    };

                    field.OnFocusGained += () => StartUndo(hoistedRow + "x" + hoistedCol);
                    field.OnFocusLost += EndUndo;
                    field.OnConfirmed += () =>
                    {
                        EndUndo();
                        StartUndo(hoistedRow + "x" + hoistedCol);
                    };
                }
            }
        }

        /// <inheritdoc/>
        internal override void Refresh(Material material)
        {
            Matrix3 value = material.GetMatrix3(shaderParam.name);

            for (int row = 0; row < MAT_SIZE; row++)
            {
                for (int col = 0; col < MAT_SIZE; col++)
                {
                    int index = row * MAT_SIZE + col;
                    guiMatFields[index].Value = value[row, col];
                }
            }
        }

        /// <inheritdoc/>
        internal override void Destroy()
        {
            mainLayout.Destroy();
        }

        /// <inheritdoc />
        public override void SetHasFocus(string subFieldName = null)
        {
            if (string.IsNullOrEmpty(subFieldName))
                return;

            string[] parts = subFieldName.Split('x');
            if (parts.Length != 2)
                return;

            if (!int.TryParse(parts[0], out int row) || !int.TryParse(parts[1], out int col))
                return;

            if (row >= MAT_SIZE && col >= MAT_SIZE)
                return;

            int index = row * MAT_SIZE + col;
            guiMatFields[index].Focus = true;

        }
    }

    /// <summary>
    /// Contains GUI element(s) for a single 4x4 matrix parameter in a <see cref="Material"/>.
    /// </summary>
    internal class MaterialParamMat4GUI : MaterialParamGUI
    {
        private const int MAT_SIZE = 4;

        private GUILayout mainLayout;
        private GUIFloatField[] guiMatFields = new GUIFloatField[MAT_SIZE * MAT_SIZE];

        /// <summary>
        /// Creates a new material parameter GUI.
        /// </summary>
        /// <param name="shaderParam">Shader parameter to create the GUI for. Must be of 4x4 matrix type.</param>
        /// <param name="path">Path to the material field in the parent object.</param>
        /// <param name="component">Optional component the material is part of (if any).</param>
        /// <param name="material">Material the parameter is a part of.</param>
        /// <param name="layout">Layout to append the GUI elements to.</param>
        internal MaterialParamMat4GUI(ShaderParameter shaderParam, string path, Component component, Material material, 
            GUILayout layout)
            : base(shaderParam, path, component)
        {
            LocString title = new LocEdString(shaderParam.name);
            GUILabel guiTitle = new GUILabel(title, GUIOption.FixedWidth(100));

            mainLayout = layout.AddLayoutY();
            GUILayoutX titleLayout = mainLayout.AddLayoutX();
            titleLayout.AddElement(guiTitle);
            titleLayout.AddFlexibleSpace();

            GUILayoutY contentLayout = mainLayout.AddLayoutY();

            GUILayoutX[] rows = new GUILayoutX[MAT_SIZE];
            for (int i = 0; i < rows.Length; i++)
                rows[i] = contentLayout.AddLayoutX();

            for (int row = 0; row < MAT_SIZE; row++)
            {
                for (int col = 0; col < MAT_SIZE; col++)
                {
                    int index = row * MAT_SIZE + col;
                    guiMatFields[index] = new GUIFloatField(row + "," + col, 20, "", GUIOption.FixedWidth(80));

                    GUIFloatField field = guiMatFields[index];
                    rows[row].AddElement(field);
                    rows[row].AddSpace(5);

                    int hoistedRow = row;
                    int hoistedCol = col;
                    field.OnChanged += (x) =>
                    {
                        Matrix4 value = material.GetMatrix4(shaderParam.name);
                        value[hoistedRow, hoistedCol] = x;
                        material.SetMatrix4(shaderParam.name, value);
                        EditorApplication.SetDirty(material);
                    };

                    field.OnFocusGained += () => StartUndo(hoistedRow + "x" + hoistedCol);
                    field.OnFocusLost += EndUndo;
                    field.OnConfirmed += () =>
                    {
                        EndUndo();
                        StartUndo(hoistedRow + "x" + hoistedCol);
                    };
                }
            }
        }

        /// <inheritdoc/>
        internal override void Refresh(Material material)
        {
            Matrix4 value = material.GetMatrix4(shaderParam.name);

            for (int row = 0; row < MAT_SIZE; row++)
            {
                for (int col = 0; col < MAT_SIZE; col++)
                {
                    int index = row * MAT_SIZE + col;
                    guiMatFields[index].Value = value[row, col];
                }
            }
        }

        /// <inheritdoc/>
        internal override void Destroy()
        {
            mainLayout.Destroy();
        }

        /// <inheritdoc />
        public override void SetHasFocus(string subFieldName = null)
        {
            if (string.IsNullOrEmpty(subFieldName))
                return;

            string[] parts = subFieldName.Split('x');
            if (parts.Length != 2)
                return;

            if (!int.TryParse(parts[0], out int row) || !int.TryParse(parts[1], out int col))
                return;

            if (row >= MAT_SIZE && col >= MAT_SIZE)
                return;

            int index = row * MAT_SIZE + col;
            guiMatFields[index].Focus = true;

        }
    }

    /// <summary>
    /// Contains GUI element(s) for a single color parameter in a <see cref="Material"/>.
    /// </summary>
    internal class MaterialParamColorGUI : MaterialParamGUI
    {
        private GUILayout fieldLayout;
        private GUIColorField guiColor;
        private GUIColorGradientHDRField guiColorGradient;

        /// <summary>
        /// Creates a new material parameter GUI.
        /// </summary>
        /// <param name="shaderParam">Shader parameter to create the GUI for. Must be of color type.</param>
        /// <param name="path">Path to the material field in the parent object.</param>
        /// <param name="hdr">If true the color supports range outside of [0, 1].</param>
        /// <param name="component">Optional component the material is part of (if any).</param>
        /// <param name="material">Material the parameter is a part of.</param>
        /// <param name="layout">Layout to append the GUI elements to.</param>
        internal MaterialParamColorGUI(ShaderParameter shaderParam, string path, bool hdr, Component component, Material material, 
            GUILayout layout)
            : base(shaderParam, path, component)
        {
            LocString title = new LocEdString(shaderParam.name);

            var guiToggle = new GUIToggle(new GUIContent( 
                EditorBuiltin.GetEditorToggleIcon(EditorToggleIcon.AnimateProperty), new LocString("Animate")));
            guiColor = new GUIColorField(title);
            guiColor.AllowHDR = hdr;
            guiColorGradient = new GUIColorGradientHDRField(title);

            bool isAnimated = material.IsAnimated(shaderParam.name);
            guiColor.Active = !isAnimated;
            guiColorGradient.Active = isAnimated;

            fieldLayout = layout.AddLayoutX();
            fieldLayout.AddElement(guiColor);
            fieldLayout.AddElement(guiColorGradient);
            fieldLayout.AddSpace(10);
            fieldLayout.AddElement(guiToggle);

            guiColor.OnChanged += (x) =>
            {
                StartUndo();

                material.SetColor(shaderParam.name, x);
                EditorApplication.SetDirty(material);

                EndUndo();
            };

            guiColorGradient.OnChanged += x =>
            {
                StartUndo();

                material.SetColorGradient(shaderParam.name, x);
                EditorApplication.SetDirty(material);

                EndUndo();
            };

            guiToggle.OnToggled += x =>
            {
                guiColor.Active = !x;
                guiColorGradient.Active = x;

                if (x)
                {
                    ColorGradientHDR gradient = material.GetColorGradient(shaderParam.name);
                    if (gradient.NumKeys == 0)
                        material.SetColorGradient(shaderParam.name, new ColorGradientHDR(material.GetColor(shaderParam.name)));
                }
            };
        }

        /// <inheritdoc/>
        internal override void Refresh(Material material)
        {
            bool isAnimated = material.IsAnimated(shaderParam.name);
            if (isAnimated)
                guiColorGradient.Value = material.GetColorGradient(shaderParam.name);
            else
                guiColor.Value = material.GetColor(shaderParam.name);
        }

        /// <inheritdoc/>
        internal override void Destroy()
        {
            fieldLayout.Destroy();
        }
    }

    /// <summary>
    /// Contains GUI element(s) for a single texture parameter in a <see cref="Material"/>.
    /// </summary>
    internal class MaterialParamTextureGUI : MaterialParamGUI
    {
        private GUITextureField guiElem;

        /// <summary>
        /// Creates a new material parameter GUI.
        /// </summary>
        /// <param name="shaderParam">Shader parameter to create the GUI for. Must be of texture type.</param>
        /// <param name="path">Path to the material field in the parent object.</param>
        /// <param name="component">Optional component the material is part of (if any).</param>
        /// <param name="material">Material the parameter is a part of.</param>
        /// <param name="layout">Layout to append the GUI elements to.</param>
        internal MaterialParamTextureGUI(ShaderParameter shaderParam, string path, Component component, Material material, 
            GUILayout layout)
            : base(shaderParam, path, component)
        {
            LocString title = new LocEdString(shaderParam.name);

            GUITextureFieldType type = shaderParam.type == ShaderParameterType.Texture2D ? 
                GUITextureFieldType.TextureOrSpriteTexture :
                GUITextureFieldType.Texture;

            guiElem = new GUITextureField(type, title);

            switch (shaderParam.type)
            {
                case ShaderParameterType.Texture2D:
                case ShaderParameterType.Texture3D:
                case ShaderParameterType.TextureCube:
                    guiElem.OnChanged += (x) =>
                    {
                        string texPath = ProjectLibrary.GetPath(x.UUID);
                        if (!string.IsNullOrEmpty(texPath))
                        {
                            if (ProjectLibrary.GetEntry(texPath) is FileEntry fileEntry)
                            {
                                if (fileEntry.ResourceMetas.Length > 0)
                                {
                                    StartUndo();

                                    // Note: Ideally we can avoid loading texture resources if the material is not
                                    // referenced anywhere in the scene. But we don't have a good way to check that at
                                    // the moment.
                                    ResourceMeta meta = fileEntry.ResourceMetas[0];
                                    if (meta.ResType == ResourceType.SpriteTexture)
                                        material.SetSpriteTexture(shaderParam.name, Resources.LoadAsync<SpriteTexture>(x.UUID));
                                    else if (meta.ResType == ResourceType.Texture)
                                        material.SetTexture(shaderParam.name, Resources.LoadAsync<Texture>(x.UUID));

                                    EndUndo();
                                }
                            }
                        }
                        else
                        {
                            StartUndo();
                            material.SetTexture(shaderParam.name, null);
                            EndUndo();
                        }

                        EditorApplication.SetDirty(material);
                    };
                    break;
            }

            layout.AddElement(guiElem);
        }

        /// <inheritdoc/>
        internal override void Refresh(Material material)
        {
            RRef<Texture> texture;
            switch (shaderParam.type)
            {
                case ShaderParameterType.Texture2D:
                    RRef<SpriteTexture> spriteTex = material.GetSpriteTexture(shaderParam.name);

                    if (spriteTex != null && spriteTex.UUID != UUID.Empty)
                        guiElem.SpriteTextureRef = spriteTex;
                    else
                    {
                        texture = material.GetTexture(shaderParam.name);
                        guiElem.TextureRef = texture;
                    }

                    break;
                case ShaderParameterType.Texture3D:
                case ShaderParameterType.TextureCube:
                    texture = material.GetTexture(shaderParam.name);
                    guiElem.TextureRef = texture;
                    break;
            }
        }

        /// <inheritdoc/>
        internal override void Destroy()
        {
            guiElem.Destroy();
        }
    }

    /** @} */
}
