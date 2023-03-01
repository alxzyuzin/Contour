using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

namespace ContourUI
{
    public sealed partial class PaletteItem : UserControl
    {
        public PaletteItem()
        {
            this.InitializeComponent();
        }

        public PaletteItem(uint color) : this()
        {
            IntColor currentColor = new IntColor(color);

            Windows.UI.Color itemColor = new Windows.UI.Color();
            itemColor.R = currentColor.Red;
            itemColor.G = currentColor.Green;
            itemColor.B = currentColor.Blue;
            itemColor.A = currentColor.Alfa;
           
            ControlGrid.Background = new Windows.UI.Xaml.Media.SolidColorBrush( itemColor);
            
        }

        bool _isChecked = false;
        bool IsChecked
        {
            get =>  _isChecked;
            set => _isChecked = value;
        }
        private void ControlGrig_Tapped(object sender, TappedRoutedEventArgs e)
        {
            //if (IsChecked)
            //    ControlCheckBox.IsChecked = IsChecked = false;
            //else
            //   ControlCheckBox.IsChecked = IsChecked = true;
            ControlCheckBox.IsChecked = this.IsChecked = this.IsChecked ? false : true;
        }
    }
}
