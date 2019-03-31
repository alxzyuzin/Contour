using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Input;

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
        public byte Color => _color;
        
        private bool _displayShapes = false;
        public bool DisplayShapes => _displayShapes;
        
        private bool _displayContours;
        public bool DisplayContours => _displayContours;
        

        public string LayerName { get; set; }
        

        public LayerDisplayParams()
        {
            this.InitializeComponent();
        }

        public LayerDisplayParams(byte color)
        {
            this.InitializeComponent();
            _color = color;
            LayerName = $"Layer {color.ToString()}";
        }
        
        private void Outline_Tapped(object sender, TappedRoutedEventArgs e)
        {
            NotifyOutlineButtonTapped(e);
        }

        private void Shape_Toggled(object sender, RoutedEventArgs e)
        {
            _displayShapes = ((ToggleSwitch)sender).IsOn;
            NotifyShapeSwitchToggled(e);
        }

        private void Contour_Toggled(object sender, RoutedEventArgs e)
        {
            _displayContours = ((ToggleSwitch)sender).IsOn;
            NotifyContourSwitchToggled(e);
        }
    }
}
