﻿using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Input;


// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

namespace ContourUI
{
    public enum MsgBoxType : byte
    {
        Error,
        Info,
        Warning
    }


    [Flags]
    public enum MsgBoxButton
    {
        OK = 1,
        Close = 2,
        Cancel = 4,
        Continue = 8,
        Retry = 16,
        GoToSettings = 32,
        Reconnect = 64,
        CloseApplication = 128,
    }

    public sealed partial class MessageBox : UserControl
    {
        private Dictionary<MsgBoxButton, string> MsgBoxButtonsNames = new Dictionary<MsgBoxButton, string>
        {
            { MsgBoxButton.OK, "OK" },
            { MsgBoxButton.Close, "Close" },
            { MsgBoxButton.Cancel, "Cancel" },
            { MsgBoxButton.Continue, "Continue" },
            { MsgBoxButton.Retry, "Retry" },
            { MsgBoxButton.GoToSettings, "Go to 'Settings'"},
            { MsgBoxButton.Reconnect, "Reconnect" },
            { MsgBoxButton.CloseApplication, "Close application" } ,
        };

        private bool _stopWating;
        private int _width;
        private int _height;

        private MsgBoxButton _pressedButton = MsgBoxButton.Close;
        public MsgBoxButton PressedButton
        {
            get { return _pressedButton; }
        }

        #region Constructors
        public MessageBox()
        {
            this.InitializeComponent();
        }

        public MessageBox(int height) : this()
        {
            _height = height;

            GridMain.RowDefinitions[1].Height = new GridLength(_height);
        }

        public MessageBox(int width, int height) : this()
        {
            _width = width;
            _height = height;

            GridMain.ColumnDefinitions[1].Width = new GridLength(_width);
            GridMain.RowDefinitions[1].Height = new GridLength(_height);
        }
        #endregion Constructors

        #region Properties
        public string Message
        {
            get { return (string)GetValue(MessageProperty); }
            set { SetValue(MessageProperty, value); }
        }

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

        public static readonly DependencyProperty MessageProperty =
            DependencyProperty.Register("Message", typeof(string), typeof(MessageBox), null);

        public static readonly DependencyProperty InnerBoxHeightProperty =
            DependencyProperty.Register("BoxHeight", typeof(double), typeof(MessageBox), null);

        public static readonly DependencyProperty InnerBoxWidthProperty =
                    DependencyProperty.Register("BoxWidth", typeof(double), typeof(MessageBox), null);

        #endregion Properties

        #region Methods
        public async Task<MsgBoxButton> Show()
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
            return _pressedButton;
        }

        private void OnMainpage_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            Grid parent = (Grid)Parent;
            this.Width = parent.ActualWidth;
            this.Height = parent.ActualHeight;
            GridMain.ColumnDefinitions[1].Width = new GridLength(InnerBoxWidth);
            GridMain.ColumnDefinitions[0].Width = new GridLength((parent.ActualWidth - InnerBoxWidth) / 2);
        }

        private void button_Tapped(object sender, TappedRoutedEventArgs e)
        {
            _pressedButton = (MsgBoxButton)Enum.Parse(typeof(MsgBoxButton), ((Button)sender).Name);
            HideControl.Begin();
            _stopWating = true;
        }

        public void SetButtons(MsgBoxButton buttons)
        {
            ButtonsPanel.Children.Clear();

            foreach (MsgBoxButton b in (MsgBoxButton[])Enum.GetValues(typeof(MsgBoxButton)))
            {
                if (buttons.HasFlag(b))
                {
                    Button newButton = new Button
                    {
                        Name = b.ToString(),
                        Content = MsgBoxButtonsNames[b],
                        Width = Math.Max(MsgBoxButtonsNames[b].Length, 5) * 7 + 50,
                        Style = this.Resources["MsgBoxButtonStyle"] as Style,
                    };
                    newButton.Tapped += button_Tapped;

                    ButtonsPanel.Children.Add(newButton);
                }
            }

        }
        #endregion Methods
    }
}
