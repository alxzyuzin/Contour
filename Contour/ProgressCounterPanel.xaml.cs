using System;
using System.Collections.Generic;
using System.ComponentModel;
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
    public sealed partial class ProgressCounterPanel : UserControl, INotifyPropertyChanged
    {
        public ProgressCounterPanel()
        {
            this.InitializeComponent();
            this.DataContext = this;
        }

        public event PropertyChangedEventHandler PropertyChanged;

        private string _title = string.Empty;
        public string Title
        {
            get => _title;

            set
            {
                if (_title != value)
                {
                    _title = value;
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(Title)));
                }
            }
        }

        private int _counter = 0;
        public int CounterValue
        {
            get => _counter;

            set
            {
                if (_counter != value)
                {
                    _counter = value;
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(CounterValue)));
                }
            }
        }

        public void Show()
        {
            this.Visibility = Visibility.Visible;
        }


        public void Hide()
        {
            this.Visibility = Visibility.Collapsed;
        }
    }
}
