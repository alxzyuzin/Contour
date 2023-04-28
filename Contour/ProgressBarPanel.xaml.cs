using System.ComponentModel;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

// The User Control item template is documented at https://go.microsoft.com/fwlink/?LinkId=234236

namespace ContourUI
{
    public sealed partial class ProgressBarPanel : UserControl, INotifyPropertyChanged
    {
        //public enum ProgressPanelType { ProgressBar, Counter}

        public event PropertyChangedEventHandler PropertyChanged;
        public ProgressBarPanel()
        {
            this.InitializeComponent();
            this.DataContext = this;
            this.Visibility = Visibility.Collapsed;
        }

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
