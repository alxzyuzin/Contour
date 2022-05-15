
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
        private bool _hideImage = false;
        public bool HideImage
        {
            get { return _hideImage; }
            set
            {
                if (_hideImage != value)
                {
                    _hideImage = value;
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(HideImage)));
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

        private int _numberOfContours = 0;
        public int NumberOfContours
        {
            get => _numberOfContours;
            set
            {
                if (_numberOfContours != value)
                {
                    _numberOfContours = value;
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(NumberOfContours)));
                }

            }
        }


        private int _numberOfLevels = 0;
        public int NumberOfLevels
        {
            get => _numberOfLevels;
            set
            {
                if (_numberOfLevels != value)
                {
                    _numberOfLevels = value;
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(NumberOfLevels)));
                }

            }
        }

        private int _progressValue = 0;
        public int ProgressValue
        {
            get => _progressValue;
            set
            {
                if (_progressValue != value)
                {
                    _progressValue = value;
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(ProgressValue)));
                }

            }
        }

        public void Reset()
        {
            ImageLoaded = false;
            ImageConverted = false;
            ImageOutlined = false;
            DisplayConverted = false;
            HideImage = false;
            DisplayContour = false;
        }
    }
}
