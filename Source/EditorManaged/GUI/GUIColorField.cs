//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using System;
using System.Runtime.CompilerServices;
using bs;

namespace bs.Editor
{
    public partial class GUIColorField
    {
        /// <summary>
        /// Triggered when the color in the field changes.
        /// </summary>
        public event Action<Color> OnChanged;

        /// <summary>
        /// Triggered when the user closes the color picker window.
        /// </summary>
        /// <param name="selected">True if the user confirms color selection, false if he cancels.</param>
        /// <param name="color">Newly selected color.</param>
        private void ColorPickerClosed(bool selected, Color color)
        {
            if (!selected)
                return;

            if (Value != color)
            {
                Value = color;
                OnChanged?.Invoke(color);
            }
        }

        partial void Callback_OnClicked()
        {
            ColorPicker.Show(Value, AllowHDR, ColorPickerClosed);
        }
    }
}
