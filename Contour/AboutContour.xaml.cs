using System;
using System.Threading.Tasks;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

namespace ContourUI
{
    public sealed partial class AboutContour : UserControl
    {
        private bool _stopWating;

        public AboutContour()
        {
            this.InitializeComponent();
        }



        #region DependencyProperties
        public double InnerBoxHeight
        {
            get { return (double)GetValue(InnerBoxHeightProperty); }
            set { SetValue(InnerBoxHeightProperty, value); }
        }

        public double InnerBoxWidth
        {
            get { return (double)GetValue(InnerBoxWidthProperty); }
            set { SetValue(InnerBoxWidthProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Message. 
        //This enables animation, styling, binding, etc...

        public static readonly DependencyProperty InnerBoxHeightProperty =
            DependencyProperty.Register("BoxHeight", typeof(double), typeof(MessageBox), null);

        public static readonly DependencyProperty InnerBoxWidthProperty =
                    DependencyProperty.Register("BoxWidth", typeof(double), typeof(MessageBox), null);

        #endregion DependencyProperties

        #region Methods
        public async Task Show()
        {
            ((Grid)this.Parent).SizeChanged += OnMainpage_SizeChanged;

            GridMain.RowDefinitions[1].Height = new GridLength(InnerBoxHeight);
            GridMain.ColumnDefinitions[1].Width = new GridLength(InnerBoxWidth);
            GridMain.ColumnDefinitions[0].Width = new GridLength((((Grid)this.Parent).ActualWidth - InnerBoxWidth) / 2);
            this.Visibility = Visibility.Visible;

            ShowControl.Begin();
            _stopWating = false;
            do        // В цикле с интервалом 100 миллисекунд проверяем статус процесса
                      //поиска пока он не закончится
            {
                await Task.Delay(TimeSpan.FromMilliseconds(100));
            }
            while (!_stopWating);

            this.Visibility = Visibility.Collapsed;
            return;
        }

        private void OnMainpage_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            Grid parent = (Grid)Parent;
            this.Width = parent.ActualWidth;
            this.Height = parent.ActualHeight;
            GridMain.ColumnDefinitions[1].Width = new GridLength(InnerBoxWidth);
            GridMain.ColumnDefinitions[0].Width = new GridLength((parent.ActualWidth - InnerBoxWidth) / 2);
        }
        private void Button_Click(object sender, RoutedEventArgs e)
        {
            HideControl.Begin();
            _stopWating = true;

        }
        #endregion Methods
    }
}
