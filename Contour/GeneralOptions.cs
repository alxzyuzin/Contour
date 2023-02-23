/*---------------------------------------------------------------------------------
 * Copyright(c) 2023 Alexandr Ziuzin.
 *
 * This file is part of Contour project.
 *
 * This class presents conversion options
 *
 ---------------------------------------------------------------------------------*/


using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using Windows.Storage;
using ContourExtractorWindowsRuntimeComponent;

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

        private List<string> _convertionTypesList = new List<string>();
        public List<string> ConvertionTypesList { get => _convertionTypesList;  }

        private byte[] _arrNumberOfColorsList = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
        public byte[] NumberOfColorsList
        {
            get => _arrNumberOfColorsList;
        }

        private int[] _arrcbxCleanupValues = { 3, 4, 5, 6, 7, 8, 9, 10, 11 };
        public int[] cbxCleanupValuesList { get => _arrcbxCleanupValues; }

        private List<string> _contourColorList = new List<string>();
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
                        NumberOfColors = 8;
                    }
                    else
                    {
                        _arrNumberOfColorsList = new byte[] { 2, 4, 8, 16, 32, 64, 128 };
                        NumberOfColors = 32;
                    }

                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(ConversionTypeName)));

                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(NumberOfColorsList)));

                    
                }
            }
        }

        public TypeOfConvertion ConversionType { get => ConvertionTypeItems[_conversionTypeName]; }

        private byte _numberOfColors;
        public byte NumberOfColors
        { 
            get =>_numberOfColors;
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
            get =>_cleanupValue;
            set
            {
                if (_cleanupValue != value)
                {
                    _cleanupValue = value;
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(CleanupValue)));
                }
            }
        }

        private int _maxContourLength;
        public int MaxContourLength
        {
            get => _maxContourLength; 
            set
            {
                if (_maxContourLength != value)
                {
                    _maxContourLength = value;
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(MaxContourLength)));
                }
            }
        }

        private string _contourColorName;
        public string ContourColorName
        {
            get =>_contourColorName;
            set
            {
                if (_contourColorName != value)
                {
                    _contourColorName = value;
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(ContourColorName)));
                }
            }
        }
        public ContourColors ContourColorValue { get => ContourColorItems[_contourColorName]; }

        public bool SaveData { get; set; }
        

     

        public void Save()
        {
            ApplicationDataContainer LocalSettings = ApplicationData.Current.LocalSettings;

            LocalSettings.Values["ConversionTypeName"] = ConversionTypeName;
            LocalSettings.Values["NumberOfColors"] = NumberOfColors;
            LocalSettings.Values["CleanupValue"] = CleanupValue;
            LocalSettings.Values["MaxContourLength"] = MaxContourLength;
            LocalSettings.Values["ContourColorName"] = ContourColorName;
           
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

            value = LocalSettings.Values["MaxContourLength"];
            MaxContourLength = (value != null) ? (int)value : 10;

            value = LocalSettings.Values["ContourColorName"];
            ContourColorName = value != null ? (string)value : "Red";

            
        }

    }
}
