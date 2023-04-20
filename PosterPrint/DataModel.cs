using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PosterPrint
{
    internal class DataModel : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;
    
        private string _imageFileName;
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

        private string _imageFileNameExtention;
        public string ImageFileNameExtention
        {
            get => _imageFileNameExtention;

            set
            {
                if (_imageFileNameExtention != value)
                {
                    _imageFileNameExtention = value;
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(ImageFileNameExtention)));

                }
            }
        }
        private string _imageFilePath;
        public string ImageFilePath
        {
            get => _imageFilePath;

            set
            {
                if (_imageFilePath != value)
                {
                    _imageFilePath = value;
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(ImageFilePath)));

                }
            }
        }
    }
}
