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

// The Content Dialog item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

namespace ContourUI
{
    public sealed partial class OperationProgressContentDialog : ContentDialog, INotifyPropertyChanged
    {
        public OperationProgressContentDialog()
        {
            this.InitializeComponent();
            this.DataContext = this;
        }

        private string _operationName = string.Empty;
        public string OperationName
        {
            get => _operationName;

            set
            {
                if (_operationName != value)
                {
                    _operationName = value;
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(OperationName)));
                }
            }
        }

        private double _progress = 0;
        public double ProgressValue
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

        public event PropertyChangedEventHandler PropertyChanged;

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

        //public ProgressPanel.ProgressPanelType _progressType;
        //public ProgressPanel.ProgressPanelType ProgressType
        //{
        //    get => _progressType;

        //    set
        //    {
        //        if (_progressType != value)
        //        {
        //            _progressType = value;
        //            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(ProgressType)));
        //        }
        //    }
        //}

        private void ContentDialog_PrimaryButtonClick(ContentDialog sender, ContentDialogButtonClickEventArgs args)
        {
        }

        private void ContentDialog_SecondaryButtonClick(ContentDialog sender, ContentDialogButtonClickEventArgs args)
        {
        }
    }
}
