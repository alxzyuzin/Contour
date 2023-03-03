/*---------------------------------------------------------------------------------
 * Copyright(c) 2023 Alexandr Ziuzin.
 * e-mail alx.zyuzin@gmail.com
 * This file is part of Contour project.
 *
 * This class presents main application window
 *
 ---------------------------------------------------------------------------------*/

using System;
using Windows.Storage;
using Windows.Storage.FileProperties;
using Windows.Storage.Pickers;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Input;
using System.Runtime.InteropServices.WindowsRuntime;

using ContourExtractorWindowsRuntimeComponent;
using Windows.Storage.Streams;
using System.Collections.Generic;
using Windows.Graphics.Imaging;
using System.IO;
using Windows.UI.Xaml;
using System.Threading.Tasks;


namespace ContourUI
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page //, INotifyPropertyChanged
    {
        private GeneralOptions Options = new GeneralOptions();
        AppStatus ApplicationStatus = new AppStatus();
        private ContourBitmap bitmap = null;
        
        private string _imageFileNameExtention;
        private string _imageFilePath;
        
        public MainPage()
        {
            InitializeComponent();
            OptionsWindow.DataContext = Options;
            Options.Restore();
            gridMain.DataContext = ApplicationStatus;
            ApplicationStatus.PropertyChanged += ApplicationStatus_PropertyChanged;
            ApplicationStatus.ProgressValue = 0;
            ApplicationStatus.ProgressBarVisibility = Visibility.Collapsed;
            ApplicationStatus.NumberOfLevels = Options.NumberOfColors;
            ApplicationStatus.ConversionMode = Options.ConversionTypeName + ".";

            Palette.PropertyChanged += Palette_PropertyChanged;

        }

        private void Palette_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            SortedDictionary<uint, bool> colors = ((PalettePanel)sender).ActiveColors;
        }

        private void ApplicationStatus_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "HideImage" || e.PropertyName == "DisplayConverted" || e.PropertyName == "DisplayContour")
            {
                // Disable Display Converted/Original toggle switch if image hided 
                ApplicationStatus.ImageConverted = !ApplicationStatus.HideImage;
                    
                bitmap.DisplayAll(ApplicationStatus.HideImage, ApplicationStatus.DisplayConverted,
                                    ApplicationStatus.DisplayContour, Options.ContourColorValue);
                bitmap.ImageData.Invalidate();
            }
        }

        private async Task<MsgBoxButton> DisplayMessage(UserMessage message)
        {
            MsgBox.SetButtons(message.Buttons);
            MsgBox.Message = message.Text;
            MsgBox.InnerBoxHeight = message.BoxHeight;
            MsgBox.InnerBoxWidth = message.BoxWidth;
            return await MsgBox.Show();
        }

        private async void FunctionNotImplementerMessage()
        {
            UserMessage msg = new UserMessage(MsgBoxType.Info, "Function not imlemented");
            MsgBoxButton mbb = await DisplayMessage(msg);
        }

        /// <summary>
        /// Load JPG or BMP image to bitmap object
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private async void mfiOpen_Clicked(object sender, RoutedEventArgs e)
        {
            FileOpenPicker openPicker = new FileOpenPicker
            {
                SuggestedStartLocation = PickerLocationId.PicturesLibrary,
            };
            openPicker.FileTypeFilter.Add(".jpg");
            openPicker.FileTypeFilter.Add(".bmp");

            StorageFile file = await openPicker.PickSingleFileAsync();

            if (file != null)
            {
                
                ApplicationStatus.Reset();

                ApplicationStatus.ImageFileName = file.Name;
                _imageFilePath = file.Path.Replace(ApplicationStatus.ImageFileName,"");
                _imageFileNameExtention = file.FileType;
                
                ImageProperties props = await file.Properties.GetImagePropertiesAsync();

                bitmap = new ContourBitmap((int)props.Width, (int)props.Height);

                IRandomAccessStream stream = await file.OpenAsync(FileAccessMode.Read);
                bitmap.SetSource(stream);
                ExposedImage.Source = bitmap.ImageData;
                ApplicationStatus.ImageLoaded = true;

                
            }
            else
            {
                //ImageFileName = "No loaded files.";
            }
        }

        private async void mfiSave_Clicked(object sender, RoutedEventArgs e)
        {
            if (bitmap == null) return;
            StorageFolder folder = await StorageFolder.GetFolderFromPathAsync(_imageFilePath);
            StorageFile file = await folder.CreateFileAsync(ApplicationStatus.ImageFileName, CreationCollisionOption.ReplaceExisting);

            IRandomAccessStream stream = await file.OpenAsync(FileAccessMode.ReadWrite);
            Guid EncoderID = GetEncoderIDFromFileType(_imageFileNameExtention);
            BitmapEncoder encoder = await BitmapEncoder.CreateAsync(EncoderID, stream);
            // Get pixels of the WriteableBitmap object 
            Stream pixelStream = bitmap.ImageData.PixelBuffer.AsStream();
            byte[] pixels = new byte[pixelStream.Length];
            await pixelStream.ReadAsync(pixels, 0, pixels.Length);
            encoder.SetPixelData(BitmapPixelFormat.Bgra8, BitmapAlphaMode.Ignore, (uint)bitmap.ImageData.PixelWidth, (uint)bitmap.ImageData.PixelHeight, 96.0, 96.0, pixels);
            await encoder.FlushAsync();
        }

        private Guid GetEncoderIDFromFileType(string filetype)
        {
            switch (filetype)
            {
                case ".bmp": return BitmapEncoder.BmpEncoderId;
                case ".jpg": return BitmapEncoder.JpegEncoderId;

            }
            return BitmapEncoder.JpegEncoderId;

        }

        private async void mfiSaveAs_Clicked(object sender, RoutedEventArgs e)
        {
            if (bitmap == null) return;
            FileSavePicker picker = new FileSavePicker();
            picker.FileTypeChoices.Add("bmp File", new List<string>() { ".bmp" });
            picker.FileTypeChoices.Add("jpg File", new List<string>() { ".jpg" });
            StorageFile file = await picker.PickSaveFileAsync();
            if (file == null)
                return;
            IRandomAccessStream stream = await file.OpenAsync(FileAccessMode.ReadWrite);

            Guid EncoderID = GetEncoderIDFromFileType(file.FileType);
            BitmapEncoder encoder = await BitmapEncoder.CreateAsync(EncoderID, stream);
            // Get pixels of the WriteableBitmap object 
            Stream pixelStream = bitmap.ImageData.PixelBuffer.AsStream();
            byte[] pixels = new byte[pixelStream.Length];
            await pixelStream.ReadAsync(pixels, 0, pixels.Length);
            encoder.SetPixelData(BitmapPixelFormat.Bgra8, BitmapAlphaMode.Ignore, (uint)bitmap.ImageData.PixelWidth, (uint)bitmap.ImageData.PixelHeight, 96.0, 96.0, pixels);
            await encoder.FlushAsync();
        }

        private async void mfiPrint_Clicked(object sender, RoutedEventArgs e)
        {
            UserMessage message = new UserMessage()
            {
                Type = MsgBoxType.Error,
                Text = "Function not implemented.",
                BoxWidth = 350,
                BoxHeight = 150
            };
            await DisplayMessage(message);
        }

        private void mfiExit_Clicked(object sender, RoutedEventArgs e)
        {
            Application.Current.Exit();
        }

        private async void mfiOptions_Clicked(object sender, RoutedEventArgs e)
        {
            Options.Restore();
            await OptionsWindow.Show();
            ApplicationStatus.NumberOfLevels = Options.NumberOfColors;
            ApplicationStatus.ConversionMode = Options.ConversionTypeName + ".";
        }

        private async void mfiConvert_Clicked(object sender, RoutedEventArgs e)
        {
            if (ApplicationStatus.ImageLoaded)
            {
                
                if (Options.ConversionType == TypeOfConvertion.Grayscale)
                {
                    bitmap.ConvertToGrayscale(Options.NumberOfColors);
                 }
                if (Options.ConversionType == TypeOfConvertion.ReducedColors)
                {
                    bitmap.ConvertToReducedColors(Options.NumberOfColors);
                }

                int numberOfLevels = bitmap.ExtractLevels();

                this.Palette.Build2(bitmap.Colors);

                ApplicationStatus.ImageConverted = true;
                ApplicationStatus.DisplayConverted = true;
                bitmap.ImageData.Invalidate();
            }
            else
            {
                UserMessage message = new UserMessage()
                { 
                    Type = MsgBoxType.Error,
                    Text = "Image not loaded.",
                    BoxWidth = 350,
                    BoxHeight = 150
                };
                await DisplayMessage(message);
            }
        }

        private async void mfiClean_Clicked(object sender, RoutedEventArgs e)
        {
            UserMessage message = new UserMessage()
            {
                Type = MsgBoxType.Error,
                Text = "Function not implemented.",
                BoxWidth = 350,
                BoxHeight = 150

            };
            await DisplayMessage(message);
        }
        
        private async  void mfiOutline_Clicked(object sender, RoutedEventArgs e)
        {
            
            if (ApplicationStatus.ImageConverted)
            {
                ApplicationStatus.ProgressValue = 0;
                ApplicationStatus.ProgressBarVisibility = Visibility.Visible;
                
                int numberOfLevels = bitmap.LevelsCount;

                ApplicationStatus.NumberOfLevels = numberOfLevels;

                List<Windows.Foundation.IAsyncOperation<int>> taskList = new List<Windows.Foundation.IAsyncOperation<int>>();
                var starttime = DateTime.Now;
                for (int levelNumber = 0; levelNumber < numberOfLevels; levelNumber++)
                {
                    taskList.Add(bitmap.FindLevelContoursAsync(levelNumber));
                }

                foreach (var task in taskList)
                {
                    await task;
                    ApplicationStatus.ProgressValue++;
                }

                DateTime endtime = DateTime.Now;
                TimeSpan lll = endtime - starttime;
                ApplicationStatus.ProgressBarVisibility = Visibility.Collapsed;
                ApplicationStatus.ImageOutlined = true;
                ApplicationStatus.DisplayContour = true;
            }
        }
       
        private async void mfiAbout_Clicked(object sender, RoutedEventArgs e)
        {
            await AboutWindow.Show();
        }

    } // End of MainPage class definition

}
