﻿using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using Windows.Storage;
using ContourExtractorWindowsRuntimeComponent;

namespace ContourUI
{
    public enum TypeOfConvertion { Grayscale = 0, ReducedColors = 1 }
    
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

        private List<string> _convertionTypesList = new List<string>();
        public List<string> ConvertionTypesList { get => _convertionTypesList;  }

        private int[] _arrNumberOfColors = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
        public int[] NumberOfColorsList { get => _arrNumberOfColors;  }

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
                    PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(ConversionTypeName)));
                }
            }
        }

        public TypeOfConvertion ConversionType { get => ConvertionTypeItems[_conversionTypeName]; }

        private int _numberOfColors;
        public int NumberOfColors
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
            NumberOfColors = (value != null) ? (int)value : 6; 

            value = LocalSettings.Values["CleanupValue"];
            CleanupValue = (value != null) ? (int)value : 1; 

            value = LocalSettings.Values["MaxContourLength"];
            MaxContourLength = (value != null) ? (int)value : 10;

            value = LocalSettings.Values["ContourColorName"];
            ContourColorName = value != null ? (string)value : "Red";

            
        }

    }
}
