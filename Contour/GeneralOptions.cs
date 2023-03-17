/*---------------------------------------------------------------------------------
 * Copyright(c) 2023 Alexandr Ziuzin.
 *
 * This file is part of Contour project.
 *
 * This class presents conversion options
 *
 ---------------------------------------------------------------------------------*/


using ContourExtractorWindowsRuntimeComponent;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using Windows.Storage;
using static System.Net.Mime.MediaTypeNames;
using Windows.UI.Xaml.Controls;

namespace ContourUI
{


    class GeneralOptions : INotifyPropertyChanged
    {
        private readonly Dictionary<string, TypeOfConvertion> ConvertionTypeItems = new Dictionary<string, TypeOfConvertion>
            {
              { "Grayscale", TypeOfConvertion.Grayscale },
              { "Reduced colors", TypeOfConvertion.ReducedColors }
            };


        private readonly Dictionary<string, ContourExtractorWindowsRuntimeComponent.ContourColors> ContourColorItems = new Dictionary<string, ContourColors>
        {
            {"Black", ContourColors.Black},
            {"White", ContourColors.White },
            {"Red",   ContourColors.Red },
            {"Green", ContourColors.Green },
            {"Blue",  ContourColors.Blue}
        };
        public event PropertyChangedEventHandler PropertyChanged;

        public GeneralOptions()
        {
            foreach (KeyValuePair<string, TypeOfConvertion> di in ConvertionTypeItems)
                _convertionTypesList.Add(di.Key);

            foreach (KeyValuePair<string, ContourColors> di in ContourColorItems)
                _contourColorList.Add(di.Key);

            ContourColorName = "Red";
        }



        #region Properties for ComboBox DataSource
        //private string[] arrConversionTypesNames = { "Grayscale", "Reduced colors" };

        private readonly List<string> _convertionTypesList = new List<string>();
        public List<string> ConvertionTypesList { get => _convertionTypesList; }

        private byte[] _arrNumberOfColorsList = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

        private readonly byte[] _contourDensityList = { 1, 2, 4, 8, 16, 32, 64, 128, 255 };
            
        public byte[] ContourDensityList => _contourDensityList;
        
        public byte[] NumberOfColorsList => _arrNumberOfColorsList;


        //private readonly int[] _arrcbxCleanupValues = { 3, 4, 5, 6, 7, 8, 9, 10, 11 };
        private readonly int[] _arrcbxCleanupValues = { 1, 2, 4, 8, 16, 32, 64 };
        public int[] cbxCleanupValuesList => _arrcbxCleanupValues; 

        private readonly List<string> _contourColorList = new List<string>();
        public List<string> ContourColorsList { get => _contourColorList; }

        #endregion

        public bool InitialSetupDone { get; set; }

        private string _conversionTypeName;
        public string ConversionTypeName
        {
            get => _conversionTypeName;

            set
            {
                if (_conversionTypeName != value)
                {
                    _conversionTypeName = value;

                    if (_conversionTypeName == "Grayscale")
                    {
                        _arrNumberOfColorsList = new byte[] { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
                        //NumberOfColors = 8;
                    }
                    else
                    {
                        _arrNumberOfColorsList = new byte[] { 2, 4, 8, 16, 32, 64, 128 };
                        //NumberOfColors = 32;
                    }

                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(ConversionTypeName)));

                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(NumberOfColorsList)));


                }
            }
        }

        public TypeOfConvertion ConversionType => ConvertionTypeItems[_conversionTypeName]; 

        private byte _numberOfColors;
        public byte NumberOfColors
        {
            get => _numberOfColors;
            set
            {
                if (_numberOfColors != value)
                {
                    _numberOfColors = value;
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(NumberOfColors)));
                }
            }

        }

        private int _cleanupValue;
        public int CleanupValue
        {
            get => _cleanupValue;
            set
            {
                if (_cleanupValue != value)
                {
                    _cleanupValue = value;
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(CleanupValue)));
                }
            }
        }

        //private int _maxContourLength;
        //public int MaxContourLength
        //{
        //    get => _maxContourLength;
        //    set
        //    {
        //        if (_maxContourLength != value)
        //        {
        //            _maxContourLength = value;
        //            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(MaxContourLength)));
        //        }
        //    }
        //}

        private string _contourColorName;
        public string ContourColorName
        {
            get => _contourColorName;
            set
            {
                if (_contourColorName != value)
                {
                    _contourColorName = value;
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(ContourColorName)));
                }
            }
        }


      
        private byte _contourDensityValue = 255;
        public byte ContourDensityValue
        {
            get => _contourDensityValue;
            set
            {
                if (_contourDensityValue != value)
                {
                    _contourDensityValue = value;
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(ContourDensityValue)));
                }
            }
        }

        
        private int _minContourLength = 0;
        public int MinContourLength
        {
            get => _minContourLength;
            set
            {
                if (_minContourLength != value)
                {
                    _minContourLength = value;
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(MinContourLength)));
                }
            }
        }
       
        public ContourColors ContourColorValue { get => ContourColorItems[_contourColorName]; }

        private double _timeSpended;
        public double TimeSpended
        {
            get => _timeSpended;
            set
            {
                if (_timeSpended != value)
                {
                    _timeSpended = value;
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(TimeSpended)));
                }
            }
        }

       
        public bool SaveData { get; set; }




        public void Save()
        {
            ApplicationDataContainer LocalSettings = ApplicationData.Current.LocalSettings;

            LocalSettings.Values["ConversionTypeName"] = ConversionTypeName;
            LocalSettings.Values["NumberOfColors"] = NumberOfColors;
            LocalSettings.Values["CleanupValue"] = CleanupValue;
            LocalSettings.Values["MinContourLength"] = MinContourLength;
            LocalSettings.Values["ContourColorName"] = ContourColorName;
            LocalSettings.Values["ContourDensityValue"] = ContourDensityValue;
                   
        }

        public void Restore()
        {
            Object value;

            ApplicationDataContainer LocalSettings = ApplicationData.Current.LocalSettings;

            value = LocalSettings.Values["ConversionTypeName"];
            ConversionTypeName = (value != null) ? (string)value : "Grayscale";

            value = LocalSettings.Values["NumberOfColors"];
            NumberOfColors = (value != null) ? (byte)value : (byte)6;

            value = LocalSettings.Values["CleanupValue"];
            CleanupValue = (value != null) ? (int)value : 1;

            value = LocalSettings.Values["MinContourLength"];
            MinContourLength = (value != null) ? (int)value : 10;

            value = LocalSettings.Values["ContourColorName"];
            ContourColorName = value != null ? (string)value : "Red";

            value = LocalSettings.Values["ContourDensityValue"];
            ContourDensityValue = value != null ? (byte)value : (byte)0xFF;
             
        }

    }
}
