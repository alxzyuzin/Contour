/*---------------------------------------------------------------------------------
 * Copyright(c) 2023 Alexandr Ziuzin.
 * e-mail alx.zyuzin@gmail.com
 * This file is part of Contour project.
 *
 * This class presents main application window
 *
 ---------------------------------------------------------------------------------*/

using ContourExtractorWindowsRuntimeComponent;
using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.Graphics.Imaging;
using Windows.Storage;
using Windows.Storage.FileProperties;
using Windows.Storage.Pickers;
using Windows.Storage.Streams;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Media.Imaging;

namespace ContourUI
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page //, INotifyPropertyChanged
    {
        private readonly GeneralOptions Options = new GeneralOptions();
        readonly AppStatus ApplicationStatus = new AppStatus();
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
            RedrawImageArea();
        }

        private void ApplicationStatus_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            // Disable Display Converted/Original toggle switch if image hided 
            ApplicationStatus.ImageConverted = !ApplicationStatus.HideImage;

            if (e.PropertyName == "HideImage" || e.PropertyName == "DisplayConverted" || e.PropertyName == "DisplayContour")
            {
                RedrawImageArea();
            }
        }

        private void RedrawImageArea()
        {
            bitmap.ClearPixelBuffer();
            if (!ApplicationStatus.HideImage)
            {
                if (ApplicationStatus.DisplayConverted)
                {
                    foreach (var color in Palette.ActiveColors)
                    {
                        if (color.Value)
                            bitmap.SetLevelDataToDisplayBuffer(color.Key);
                    }
                }
                else
                    bitmap.SetOriginalImageDataToDisplayBuffer();
            }
            if (ApplicationStatus.DisplayContour)
                bitmap.DisplayContours(Options.ContourColorValue, Options.MinContourLength, Options.ContourDensityValue);

             bitmap.Invalidate();
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
        private async void MenuFileOpen_Clicked(object sender, RoutedEventArgs e)
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
                _imageFilePath = file.Path.Replace(ApplicationStatus.ImageFileName, "");
                _imageFileNameExtention = file.FileType;

                ImageProperties props = await file.Properties.GetImagePropertiesAsync();

                bitmap = new ContourBitmap((int)props.Width, (int)props.Height);

                IRandomAccessStream stream = await file.OpenAsync(FileAccessMode.Read);
                bitmap.SetSource(stream);
                ExposedImage.Source = bitmap.ImageData;
                //stream.Seek(0);
                //BitmapImage img = new BitmapImage();
                //img.SetSource(stream);
                //ExposedImage.Source = img;

                ApplicationStatus.ImageLoaded = true;
                Palette.Clear();
            }
            else
            {
                //ImageFileName = "No loaded files.";
            }
        }

        private async void MenuFileSave_Clicked(object sender, RoutedEventArgs e)
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
            encoder.SetPixelData(BitmapPixelFormat.Bgra8, BitmapAlphaMode.Ignore, bitmap.Width, bitmap.Height, 96.0, 96.0, pixels);
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

        private async void MenuFileSaveAs_Clicked(object sender, RoutedEventArgs e)
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
            encoder.SetPixelData(BitmapPixelFormat.Bgra8, BitmapAlphaMode.Ignore, bitmap.Width, bitmap.Height, 96.0, 96.0, pixels);

            await encoder.FlushAsync();
        }

        private async void MenuFilePrint_Clicked(object sender, RoutedEventArgs e)
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

        private void MenuFileExit_Clicked(object sender, RoutedEventArgs e)
        {
            Application.Current.Exit();
        }

        private async void MenuFileOptions_Clicked(object sender, RoutedEventArgs e)
        {
            Options.Restore();
            await OptionsWindow.Show();
            ApplicationStatus.NumberOfLevels = Options.NumberOfColors;
            ApplicationStatus.ConversionMode = Options.ConversionTypeName + ".";
        }

        private async void MenuOperationConvert_Clicked(object sender, RoutedEventArgs e)
        {
            if (ApplicationStatus.ImageLoaded)
            {
                ApplicationStatus.ProgressValue = 0;
                ApplicationStatus.ProgressBarVisibility = Visibility.Visible;
                IAsyncActionWithProgress<double> asyncAction = null;
                if (Options.ConversionType == TypeOfConvertion.Grayscale)
                {
                    asyncAction = bitmap.ConvertToGrayscaleAsync(Options.NumberOfColors);
                }
                if (Options.ConversionType == TypeOfConvertion.ReducedColors)
                {
                    asyncAction = bitmap.ConvertToReducedColorsAsync(Options.NumberOfColors);
                }

                asyncAction.Progress = new AsyncActionProgressHandler<double>((action, progress) =>
                {
                    double i = progress;
                    ApplicationStatus.ProgressValue = ((float)progress);
                });
                await asyncAction;

                bitmap.ExtractLevels();
                this.Palette.Build(bitmap.Colors);

                ApplicationStatus.ImageConverted = true;
                ApplicationStatus.DisplayConverted = true;
                bitmap.Invalidate();
                ApplicationStatus.ProgressBarVisibility = Visibility.Collapsed;
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

        private async void MenuOperationClean_Clicked(object sender, RoutedEventArgs e)
        {
            var starttime = DateTime.Now;

            ApplicationStatus.ProgressValue = 0;
            ApplicationStatus.ProgressBarVisibility = Visibility.Visible;
            IAsyncActionWithProgress<double> asyncAction = null;
            asyncAction = bitmap.CleanUpImageAsync(Options.CleanupValue);
            asyncAction.Progress = new AsyncActionProgressHandler<double>((action, progress) =>
            {
                double i = progress;
                ApplicationStatus.ProgressValue = ((float)progress);
            });
            await asyncAction;

            bitmap.ExtractLevels();
            Palette.Build(bitmap.Colors);
            bitmap.Invalidate();
            ApplicationStatus.ProgressBarVisibility = Visibility.Collapsed;
            Options.TimeSpended = (DateTime.Now - starttime).TotalMilliseconds;
         }

        private async void MenuOperationOutline_Clicked(object sender, RoutedEventArgs e)
        {
            ApplicationStatus.DisplayContour = false;

            if (ApplicationStatus.ImageConverted)
            {
                ApplicationStatus.ProgressValue = 0;
                int progress = 0;
                ApplicationStatus.ProgressBarVisibility = Visibility.Visible;

                int numberOfLevels = bitmap.LevelsCount;

                ApplicationStatus.NumberOfLevels = numberOfLevels;

                List<Windows.Foundation.IAsyncOperation<int>> taskList = new List<Windows.Foundation.IAsyncOperation<int>>();

                var starttime = DateTime.Now;
                foreach (var color in Palette.ActiveColors)
                    taskList.Add(bitmap.FindLevelContoursAsync(color.Key));


                foreach (var task in taskList)
                {
                    await task;
                    ApplicationStatus.ProgressValue = progress / Palette.ActiveColors.Count;
                    ++progress;
                }


                //bitmap.OutlineImage();
                
                Options.TimeSpended = (DateTime.Now - starttime).TotalMilliseconds;

                ApplicationStatus.ProgressBarVisibility = Visibility.Collapsed;
                ApplicationStatus.ImageOutlined = true;
                ApplicationStatus.DisplayContour = true;
            }
        }

        private async void MenuHelpAbout_Clicked(object sender, RoutedEventArgs e)
        {
            await AboutWindow.Show();
        }

    } // End of MainPage class definition

}
