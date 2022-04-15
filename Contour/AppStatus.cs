
using System.ComponentModel;

namespace ContourUI
{
    class AppStatus: INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;
        
        private bool _imageLoaded = false;
        public bool ImageLoaded
        {
            get { return _imageLoaded; }
            set
            {
                if (_imageLoaded != value)
                {
                    _imageLoaded = value;
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(ImageLoaded)));
                }
            }
        }

        private bool _imageConverted = false;
        public bool ImageConverted
        {
            get { return _imageConverted; }
            set
            {
                if (_imageConverted != value)
                {
                    _imageConverted = value;
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(ImageConverted)));
                }
            }
        }

        private bool _imageOutlined = false;
        public bool ImageOutlined
        {
            get { return _imageOutlined; }
            set
            {
                if (_imageOutlined != value)
                {
                    _imageOutlined = value;
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(ImageOutlined)));
                }
            }
        }
        private bool _displayImage = false;
        public bool DisplayImage
        {
            get { return _displayImage; }
            set
            {
                if (_displayImage != value)
                {
                    _displayImage = value;
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(DisplayImage)));
                }
            }
        }

        public bool _displayConverted = false;
        public bool DisplayConverted
        {
            get { return _displayConverted; }
            set
            {
                if (_displayConverted != value)
                {
                    _displayConverted = value;
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(DisplayConverted)));
                }
            }
        }

        public bool _displayContour = false;
        public bool DisplayContour
        {
            get { return _displayContour; }
            set
            {
                if (_displayContour != value)
                {
                    _displayContour = value;
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(DisplayContour)));
                }
            }
        }

        private string _imageFileName = string.Empty;
        public string ImageFileName
        {
            get => _imageFileName;
            set
            {
                if (_imageFileName != value)
                {
                    _imageFileName = value;
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(ImageFileName)));
                }

            }
        }

        public void Reset()
        {
            ImageLoaded = false;
            ImageConverted = false;
            ImageOutlined = false;
            DisplayConverted = true;
            DisplayImage = false;
            DisplayContour = false;
        }
    }
}
