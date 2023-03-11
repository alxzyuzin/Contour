/*---------------------------------------------------------------------------------
 * Copyright(c) 2023 Alexandr Ziuzin.
 *
 * This file is part of Contour project.
 *
 * This class presents window for setting conversion options
 *
 ---------------------------------------------------------------------------------*/


using System;
using System.Threading.Tasks;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;


// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

namespace ContourUI
{

    public sealed partial class OptionsWindow : UserControl
    {
        //private string[] arrConversionTypesNames = { "Grayscale", "Reduced colors" };



        private bool _stopWating;
        private readonly int _width;
        private readonly int _height;

        #region Constructors
        public OptionsWindow()
        {
            this.InitializeComponent();
        }

        public OptionsWindow(int height) : this()
        {
            _height = height;
            GridMain.RowDefinitions[1].Height = new GridLength(_height);
        }

        public OptionsWindow(int width, int height) : this()
        {
            _width = width;
            _height = height;

            GridMain.ColumnDefinitions[1].Width = new GridLength(_width);
            GridMain.RowDefinitions[1].Height = new GridLength(_height);
        }
        #endregion Constructors

        #region Properties

        //public int ConversionTypeIndex { get; set; }
        //public int NumberOfColorIndex { get; set; }
        //public int CleanupValueIndex { get; set; }
        //public int MaxContourLenghtValue { get; set; }
        //public int ContourColorIndex { get; set; }

        //private TypeOfConvertion _ConvertionType;
        //public TypeOfConvertion ConvertionType { get => _ConvertionType; }

        //private int _NumberOfColors;
        //public int NumberOfColors { get => _NumberOfColors; }


        //public int CleanupValue { public get; set }
        //MaxContourLenghtValue
        //ContourColor


        #endregion Properties

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

        private void btnSave_Click(object sender, RoutedEventArgs e)
        {
            HideControl.Begin();
            _stopWating = true;
            ((GeneralOptions)this.DataContext).Save();
        }

        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            HideControl.Begin();
            _stopWating = true;
            ((GeneralOptions)this.DataContext).Restore();
        }
        #endregion Methods
        /// <summary>
        ///  Set default number of colors for conversion depends of conversion mode
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cbxConvType_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            ((GeneralOptions)this.DataContext).NumberOfColors = 8;
            if (((GeneralOptions)this.DataContext).ConversionType == ContourExtractorWindowsRuntimeComponent.TypeOfConvertion.ReducedColors)
                ((GeneralOptions)this.DataContext).NumberOfColors = 16;
        }
    }




}
