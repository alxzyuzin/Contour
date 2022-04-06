﻿using System;
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
using MUXC = Microsoft.UI.Xaml.Controls;
using System.Threading.Tasks;
using System.ComponentModel;


// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace Contour
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page, INotifyPropertyChanged
    {
        private GeneralOptions Options = new GeneralOptions();
        AppStatus ApplicationStatus = new AppStatus();

        private ContourBitmap bitmap = null;
        //public byte LevelsNumber { get; set; } = 2;


        private string _imageFileDisplayName;
        private string _imageFileNameExtention;
        private string _imageFilePath;
        private string _imageFileName = string.Empty;

        public event PropertyChangedEventHandler PropertyChanged;

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

        public MainPage()
        {
            InitializeComponent();
            OptionsWindow.DataContext = Options;
            DisplayOptions.DataContext = ApplicationStatus;

        }
        /// <summary>
        /// Load JPG or BMP image to bitmap object
        /// Convert color image to grayscale with defined levels of gray color
        /// Extract each gray color to separate layer in bitmap object
        /// </summary>
 

        private void BtnConvertToGrayScale_Tapped(object sender, TappedRoutedEventArgs e)
        {
//            string NumberOfLevels = ((ComboBoxItem)cbx_levels.SelectedValue).Content.ToString();
//            bitmap.ConvertToGrayscale(byte.Parse(NumberOfLevels));
//            bitmap.ExtractLevels();
//            BuildLayersWindow();
        }
        private void BtnExtractLevels_Tapped(object sender, TappedRoutedEventArgs e)
        {
            BuildLayersWindow();
        }

        
        /// <summary>
        /// Create window with toggle controls to switch on(off) grayscale levels and contours associated with these levels
        /// </summary>
        private void BuildLayersWindow()
        {
            //Button b = new Button();
            //// Clear Layers window from controls created for previous convertion color image to grayscale
            //int lcount = stp_Layers.Children.Count;
            //for (int i = 0; i < lcount; i++)
            //    stp_Layers.Children.RemoveAt(0);
            //// Create new control for each gray color in bitmap
            //foreach (byte color in bitmap.GrayScaleColorMap)
            //{
            //    LayerDisplayParams ldp =  new LayerDisplayParams(color);
            //    ldp.ContourSwitchToggled += OnContourSwitchToggled;
            //    ldp.ShapeSwitchToggled += OnShapeSwitchToggled;
            //    stp_Layers.Children.Add(ldp);
            //}
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private DisplayParams[] BuilDisplayParamsArray()
        {
            DisplayParams[] parameters = new DisplayParams[0];
            //DisplayParams[] parameters = new DisplayParams[stp_Layers.Children.Count];

            //for (int i = 0; i < stp_Layers.Children.Count; i++)
            //{
            //    DisplayParams displayParams = new DisplayParams();

            //    LayerDisplayParams userParams = (LayerDisplayParams)stp_Layers.Children[i];

            //    displayParams.Color = userParams.Color;
            //    displayParams.DisplayShapes = userParams.DisplayShapes;
            //    displayParams.DisplayContours = userParams.DisplayContours;

            //    parameters.SetValue(displayParams, i);
            //}
            return parameters;
        }

        private void BtnOutlineImage_Tapped(object sender, TappedRoutedEventArgs e)
        {
            if (bitmap == null) return;
            bitmap.ConvertToGrayscale(2);
            bitmap.ExtractLevels();
            bitmap.OutlineImage();

        }

        private void OnShapeSwitchToggled(object obj, RoutedEventArgs e)
        {
            DisplayParams[] parameters = BuilDisplayParamsArray();
            bitmap.DisplayOutlinedImage(parameters);
            bitmap.ImageData.Invalidate();
        }

        private void OnContourSwitchToggled(object obj, RoutedEventArgs e)
        {
            DisplayParams[] parameters = BuilDisplayParamsArray();
            bitmap.DisplayOutlinedImage(parameters);
            bitmap.ImageData.Invalidate();
        }

        private void BtnRestoreOriginalImage_Tapped(object sender, TappedRoutedEventArgs e)
        {
            bitmap.RestoreOriginalImage();
            bitmap.ImageData.Invalidate();
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
                
                ImageFileName = file.Name;
                _imageFilePath = file.Path.Replace(ImageFileName,"");
                _imageFileNameExtention = file.FileType;
                //_imageFileType
                ImageProperties props = await file.Properties.GetImagePropertiesAsync();

                bitmap = new ContourBitmap((int)props.Width, (int)props.Height);

                IRandomAccessStream stream = await file.OpenAsync(FileAccessMode.Read);
                bitmap.SetSource(stream);
                ExposedImage.Source = bitmap.ImageData;
            }
            else
            {
                //ImageFileName = "No loaded files.";
            }
        }

        private async void mfiSave_Clicked(object sender, RoutedEventArgs e)
        {
            if (bitmap == null) return;
            try
            {
                StorageFolder folder = await StorageFolder.GetFolderFromPathAsync(_imageFilePath);
                StorageFile file = await folder.CreateFileAsync(ImageFileName, CreationCollisionOption.ReplaceExisting);

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
            catch (Exception ex)
            {
                int i = 0;
            }
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
            try
            {
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
            catch(Exception ex)
            {
                int i = 0;
            }
        }

        private void mfiPrint_Clicked(object sender, RoutedEventArgs e)
        {

        }

        private void mfiExit_Clicked(object sender, RoutedEventArgs e)
        {
            Application.Current.Exit();
        }

        private async  void mfiOptions_Clicked(object sender, RoutedEventArgs e)
        {

            Options.Restore();
            await OptionsWindow.Show();
   
            
            int i = 0;
        }

        private void mfiConvert_Clicked(object sender, RoutedEventArgs e)
        {

        }

        private void mfiClean_Clicked(object sender, RoutedEventArgs e)
        {

        }
        private void mfiRectify_Clicked(object sender, RoutedEventArgs e)
        {

        }

        private void mfiOutline_Clicked(object sender, RoutedEventArgs e)
        {

        }

        private void mfiViewOriginal_Clicked(object sender, RoutedEventArgs e)
        {

        }

        private void mfiViewConverted_Clicked(object sender, RoutedEventArgs e)
        {

        }

        private void mfiViewCleaned_Clicked(object sender, RoutedEventArgs e)
        {

        }

        private void mfiViewContours_Clicked(object sender, RoutedEventArgs e)
        {

        }

        private void mfiAbout_Clicked(object sender, RoutedEventArgs e)
        {

        }

        private void StackPanel_ActualThemeChanged(FrameworkElement sender, object args)
        {

        }
    } // End of MainPage class definition




}
