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
    public sealed partial class ProgressPanel : UserControl, INotifyPropertyChanged
    {
        public enum ProgressPanelType { ProgressBar, Counter}

        public event PropertyChangedEventHandler PropertyChanged;
        public ProgressPanel()
        {
            this.InitializeComponent();
            this.DataContext = this;
            this.Visibility = Visibility.Collapsed;
        }

        private float _progress = 0;
        public float ProgressValue
        {
            get => _progress;
            
            set
            {
               if (_progress != value)
                {
                    _progress = value;
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(ProgressValue)));
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


        public void Show(ProgressPanelType progressPanelType)
        {
            switch (progressPanelType) 
            {
                case ProgressPanelType.ProgressBar:
                    ActionProgress.Visibility = Visibility.Visible; break;
                case ProgressPanelType.Counter:
                    ActionCounter.Visibility = Visibility.Visible;   break;
            }
            this.Visibility = Visibility.Visible;   
        }


        public void Hide()
        {
            ActionProgress.Visibility = Visibility.Collapsed;
            ActionCounter.Visibility = Visibility.Collapsed;
            this.Visibility = Visibility.Collapsed;
        }
    }

   
}
