/*---------------------------------------------------------------------------------
 * Copyright(c) 2023 Alexandr Ziuzin.
 *
 * This file is part of Contour project.
 *
 * This class presents single palette item
 *
 ---------------------------------------------------------------------------------*/

using System.ComponentModel;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Input;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

namespace ContourUI
{
    public sealed partial class PaletteItem : UserControl, INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;
        public PaletteItem()
        {
            this.InitializeComponent();
        }

        public PaletteItem(uint color) : this()
        {
            _color = color;
            IntColor currentColor = new IntColor(color);

            Windows.UI.Color itemColor = new Windows.UI.Color();
            itemColor.R = currentColor.Red;
            itemColor.G = currentColor.Green;
            itemColor.B = currentColor.Blue;
            itemColor.A = currentColor.Alfa;

            ControlGrid.Background = new Windows.UI.Xaml.Media.SolidColorBrush(itemColor);
            ControlCheckBox.IsChecked = true;
            _isChecked = true;
        }

        uint _color = 0xFF000000;
        bool _isChecked = true;

        public bool IsChecked
        {
            get => _isChecked;
            set
            {
                if (_isChecked != value)
                {
                    _isChecked = value;
                    ControlCheckBox.IsChecked = value;
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(IsChecked)));
                }
            }
        }

        public void SetIsChecked(bool value)
        {
            _isChecked = value;
            ControlCheckBox.IsChecked = value;
        }
        public uint Color
        {
            get => _color;

        }
        private void ControlGrig_Tapped(object sender, TappedRoutedEventArgs e)
        {
            if (this.IsChecked)
            {
                ControlCheckBox.IsChecked = false;
                IsChecked = false;
            }
            else
            {
                ControlCheckBox.IsChecked = true; ;
                IsChecked = true; ;
            }



        }
    }
}
