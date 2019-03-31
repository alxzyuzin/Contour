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

namespace Contour
{
 
    public sealed partial class LayerDisplayParams : UserControl
    {

        public event TappedEventHandler OutlineButtonTapped;
        public event RoutedEventHandler ShapeSwitchToggled;
        public event RoutedEventHandler ContourSwitchToggled;
        

        private void NotifyOutlineButtonTapped(TappedRoutedEventArgs args)
        {
            if (OutlineButtonTapped != null)
                OutlineButtonTapped(this, args);
        }

        private void NotifyShapeSwitchToggled(RoutedEventArgs args)
        {
            if (ShapeSwitchToggled != null)
                ShapeSwitchToggled(this, args);
        }

        private void NotifyContourSwitchToggled(RoutedEventArgs args)
        {
            if (ContourSwitchToggled != null)
                ContourSwitchToggled(this, args);
        }


        private byte _color;
        public byte Color
        {
            get { return _color; }
        }

        private bool _displayShapes = false;
        public bool DisplayShapes
        {
            get { return _displayShapes; }
        }

        private bool _displayContours;
        public bool DisplayContours
        {
            get { return _displayContours; }
        }

        public string LayerName
        {
            set { tbl_Layer.Text = value; }
        }

        public LayerDisplayParams()
        {
            this.InitializeComponent();
        }

        public LayerDisplayParams(byte color)
        {
            this.InitializeComponent();
            _color = color;
            tbl_Layer.Text = string.Format("Layer {0}", color.ToString());
        }
        
        private void btn_Outline_Tapped(object sender, TappedRoutedEventArgs e)
        {
            NotifyOutlineButtonTapped(e);
        }

        private void tgs_Shape_Toggled(object sender, RoutedEventArgs e)
        {
            _displayShapes = ((ToggleSwitch)sender).IsOn;
            NotifyShapeSwitchToggled(e);


        }

        private void tgs_Contour_Toggled(object sender, RoutedEventArgs e)
        {
            _displayContours = ((ToggleSwitch)sender).IsOn;
            NotifyContourSwitchToggled(e);
        }
    }
}
